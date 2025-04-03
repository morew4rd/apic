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
    TypedExports* tex = apic_ext_create_typed_exports(&MyExports);
    apic_ext_print_typed(tex);

    // printf("\n\n----------------------------------------------------------\n");
    printf("\n*/\n");
    // apic_ext_gen_lua51_bindings(&MyExports, stdout);


    printf("\n\n//----------------------------------------------------------\n\n");
    return 0;
}