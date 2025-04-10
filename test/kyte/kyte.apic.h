// ================ File: kyte.apic.h ================
#ifndef _KYTE_APIC_H_
#define _KYTE_APIC_H_

#include "apic_def.h"

#include "keys.apic.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// --- Assumed Base Type Aliases (Adjust if defined elsewhere, e.g., moreward.h) ---
// These ensure the reflection system knows about these types.
ALIAS(IErr, int)
ALIAS(Bool, bool) // Requires <stdbool.h> implicitly or via other includes
ALIAS(F32, float)
ALIAS(F64, double)
ALIAS(I32, int)
// ALIAS(ptrdiff_t, size_t)

// Aliases for input types (Assuming defined in _keys.h or similar)
// ALIAS(KyteKey, enum KyteKey)           // Placeholder - adjust actual type if known
// ALIAS(KyteMouseButton, enum KyteMouseButton) // Placeholder
// ALIAS(KyteGamepadButton, enum KyteGamepadButton) // Placeholder
// ALIAS(KyteGamepadAxis, enum KyteGamepadAxis)   // Placeholder
// TODO: include these directly in the API
// ALIAS(KyteKey, int)           // Placeholder - adjust actual type if known
// ALIAS(KyteMouseButton, int) // Placeholder
// ALIAS(KyteGamepadButton, int) // Placeholder
// ALIAS(KyteGamepadAxis, int)   // Placeholder


// --- Constants ---
#define KYTE_MAX_VERTICES 6553600
#define K_TEXTINPUT_MAX_CODEPOINTS 32

// --- Opaque Struct Typedefs (Forward Declarations via Alias) ---
ALIAS(KyteBlob, struct KyteBlob)
ALIAS(KyteImage, struct KyteImage)
ALIAS(KyteFont, struct KyteFont)

// --- Enums ---

// Note: Unnamed C enums need names for reflection.
ENUM_(KyteError, "Kyte Error Codes",
    N(K_ERR_STATE_NULL,         600001, ""),
    N(K_ERR_TICK_FN_NULL,       600002, ""),
    N(K_ERR_WINDOW_NO_WINDOW,   610001, ""),
    N(K_ERR_WINDOW_ALREADY_OPEN, 610002, ""),
    N(K_ERR_WINDOW_INIT_FAILED, 610003, ""),
    N(K_ERR_WINDOW_OPEN_FAILED, 610004, ""),
    N(K_ERR_GRAPHICS_INIT_SG,   620001, ""),
    N(K_ERR_GRAPHICS_INIT_SGP,  620002, ""),
    N(K_ERR_FS_CANT_ACCESS,     630001, ""),
    N(K_ERR_FS_NOT_DIR,         630002, ""),
    N(K_ERR_FS_NOT_FILE,        630003, ""),
    N(K_ERR_FS_UNKNOWN,         630004, ""),
    N(K_ERR_FS_WRITE_DIR_FAILED, 630005, ""),
    N(K_ERR_FS_READ_DIR_FAILED, 630006, ""),
    N(K_ERR_FS_READ_BLOB_FAILED, 630007, ""),
    N(K_ERR_FS_CANT_OPEN_FILE,  630008, ""),
    N(K_ERR_FS_CANT_READ_FULLY, 630009, ""),
    N(K_ERR_FS_SAVE_TEXT,       630010, ""),
    N(K_ERR_FS_SAVE_TEXT_APPEND, 630011, ""),
    N(K_ERR_FS_SAVE_BLOB,       630012, ""),
    N(K_ERR_FS_BLOB_NIL,        640013, ""), // Value seems off, kept original
    N(K_IMG_CANNOT_MAKE_IMAGE,  640001, ""),
    N(K_ERR_IMAGE_NIL,          640002, ""),
    N(K_ERR_CANVAS_BUFFER_IMG,  640003, ""),
    N(K_ERR_CANVAS_DEPTH_IMG,   640004, ""),
    N(K_ERR_CANVAS_ATTACHMENTS, 640005, ""),
    N(K_ERR_IMG_NOT_CANVAS,     640006, ""),
    N(K_FONT_NO_FONT_SET,       650001, ""),
    N(K_FONT_INVALID,           650002, ""),
    N(K_ERR_FONT_NIL,           650003, "")
)

