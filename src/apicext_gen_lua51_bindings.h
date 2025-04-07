// ================ File: src/apicext_gen_lua51_bindings.h ================
#ifndef APICEXT_GEN_LUA51_BINDINGS_H
#define APICEXT_GEN_LUA51_BINDINGS_H

#ifdef APIC_REFLECT

#include "apic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Ensure bool is available

// Lua headers - These must be available when compiling the generated code
// #include <lua.h>
// #include <lauxlib.h>
// #include <lualib.h>

// Forward declaration for helper from C header gen
static void type_to_str_recursive(Type* type, char* buf, size_t size);

// --- Configuration ---
#define LUA_METATABLE_PREFIX "apic.meta." // Prefix for metatable names (for Lua handle tables)
#define LUA_HANDLE_ID_FIELD "_id"         // Field in Lua table storing lightuserdata

// --- Helper Functions ---

// Get the full Lua metatable name for the *handle table* of a struct/union type
static void get_lua_handle_metatable_name(Type* type, const char* module_name, char* buf, size_t size) {
    const char* base_name = NULL;
    if (!type || !buf || size == 0 || !module_name) {
        if (buf && size > 0) buf[0] = '\0';
        return;
    }

    while (type && type->kind == TK_TYPEDEF) {
        type = type->data.typedef_target;
    }
    if (!type) { buf[0] = '\0'; return; }

    if (type->kind == TK_STRUCT && type->data.struct_type) {
        base_name = type->data.struct_type->name;
    } else if (type->kind == TK_UNION && type->data.union_type) {
        base_name = type->data.union_type->name;
    } else if (type->name) {
        base_name = type->name;
    }

    if (base_name) {
        snprintf(buf, size, "%s%s.%s", LUA_METATABLE_PREFIX, module_name, base_name);
    } else {
        buf[0] = '\0';
    }
}

// Check if type is wrappable for functions (Primitives, Enums, const char*, T*)
static int c_type_is_function_wrappable(Type* type) {
    if (!type) return 0;

    while (type->kind == TK_TYPEDEF) {
        type = type->data.typedef_target;
        if (!type) return 0;
    }

    switch (type->kind) {
        case TK_PRIMITIVE: // Allow supported primitives
             switch (type->data.primitive) { /* Primitive cases */
                 case PT_VOID: case PT_CHAR: case PT_UCHAR: case PT_SCHAR:
                 case PT_SHORT: case PT_USHORT: case PT_INT: case PT_UINT:
                 case PT_LONG: case PT_ULONG: case PT_FLOAT: case PT_DOUBLE:
                 case PT_BOOL: case PT_CSTRING: return 1;
                 default: return 0;
             }
        case TK_ENUM: return 1;
        case TK_POINTER: { // Allow specific pointers
            Type* base = type->data.pointer.base_type;
            if (!base) return 0;
            while (base->kind == TK_TYPEDEF) { base = base->data.typedef_target; if (!base) return 0;}

            if (base->kind == TK_PRIMITIVE && base->data.primitive == PT_CHAR) return 1; // char* / const char*
            if (base->kind == TK_STRUCT || base->kind == TK_UNION) {
                // Check definition exists (not forward decl) to ensure metatable can be found
                 if ((base->kind == TK_STRUCT && base->data.struct_type != NULL) ||
                     (base->kind == TK_UNION && base->data.union_type != NULL)) {
                      return 1; // T* is allowed
                 }
            }
            return 0; // Other pointers (int*, void**) disallowed
        }
        // Structs/Unions BY VALUE are DISALLOWED in lightuserdata approach
        case TK_STRUCT:
        case TK_UNION:
            return 0; // Cannot pass/return T by value

        // Other types disallowed for now
        case TK_ARRAY: case TK_FUNCTION: case TK_FUNCPTR: case TK_UNRESOLVED: default: return 0;
    }
}

