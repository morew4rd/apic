#ifdef APIC_REFLECT

#include "apic.h"

/*
    APIC Extension for pretty printing exported names.
*/

#include <stdio.h>

static void apic_ext_print_raw(apic_Exports *exports) {
    printf("\n=============================================\n\n");
    printf("apic_Exports: %s\n\nDescription: %s\n\n", exports->name, exports->doc);

    printf("=== apic_Structs (%d) ===\n", exports->struct_count);
    for(int i = 0; i < exports->struct_count; i++) {
        apic_Struct *st = exports->structs[i];
        printf("%s: %s\napic_Fields (%d):\n", st->name, st->doc, st->count);
        for(int j = 0; j < st->count; j++) {
            apic_Field f = st->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < exports->struct_count-1) printf("\n");
    }

    printf("\n=== apic_Unions (%d) ===\n", exports->union_count);
    for(int i = 0; i < exports->union_count; i++) {
        apic_Union *u = exports->unions[i];
        printf("%s: %s\napic_Fields (%d):\n", u->name, u->doc, u->count);
        for(int j = 0; j < u->count; j++) {
            apic_Field f = u->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < exports->union_count-1) printf("\n");
    }

    printf("\n=== apic_Functions (%d) ===\n", exports->func_count);
    for(int i = 0; i < exports->func_count; i++) {
        apic_Func *func = exports->funcs[i];
        printf("%s: %s\nReturns: %s\napic_Args (%d):\n",
               func->name, func->doc, func->ret, func->count);
        for(int j = 0; j < func->count; j++) {
            apic_Arg a = func->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < exports->func_count-1) printf("\n");
    }

    printf("\n=== apic_FuncPtrs (%d) ===\n", exports->funcptr_count);
    for(int i = 0; i < exports->funcptr_count; i++) {
        apic_FuncPtr *lam = exports->funcptrs[i];
        printf("%s: %s\nReturns: %s\napic_Args (%d):\n",
               lam->name, lam->doc, lam->ret, lam->count);
        for(int j = 0; j < lam->count; j++) {
            apic_Arg a = lam->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < exports->funcptr_count-1) printf("\n");
    }

    printf("\n=== apic_Enums (%d) ===\n", exports->enum_count);
    for(int i = 0; i < exports->enum_count; i++) {
        apic_Enum *en = exports->enums[i];
        printf("%s: %s\nEntries (%d):\n", en->name, en->doc, en->count);
        for(int j = 0; j < en->count; j++) {
            apic_apic_EnumEntry e = en->entries[j];
            printf("  %s = %d (%s, %s)\n", e.name, e.value, e.str, e.doc);
        }
        if(i < exports->enum_count-1) printf("\n");
    }


    printf("\n=== apic_Aliass (%d) ===\n", exports->typedef_count);
    for(int i = 0; i < exports->typedef_count; i++) {
        apic_Alias *t = exports->aliases[i];
        printf("%s = %s (%s)\n", t->name, t->type, t->doc);
    }
    printf("\n=============================================\n\n");
}

#endif // APIC_REFLECT
