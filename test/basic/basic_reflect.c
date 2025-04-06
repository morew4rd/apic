#define APIC_REFLECT
#include "basic.apic.h"


int main() {
    printf("\n\n//----------------------------------------------------------\n\n");
    apic_ext_gen_exported_c_header(&mysimplelib);

    printf("\n/*");
    printf("\n\n//----------------------------------------------------------\n\n");
    apic_ext_print_raw(&mysimplelib);

    printf("\n\n//----------------------------------------------------------\n\n");
    apic_ext_typecheck(&mysimplelib);
    Typed_MySimpleLib* tex = apic_ext_create_typed_exports(&mysimplelib);
    apic_ext_print_typed(tex);

    // printf("\n\n----------------------------------------------------------\n");
    printf("\n*/\n");
    // apic_ext_gen_lua51_bindings(&mysimplelib, stdout);


    printf("\n\n//----------------------------------------------------------\n\n");
    return 0;
}