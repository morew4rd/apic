#ifdef APIC_REFLECT

#include "apic_def.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define STRIP_STRUCT_PREFIX(type_str) \
    (strncmp(type_str, "struct ", 7) == 0) ? type_str + 7 : type_str

/* Forward declarations */
typedef struct apic_StructType apic_StructType;
typedef struct apic_UnionType apic_UnionType;
typedef struct apic_EnumType apic_EnumType;
typedef struct Type Type;

typedef enum PrimitiveType {
    PT_VOID,
    PT_CHAR, PT_UCHAR,
    PT_SHORT, PT_USHORT,
    PT_INT, PT_UINT,
    PT_LONG, PT_ULONG,
    PT_LONGLONG, PT_ULONGLONG,
    PT_FLOAT, PT_DOUBLE,
    PT_BOOL, PT_SIZE_T,
    PT_INTPTR, PT_UINTPTR,
    PT_CSTRING,
    PT_CUSTOM
} PrimitiveType;

typedef enum TypeQualifier {
    TQ_CONST = 1 << 0,
    TQ_VOLATILE = 1 << 1,
    TQ_RESTRICT = 1 << 2
} TypeQualifier;

typedef struct ArrayInfo {
    Type* element_type;
    size_t array_size;
} ArrayInfo;

typedef struct PointerInfo {
    Type* base_type;
} PointerInfo;

typedef struct apic_FunctionTypeInfo {
    Type** param_types;
    size_t num_params;
    Type* return_type;
} apic_FunctionTypeInfo;

typedef enum TypeKind {
    TK_PRIMITIVE,
    TK_POINTER,
    TK_ARRAY,
    TK_STRUCT,
    TK_UNION,
    TK_ENUM,
    TK_FUNCTION,
    TK_TYPEDEF
} TypeKind;

struct Type {
    const char* name;
    TypeKind kind;
    unsigned qualifiers;
    union {
        PrimitiveType primitive;
        PointerInfo pointer;
        ArrayInfo array;
        apic_StructType* struct_type;
        apic_UnionType* union_type;
        apic_EnumType* enum_type;
        apic_FunctionTypeInfo function;
        Type* typedef_target;
    } data;
};

typedef struct apic_TypedField {
    const char* name;
    Type* type;
    const char* doc;
} apic_TypedField;

struct apic_StructType {
    const char* name;
    apic_TypedField* fields;
    size_t num_fields;
    const char* doc;
};

struct apic_UnionType {
    const char* name;
    apic_TypedField* fields;
    size_t num_fields;
    const char* doc;
};

typedef struct apic_Typedapic_EnumEntry {
    const char* name;
    int value;
    const char* doc;
} apic_Typedapic_EnumEntry;

struct apic_EnumType {
    const char* name;
    apic_Typedapic_EnumEntry* entries;
    size_t num_entries;
    const char* doc;
};

typedef struct apic_TypedVar {
    const char* name;
    Type* type;
    const char* doc;
} apic_TypedVar;

typedef struct apic_TypedFunction {
    const char* name;
    Type* return_type;
    apic_TypedField* params;
    size_t num_params;
    const char* doc;
} apic_TypedFunction;

typedef struct apic_TypedExports {
    const char* name;
    const char* doc;

    apic_StructType** structs;
    size_t num_structs;

    apic_UnionType** unions;
    size_t num_unions;

    apic_EnumType** enums;
    size_t num_enums;

    apic_TypedFunction** functions;
    size_t num_functions;

    apic_TypedVar** variables;
    size_t num_variables;

    Type** aliases;
    size_t num_aliases;
} apic_TypedExports;


void apicext_typecheck(apic_Exports* exports);
void apicext_print_typed(apic_TypedExports* tex);

/* Helper function declarations */
static PrimitiveType get_primitive_type(const char* type_str);
static Type* create_type(const char* name, TypeKind kind);
static Type* resolve_type(apic_Exports* exports, const char* type_str);
static Type* resolve_typedef(apic_Exports* exports, const char* name);
static Type* resolve_struct_type(apic_Exports* exports, const char* name);
apic_TypedExports* apicext_create_typed_exports(apic_Exports* exports);

