#ifdef APIC_REFLECT

#include "apic.h"
#include <stdio.h>
#include <string.h>

static void __impl_ext_print_public_header(apic_Exports *exports, FILE *out) {
    fprintf(out, "// =============================================\n");
    fprintf(out, "// Public API Header: %s\n", exports->name);
    fprintf(out, "// Description: %s\n", exports->doc);
    fprintf(out, "// =============================================\n\n");
    fprintf(out, "#ifndef %s_PUBLIC_H\n", exports->name);
    fprintf(out, "#define %s_PUBLIC_H\n\n", exports->name);
    fprintf(out, "#include <stdint.h>\n\n");

    // Print aliases first
    if (exports->typedef_count > 0) {
        fprintf(out, "/* =============== apic_Aliass =============== */\n");
        for (int i = 0; i < exports->typedef_count; i++) {
            apic_Alias *t = exports->aliases[i];
            fprintf(out, "// %s\n", t->doc);
            fprintf(out, "typedef %s %s;\n\n", t->type, t->name);
        }
    }

    // Print enums
    if (exports->enum_count > 0) {
        fprintf(out, "/* ================ apic_Enums ================ */\n");
        for (int i = 0; i < exports->enum_count; i++) {
            apic_Enum *en = exports->enums[i];
            fprintf(out, "// %s\n", en->doc);
            fprintf(out, "typedef enum %s {\n", en->name);
            for (int j = 0; j < en->count; j++) {
                apic_apic_EnumEntry e = en->entries[j];
                fprintf(out, "    %s = %d, // %s (%s)\n",
                       e.name, e.value, e.str, e.doc);
            }
            fprintf(out, "} %s;\n\n", en->name);
        }
    }

    // Print forward declarations
    if (exports->struct_count > 0) {
        fprintf(out, "/* ========== Forward Declarations ========= */\n");
        for (int i = 0; i < exports->struct_count; i++) {
            apic_Struct *st = exports->structs[i];
            fprintf(out, "typedef struct %s %s;\n", st->name, st->name);
        }
        fprintf(out, "\n");
    }

    // Print structs with fixed array syntax
    if (exports->struct_count > 0) {
        fprintf(out, "/* ============== apic_Structs ============== */\n");
        for (int i = 0; i < exports->struct_count; i++) {
            apic_Struct *st = exports->structs[i];
            fprintf(out, "// %s\n", st->doc);
            fprintf(out, "struct %s {\n", st->name);

            for (int j = 0; j < st->count; j++) {
                apic_Field f = st->fields[j];
                char *bracket = strchr(f.type, '[');

                if (bracket) {
                    // Handle array types
                    fprintf(out, "    %.*s %s%s; // %s\n",
                           (int)(bracket - f.type), f.type,
                           f.name, bracket, f.doc);
                } else {
                    // Normal types
                    fprintf(out, "    %s %s; // %s\n", f.type, f.name, f.doc);
                }
            }
            fprintf(out, "};\n\n");
        }
    }

    // Print unions
    if (exports->union_count > 0) {
        fprintf(out, "/* =============== apic_Unions =============== */\n");
        for (int i = 0; i < exports->union_count; i++) {
            apic_Union *u = exports->unions[i];
            fprintf(out, "// %s\n", u->doc);
            fprintf(out, "typedef union %s {\n", u->name);
            for (int j = 0; j < u->count; j++) {
                apic_Field f = u->fields[j];
                fprintf(out, "    %s %s; // %s\n", f.type, f.name, f.doc);
            }
            fprintf(out, "} %s;\n\n", u->name);
        }
    }

    // Print funcptrs
    if (exports->funcptr_count > 0) {
        fprintf(out, "/* ============== apic_FuncPtrs ============== */\n");
        for (int i = 0; i < exports->funcptr_count; i++) {
            apic_FuncPtr *lam = exports->funcptrs[i];
            fprintf(out, "// %s\n", lam->doc);
            fprintf(out, "typedef %s (*%s)(", lam->ret, lam->name);
            for (int j = 0; j < lam->count; j++) {
                apic_Arg a = lam->args[j];
                fprintf(out, "%s %s", a.type, a.name);
                if (j < lam->count - 1) fprintf(out, ", ");
            }
            fprintf(out, ");\n\n");
        }
    }


    // Print functions
    if (exports->func_count > 0) {
        fprintf(out, "/* ============= apic_Functions ============= */\n");
        for (int i = 0; i < exports->func_count; i++) {
            apic_Func *func = exports->funcs[i];
            fprintf(out, "// %s\n", func->doc);
            fprintf(out, "%s %s(", func->ret, func->name);
            for (int j = 0; j < func->count; j++) {
                apic_Arg a = func->args[j];
                fprintf(out, "%s %s", a.type, a.name);
                if (j < func->count - 1) fprintf(out, ", ");
            }
            fprintf(out, ");\n\n");
        }
    }

    fprintf(out, "#endif // %s_PUBLIC_H\n", exports->name);
}

static inline void apic_ext_gen_exported_c_header(apic_Exports *exports) {
    __impl_ext_print_public_header(exports, stdout);
}

#endif // APIC_REFLECT