ENUM_(KyteLogLevel, "Kyte Log Levels (must match log.h)",
    N_(K_LOG_TRACE, 0, "trace", ""),
    N_(K_LOG_DEBUG, 1, "debug", ""),
    N_(K_LOG_INFO,  2, "info", ""),
    N_(K_LOG_WARN,  3, "warn", ""),
    N_(K_LOG_ERROR, 4, "error", ""),
    N_(K_LOG_FATAL, 5, "fatal", "")
)

ENUM_(KyteBlendMode, "Blend modes (values map to SOKOL GP)",
    N_(K_BLENDMODE_NONE,  0, "none", ""),
    N_(K_BLENDMODE_BLEND, 1, "blend", ""),
    N_(K_BLENDMODE_ADD,   2, "add", ""),
    N_(K_BLENDMODE_MOD,   3, "mod", ""),
    N_(K_BLENDMODE_MUL,   4, "mul", "")
)

ENUM_(KyteFilterMode, "Texture filter modes (values map to SOKOL GP)",
    N_(K_FILTERMODE__INVALID, 1, "_invalid", ""), // Value is 1, order 0? Kept value.
    N_(K_FILTERMODE_NEAREST,  2, "nearest", ""),
    N_(K_FILTERMODE_LINEAR,   3, "linear", "")
)

ENUM_(KyteFetchStatus, "Status for asynchronous file fetching", // Gave name to enum
    N(K_BLOB_IN_PROGRESS, 0, "inprogress"),
    N(K_BLOB_READY, 1, "ready"),
    N(K_BLOB_FAILED, 2, "failed")
)


// --- Structs ---

STRUCT_(KyteInit, "Initialization settings for Kyte",
    F_(window_title, const char*, "Title of the application window"),
    F_(width, int, "Initial window width"),
    F_(height, int, "Initial window height"),
    F_(min_width, int, "Minimum window width"),
    F_(min_height, int, "Minimum window height"),
    F_(fullscreen, Bool, "Start in fullscreen mode"),
    F_(vsync, Bool, "Enable vertical sync"),
    F_(window_resizable, Bool, "Allow window resizing"),
    F_(blendmode, KyteBlendMode, "Default blend mode"),
    F_(filtermode, KyteFilterMode, "Default texture filter mode"),
    F_(argc, int, "Argument count from main()"),
    F_(argv, char**, "Argument values from main()") // Note: char** reflection needs care
)

// Keep default define separate as it's C syntax, not APIC DSL
#define KYTEINIT_DEFAULT {nil, 400,400, 400, 400, false, true, true, K_BLENDMODE_BLEND, K_FILTERMODE_LINEAR, 0, nil}


STRUCT_(KyteTickState, "State passed to the user-defined tick function each frame",
    F_(udata, void*, "User data pointer set via k_set_tick_data"),
    F_(delta_time, F64, "Time elapsed since the last frame in seconds"),
    F_(total_time, F64, "Total time elapsed since application start in seconds"),
    F_(width, int, "Current window width"),
    F_(height, int, "Current window height"),
    F_(resized, Bool, "True if the window was resized this frame"),
    F_(fullscreen, Bool, "True if currently in fullscreen mode")
)


// --- Function Pointers ---

FUNCPTR_(KyteTickFn, "User-defined function called each frame", void,
    A_(kts, KyteTickState, "Current frame state")
)


// --- Functions ---

