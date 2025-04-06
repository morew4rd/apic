

//----------------------------------------------------------

// =============================================
// Public API Header: mysimplelib
// Description: Example exports with full documentation support
// =============================================

#ifndef mysimplelib_PUBLIC_H
#define mysimplelib_PUBLIC_H

#include <stdint.h>

/* =============== apic_Typedefs =============== */
// Single byte type
typedef unsigned char Byte;

/* ================ apic_Enums ================ */
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

/* ============== apic_Structs ============== */
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

/* =============== apic_Unions =============== */
// 
typedef union Number {
    int i; // Integer storage
    float f; // Floating point storage
} Number;

/* ============== apic_Lambdas ============== */
// Adds int and float
typedef int (*Adder)(int a, float b);

/* ============= apic_Functions ============= */
// Adds int and float
int add_mixed(int a, float b);

#endif // mysimplelib_PUBLIC_H

/*

//----------------------------------------------------------


=============================================

apic_Exports: mysimplelib

Description: Example exports with full documentation support

=== apic_Structs (6) ===
Vec2I: 
apic_Fields (2):
  x: int (X coordinate)
  y: int (Y coordinate)

Vec3I: 3D integer vector
apic_Fields (3):
  x: int ()
  y: int ()
  z: int ()

Calculator: 
apic_Fields (2):
  adder: Adder ()
  value: float ()

Node: 
apic_Fields (2):
  value: int ()
  next: struct Node* ()

Buffer: 
apic_Fields (2):
  data: char[256] ()
  size: int (Current buffer size)

Matrix: 
apic_Fields (3):
  values: float[16] ()
  rows: int (Matrix dimensions)
  cols: int ()

=== apic_Unions (1) ===
Number: 
apic_Fields (2):
  i: int (Integer storage)
  f: float (Floating point storage)

=== apic_Functions (1) ===
add_mixed: Adds int and float
Returns: int
apic_Args (2):
  a: int ()
  b: float ()

=== apic_Lambdas (1) ===
Adder: Adds int and float
Returns: int
apic_Args (2):
  a: int ()
  b: float ()

=== apic_Enums (1) ===
COLOR: Basic colors
Entries (3):
  RED = 0 (red, Primary color)
  GREEN = 1 (green, Secondary color)
  BLUE = 2 (blue, Primary color)

=== apic_Variables (0) ===

=== apic_Typedefs (1) ===
Byte = unsigned char (Single byte type)

=============================================



//----------------------------------------------------------

All type checks passed successfully!

========== Typed API: mysimplelib ==========
// Example exports with full documentation support

apic_Typedefs (1):
  typedef unsigned char        → Byte

apic_Structures (6):
struct Vec2I {
  // 
  int                  x  // X coordinate
  int                  y  // Y coordinate
}

struct Vec3I {
  // 3D integer vector
  int                  x  // 
  int                  y  // 
  int                  z  // 
}

struct Calculator {
  // 
  char*                adder  // 
  float                value  // 
}

struct Node {
  // 
  int                  value  // 
  struct *             next  // 
}

struct Buffer {
  // 
  char[256]            data  // 
  int                  size  // Current buffer size
}

struct Matrix {
  // 
  float[16]            values  // 
  int                  rows  // Matrix dimensions
  int                  cols  // 
}


apic_Functions (1):
int add_mixed(int a, float b)  // Adds int and float
========================================


*/


//----------------------------------------------------------

