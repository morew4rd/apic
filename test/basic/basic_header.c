#include "basic.apic.h"
#include <stdio.h>

// will be assigned to a funcptr value
int add_impl(int a, float b) { return a + (int)b; }

int main() {
    Vec2I v2 = {10, 20};
    printf("Vec2I: %d, %d\n", v2.x, v2.y);

    // Using typedef
    Byte byte = 255;
    printf("Byte value: %u\n", byte);

    Buffer buf = {.data = {0}, .size = 0};
    Matrix mat = {.values = {1.0f}, .rows = 4, .cols = 4};

    printf("Matrix[15]: %.1f\n", mat.values[15]);

    printf("5 + 3.7 = %d\n", add_mixed(5, 3.7f));

    Calculator calc = {.adder = add_impl, .value = 3.14f};
    printf("5 + 3.7 via funcptr: %d\n", calc.adder(5, 3.7f));

    COLOR c = RED;
    printf("Color value: %d\n", c);

    return 0;
}


int add_mixed(int a, float b) { return a + (int)b; }