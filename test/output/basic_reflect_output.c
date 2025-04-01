

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
    char[256] data; // 
    int size; // Current buffer size
};

// 
struct Matrix {
    float[16] values; // 
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

*/

----------------------------------------------------------

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static const char* resolve_typedef(Exports *exports, const char *type) {
    for (int i = 0; i < exports->typedef_count; i++) {
        if (strcmp(exports->typedefs[i]->name, type) == 0)
            return exports->typedefs[i]->type;
    }
    return type;
}

static int get_enum_value(Enum *en, const char *str) {
    for (int i = 0; i < en->count; i++) {
        if (strcmp(en->entries[i].str, str) == 0)
            return en->entries[i].value;
    }
    return -1;
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
        lua_pushlightuserdata(L, &ud->x);
        return 1;
    }
    if (strcmp(field, "y") == 0) {
        lua_pushlightuserdata(L, &ud->y);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Vec2I_newindex(lua_State *L) {
    Vec2I *ud = (Vec2I *)luaL_checkudata(L, 1, "Vec2I");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "x") == 0) {
        memcpy(&ud->x, lua_touserdata(L, 3), sizeof(ud->x));
        return 0;
    }
    if (strcmp(field, "y") == 0) {
        memcpy(&ud->y, lua_touserdata(L, 3), sizeof(ud->y));
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
        lua_pushlightuserdata(L, &ud->x);
        return 1;
    }
    if (strcmp(field, "y") == 0) {
        lua_pushlightuserdata(L, &ud->y);
        return 1;
    }
    if (strcmp(field, "z") == 0) {
        lua_pushlightuserdata(L, &ud->z);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Vec3I_newindex(lua_State *L) {
    Vec3I *ud = (Vec3I *)luaL_checkudata(L, 1, "Vec3I");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "x") == 0) {
        memcpy(&ud->x, lua_touserdata(L, 3), sizeof(ud->x));
        return 0;
    }
    if (strcmp(field, "y") == 0) {
        memcpy(&ud->y, lua_touserdata(L, 3), sizeof(ud->y));
        return 0;
    }
    if (strcmp(field, "z") == 0) {
        memcpy(&ud->z, lua_touserdata(L, 3), sizeof(ud->z));
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
        lua_pushlightuserdata(L, &ud->adder);
        return 1;
    }
    if (strcmp(field, "value") == 0) {
        lua_pushlightuserdata(L, &ud->value);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Calculator_newindex(lua_State *L) {
    Calculator *ud = (Calculator *)luaL_checkudata(L, 1, "Calculator");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "adder") == 0) {
        memcpy(&ud->adder, lua_touserdata(L, 3), sizeof(ud->adder));
        return 0;
    }
    if (strcmp(field, "value") == 0) {
        memcpy(&ud->value, lua_touserdata(L, 3), sizeof(ud->value));
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
        lua_pushlightuserdata(L, &ud->value);
        return 1;
    }
    if (strcmp(field, "next") == 0) {
        lua_pushlightuserdata(L, &ud->next);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Node_newindex(lua_State *L) {
    Node *ud = (Node *)luaL_checkudata(L, 1, "Node");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "value") == 0) {
        memcpy(&ud->value, lua_touserdata(L, 3), sizeof(ud->value));
        return 0;
    }
    if (strcmp(field, "next") == 0) {
        memcpy(&ud->next, lua_touserdata(L, 3), sizeof(ud->next));
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
        lua_pushlightuserdata(L, &ud->data);
        return 1;
    }
    if (strcmp(field, "size") == 0) {
        lua_pushlightuserdata(L, &ud->size);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Buffer_newindex(lua_State *L) {
    Buffer *ud = (Buffer *)luaL_checkudata(L, 1, "Buffer");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "data") == 0) {
        memcpy(&ud->data, lua_touserdata(L, 3), sizeof(ud->data));
        return 0;
    }
    if (strcmp(field, "size") == 0) {
        memcpy(&ud->size, lua_touserdata(L, 3), sizeof(ud->size));
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
        lua_pushlightuserdata(L, &ud->values);
        return 1;
    }
    if (strcmp(field, "rows") == 0) {
        lua_pushlightuserdata(L, &ud->rows);
        return 1;
    }
    if (strcmp(field, "cols") == 0) {
        lua_pushlightuserdata(L, &ud->cols);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int lua_Matrix_newindex(lua_State *L) {
    Matrix *ud = (Matrix *)luaL_checkudata(L, 1, "Matrix");
    const char *field = luaL_checkstring(L, 2);
    if (strcmp(field, "values") == 0) {
        memcpy(&ud->values, lua_touserdata(L, 3), sizeof(ud->values));
        return 0;
    }
    if (strcmp(field, "rows") == 0) {
        memcpy(&ud->rows, lua_touserdata(L, 3), sizeof(ud->rows));
        return 0;
    }
    if (strcmp(field, "cols") == 0) {
        memcpy(&ud->cols, lua_touserdata(L, 3), sizeof(ud->cols));
        return 0;
    }
    return luaL_error(L, "Invalid field: %s", field);
}

static int lua_add_mixed(lua_State *L) {
    int arg1;
    float arg2;
    if (lua_gettop(L) != 2) return luaL_error(L, "add_mixed expects 2 args");
    arg1 = *((int *)lua_touserdata(L, 1));
    arg2 = *((float *)lua_touserdata(L, 2));
    int result = add_mixed(arg1, arg2);
    lua_pushlightuserdata(L, &result);
    return 1;
}

static void register_enums(lua_State *L) {
    lua_newtable(L);
    lua_pushstring(L, "red");
    lua_setfield(L, -2, "RED");
    lua_pushstring(L, "green");
    lua_setfield(L, -2, "GREEN");
    lua_pushstring(L, "blue");
    lua_setfield(L, -2, "BLUE");
    lua_setglobal(L, "COLOR");
}

int luaopen_MyExports(lua_State *L) {
    luaL_openlibs(L);
    register_enums(L);
    luaL_newmetatable(L, "Vec2I");
    lua_pushcfunction(L, lua_Vec2I_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Vec2I_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_register(L, "Vec2I_new", lua_Vec2I_new);
    luaL_newmetatable(L, "Vec3I");
    lua_pushcfunction(L, lua_Vec3I_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Vec3I_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_register(L, "Vec3I_new", lua_Vec3I_new);
    luaL_newmetatable(L, "Calculator");
    lua_pushcfunction(L, lua_Calculator_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Calculator_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_register(L, "Calculator_new", lua_Calculator_new);
    luaL_newmetatable(L, "Node");
    lua_pushcfunction(L, lua_Node_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Node_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_register(L, "Node_new", lua_Node_new);
    luaL_newmetatable(L, "Buffer");
    lua_pushcfunction(L, lua_Buffer_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Buffer_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_register(L, "Buffer_new", lua_Buffer_new);
    luaL_newmetatable(L, "Matrix");
    lua_pushcfunction(L, lua_Matrix_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_Matrix_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
    lua_register(L, "Matrix_new", lua_Matrix_new);
    lua_register(L, "add_mixed", lua_add_mixed);
    return 1;
}


//----------------------------------------------------------