// Generate C code to check Lua value at 'index', expecting a handle table for T*,
// extract the lightuserdata, and assign the T* pointer to 'var_name'.
static void generate_lua_check_handle_code(FILE* out, Type* ptr_type, int index, const char* var_name, const char* func_name, const char* module_name) {
     if (!ptr_type || ptr_type->kind != TK_POINTER || !ptr_type->data.pointer.base_type) {
         fprintf(out, "    luaL_error(L, \"Internal error: Invalid pointer type for handle check in %s\");\n", func_name);
         return;
     }
     Type* base_type = ptr_type->data.pointer.base_type;
     char c_ptr_type_str[256]; // e.g., "struct Vec2I*"
     char base_c_type_str[256]; // e.g., "struct Vec2I"
     char meta_name[256];

     type_to_str_recursive(ptr_type, c_ptr_type_str, sizeof(c_ptr_type_str));
     type_to_str_recursive(base_type, base_c_type_str, sizeof(base_c_type_str));
     get_lua_handle_metatable_name(base_type, module_name, meta_name, sizeof(meta_name));

      if (meta_name[0] == '\0' || c_ptr_type_str[0] == '\0') {
           fprintf(out, "    luaL_error(L, \"Internal error: Cannot check handle for type %%s in %s (metatable/typename missing)\", \"%s\");\n", func_name, base_type->name ? base_type->name : "unknown");
           return;
      }

     fprintf(out, "    // Check for handle table for %s\n", c_ptr_type_str);
     fprintf(out, "    if (lua_type(L, %d) != LUA_TTABLE) { luaL_argerror(L, %d, \"Expected handle table for %s\"); }\n", index, index, base_type->name ? base_type->name : "type");
     // Check metatable
     fprintf(out, "    if (!lua_getmetatable(L, %d)) { luaL_argerror(L, %d, \"Handle table missing metatable\"); }\n", index, index); // Stack: ..., handle_table, meta
     fprintf(out, "    luaL_getmetatable(L, \"%s\"); // Stack: ..., handle_table, meta, expected_meta\n", meta_name);
     fprintf(out, "    if (!lua_rawequal(L, -1, -2)) { lua_pop(L, 2); luaL_argerror(L, %d, \"Incorrect handle type (expected %s)\"); }\n", index, meta_name);
     fprintf(out, "    lua_pop(L, 2); // Pop both metatables\n");
     // Get the _id field (lightuserdata)
     fprintf(out, "    lua_getfield(L, %d, \"%s\"); // Stack: ..., handle_table, lightuserdata\n", index, LUA_HANDLE_ID_FIELD);
     fprintf(out, "    if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) { lua_pop(L, 1); luaL_argerror(L, %d, \"Handle table '_id' field is not lightuserdata\"); }\n", index);
     fprintf(out, "    %s %s = (%s)lua_touserdata(L, -1);\n", c_ptr_type_str, var_name, c_ptr_type_str);
     fprintf(out, "    lua_pop(L, 1); // Pop lightuserdata\n");
     // Optional: Add NULL check? Depends if NULL pointers should be allowed.
     // fprintf(out, "    if (%s == NULL) { luaL_argerror(L, %d, \"Handle points to NULL\"); }\n", var_name, index);
}


// Generate C code to check a Lua value at 'index' and assign it to C variable 'var_name'.
static void generate_lua_check_code(FILE* out, Type* type, int index, const char* var_name, const char* func_name, const char* module_name) {
    // ... (Handling for primitives, enums, char*, const char* remains largely the same as previous full userdata version) ...
    // ... (ensure generate_lua_check_code includes the necessary cases from the previous version) ...
     if (!type) { /* ... error ... */ return; }
    char c_type_str[256];
    type_to_str_recursive(type, c_type_str, sizeof(c_type_str));
    Type* check_type = type;
    while (check_type && check_type->kind == TK_TYPEDEF) { check_type = check_type->data.typedef_target; }
     if (!check_type) { /* ... error ... */ return; }

    switch (check_type->kind) {
        case TK_ENUM: // Enums as integers
        case TK_PRIMITIVE:
            switch (check_type->data.primitive) {
                //  case PT_CHAR: case PT_SCHAR: case PT_UCHAR: case PT_SHORT: case PT_USHORT: case PT_INT: case PT_UINT: case PT_LONG: case PT_ULONG:
                //     fprintf(out, "    %s %s = (%s)luaL_checkinteger(L, %d);\n", c_type_str, var_name, c_type_str, index); break;
                 case PT_FLOAT: case PT_DOUBLE:
                    fprintf(out, "    %s %s = (%s)luaL_checknumber(L, %d);\n", c_type_str, var_name, c_type_str, index); break;
                 case PT_BOOL:
                    fprintf(out, "    if (!lua_isboolean(L, %d)) { luaL_argerror(L, %d, \"boolean expected\"); }\n", index, index);
                    fprintf(out, "    %s %s = (%s)lua_toboolean(L, %d);\n", c_type_str, var_name, c_type_str, index); break;
                 case PT_CSTRING: // const char*
                 case PT_CHAR: // Also handle non-const char* as input string
                      if (type->kind == TK_POINTER && type->data.pointer.base_type->kind == TK_PRIMITIVE && type->data.pointer.base_type->data.primitive == PT_CHAR) {
                          // This specific case was handled under TK_POINTER below, reusing that logic
                           fprintf(out, "    %s %s = (%s)luaL_checkstring(L, %d);\n", c_type_str, var_name, c_type_str, index);
                      } else { // Assuming PT_CSTRING was resolved directly
                           fprintf(out, "    %s %s = (%s)luaL_checkstring(L, %d);\n", c_type_str, var_name, c_type_str, index);
                      }
                     break;
                default:
                     fprintf(out, "    luaL_error(L, \"Unsupported primitive type '%%s' for argument %%d in %%s\", \"%s\", %d, \"%s\");\n", c_type_str, index, func_name); break;
            }
            break;

        case TK_POINTER: {
            Type* base = check_type->data.pointer.base_type;
            if (!base) { /* Error */ fprintf(out, "        luaL_error(L, \"Internal error: Pointer with NULL base type for arg %%d in %%s\", %d, \"%s\");\n", index, func_name); break; }
             while (base && base->kind == TK_TYPEDEF) { base = base->data.typedef_target; }
             if (!base) { /* Error */ fprintf(out, "        luaL_error(L, \"Internal error: Invalid pointer base type after typedef resolution for arg %%d in %%s\", %d, \"%s\");\n", index, func_name); break;}

            if (base->kind == TK_PRIMITIVE && base->data.primitive == PT_CHAR) {
                // Allow char* and const char*
                fprintf(out, "    %s %s = (%s)luaL_checkstring(L, %d);\n", c_type_str, var_name, c_type_str, index);
            } else if (base->kind == TK_STRUCT || base->kind == TK_UNION) {
                // Use the dedicated handle checking function
                 generate_lua_check_handle_code(out, check_type, index, var_name, func_name, module_name);
             } else {
                fprintf(out, "    luaL_error(L, \"Unsupported pointer type '%%s' for argument %%d in %%s\", \"%s\", %d, \"%s\");\n", c_type_str, index, func_name);
            }
            break;
        }
         // Struct/Union by value is NOT supported
         case TK_STRUCT: case TK_UNION: default:
            fprintf(out, "    luaL_error(L, \"Unsupported type '%%s' (kind %%d) for argument %%d in %%s\", \"%s\", %d, %d, \"%s\");\n", c_type_str, check_type->kind, index, func_name);
            break;
    }
}

