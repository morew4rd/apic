// ================ File: src/apicext_gen_exported_c_header.h ================
#ifndef APICEXT_GEN_EXPORTED_C_HEADER_H
#define APICEXT_GEN_EXPORTED_C_HEADER_H

#ifdef APIC_REFLECT

#include "apic_def.h"   // For basic APIC types if needed indirectly
#include "apic_types.h" // Include the new typed structures and helpers
#include <stdio.h>
#include <string.h>


static const char* type_qualifier_str(unsigned qualifiers) {
    static char buf[64]; // Static buffer, not thread-safe
    buf[0] = '\0';

    if (qualifiers & TQ_CONST) strcat(buf, "const ");
    if (qualifiers & TQ_VOLATILE) strcat(buf, "volatile ");
    if (qualifiers & TQ_RESTRICT) strcat(buf, "restrict ");

    // Remove trailing space if any qualifiers were added
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == ' ') {
        buf[len - 1] = '\0';
    }
    return buf;
}


// Improved type_to_str to handle pointers/arrays/qualifiers better
static void type_to_str_recursive(Type* type, char* buf, size_t size) {
     if (!type || !buf || size == 0) {
         if (buf && size > 0) buf[0] = '\0';
         return;
     }

     char current_part[256] = {0}; // Buffer for the current part of the type string

     // 1. Handle base type name and fundamental kind
     switch (type->kind) {
         case TK_PRIMITIVE: {
             const char* prim_names[] = { // Must match PrimitiveType enum order closely
                 "void", "char", "unsigned char", "signed char", "short", "unsigned short",
                 "int", "unsigned int", "long", "unsigned long", "long long",
                 "unsigned long long", "float", "double", "bool", "size_t",
                 "int8_t", "uint8_t", "int16_t", "uint16_t",
                 "int32_t", "uint32_t", "int64_t", "uint64_t",
                 "intptr_t", "uintptr_t", "FILE", "char*", /*PT_CSTRING placeholder*/
                 "custom" /*PT_CUSTOM*/
             };
              // Correct handling for CSTRING based on context might be needed
              // For now, use the name stored in the Type if available, else map enum
              if (type->data.primitive < sizeof(prim_names)/sizeof(prim_names[0])) {
                  snprintf(current_part, sizeof(current_part), "%s", prim_names[type->data.primitive]);
              } else if (type->name){
                   snprintf(current_part, sizeof(current_part), "%s", type->name); // Use stored name for custom
              } else {
                   snprintf(current_part, sizeof(current_part), "unknown_primitive");
              }
             break;
         }
         case TK_STRUCT:
             snprintf(current_part, sizeof(current_part), "struct %s", type->data.struct_type ? type->data.struct_type->name : (type->name ? type->name : "unknown_struct"));
             break;
         case TK_UNION:
              snprintf(current_part, sizeof(current_part), "union %s", type->data.union_type ? type->data.union_type->name : (type->name ? type->name : "unknown_union"));
             break;
         case TK_ENUM:
              snprintf(current_part, sizeof(current_part), "enum %s", type->data.enum_type ? type->data.enum_type->name : (type->name ? type->name : "unknown_enum"));
             break;
         case TK_TYPEDEF:
             // For a typedef, usually just print its name
              snprintf(current_part, sizeof(current_part), "%s", type->name ? type->name : "unknown_typedef");
             break;
         case TK_FUNCPTR:
              // For a funcptr typedef, print its name
              snprintf(current_part, sizeof(current_part), "%s", type->data.function.name ? type->data.function.name : (type->name ? type->name : "unknown_funcptr"));
             break;

         // Recursive cases need special handling for string construction order
         case TK_POINTER:
         case TK_ARRAY:
          case TK_FUNCTION: // Should function signatures be printable directly? Usually via funcptr.
              // These are handled below after qualifiers
              break;

         case TK_UNRESOLVED:
              snprintf(current_part, sizeof(current_part), "/*unresolved*/%s", type->name ? type->name : "??");
              break;
         default:
             snprintf(current_part, sizeof(current_part), "unknown_kind(%d)", type->kind);
     }

     // 2. Add qualifiers (if any) before the base type name
     const char* quals = type_qualifier_str(type->qualifiers);
     if (quals[0] != '\0') {
         char temp[256];
         snprintf(temp, sizeof(temp), "%s %s", quals, current_part);
         strncpy(current_part, temp, sizeof(current_part) - 1);
         current_part[sizeof(current_part) - 1] = '\0';
     }

      // 3. Handle recursive construction for Pointer/Array
      if (type->kind == TK_POINTER) {
          char base_buf[256];
          type_to_str_recursive(type->data.pointer.base_type, base_buf, sizeof(base_buf));
          snprintf(buf, size, "%s*", base_buf);
      } else if (type->kind == TK_ARRAY) {
           char base_buf[256];
           type_to_str_recursive(type->data.array.element_type, base_buf, sizeof(base_buf));
           if (type->data.array.array_size > 0) {
                snprintf(buf, size, "%s[%zu]", base_buf, type->data.array.array_size);
           } else {
                snprintf(buf, size, "%s[]", base_buf); // Unknown size
           }
       } else {
           // For non-recursive types, copy the constructed part
           strncpy(buf, current_part, size - 1);
           buf[size - 1] = '\0';
       }
       // TODO: Function signature formatting if needed for TK_FUNCTION directly
}