// kyte state
FUNC_(k_create, "Initialize Kyte system", IErr, A_(ki, KyteInit*, "Initialization parameters"))
FUNC_(k_destroy, "Shutdown Kyte system", IErr)
FUNC_(k_set_tick_fn, "Set the main update/draw function", IErr, A_(fn, KyteTickFn, "(Pointer) the tick function"))
FUNC_(k_set_tick_data, "Set the user data pointer passed to the tick function", IErr, A_(udata, void*, "User data pointer"))
FUNC_(k_start_tickloop, "Start the main application loop", IErr)
FUNC_(k_quit, "Signal the application to quit", IErr)
FUNC_(k_set_blendmode, "Set the current global blend mode", IErr, A_(mode, KyteBlendMode, "New blend mode"))
FUNC_(k_set_filtermode, "Set the current global texture filter mode", IErr, A_(mode, KyteFilterMode, "New filter mode"))
FUNC_(k_set_color, "Set the current drawing color (RGBA)", IErr, A_(r, F32, ""), A_(g, F32, ""), A_(b, F32, ""), A_(a, F32, ""))
FUNC_(k_reset_color, "Reset the drawing color to default (white)", IErr)
FUNC_(k_cls, "Clear the screen with the current color", IErr)
FUNC_(k_push_matrix, "Push the current transformation matrix onto the stack", IErr)
FUNC_(k_pop_matrix, "Pop the current transformation matrix from the stack", IErr)
FUNC_(k_reset_matrix, "Reset the current transformation matrix to identity", IErr)
FUNC_(k_translate, "Apply translation to the current matrix", IErr, A_(delta_x, F32, ""), A_(delta_y, F32, ""))
FUNC_(k_rotate, "Apply rotation to the current matrix around the origin", IErr, A_(angle, F32, "Rotation angle in radians"))
FUNC_(k_rotate_at, "Apply rotation around a specific point", IErr, A_(angle, F32, ""), A_(x, F32, ""), A_(y, F32, ""))
FUNC_(k_scale, "Apply scaling to the current matrix from the origin", IErr, A_(scale_x, F32, ""), A_(scale_y, F32, ""))
FUNC_(k_scale_at, "Apply scaling relative to a specific point", IErr, A_(scale_x, F32, ""), A_(scale_y, F32, ""), A_(x, F32, ""), A_(y, F32, ""))