/* Implementation */
static PrimitiveType get_primitive_type(const char* type_str) {
    static const struct {
        const char* name;
        PrimitiveType pt;
    } type_map[] = {
        {"void", PT_VOID},
        {"char", PT_CHAR}, {"unsigned char", PT_UCHAR},
        {"short", PT_SHORT}, {"unsigned short", PT_USHORT},
        {"int", PT_INT}, {"unsigned int", PT_UINT},
        {"long", PT_LONG}, {"unsigned long", PT_ULONG},
        {"long long", PT_LONGLONG}, {"unsigned long long", PT_ULONGLONG},
        {"float", PT_FLOAT}, {"double", PT_DOUBLE},
        {"bool", PT_BOOL}, {"size_t", PT_SIZE_T},
        {"intptr_t", PT_INTPTR}, {"uintptr_t", PT_UINTPTR},
        {"const char*", PT_CSTRING}, {"char*", PT_CSTRING}
    };

    for (size_t i = 0; i < sizeof(type_map)/sizeof(type_map[0]); i++) {
        if (strcmp(type_str, type_map[i].name) == 0) {
            return type_map[i].pt;
        }
    }
    return PT_CUSTOM;
}

static Type* create_type(const char* name, TypeKind kind) {
    Type* t = calloc(1, sizeof(Type));
    if (!t) return NULL;
    t->name = name;
    t->kind = kind;
    return t;
}

static Type* resolve_struct_type(apic_Exports* exports, const char* name) {
    /* Strip 'struct ' prefix if present */
    const char* stripped_name = STRIP_STRUCT_PREFIX(name);

    for (int i = 0; i < exports->struct_count; i++) {
        if (strcmp(exports->structs[i]->name, stripped_name) == 0) {
            Type* t = create_type(stripped_name, TK_STRUCT);
            if (!t) return NULL;
            t->data.struct_type = calloc(1, sizeof(apic_StructType));
            if (!t->data.struct_type) {
                free(t);
                return NULL;
            }
            t->data.struct_type->name = stripped_name;
            return t;
        }
    }
    return NULL;
}

static Type* resolve_typedef(apic_Exports* exports, const char* name) {
    /* Strip 'struct ' prefix for typedef resolution */
    const char* stripped_name = STRIP_STRUCT_PREFIX(name);

    for (int i = 0; i < exports->typedef_count; i++) {
        if (strcmp(exports->aliases[i]->name, stripped_name) == 0) {
            return resolve_type(exports, exports->aliases[i]->type);
        }
    }
    return NULL;
}