static void type_to_str(Type* type, char* buf, size_t size, int indent) {
    // Indentation complicates recursive construction, remove for now
    (void)indent; // Mark as unused
    type_to_str_recursive(type, buf, size);
}


static void print_struct(apic_StructType* st, int indent) {
   char indent_str[32] = {0};
   for (int i = 0; i < indent; i++) strcat(indent_str, "  ");

   printf("%sstruct %s {\n", indent_str, st->name);
   if (st->doc && st->doc[0] != '\0') printf("%s  // %s\n", indent_str, st->doc);

   for (size_t i = 0; i < st->num_fields; i++) {
       apic_TypedField* f = &st->fields[i];
       char type_buf[256];
       type_to_str(f->type, type_buf, sizeof(type_buf), indent + 1);
       // Adjust spacing: Type first, then name
       printf("%s  %-20s %s;", indent_str, type_buf, f->name);
       if (f->doc && f->doc[0] != '\0') printf(" // %s", f->doc);
       printf("\n");
   }
   printf("%s};\n\n", indent_str);
}

static void print_union(apic_UnionType* un, int indent) {
    char indent_str[32] = {0};
    for (int i = 0; i < indent; i++) strcat(indent_str, "  ");

    printf("%sunion %s {\n", indent_str, un->name);
    if (un->doc && un->doc[0] != '\0') printf("%s  // %s\n", indent_str, un->doc);

    for (size_t i = 0; i < un->num_fields; i++) {
        apic_TypedField* f = &un->fields[i];
        char type_buf[256];
        type_to_str(f->type, type_buf, sizeof(type_buf), indent + 1);
        printf("%s  %-20s %s;", indent_str, type_buf, f->name);
        if (f->doc && f->doc[0] != '\0') printf(" // %s", f->doc);
        printf("\n");
    }
    printf("%s};\n\n", indent_str);
}


static void print_enum(apic_EnumType* en, int indent) {
   char indent_str[32] = {0};
   for (int i = 0; i < indent; i++) strcat(indent_str, "  ");

   printf("%senum %s {\n", indent_str, en->name);
   if (en->doc && en->doc[0] != '\0') printf("%s  // %s\n", indent_str, en->doc);

   for (size_t i = 0; i < en->num_entries; i++) {
       apic_TypedEnumEntry* e = &en->entries[i];
       printf("%s  %s = %d", indent_str, e->name, e->value);
       if (i < en->num_entries - 1) printf(",");
       // Combine string and doc if present
        char comment[256] = {0};
        if (e->str_value && e->str_value[0] != '\0') {
             snprintf(comment, sizeof(comment), "%s", e->str_value);
        }
        if (e->doc && e->doc[0] != '\0') {
             if (comment[0] != '\0') strncat(comment, ", ", sizeof(comment) - strlen(comment) - 1);
             strncat(comment, e->doc, sizeof(comment) - strlen(comment) - 1);
        }
       if (comment[0] != '\0') printf(" // %s", comment);
       printf("\n");
   }
   printf("%s};\n\n", indent_str);
}

