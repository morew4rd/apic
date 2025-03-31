#include "apic.h"

STRUCT(Vec2I,
    F_(x, int, "X coordinate"),
    F_(y, int, "Y coordinate")
);

STRUCT_(Vec3I, "3D integer vector",
    F(x, int),
    F(y, int),
    F(z, int)
);

UNION(Number,
    F_(i, int, "Integer storage"),
    F_(f, float, "Floating point storage")
);

STRUCT(Node,
    F(value, int),
    F(next, PTR(struct Node))
);

STRUCT(Buffer,
    FA(data, char, 256),
    F_(size, int, "Current buffer size")
);

STRUCT(Matrix,
    FA(values, float, 16),
    F_(rows, int, "Matrix dimensions"),
    F(cols, int)
);

// Global variables
VAR_(app_name, const char*, "Application name");
VAR_(max_connections, int, "Maximum allowed connections");

// Typedef examples
TDEF_(Byte, unsigned char, "Single byte type");

FUNC_(add_mixed, "Adds int and float", int,
    A(int, a),
    A(float, b)
);

LAMBDA_(Adder, "Adds int and float", int,
    A(int, a),
    A(float, b)
);

STRUCT(Calculator,
    F(adder, Adder),
    F(value, float)
);

ENUM_(COLOR, "Basic colors",
    N_(RED, 0, "red", "Primary color"),
    N_(GREEN, 1, "green", "Secondary color"),
    N_(BLUE, 2, "blue", "Primary color")
);

MODULE_(MyModule, "Example module with full documentation support",
    ENUMS(&COLOR),
    STRUCTS(&Vec2I, &Vec3I, &Calculator, &Node, &Buffer, &Matrix),
    UNIONS(&Number),
    FUNCS(&add_mixed),
    LAMBDAS(&Adder),
    VARS(&app_name, &max_connections),
    TYPEDEFS(&Byte)
);
