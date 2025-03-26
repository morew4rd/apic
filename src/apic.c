#include "apic.h"

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

// --- Lua Binding Generation Helpers ---
const char* map_type_to_lua(const Module* mod, const char* type_name) {
    // Check for enum first
    if (find_enum(mod, type_name)) return "enum";

    // Primitive types
    if (strcmp(type_name, INT32) == 0) return "integer";
    if (strcmp(type_name, INT64) == 0) return "integer";
    if (strcmp(type_name, FLOAT) == 0) return "number";
    if (strcmp(type_name, DOUBLE) == 0) return "number";
    if (strcmp(type_name, ISIZE) == 0) return "integer";
    if (strcmp(type_name, USIZE) == 0) return "integer";
    if (strcmp(type_name, BYTE) == 0) return "integer";
    if (strcmp(type_name, STRING) == 0) return "string";
    if (strcmp(type_name, VOID) == 0) return "none";

    // Check for other complex types
    if (find_record(mod, type_name)) return "userdata";
    if (find_variant(mod, type_name)) return "userdata";
    return "userdata";
}

void generate_lua_bindings(const Module* mod) {
    printf("// Lua 5.1 Bindings for %s\n", mod->name);
    printf("#include <string.h>\n");
    printf("#include <lua.h>\n#include <lauxlib.h>\n#include <lualib.h>\n\n");

    // Generate forward declarations for wrapper functions
    for (size_t i = 0; i < mod->functions_count; i++) {
        const Function* func = mod->functions[i];
        printf("static int lua_%s(lua_State *L);\n", func->name);
    }

    // Generate constructor functions for records
    for (size_t i = 0; i < mod->records_count; i++) {
        const Record* rec = mod->records[i];
        printf("static int lua_create_%s(lua_State *L);\n", rec->name);
    }

    printf("\n// Metatable registration\n");
    printf("static void register_metatables(lua_State *L) {\n");

    // Register records
    for (size_t i = 0; i < mod->records_count; i++) {
        printf("    // Metatable for %s\n", mod->records[i]->name);
        printf("    luaL_newmetatable(L, \"%s\");\n", mod->records[i]->name);
        printf("    lua_pop(L, 1);\n\n");
    }

    // Register variants
    for (size_t i = 0; i < mod->variants_count; i++) {
        printf("    // Metatable for %s\n", mod->variants[i]->name);
        printf("    luaL_newmetatable(L, \"%s\");\n", mod->variants[i]->name);
        printf("    lua_pop(L, 1);\n\n");
    }

    printf("}\n\n");

    // Generate constructor implementations
    for (size_t i = 0; i < mod->records_count; i++) {
        const Record* rec = mod->records[i];
        printf("static int lua_create_%s(lua_State *L) {\n", rec->name);
        printf("    // Create new %s object\n", rec->name);
        printf("    %s* obj = (%s*)lua_newuserdata(L, sizeof(%s));\n",
              rec->name, rec->name, rec->name);
        printf("    luaL_getmetatable(L, \"%s\");\n", rec->name);
        printf("    lua_setmetatable(L, -2);\n\n");

        // Initialize fields from Lua stack
        for (size_t j = 0; j < rec->fields_count; j++) {
            const Field* f = &rec->fields[j];
            printf("    // %s: %s\n", f->name, f->doc ? f->doc : "");
            printf("    obj->%s = ", f->name);

            if (strcmp(f->type, FLOAT) == 0 || strcmp(f->type, DOUBLE) == 0) {
                printf("luaL_checknumber(L, %zu);\n", j+1);
            }
            else if (strcmp(f->type, INT32) == 0 || strcmp(f->type, INT64) == 0) {
                printf("luaL_checkinteger(L, %zu);\n", j+1);
            }
            else if (strcmp(f->type, STRING) == 0) {
                printf("luaL_checkstring(L, %zu);\n", j+1);
            }
            else {
                printf("*(%s*)luaL_checkudata(L, %zu, \"%s\");\n",
                      f->type, j+1, f->type);
            }
        }

        printf("    return 1;\n");
        printf("}\n\n");
    }

    // Generate wrapper implementations
    for (size_t i = 0; i < mod->functions_count; i++) {
        const Function* func = mod->functions[i];
        printf("static int lua_%s(lua_State *L) {\n", func->name);

        // Argument checking
        printf("    // %s: %s\n", func->name, func->doc ? func->doc : "");
        printf("    if (lua_gettop(L) != %zu) {\n", func->inputs_count);
        printf("        return luaL_error(L, \"%s expects %zu arguments\");\n    }\n\n",
              func->name, func->inputs_count);

        // Input processing
        for (size_t j = 0; j < func->inputs_count; j++) {
            const Arg* arg = &func->inputs[j];
            const Enum* en = find_enum(mod, arg->type);

            printf("    // %s: %s\n", arg->name, arg->doc ? arg->doc : "");

            if (en) {
                // Enum string handling
                printf("    const char* %s_str = luaL_checkstring(L, %zu);\n", arg->name, j+1);
                printf("    int %s;\n", arg->name);

                // Generate string comparisons
                for (size_t k = 0; k < en->fields_count; k++) {
                    const EnumField* ef = &en->fields[k];
                    const char* cmp = (k == 0) ? "if" : "else if";
                    printf("    %s (strcmp(%s_str, \"%s\") == 0) {\n", cmp, arg->name, ef->str_value);
                    printf("        %s = %d;\n", arg->name, ef->value);
                    printf("    }\n");
                }
                printf("    else {\n");
                printf("        return luaL_error(L, \"Invalid %s value: %%s\", %s_str);\n", en->name, arg->name);
                printf("    }\n");
            }
            else if (strcmp(arg->type, INT32) == 0 || strcmp(arg->type, INT64) == 0) {
                printf("    lua_Integer %s = luaL_checkinteger(L, %zu);\n", arg->name, j+1);
            }
            else if (strcmp(arg->type, FLOAT) == 0 || strcmp(arg->type, DOUBLE) == 0) {
                printf("    lua_Number %s = luaL_checknumber(L, %zu);\n", arg->name, j+1);
            }
            else if (strcmp(arg->type, STRING) == 0) {
                printf("    const char* %s = luaL_checkstring(L, %zu);\n", arg->name, j+1);
            }
            else {
                printf("    %s* %s = (%s*)luaL_checkudata(L, %zu, \"%s\");\n",
                      arg->type, arg->name, arg->type, j+1, arg->type);
            }
        }
        printf("\n");

        // Function call
        const Enum* return_enum = find_enum(mod, func->output->type);
        if (strcmp(func->output->type, VOID) != 0) {
            printf("    %s result = %s(",
                  map_type_to_c(mod, func->output->type),
                  func->name);
        } else {
            printf("    %s(", func->name);
        }

        for (size_t j = 0; j < func->inputs_count; j++) {
            const Arg* arg = &func->inputs[j];
            printf("%s%s", arg->name, j < func->inputs_count-1 ? ", " : "");
        }
        printf(");\n\n");

        // Return handling
        if (strcmp(func->output->type, VOID) != 0) {
            printf("    // Return: %s\n", func->output->doc ? func->output->doc : "");

            if (return_enum) {
                // Convert enum to string
                printf("    const char* result_str = \"\";\n");
                printf("    switch(result) {\n");
                for (size_t k = 0; k < return_enum->fields_count; k++) {
                    const EnumField* ef = &return_enum->fields[k];
                    printf("        case %d: result_str = \"%s\"; break;\n",
                          ef->value, ef->str_value);
                }
                printf("        default: return luaL_error(L, \"Invalid %s value\");\n", return_enum->name);
                printf("    }\n");
                printf("    lua_pushstring(L, result_str);\n");
            }
            else if (strcmp(func->output->type, INT32) == 0 ||
                    strcmp(func->output->type, INT64) == 0) {
                printf("    lua_pushinteger(L, result);\n");
            }
            else if (strcmp(func->output->type, FLOAT) == 0 ||
                    strcmp(func->output->type, DOUBLE) == 0) {
                printf("    lua_pushnumber(L, result);\n");
            }
            else if (strcmp(func->output->type, STRING) == 0) {
                printf("    lua_pushstring(L, result);\n");
            }
            else {
                printf("    // Push userdata\n");
                printf("    %s* result_ptr = (%s*)lua_newuserdata(L, sizeof(%s));\n",
                      func->output->type, func->output->type, func->output->type);
                printf("    *result_ptr = result;\n");
                printf("    luaL_getmetatable(L, \"%s\");\n", func->output->type);
                printf("    lua_setmetatable(L, -2);\n");
            }
            printf("    return 1;\n");
        } else {
            printf("    return 0;\n");
        }

        printf("}\n\n");
    }

    // Module registration
    printf("int luaopen_%s(lua_State *L) {\n", mod->name);
    printf("    register_metatables(L);\n\n");
    printf("    static const luaL_Reg funcs[] = {\n");

    // Add constructor functions
    for (size_t i = 0; i < mod->records_count; i++) {
        printf("        {\"create_%s\", lua_create_%s},\n",
              mod->records[i]->name, mod->records[i]->name);
    }

    // Add regular functions
    for (size_t i = 0; i < mod->functions_count; i++) {
        const Function* func = mod->functions[i];
        printf("        {\"%s\", lua_%s},\n", func->name, func->name);
    }

    printf("        {NULL, NULL}\n");
    printf("    };\n\n");

    // Add enum values
    for (size_t i = 0; i < mod->enums_count; i++) {
        const Enum* en = mod->enums[i];
        printf("    // %s enum values\n", en->name);
        for (size_t j = 0; j < en->fields_count; j++) {
            const EnumField* ef = &en->fields[j];
            printf("    lua_pushstring(L, \"%s\");\n", ef->str_value);
            printf("    lua_setfield(L, -2, \"%s\");\n", ef->name);
        }
        printf("\n");
    }

    printf("    luaL_register(L, \"%s\", funcs);\n", mod->name);
    printf("    return 1;\n}\n");
}