static void print_funcptr(apic_TypedExports* tex, Type* fp_type, int indent) { // Added tex parameter
   char indent_str[32] = {0};
   for (int i = 0; i < indent; i++) strcat(indent_str, "  ");

   if (!tex || !fp_type || (fp_type->kind != TK_FUNCPTR && fp_type->kind != TK_FUNCTION)) {
        printf("%s// Invalid funcptr type or context\n", indent_str);
        return;
   }

   apic_FunctionTypeInfo* fpi = &fp_type->data.function;
   char ret_buf[256];
   // Use the helper function with indentation 0 for components
   type_to_str(fpi->return_type, ret_buf, sizeof(ret_buf), 0);

   // Find the original definition doc string and param names from exports
   const char* doc = NULL;
   apic_Exports* exports = tex->context.current_exports; // Get exports from context
   apic_FuncPtr* funcptr_def = NULL; // To store the original definition

    if (fp_type->kind == TK_FUNCPTR && fpi->name && exports) {
        for(int i=0; i < exports->funcptr_count; ++i) {
             // Compare the name stored in the FunctionTypeInfo with the definitions
             if(strcmp(exports->funcptrs[i]->name, fpi->name) == 0) {
                  funcptr_def = exports->funcptrs[i]; // Found the definition
                  doc = funcptr_def->doc;
                  break;
             }
        }
    }


   printf("%stypedef %s (*%s)(", indent_str, ret_buf, fpi->name ? fpi->name : "anonymous_funcptr");

   for (size_t j = 0; j < fpi->num_params; j++) {
       char param_buf[256];
       type_to_str(fpi->param_types[j], param_buf, sizeof(param_buf), 0);
       printf("%s", param_buf);

       // Print parameter name if we found the original definition
       if (funcptr_def && j < funcptr_def->count) {
           printf(" %s", funcptr_def->args[j].name);
       }

       if (j < fpi->num_params - 1) printf(", ");
   }
   // Handle void parameters (no arguments)
    if (fpi->num_params == 0) {
         printf("void");
    }

   printf(");");
   if (doc && doc[0] != '\0') printf(" // %s", doc);
   printf("\n");
}


// Helper to combine string value and doc for enum comments
static void format_enum_comment(char* buf, size_t size, const apic_TypedEnumEntry* e) {
    buf[0] = '\0';
    int first = 1;
    if (e->str_value && e->str_value[0] != '\0') {
        snprintf(buf, size, "%s", e->str_value);
        first = 0;
    }
    if (e->doc && e->doc[0] != '\0') {
        if (!first) strncat(buf, ", ", size - strlen(buf) - 1);
        strncat(buf, e->doc, size - strlen(buf) - 1);
    }
}

