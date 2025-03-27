#include "apic.h"
#include <string.h>

int enum_str_to_int(const Enum* en, const char* str) {
    if (!en || !str) return -1;
    for (size_t i = 0; i < en->fields_count; i++) {
        if (strcmp(str, en->fields[i].str_value) == 0) {
            return en->fields[i].value;
        }
    }
    return -1; // Not found
}

const char* enum_int_to_str(const Enum* en, int value) {
    if (!en) return NULL;
    for (size_t i = 0; i < en->fields_count; i++) {
        if (en->fields[i].value == value) {
            return en->fields[i].str_value;
        }
    }
    return NULL; // Not found
}

// // --- Forward Declarations for Type Resolution Helpers ---
// const ArrayType* find_array_type(const Module* mod, const char* name);
// const PtrType* find_ptr_type(const Module* mod, const char* name);
// const Record* find_record(const Module* mod, const char* name);
// const Variant* find_variant(const Module* mod, const char* name);
// const Enum* find_enum(const Module* mod, const char* name);

// --- Type Resolution Helpers ---
const ArrayType* find_array_type(const Module* mod, const char* name) {
    for (size_t i = 0; i < mod->arrays_count; i++) {
        if (strcmp(mod->arrays[i]->name, name) == 0) return mod->arrays[i];
    }
    return NULL;
}

const PtrType* find_ptr_type(const Module* mod, const char* name) {
    for (size_t i = 0; i < mod->ptrs_count; i++) {
        if (strcmp(mod->ptrs[i]->name, name) == 0) return mod->ptrs[i];
    }
    return NULL;
}

const Record* find_record(const Module* mod, const char* name) {
    for (size_t i = 0; i < mod->records_count; i++) {
        if (strcmp(mod->records[i]->name, name) == 0) return mod->records[i];
    }
    return NULL;
}

const Variant* find_variant(const Module* mod, const char* name) {
    for (size_t i = 0; i < mod->variants_count; i++) {
        if (strcmp(mod->variants[i]->name, name) == 0) return mod->variants[i];
    }
    return NULL;
}

const Enum* find_enum(const Module* mod, const char* name) {
    for (size_t i = 0; i < mod->enums_count; i++) {
        if (strcmp(mod->enums[i]->name, name) == 0) return mod->enums[i];
    }
    return NULL;
}

