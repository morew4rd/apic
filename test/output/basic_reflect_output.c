

//----------------------------------------------------------

// =============================================
// Public API Header: MyExports
// Description: Example exports with full documentation support
// =============================================

#ifndef MyExports_PUBLIC_H
#define MyExports_PUBLIC_H

#include <stdint.h>

/* =============== Typedefs =============== */
// Single byte type
typedef unsigned char Byte;

/* ================ Enums ================ */
// Basic colors
typedef enum COLOR {
    RED = 0, // red (Primary color)
    GREEN = 1, // green (Secondary color)
    BLUE = 2, // blue (Primary color)
} COLOR;

/* ========== Forward Declarations ========= */
typedef struct Vec2I Vec2I;
typedef struct Vec3I Vec3I;
typedef struct Calculator Calculator;
typedef struct Node Node;
typedef struct Buffer Buffer;
typedef struct Matrix Matrix;

/* ============== Structs ============== */
// 
struct Vec2I {
    int x; // X coordinate
    int y; // Y coordinate
};

// 3D integer vector
struct Vec3I {
    int x; // 
    int y; // 
    int z; // 
};

// 
struct Calculator {
    Adder adder; // 
    float value; // 
};

// 
struct Node {
    int value; // 
    struct Node* next; // 
};

// 
struct Buffer {
    char data[256]; // 
    int size; // Current buffer size
};

// 
struct Matrix {
    float values[16]; // 
    int rows; // Matrix dimensions
    int cols; // 
};

/* =============== Unions =============== */
// 
typedef union Number {
    int i; // Integer storage
    float f; // Floating point storage
} Number;

/* ============== Lambdas ============== */
// Adds int and float
typedef int (*Adder)(int a, float b);

/* ============= Variables ============= */
// Application name
extern const char* app_name;

// Maximum allowed connections
extern int max_connections;

/* ============= Functions ============= */
// Adds int and float
int add_mixed(int a, float b);

#endif // MyExports_PUBLIC_H

/*

//----------------------------------------------------------


=============================================

Exports: MyExports

Description: Example exports with full documentation support

=== Structs (6) ===
Vec2I: 
Fields (2):
  x: int (X coordinate)
  y: int (Y coordinate)

Vec3I: 3D integer vector
Fields (3):
  x: int ()
  y: int ()
  z: int ()

Calculator: 
Fields (2):
  adder: Adder ()
  value: float ()

Node: 
Fields (2):
  value: int ()
  next: struct Node* ()

Buffer: 
Fields (2):
  data: char[256] ()
  size: int (Current buffer size)

Matrix: 
Fields (3):
  values: float[16] ()
  rows: int (Matrix dimensions)
  cols: int ()

=== Unions (1) ===
Number: 
Fields (2):
  i: int (Integer storage)
  f: float (Floating point storage)

=== Functions (1) ===
add_mixed: Adds int and float
Returns: int
Args (2):
  a: int ()
  b: float ()

=== Lambdas (1) ===
Adder: Adds int and float
Returns: int
Args (2):
  a: int ()
  b: float ()

=== Enums (1) ===
COLOR: Basic colors
Entries (3):
  RED = 0 (red, Primary color)
  GREEN = 1 (green, Secondary color)
  BLUE = 2 (blue, Primary color)

=== Variables (2) ===
app_name: const char* (Application name)
max_connections: int (Maximum allowed connections)

=== Typedefs (1) ===
Byte = unsigned char (Single byte type)

=============================================



//----------------------------------------------------------

All type checks passed successfully!


----------------------------------------------------------

*/
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <string.h>

static int is_integer_type(const char *type) {
    return strstr(type, "int") != NULL || 
           strstr(type, "long") != NULL || 
           strstr(type, "short") != NULL || 
           strstr(type, "char") != NULL;
}

static int is_float_type(const char *type) {
    return strstr(type, "float") != NULL || 
           strstr(type, "double") != NULL;
}

static int is_string_type(const char *type) {
    return strstr(type, "char*") != NULL || 
           strstr(type, "const char*") != NULL;
}

static int lua_Vec2I_new(lua_State *L) {
    Vec2I *ud = (Vec2I *)lua_newuserdata(L, sizeof(Vec2I));
    memset(ud, 0, sizeof(Vec2I));
    luaL_getmetatable(L, "Vec2I");
    lua_setmetatable(L, -2);
    return 1;
}

