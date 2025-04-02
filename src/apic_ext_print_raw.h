#ifdef APIC_REFLECT

#include "apic.h"

/*
    APIC Extension for pretty printing exported names.
*/

#include <stdio.h>

static void apic_ext_print_raw(Exports *exports) {
    printf("\n=============================================\n\n");
    printf("Exports: %s\n\nDescription: %s\n\n", exports->name, exports->doc);

    printf("=== Structs (%d) ===\n", exports->struct_count);
    for(int i = 0; i < exports->struct_count; i++) {
        Struct *st = exports->structs[i];
        printf("%s: %s\nFields (%d):\n", st->name, st->doc, st->count);
        for(int j = 0; j < st->count; j++) {
            Field f = st->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < exports->struct_count-1) printf("\n");
    }

    printf("\n=== Unions (%d) ===\n", exports->union_count);
    for(int i = 0; i < exports->union_count; i++) {
        Union *u = exports->unions[i];
        printf("%s: %s\nFields (%d):\n", u->name, u->doc, u->count);
        for(int j = 0; j < u->count; j++) {
            Field f = u->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < exports->union_count-1) printf("\n");
    }

    printf("\n=== Functions (%d) ===\n", exports->func_count);
    for(int i = 0; i < exports->func_count; i++) {
        Func *func = exports->funcs[i];
        printf("%s: %s\nReturns: %s\nArgs (%d):\n",
               func->name, func->doc, func->ret, func->count);
        for(int j = 0; j < func->count; j++) {
            Arg a = func->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < exports->func_count-1) printf("\n");
    }

    printf("\n=== Lambdas (%d) ===\n", exports->lambda_count);
    for(int i = 0; i < exports->lambda_count; i++) {
        Lambda *lam = exports->lambdas[i];
        printf("%s: %s\nReturns: %s\nArgs (%d):\n",
               lam->name, lam->doc, lam->ret, lam->count);
        for(int j = 0; j < lam->count; j++) {
            Arg a = lam->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < exports->lambda_count-1) printf("\n");
    }

    printf("\n=== Enums (%d) ===\n", exports->enum_count);
    for(int i = 0; i < exports->enum_count; i++) {
        Enum *en = exports->enums[i];
        printf("%s: %s\nEntries (%d):\n", en->name, en->doc, en->count);
        for(int j = 0; j < en->count; j++) {
            EnumEntry e = en->entries[j];
            printf("  %s = %d (%s, %s)\n", e.name, e.value, e.str, e.doc);
        }
        if(i < exports->enum_count-1) printf("\n");
    }

    printf("\n=== Variables (%d) ===\n", exports->var_count);
    for(int i = 0; i < exports->var_count; i++) {
        Var *v = exports->vars[i];
        printf("%s: %s (%s)\n", v->name, v->type, v->doc);
    }

    printf("\n=== Typedefs (%d) ===\n", exports->typedef_count);
    for(int i = 0; i < exports->typedef_count; i++) {
        Typedef *t = exports->typedefs[i];
        printf("%s = %s (%s)\n", t->name, t->type, t->doc);
    }
    printf("\n=============================================\n\n");
}

#endif // APIC_REFLECT
