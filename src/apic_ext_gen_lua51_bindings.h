#ifdef APIC_REFLECT

#include "apic.h"
#include <stdio.h>
#include <string.h>

// Helper functions for the generator itself
static int is_integer_type(const char *type) {
    return strstr(type, "int") ||
           strstr(type, "long") ||
           strstr(type, "short") ||
           strstr(type, "char");
}

static int is_float_type(const char *type) {
    return strstr(type, "float") ||
           strstr(type, "double");
}

static int is_string_type(const char *type) {
    return strstr(type, "char*") ||
           strstr(type, "const char*");
}

static void apic_ext_gen_lua51_bindings(Exports *exports, FILE *out) {
    fprintf(out, "#include <lua.h>\n");
    fprintf(out, "#include <lauxlib.h>\n");
    fprintf(out, "#include <lualib.h>\n");
    fprintf(out, "#include <string.h>\n\n");

    // Helper functions
    fprintf(out, "static int is_integer_type(const char *type) {\n");
    fprintf(out, "    return strstr(type, \"int\") != NULL || \n");
    fprintf(out, "           strstr(type, \"long\") != NULL || \n");
    fprintf(out, "           strstr(type, \"short\") != NULL || \n");
    fprintf(out, "           strstr(type, \"char\") != NULL;\n");
    fprintf(out, "}\n\n");

    fprintf(out, "static int is_float_type(const char *type) {\n");
    fprintf(out, "    return strstr(type, \"float\") != NULL || \n");
    fprintf(out, "           strstr(type, \"double\") != NULL;\n");
    fprintf(out, "}\n\n");

    fprintf(out, "static int is_string_type(const char *type) {\n");
    fprintf(out, "    return strstr(type, \"char*\") != NULL || \n");
    fprintf(out, "           strstr(type, \"const char*\") != NULL;\n");
    fprintf(out, "}\n\n");

    // Struct metatables and constructors
    for (int i = 0; i < exports->struct_count; i++) {
        Struct *st = exports->structs[i];

        // Constructor
        fprintf(out, "static int lua_%s_new(lua_State *L) {\n", st->name);
        fprintf(out, "    %s *ud = (%s *)lua_newuserdata(L, sizeof(%s));\n",
                st->name, st->name, st->name);
        fprintf(out, "    memset(ud, 0, sizeof(%s));\n", st->name);
        fprintf(out, "    luaL_getmetatable(L, \"%s\");\n", st->name);
        fprintf(out, "    lua_setmetatable(L, -2);\n");
        fprintf(out, "    return 1;\n}\n\n");

        // __index
        fprintf(out, "static int lua_%s_index(lua_State *L) {\n", st->name);
        fprintf(out, "    %s *ud = (%s *)luaL_checkudata(L, 1, \"%s\");\n",
                st->name, st->name, st->name);
        fprintf(out, "    const char *field = luaL_checkstring(L, 2);\n");

        for (int j = 0; j < st->count; j++) {
            Field f = st->fields[j];

            if (strstr(f.type, "[")) {
                fprintf(out, "    if (strcmp(field, \"%s\") == 0) {\n", f.name);
                fprintf(out, "        lua_pushlstring(L, (const char*)ud->%s, sizeof(ud->%s));\n", f.name, f.name);
                fprintf(out, "        return 1;\n    }\n");
                continue;
            }

            fprintf(out, "    if (strcmp(field, \"%s\") == 0) {\n", f.name);

            if (is_integer_type(f.type)) {
                fprintf(out, "        lua_pushinteger(L, ud->%s);\n", f.name);
            } else if (is_float_type(f.type)) {
                fprintf(out, "        lua_pushnumber(L, ud->%s);\n", f.name);
            } else if (is_string_type(f.type)) {
                fprintf(out, "        lua_pushstring(L, ud->%s);\n", f.name);
            } else {
                fprintf(out, "        lua_pushlightuserdata(L, ud->%s);\n", f.name);
            }

            fprintf(out, "        return 1;\n    }\n");
        }
        fprintf(out, "    lua_pushnil(L);\n    return 1;\n}\n\n");

        // __newindex
        fprintf(out, "static int lua_%s_newindex(lua_State *L) {\n", st->name);
        fprintf(out, "    %s *ud = (%s *)luaL_checkudata(L, 1, \"%s\");\n",
                st->name, st->name, st->name);
        fprintf(out, "    const char *field = luaL_checkstring(L, 2);\n");

        for (int j = 0; j < st->count; j++) {
            Field f = st->fields[j];

            if (strstr(f.type, "[")) continue;

            fprintf(out, "    if (strcmp(field, \"%s\") == 0) {\n", f.name);

            if (is_integer_type(f.type)) {
                fprintf(out, "        ud->%s = luaL_checkinteger(L, 3);\n", f.name);
            } else if (is_float_type(f.type)) {
                fprintf(out, "        ud->%s = luaL_checknumber(L, 3);\n", f.name);
            } else if (is_string_type(f.type)) {
                fprintf(out, "        ud->%s = (char*)luaL_checkstring(L, 3);\n", f.name);
            } else {
                fprintf(out, "        ud->%s = lua_touserdata(L, 3);\n", f.name);
            }

            fprintf(out, "        return 0;\n    }\n");
        }
        fprintf(out, "    return luaL_error(L, \"Invalid field: %%s\", field);\n}\n\n");
    }

    // Function wrappers
    for (int i = 0; i < exports->func_count; i++) {
        Func *func = exports->funcs[i];
        fprintf(out, "static int lua_%s(lua_State *L) {\n", func->name);

        // Check argument count
        fprintf(out, "    if (lua_gettop(L) != %d) ", func->count);
        fprintf(out, "return luaL_error(L, \"%s expects %d args\");\n",
                func->name, func->count);

        // Get arguments
        for (int j = 0; j < func->count; j++) {
            Arg a = func->args[j];
            if (is_integer_type(a.type)) {
                fprintf(out, "    %s arg%d = luaL_checkinteger(L, %d);\n",
                        a.type, j+1, j+1);
            } else if (is_float_type(a.type)) {
                fprintf(out, "    %s arg%d = luaL_checknumber(L, %d);\n",
                        a.type, j+1, j+1);
            } else if (is_string_type(a.type)) {
                fprintf(out, "    %s arg%d = (char*)luaL_checkstring(L, %d);\n",
                        a.type, j+1, j+1);
            } else {
                fprintf(out, "    %s arg%d = *(%s*)luaL_checkudata(L, %d, \"%s\");\n",
                        a.type, j+1, a.type, j+1, a.type);
            }
        }

        // Call function
        if (strcmp(func->ret, "void") == 0) {
            fprintf(out, "    %s(", func->name);
            for (int j = 0; j < func->count; j++) {
                fprintf(out, "%sarg%d%s", j>0?", ":"", j+1, j==func->count-1?");\n":"");
            }
            fprintf(out, "    return 0;\n");
        } else {
            fprintf(out, "    %s result = %s(", func->ret, func->name);
            for (int j = 0; j < func->count; j++) {
                fprintf(out, "%sarg%d%s", j>0?", ":"", j+1, j==func->count-1?");\n":"");
            }

            if (is_integer_type(func->ret)) {
                fprintf(out, "    lua_pushinteger(L, result);\n");
            } else if (is_float_type(func->ret)) {
                fprintf(out, "    lua_pushnumber(L, result);\n");
            } else if (is_string_type(func->ret)) {
                fprintf(out, "    lua_pushstring(L, result);\n");
            } else {
                fprintf(out, "    %s *ptr = (%s*)lua_newuserdata(L, sizeof(%s));\n",
                        func->ret, func->ret, func->ret);
                fprintf(out, "    *ptr = result;\n");
                fprintf(out, "    luaL_getmetatable(L, \"%s\");\n", func->ret);
                fprintf(out, "    lua_setmetatable(L, -2);\n");
            }
            fprintf(out, "    return 1;\n");
        }
        fprintf(out, "}\n\n");
    }

    // Enum registration
    fprintf(out, "static void register_enums(lua_State *L) {\n");
    for (int i = 0; i < exports->enum_count; i++) {
        Enum *en = exports->enums[i];
        fprintf(out, "    lua_newtable(L);\n");
        for (int j = 0; j < en->count; j++) {
            EnumEntry e = en->entries[j];
            fprintf(out, "    lua_pushinteger(L, %d);\n", e.value);
            fprintf(out, "    lua_setfield(L, -2, \"%s\");\n", e.name);
        }
        fprintf(out, "    lua_setglobal(L, \"%s\");\n", en->name);
    }
    fprintf(out, "}\n\n");

    // Global variables
    fprintf(out, "static void register_vars(lua_State *L) {\n");
    for (int i = 0; i < exports->var_count; i++) {
        Var *v = exports->vars[i];
        if (is_integer_type(v->type)) {
            fprintf(out, "    lua_pushinteger(L, %s);\n", v->name);
        } else if (is_float_type(v->type)) {
            fprintf(out, "    lua_pushnumber(L, %s);\n", v->name);
        } else if (is_string_type(v->type)) {
            fprintf(out, "    lua_pushstring(L, %s);\n", v->name);
        } else {
            fprintf(out, "    %s *ptr = &%s;\n", v->type, v->name);
            fprintf(out, "    lua_pushlightuserdata(L, ptr);\n");
        }
        fprintf(out, "    lua_setglobal(L, \"%s\");\n", v->name);
    }
    fprintf(out, "}\n\n");

    // Main registration
    fprintf(out, "int luaopen_%s(lua_State *L) {\n", exports->name);
    fprintf(out, "    luaL_openlibs(L);\n");
    fprintf(out, "    register_enums(L);\n");
    fprintf(out, "    register_vars(L);\n");

    // Register struct metatables
    for (int i = 0; i < exports->struct_count; i++) {
        Struct *st = exports->structs[i];
        fprintf(out, "    luaL_newmetatable(L, \"%s\");\n", st->name);
        fprintf(out, "    lua_pushcfunction(L, lua_%s_index);\n", st->name);
        fprintf(out, "    lua_setfield(L, -2, \"__index\");\n");
        fprintf(out, "    lua_pushcfunction(L, lua_%s_newindex);\n", st->name);
        fprintf(out, "    lua_setfield(L, -2, \"__newindex\");\n");
        fprintf(out, "    lua_pop(L, 1);\n");

        // Register constructor
        fprintf(out, "    lua_pushcfunction(L, lua_%s_new);\n", st->name);
        fprintf(out, "    lua_setglobal(L, \"%s_new\");\n", st->name);
    }

    // Register functions
    for (int i = 0; i < exports->func_count; i++) {
        Func *func = exports->funcs[i];
        fprintf(out, "    lua_pushcfunction(L, lua_%s);\n", func->name);
        fprintf(out, "    lua_setglobal(L, \"%s\");\n", func->name);
    }

    fprintf(out, "    return 1;\n}\n");
}

#endif // APIC_REFLECT