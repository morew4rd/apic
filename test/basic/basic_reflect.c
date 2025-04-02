#define APIC_REFLECT
#include "basic_apic.h"


int main() {
    printf("\n\n//----------------------------------------------------------\n\n");
    apic_ext_gen_exported_c_header(&MyExports);

    printf("\n/*");
    printf("\n\n//----------------------------------------------------------\n\n");
    apic_ext_print_raw(&MyExports);
    printf("\n\n//----------------------------------------------------------\n\n");
    apic_ext_typecheck(&MyExports);

    printf("\n\n----------------------------------------------------------\n");
    printf("\n*/\n");
    apic_ext_gen_lua51_bindings(&MyExports, stdout);

    printf("\n\n//----------------------------------------------------------\n\n");
    return 0;
}