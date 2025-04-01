#define APIC_REFLECT
#include "basic_apic.h"
#include "apic_ext_prettyprint.h"
#include "apic_ext_print_public_header.h"
#include "apic_ext_typecheck.h"
#include "apic_ext_gen_lua51_bindings.h"

int main() {
    printf("\n\n//----------------------------------------------------------\n\n");
    apic_ext_print_public_header(&MyExports);

    printf("\n/*");
    printf("\n\n//----------------------------------------------------------\n\n");
    apic_ext_prettyprint(&MyExports);
    printf("\n\n//----------------------------------------------------------\n\n");
    apic_ext_typecheck(&MyExports);

    printf("\n*/");
    printf("\n\n----------------------------------------------------------\n\n");
    apic_ext_gen_lua51_bindings(&MyExports, stdout);

    printf("\n\n//----------------------------------------------------------\n\n");
    return 0;
}