// Generate C code to push C variable 'var_name' of type 'type' onto Lua stack.
// For T*, creates a handle table with lightuserdata.
// Returns number of Lua values pushed.
static int generate_lua_push_code(FILE* out, Type* type, const char* var_name, const char* module_name) {
    // ... (Handling for primitives, enums, char*, const char* remains largely the same) ...
    // ... (ensure generate_lua_push_code includes the necessary cases from the previous version) ...
     if (!type) { /* ... error ... */ fprintf(out, "    lua_pushnil(L);\n"); return 1; }
     Type* push_type = type;
     while (push_type && push_type->kind == TK_TYPEDEF) { push_type = push_type->data.typedef_target; }
     if (!push_type) { /* ... error ... */ fprintf(out, "    lua_pushnil(L);\n"); return 1; }

     switch (push_type->kind) {
        case TK_PRIMITIVE:
            switch (push_type->data.primitive) {
                case PT_VOID: return 0;
                case PT_CHAR: case PT_SCHAR: case PT_UCHAR: case PT_SHORT: case PT_USHORT: case PT_INT: case PT_UINT: case PT_LONG: case PT_ULONG:
                     fprintf(out, "    lua_pushinteger(L, (lua_Integer)(%s));\n", var_name); return 1;
                case PT_FLOAT: case PT_DOUBLE:
                     fprintf(out, "    lua_pushnumber(L, (lua_Number)(%s));\n", var_name); return 1;
                case PT_BOOL:
                     fprintf(out, "    lua_pushboolean(L, (int)(%s));\n", var_name); return 1;
                 case PT_CSTRING: // const char*
                      // Handle NULL C strings
                      fprintf(out, "    if (%s == NULL) { lua_pushnil(L); } else { lua_pushstring(L, %s); }\n", var_name, var_name); return 1;
                 default: fprintf(out, "    /* Unsupported primitive */ lua_pushnil(L);\n"); return 1;
             }
             break; // Should not be reached

        case TK_ENUM:
             fprintf(out, "    lua_pushinteger(L, (lua_Integer)(%s));\n", var_name); return 1;

        case TK_POINTER: {
             Type* base = push_type->data.pointer.base_type;
              if (!base) { /* Error */ fprintf(out, "    /* Error: Pointer base NULL */ lua_pushnil(L);\n"); return 1; }
              while (base && base->kind == TK_TYPEDEF) { base = base->data.typedef_target; }
              if (!base) { /* Error */ fprintf(out, "    /* Error: Invalid base */ lua_pushnil(L);\n"); return 1;}

             if (base->kind == TK_PRIMITIVE && base->data.primitive == PT_CHAR) {
                  // Assuming char* or const char* points to a C string
                   fprintf(out, "    if (%s == NULL) { lua_pushnil(L); } else { lua_pushstring(L, %s); }\n", var_name, var_name); return 1;
             } else if (base->kind == TK_STRUCT || base->kind == TK_UNION) {
                  // Create handle table wrapping lightuserdata
                  char meta_name[256];
                  char base_type_str[256];
                  type_to_str_recursive(base, base_type_str, sizeof(base_type_str));
                  get_lua_handle_metatable_name(base, module_name, meta_name, sizeof(meta_name));
                   if (meta_name[0] == '\0') {
                        fprintf(out, "    /* Error: Cannot find meta for %s* */ lua_pushnil(L);\n", base_type_str); return 1;
                   }
                  fprintf(out, "    // Push %s* handle\n", base_type_str);
                  // Handle NULL C pointer case
                   fprintf(out, "    if (%s == NULL) {\n        lua_pushnil(L);\n    } else {\n", var_name);
                   // Create table: {}
                   fprintf(out, "        lua_newtable(L); // Stack: handle_table\n");
                   // Push lightuserdata: lightuserdata(var_name)
                   fprintf(out, "        lua_pushlightuserdata(L, (void*)(%s)); // Stack: handle_table, lightud\n", var_name);
                   // Set table._id = lightuserdata: handle_table
                   fprintf(out, "        lua_setfield(L, -2, \"%s\"); // Stack: handle_table\n", LUA_HANDLE_ID_FIELD);
                   // Set metatable: handle_table
                   fprintf(out, "        luaL_getmetatable(L, \"%s\"); // Stack: handle_table, meta\n", meta_name);
                   fprintf(out, "        if (lua_isnil(L, -1)) { /* Meta not found! */ lua_pop(L, 2); lua_pushnil(L); } else { lua_setmetatable(L, -2); }\n"); // Stack: handle_table
                   fprintf(out, "    }\n");
                   return 1;
              } else {
                   fprintf(out, "    /* Unsupported pointer type %s* */ lua_pushnil(L);\n", base->name ? base->name : "unknown"); return 1;
              }
        }
         // Struct/Union by value is NOT supported
         case TK_STRUCT: case TK_UNION: default:
             fprintf(out, "    /* Unsupported type %s */ lua_pushnil(L);\n", push_type->name ? push_type->name : "unknown"); return 1;
    }
}

