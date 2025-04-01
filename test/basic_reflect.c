#define APIC_REFLECT
#include "basic_apic.h"
#include "apic_ext_prettyprint.h"

int main() {
    apic_ext_prettyprint(&MyExports);
    return 0;
}