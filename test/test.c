#include "apic.h"
#include "geometry.api.c"
#include "lyte.api.c"

int main() {
    printf("/*\n");
    pretty_print_module(&lyte_core);
    printf("*/\n");
    printf("\n\n/******* Generated Header ********/\n\n");
    generate_c_header(&lyte_core);
    printf("\n\n/******* Lua Bindings ********/\n\n");
    generate_lua_bindings(&lyte_core);
    return 0;
}