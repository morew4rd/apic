#ifdef APIC_REFLECT

#include "apic.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>

typedef struct {
    int error_count;
} TypeCheckContext;

static const char* primitive_types[] = {
    "void", "char", "unsigned char", "signed char",
    "short", "unsigned short", "int", "unsigned int",
    "long", "unsigned long", "long long", "unsigned long long",
    "float", "double", "size_t", "ssize_t",
    "int8_t", "uint8_t", "int16_t", "uint16_t",
    "int32_t", "uint32_t", "int64_t", "uint64_t",
    "bool", "FILE", NULL
};

static void report_error(TypeCheckContext* ctx, const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf("Type Error: ");
    vprintf(format, args);
    va_end(args);
    printf("\n");
    ctx->error_count++;
}

static const char* unqualified_type(const char* type) {
    const char* qualifiers[] = {"const ", "volatile ", "restrict "};
    for (size_t i = 0; i < sizeof(qualifiers)/sizeof(qualifiers[0]); i++) {
        size_t qlen = strlen(qualifiers[i]);
        if (strncmp(type, qualifiers[i], qlen) == 0) {
            return type + qlen;
        }
    }
    return type;
}

static int is_primitive(const char* type) {
    const char* base = unqualified_type(type);
    for (const char** t = primitive_types; *t; t++) {
        if (strcmp(base, *t) == 0) return 1;
    }
    return 0;
}

static int find_struct(apic_Exports* exports, const char* name) {
    for (int i = 0; i < exports->struct_count; i++) {
        if (strcmp(exports->structs[i]->name, name) == 0) return 1;
    }
    return 0;
}

static int find_union(apic_Exports* exports, const char* name) {
    for (int i = 0; i < exports->union_count; i++) {
        if (strcmp(exports->unions[i]->name, name) == 0) return 1;
    }
    return 0;
}

static int find_enum(apic_Exports* exports, const char* name) {
    for (int i = 0; i < exports->enum_count; i++) {
        if (strcmp(exports->enums[i]->name, name) == 0) return 1;
    }
    return 0;
}

static int find_typedef(apic_Exports* exports, const char* name) {
    for (int i = 0; i < exports->typedef_count; i++) {
        if (strcmp(exports->typedefs[i]->name, name) == 0) return 1;
    }
    return 0;
}

static int find_lambda(apic_Exports* exports, const char* name) {
    for (int i = 0; i < exports->lambda_count; i++) {
        if (strcmp(exports->lambdas[i]->name, name) == 0) return 1;
    }
    return 0;
}

static int check_type(apic_Exports* exports, TypeCheckContext* ctx, const char* type);

static int check_pointer_type(apic_Exports* exports, TypeCheckContext* ctx, const char* type) {
    const char* ptr = strrchr(type, '*');
    if (!ptr) return 0;

    char base[256];
    size_t len = ptr - type;
    strncpy(base, type, len);
    base[len] = '\0';

    char* end = base + strlen(base) - 1;
    while (end > base && isspace(*end)) end--;
    *(end + 1) = '\0';

    return check_type(exports, ctx, base);
}

static int check_array_type(apic_Exports* exports, TypeCheckContext* ctx, const char* type) {
    const char* bracket = strchr(type, '[');
    if (!bracket) return 0;

    char base[256];
    strncpy(base, type, bracket - type);
    base[bracket - type] = '\0';
    return check_type(exports, ctx, base);
}

static int check_type(apic_Exports* exports, TypeCheckContext* ctx, const char* type) {
    // Handle pointers
    if (strchr(type, '*')) {
        return check_pointer_type(exports, ctx, type);
    }

    // Handle struct/union/enum qualifiers
    if (strncmp(type, "struct ", 7) == 0) {
        return find_struct(exports, type + 7);
    }
    if (strncmp(type, "union ", 6) == 0) {
        return find_union(exports, type + 6);
    }
    if (strncmp(type, "enum ", 5) == 0) {
        return find_enum(exports, type + 5);
    }

    // Handle arrays
    if (strchr(type, '[')) {
        return check_array_type(exports, ctx, type);
    }

    const char* base = unqualified_type(type);

    return is_primitive(base) ||
           find_struct(exports, base) ||
           find_union(exports, base) ||
           find_enum(exports, base) ||
           find_typedef(exports, base) ||
           find_lambda(exports, base);
}

static void check_field(apic_Exports* exports, TypeCheckContext* ctx, const char* context, apic_Field f) {
    if (!check_type(exports, ctx, f.type)) {
        report_error(ctx, "Invalid type '%s' for field '%s' in %s",
                    f.type, f.name, context);
    }
}

static void check_arg(apic_Exports* exports, TypeCheckContext* ctx, const char* context, apic_Arg a) {
    if (!check_type(exports, ctx, a.type)) {
        report_error(ctx, "Invalid type '%s' for argument '%s' in %s",
                    a.type, a.name, context);
    }
}

void apic_ext_typecheck(apic_Exports* exports) {
    TypeCheckContext ctx = {0};

    // Validate struct fields
    for (int i = 0; i < exports->struct_count; i++) {
        apic_Struct* st = exports->structs[i];
        char context[128];
        snprintf(context, sizeof(context), "struct %s", st->name);
        for (int j = 0; j < st->count; j++) {
            check_field(exports, &ctx, context, st->fields[j]);
        }
    }

    // Validate union fields
    for (int i = 0; i < exports->union_count; i++) {
        apic_Union* u = exports->unions[i];
        char context[128];
        snprintf(context, sizeof(context), "union %s", u->name);
        for (int j = 0; j < u->count; j++) {
            check_field(exports, &ctx, context, u->fields[j]);
        }
    }

    // Validate functions
    for (int i = 0; i < exports->func_count; i++) {
        apic_Func* func = exports->funcs[i];
        if (!check_type(exports, &ctx, func->ret)) {
            report_error(&ctx, "Invalid return type '%s' for function %s",
                        func->ret, func->name);
        }
        char context[128];
        snprintf(context, sizeof(context), "function %s", func->name);
        for (int j = 0; j < func->count; j++) {
            check_arg(exports, &ctx, context, func->args[j]);
        }
    }

    // Validate lambdas
    for (int i = 0; i < exports->lambda_count; i++) {
        apic_Lambda* lam = exports->lambdas[i];
        if (!check_type(exports, &ctx, lam->ret)) {
            report_error(&ctx, "Invalid return type '%s' for lambda %s",
                        lam->ret, lam->name);
        }
        char context[128];
        snprintf(context, sizeof(context), "lambda %s", lam->name);
        for (int j = 0; j < lam->count; j++) {
            check_arg(exports, &ctx, context, lam->args[j]);
        }
    }

    // Validate variables
    for (int i = 0; i < exports->var_count; i++) {
        apic_Var* v = exports->vars[i];
        if (!check_type(exports, &ctx, v->type)) {
            report_error(&ctx, "Invalid type '%s' for variable %s",
                        v->type, v->name);
        }
    }

    // Validate typedefs
    for (int i = 0; i < exports->typedef_count; i++) {
        apic_Typedef* t = exports->typedefs[i];
        if (!check_type(exports, &ctx, t->type)) {
            report_error(&ctx, "Invalid underlying type '%s' for typedef %s",
                        t->type, t->name);
        }
    }

    if (ctx.error_count > 0) {
        printf("\nFound %d type errors!\n", ctx.error_count);
    } else {
        printf("All type checks passed successfully!\n");
    }
}

#endif // APIC_REFLECT