// --- Pretty Print Function ---
void pretty_print_module(const Module* mod) {
    printf("Module: %s\n", mod->name);
    printf("Description: %s\n\n", mod->doc ? mod->doc : "(No documentation)");

    // Arrays
    if (mod->arrays_count > 0) {
        printf("Array Types (%zu):\n", mod->arrays_count);
        for (size_t i = 0; i < mod->arrays_count; i++) {
            const ArrayType* arr = mod->arrays[i];
            printf("  %s: %s\n", arr->name, arr->doc ? arr->doc : "(No documentation)");
            printf("    Item Type: %s\n", arr->item_type);
            printf("    Size: %zu\n\n", arr->size);
        }
    }

    // Pointers
    if (mod->ptrs_count > 0) {
        printf("Pointer Types (%zu):\n", mod->ptrs_count);
        for (size_t i = 0; i < mod->ptrs_count; i++) {
            const PtrType* ptr = mod->ptrs[i];
            printf("  %s: %s\n", ptr->name, ptr->doc ? ptr->doc : "(No documentation)");
            printf("    Pointee Type: %s\n\n", ptr->pointee_type);
        }
    }

    // Records
    if (mod->records_count > 0) {
        printf("Records (%zu):\n", mod->records_count);
        for (size_t i = 0; i < mod->records_count; i++) {
            const Record* rec = mod->records[i];
            printf("  %s: %s\n", rec->name, rec->doc ? rec->doc : "(No documentation)");
            for (size_t j = 0; j < rec->fields_count; j++) {
                const Field* f = &rec->fields[j];
                printf("    - %s: %s", f->name, f->type);

                const ArrayType* arr = find_array_type(mod, f->type);
                const PtrType* ptr = find_ptr_type(mod, f->type);
                const Record* rec_type = find_record(mod, f->type);

                if (arr) printf(" (array of %s[%zu])", arr->item_type, arr->size);
                else if (ptr) printf(" (pointer to %s)", ptr->pointee_type);
                else if (rec_type) printf(" (record)");

                printf("\n    | %s\n", f->doc ? f->doc : "(No documentation)");
            }
            printf("\n");
        }
    }

    // Variants
    if (mod->variants_count > 0) {
        printf("Variants (%zu):\n", mod->variants_count);
        for (size_t i = 0; i < mod->variants_count; i++) {
            const Variant* var = mod->variants[i];
            printf("  %s: %s\n", var->name, var->doc ? var->doc : "(No documentation)");
            for (size_t j = 0; j < var->choices_count; j++) {
                const Choice* ch = &var->choices[j];
                printf("    - %s: %s\n    | %s\n",
                    ch->name, ch->type, ch->doc ? ch->doc : "(No documentation)");
            }
            printf("\n");
        }
    }

    // Functions
    if (mod->functions_count > 0) {
        printf("Functions (%zu):\n", mod->functions_count);
        for (size_t i = 0; i < mod->functions_count; i++) {
            const Function* func = mod->functions[i];
            printf("  %s: %s\n", func->name, func->doc ? func->doc : "(No documentation)");

            if (func->inputs_count > 0) {
                printf("    Inputs:\n");
                for (size_t j = 0; j < func->inputs_count; j++) {
                    const Arg* arg = &func->inputs[j];
                    printf("    - %s: %s", arg->name, arg->type);

                    const ArrayType* arr = find_array_type(mod, arg->type);
                    const PtrType* ptr = find_ptr_type(mod, arg->type);

                    if (arr) printf(" (array of %s[%zu])", arr->item_type, arr->size);
                    else if (ptr) printf(" (pointer to %s)", ptr->pointee_type);

                    printf("\n    | %s\n", arg->doc ? arg->doc : "(No documentation)");
                }
            }

            if (func->output) {
                if (strcmp(func->output->type, VOID) != 0) {
                    printf("    Output:\n");
                    printf("    - %s\n    | %s\n",
                        func->output->type,
                        func->output->doc ? func->output->doc : "(No documentation)");
                }
            }
            printf("\n");
        }
    }

    // Enums
    if (mod->enums_count > 0) {
        printf("Enums (%zu):\n", mod->enums_count);
        for (size_t i = 0; i < mod->enums_count; i++) {
            const Enum* en = mod->enums[i];
            printf("  %s: %s\n", en->name, en->doc ? en->doc : "(No documentation)");
            for (size_t j = 0; j < en->fields_count; j++) {
                const EnumField* ef = &en->fields[j];
                printf("    - %s = %d (%s)\n    | %s\n",
                       ef->name, ef->value, ef->str_value,
                       ef->doc ? ef->doc : "(No documentation)");
            }
            printf("\n");
        }
    }
}

// --- Header Generation Helpers ---
const char* map_type_to_c(const Module* mod, const char* type_name) {
    // Map primitive types
    if (strcmp(type_name, INT32) == 0) return "int32_t";
    if (strcmp(type_name, INT64) == 0) return "int64_t";
    if (strcmp(type_name, FLOAT) == 0) return "float";
    if (strcmp(type_name, DOUBLE) == 0) return "double";
    if (strcmp(type_name, ISIZE) == 0) return "intptr_t";
    if (strcmp(type_name, USIZE) == 0) return "uintptr_t";
    if (strcmp(type_name, BYTE) == 0) return "uint8_t";
    if (strcmp(type_name, STRING) == 0) return "const char*";
    if (strcmp(type_name, VOID) == 0) return "void";

    // Check for array types
    if (find_array_type(mod, type_name)) return type_name;

    // Check for pointer types
    if (find_ptr_type(mod, type_name)) return type_name;

    // Check for records, variants, enums
    if (find_record(mod, type_name) ||
        find_variant(mod, type_name) ||
        find_enum(mod, type_name)) {
        return type_name;
    }

    return type_name;
}