// --- Struct/Union Metamethod Implementations (Operating on Handle Tables) ---

static void generate_struct_union_metatable(FILE* out, Type* type, const char* module_name) {
    // ... (Initial checks for type, module_name, names remain similar) ...
     if (!type || !module_name || (type->kind != TK_STRUCT && type->kind != TK_UNION)) return;
     if ((type->kind == TK_STRUCT && !type->data.struct_type) || (type->kind == TK_UNION && !type->data.union_type)) { /* ... skip ... */ return; }
     // ... (Get meta_name, c_type_str, type_kind_str, type_name, fields, num_fields) ...
    char meta_name[256]; char c_type_str[256]; // Use base type for C name
    const char* type_kind_str = (type->kind == TK_STRUCT) ? "struct" : "union";
    const char* type_name = (type->kind == TK_STRUCT) ? type->data.struct_type->name : type->data.union_type->name;
    apic_TypedField* fields = (type->kind == TK_STRUCT) ? type->data.struct_type->fields : type->data.union_type->fields;
    size_t num_fields = (type->kind == TK_STRUCT) ? type->data.struct_type->num_fields : type->data.union_type->num_fields;
     if (!type_name) { /* ... skip ... */ return; }
    get_lua_handle_metatable_name(type, module_name, meta_name, sizeof(meta_name));
    type_to_str_recursive(type, c_type_str, sizeof(c_type_str)); // e.g., "struct Vec2I"
     if (meta_name[0] == '\0' || c_type_str[0] == '\0') { /* ... skip ... */ return; }


    fprintf(out, "// --- Metatable for %s %s Handle (%s) ---\n\n", type_kind_str, type_name, meta_name);

    // Helper to get T* pointer from handle table at index 1
    const char* get_ptr_helper = "__apic_get_ptr";
    fprintf(out, "// Helper to safely get C pointer from handle table\n");
    fprintf(out, "static %s* %s_%s_%s(lua_State *L, const char* meta_name) {\n", c_type_str, module_name, type_name, get_ptr_helper);
    fprintf(out, "    // Assumes arg 1 is the handle table already validated\n");
    fprintf(out, "    lua_getfield(L, 1, \"%s\");\n", LUA_HANDLE_ID_FIELD);
    fprintf(out, "    if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) {\n");
    fprintf(out, "        lua_pop(L, 1);\n");
    fprintf(out, "        luaL_error(L, \"Handle table for %%s missing or invalid '_id' field\", meta_name);\n");
    fprintf(out, "        return NULL; // Should not be reached\n");
    fprintf(out, "    }\n");
    fprintf(out, "    %s* ptr = (%s*)lua_touserdata(L, -1);\n", c_type_str, c_type_str);
    fprintf(out, "    lua_pop(L, 1);\n");
    fprintf(out, "    if (ptr == NULL) {\n");
    fprintf(out, "        luaL_error(L, \"Handle table for %%s '_id' points to NULL\", meta_name);\n");
    fprintf(out, "        // return NULL; // Error throws\n");
    fprintf(out, "    }\n");
    fprintf(out, "    return ptr;\n");
    fprintf(out, "}\n\n");


    // --- __index metamethod ---
    fprintf(out, "static int %s_%s__index(lua_State *L) {\n", module_name, type_name);
    fprintf(out, "    // Arg 1: handle table, Arg 2: key\n");
    fprintf(out, "    const char* key = luaL_checkstring(L, 2);\n");
    fprintf(out, "    %s* self = %s_%s_%s(L, \"%s\"); // Get C pointer\n", c_type_str, module_name, type_name, get_ptr_helper, meta_name);
    fprintf(out, "    if (!self) return 0; // Error handled by helper\n\n");

    // Field Access (Structs and Unions - read carefully for unions)
     if (type->kind == TK_UNION) fprintf(out, "    // Reading union fields - Use with caution!\n");
     for (size_t i = 0; i < num_fields; ++i) {
         apic_TypedField* field = &fields[i];
         if (!field || !field->name || !field->type) continue;
         Type* field_type = field->type;

         fprintf(out, "    if (strcmp(key, \"%s\") == 0) {\n", field->name);
         char field_var[256];
         snprintf(field_var, sizeof(field_var), "self->%s", field->name);

         if (field_type->kind == TK_ARRAY) {
             fprintf(out, "        luaL_error(L, \"Direct access to array field '%s' not implemented.\");\n", field->name);
         } else {
             // Push primitive, enum, or handle table for pointer T*
             generate_lua_push_code(out, field_type, field_var, module_name);
         }
         fprintf(out, "        return 1;\n");
         fprintf(out, "    }\n");
     }

    // Fallback to Metatable Methods
    fprintf(out, "\n    // Fallback: Check for methods in metatable\n");
    fprintf(out, "    lua_getmetatable(L, 1);\n"); // Stack: meta
    fprintf(out, "    lua_pushvalue(L, 2);\n");    // Stack: meta, key
    fprintf(out, "    lua_rawget(L, -2);\n");     // Stack: meta, method_or_nil
    fprintf(out, "    lua_remove(L, -2);\n");      // Stack: method_or_nil
    fprintf(out, "    if (!lua_isnil(L, -1)) {\n");
    fprintf(out, "        return 1; // Return method\n");
    fprintf(out, "    } else {\n");
    // Escaped %s
    fprintf(out, "        return luaL_error(L, \"No such field or method '%%s' in %s %%s handle\", key, \"%s\", \"%s\");\n", type_kind_str, type_name);
    fprintf(out, "    }\n");
    fprintf(out, "}\n\n");

    // --- __newindex metamethod ---
    fprintf(out, "static int %s_%s__newindex(lua_State *L) {\n", module_name, type_name);
    fprintf(out, "    // Arg 1: handle table, Arg 2: key, Arg 3: value\n");
    fprintf(out, "    const char* key = luaL_checkstring(L, 2);\n");
    fprintf(out, "    %s* self = %s_%s_%s(L, \"%s\"); // Get C pointer\n", c_type_str, module_name, type_name, get_ptr_helper, meta_name);
    fprintf(out, "    if (!self) return 0; // Error handled by helper\n\n");

     // Field Assignment (Structs and Unions - write carefully for unions)
     if (type->kind == TK_UNION) fprintf(out, "    // Assigning union fields - Use with caution!\n");
     for (size_t i = 0; i < num_fields; ++i) {
        apic_TypedField* field = &fields[i];
         if (!field || !field->name || !field->type) continue;
        Type* field_type = field->type;

        fprintf(out, "    if (strcmp(key, \"%s\") == 0) {\n", field->name);
        char field_name_str[128];
        snprintf(field_name_str, sizeof(field_name_str), "new_value_%s", field->name);

         if (field_type->kind == TK_ARRAY) {
             fprintf(out, "        return luaL_error(L, \"Direct assignment to array field '%s' not implemented.\");\n", field->name);
         } else {
              // Check value at index 3 and assign to self->field
              generate_lua_check_code(out, field_type, 3, field_name_str, "__newindex", module_name);
              fprintf(out, "        self->%s = %s;\n", field->name, field_name_str);
         }
        fprintf(out, "        return 0; // Assignment successful\n");
        fprintf(out, "    }\n");
     }

    // Escaped %s
    fprintf(out, "\n    return luaL_error(L, \"No such field '%%s' in %s %%s handle to assign to\", key, \"%s\", \"%s\");\n", type_kind_str, type_name);
    fprintf(out, "}\n\n");

    // --- __tostring metamethod ---
    fprintf(out, "static int %s_%s__tostring(lua_State *L) {\n", module_name, type_name);
    fprintf(out, "    // Arg 1: handle table\n");
    fprintf(out, "    %s* self = %s_%s_%s(L, \"%s\"); // Get C pointer\n", c_type_str, module_name, type_name, get_ptr_helper, meta_name);
    fprintf(out, "    if (!self) { lua_pushstring(L, \"Invalid handle\"); return 1; } // Handle NULL case gracefully?\n");
    // Escape %p
    fprintf(out, "    lua_pushfstring(L, \"%s %s Handle: %%p\", (void*)self);\n", type_kind_str, type_name);
    fprintf(out, "    return 1;\n");
    fprintf(out, "}\n\n");

    // --- __gc metamethod ---
    fprintf(out, "// User must define %s_release( %s* obj ) if GC cleanup is needed.\n", type_name, c_type_str);
    fprintf(out, "// Example: void %s_release(%s* obj) { printf(\"Releasing %s %%p\\n\", (void*)obj); free(obj); }\n", type_name, c_type_str, type_name);
    // Generate a weak definition if possible, or just the call site.
    fprintf(out, "// extern void %s_release(%s* obj); // Weak link? Or just call and let linker find it.\n\n", type_name, c_type_str);
    fprintf(out, "static int %s_%s__gc(lua_State *L) {\n", module_name, type_name);
    fprintf(out, "    // Arg 1: handle table being collected\n");
    fprintf(out, "    printf(\"GC triggered for %s handle.\\n\"); // Debug\n", type_name);
    fprintf(out, "    lua_getfield(L, 1, \"%s\");\n", LUA_HANDLE_ID_FIELD);
    fprintf(out, "    if (lua_type(L, -1) == LUA_TLIGHTUSERDATA) {\n");
    fprintf(out, "        %s* ptr = (%s*)lua_touserdata(L, -1);\n", c_type_str, c_type_str);
    fprintf(out, "        lua_pop(L, 1);\n"); // Pop lightuserdata
    fprintf(out, "        if (ptr != NULL) {\n");
    fprintf(out, "            printf(\"  >> GC: C Pointer is %%p\\n\", (void*)ptr);//mg:%s\n", type_name);
    fprintf(out, "            // --- Call user-defined C cleanup function --- \n");
    fprintf(out, "            // %s_release(ptr); // Uncomment this line or provide your cleanup call \n", type_name);
    fprintf(out, "            printf(\"  >> GC: Object cleanup function '%s_release' should be called here.\\n\");\n", type_name);
    fprintf(out, "            // Important: Set handle's _id to NULL after cleanup to prevent double-free/use-after-free\n");
    fprintf(out, "            lua_pushnil(L);\n");
    fprintf(out, "            lua_setfield(L, 1, \"%s\");\n", LUA_HANDLE_ID_FIELD);
    fprintf(out, "        } else {\n");
    fprintf(out, "             printf(\"  >> GC: C Pointer was NULL.\\n\");\n");
    fprintf(out, "        }\n");
    fprintf(out, "    } else {\n");
    fprintf(out, "        printf(\"  >> GC: Handle table '_id' not lightuserdata!\\n\");\n");
    fprintf(out, "        lua_pop(L, 1);\n"); // Pop invalid _id field
    fprintf(out, "    }\n");
    fprintf(out, "    return 0;\n");
    fprintf(out, "}\n\n");


    // --- Metatable Registration Array ---
    fprintf(out, "static const luaL_Reg %s_%s_meta[] = {\n", module_name, type_name);
    fprintf(out, "    {\"__index\", %s_%s__index},\n", module_name, type_name);
    fprintf(out, "    {\"__newindex\", %s_%s__newindex},\n", module_name, type_name);
    fprintf(out, "    {\"__tostring\", %s_%s__tostring},\n", module_name, type_name);
    fprintf(out, "    {\"__gc\", %s_%s__gc}, // Add GC hook\n",  module_name, type_name);
    // Add methods here if any, e.g. {"methodName", lua_TypeName_methodName}
    fprintf(out, "    {NULL, NULL}\n");
    fprintf(out, "};\n\n");


    fprintf(out, "// --- End Metatable for %s %s Handle ---\n\n", type_kind_str, type_name);
}

