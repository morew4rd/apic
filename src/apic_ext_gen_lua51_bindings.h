#ifndef APIC_EXT_LUA51_H
#define APIC_EXT_LUA51_H

#define APIC_REFLECT
#include "apic.h"
#include <stdio.h>
#include <string.h>

static void apic_ext_gen_lua51_bindings(Exports *exports, FILE *out) {
    fprintf(out, "#include <lua.h>\n");
    fprintf(out, "#include <lauxlib.h>\n");
    fprintf(out, "#include <lualib.h>\n\n");

    // Helper function to resolve typedefs
    fprintf(out, "static const char* resolve_typedef(Exports *exports, const char *type) {\n");
    fprintf(out, "    for (int i = 0; i < exports->typedef_count; i++) {\n");
    fprintf(out, "        if (strcmp(exports->typedefs[i]->name, type) == 0)\n");
    fprintf(out, "            return exports->typedefs[i]->type;\n");
    fprintf(out, "    }\n");
    fprintf(out, "    return type;\n");
    fprintf(out, "}\n\n");

    // Enum value lookup helper
    fprintf(out, "static int get_enum_value(Enum *en, const char *str) {\n");
    fprintf(out, "    for (int i = 0; i < en->count; i++) {\n");
    fprintf(out, "        if (strcmp(en->entries[i].str, str) == 0)\n");
    fprintf(out, "            return en->entries[i].value;\n");
    fprintf(out, "    }\n");
    fprintf(out, "    return -1;\n");
    fprintf(out, "}\n\n");

    // Struct metatables and constructors
    for (int i = 0; i < exports->struct_count; i++) {
        Struct *st = exports->structs[i];
        fprintf(out, "static int lua_%s_new(lua_State *L) {\n", st->name);
        fprintf(out, "    %s *ud = (%s *)lua_newuserdata(L, sizeof(%s));\n",
                st->name, st->name, st->name);
        fprintf(out, "    memset(ud, 0, sizeof(%s));\n", st->name);
        fprintf(out, "    luaL_getmetatable(L, \"%s\");\n", st->name);
        fprintf(out, "    lua_setmetatable(L, -2);\n");
        fprintf(out, "    return 1;\n}\n\n");

        fprintf(out, "static int lua_%s_index(lua_State *L) {\n", st->name);
        fprintf(out, "    %s *ud = (%s *)luaL_checkudata(L, 1, \"%s\");\n",
                st->name, st->name, st->name);
        fprintf(out, "    const char *field = luaL_checkstring(L, 2);\n");
        for (int j = 0; j < st->count; j++) {
            Field f = st->fields[j];
            fprintf(out, "    if (strcmp(field, \"%s\") == 0) {\n", f.name);
            fprintf(out, "        lua_pushlightuserdata(L, &ud->%s);\n", f.name);
            fprintf(out, "        return 1;\n    }\n");
        }
        fprintf(out, "    lua_pushnil(L);\n    return 1;\n}\n\n");

        fprintf(out, "static int lua_%s_newindex(lua_State *L) {\n", st->name);
        fprintf(out, "    %s *ud = (%s *)luaL_checkudata(L, 1, \"%s\");\n",
                st->name, st->name, st->name);
        fprintf(out, "    const char *field = luaL_checkstring(L, 2);\n");
        for (int j = 0; j < st->count; j++) {
            Field f = st->fields[j];
            fprintf(out, "    if (strcmp(field, \"%s\") == 0) {\n", f.name);
            fprintf(out, "        memcpy(&ud->%s, lua_touserdata(L, 3), sizeof(ud->%s));\n",
                    f.name, f.name);
            fprintf(out, "        return 0;\n    }\n");
        }
        fprintf(out, "    return luaL_error(L, \"Invalid field: %%s\", field);\n}\n\n");
    }

    // Function wrappers
    for (int i = 0; i < exports->func_count; i++) {
        Func *func = exports->funcs[i];
        fprintf(out, "static int lua_%s(lua_State *L) {\n", func->name);
        for (int j = 0; j < func->count; j++) {
            Arg a = func->args[j];
            fprintf(out, "    %s arg%d;\n", a.type, j+1);
        }
        fprintf(out, "    if (lua_gettop(L) != %d) return luaL_error(L, \"%s expects %d args\");\n",
                func->count, func->name, func->count);

        for (int j = 0; j < func->count; j++) {
            Arg a = func->args[j];
            fprintf(out, "    arg%d = *((%s *)lua_touserdata(L, %d));\n",
                    j+1, a.type, j+1);
        }

        if (strcmp(func->ret, "void") == 0) {
            fprintf(out, "    %s(", func->name);
            for (int j = 0; j < func->count; j++)
                fprintf(out, "%sarg%d%s", j>0?", ":"", j+1, j==func->count-1?");\n":"");
            fprintf(out, "    return 0;\n}\n\n");
        } else {
            fprintf(out, "    %s result = %s(", func->ret, func->name);
            for (int j = 0; j < func->count; j++)
                fprintf(out, "%sarg%d%s", j>0?", ":"", j+1, j==func->count-1?");\n":"");
            fprintf(out, "    lua_pushlightuserdata(L, &result);\n");
            fprintf(out, "    return 1;\n}\n\n");
        }
    }

    // Enum registration
    fprintf(out, "static void register_enums(lua_State *L) {\n");
    for (int i = 0; i < exports->enum_count; i++) {
        Enum *en = exports->enums[i];
        fprintf(out, "    lua_newtable(L);\n");
        for (int j = 0; j < en->count; j++) {
            EnumEntry e = en->entries[j];
            fprintf(out, "    lua_pushstring(L, \"%s\");\n", e.str);
            fprintf(out, "    lua_setfield(L, -2, \"%s\");\n", e.name);
        }
        fprintf(out, "    lua_setglobal(L, \"%s\");\n", en->name);
    }
    fprintf(out, "}\n\n");

    // Main registration
    fprintf(out, "int luaopen_%s(lua_State *L) {\n", exports->name);
    fprintf(out, "    luaL_openlibs(L);\n");
    fprintf(out, "    register_enums(L);\n");

    for (int i = 0; i < exports->struct_count; i++) {
        Struct *st = exports->structs[i];
        fprintf(out, "    luaL_newmetatable(L, \"%s\");\n", st->name);
        fprintf(out, "    lua_pushcfunction(L, lua_%s_index);\n", st->name);
        fprintf(out, "    lua_setfield(L, -2, \"__index\");\n");
        fprintf(out, "    lua_pushcfunction(L, lua_%s_newindex);\n", st->name);
        fprintf(out, "    lua_setfield(L, -2, \"__newindex\");\n");
        fprintf(out, "    lua_pop(L, 1);\n");
        fprintf(out, "    lua_register(L, \"%s_new\", lua_%s_new);\n", st->name, st->name);
    }

    for (int i = 0; i < exports->func_count; i++) {
        Func *func = exports->funcs[i];
        fprintf(out, "    lua_register(L, \"%s\", lua_%s);\n", func->name, func->name);
    }

    fprintf(out, "    return 1;\n}\n");
}

#endif // APIC_EXT_LUA51_H