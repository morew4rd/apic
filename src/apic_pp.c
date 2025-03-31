#include "apic_pp.h"
#include <stdio.h>

void apic_print_module(Module *module) {
    printf("Module: %s\nDescription: %s\n\n", module->name, module->doc);

    printf("=== Structs (%d) ===\n", module->struct_count);
    for(int i = 0; i < module->struct_count; i++) {
        Struct *st = module->structs[i];
        printf("%s: %s\nFields (%d):\n", st->name, st->doc, st->count);
        for(int j = 0; j < st->count; j++) {
            Field f = st->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < module->struct_count-1) printf("\n");
    }

    printf("\n=== Unions (%d) ===\n", module->union_count);
    for(int i = 0; i < module->union_count; i++) {
        Union *u = module->unions[i];
        printf("%s: %s\nFields (%d):\n", u->name, u->doc, u->count);
        for(int j = 0; j < u->count; j++) {
            Field f = u->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < module->union_count-1) printf("\n");
    }

    printf("\n=== Functions (%d) ===\n", module->func_count);
    for(int i = 0; i < module->func_count; i++) {
        Func *func = module->funcs[i];
        printf("%s: %s\nReturns: %s\nArgs (%d):\n",
               func->name, func->doc, func->ret, func->count);
        for(int j = 0; j < func->count; j++) {
            Arg a = func->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < module->func_count-1) printf("\n");
    }

    printf("\n=== Lambdas (%d) ===\n", module->lambda_count);
    for(int i = 0; i < module->lambda_count; i++) {
        Lambda *lam = module->lambdas[i];
        printf("%s: %s\nReturns: %s\nArgs (%d):\n",
               lam->name, lam->doc, lam->ret, lam->count);
        for(int j = 0; j < lam->count; j++) {
            Arg a = lam->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < module->lambda_count-1) printf("\n");
    }

    printf("\n=== Enums (%d) ===\n", module->enum_count);
    for(int i = 0; i < module->enum_count; i++) {
        Enum *en = module->enums[i];
        printf("%s: %s\nEntries (%d):\n", en->name, en->doc, en->count);
        for(int j = 0; j < en->count; j++) {
            EnumEntry e = en->entries[j];
            printf("  %s = %d (%s, %s)\n", e.name, e.value, e.str, e.doc);
        }
        if(i < module->enum_count-1) printf("\n");
    }

    printf("\n=== Variables (%d) ===\n", module->var_count);
    for(int i = 0; i < module->var_count; i++) {
        Var *v = module->vars[i];
        printf("%s: %s (%s)\n", v->name, v->type, v->doc);
    }

    printf("\n=== Typedefs (%d) ===\n", module->typedef_count);
    for(int i = 0; i < module->typedef_count; i++) {
        Typedef *t = module->typedefs[i];
        printf("%s = %s (%s)\n", t->name, t->type, t->doc);
    }
}