// --- Main Generation Function ---

static void __impl_gen_lua51_bindings(apic_TypedExports *tex, FILE *out, const char* module_name, const char* public_header_path) {
    // ... (Initial checks and File Header remain the same) ...
     if (!tex || !out || !module_name || !public_header_path) { /*...*/ return; }
     if (tex->context.error_count > 0) { /*...*/ }

    fprintf(out, "// ==========================================================\n");
    fprintf(out, "// Lua 5.1 Bindings for %s\n", tex->name);
    fprintf(out, "// Generated by APIC reflection system (Light Userdata Handle Version)\n");
    fprintf(out, "// ==========================================================\n\n");
    // ... (Includes: lua, laux, lualib, string, stdio, stdbool) ...
    fprintf(out, "#include <lua.h>\n");
    fprintf(out, "#include <lauxlib.h>\n");
    fprintf(out, "#include <lualib.h>\n");
    fprintf(out, "#include <string.h>\n");
    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <stdbool.h>\n");
    fprintf(out, "\n#include \"%s\"\n\n", public_header_path);

    // --- Generate Metatable Code for Structs/Unions FIRST ---
     fprintf(out, "// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
     fprintf(out, "//         HANDLE TABLE METATABLE DEFINITIONS\n");
     fprintf(out, "// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
    for (size_t i = 0; i < tex->num_structs; ++i) {
        if (!tex->structs[i] || !tex->structs[i]->self_type) continue;
         generate_struct_union_metatable(out, tex->structs[i]->self_type, module_name);
    }
     for (size_t i = 0; i < tex->num_unions; ++i) {
         if (!tex->unions[i] || !tex->unions[i]->self_type) continue;
         generate_struct_union_metatable(out, tex->unions[i]->self_type, module_name);
     }
     fprintf(out, "// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
     fprintf(out, "//               END METATABLE DEFINITIONS\n");
     fprintf(out, "// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

    // --- Generate Function Wrappers ---
    // ... (Function wrapper generation loop remains similar, but relies on the updated check/push code) ...
    fprintf(out, "// --- Wrapped C Functions ---\n");
    int generated_func_count = 0;
    for (size_t i = 0; i < tex->num_functions; i++) {
        // ... (Function definition and support checks using c_type_is_function_wrappable) ...
        apic_TypedFunction *fn = tex->functions[i];
         if (!fn || !fn->name) continue;
         int supported = 1; char unsupported_reason[256] = "";
          if (!c_type_is_function_wrappable(fn->return_type) && !(fn->return_type && fn->return_type->kind == TK_PRIMITIVE && fn->return_type->data.primitive == PT_VOID)) { /*...*/ supported = 0; }
          for (size_t j = 0; supported && j < fn->num_params; j++) { if (!fn->params || !fn->params[j].type || !c_type_is_function_wrappable(fn->params[j].type)) { /*...*/ supported = 0; break; } }
          if (!supported) { /*...*/ continue; }

        generated_func_count++;
        // ... (Generate function signature comment) ...
        fprintf(out, "// Wrapper for %s\n", fn->name); // Simplified comment
        fprintf(out, "static int lua_%s(lua_State *L) {\n", fn->name);
        // ... (Argument count check) ...
         fprintf(out, "    int n_args = lua_gettop(L);\n");
         fprintf(out, "    if (n_args != %zu) { return luaL_error(L, \"Function '%s' expects %zu arguments, got %%d\", n_args); }\n\n", fn->num_params, fn->name, fn->num_params);

        // Argument checking and retrieval
        for (size_t j = 0; j < fn->num_params; j++) {
            apic_TypedField *param = &fn->params[j];
             if (!param || !param->name || !param->type) continue;
            char arg_name[128]; snprintf(arg_name, sizeof(arg_name), "arg_%s", param->name);
            fprintf(out, "    // Arg %zu: %s\n", j + 1, param->name);
            generate_lua_check_code(out, param->type, (int)j + 1, arg_name, fn->name, module_name);
        }
        fprintf(out, "\n");

        // Call C function
        fprintf(out, "    // Call C function '%s'\n", fn->name);
        int num_return_values = 0; char ret_var_name[64] = "c_result";
        if (fn->return_type && !(fn->return_type->kind == TK_PRIMITIVE && fn->return_type->data.primitive == PT_VOID)) {
             char ret_type_str[256];
             if (fn->return_type) { type_to_str_recursive(fn->return_type, ret_type_str, sizeof(ret_type_str)); fprintf(out, "    %s %s = ", ret_type_str, ret_var_name); num_return_values = 1; }
             else { fprintf(out, "    // Error: NULL return type\n    "); } // Error case
        } else { fprintf(out, "    "); num_return_values = 0; }
        fprintf(out, "%s(", fn->name);
        for (size_t j = 0; j < fn->num_params; j++) { if (!fn->params || !fn->params[j].name) continue; fprintf(out, "arg_%s%s", fn->params[j].name, (j < fn->num_params - 1) ? ", " : ""); }
        fprintf(out, ");\n\n");

        // Push result
        int lua_returns = 0;
        if (num_return_values == 1 && fn->return_type) { lua_returns = generate_lua_push_code(out, fn->return_type, ret_var_name, module_name); }
        fprintf(out, "    return %d;\n", lua_returns);
        fprintf(out, "}\n\n");
    }
    fprintf(out, "// --- End Wrapped C Functions ---\n\n");

    // --- Generate luaL_Reg Array for Module Functions ---
    fprintf(out, "// --- Lua Module Function Registration Data ---\n");
    fprintf(out, "static const struct luaL_Reg %s_funcs[] = {\n", module_name);
    if (generated_func_count > 0) {
         for (size_t i = 0; i < tex->num_functions; i++) {
            // ... (Check support again and add entry if supported) ...
             apic_TypedFunction *fn = tex->functions[i]; if (!fn || !fn->name) continue;
             int supported = 1; /*...*/ if (!c_type_is_function_wrappable(fn->return_type) && !(fn->return_type && fn->return_type->kind == TK_PRIMITIVE && fn->return_type->data.primitive == PT_VOID)) { supported = 0; } /*...*/
             if (supported) { fprintf(out, "    {\"%s\", lua_%s},\n", fn->name, fn->name); }
         }
    }
     // NO _new functions are registered in this lightuserdata approach
    fprintf(out, "    {NULL, NULL} // Sentinel\n");
    fprintf(out, "};\n\n");

    // --- Generate Enum Tables ---
    // ... (Enum registration remains the same) ...
    fprintf(out, "// --- Enum Registration Function ---\n");
    fprintf(out, "static void register_enums_%s(lua_State *L) {\n", module_name);
    if (tex->num_enums > 0) { /* ... same as before ... */ }
    else { fprintf(out, "    // No enums defined.\n"); }
    fprintf(out, "}\n\n");

    // --- Generate Metatable Registration Function ---
     fprintf(out, "// --- Metatable Registration Function ---\n");
     fprintf(out, "static void register_metatables_%s(lua_State *L) {\n", module_name);
     int registered_meta = 0;
     for (size_t i = 0; i < tex->num_structs; ++i) {
          if (!tex->structs[i] || !tex->structs[i]->name || !tex->structs[i]->self_type || !tex->structs[i]->self_type->data.struct_type) continue;
          char meta_name[256]; get_lua_handle_metatable_name(tex->structs[i]->self_type, module_name, meta_name, sizeof(meta_name)); if (meta_name[0] == '\0') continue;
          fprintf(out, "    // Register handle metatable for struct %s\n", tex->structs[i]->name);
          fprintf(out, "    luaL_newmetatable(L, \"%s\"); // Create if not exists\n", meta_name);
          fprintf(out, "    luaL_register(L, NULL, %s_%s_meta); // Register methods (__index etc)\n", module_name, tex->structs[i]->name);
          fprintf(out, "    lua_pop(L, 1); // Pop metatable\n\n"); registered_meta = 1;
     }
      for (size_t i = 0; i < tex->num_unions; ++i) {
           if (!tex->unions[i] || !tex->unions[i]->name || !tex->unions[i]->self_type || !tex->unions[i]->self_type->data.union_type) continue;
           char meta_name[256]; get_lua_handle_metatable_name(tex->unions[i]->self_type, module_name, meta_name, sizeof(meta_name)); if (meta_name[0] == '\0') continue;
            fprintf(out, "    // Register handle metatable for union %s\n", tex->unions[i]->name);
           fprintf(out, "    luaL_newmetatable(L, \"%s\");\n", meta_name);
           fprintf(out, "    luaL_register(L, NULL, %s_%s_meta);\n", module_name, tex->unions[i]->name);
           fprintf(out, "    lua_pop(L, 1);\n\n"); registered_meta = 1;
      }
       if (!registered_meta) { fprintf(out, "    // No metatables registered.\n"); }
     fprintf(out, "}\n\n");


    // --- Generate luaopen Function ---
    // ... (luaopen remains similar, registers functions, metatables, enums) ...
    fprintf(out, "// --- Module Entry Point ---\n");
    // ... (extern "C" guards and declaration) ...
    fprintf(out, "#ifdef __cplusplus\nextern \"C\" {\n#endif\n");
    fprintf(out, "LUALIB_API int luaopen_%s(lua_State *L);\n", module_name);
    fprintf(out, "#ifdef __cplusplus\n}\n#endif\n\n");
    fprintf(out, "LUALIB_API int luaopen_%s(lua_State *L) {\n", module_name);
    fprintf(out, "    luaL_register(L, \"%s\", %s_funcs);\n\n", module_name, module_name);
    fprintf(out, "    register_metatables_%s(L);\n\n", module_name);
    fprintf(out, "    register_enums_%s(L);\n\n", module_name);
    // ... (Optional _DOC, _VERSION) ...
    fprintf(out, "    lua_pushstring(L, \"%s\"); lua_setfield(L, -2, \"_DOC\");\n", tex->doc ? tex->doc : "");
    fprintf(out, "    return 1; // Return module table\n");
    fprintf(out, "}\n");
    fprintf(out, "// --- End Module Entry Point ---\n");

}


// --- Public API ---
// ... (apicext_gen_lua51_bindings and apicext_gen_lua51_bindings_to_file remain the same) ...
static inline void apicext_gen_lua51_bindings(apic_TypedExports *tex, const char* module_name, const char* public_header_path) { __impl_gen_lua51_bindings(tex, stdout, module_name, public_header_path); }
static inline void apicext_gen_lua51_bindings_to_file(apic_TypedExports *tex, const char* module_name, const char* public_header_path, const char* output_filename) { /* ... file opening logic ... */ FILE *outfile = fopen(output_filename, "w"); if (!outfile) { /* error */ return; } __impl_gen_lua51_bindings(tex, outfile, module_name, public_header_path); fclose(outfile); }


#endif // APIC_REFLECT

#endif // APICEXT_GEN_LUA51_BINDINGS_H