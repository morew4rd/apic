#define APIC_REFLECT
#include "basic_apic.h"
#include "apic_ext_prettyprint.h"
#include "apic_ext_print_public_header.h"

int main() {
    apic_ext_prettyprint(&MyExports);

    printf("\n----------------------------------------------------------\n");
    printf("\n----------------------------------------------------------\n");
    printf("\n----------------------------------------------------------\n\n\n");

    apic_ext_print_public_header(&MyExports);

    return 0;
}