void generate_c_header(const Module* mod) {
    // Header guard
    printf("#ifndef %s_H\n", mod->name);
    printf("#define %s_H\n\n", mod->name);

    // Basic includes
    printf("#include <stdint.h>\n");
    printf("#include <stddef.h>\n\n");

    // Forward declarations
    for (size_t i = 0; i < mod->records_count; i++) {
        printf("struct %s;\n", mod->records[i]->name);
    }
    for (size_t i = 0; i < mod->variants_count; i++) {
        printf("union %s;\n", mod->variants[i]->name);
    }
    printf("\n");

    // Pointer typedefs
    for (size_t i = 0; i < mod->ptrs_count; i++) {
        const PtrType* ptr = mod->ptrs[i];
        const Record* rec = find_record(mod, ptr->pointee_type);
        const Variant* var = find_variant(mod, ptr->pointee_type);

        if (rec) {
            printf("typedef struct %s* %s;\n", ptr->pointee_type, ptr->name);
        } else if (var) {
            printf("typedef union %s* %s;\n", ptr->pointee_type, ptr->name);
        } else {
            const char* base = map_type_to_c(mod, ptr->pointee_type);
            printf("typedef %s* %s;\n", base, ptr->name);
        }
    }
    printf("\n");

    // Array typedefs
    for (size_t i = 0; i < mod->arrays_count; i++) {
        const ArrayType* arr = mod->arrays[i];
        const char* item_type = map_type_to_c(mod, arr->item_type);
        printf("typedef %s %s[%zu];\n", item_type, arr->name, arr->size);
    }
    printf("\n");

    // Record definitions
    for (size_t i = 0; i < mod->records_count; i++) {
        const Record* rec = mod->records[i];
        printf("typedef struct %s {\n", rec->name);

        for (size_t j = 0; j < rec->fields_count; j++) {
            const Field* f = &rec->fields[j];
            const char* c_type = map_type_to_c(mod, f->type);
            printf("    %s %s;\n", c_type, f->name);
        }

        printf("} %s;\n\n", rec->name);
    }

    // Variant definitions
    for (size_t i = 0; i < mod->variants_count; i++) {
        const Variant* var = mod->variants[i];
        printf("typedef union %s {\n", var->name);

        for (size_t j = 0; j < var->choices_count; j++) {
            const Choice* ch = &var->choices[j];
            const char* c_type = map_type_to_c(mod, ch->type);
            printf("    %s %s;\n", c_type, ch->name);
        }

        printf("} %s;\n\n", var->name);
    }

    // Enum definitions
    for (size_t i = 0; i < mod->enums_count; i++) {
        const Enum* en = mod->enums[i];
        printf("typedef enum %s {\n", en->name);

        for (size_t j = 0; j < en->fields_count; j++) {
            const EnumField* ef = &en->fields[j];
            printf("    %s = %d", ef->name, ef->value);
            if (ef->str_value) printf(" /* %s */", ef->str_value);
            if (j < en->fields_count-1) printf(",");
            printf("\n");
        }

        printf("} %s;\n\n", en->name);
    }

    // Function prototypes
    for (size_t i = 0; i < mod->functions_count; i++) {
        const Function* func = mod->functions[i];
        const char* ret_type = map_type_to_c(mod, func->output->type);

        printf("%s %s(", ret_type, func->name);

        for (size_t j = 0; j < func->inputs_count; j++) {
            const Arg* arg = &func->inputs[j];
            const char* c_type = map_type_to_c(mod, arg->type);
            printf("%s %s", c_type, arg->name);
            if (j < func->inputs_count-1) printf(", ");
        }

        printf(");\n");
    }

    printf("\n#endif // %s_H\n", mod->name);
}

