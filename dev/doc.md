# APIC - Automated Programming Interface Constructor

**APIC** is a dual-mode C code generation system that combines:
1. **Runtime Reflection Metadata** - For introspecting types/functions at runtime
2. **Header Generation** - For normal C type definitions and API consumption

## Core Concepts

### Dual-Mode Operation
Define `APIC_REFLECT` to enable reflection mode:
```c
#define APIC_REFLECT
#include "apic.h"
```

**Reflection Mode** generates:
- Type metadata structures
- Function signatures
- Documentation storage
- Export aggregation

**Header Mode** (default) generates:
- Standard C type definitions
- Function prototypes
- Normal header content

## Core Macros

### 1. Type Definitions

#### Structs
```c
STRUCT(Name,
    F(field1, type1),
    F_(field2, type2, "doc"),
    FA(array_field, type, size)
);

STRUCT_(Name, "Documentation",
    // fields...
);
```
Example:
```c
STRUCT(Vec2I,
    F_(x, int, "X coordinate"),
    F_(y, int, "Y coordinate")
);
```

#### Unions
```c
UNION(Name,
    F(field1, type1),
    F_(field2, type2, "doc")
);
```

#### Enums
```c
ENUM_(Name, "Documentation",
    N_(RED, 0, "red", "Primary color"),
    N(GREEN, 1, "green")
);
```

### 2. Function Definitions
```c
FUNC_(add_mixed, "Adds numbers", int,
    A(int, a),
    A_(float, b, "Second operand")
);

// Lambda = function pointer type
LAMBDA_(Adder, "Adds numbers", int,
    A(int, a),
    A(float, b)
);
```

### 3. Variables & Typedefs
```c
VAR_(max_conn, int, "Max connections");
TDEF_(Byte, unsigned char, "Single byte");
```

### 4. Export Aggregation
```c
EXPORTS_(MyAPI, "Public API Documentation",
    STRUCTS(&Vec2I, &Buffer),
    ENUMS(&COLOR),
    FUNCS(&add_mixed),
    LAMBDAS(&Adder),
    VARS(&max_conn),
    TYPEDEFS(&Byte)
);
```

## Extensions

### 1. Pretty Printing (apic_ext_prettyprint.h)
```c
apic_ext_prettyprint(&MyExports);
```
Sample Output:
```
=============================================
Exports: MyExports

Description: Example exports...

=== Structs (2) ===
Vec2I: 2D vector
Fields (2):
  x: int (X coordinate)
  y: int (Y coordinate)
...
```

### 2. Header Generation (apic_ext_print_public_header.h)
```c
FILE *f = fopen("myapi.h", "w");
apic_ext_print_public_header(&MyExports, f);
```
Generates:
```c
// =============================================
// Public API Header: MyAPI
// Description: Public API Documentation
// =============================================

#ifndef MYAPI_PUBLIC_H
#define MYAPI_PUBLIC_H

#include <stdint.h>

/* =============== Typedefs =============== */
// Single byte type
typedef unsigned char Byte;

/* ================ Enums ================ */
// Basic colors
typedef enum COLOR {
    RED = 0, // red (Primary color)
    GREEN = 1, // green
} COLOR;

/* ============== Structs ============== */
// 2D vector
struct Vec2I {
    int x; // X coordinate
    int y; // Y coordinate
};
...
```

## Full Example

### API Definition (my_api.h)
```c
#include "apic.h"

STRUCT(Vec2I,
    F_(x, int, "X coordinate"),
    F_(y, int, "Y coordinate")
);

FUNC_(add_mixed, "Adds numbers", int,
    A(int, a),
    A(float, b)
);

EXPORTS_(MyAPI, "Math API",
    STRUCTS(&Vec2I),
    FUNCS(&add_mixed)
);
```

### Runtime Usage
```c
#define APIC_REFLECT
#include "my_api.h"
#include "apic_ext_prettyprint.h"

int main() {
    apic_ext_prettyprint(&MyAPI);
    return 0;
}
```

### Header Generation
```c
#define APIC_REFLECT
#include "my_api.h"
#include "apic_ext_print_public_header.h"

int main() {
    FILE *f = fopen("public_api.h", "w");
    apic_ext_print_public_header(&MyAPI, f);
    fclose(f);
    return 0;
}
```

## Benefits

1. **Single Source of Truth** - Define API once, get both implementation and docs
2. **Self-documenting** - Documentation lives with code definitions
3. **Runtime Inspection** - Enumerate types/functions programmatically
4. **Consistency** - Generated headers ensure interface stability
5. **Documentation Generation** - Automatically create API reference docs

## Typical Workflow

1. Define API using APIC macros
2. Generate public headers for consumers
3. Use reflection for:
   - Serialization/Deserialization
   - RPC systems
   - Automated testing
   - Documentation generation
   - Debugging tools

APIC provides powerful meta-programming capabilities while maintaining standard C compatibility.