static Type* resolve_type(apic_Exports* exports, const char* type_str) {
    const char* orig_str = type_str;
    unsigned qualifiers = 0;
    Type* t = NULL;

    /* Parse qualifiers */
    while (1) {
        if (strncmp(type_str, "const ", 6) == 0) {
            qualifiers |= TQ_CONST;
            type_str += 6;
        }
        else if (strncmp(type_str, "volatile ", 9) == 0) {
            qualifiers |= TQ_VOLATILE;
            type_str += 9;
        }
        else if (strncmp(type_str, "restrict ", 9) == 0) {
            qualifiers |= TQ_RESTRICT;
            type_str += 9;
        }
        else {
            break;
        }
    }

    /* Check aliases */
    Type* tdef;
    if ((tdef = resolve_typedef(exports, type_str))) {
        tdef->qualifiers = qualifiers;
        return tdef;
    }

    /* Handle pointers */
    char* asterisk = strchr(type_str, '*');
    if (asterisk) {
        char* base_str = strdup(type_str);
        asterisk = strchr(base_str, '*');
        *asterisk = '\0';

        /* Recursive resolution with struct prefix handling */
        Type* base_type = resolve_type(exports, base_str);
        free(base_str);

        if (base_type) {
            t = create_type(orig_str, TK_POINTER);
            if (t) {
                t->qualifiers = qualifiers;
                t->data.pointer.base_type = base_type;
            }
        }
        return t;
    }

    /* Handle arrays */
    const char* bracket = strchr(type_str, '[');
    if (bracket) {
        char* base_str = strdup(type_str);
        char* arr_pos = strchr(base_str, '[');
        *arr_pos = '\0';

        Type* element_type = resolve_type(exports, base_str);
        size_t array_size = 0;
        if (isdigit(bracket[1])) {
            array_size = atoi(bracket + 1);
        }

        free(base_str);

        if (element_type) {
            t = create_type(orig_str, TK_ARRAY);
            if (t) {
                t->qualifiers = qualifiers;
                t->data.array.element_type = element_type;
                t->data.array.array_size = array_size;
            }
        }
        return t;
    }

    /* Check primitive types */
    PrimitiveType pt = get_primitive_type(type_str);
    if (pt != PT_CUSTOM) {
        t = create_type(type_str, TK_PRIMITIVE);
        if (t) {
            t->qualifiers = qualifiers;
            t->data.primitive = pt;
        }
        return t;
    }

    /* Check structs */
    Type* st;
    if ((st = resolve_struct_type(exports, type_str))) {
        st->qualifiers = qualifiers;
        return st;
    }

    /* Fallback to custom type */
    t = create_type(type_str, TK_PRIMITIVE);
    if (t) {
        t->qualifiers = qualifiers;
        t->data.primitive = PT_CUSTOM;
    }
    return t;
}

apic_TypedExports* apicext_create_typed_exports(apic_Exports* exports) {
    if (!exports) return NULL;

    apic_TypedExports* tex = calloc(1, sizeof(apic_TypedExports));
    if (!tex) return NULL;

    tex->name = exports->name;
    tex->doc = exports->doc;

    /* Convert structs */
    tex->num_structs = exports->struct_count;
    if (tex->num_structs > 0) {
        tex->structs = calloc(tex->num_structs, sizeof(apic_StructType*));
        if (!tex->structs) goto cleanup;

        for (int i = 0; i < exports->struct_count; i++) {
            apic_Struct* st = exports->structs[i];
            apic_StructType* stype = calloc(1, sizeof(apic_StructType));
            if (!stype) goto cleanup;

            stype->name = st->name;
            stype->doc = st->doc;
            stype->num_fields = st->count;

            if (st->count > 0) {
                stype->fields = calloc(st->count, sizeof(apic_TypedField));
                if (!stype->fields) {
                    free(stype);
                    goto cleanup;
                }

                for (int j = 0; j < st->count; j++) {
                    apic_Field f = st->fields[j];
                    stype->fields[j].name = f.name;
                    stype->fields[j].doc = f.doc;
                    stype->fields[j].type = resolve_type(exports, f.type);
                }
            }
            tex->structs[i] = stype;
        }
    }

    /* Convert aliases */
    tex->num_aliases = exports->typedef_count;
    if (tex->num_aliases > 0) {
        tex->aliases = calloc(tex->num_aliases, sizeof(Type*));
        if (!tex->aliases) goto cleanup;

        for (int i = 0; i < exports->typedef_count; i++) {
            apic_Alias* td = exports->aliases[i];
            Type* t = create_type(td->name, TK_TYPEDEF);
            if (!t) goto cleanup;

            t->data.typedef_target = resolve_type(exports, td->type);
            tex->aliases[i] = t;
        }
    }

    /* Convert functions */
    tex->num_functions = exports->func_count;
    if (tex->num_functions > 0) {
        tex->functions = calloc(tex->num_functions, sizeof(apic_TypedFunction*));
        if (!tex->functions) goto cleanup;

        for (int i = 0; i < exports->func_count; i++) {
            apic_Func* fn = exports->funcs[i];
            apic_TypedFunction* tfn = calloc(1, sizeof(apic_TypedFunction));
            if (!tfn) goto cleanup;

            tfn->name = fn->name;
            tfn->doc = fn->doc;
            tfn->return_type = resolve_type(exports, fn->ret);
            tfn->num_params = fn->count;

            if (fn->count > 0) {
                tfn->params = calloc(fn->count, sizeof(apic_TypedField));
                if (!tfn->params) {
                    free(tfn);
                    goto cleanup;
                }

                for (int j = 0; j < fn->count; j++) {
                    apic_Arg a = fn->args[j];
                    tfn->params[j].name = a.name;
                    tfn->params[j].doc = a.doc;
                    tfn->params[j].type = resolve_type(exports, a.type);
                }
            }
            tex->functions[i] = tfn;
        }
    }

    return tex;

cleanup:
    /* Add proper cleanup code here */
    return NULL;
}