static int lua_Vec2I_index(lua_State *L) {
    Vec2I *ud = (Vec2I *)luaL_checkudata(L, 1, "Vec2I");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "x") == 0) {
        lua_pushinteger(L, ud->x);
        return 1;
    }
    if (strcmp(field, "y") == 0) {
        lua_pushinteger(L, ud->y);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Vec2I_newindex(lua_State *L) {
    Vec2I *ud = (Vec2I *)luaL_checkudata(L, 1, "Vec2I");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "x") == 0) {
        ud->x = luaL_checkinteger(L, 3);
        return 0;
    }
    if (strcmp(field, "y") == 0) {
        ud->y = luaL_checkinteger(L, 3);
        return 0;
    }
    return luaL_error(L, "Invalid field: %s", field);
}

static int lua_Vec3I_new(lua_State *L) {
    Vec3I *ud = (Vec3I *)lua_newuserdata(L, sizeof(Vec3I));
    memset(ud, 0, sizeof(Vec3I));
    luaL_getmetatable(L, "Vec3I");
    lua_setmetatable(L, -2);
    return 1;
}

static int lua_Vec3I_index(lua_State *L) {
    Vec3I *ud = (Vec3I *)luaL_checkudata(L, 1, "Vec3I");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "x") == 0) {
        lua_pushinteger(L, ud->x);
        return 1;
    }
    if (strcmp(field, "y") == 0) {
        lua_pushinteger(L, ud->y);
        return 1;
    }
    if (strcmp(field, "z") == 0) {
        lua_pushinteger(L, ud->z);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Vec3I_newindex(lua_State *L) {
    Vec3I *ud = (Vec3I *)luaL_checkudata(L, 1, "Vec3I");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "x") == 0) {
        ud->x = luaL_checkinteger(L, 3);
        return 0;
    }
    if (strcmp(field, "y") == 0) {
        ud->y = luaL_checkinteger(L, 3);
        return 0;
    }
    if (strcmp(field, "z") == 0) {
        ud->z = luaL_checkinteger(L, 3);
        return 0;
    }
    return luaL_error(L, "Invalid field: %s", field);
}

static int lua_Calculator_new(lua_State *L) {
    Calculator *ud = (Calculator *)lua_newuserdata(L, sizeof(Calculator));
    memset(ud, 0, sizeof(Calculator));
    luaL_getmetatable(L, "Calculator");
    lua_setmetatable(L, -2);
    return 1;
}

static int lua_Calculator_index(lua_State *L) {
    Calculator *ud = (Calculator *)luaL_checkudata(L, 1, "Calculator");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "adder") == 0) {
        lua_pushlightuserdata(L, ud->adder);
        return 1;
    }
    if (strcmp(field, "value") == 0) {
        lua_pushnumber(L, ud->value);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Calculator_newindex(lua_State *L) {
    Calculator *ud = (Calculator *)luaL_checkudata(L, 1, "Calculator");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "adder") == 0) {
        ud->adder = lua_touserdata(L, 3);
        return 0;
    }
    if (strcmp(field, "value") == 0) {
        ud->value = luaL_checknumber(L, 3);
        return 0;
    }
    return luaL_error(L, "Invalid field: %s", field);
}

static int lua_Node_new(lua_State *L) {
    Node *ud = (Node *)lua_newuserdata(L, sizeof(Node));
    memset(ud, 0, sizeof(Node));
    luaL_getmetatable(L, "Node");
    lua_setmetatable(L, -2);
    return 1;
}

static int lua_Node_index(lua_State *L) {
    Node *ud = (Node *)luaL_checkudata(L, 1, "Node");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "value") == 0) {
        lua_pushinteger(L, ud->value);
        return 1;
    }
    if (strcmp(field, "next") == 0) {
        lua_pushlightuserdata(L, ud->next);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Node_newindex(lua_State *L) {
    Node *ud = (Node *)luaL_checkudata(L, 1, "Node");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "value") == 0) {
        ud->value = luaL_checkinteger(L, 3);
        return 0;
    }
    if (strcmp(field, "next") == 0) {
        ud->next = lua_touserdata(L, 3);
        return 0;
    }
    return luaL_error(L, "Invalid field: %s", field);
}

static int lua_Buffer_new(lua_State *L) {
    Buffer *ud = (Buffer *)lua_newuserdata(L, sizeof(Buffer));
    memset(ud, 0, sizeof(Buffer));
    luaL_getmetatable(L, "Buffer");
    lua_setmetatable(L, -2);
    return 1;
}

