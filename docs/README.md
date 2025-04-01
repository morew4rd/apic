# APIC

**THIS IS WIP. DON'T USE YET **

## Overview

API definition DSL in C.

Use a basicly defined API structure. By default it can be used
as a replacement for a header file.

It can also be used for compile time reflection for types defined with this DSL.

## Example

See `./test` directory.

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