static const char* type_qualifier_str(unsigned qualifiers) {
    static char buf[64];
    buf[0] = '\0';

    if (qualifiers & TQ_CONST) strcat(buf, "const ");
    if (qualifiers & TQ_VOLATILE) strcat(buf, "volatile ");
    if (qualifiers & TQ_RESTRICT) strcat(buf, "restrict ");

    if (buf[0] != '\0') buf[strlen(buf)-1] = '\0'; // Remove trailing space
    return buf;
}


static void type_to_str(Type* type, char* buf, size_t size, int indent) {
    if (!type || !buf) return;

    char indent_str[32] = {0};
    for (int i = 0; i < indent; i++) strcat(indent_str, "  ");

    const char* quals = type_qualifier_str(type->qualifiers);
    if (quals[0] != '\0') snprintf(buf, size, "%s%s ", indent_str, quals);
    else buf[0] = '\0';

    switch (type->kind) {
        case TK_PRIMITIVE: {
            const char* prim_names[] = {
                "void", "char", "unsigned char", "short", "unsigned short",
                "int", "unsigned int", "long", "unsigned long", "long long",
                "unsigned long long", "float", "double", "bool", "size_t",
                "intptr_t", "uintptr_t", "const char*", "char*", "custom"
            };
            strncat(buf, prim_names[type->data.primitive], size - strlen(buf));
            break;
        }
        case TK_POINTER: {
            char inner[256];
            type_to_str(type->data.pointer.base_type, inner, sizeof(inner), indent);
            snprintf(buf + strlen(buf), size - strlen(buf), "%s*", inner);
            break;
        }
        case TK_ARRAY: {
            char inner[256];
            type_to_str(type->data.array.element_type, inner, sizeof(inner), indent);
            snprintf(buf + strlen(buf), size - strlen(buf), "%s[%zu]",
                    inner, type->data.array.array_size);
            break;
        }
        case TK_STRUCT: {
            apic_StructType* st = type->data.struct_type;
            snprintf(buf + strlen(buf), size - strlen(buf), "struct %s", st->name);
            break;
        }
        case TK_UNION: {
            apic_UnionType* un = type->data.union_type;
            snprintf(buf + strlen(buf), size - strlen(buf), "union %s", un->name);
            break;
        }
        case TK_ENUM: {
            apic_EnumType* en = type->data.enum_type;
            snprintf(buf + strlen(buf), size - strlen(buf), "enum %s", en->name);
            break;
        }
        case TK_TYPEDEF: {
            char inner[256];
            type_to_str(type->data.typedef_target, inner, sizeof(inner), indent);
            snprintf(buf + strlen(buf), size - strlen(buf), "typedef %s", inner);
            break;
        }
        case TK_FUNCTION: {
            apic_FunctionTypeInfo* fi = &type->data.function;
            snprintf(buf + strlen(buf), size - strlen(buf), "(");
            for (size_t i = 0; i < fi->num_params; i++) {
                char param[256];
                type_to_str(fi->param_types[i], param, sizeof(param), indent);
                strncat(buf, param, size - strlen(buf));
                if (i < fi->num_params - 1) strncat(buf, ", ", size - strlen(buf));
            }
            strncat(buf, ")", size - strlen(buf));
            break;
        }
        default:
            strncat(buf, "unknown", size - strlen(buf));
    }
}

