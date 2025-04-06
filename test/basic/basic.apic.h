#define APIC_SHORTCUTS
#include "../../src/apic.h"

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

// apic_Typedef examples
ALIAS_(Byte, unsigned char, "Single byte type");

FUNC_(add_mixed, "Adds int and float", int,
    A(a, int),
    A(b, float)
);

FUNCPTR_(Adder, "Adds int and float", int,
    A(a, int),
    A(b, float)
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

EXPORTS_(mysimplelib, "Example exports with full documentation support",
    ENUMS(&COLOR),
    STRUCTS(&Vec2I, &Vec3I, &Calculator, &Node, &Buffer, &Matrix),
    UNIONS(&Number),
    FUNCS(&add_mixed),
    FUNCPTRS(&Adder),
    TYPEDEFS(&Byte)
);
