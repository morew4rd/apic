#define APIC_REFLECT
#include "basic_apic.h"
#include "apic_ext_pp.h"


int main() {
    apic_ext_pp(&MyModule);
    return 0;
}