// cmd
FUNC_(k_cmd_get_exe_path, "Get the full path to the executable", const char*, A_(errptr, IErr*, "Error code output"))
FUNC_(k_cmd_arg_count, "Get the number of command line arguments", I32, A_(errptr, IErr*, "Error code output"))
FUNC_(k_cmd_arg_key_at, "Get the argument key at a specific index", const char*, A_(idx, int, "Argument index"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_cmd_arg_value_at, "Get the argument value at a specific index", const char*, A_(idx, int, "Argument index"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_cmd_arg_value, "Get the value associated with a specific argument key", const char*, A_(key, const char*, "Argument key (e.g., --input)"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_cmd_key_index, "Find the index of a specific argument key", I32, A_(key, const char*, "Argument key"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_cmd_equals, "Check if an argument key exists and equals a specific value", Bool, A_(key, const char*, "Argument key"), A_(value, const char*, "Value to compare against"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_cmd_has_key, "Check if an argument key exists", Bool, A_(key, const char*, "Argument key"), A_(errptr, IErr*, "Error code output"))

// log (Macros map these to external log_* functions)
#define k_log_trace log_trace
#define k_log_debug log_debug
#define k_log_info log_info
#define k_log_warn log_warn
#define k_log_error log_error
#define k_log_fatal log_fatal
#define k_log_set_quiet log_set_quiet
#define k_log_set_level log_set_level
#define k_log log_info

// time
FUNC_(k_get_clock_time, "Get high-resolution time in seconds since an arbitrary point", F64, A_(errptr, IErr*, "Error code output"))
FUNC_(k_set_clock_time, "Set the current clock time (useful for testing/debugging)", IErr, A_(t, F64, "New time value"))
FUNC_(k_get_delta_time, "Get time elapsed since last frame in seconds", F64, A_(errptr, IErr*, "Error code output"))
FUNC_(k_get_total_time, "Get total time elapsed since start in seconds", F64, A_(errptr, IErr*, "Error code output"))

// image
FUNC_(k_img_load_from_blob, "Create an image from a memory blob", KyteImage*, A_(blob, KyteBlob*, "Blob containing image data"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_img_load_from_file, "Load an image from a file path", KyteImage*, A_(fullpath, const char*, "Path to the image file"), A_(errptr, IErr*, "Error code output"))
// FUNC_(k_img_clone, "Clone an existing image (potentially removed?)", KyteImage*, A_(image, KyteImage*, "Source image"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_img_save, "Save an image to a file (format based on extension)", IErr, A_(image, KyteImage*, "Image to save"), A_(fullpath, const char*, "Output file path"))
FUNC_(k_img_remove, "Release image resources", IErr, A_(image, KyteImage*, "Image to remove"))
FUNC_(k_img_is_canvas, "Check if an image is a render target (canvas)", Bool, A_(image, KyteImage*, "Image to check"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_img_get_size, "Get the width and height of an image", IErr, A_(image, KyteImage*, "Image"), A_(width, I32*, "Output width"), A_(height, I32*, "Output height"))
FUNC_(k_img_draw, "Draw an image at the specified position", IErr, A_(image, KyteImage*, "Image to draw"), A_(x, F32, "Destination X"), A_(y, F32, "Destination Y"))

// canvas
FUNC_(k_new_canvas, "Create a new empty image that can be used as a render target", KyteImage*, A_(w, I32, "Width"), A_(h, I32, "Height"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_set_canvas, "Set the current render target to a canvas image", IErr, A_(c, KyteImage*, "Canvas image (or NULL to reset)"))
FUNC_(k_reset_canvas, "Reset the render target to the main window backbuffer", IErr)

// capture
FUNC_(k_capture_to_blob, "Capture a region of the current render target to a memory blob (PNG)", KyteBlob*, A_(x, F32, ""), A_(y, F32, ""), A_(width, F32, ""), A_(height, F32, ""), A_(errptr, IErr*, "Error code output"))
FUNC_(k_capture_to_image, "Capture a region of the current render target to a new image", KyteImage*, A_(x, F32, ""), A_(y, F32, ""), A_(width, F32, ""), A_(height, F32, ""), A_(errptr, IErr*, "Error code output"))
FUNC_(k_capture_to_file, "Capture a region of the current render target directly to a file (PNG)", IErr, A_(x, int, ""), A_(y, int, ""), A_(width, int, ""), A_(height, int, ""), A_(fullpath, const char*, "Output file path"))

// font
FUNC_(k_font_load_from_blob, "Load a font from a memory blob (TTF/OTF)", KyteFont*, A_(blob, KyteBlob*, "Blob containing font data"), A_(name, const char*, "Internal name for the font"), A_(initheight, F32, "Initial font height"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_font_load_from_file, "Load a font from a file path (TTF/OTF)", KyteFont*, A_(fullpath, const char*, "Path to the font file"), A_(initheight, F32, "Initial font height"), A_(errptr, IErr*, "Error code output")) // Note: initheigh typo in original fixed
FUNC_(k_font_get_height, "Get the current height of the font", F32, A_(font, KyteFont*, "Font object"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_font_set_height, "Set the current rendering height for the font", IErr, A_(font, KyteFont*, "Font object"), A_(newheight, F32, "New height"))
FUNC_(k_font_remove, "Release font resources", IErr, A_(font, KyteFont*, "Font to remove"))
FUNC_(k_set_font, "Set the currently active font for text drawing", IErr, A_(font, KyteFont*, "Font to set active"))
FUNC_(k_get_text_size, "Calculate the rendering size of the current font (requires font set)", IErr, A_(width, F32*, "Output width"), A_(height, F32*, "Output height"))
FUNC_(k_draw_text, "Draw text at the specified position using the current font", IErr, A_(text, const char*, "Text to draw"), A_(x, F32, "Destination X"), A_(y, F32, "Destination Y"))
FUNC_(k_draw_text_get_size, "Draw text and also return its calculated size", IErr, A_(text, const char*, ""), A_(x, F32, ""), A_(y, F32, ""), A_(width, F32*, "Output width"), A_(height, F32*, "Output height"))

// window
FUNC_(k_window_create, "Create the application window (called internally by k_create)", IErr)
FUNC_(k_window_destroy, "Destroy the application window (called internally by k_destroy)", IErr)
FUNC_(k_window_get_title, "Get the current window title", const char*, A_(e, IErr*, "Error code output"))
FUNC_(k_window_set_title, "Set the window title", IErr, A_(title, const char*, "New window title"))
FUNC_(k_window_get_vsync, "Check if vsync is currently enabled", Bool, A_(e, IErr*, "Error code output"))
FUNC_(k_window_set_vsync, "Enable or disable vsync", IErr, A_(new_vsync, Bool, "True to enable, false to disable"))
FUNC_(k_window_get_size, "Get the current window dimensions", IErr, A_(w, int*, "Output width"), A_(h, int*, "Output height"))
FUNC_(k_window_set_size, "Set the window dimensions", IErr, A_(w, int, "New width"), A_(h, int, "New height"))
FUNC_(k_window_get_position, "Get the window position on screen", IErr, A_(x, int*, "Output X"), A_(y, int*, "Output Y"))
FUNC_(k_window_set_position, "Set the window position on screen", IErr, A_(x, int, "New X"), A_(y, int, "New Y"))
FUNC_(k_window_get_fullscreen, "Check if the window is currently fullscreen", Bool, A_(e, IErr*, "Error code output"))
FUNC_(k_window_set_fullscreen, "Enable or disable fullscreen mode", IErr, A_(fullscreen, Bool, "True for fullscreen, false for windowed"))
FUNC_(k_window_get_margins, "Get window margins (platform specific)", IErr, A_(left, int*, ""), A_(right, int*, ""), A_(top, int*, ""), A_(bottom, int*, ""))
FUNC_(k_window_set_margins, "Set window margins (platform specific)", IErr, A_(left, int, ""), A_(right, int, ""), A_(top, int, ""), A_(bottom, int, ""))
FUNC_(k_window_get_paddings, "Get window paddings (platform specific)", IErr, A_(left, int*, ""), A_(right, int*, ""), A_(top, int*, ""), A_(bottom, int*, ""))
FUNC_(k_window_set_paddings, "Set window paddings (platform specific)", IErr, A_(left, int, ""), A_(right, int, ""), A_(top, int, ""), A_(bottom, int, ""))

// filesystem
FUNC_(k_fs_mount_set_writable_path, "Set the base path for writing files", IErr, A_(localpath, const char*, "Path on the host filesystem"))
FUNC_(k_fs_mount_add_readable_path, "Mount a host directory as readable at a virtual path", IErr, A_(localpath, const char*, "Path on the host filesystem"), A_(mountpath, const char*, "Virtual path inside Kyte"))
FUNC_(k_fs_mount_add_readable_path_zip, "Mount a zip archive from the host as readable at a virtual path", IErr, A_(localzippath, const char*, "Path to the zip file on the host"), A_(mountpath, const char*, "Virtual path inside Kyte"))
FUNC_(k_fs_mount_add_readable_path_blob_zip, "Mount a zip archive from a memory blob as readable", IErr, A_(blob, KyteBlob*, "Blob containing zip data"), A_(mountpath, char*, "Virtual path inside Kyte")) // Note: mountpath non-const?
FUNC_(k_fs_load_file, "Load a file from the virtual filesystem into a memory blob", KyteBlob*, A_(fullpath, const char*, "Virtual path to the file"), A_(e, IErr*, "Error code output"))
FUNC_(k_fs_create_blob_from_buffer, "Create a blob by copying data from an existing buffer", KyteBlob*, A_(buf, void*, "Source buffer"), A_(size, ptrdiff_t, "Size of buffer"), A_(blobname, const char*, "Internal name for the blob"), A_(e, IErr*, "Error code output"))
FUNC_(k_fs_create_blob_empty, "Create an empty blob of a specific size", KyteBlob*, A_(size, ptrdiff_t, "Size of blob to create"), A_(blobname, const char*, "Internal name for the blob"), A_(e, IErr*, "Error code output"))
FUNC_(k_fs_fetch_file_async, "Asynchronously fetch a file from a URL (e.g., HTTP)", KyteBlob*, A_(url, const char*, "URL to fetch"), A_(blobname, const char*, "Internal name for the blob"), A_(estimated_size, ptrdiff_t, "Estimated size for preallocation"), A_(e, IErr*, "Error code output"))
FUNC_(k_fs_check_fetch_status, "Check the status of an asynchronously fetched blob", KyteFetchStatus, A_(blob, KyteBlob*, "Blob being fetched"), A_(e, IErr*, "Error code output"))
FUNC_(k_fs_save_blob_to_file, "Save a blob's content to the host filesystem (requires writable path)", IErr, A_(blob, KyteBlob*, "Blob to save"), A_(path_or_nil, const char*, "Output path (or use blob name if NULL)"))
FUNC_(k_fs_remove_blob, "Release blob resources", IErr, A_(blob, KyteBlob*, "Blob to remove"))
FUNC_(k_fs_get_blob_data, "Get a pointer to the blob's internal data buffer and its size", IErr, A_(blob, KyteBlob*, ""), A_(buf, void**, "Output data pointer"), A_(size, ptrdiff_t*, "Output size"))
FUNC_(k_fs_get_blob_name, "Get the internal name of a blob", const char*, A_(blob, KyteBlob*, ""), A_(e, IErr*, "Error code output"))
FUNC_(k_fs_set_blob_name, "Set the internal name of a blob", IErr, A_(blob, KyteBlob*, ""), A_(new_name, const char*, "New name for the blob"))
FUNC_(k_fs_get_droppedfile_blobs, "Get a list of blobs created from files dropped onto the window", KyteBlob**, A_(count, ptrdiff_t*, "Output number of files dropped"), A_(errptr, IErr*, "Error code output"))
FUNC_(k_fs_droppedfile_list_cleanup, "Clean up the internal list of dropped file blobs", IErr)
FUNC_(k_fs_add_droppedfile, "Add a dropped file programmatically (mainly for JS bridge)", IErr, A_(name, const char*, "Filename"), A_(buf, void*, "File data buffer"), A_(size, ptrdiff_t, "Size of data buffer"))
FUNC_(k_fs_get_droppedfile_count, "Get the number of files currently in the dropped file list", ptrdiff_t, A_(errptr, IErr*, "Error code output"))
FUNC_(k_textfile_load, "Load a file as a null-terminated text string", const char*, A_(fullpath, const char*, "Virtual path"), A_(blob, KyteBlob**, "Optional output blob containing the data"), A_(e, IErr*, "Error code output"))
FUNC_(k_textfile_save, "Save text to a file in the writable directory", IErr, A_(fullpath, const char*, "Output virtual path"), A_(text, const char*, "Text content to save"))
FUNC_(k_textfile_append, "Append text to a file in the writable directory", IErr, A_(fullpath, const char*, "Virtual path"), A_(text, const char*, "Text content to append"))


// shapes
FUNC_(k_draw_point, "Draw a single point", IErr, A_(x, F32, ""), A_(y, F32, ""))
FUNC_(k_draw_line, "Draw a line segment", IErr, A_(x1, F32, ""), A_(y1, F32, ""), A_(x2, F32, ""), A_(y2, F32, ""))
FUNC_(k_draw_triangle, "Draw a filled triangle", IErr, A_(ax, F32, ""), A_(ay, F32, ""), A_(bx, F32, ""), A_(by, F32, ""), A_(cx, F32, ""), A_(cy, F32, ""))
FUNC_(k_draw_triangle_line, "Draw the outline of a triangle", IErr, A_(ax, F32, ""), A_(ay, F32, ""), A_(bx, F32, ""), A_(by, F32, ""), A_(cx, F32, ""), A_(cy, F32, ""))
FUNC_(k_draw_rect, "Draw a filled rectangle", IErr, A_(x, F32, ""), A_(y, F32, ""), A_(w, F32, ""), A_(h, F32, ""))
FUNC_(k_draw_rect_line, "Draw the outline of a rectangle", IErr, A_(x, F32, ""), A_(y, F32, ""), A_(w, F32, ""), A_(h, F32, ""))
FUNC_(k_draw_circle, "Draw a filled circle", IErr, A_(dest_x, F32, ""), A_(dest_y, F32, ""), A_(radius, F32, ""))
FUNC_(k_draw_circle_line, "Draw the outline of a circle", IErr, A_(dest_x, F32, ""), A_(dest_y, F32, ""), A_(radius, F32, ""))
FUNC_(k_draw_ellipse, "Draw a filled ellipse", IErr, A_(dest_x, F32, ""), A_(dest_y, F32, ""), A_(radius_x, F32, ""), A_(radius_y, F32, ""))
FUNC_(k_draw_ellipse_line, "Draw the outline of an ellipse", IErr, A_(dest_x, F32, ""), A_(dest_y, F32, ""), A_(radius_x, F32, ""), A_(radius_y, F32, ""))

// input
FUNC_(k_input_key_down, "Check if a keyboard key is currently held down", Bool, A_(key, KyteKey, ""), A_(e, IErr*, "Error code output"))
FUNC_(k_input_key_pressed, "Check if a keyboard key was pressed this frame", Bool, A_(key, KyteKey, ""), A_(e, IErr*, "Error code output"))
FUNC_(k_input_key_released, "Check if a keyboard key was released this frame", Bool, A_(key, KyteKey, ""), A_(e, IErr*, "Error code output"))
FUNC_(k_input_key_repeat, "Check if a keyboard key press is repeating this frame", Bool, A_(key, KyteKey, ""), A_(e, IErr*, "Error code output"))
FUNC_(k_input_mouse_down, "Check if a mouse button is currently held down", Bool, A_(but, KyteMouseButton, ""), A_(e, IErr*, "Error code output"))
FUNC_(k_input_mouse_pressed, "Check if a mouse button was pressed this frame", Bool, A_(but, KyteMouseButton, ""), A_(e, IErr*, "Error code output"))
FUNC_(k_input_mouse_released, "Check if a mouse button was released this frame", Bool, A_(but, KyteMouseButton, ""), A_(e, IErr*, "Error code output"))
FUNC_(k_input_mouse_get_position, "Get the current mouse cursor position", IErr, A_(x, F64*, "Output X position"), A_(y, F64*, "Output Y position"))

// Conditional Input Section
#ifdef ___INCLUDE_GAMEPAD
FUNC_(k_input_gamepad_get_count, "Get the number of connected gamepads", int, A_(e, IErr*, ""))
FUNC_(k_input_gamepad_get_name, "Get the name of a specific gamepad", const char*, A_(idx, int, ""), A_(e, IErr*, ""))
FUNC_(k_input_gamepad_down, "Check if a gamepad button is held down", Bool, A_(idx, int, ""), A_(but, KyteGamepadButton, ""), A_(e, IErr*, ""))
FUNC_(k_input_gamepad_pressed, "Check if a gamepad button was pressed this frame", Bool, A_(idx, int, ""), A_(but, KyteGamepadButton, ""), A_(e, IErr*, ""))
FUNC_(k_input_gamepad_released, "Check if a gamepad button was released this frame", Bool, A_(idx, int, ""), A_(but, KyteGamepadButton, ""), A_(e, IErr*, ""))
FUNC_(k_input_gamepad_get_axis, "Get the value of a gamepad axis", F64, A_(idx, int, ""), A_(axis, KyteGamepadAxis, ""), A_(e, IErr*, ""))
#endif // ___INCLUDE_GAMEPAD

// Conditional Emscripten Section
#if defined(__EMSCRIPTEN__)
FUNC_(k_emsc_run_js, "Execute arbitrary JavaScript code (Emscripten only)", void, A_(js, const char*, "JavaScript code string"))
FUNC_(k_emsc_send_file_save, "Trigger a file save dialog in the browser (Emscripten only)", void, A_(blob, KyteBlob*, "Blob data to save"), A_(download, Bool, "True to force download"))
#endif // __EMSCRIPTEN__


// --- API Exports ---

// Note: Manually list all reflected items here. Keep conditional items separate or inside #ifdefs.
EXPORTS_(kyte_api, "Kyte Core API",
    ALIASES( // Include base type aliases and forward decl aliases
        &IErr, &Bool, &F32, &F64, &I32, // ptrdiff_t, // ptrdiff_t, //&ptrdiff_t,
        &KyteBlob, &KyteImage, &KyteFont
    ),
    ENUMS( // List all defined ENUMs
        &KyteError, &KyteLogLevel, &KyteBlendMode, &KyteFilterMode, &KyteFetchStatus,
        &KyteKey, &KyteMouseButton, // TODO: gamepad
    ),
    STRUCTS( // List all defined STRUCTs
        &KyteInit, &KyteTickState
    ),
    FUNCPTRS( // List all defined FUNCPTRs
        &KyteTickFn
    ),
    FUNCS( // List ALL non-conditional defined FUNCs
        // kyte state
        &k_create, &k_destroy, &k_set_tick_fn, &k_set_tick_data, &k_start_tickloop,
        &k_quit, &k_set_blendmode, &k_set_filtermode, &k_set_color, &k_reset_color,
        &k_cls, &k_push_matrix, &k_pop_matrix, &k_reset_matrix, &k_translate,
        &k_rotate, &k_rotate_at, &k_scale, &k_scale_at,
        // cmd
        &k_cmd_get_exe_path, &k_cmd_arg_count, &k_cmd_arg_key_at,
        &k_cmd_arg_value_at, &k_cmd_arg_value, &k_cmd_key_index,
        &k_cmd_equals, &k_cmd_has_key,
        // time
        &k_get_clock_time, &k_set_clock_time, &k_get_delta_time, &k_get_total_time,
        // image
        &k_img_load_from_blob, &k_img_load_from_file, // &k_img_clone,
        &k_img_save,
        &k_img_remove, &k_img_is_canvas, &k_img_get_size, &k_img_draw,
        // canvas
        &k_new_canvas, &k_set_canvas, &k_reset_canvas,
        // capture
        &k_capture_to_blob, &k_capture_to_image, &k_capture_to_file,
        // font
        &k_font_load_from_blob, &k_font_load_from_file, &k_font_get_height,
        &k_font_set_height, &k_font_remove, &k_set_font, &k_get_text_size,
        &k_draw_text, &k_draw_text_get_size,
        // window
        &k_window_create, &k_window_destroy, &k_window_get_title, &k_window_set_title,
        &k_window_get_vsync, &k_window_set_vsync, &k_window_get_size, &k_window_set_size,
        &k_window_get_position, &k_window_set_position, &k_window_get_fullscreen, &k_window_set_fullscreen,
        &k_window_get_margins, &k_window_set_margins, &k_window_get_paddings, &k_window_set_paddings,
        // filesystem
        &k_fs_mount_set_writable_path, &k_fs_mount_add_readable_path, &k_fs_mount_add_readable_path_zip,
        &k_fs_mount_add_readable_path_blob_zip, &k_fs_load_file, &k_fs_create_blob_from_buffer,
        &k_fs_create_blob_empty, &k_fs_fetch_file_async, &k_fs_check_fetch_status,
        &k_fs_save_blob_to_file, &k_fs_remove_blob, &k_fs_get_blob_data,
        &k_fs_get_blob_name, &k_fs_set_blob_name, &k_fs_get_droppedfile_blobs,
        &k_fs_droppedfile_list_cleanup, &k_fs_add_droppedfile, &k_fs_get_droppedfile_count,
        &k_textfile_load, &k_textfile_save, &k_textfile_append,
        // shapes
        &k_draw_point, &k_draw_line, &k_draw_triangle, &k_draw_triangle_line,
        &k_draw_rect, &k_draw_rect_line, &k_draw_circle, &k_draw_circle_line,
        &k_draw_ellipse, &k_draw_ellipse_line,
        // input
        &k_input_key_down, &k_input_key_pressed, &k_input_key_released, &k_input_key_repeat,
        &k_input_mouse_down, &k_input_mouse_pressed, &k_input_mouse_released, &k_input_mouse_get_position
    )
    // No UNIONS in kyte.h
)

// --- Conditional Exports ---
#ifdef ___INCLUDE_GAMEPAD
EXPORTS_(kyte_api_gamepad, "Kyte Gamepad API",
    ALIASES(&KyteGamepadButton, &KyteGamepadAxis), // Include needed aliases
    FUNCS(
        &k_input_gamepad_get_count, &k_input_gamepad_get_name, &k_input_gamepad_down,
        &k_input_gamepad_pressed, &k_input_gamepad_released, &k_input_gamepad_get_axis
    )
)
#endif // ___INCLUDE_GAMEPAD

#if defined(__EMSCRIPTEN__)
EXPORTS_(kyte_api_emscripten, "Kyte Emscripten specific API",
    ALIASES(&KyteBlob, &Bool), // Include needed aliases
    FUNCS(
        &k_emsc_run_js, &k_emsc_send_file_save
    )
)
#endif // __EMSCRIPTEN__


#endif