static int lua_Buffer_index(lua_State *L) {
    Buffer *ud = (Buffer *)luaL_checkudata(L, 1, "Buffer");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "data") == 0) {
        lua_pushlstring(L, (const char*)ud->data, sizeof(ud->data));
        return 1;
    }
    if (strcmp(field, "size") == 0) {
        lua_pushinteger(L, ud->size);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Buffer_newindex(lua_State *L) {
    Buffer *ud = (Buffer *)luaL_checkudata(L, 1, "Buffer");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "size") == 0) {
        ud->size = luaL_checkinteger(L, 3);
        return 0;
    }
    return luaL_error(L, "Invalid field: %s", field);
}

static int lua_Matrix_new(lua_State *L) {
    Matrix *ud = (Matrix *)lua_newuserdata(L, sizeof(Matrix));
    memset(ud, 0, sizeof(Matrix));
    luaL_getmetatable(L, "Matrix");
    lua_setmetatable(L, -2);
    return 1;
}

static int lua_Matrix_index(lua_State *L) {
    Matrix *ud = (Matrix *)luaL_checkudata(L, 1, "Matrix");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "values") == 0) {
        lua_pushlstring(L, (const char*)ud->values, sizeof(ud->values));
        return 1;
    }
    if (strcmp(field, "rows") == 0) {
        lua_pushinteger(L, ud->rows);
        return 1;
    }
    if (strcmp(field, "cols") == 0) {
        lua_pushinteger(L, ud->cols);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Matrix_newindex(lua_State *L) {
    Matrix *ud = (Matrix *)luaL_checkudata(L, 1, "Matrix");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "rows") == 0) {
        ud->rows = luaL_checkinteger(L, 3);
        return 0;
    }
    if (strcmp(field, "cols") == 0) {
        ud->cols = luaL_checkinteger(L, 3);
        return 0;
    }
    return luaL_error(L, "Invalid field: %s", field);
}

static int lua_add_mixed(lua_State *L) {
    if (lua_gettop(L) != 2) return luaL_error(L, "add_mixed expects 2 args");
    int arg1 = luaL_checkinteger(L, 1);
    float arg2 = luaL_checknumber(L, 2);
    int result = add_mixed(arg1, arg2);
    lua_pushinteger(L, result);
    return 1;
}

static void register_enums(lua_State *L) {
    lua_newtable(L);
    lua_pushinteger(L, 0);
    lua_setfield(L, -2, "RED");
    lua_pushinteger(L, 1);
    lua_setfield(L, -2, "GREEN");
    lua_pushinteger(L, 2);
    lua_setfield(L, -2, "BLUE");
    lua_setglobal(L, "COLOR");
}

static void register_vars(lua_State *L) {
    lua_pushinteger(L, app_name);
    lua_setglobal(L, "app_name");
    lua_pushinteger(L, max_connections);
    lua_setglobal(L, "max_connections");
}

int luaopen_MyExports(lua_State *L) {
    luaL_openlibs(L);
    register_enums(L);
    register_vars(L);
    luaL_newmetatable(L, "Vec2I");
    lua_pushcfunction(L, lua_Vec2I_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Vec2I_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_pushcfunction(L, lua_Vec2I_new);
    lua_setglobal(L, "Vec2I_new");
    luaL_newmetatable(L, "Vec3I");
    lua_pushcfunction(L, lua_Vec3I_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Vec3I_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_pushcfunction(L, lua_Vec3I_new);
    lua_setglobal(L, "Vec3I_new");
    luaL_newmetatable(L, "Calculator");
    lua_pushcfunction(L, lua_Calculator_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Calculator_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_pushcfunction(L, lua_Calculator_new);
    lua_setglobal(L, "Calculator_new");
    luaL_newmetatable(L, "Node");
    lua_pushcfunction(L, lua_Node_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Node_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_pushcfunction(L, lua_Node_new);
    lua_setglobal(L, "Node_new");
    luaL_newmetatable(L, "Buffer");
    lua_pushcfunction(L, lua_Buffer_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Buffer_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_pushcfunction(L, lua_Buffer_new);
    lua_setglobal(L, "Buffer_new");
    luaL_newmetatable(L, "Matrix");
    lua_pushcfunction(L, lua_Matrix_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Matrix_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_pushcfunction(L, lua_Matrix_new);
    lua_setglobal(L, "Matrix_new");
    lua_pushcfunction(L, lua_add_mixed);
    lua_setglobal(L, "add_mixed");
    return 1;
}


//----------------------------------------------------------