static const char* map_type_to_lua(const Module* mod, const char* type_name) {
    if (find_enum(mod, type_name)) return "enum";
    if (strcmp(type_name, FLOAT) == 0 || strcmp(type_name, DOUBLE) == 0) return "number";
    if (strcmp(type_name, INT32) == 0 || strcmp(type_name, INT64) == 0) return "integer";
    if (strcmp(type_name, STRING) == 0) return "string";
    return type_name; // userdata
}



// Define the hash function as a macro for consistency
#define HASH_FUNCTION(str) \
    do { \
        unsigned int hash = 5381; \
        int c; \
        while ((c = *str++)) { \
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */ \
        } \
        return hash; \
    } while (0)

// Compute hash using the macro for precomputing enum string hashes
static unsigned int compute_hash(const char* str) {
    HASH_FUNCTION(str);
}

// Print a string with proper escaping for C string literals
static void print_escaped_string(const char* str) {
    while (*str) {
        switch (*str) {
            case '\\': printf("\\\\"); break;
            case '"': printf("\\\""); break;
            case '\n': printf("\\n"); break;
            case '\r': printf("\\r"); break;
            case '\t': printf("\\t"); break;
            default: putchar(*str); break;
        }
        str++;
    }
}

void generate_lua_bindings(const Module* mod) {
    // Include necessary headers
    printf("#include <lua.h>\n");
    printf("#include <lauxlib.h>\n");
    printf("#include <string.h>\n\n");

    // Define the EnumEntry struct
    printf("typedef struct {\n");
    printf("    int value;\n");
    printf("    const char* str;\n");
    printf("    unsigned int hash;\n");
    printf("} EnumEntry;\n\n");

    // Generate the compute_hash function
    printf("static unsigned int compute_hash(const char* str) {\n");
    printf("    unsigned int hash = 5381;\n");
    printf("    int c;\n");
    printf("    while ((c = *str++)) {\n");
    printf("        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */\n");
    printf("    }\n");
    printf("    return hash;\n");
    printf("}\n\n");

    // Generate code for each enum
    for (size_t i = 0; i < mod->enums_count; i++) {
        const Enum* en = mod->enums[i];

        // Generate the enum entries array
        printf("static const EnumEntry enum_%s_entries[] = {\n", en->name);
        for (size_t j = 0; j < en->fields_count; j++) {
            const EnumField* ef = &en->fields[j];
            unsigned int hash = compute_hash(ef->str_value); // Precompute hash during generation
            printf("    { %d, \"", ef->value);
            print_escaped_string(ef->str_value);
            printf("\", %u }%s\n", hash, j < en->fields_count - 1 ? "," : "");
        }
        printf("};\n\n");

        // Generate the str_to_int function
        printf("static int enum_%s_str_to_int(const char* str) {\n", en->name);
        printf("    unsigned int h = compute_hash(str);\n");
        printf("    for (size_t i = 0; i < sizeof(enum_%s_entries)/sizeof(EnumEntry); i++) {\n", en->name);
        printf("        if (enum_%s_entries[i].hash == h && strcmp(str, enum_%s_entries[i].str) == 0) {\n", en->name, en->name);
        printf("            return enum_%s_entries[i].value;\n", en->name);
        printf("        }\n");
        printf("    }\n");
        printf("    return -1; // Return -1 for invalid string\n");
        printf("}\n\n");
    }

    // Generate record constructors
    for (size_t i = 0; i < mod->records_count; i++) {
        const Record* rec = mod->records[i];
        printf("static int l_new_%s(lua_State* L) {\n", rec->name);
        printf("    %s* obj = (%s*)lua_newuserdata(L, sizeof(%s));\n", rec->name, rec->name, rec->name);
        printf("    luaL_getmetatable(L, \"%s\");\n", rec->name);
        printf("    lua_setmetatable(L, -2);\n");
        for (size_t j = 0; j < rec->fields_count; j++) {
            const Field* f = &rec->fields[j];
            if (find_enum(mod, f->type)) {
                printf("    const char* %s_str = luaL_checkstring(L, %zu);\n", f->name, j + 1);
                printf("    int %s_int = enum_%s_str_to_int(%s_str);\n", f->name, f->type, f->name);
                printf("    if (%s_int == -1) {\n", f->name);
                printf("        return luaL_error(L, \"Invalid %s value: %%s\", %s_str);\n", f->type, f->name);
                printf("    }\n");
                printf("    obj->%s = %s_int;\n", f->name, f->name);
            } else {
                printf("    obj->%s = luaL_checkinteger(L, %zu);\n", f->name, j + 1);
            }
        }
        printf("    return 1;\n");
        printf("}\n\n");
    }

    // Generate function wrappers
    for (size_t i = 0; i < mod->functions_count; i++) {
        const Function* fn = mod->functions[i];
        // Generate the wrapper function signature
        printf("static int l_%s(lua_State* L) {\n", fn->name);

        // Get input arguments from Lua (simplified for this example)
        for (size_t j = 0; j < fn->inputs_count; j++) {
            const Arg* arg = &fn->inputs[j];
            printf("    int %s = luaL_checkinteger(L, %zu);\n", arg->name, j + 1);
        }

        // Call the function with the correct return type
        const char* return_type = fn->output ? fn->output->type : "void";
        if (strcmp(return_type, "void") != 0) {
            printf("    %s result = %s(", map_type_to_c(mod, return_type), fn->name);
        } else {
            printf("    %s(", fn->name);
        }
        // Pass arguments
        for (size_t j = 0; j < fn->inputs_count; j++) {
            if (j > 0) printf(", ");
            printf("%s", fn->inputs[j].name);
        }
        printf(");\n");

        // Handle the return value
        if (strcmp(return_type, "void") != 0) {
            if (find_record(mod, return_type)) {
                // Struct return type (e.g., Vec2)
                printf("    %s* ud = (%s*)lua_newuserdata(L, sizeof(%s));\n",
                       return_type, return_type, return_type);
                printf("    *ud = result;\n");
                printf("    luaL_getmetatable(L, \"%s\");\n", return_type);
                printf("    lua_setmetatable(L, -2);\n");
            } else if (strcmp(return_type, "int") == 0) {
                printf("    lua_pushinteger(L, result);\n");
            } else if (strcmp(return_type, "float") == 0 || strcmp(return_type, "double") == 0) {
                printf("    lua_pushnumber(L, result);\n");
            } else if (strcmp(return_type, "string") == 0) {
                printf("    lua_pushstring(L, result);\n");
            } else {
                printf("    // Unsupported return type: %s\n", return_type);
            }
            printf("    return 1;\n");
        } else {
            printf("    return 0;\n");
        }
        printf("}\n\n");
    }

    // Generate the Lua module initialization function
    printf("int luaopen_%s(lua_State* L) {\n", mod->name);
    printf("    static const luaL_Reg funcs[] = {\n");
    for (size_t i = 0; i < mod->functions_count; i++) {
        printf("        {\"%s\", l_%s},\n", mod->functions[i]->name, mod->functions[i]->name);
    }
    for (size_t i = 0; i < mod->records_count; i++) {
        printf("        {\"new_%s\", l_new_%s},\n", mod->records[i]->name, mod->records[i]->name);
    }
    printf("        {NULL, NULL}\n");
    printf("    };\n");
    printf("    luaL_register(L, \"%s\", funcs);\n", mod->name);
    // Register metatables for records
    for (size_t i = 0; i < mod->records_count; i++) {
        printf("    luaL_newmetatable(L, \"%s\");\n", mod->records[i]->name);
        printf("    lua_pop(L, 1);\n");
    }
    printf("    return 1;\n");
    printf("}\n");
}