// Main implementation function, now takes apic_TypedExports
static void __impl_ext_print_public_header(apic_TypedExports *tex, FILE *out) {
    if (!tex) {
        fprintf(stderr, "Error: Cannot generate header from NULL TypedExports.\n");
        return;
    }
    apic_Exports* exports = tex->context.current_exports; // Get raw exports for docs if needed

    fprintf(out, "// =============================================\n");
    fprintf(out, "// Public API Header: %s\n", tex->name);
    if (tex->doc && tex->doc[0] != '\0') {
        fprintf(out, "// Description: %s\n", tex->doc);
    }
    fprintf(out, "// Generated by APIC reflection system\n");
    fprintf(out, "// =============================================\n\n");
    fprintf(out, "#ifndef %s_PUBLIC_H\n", tex->name);
    fprintf(out, "#define %s_PUBLIC_H\n\n", tex->name);
    fprintf(out, "#include <stdint.h> // Standard types\n");
    fprintf(out, "#include <stddef.h> // For size_t\n");
    fprintf(out, "#include <stdbool.h> // For bool\n");
    // Potentially include other common headers if needed based on types used
    fprintf(out, "\n");

    // --- Print aliases (typedefs) ---
    if (tex->num_aliases > 0) {
        fprintf(out, "// =============== Typedefs / Aliases ===============\n");
        for (size_t i = 0; i < tex->num_aliases; i++) {
            Type* t = tex->aliases[i];
            if (!t || t->kind != TK_TYPEDEF || !t->data.typedef_target) continue;

            char target_buf[256];
            type_to_str(t->data.typedef_target, target_buf, sizeof(target_buf), 0);

            // Find original doc
            const char* doc = NULL;
            apic_Alias* alias_def = find_typedef_def(exports, t->name);
            if (alias_def) doc = alias_def->doc;

            if (doc && doc[0] != '\0') fprintf(out, "// %s\n", doc);
            fprintf(out, "typedef %s %s;\n", target_buf, t->name);
        }
        fprintf(out, "\n");
    }

    // --- Print enums ---
    if (tex->num_enums > 0) {
        fprintf(out, "// =================== Enums ===================/\n");
        for (size_t i = 0; i < tex->num_enums; i++) {
            apic_EnumType *en = tex->enums[i];
            if (!en) continue;

            if (en->doc && en->doc[0] != '\0') fprintf(out, "// %s\n", en->doc);
            fprintf(out, "typedef enum %s {\n", en->name);
            for (size_t j = 0; j < en->num_entries; j++) {
                apic_TypedEnumEntry *e = &en->entries[j];
                char comment_buf[256];
                format_enum_comment(comment_buf, sizeof(comment_buf), e);

                fprintf(out, "    %s = %d", e->name, e->value);
                if (j < en->num_entries - 1) fprintf(out, ",");
                if (comment_buf[0] != '\0') fprintf(out, " // %s", comment_buf);
                fprintf(out, "\n");
            }
            fprintf(out, "} %s;\n\n", en->name);
        }
    }

    // --- Print forward declarations for structs ---
    // Unions are typically fully defined with typedef, structs often need forward decls
    if (tex->num_structs > 0) {
        fprintf(out, "// ============ Forward Declarations ============/\n");
        for (size_t i = 0; i < tex->num_structs; i++) {
            apic_StructType *st = tex->structs[i];
            if (!st) continue;
            // Create a typedef for the struct name for easier use
            fprintf(out, "typedef struct %s %s;\n", st->name, st->name);
        }
        fprintf(out, "\n");
    }
     // Forward declare funcptrs too? Usually the typedef serves this purpose. Skip for now.


    // --- Print Function Pointer Typedefs ---
     if (tex->num_funcptrs > 0) {
         fprintf(out, "// ============ Function Pointer Types ============/\n");
         for (size_t i = 0; i < tex->num_funcptrs; i++) {
             Type* fp_type = tex->funcptrs[i];
             if (!fp_type || fp_type->kind != TK_FUNCPTR) continue;

             apic_FunctionTypeInfo* fpi = &fp_type->data.function;
             if (!fpi->return_type) continue; // Skip if resolution failed

             char ret_buf[256];
             type_to_str(fpi->return_type, ret_buf, sizeof(ret_buf), 0);

             // Find the original definition for docs and param names
             const char* doc = NULL;
             apic_FuncPtr* funcptr_def = NULL;
             if (fpi->name && exports) {
                 for (int k = 0; k < exports->funcptr_count; ++k) {
                     if (strcmp(exports->funcptrs[k]->name, fpi->name) == 0) {
                         funcptr_def = exports->funcptrs[k];
                         doc = funcptr_def->doc;
                         break;
                     }
                 }
             }

             if (doc && doc[0] != '\0') fprintf(out, "// %s\n", doc);
             fprintf(out, "typedef %s (*%s)(", ret_buf, fpi->name ? fpi->name : "anonymous_funcptr");

             if (fpi->num_params == 0) {
                 fprintf(out, "void");
             } else {
                 for (size_t j = 0; j < fpi->num_params; j++) {
                     if (!fpi->param_types[j]) { // Check resolution success
                          fprintf(out, "/* Error: Unresolved param type */");
                          continue;
                     }
                     char param_buf[256];
                     type_to_str(fpi->param_types[j], param_buf, sizeof(param_buf), 0);
                     fprintf(out, "%s", param_buf);

                     // Print parameter name if available from original def
                     if (funcptr_def && j < funcptr_def->count) {
                         fprintf(out, " %s", funcptr_def->args[j].name);
                     }

                     if (j < fpi->num_params - 1) fprintf(out, ", ");
                 }
             }
             fprintf(out, ");\n\n");
         }
     }


    // --- Print struct definitions ---
    if (tex->num_structs > 0) {
        fprintf(out, "// ================ Structures ================/\n");
        for (size_t i = 0; i < tex->num_structs; i++) {
            apic_StructType *st = tex->structs[i];
             if (!st) continue;

            if (st->doc && st->doc[0] != '\0') fprintf(out, "// %s\n", st->doc);
            // Definition uses 'struct', typedef was done earlier
            fprintf(out, "struct %s {\n", st->name);

            for (size_t j = 0; j < st->num_fields; j++) {
                apic_TypedField *f = &st->fields[j];
                if (!f->type) { // Check resolution success
                     fprintf(out, "    /* Error: Unresolved type for field '%s' */;\n", f->name);
                     continue;
                }

                // *** Corrected Array Handling ***
                if (f->type->kind == TK_ARRAY) {
                    char base_type_buf[256];
                    type_to_str(f->type->data.array.element_type, base_type_buf, sizeof(base_type_buf), 0);
                    fprintf(out, "    %s %s", base_type_buf, f->name); // Print "base_type field_name"
                    if (f->type->data.array.array_size > 0) {
                        fprintf(out, "[%zu]", f->type->data.array.array_size); // Print "[size]"
                    } else {
                        fprintf(out, "[]"); // Incomplete array type (flexible array member?)
                    }
                    fprintf(out, ";"); // Print semicolon
                } else {
                    // Handle non-array types (including pointers, primitives, structs, etc.)
                    char type_buf[256];
                    type_to_str(f->type, type_buf, sizeof(type_buf), 0); // Get full type string
                    fprintf(out, "    %s %s;", type_buf, f->name); // Print "full_type field_name;"
                }
                // *** End Correction ***

                if (f->doc && f->doc[0] != '\0') fprintf(out, " // %s", f->doc);
                fprintf(out, "\n");
            }
            fprintf(out, "};\n\n");
        }
    }

    // --- Print union definitions ---
    if (tex->num_unions > 0) {
        fprintf(out, "// ================== Unions ==================/\n");
        for (size_t i = 0; i < tex->num_unions; i++) {
            apic_UnionType *u = tex->unions[i];
            if (!u) continue;

            if (u->doc && u->doc[0] != '\0') fprintf(out, "// %s\n", u->doc);
            // Unions are typically defined and typedef'd together
            fprintf(out, "typedef union %s {\n", u->name);
            for (size_t j = 0; j < u->num_fields; j++) {
                apic_TypedField *f = &u->fields[j];
                 if (!f->type) { // Check resolution success
                     fprintf(out, "    /* Error: Unresolved type for field '%s' */;\n", f->name);
                     continue;
                }

                // *** Corrected Array Handling (also needed for unions) ***
                 if (f->type->kind == TK_ARRAY) {
                     char base_type_buf[256];
                     type_to_str(f->type->data.array.element_type, base_type_buf, sizeof(base_type_buf), 0);
                     fprintf(out, "    %s %s", base_type_buf, f->name); // Print "base_type field_name"
                     if (f->type->data.array.array_size > 0) {
                         fprintf(out, "[%zu]", f->type->data.array.array_size); // Print "[size]"
                     } else {
                         fprintf(out, "[]");
                     }
                     fprintf(out, ";"); // Print semicolon
                 } else {
                     // Handle non-array types
                     char type_buf[256];
                     type_to_str(f->type, type_buf, sizeof(type_buf), 0); // Get full type string
                     fprintf(out, "    %s %s;", type_buf, f->name); // Print "full_type field_name;"
                 }
                 // *** End Correction ***

                if (f->doc && f->doc[0] != '\0') fprintf(out, " // %s", f->doc);
                fprintf(out, "\n");
            }
            fprintf(out, "} %s;\n\n", u->name);
        }
    }


    // --- Print function declarations ---
    if (tex->num_functions > 0) {
        fprintf(out, "// ================ Functions ================/\n");
        for (size_t i = 0; i < tex->num_functions; i++) {
            apic_TypedFunction *fn = tex->functions[i];
            if (!fn || !fn->return_type) continue; // Skip if resolution failed

            char ret_buf[256];
            type_to_str(fn->return_type, ret_buf, sizeof(ret_buf), 0);

            if (fn->doc && fn->doc[0] != '\0') fprintf(out, "// %s\n", fn->doc);
            fprintf(out, "%s %s(", ret_buf, fn->name);

            if (fn->num_params == 0) {
                fprintf(out, "void");
            } else {
                for (size_t j = 0; j < fn->num_params; j++) {
                    apic_TypedField *p = &fn->params[j];
                     if (!p->type) { // Check resolution success
                         fprintf(out, "/* Error: Unresolved param type */");
                         continue;
                     }
                    char param_buf[256];
                    type_to_str(p->type, param_buf, sizeof(param_buf), 0);
                    fprintf(out, "%s %s", param_buf, p->name);
                    if (j < fn->num_params - 1) fprintf(out, ", ");
                }
            }
            fprintf(out, ");\n\n");
        }
    }

    fprintf(out, "#endif // %s_PUBLIC_H\n", tex->name);
}

// Public inline function, now takes apic_TypedExports*
// The user must now call apicext_create_typed_exports first.
static inline void apicext_gen_exported_c_header(apic_TypedExports *tex) {
     // Default to stdout, could be parameterized later
    __impl_ext_print_public_header(tex, stdout);
}

// Optional: Function to write to a specific file
static inline void apicext_gen_exported_c_header_to_file(apic_TypedExports *tex, const char* filename) {
    if (!tex) {
        fprintf(stderr, "Error: Cannot generate header from NULL TypedExports.\n");
        return;
    }
     if (!filename) {
          fprintf(stderr, "Error: No filename provided for header generation.\n");
          return;
     }
    FILE *outfile = fopen(filename, "w");
    if (!outfile) {
        perror("Error opening output file for generated header");
        return;
    }
    __impl_ext_print_public_header(tex, outfile);
    fclose(outfile);
    // Optional: Keep this print statement or remove for cleaner tool output
    // printf("Generated header written to %s\n", filename);
}


#endif // APIC_REFLECT

#endif // APICEXT_GEN_EXPORTED_C_HEADER_H