#define APIC_REFLECT
#define APIC_REFLECT_PRETTYPRINT
#include "../../src/apic_all.h"
#include "basic.apic.h"


int main() {
    printf("\n\n//----------------------------------------------------------\n\n");
    apicext_gen_exported_c_header(&mysimplelib);

    printf("\n/*");
    printf("\n\n//----------------------------------------------------------\n\n");
    apic_prettyprint(&mysimplelib);

    printf("\n\n//----------------------------------------------------------\n\n");
    apicext_typecheck(&mysimplelib);
    apic_TypedExports* tex = apicext_create_typed_exports(&mysimplelib);
    apicext_print_typed(tex);

    // printf("\n\n----------------------------------------------------------\n");
    printf("\n*/\n");
    // apicext_gen_lua51_bindings(&mysimplelib, stdout);


    printf("\n\n//----------------------------------------------------------\n\n");
    return 0;
}