static void print_struct(apic_StructType* st, int indent) {
    char indent_str[32] = {0};
    for (int i = 0; i < indent; i++) strcat(indent_str, "  ");

    printf("%sstruct %s {\n", indent_str, st->name);
    if (st->doc) printf("%s  // %s\n", indent_str, st->doc);

    for (size_t i = 0; i < st->num_fields; i++) {
        apic_TypedField* f = &st->fields[i];
        char type_buf[256];
        type_to_str(f->type, type_buf, sizeof(type_buf), indent + 1);
        printf("%s  %-20s %s", indent_str, type_buf, f->name);
        if (f->doc) printf("  // %s", f->doc);
        printf("\n");
    }
    printf("%s}\n\n", indent_str);
}

static void print_enum(apic_EnumType* en, int indent) {
    char indent_str[32] = {0};
    for (int i = 0; i < indent; i++) strcat(indent_str, "  ");

    printf("%senum %s {\n", indent_str, en->name);
    if (en->doc) printf("%s  // %s\n", indent_str, en->doc);

    for (size_t i = 0; i < en->num_entries; i++) {
        apic_Typedapic_EnumEntry* e = &en->entries[i];
        printf("%s  %s = %d", indent_str, e->name, e->value);
        if (e->doc) printf("  // %s", e->doc);
        printf("\n");
    }
    printf("%s}\n\n", indent_str);
}

void apicext_print_typed(apic_TypedExports* tex) {
    if (!tex) return;

    printf("\n========== Typed API: %s ==========\n", tex->name);
    if (tex->doc) printf("// %s\n", tex->doc);

    /* Print aliases */
    if (tex->num_aliases > 0) {
        printf("\napic_Aliass (%zu):\n", tex->num_aliases);
        for (size_t i = 0; i < tex->num_aliases; i++) {
            Type* t = tex->aliases[i];
            char type_buf[256];
            type_to_str(t->data.typedef_target, type_buf, sizeof(type_buf), 0);
            printf("  typedef %-20s → %s\n", type_buf, t->name);
        }
    }

    /* Print structs */
    if (tex->num_structs > 0) {
        printf("\napic_Structures (%zu):\n", tex->num_structs);
        for (size_t i = 0; i < tex->num_structs; i++) {
            print_struct(tex->structs[i], 0);
        }
    }

    /* Print enums */
    if (tex->num_enums > 0) {
        printf("\napic_Enums (%zu):\n", tex->num_enums);
        for (size_t i = 0; i < tex->num_enums; i++) {
            print_enum(tex->enums[i], 0);
        }
    }

    /* Print functions */
    if (tex->num_functions > 0) {
        printf("\napic_Functions (%zu):\n", tex->num_functions);
        for (size_t i = 0; i < tex->num_functions; i++) {
            apic_TypedFunction* fn = tex->functions[i];
            char ret_buf[256];
            type_to_str(fn->return_type, ret_buf, sizeof(ret_buf), 0);

            printf("%s %s(", ret_buf, fn->name);
            for (size_t j = 0; j < fn->num_params; j++) {
                char param_buf[256];
                type_to_str(fn->params[j].type, param_buf, sizeof(param_buf), 0);
                printf("%s %s", param_buf, fn->params[j].name);
                if (j < fn->num_params - 1) printf(", ");
            }
            printf(")");
            if (fn->doc) printf("  // %s", fn->doc);
            printf("\n");
        }
    }

    /* Print variables */
    if (tex->num_variables > 0) {
        printf("\napic_Variables (%zu):\n", tex->num_variables);
        for (size_t i = 0; i < tex->num_variables; i++) {
            apic_TypedVar* var = tex->variables[i];
            char type_buf[256];
            type_to_str(var->type, type_buf, sizeof(type_buf), 0);
            printf("  %-20s %s", type_buf, var->name);
            if (var->doc) printf("  // %s", var->doc);
            printf("\n");
        }
    }

    printf("========================================\n\n");
}

#endif /* APIC_REFLECT */
