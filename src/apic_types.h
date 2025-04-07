// ================ File: src/apic_types.h ================
#ifndef APIC_TYPES_H
#define APIC_TYPES_H

#ifdef APIC_REFLECT

#include "apic_def.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h> // For error reporting
#include <stddef.h> // For size_t

// Error Reporting Context (Moved from typecheck.h)
typedef struct {
    int error_count;
    apic_Exports* current_exports; // Keep context for lookups
} TypeResolutionContext;

static void report_resolution_error(TypeResolutionContext* ctx, const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf("Type Resolution Error: ");
    vprintf(format, args);
    va_end(args);
    printf("\n");
    if (ctx) {
        ctx->error_count++;
    }
}

// Type Name Helpers (Moved and adapted from typecheck.h)
static const char* strip_prefixes(const char* type) {
    const char* prefixes[] = {"const ", "volatile ", "restrict ", "struct ", "union ", "enum "};
    int changed;
    do {
        changed = 0;
        for (size_t i = 0; i < sizeof(prefixes)/sizeof(prefixes[0]); i++) {
            size_t qlen = strlen(prefixes[i]);
            if (strncmp(type, prefixes[i], qlen) == 0) {
                type += qlen;
                changed = 1;
            }
        }
    } while(changed);

    // Trim trailing whitespace
    char* type_copy = strdup(type);
    if (!type_copy) return type; // Allocation failed, return original
    char* end = type_copy + strlen(type_copy) - 1;
    while (end >= type_copy && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
    // Note: This leaks memory (strdup). This needs a more robust solution
    // in production, perhaps passing buffers or using a dedicated arena.
    // For now, accept the leak for demonstration. A better way is needed.
    // A temporary fix: return the original pointer if no trimming happened.
    if (strlen(type_copy) == strlen(type)) {
        free(type_copy);
        return type;
    }
    // Or, always return the copy and manage its lifetime
     // For now, returning the potentially modified string (leaked).
     // Let's revert to a simpler, non-modifying approach for now.
    free(type_copy); // Free the copy regardless

    // Simpler approach: Find the first non-qualifier word
    while (1) {
         if (strncmp(type, "const ", 6) == 0) type += 6;
         else if (strncmp(type, "volatile ", 9) == 0) type += 9;
         else if (strncmp(type, "restrict ", 9) == 0) type += 9;
         else break;
    }
    // Strip struct/union/enum keywords for lookup
    if (strncmp(type, "struct ", 7) == 0) type += 7;
    else if (strncmp(type, "union ", 6) == 0) type += 6;
    else if (strncmp(type, "enum ", 5) == 0) type += 5;

    // Trim leading/trailing spaces from the result (read-only)
    while (isspace((unsigned char)*type)) type++;
    // Trailing space removal requires modification or copy. Skip for now.
    return type;
}


// static int is_primitive(const char* type) {
//     // Basic check first
//     for (const char** t = primitive_types; *t; t++) {
//         if (strcmp(type, *t) == 0) return 1;
//     }
//     // Check without const/volatile etc.
//     const char* base = strip_prefixes(type);
//      for (const char** t = primitive_types; *t; t++) {
//         if (strcmp(base, *t) == 0) return 1;
//     }
//     return 0;
// }

// static int find_struct(apic_Exports* exports, const char* name) {
//     const char* base_name = strip_prefixes(name);
//     for (int i = 0; i < exports->struct_count; i++) {
//         if (strcmp(exports->structs[i]->name, base_name) == 0) return 1;
//     }
//     return 0;
// }

// static int find_union(apic_Exports* exports, const char* name) {
//     const char* base_name = strip_prefixes(name);
//     for (int i = 0; i < exports->union_count; i++) {
//         if (strcmp(exports->unions[i]->name, base_name) == 0) return 1;
//     }
//     return 0;
// }

// static int find_enum(apic_Exports* exports, const char* name) {
//     const char* base_name = strip_prefixes(name);
//     for (int i = 0; i < exports->enum_count; i++) {
//         if (strcmp(exports->enums[i]->name, base_name) == 0) return 1;
//     }
//     return 0;
// }

static apic_Alias* find_typedef_def(apic_Exports* exports, const char* name) {
    const char* base_name = strip_prefixes(name);
    for (int i = 0; i < exports->typedef_count; i++) {
        if (strcmp(exports->aliases[i]->name, base_name) == 0) return exports->aliases[i];
    }
    return NULL;
}

// static int find_typedef(apic_Exports* exports, const char* name) {
//     return find_typedef_def(exports, name) != NULL;
// }


// static int find_funcptr(apic_Exports* exports, const char* name) {
//      const char* base_name = strip_prefixes(name);
//     for (int i = 0; i < exports->funcptr_count; i++) {
//         if (strcmp(exports->funcptrs[i]->name, base_name) == 0) return 1;
//     }
//     return 0;
// }

// End of moved functions from typecheck.h


#define STRIP_STRUCT_PREFIX(type_str) \
    (strncmp(type_str, "struct ", 7) == 0) ? type_str + 7 : type_str
#define STRIP_UNION_PREFIX(type_str) \
    (strncmp(type_str, "union ", 6) == 0) ? type_str + 6 : type_str
#define STRIP_ENUM_PREFIX(type_str) \
    (strncmp(type_str, "enum ", 5) == 0) ? type_str + 5 : type_str


/* Forward declarations */
typedef struct apic_StructType apic_StructType;
typedef struct apic_UnionType apic_UnionType;
typedef struct apic_EnumType apic_EnumType;
typedef struct Type Type; // Renamed from internal Type to avoid conflict if user defines Type
typedef struct apic_FunctionTypeInfo apic_FunctionTypeInfo;


typedef enum PrimitiveType {
    PT_VOID,
    PT_CHAR, PT_UCHAR, PT_SCHAR, // Added signed char
    PT_SHORT, PT_USHORT,
    PT_INT, PT_UINT,
    PT_LONG, PT_ULONG,
    PT_LONGLONG, PT_ULONGLONG,
    PT_FLOAT, PT_DOUBLE,
    PT_BOOL, PT_SIZE_T, // ssize_t ?
    PT_INT8, PT_UINT8, PT_INT16, PT_UINT16, PT_INT32, PT_UINT32, PT_INT64, PT_UINT64,
    PT_INTPTR, PT_UINTPTR,
    PT_FILE, // Added FILE* ? Needs thought. Let's keep FILE for now.
    PT_CSTRING, // Represents char* or const char* conceptually
    PT_CUSTOM // Placeholder for unrecognized primitives or user types not reflected
} PrimitiveType;

typedef enum TypeQualifier {
    TQ_NONE = 0,
    TQ_CONST = 1 << 0,
    TQ_VOLATILE = 1 << 1,
    TQ_RESTRICT = 1 << 2
} TypeQualifier;

typedef struct ArrayInfo {
    Type* element_type;
    size_t array_size; // 0 for unknown size T[]
} ArrayInfo;

typedef struct PointerInfo {
    Type* base_type;
} PointerInfo;

struct apic_FunctionTypeInfo {
    const char* name; // Name of the funcptr typedef, if applicable
    Type** param_types;
    size_t num_params;
    Type* return_type;
};

typedef enum TypeKind {
    TK_UNRESOLVED, // Added state for during resolution
    TK_PRIMITIVE,
    TK_POINTER,
    TK_ARRAY,
    TK_STRUCT,
    TK_UNION,
    TK_ENUM,
    TK_FUNCTION, // Represents the signature, used by funcptr
    TK_FUNCPTR,  // Represents a named function pointer type (typedef)
    TK_TYPEDEF   // Represents a named alias to another type
} TypeKind;

struct Type {
    const char* name;       // Original name string (or derived name)
    TypeKind kind;
    unsigned qualifiers;    // Bitmask of TypeQualifier
    int is_forward_decl;    // Flag for forward declared structs/unions
    union {
        PrimitiveType primitive;
        PointerInfo pointer;
        ArrayInfo array;
        apic_StructType* struct_type; // Reference to definition
        apic_UnionType* union_type;   // Reference to definition
        apic_EnumType* enum_type;     // Reference to definition
        apic_FunctionTypeInfo function; // For function signatures
        Type* typedef_target;         // The type the alias points to
        // TK_FUNCPTR uses 'function' field and kind = TK_FUNCPTR
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
    Type* self_type; // Point back to the Type representing this struct
};

struct apic_UnionType {
    const char* name;
    apic_TypedField* fields;
    size_t num_fields;
    const char* doc;
    Type* self_type; // Point back to the Type representing this union
};

typedef struct apic_TypedEnumEntry { // Renamed to avoid apic_apic
    const char* name;
    int value;
    const char* str_value; // string representation if provided
    const char* doc;
} apic_TypedEnumEntry;

struct apic_EnumType {
    const char* name;
    apic_TypedEnumEntry* entries;
    size_t num_entries;
    const char* doc;
    Type* self_type; // Point back to the Type representing this enum
};

typedef struct apic_TypedVar {
    const char* name;
    Type* type;
    const char* doc;
} apic_TypedVar;

typedef struct apic_TypedFunction {
    const char* name;
    Type* return_type;
    apic_TypedField* params; // Note: using TypedField for params {name, type, doc}
    size_t num_params;
    const char* doc;
} apic_TypedFunction;


// Cache for resolved types to avoid redundant work and cycles
#define TYPE_CACHE_SIZE 256
typedef struct TypeCache {
    struct TypeCacheEntry {
        const char* name;
        Type* type;
    } entries[TYPE_CACHE_SIZE];
    size_t count;
} TypeCache;

static void init_type_cache(TypeCache* cache) {
    cache->count = 0;
    memset(cache->entries, 0, sizeof(cache->entries));
}

static Type* find_in_cache(TypeCache* cache, const char* name) {
    for (size_t i = 0; i < cache->count; ++i) {
        if (strcmp(cache->entries[i].name, name) == 0) {
            return cache->entries[i].type;
        }
    }
    return NULL;
}

static void add_to_cache(TypeCache* cache, const char* name, Type* type) {
    if (cache->count < TYPE_CACHE_SIZE) {
        // Check if already exists (shouldn't happen if find_in_cache is used properly)
        if (find_in_cache(cache, name)) return;
        cache->entries[cache->count].name = name; // Store original name string
        cache->entries[cache->count].type = type;
        cache->count++;
    } else {
        // Cache full - consider error or eviction strategy if needed
        fprintf(stderr, "Warning: Type cache full. Cannot cache '%s'.\n", name);
    }
}


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

    Type** funcptrs; // Store resolved FuncPtr Types
    size_t num_funcptrs;

    apic_TypedVar** variables;
    size_t num_variables;

    Type** aliases; // Store resolved Typedef Types
    size_t num_aliases;

    // Internal state during creation
    TypeResolutionContext context;
    TypeCache type_cache;

} apic_TypedExports;


// // void apicext_typecheck(apic_Exports* exports); // REMOVED
// void apicext_print_typed(apic_TypedExports* tex);

/* Helper function declarations */
static PrimitiveType get_primitive_type_enum(const char* type_str);
static Type* create_type(const char* name, TypeKind kind);
static Type* resolve_type_recursive(apic_TypedExports* tex, apic_Exports* exports, const char* type_str);
apic_TypedExports* apicext_create_typed_exports(apic_Exports* exports);
static void cleanup_typed_exports(apic_TypedExports* tex); // Added cleanup


/* Implementation */

static PrimitiveType get_primitive_type_enum(const char* type_str) {
    // This map should be comprehensive and match the primitive_types array logic
    static const struct { const char* name; PrimitiveType pt; } type_map[] = {
        {"void", PT_VOID},
        {"char", PT_CHAR}, {"unsigned char", PT_UCHAR}, {"signed char", PT_SCHAR},
        {"short", PT_SHORT}, {"unsigned short", PT_USHORT},
        {"int", PT_INT}, {"unsigned int", PT_UINT},
        {"long", PT_LONG}, {"unsigned long", PT_ULONG},
        {"long long", PT_LONGLONG}, {"unsigned long long", PT_ULONGLONG},
        {"float", PT_FLOAT}, {"double", PT_DOUBLE},
        {"bool", PT_BOOL}, {"_Bool", PT_BOOL}, // Allow _Bool
        {"size_t", PT_SIZE_T},
        {"int8_t", PT_INT8}, {"uint8_t", PT_UINT8},
        {"int16_t", PT_INT16}, {"uint16_t", PT_UINT16},
        {"int32_t", PT_INT32}, {"uint32_t", PT_UINT32},
        {"int64_t", PT_INT64}, {"uint64_t", PT_UINT64},
        {"intptr_t", PT_INTPTR}, {"uintptr_t", PT_UINTPTR},
        {"FILE", PT_FILE},
        {"char*", PT_CSTRING}, {"const char*", PT_CSTRING} // Map both to conceptual CString
    };

    for (size_t i = 0; i < sizeof(type_map)/sizeof(type_map[0]); i++) {
        if (strcmp(type_str, type_map[i].name) == 0) {
            return type_map[i].pt;
        }
    }

    // Check base name as well after stripping qualifiers
    const char* base_name = strip_prefixes(type_str);
     if (base_name != type_str) { // Avoid infinite recursion if strip didn't change
         for (size_t i = 0; i < sizeof(type_map)/sizeof(type_map[0]); i++) {
             if (strcmp(base_name, type_map[i].name) == 0) {
                 return type_map[i].pt;
             }
         }
     }


    return PT_CUSTOM; // Not a recognized primitive
}

static Type* create_type(const char* name, TypeKind kind) {
    Type* t = calloc(1, sizeof(Type));
    if (!t) {
        perror("Failed to allocate Type");
        return NULL;
    }
    t->name = name ? strdup(name) : NULL; // Duplicate the name string
    if (name && !t->name) {
        perror("Failed to duplicate type name");
        free(t);
        return NULL;
    }
    t->kind = kind;
    t->qualifiers = TQ_NONE;
    t->is_forward_decl = 0;
    return t;
}

// Main type resolution function - integrates validation
static Type* resolve_type_recursive(apic_TypedExports* tex, apic_Exports* exports, const char* type_str) {
    if (!type_str || type_str[0] == '\0') {
        report_resolution_error(&tex->context, "Empty type string encountered");
        return NULL;
    }

    // 0. Check cache first
    Type* cached_type = find_in_cache(&tex->type_cache, type_str);
    if (cached_type) {
        // Check for unresolved forward decl cycles
         if (cached_type->kind == TK_UNRESOLVED) {
             report_resolution_error(&tex->context, "Circular dependency detected for type '%s'", type_str);
             return NULL; // Return NULL on cycle detection
         }
        return cached_type;
    }

    // Add placeholder to cache to detect cycles
    Type* placeholder = create_type(type_str, TK_UNRESOLVED);
    if (!placeholder) return NULL; // Allocation failed
    add_to_cache(&tex->type_cache, type_str, placeholder);


    const char* original_str = type_str; // Keep original for potential naming/caching
    Type* resolved_type = NULL;
    unsigned qualifiers = 0;
    char* base_type_name = NULL; // For storing malloc'd base name

    // 1. Parse Qualifiers
    while (1) {
        if (strncmp(type_str, "const ", 6) == 0) {
            qualifiers |= TQ_CONST;
            type_str += 6;
        } else if (strncmp(type_str, "volatile ", 9) == 0) {
            qualifiers |= TQ_VOLATILE;
            type_str += 9;
        } else if (strncmp(type_str, "restrict ", 9) == 0) {
            qualifiers |= TQ_RESTRICT;
            type_str += 9;
        } else {
            break;
        }
        while (isspace((unsigned char)*type_str)) type_str++; // Skip spaces after qualifier
    }

    // Trim leading spaces from the remaining type string
    while (isspace((unsigned char)*type_str)) type_str++;

    // 2. Handle Pointers (*)
    const char* last_asterisk = strrchr(type_str, '*');
    if (last_asterisk) {
        size_t base_len = last_asterisk - type_str;
        base_type_name = malloc(base_len + 1);
        if (!base_type_name) { perror("malloc failed"); goto resolve_fail; }
        strncpy(base_type_name, type_str, base_len);
        base_type_name[base_len] = '\0';

        // Trim trailing whitespace from base type name before recursive call
        char* end = base_type_name + strlen(base_type_name) - 1;
        while (end >= base_type_name && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }

        Type* base_type = resolve_type_recursive(tex, exports, base_type_name);
        free(base_type_name); // Free the temp base name
        base_type_name = NULL;

        if (base_type) {
            resolved_type = create_type(original_str, TK_POINTER);
            if (!resolved_type) goto resolve_fail; // Allocation failed
            resolved_type->data.pointer.base_type = base_type;
            // Apply qualifiers to the pointer itself
            resolved_type->qualifiers = qualifiers;
        } else {
             // Error reported by recursive call
             goto resolve_fail;
        }
        goto resolve_success; // Found pointer type
    }

    // 3. Handle Arrays ([]) - TODO: Handle multi-dimensional, pointer arrays etc.
    const char* first_bracket = strchr(type_str, '[');
    if (first_bracket) {
        size_t base_len = first_bracket - type_str;
        base_type_name = malloc(base_len + 1);
        if (!base_type_name) { perror("malloc failed"); goto resolve_fail; }
        strncpy(base_type_name, type_str, base_len);
        base_type_name[base_len] = '\0';

        // Trim trailing whitespace
        char* end = base_type_name + strlen(base_type_name) - 1;
        while (end >= base_type_name && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }

        Type* element_type = resolve_type_recursive(tex, exports, base_type_name);
        free(base_type_name); // Free the temp base name
        base_type_name = NULL;

        if (element_type) {
            size_t array_size = 0;
            const char* size_start = first_bracket + 1;
            const char* size_end = strchr(size_start, ']');
            if (size_end && size_start != size_end) { // Check for digits between []
                // Simple atoi check, production might need strtol for error handling
                 if (isdigit((unsigned char)*size_start)) {
                     array_size = (size_t)atoi(size_start);
                 }
            } // else: size is 0, representing T[]

            resolved_type = create_type(original_str, TK_ARRAY);
             if (!resolved_type) goto resolve_fail; // Allocation failed
            resolved_type->data.array.element_type = element_type;
            resolved_type->data.array.array_size = array_size;
            // Qualifiers apply to the element type in C arrays, not the array itself
            // Let the element_type carry the qualifiers parsed earlier?
            // Re-applying here might be wrong. Qualifiers usually precede the base type.
            // Let's assume qualifiers apply to the base element type
            // If element_type didn't capture them, maybe re-parse?
            // For now, let element_type handle its own qualifiers from recursive call.
            // The 'qualifiers' variable here might be for the array *itself* if that exists? C doesn't have this.
            // Let's ignore 'qualifiers' for arrays for now.
             resolved_type->qualifiers = TQ_NONE; // Qualifiers belong to element type

        } else {
             // Error reported by recursive call
            goto resolve_fail;
        }
         goto resolve_success; // Found array type
    }


    // 4. Handle Base Types (Primitives, Structs, Unions, Enums, Typedefs, FuncPtrs)

    // Need the base name without struct/union/enum prefixes for lookups
    const char* lookup_name = type_str; // Start with potentially qualified name
    if (strncmp(lookup_name, "struct ", 7) == 0) lookup_name += 7;
    else if (strncmp(lookup_name, "union ", 6) == 0) lookup_name += 6;
    else if (strncmp(lookup_name, "enum ", 5) == 0) lookup_name += 5;
    while (isspace((unsigned char)*lookup_name)) lookup_name++; // Skip spaces

    // 4a. Check Primitive Types
    PrimitiveType pt = get_primitive_type_enum(type_str); // Check original potentially qualified string first
    if (pt == PT_CUSTOM) {
        pt = get_primitive_type_enum(lookup_name); // Check stripped name if needed
    }
    if (pt != PT_CUSTOM) {
        resolved_type = create_type(original_str, TK_PRIMITIVE);
        if (!resolved_type) goto resolve_fail;
        resolved_type->data.primitive = pt;
        resolved_type->qualifiers = qualifiers;
        goto resolve_success;
    }

    // 4b. Check Typedefs (Aliases) - Must happen before Struct/Union/Enum check
    // because a typedef might shadow a struct name.
    apic_Alias* alias_def = find_typedef_def(exports, lookup_name);
    if (alias_def) {
        // Resolve the target type recursively
        Type* target_type = resolve_type_recursive(tex, exports, alias_def->type);
        if (target_type) {
            resolved_type = create_type(original_str, TK_TYPEDEF); // Name is the alias name
             if (!resolved_type) goto resolve_fail;
             // Use original_str (which includes qualifiers) for the typedef name if desired?
             // Let's use the lookup_name (the alias name itself)
             free((void*)resolved_type->name); // free default strdup
             resolved_type->name = strdup(lookup_name); // Use the clean alias name
             if (!resolved_type->name) { free(resolved_type); resolved_type = NULL; goto resolve_fail; }

            resolved_type->data.typedef_target = target_type;
            // Apply qualifiers specified *for the typedef usage*
            resolved_type->qualifiers = qualifiers;
            // Qualifiers on the target type are handled by the recursive call
        } else {
            // Error resolving typedef target
             report_resolution_error(&tex->context, "Could not resolve target type '%s' for typedef '%s'", alias_def->type, lookup_name);
            goto resolve_fail;
        }
        goto resolve_success;
    }

    // 4c. Check FuncPtrs
    apic_FuncPtr* funcptr_def = NULL;
    for (int i = 0; i < exports->funcptr_count; i++) {
        if (strcmp(exports->funcptrs[i]->name, lookup_name) == 0) {
            funcptr_def = exports->funcptrs[i];
            break;
        }
    }
    if (funcptr_def) {
        // Found a named function pointer type
        resolved_type = create_type(lookup_name, TK_FUNCPTR); // Use the clean funcptr name
        if (!resolved_type) goto resolve_fail;
        resolved_type->qualifiers = qualifiers; // Apply qualifiers to the typedef name itself

        // Resolve return and argument types (needed for the signature info)
        resolved_type->data.function.name = funcptr_def->name; // Store typedef name
        resolved_type->data.function.return_type = resolve_type_recursive(tex, exports, funcptr_def->ret);
        if (!resolved_type->data.function.return_type) {
            report_resolution_error(&tex->context, "Failed to resolve return type '%s' for funcptr '%s'", funcptr_def->ret, lookup_name);
             free((void*)resolved_type->name); free(resolved_type); resolved_type = NULL;
             goto resolve_fail;
        }

        resolved_type->data.function.num_params = funcptr_def->count;
        if (funcptr_def->count > 0) {
            resolved_type->data.function.param_types = calloc(funcptr_def->count, sizeof(Type*));
            if (!resolved_type->data.function.param_types) {
                 perror("calloc params failed");
                 free((void*)resolved_type->name); free(resolved_type); resolved_type = NULL;
                 goto resolve_fail;
            }
            for (int j = 0; j < funcptr_def->count; ++j) {
                resolved_type->data.function.param_types[j] = resolve_type_recursive(tex, exports, funcptr_def->args[j].type);
                if (!resolved_type->data.function.param_types[j]) {
                    report_resolution_error(&tex->context, "Failed to resolve param type '%s' for funcptr '%s'", funcptr_def->args[j].type, lookup_name);
                     // Cleanup already allocated params? Yes.
                     for(int k=0; k<j; ++k) { /* Assuming Type* doesn't need deep free here */ }
                     free(resolved_type->data.function.param_types);
                     free((void*)resolved_type->name); free(resolved_type); resolved_type = NULL;
                     goto resolve_fail;
                }
            }
        } else {
            resolved_type->data.function.param_types = NULL;
        }
        goto resolve_success;
    }

    // 4d. Check Structs
    apic_Struct* struct_def = NULL;
     for (int i = 0; i < exports->struct_count; i++) {
        if (strcmp(exports->structs[i]->name, lookup_name) == 0) {
            struct_def = exports->structs[i];
            break;
        }
    }
    if (struct_def) {
         // Search if the StructType has already been created
         Type* existing_struct_type = NULL;
         for(size_t i=0; i < tex->num_structs; ++i) {
              if(tex->structs[i] && strcmp(tex->structs[i]->name, lookup_name) == 0) {
                   existing_struct_type = tex->structs[i]->self_type;
                   break;
              }
         }
         if (existing_struct_type) {
              resolved_type = existing_struct_type;
              // Need to apply qualifiers from *this* usage
              // How? Create a wrapper type? No, apply to the usage reference.
              // We need a way to return the base type AND the qualifiers.
              // Let's create a new Type instance that points to the shared struct def,
              // but carries the qualifiers specific to this usage.
              Type* usage_type = create_type(original_str, TK_STRUCT);
              if (!usage_type) goto resolve_fail;
              usage_type->qualifiers = qualifiers;
              usage_type->data.struct_type = resolved_type->data.struct_type; // Point to same definition
              resolved_type = usage_type;

         } else {
              // Struct definition exists but Typed struct hasn't been created yet.
              // This happens with forward references (e.g. struct Node* next).
              // Create a forward declaration type.
              resolved_type = create_type(original_str, TK_STRUCT);
              if (!resolved_type) goto resolve_fail;
              resolved_type->qualifiers = qualifiers;
              resolved_type->is_forward_decl = 1;
              // We don't have the apic_StructType* yet, store the name for later linking
              // Let's just store the name in the type for now.
               free((void*)resolved_type->name); // free default strdup
               resolved_type->name = strdup(lookup_name); // Use the clean struct name
               if (!resolved_type->name) { free(resolved_type); resolved_type = NULL; goto resolve_fail; }
              // Linkage will happen when the actual struct is processed.
         }
         goto resolve_success;
    }

    // 4e. Check Unions
    apic_Union* union_def = NULL;
    for (int i = 0; i < exports->union_count; i++) {
        if (strcmp(exports->unions[i]->name, lookup_name) == 0) {
            union_def = exports->unions[i];
            break;
        }
    }
     if (union_def) {
         Type* existing_union_type = NULL;
         for(size_t i=0; i < tex->num_unions; ++i) {
             if(tex->unions[i] && strcmp(tex->unions[i]->name, lookup_name) == 0) {
                  existing_union_type = tex->unions[i]->self_type;
                  break;
             }
         }
         if(existing_union_type) {
             // Create a usage-specific type wrapper for qualifiers
             Type* usage_type = create_type(original_str, TK_UNION);
             if (!usage_type) goto resolve_fail;
             usage_type->qualifiers = qualifiers;
             usage_type->data.union_type = existing_union_type->data.union_type;
             resolved_type = usage_type;
         } else {
             // Forward declaration needed
             resolved_type = create_type(original_str, TK_UNION);
              if (!resolved_type) goto resolve_fail;
              resolved_type->qualifiers = qualifiers;
              resolved_type->is_forward_decl = 1;
              free((void*)resolved_type->name); // free default strdup
              resolved_type->name = strdup(lookup_name); // Use the clean union name
              if (!resolved_type->name) { free(resolved_type); resolved_type = NULL; goto resolve_fail; }
         }
         goto resolve_success;
    }


    // 4f. Check Enums
     apic_Enum* enum_def = NULL;
     for (int i = 0; i < exports->enum_count; i++) {
         if (strcmp(exports->enums[i]->name, lookup_name) == 0) {
             enum_def = exports->enums[i];
             break;
         }
     }
      if (enum_def) {
         Type* existing_enum_type = NULL;
         for(size_t i=0; i < tex->num_enums; ++i) {
              if(tex->enums[i] && strcmp(tex->enums[i]->name, lookup_name) == 0) {
                   existing_enum_type = tex->enums[i]->self_type;
                   break;
              }
         }
          if(existing_enum_type) {
              // Create a usage-specific type wrapper for qualifiers
              Type* usage_type = create_type(original_str, TK_ENUM);
              if (!usage_type) goto resolve_fail;
              usage_type->qualifiers = qualifiers;
              usage_type->data.enum_type = existing_enum_type->data.enum_type;
              resolved_type = usage_type;
          } else {
              // Forward declaration (less common for enums, but possible)
              resolved_type = create_type(original_str, TK_ENUM);
              if (!resolved_type) goto resolve_fail;
              resolved_type->qualifiers = qualifiers;
              resolved_type->is_forward_decl = 1;
              free((void*)resolved_type->name); // free default strdup
              resolved_type->name = strdup(lookup_name); // Use the clean enum name
              if (!resolved_type->name) { free(resolved_type); resolved_type = NULL; goto resolve_fail; }
          }
         goto resolve_success;
     }


    // 5. If not found anywhere
    report_resolution_error(&tex->context, "Unknown type '%s'", original_str);
    // Fall through to resolve_fail

resolve_fail:
    // Remove placeholder from cache ONLY if we added it AND resolution failed
    if (placeholder) {
        for(size_t i = 0; i < tex->type_cache.count; ++i) {
            if(tex->type_cache.entries[i].type == placeholder) {
                // Shift remaining entries down
                memmove(&tex->type_cache.entries[i], &tex->type_cache.entries[i+1],
                        (tex->type_cache.count - i - 1) * sizeof(tex->type_cache.entries[0]));
                tex->type_cache.count--;
                break;
            }
        }
        free((void*)placeholder->name);
        free(placeholder);
    }
    if (base_type_name) free(base_type_name); // Cleanup if error occurred mid-parse
     // Do not free resolved_type here, it might be NULL or owned elsewhere
    return NULL;

resolve_success:
     // Replace placeholder with the actual resolved type in cache
     if (placeholder) {
         for(size_t i = 0; i < tex->type_cache.count; ++i) {
             if(tex->type_cache.entries[i].type == placeholder) {
                 tex->type_cache.entries[i].type = resolved_type; // Update cache entry
                 break;
             }
         }
         free((void*)placeholder->name); // Free the placeholder struct itself
         free(placeholder);
     } else {
          // This case should ideally not happen if caching logic is correct
          // If it wasn't cached before (e.g. direct hit), add it now.
           if (!find_in_cache(&tex->type_cache, original_str)) {
               add_to_cache(&tex->type_cache, original_str, resolved_type);
           }
     }

    // Sanity check: Ensure the resolved type has a valid kind
    if (!resolved_type || resolved_type->kind == TK_UNRESOLVED) {
        // This indicates a logic error somewhere above
        report_resolution_error(&tex->context, "Internal error: Resolution succeeded but result kind is invalid for '%s'", original_str);
        // Attempt recovery if possible, or return NULL
        return NULL;
    }

    return resolved_type;
}


// Entry point for type resolution, sets up context
static Type* resolve_type(apic_TypedExports* tex, apic_Exports* exports, const char* type_str) {
     // tex should already have its context initialized by apicext_create_typed_exports
     return resolve_type_recursive(tex, exports, type_str);
}


apic_TypedExports* apicext_create_typed_exports(apic_Exports* exports) {
    if (!exports) return NULL;

    apic_TypedExports* tex = calloc(1, sizeof(apic_TypedExports));
    if (!tex) {
        perror("Failed to allocate TypedExports");
        return NULL;
    }

    tex->name = exports->name;
    tex->doc = exports->doc;
    tex->context.error_count = 0;
    tex->context.current_exports = exports; // Set context
    init_type_cache(&tex->type_cache);

    // --- Pre-create forward declaration Types for all structs/unions/enums ---
    // This helps resolve cycles and ensures Types exist before being referenced.

    // Structs Forward Decl
    tex->num_structs = exports->struct_count;
    if (tex->num_structs > 0) {
        tex->structs = calloc(tex->num_structs, sizeof(apic_StructType*));
        if (!tex->structs) goto cleanup;
        for (int i = 0; i < exports->struct_count; i++) {
            apic_Struct* st_def = exports->structs[i];
            Type* t = create_type(st_def->name, TK_STRUCT);
            if (!t) goto cleanup;
            // Don't add to cache here yet, wait until full definition is processed
            // Create the StructType shell
             tex->structs[i] = calloc(1, sizeof(apic_StructType));
             if (!tex->structs[i]) goto cleanup;
             tex->structs[i]->name = st_def->name;
             tex->structs[i]->doc = st_def->doc;
             tex->structs[i]->self_type = t; // Link Type back
             t->data.struct_type = tex->structs[i]; // Link StructType back
              // Add this *base* type (without qualifiers) to cache
             add_to_cache(&tex->type_cache, st_def->name, t);
        }
    }
     // Unions Forward Decl
    tex->num_unions = exports->union_count;
    if (tex->num_unions > 0) {
        tex->unions = calloc(tex->num_unions, sizeof(apic_UnionType*));
        if (!tex->unions) goto cleanup;
        for (int i = 0; i < exports->union_count; i++) {
            apic_Union* u_def = exports->unions[i];
            Type* t = create_type(u_def->name, TK_UNION);
            if (!t) goto cleanup;
            tex->unions[i] = calloc(1, sizeof(apic_UnionType));
            if (!tex->unions[i]) goto cleanup;
            tex->unions[i]->name = u_def->name;
            tex->unions[i]->doc = u_def->doc;
            tex->unions[i]->self_type = t;
            t->data.union_type = tex->unions[i];
             add_to_cache(&tex->type_cache, u_def->name, t);
        }
    }
    // Enums Forward Decl
    tex->num_enums = exports->enum_count;
     if (tex->num_enums > 0) {
        tex->enums = calloc(tex->num_enums, sizeof(apic_EnumType*));
        if (!tex->enums) goto cleanup;
         for (int i = 0; i < exports->enum_count; i++) {
             apic_Enum* e_def = exports->enums[i];
             Type* t = create_type(e_def->name, TK_ENUM);
             if (!t) goto cleanup;
             tex->enums[i] = calloc(1, sizeof(apic_EnumType));
              if (!tex->enums[i]) goto cleanup;
             tex->enums[i]->name = e_def->name;
             tex->enums[i]->doc = e_def->doc;
             tex->enums[i]->self_type = t;
             t->data.enum_type = tex->enums[i];
              add_to_cache(&tex->type_cache, e_def->name, t);
         }
     }
     // FuncPtrs Forward Decl (Create Type objects for them)
      tex->num_funcptrs = exports->funcptr_count;
      if (tex->num_funcptrs > 0) {
          tex->funcptrs = calloc(tex->num_funcptrs, sizeof(Type*));
           if (!tex->funcptrs) goto cleanup;
           for (int i = 0; i < exports->funcptr_count; ++i) {
                apic_FuncPtr* fp_def = exports->funcptrs[i];
                // Check cache first in case already resolved via usage
                Type* fp_type = find_in_cache(&tex->type_cache, fp_def->name);
                if (!fp_type) {
                     fp_type = create_type(fp_def->name, TK_FUNCPTR);
                     if (!fp_type) goto cleanup;
                      fp_type->data.function.name = fp_def->name; // Store typedef name
                     // Resolve return/params later in the main loop
                      add_to_cache(&tex->type_cache, fp_def->name, fp_type); // Cache the base funcptr type
                }
                 tex->funcptrs[i] = fp_type;
           }
      }
      // Typedefs Forward Decl (Create Type objects)
       tex->num_aliases = exports->typedef_count;
       if (tex->num_aliases > 0) {
           tex->aliases = calloc(tex->num_aliases, sizeof(Type*));
            if (!tex->aliases) goto cleanup;
            for (int i = 0; i < exports->typedef_count; ++i) {
                 apic_Alias* td_def = exports->aliases[i];
                 Type* td_type = find_in_cache(&tex->type_cache, td_def->name);
                 if (!td_type) {
                      td_type = create_type(td_def->name, TK_TYPEDEF);
                      if (!td_type) goto cleanup;
                      // Resolve target later
                       add_to_cache(&tex->type_cache, td_def->name, td_type);
                 }
                  tex->aliases[i] = td_type;
            }
       }


    // --- Now resolve details ---

    /* Convert structs */
    for (size_t i = 0; i < tex->num_structs; i++) {
        apic_Struct* st_def = exports->structs[i];
        apic_StructType* stype = tex->structs[i]; // Get pre-allocated shell

        stype->num_fields = st_def->count;
        if (st_def->count > 0) {
            stype->fields = calloc(st_def->count, sizeof(apic_TypedField));
            if (!stype->fields) goto cleanup;

            for (int j = 0; j < st_def->count; j++) {
                apic_Field f = st_def->fields[j];
                stype->fields[j].name = f.name;
                stype->fields[j].doc = f.doc;
                stype->fields[j].type = resolve_type(tex, exports, f.type);
                 if (!stype->fields[j].type) {
                     // Error already reported by resolve_type
                     report_resolution_error(&tex->context, "...in field '%s' of struct '%s'", f.name, stype->name);
                 }
            }
        }
    }

    /* Convert unions */
     for (size_t i = 0; i < tex->num_unions; i++) {
        apic_Union* u_def = exports->unions[i];
        apic_UnionType* utype = tex->unions[i];

        utype->num_fields = u_def->count;
        if (u_def->count > 0) {
            utype->fields = calloc(u_def->count, sizeof(apic_TypedField));
             if (!utype->fields) goto cleanup;

             for (int j = 0; j < u_def->count; j++) {
                apic_Field f = u_def->fields[j];
                utype->fields[j].name = f.name;
                utype->fields[j].doc = f.doc;
                utype->fields[j].type = resolve_type(tex, exports, f.type);
                 if (!utype->fields[j].type) {
                      report_resolution_error(&tex->context, "...in field '%s' of union '%s'", f.name, utype->name);
                 }
            }
        }
     }

    /* Convert enums */
     for (size_t i = 0; i < tex->num_enums; i++) {
        apic_Enum* e_def = exports->enums[i];
        apic_EnumType* etype = tex->enums[i];

        etype->num_entries = e_def->count;
        if (e_def->count > 0) {
             etype->entries = calloc(e_def->count, sizeof(apic_TypedEnumEntry));
             if (!etype->entries) goto cleanup;
             for (int j = 0; j < e_def->count; j++) {
                 apic_EnumEntry entry_def = e_def->entries[j]; // Corrected type
                 etype->entries[j].name = entry_def.name;
                 etype->entries[j].value = entry_def.value;
                 etype->entries[j].str_value = entry_def.str;
                 etype->entries[j].doc = entry_def.doc;
             }
         }
     }


    /* Convert aliases (resolve target types) */
    for (size_t i = 0; i < tex->num_aliases; i++) {
        apic_Alias* td_def = exports->aliases[i];
        Type* t = tex->aliases[i]; // Get pre-allocated Type shell

         // Ensure it's still marked as TYPEDEF (could have been resolved differently?)
         if (t->kind != TK_TYPEDEF && t->kind != TK_UNRESOLVED) {
             // This might happen if the typedef name was also used directly elsewhere
             // before the typedef itself was processed. Should be okay?
             // Let's assume the existing resolution is fine. Or should we force TK_TYPEDEF?
              // For now, trust the existing resolution if it's not unresolved.
              if (t->kind != TK_UNRESOLVED) continue;
              t->kind = TK_TYPEDEF; // Force kind if unresolved
         }

        t->data.typedef_target = resolve_type(tex, exports, td_def->type);
         if (!t->data.typedef_target) {
             report_resolution_error(&tex->context, "...resolving target type '%s' for alias '%s'", td_def->type, t->name);
         }
    }

     /* Convert funcptrs (resolve return/param types) */
     for (size_t i = 0; i < tex->num_funcptrs; i++) {
         apic_FuncPtr* fp_def = exports->funcptrs[i];
         Type* t = tex->funcptrs[i]; // Get pre-allocated Type shell

          if (t->kind != TK_FUNCPTR && t->kind != TK_UNRESOLVED) {
              if (t->kind != TK_UNRESOLVED) continue; // Already resolved somehow
              t->kind = TK_FUNCPTR; // Force kind if unresolved
          }
          t->kind = TK_FUNCPTR; // Ensure kind is set

         // Resolve if not already done (e.g., if resolved via usage first)
         if (!t->data.function.return_type) {
             t->data.function.return_type = resolve_type(tex, exports, fp_def->ret);
             if (!t->data.function.return_type) {
                 report_resolution_error(&tex->context, "...resolving return type '%s' for funcptr '%s'", fp_def->ret, t->name);
             }
         }

         if (fp_def->count > 0 && !t->data.function.param_types) {
             t->data.function.num_params = fp_def->count;
             t->data.function.param_types = calloc(fp_def->count, sizeof(Type*));
             if (!t->data.function.param_types) goto cleanup;
             for (int j = 0; j < fp_def->count; j++) {
                 t->data.function.param_types[j] = resolve_type(tex, exports, fp_def->args[j].type);
                 if (!t->data.function.param_types[j]) {
                     report_resolution_error(&tex->context, "...resolving param type '%s' for funcptr '%s'", fp_def->args[j].type, t->name);
                     // Don't need to free here, main cleanup will handle it
                 }
             }
         } else if (fp_def->count == 0) {
              t->data.function.num_params = 0;
              t->data.function.param_types = NULL;
         }
     }


    /* Convert functions */
    tex->num_functions = exports->func_count;
    if (tex->num_functions > 0) {
        tex->functions = calloc(tex->num_functions, sizeof(apic_TypedFunction*));
        if (!tex->functions) goto cleanup;

        for (int i = 0; i < exports->func_count; i++) {
            apic_Func* fn_def = exports->funcs[i];
            apic_TypedFunction* tfn = calloc(1, sizeof(apic_TypedFunction));
            if (!tfn) goto cleanup;

            tfn->name = fn_def->name;
            tfn->doc = fn_def->doc;
            tfn->return_type = resolve_type(tex, exports, fn_def->ret);
             if (!tfn->return_type) {
                 report_resolution_error(&tex->context, "...resolving return type '%s' for function '%s'", fn_def->ret, tfn->name);
             }

            tfn->num_params = fn_def->count;
            if (fn_def->count > 0) {
                tfn->params = calloc(fn_def->count, sizeof(apic_TypedField));
                if (!tfn->params) {
                    free(tfn); // cleanup allocated function struct
                    goto cleanup;
                }

                for (int j = 0; j < fn_def->count; j++) {
                    apic_Arg a = fn_def->args[j];
                    tfn->params[j].name = a.name;
                    tfn->params[j].doc = a.doc; // Args don't have separate doc in apic_Arg? Copy from field.
                    tfn->params[j].type = resolve_type(tex, exports, a.type);
                     if (!tfn->params[j].type) {
                         report_resolution_error(&tex->context, "...resolving param type '%s' for function '%s'", a.type, tfn->name);
                     }
                }
            }
            tex->functions[i] = tfn;
        }
    }

    // --- Final Validation Summary ---
     if (tex->context.error_count > 0) {
         printf("\nFound %d type resolution errors!\n", tex->context.error_count);
         // Optional: consider returning NULL or a special error indicator
          // cleanup_typed_exports(tex); // Clean up partially created structure
          // return NULL;
         // For now, let's return the partial structure but indicate errors occurred.
     } else {
         printf("// All type resolutions passed successfully!\n");
     }

    return tex;

cleanup:
    perror("Memory allocation failed during typed export creation");
    // Add proper cleanup code here
    cleanup_typed_exports(tex); // Call cleanup function
    return NULL;
}

// TODO: Implement cleanup function to free all allocated memory in TypedExports
static void cleanup_typed_exports(apic_TypedExports* tex) {
     if (!tex) return;

     // Free function definitions
     if (tex->functions) {
         for (size_t i = 0; i < tex->num_functions; ++i) {
             if (tex->functions[i]) {
                 // Params are TypedFields, Type* inside is managed by cache/types
                 free(tex->functions[i]->params);
                 free(tex->functions[i]);
             }
         }
         free(tex->functions);
     }

     // Free struct definitions
      if (tex->structs) {
          for (size_t i = 0; i < tex->num_structs; ++i) {
              if (tex->structs[i]) {
                  free(tex->structs[i]->fields); // Frees array of TypedField
                  // Type* fields inside TypedField managed by cache/types
                   // self_type Type* managed by cache/types
                  free(tex->structs[i]);
              }
          }
          free(tex->structs);
      }
     // Free union definitions (similar to structs)
      if (tex->unions) {
           for (size_t i = 0; i < tex->num_unions; ++i) {
               if (tex->unions[i]) {
                   free(tex->unions[i]->fields);
                   free(tex->unions[i]);
               }
           }
           free(tex->unions);
      }
     // Free enum definitions
      if (tex->enums) {
           for (size_t i = 0; i < tex->num_enums; ++i) {
               if (tex->enums[i]) {
                   free(tex->enums[i]->entries); // Frees array of TypedEnumEntry
                   free(tex->enums[i]);
               }
           }
           free(tex->enums);
      }

     // Free funcptrs array (Type* managed by cache)
     free(tex->funcptrs);

     // Free aliases array (Type* managed by cache)
     free(tex->aliases);

     // Free variables (if implemented)
     // free(tex->variables)

     // Free Type Cache Contents
      for (size_t i = 0; i < tex->type_cache.count; ++i) {
          Type* t = tex->type_cache.entries[i].type;
          if (t) {
              // Free data specific allocations based on kind
              switch(t->kind) {
                  case TK_POINTER: // base_type is another cached type
                      break;
                  case TK_ARRAY: // element_type is another cached type
                      break;
                  case TK_FUNCTION:
                  case TK_FUNCPTR:
                      free(t->data.function.param_types);
                      // return_type and params are cached types
                       break;
                   case TK_TYPEDEF: // typedef_target is cached
                       break;
                   // Struct/Union/Enum types are freed above
                   case TK_STRUCT:
                   case TK_UNION:
                   case TK_ENUM:
                        break;
                   case TK_PRIMITIVE:
                   case TK_UNRESOLVED: // Should not persist
                   default:
                       break; // No deep free needed for primitive/base
               }
              free((void*)t->name); // Free the strdup'd name
              free(t); // Free the Type struct itself
          }
      }

     // Free the main struct
     free(tex);
}


#endif /* APIC_REFLECT */

#endif // APIC_TYPES_H