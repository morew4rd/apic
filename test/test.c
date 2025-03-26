#include "apic.h"
#include "geometry.api.c"

int main() {
    printf("/*\n");
    pretty_print_module(&geometry);
    printf("*/\n");
    printf("\n\n/******* Generated Header ********/\n\n");
    generate_c_header(&geometry);
    printf("\n\n/******* Lua Bindings ********/\n\n");
    generate_lua_bindings(&geometry);
    return 0;
}