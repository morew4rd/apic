#define APIC_REFLECT
#define APIC_REFLECT_PRETTYPRINT
#include "apic_all.h"
#include "kyte.apic.h"


/**
 * @brief The main function serves as the entry point for the kyte_reflect application.
 *
 * This function initializes the application and executes the necessary operations
 * to perform reflection on the kyte system.
 *
 * @return int Returns 0 on successful execution, non-zero on failure.
 */
int main() {
    apic_TypedExports* tex = apicext_create_typed_exports(&kyte_api);
    if (!tex) {
        fprintf(stderr, "Failed to create typed exports. Exiting.\n");
        return 1;
    }
    if (tex->context.error_count > 0) {
         fprintf(stderr, "Warning: Type resolution errors occurred.\n");
         // Decide if you want to proceed with header generation despite errors
    }


    printf("\n\n//----------------------------------------------------------\n");
    printf("\n/* --- Raw Reflection Data (Pretty Print) ---\n");
    apic_prettyprint(&kyte_api); // Can still print raw data if needed
    printf("\n--- End Raw Reflection --- */\n");


    printf("\n\n// --- Generating Public C Header ---\n");
    printf("//----------------------------------------------------------\n\n");

    // 2. Generate the Header using the Typed Exports
    apicext_gen_exported_c_header(tex); // Prints to stdout

    // OR: apicext_gen_exported_c_header_to_file(tex, "my_generated_header.h");

    printf("\n// --- Generating Lua 5.1 Bindings to test/output/basic_lua_bindings.c ---\n");
    // 3. Generate Lua Bindings
    apicext_gen_lua51_bindings_to_file(tex, "kytehehe", "kyte_generated.h", "test/output/kyte_lua_bindings.c");

    printf("// Lua Bindings generation done.\n");


    // printf("\n\n//----------------------------------------------------------\n");
    // printf("\n/* --- Typed API Representation ---\n");
    // apicext_print_typed(tex); // Print the resolved typed structure
    // printf("\n--- End Typed API --- */\n");

    // 3. Clean up Typed Exports
    cleanup_typed_exports(tex);

    return 0;
}
