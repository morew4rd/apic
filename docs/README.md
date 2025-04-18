# APIC

**THIS IS WIP. DON'T USE IT YET **

## Overview

Define your public APIs with the provided simple macros and use the file like a normal header file. But you also get reflection data for the types and the function signatures to be used by tools.

```c
#include "apic.h"

/*
A(x, I32);     // arg: functions
E(bla);        // entry: enums
F(x, I32);     // field: structs, unions, variants,
FA(x, I32, $n) // field (static array)  // array only allowed in fields. elsewhere use PTR or SLC


// ptr (*)
PTR(I32)
// slice: ptr + count
SLC(I32)
*/

// function
FUNC(new_board, PTR(Board), A(w, I32), A(h, I32));
// function
FUNCPTR(BoardMakerFn, PTR(Board), A(w, I32), A(h, I32));
// enum
ENUM(MY_COLOR, E(red), E(green), E(blue));
// struct
STRUCT(Board, F(w, I32), F(h, I32), F(data, PTR(Byte)));
// union
UNION(Result, F(board, Board), F(failure_code, I32));
// variant: enum + union
VARIANT(Result2, F(board, Board), F(failure_code, I32));
// alias
ALIAS(MyRes, Result);

// export multiple names for reflections
EXPORTS(new_board, BoardMakerFn, MY_COLOR, Board, Result, Result2, MyRes);

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


## Example

See `./test` directory.
