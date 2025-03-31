#define APIC_REFLECT
#include "basic.h"

int main() {
    printf("Module: %s\nDescription: %s\n\n", MyModule.name, MyModule.doc);

    printf("=== Structs (%d) ===\n", MyModule.struct_count);
    for(int i = 0; i < MyModule.struct_count; i++) {
        Struct *st = MyModule.structs[i];
        printf("%s: %s\nFields (%d):\n", st->name, st->doc, st->count);
        for(int j = 0; j < st->count; j++) {
            Field f = st->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < MyModule.struct_count-1) printf("\n");
    }

    printf("\n=== Unions (%d) ===\n", MyModule.union_count);
    for(int i = 0; i < MyModule.union_count; i++) {
        Union *u = MyModule.unions[i];
        printf("%s: %s\nFields (%d):\n", u->name, u->doc, u->count);
        for(int j = 0; j < u->count; j++) {
            Field f = u->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < MyModule.union_count-1) printf("\n");
    }

    printf("\n=== Functions (%d) ===\n", MyModule.func_count);
    for(int i = 0; i < MyModule.func_count; i++) {
        Func *func = MyModule.funcs[i];
        printf("%s: %s\nReturns: %s\nArgs (%d):\n",
               func->name, func->doc, func->ret, func->count);
        for(int j = 0; j < func->count; j++) {
            Arg a = func->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < MyModule.func_count-1) printf("\n");
    }

    printf("\n=== Lambdas (%d) ===\n", MyModule.lambda_count);
    for(int i = 0; i < MyModule.lambda_count; i++) {
        Lambda *lam = MyModule.lambdas[i];
        printf("%s: %s\nReturns: %s\nArgs (%d):\n",
               lam->name, lam->doc, lam->ret, lam->count);
        for(int j = 0; j < lam->count; j++) {
            Arg a = lam->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < MyModule.lambda_count-1) printf("\n");
    }

    printf("\n=== Enums (%d) ===\n", MyModule.enum_count);
    for(int i = 0; i < MyModule.enum_count; i++) {
        Enum *en = MyModule.enums[i];
        printf("%s: %s\nEntries (%d):\n", en->name, en->doc, en->count);
        for(int j = 0; j < en->count; j++) {
            EnumEntry e = en->entries[j];
            printf("  %s = %d (%s, %s)\n", e.name, e.value, e.str, e.doc);
        }
        if(i < MyModule.enum_count-1) printf("\n");
    }

    printf("\n=== Variables (%d) ===\n", MyModule.var_count);
    for(int i = 0; i < MyModule.var_count; i++) {
        Var *v = MyModule.vars[i];
        printf("%s: %s (%s)\n", v->name, v->type, v->doc);
    }

    printf("\n=== Typedefs (%d) ===\n", MyModule.typedef_count);
    for(int i = 0; i < MyModule.typedef_count; i++) {
        Typedef *t = MyModule.typedefs[i];
        printf("%s = %s (%s)\n", t->name, t->type, t->doc);
    }

    return 0;
}