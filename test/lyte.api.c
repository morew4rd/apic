#include "apic.h"

// --- Enums ---

ENUM(UniformType, DOC("Acceptable uniformtype values."),
    ENUM_FIELD(_invalid, 0, "_invalid", DOC("Invalid uniform type")),
    ENUM_FIELD(float, 1, "float", DOC("Float uniform type")),
    ENUM_FIELD(vec2, 2, "vec2", DOC("Vec2 uniform type")),
    ENUM_FIELD(vec3, 3, "vec3", DOC("Vec3 uniform type")),
    ENUM_FIELD(vec4, 4, "vec4", DOC("Vec4 uniform type")),
    ENUM_FIELD(int, 5, "int", DOC("Int uniform type")),
    ENUM_FIELD(ivec2, 6, "ivec2", DOC("IVec2 uniform type")),
    ENUM_FIELD(ivec3, 7, "ivec3", DOC("IVec3 uniform type")),
    ENUM_FIELD(ivec4, 8, "ivec4", DOC("IVec4 uniform type")),
    ENUM_FIELD(mat4, 9, "mat4", DOC("Mat4 uniform type")),
    ENUM_FIELD(sampler2D, 10, "sampler2D", DOC("Sampler2D uniform type"))
);

ENUM(BlendMode, DOC("Acceptable blendmode values."),
    ENUM_FIELD(none, 0, "none", DOC("No blending")),
    ENUM_FIELD(blend, 1, "blend", DOC("Alpha blending")),
    ENUM_FIELD(add, 2, "add", DOC("Additive blending")),
    ENUM_FIELD(mod, 3, "mod", DOC("Modulate blending")),
    ENUM_FIELD(mul, 4, "mul", DOC("Multiply blending"))
);

ENUM(FilterMode, DOC("Acceptable filtermode values."),
    ENUM_FIELD(_invalid, 0, "_invalid", DOC("Invalid filter mode")),
    ENUM_FIELD(nearest, 1, "nearest", DOC("Nearest filter mode")),
    ENUM_FIELD(linear, 2, "linear", DOC("Linear filter mode"))
);

ENUM(GamepadAxis, DOC("Acceptable gamepadaxis values."),
    ENUM_FIELD(left_x, 0, "left_x", DOC("Left stick X axis")),
    ENUM_FIELD(left_y, 1, "left_y", DOC("Left stick Y axis")),
    ENUM_FIELD(right_x, 2, "right_x", DOC("Right stick X axis")),
    ENUM_FIELD(right_y, 3, "right_y", DOC("Right stick Y axis")),
    ENUM_FIELD(left_trigger, 4, "left_trigger", DOC("Left trigger")),
    ENUM_FIELD(right_trigger, 5, "right_trigger", DOC("Right trigger"))
);

ENUM(GamepadButton, DOC("Acceptable gamepadbutton values."),
    ENUM_FIELD(pad_a, 0, "pad_a", DOC("A button")),
    ENUM_FIELD(pad_b, 1, "pad_b", DOC("B button")),
    ENUM_FIELD(pad_x, 2, "pad_x", DOC("X button")),
    ENUM_FIELD(pad_y, 3, "pad_y", DOC("Y button")),
    ENUM_FIELD(left_bumper, 4, "left_bumper", DOC("Left bumper")),
    ENUM_FIELD(right_bumper, 5, "right_bumper", DOC("Right bumper")),
    ENUM_FIELD(back, 6, "back", DOC("Back button")),
    ENUM_FIELD(start, 7, "start", DOC("Start button")),
    ENUM_FIELD(guide, 8, "guide", DOC("Guide button")),
    ENUM_FIELD(left_thumb, 9, "left_thumb", DOC("Left thumbstick button")),
    ENUM_FIELD(right_thumb, 10, "right_thumb", DOC("Right thumbstick button")),
    ENUM_FIELD(dpad_up, 11, "dpad_up", DOC("D-pad up")),
    ENUM_FIELD(dpad_right, 12, "dpad_right", DOC("D-pad right")),
    ENUM_FIELD(dpad_down, 13, "dpad_down", DOC("D-pad down")),
    ENUM_FIELD(dpad_left, 14, "dpad_left", DOC("D-pad left"))
);

ENUM(MouseButton, DOC("Acceptable mousebutton values."),
    ENUM_FIELD(mb1, 0, "mb1", DOC("Mouse button 1")),
    ENUM_FIELD(mb2, 1, "mb2", DOC("Mouse button 2")),
    ENUM_FIELD(mb3, 2, "mb3", DOC("Mouse button 3")),
    ENUM_FIELD(mb4, 3, "mb4", DOC("Mouse button 4")),
    ENUM_FIELD(mb5, 4, "mb5", DOC("Mouse button 5")),
    ENUM_FIELD(mb6, 5, "mb6", DOC("Mouse button 6")),
    ENUM_FIELD(mb7, 6, "mb7", DOC("Mouse button 7")),
    ENUM_FIELD(mb8, 7, "mb8", DOC("Mouse button 8")),
    ENUM_FIELD(scrollup, 8, "scrollup", DOC("Scroll up")),
    ENUM_FIELD(scrolldown, 9, "scrolldown", DOC("Scroll down"))
);

ENUM(KeyboardKey, DOC("Acceptable keyboardkey values."),
    ENUM_FIELD(space, 0, "space", DOC("Space key")),
    ENUM_FIELD(quote, 1, "'", DOC("Single quote key")),
    ENUM_FIELD(comma, 2, ",", DOC("Comma key")),
    ENUM_FIELD(minus, 3, "-", DOC("Minus key")),
    ENUM_FIELD(period, 4, ".", DOC("Period key")),
    ENUM_FIELD(slash, 5, "/", DOC("Slash key")),
    ENUM_FIELD(num0, 6, "0", DOC("Number 0 key")),
    ENUM_FIELD(num1, 7, "1", DOC("Number 1 key")),
    ENUM_FIELD(num2, 8, "2", DOC("Number 2 key")),
    ENUM_FIELD(num3, 9, "3", DOC("Number 3 key")),
    ENUM_FIELD(num4, 10, "4", DOC("Number 4 key")),
    ENUM_FIELD(num5, 11, "5", DOC("Number 5 key")),
    ENUM_FIELD(num6, 12, "6", DOC("Number 6 key")),
    ENUM_FIELD(num7, 13, "7", DOC("Number 7 key")),
    ENUM_FIELD(num8, 14, "8", DOC("Number 8 key")),
    ENUM_FIELD(num9, 15, "9", DOC("Number 9 key")),
    ENUM_FIELD(semicolon, 16, ";", DOC("Semicolon key")),
    ENUM_FIELD(equal, 17, "=", DOC("Equal key")),
    ENUM_FIELD(a, 18, "a", DOC("A key")),
    ENUM_FIELD(b, 19, "b", DOC("B key")),
    ENUM_FIELD(c, 20, "c", DOC("C key")),
    ENUM_FIELD(d, 21, "d", DOC("D key")),
    ENUM_FIELD(e, 22, "e", DOC("E key")),
    ENUM_FIELD(f, 23, "f", DOC("F key")),
    ENUM_FIELD(g, 24, "g", DOC("G key")),
    ENUM_FIELD(h, 25, "h", DOC("H key")),
    ENUM_FIELD(i, 26, "i", DOC("I key")),
    ENUM_FIELD(j, 27, "j", DOC("J key")),
    ENUM_FIELD(k, 28, "k", DOC("K key")),
    ENUM_FIELD(l, 29, "l", DOC("L key")),
    ENUM_FIELD(m, 30, "m", DOC("M peelingkey")),
    ENUM_FIELD(n, 31, "n", DOC("N key")),
    ENUM_FIELD(o, 32, "o", DOC("O key")),
    ENUM_FIELD(p, 33, "p", DOC("P key")),
    ENUM_FIELD(q, 34, "q", DOC("Q key")),
    ENUM_FIELD(r, 35, "r", DOC("R key")),
    ENUM_FIELD(s, 36, "s", DOC("S key")),
    ENUM_FIELD(t, 37, "t", DOC("T key")),
    ENUM_FIELD(u, 38, "u", DOC("U key")),
    ENUM_FIELD(v, 39, "v", DOC("V key")),
    ENUM_FIELD(w, 40, "w", DOC("W key")),
    ENUM_FIELD(x, 41, "x", DOC("X key")),
    ENUM_FIELD(y, 42, "y", DOC("Y key")),
    ENUM_FIELD(z, 43, "z", DOC("Z key")),
    ENUM_FIELD(left_bracket, 44, "[", DOC("Left bracket key")),
    ENUM_FIELD(backslash, 45, "\\", DOC("Backslash key")),
    ENUM_FIELD(right_bracket, 46, "]", DOC("Right bracket key")),
    ENUM_FIELD(backtick, 47, "`", DOC("Backtick key")),
    ENUM_FIELD(world_1, 48, "world_1", DOC("World 1 key")),
    ENUM_FIELD(world_2, 49, "world_2", DOC("World 2 key")),
    ENUM_FIELD(escape, 50, "escape", DOC("Escape key")),
    ENUM_FIELD(enter, 51, "enter", DOC("Enter key")),
    ENUM_FIELD(tab, 52, "tab", DOC("Tab key")),
    ENUM_FIELD(backspace, 53, "backspace", DOC("Backspace key")),
    ENUM_FIELD(insert, 54, "insert", DOC("Insert key")),
    ENUM_FIELD(delete, 55, "delete", DOC("Delete key")),
    ENUM_FIELD(right, 56, "right", DOC("Right arrow key")),
    ENUM_FIELD(left, 57, "left", DOC("Left arrow key")),
    ENUM_FIELD(down, 58, "down", DOC("Down arrow key")),
    ENUM_FIELD(up, 59, "up", DOC("Up arrow key")),
    ENUM_FIELD(page_up, 60, "page_up", DOC("Page up key")),
    ENUM_FIELD(page_down, 61, "page_down", DOC("Page down key")),
    ENUM_FIELD(home, 62, "home", DOC("Home key")),
    ENUM_FIELD(end, 63, "end", DOC("End key")),
    ENUM_FIELD(caps_lock, 64, "caps_lock", DOC("Caps lock key")),
    ENUM_FIELD(scroll_lock, 65, "scroll_lock", DOC("Scroll lock key")),
    ENUM_FIELD(num_lock, 66, "num_lock", DOC("Num lock key")),
    ENUM_FIELD(print_screen, 67, "print_screen", DOC("Print screen key")),
    ENUM_FIELD(pause, 68, "pause", DOC("Pause key")),
    ENUM_FIELD(f1, 69, "f1", DOC("F1 key")),
    ENUM_FIELD(f2, 70, "f2", DOC("F2 key")),
    ENUM_FIELD(f3, 71, "f3", DOC("F3 key")),
    ENUM_FIELD(f4, 72, "f4", DOC("F4 key")),
    ENUM_FIELD(f5, 73, "f5", DOC("F5 key")),
    ENUM_FIELD(f6, 74, "f6", DOC("F6 key")),
    ENUM_FIELD(f7, 75, "f7", DOC("F7 key")),
    ENUM_FIELD(f8, 76, "f8", DOC("F8 key")),
    ENUM_FIELD(f9, 77, "f9", DOC("F9 key")),
    ENUM_FIELD(f10, 78, "f10", DOC("F10 key")),
    ENUM_FIELD(f11, 79, "f11", DOC("F11 key")),
    ENUM_FIELD(f12, 80, "f12", DOC("F12 key")),
    ENUM_FIELD(f13, 81, "f13", DOC("F13 key")),
    ENUM_FIELD(f14, 82, "f14", DOC("F14 key")),
    ENUM_FIELD(f15, 83, "f15", DOC("F15 key")),
    ENUM_FIELD(f16, 84, "f16", DOC("F16 key")),
    ENUM_FIELD(f17, 85, "f17", DOC("F17 key")),
    ENUM_FIELD(f18, 86, "f18", DOC("F18 key")),
    ENUM_FIELD(f19, 87, "f19", DOC("F19 key")),
    ENUM_FIELD(f20, 88, "f20", DOC("F20 key")),
    ENUM_FIELD(f21, 89, "f21", DOC("F21 key")),
    ENUM_FIELD(f22, 90, "f22", DOC("F22 key")),
    ENUM_FIELD(f23, 91, "f23", DOC("F23 key")),
    ENUM_FIELD(f24, 92, "f24", DOC("F24 key")),
    ENUM_FIELD(f25, 93, "f25", DOC("F25 key")),
    ENUM_FIELD(kp_0, 94, "kp_0", DOC("Keypad 0 key")),
    ENUM_FIELD(kp_1, 95, "kp_1", DOC("Keypad 1 key")),
    ENUM_FIELD(kp_2, 96, "kp_2", DOC("Keypad 2 key")),
    ENUM_FIELD(kp_3, 97, "kp_3", DOC("Keypad 3 key")),
    ENUM_FIELD(kp_4, 98, "kp_4", DOC("Keypad 4 key")),
    ENUM_FIELD(kp_5, 99, "kp_5", DOC("Keypad 5 key")),
    ENUM_FIELD(kp_6, 100, "kp_6", DOC("Keypad 6 key")),
    ENUM_FIELD(kp_7, 101, "kp_7", DOC("Keypad 7 key")),
    ENUM_FIELD(kp_8, 102, "kp_8", DOC("Keypad 8 key")),
    ENUM_FIELD(kp_9, 103, "kp_9", DOC("Keypad 9 key")),
    ENUM_FIELD(kp_decimal, 104, "kp_decimal", DOC("Keypad decimal key")),
    ENUM_FIELD(kp_divide, 105, "kp_divide", DOC("Keypad divide key")),
    ENUM_FIELD(kp_multiply, 106, "kp_multiply", DOC("Keypad multiply key")),
    ENUM_FIELD(kp_subtract, 107, "kp_subtract", DOC("Keypad subtract key")),
    ENUM_FIELD(kp_add, 108, "kp_add", DOC("Keypad add key")),
    ENUM_FIELD(kp_enter, 109, "kp_enter", DOC("Keypad enter key")),
    ENUM_FIELD(kp_equal, 110, "kp_equal", DOC("Keypad equal key")),
    ENUM_FIELD(left_shift, 111, "left_shift", DOC("Left shift key")),
    ENUM_FIELD(left_control, 112, "left_control", DOC("Left control key")),
    ENUM_FIELD(left_alt, 113, "left_alt", DOC("Left alt key")),
    ENUM_FIELD(left_super, 114, "left_super", DOC("Left super key")),
    ENUM_FIELD(right_shift, 115, "right_shift", DOC("Right shift key")),
    ENUM_FIELD(right_control, 116, "right_control", DOC("Right control key")),
    ENUM_FIELD(right_alt, 117, "right_alt", DOC("Right alt key")),
    ENUM_FIELD(right_super, 118, "right_super", DOC("Right super key")),
    ENUM_FIELD(menu, 119, "menu", DOC("Menu key"))
);

// --- Pointer Types ---

PTR_TYPE(Image_Ptr, "Image", DOC("Pointer to Image"));
PTR_TYPE(ImageBatch_Ptr, "ImageBatch", DOC("Pointer to ImageBatch"));
PTR_TYPE(Font_Ptr, "Font", DOC("Pointer to Font"));
PTR_TYPE(Music_Ptr, "Music", DOC("Pointer to Music"));
PTR_TYPE(Sound_Ptr, "Sound", DOC("Pointer to Sound"));
PTR_TYPE(Shader_Ptr, "Shader", DOC("Pointer to Shader"));
PTR_TYPE(ShaderBuilder_Ptr, "ShaderBuilder", DOC("Pointer to ShaderBuilder"));
PTR_TYPE(World_Ptr, "World", DOC("Pointer to World"));
PTR_TYPE(Body_Ptr, "Body", DOC("Pointer to Body"));
PTR_TYPE(Space_Ptr, "Space", DOC("Pointer to Space"));
PTR_TYPE(JointGroup_Ptr, "JointGroup", DOC("Pointer to JointGroup"));
PTR_TYPE(Geom_Ptr, "Geom", DOC("Pointer to Geom"));
PTR_TYPE(Joint_Ptr, "Joint", DOC("Pointer to Joint"));

// --- Array Types ---

ARRAY_TYPE(FloatVec4, FLOAT, 4, DOC("Float values"));
ARRAY_TYPE(KeyboardKeyArray, "KeyboardKey", 16, DOC("Array of KeyboardKey"));

// --- Records ---

RECORD(Vec2, DOC("2D vector"),
    FIELD(x, DOUBLE, DOC("X coordinate")),
    FIELD(y, DOUBLE, DOC("Y coordinate"))
);

RECORD(KeyList, DOC("List of keys"),
    FIELD(count, INT32, DOC("Number of pressed keys")),
    FIELD(keys, "KeyboardKeyArray", DOC("Array of pressed keys"))
);

RECORD(CollisionData, DOC("Collision data"),
    FIELD(body2, "Body_Ptr", DOC("Colliding body")),
    FIELD(pos_x, DOUBLE, DOC("Collision position X")),
    FIELD(pos_y, DOUBLE, DOC("Collision position Y")),
    FIELD(depth, DOUBLE, DOC("Collision depth"))
);

RECORD(AABB, DOC("Axis-Aligned Bounding Box"),
    FIELD(minx, DOUBLE, DOC("Minimum X coordinate")),
    FIELD(miny, DOUBLE, DOC("Minimum Y coordinate")),
    FIELD(maxx, DOUBLE, DOC("Maximum X coordinate")),
    FIELD(maxy, DOUBLE, DOC("Maximum Y coordinate"))
);

// --- Functions ---

FUNCTION(quit, DOC("Quit the application by closing the window."), VOIDRET);

FUNCTION(cls, DOC("Clear the screen or current canvas if one is used."), VOIDRET,
    ARG(r, DOUBLE, DOC("Red component")),
    ARG(g, DOUBLE, DOC("Green component")),
    ARG(b, DOUBLE, DOC("Blue component")),
    ARG(a, DOUBLE, DOC("Alpha component"))
);

FUNCTION(set_color, DOC("Set the foreground color to be used in the drawing operations."), VOIDRET,
    ARG(r, DOUBLE, DOC("Red component")),
    ARG(g, DOUBLE, DOC("Green component")),
    ARG(b, DOUBLE, DOC("Blue component")),
    ARG(a, DOUBLE, DOC("Alpha component"))
);

FUNCTION(reset_color, DOC("Reset the foreground color to its default value."), VOIDRET);

FUNCTION(draw_point, DOC("Draw a point."), VOIDRET,
    ARG(x, DOUBLE, DOC("X coordinate")),
    ARG(y, DOUBLE, DOC("Y coordinate"))
);

FUNCTION(draw_line, DOC("Draw a line"), VOIDRET,
    ARG(x1, DOUBLE, DOC("X1 coordinate")),
    ARG(y1, DOUBLE, DOC("Y1 coordinate")),
    ARG(x2, DOUBLE, DOC("X2 coordinate")),
    ARG(y2, DOUBLE, DOC("Y2 coordinate"))
);

FUNCTION(draw_triangle, DOC("Draw a filled triangle"), VOIDRET,
    ARG(ax, DOUBLE, DOC("A X coordinate")),
    ARG(ay, DOUBLE, DOC("A Y coordinate")),
    ARG(bx, DOUBLE, DOC("B X coordinate")),
    ARG(by, DOUBLE, DOC("B Y coordinate")),
    ARG(cx, DOUBLE, DOC("C X coordinate")),
    ARG(cy, DOUBLE, DOC("C Y coordinate"))
);

FUNCTION(draw_triangle_line, DOC("Draw a triangle border"), VOIDRET,
    ARG(ax, DOUBLE, DOC("A X coordinate")),
    ARG(ay, DOUBLE, DOC("A Y coordinate")),
    ARG(bx, DOUBLE, DOC("B X coordinate")),
    ARG(by, DOUBLE, DOC("B Y coordinate")),
    ARG(cx, DOUBLE, DOC("C X coordinate")),
    ARG(cy, DOUBLE, DOC("C Y coordinate"))
);

FUNCTION(draw_rect, DOC("Draw a filled rectangle."), VOIDRET,
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(rect_width, DOUBLE, DOC("Rectangle width")),
    ARG(rect_height, DOUBLE, DOC("Rectangle height"))
);

FUNCTION(draw_rect_line, DOC("Draw a rectangle border."), VOIDRET,
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(rect_width, DOUBLE, DOC("Rectangle width")),
    ARG(rect_height, DOUBLE, DOC("Rectangle height"))
);

FUNCTION(draw_circle, DOC("Draw a filled circle."), VOIDRET,
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(radius, DOUBLE, DOC("Circle radius"))
);

FUNCTION(draw_circle_line, DOC("Draw a circle border."), VOIDRET,
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(radius, DOUBLE, DOC("Circle radius"))
);

FUNCTION(draw_ellipse, DOC("Draw a filled ellipse."), VOIDRET,
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(radius_x, DOUBLE, DOC("X radius")),
    ARG(radius_y, DOUBLE, DOC("Y radius"))
);

FUNCTION(draw_ellipse_line, DOC("Draw an ellipse border."), VOIDRET,
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(radius_x, DOUBLE, DOC("X radius")),
    ARG(radius_y, DOUBLE, DOC("Y radius"))
);

FUNCTION(image_cleanup, DOC("Free Image resources"), VOIDRET,
    ARG(image, "Image_Ptr", DOC("Image to cleanup"))
);

FUNCTION(image_load, DOC("Load the image specified in the path."), RET("Image_Ptr", DOC("Loaded image")),
    ARG(image_path, STRING, DOC("Path to the image file"))
);

FUNCTION(image_draw, DOC("Draw an image."), VOIDRET,
    ARG(image, "Image_Ptr", DOC("Image to draw")),
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate"))
);

FUNCTION(image_draw_ex, DOC("Draw an image."), VOIDRET,
    ARG(image, "Image_Ptr", DOC("Image to draw")),
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(angle, DOUBLE, DOC("Rotation angle")),
    ARG(origin_x, DOUBLE, DOC("Origin X coordinate")),
    ARG(origin_y, DOUBLE, DOC("Origin Y coordinate")),
    ARG(scale_x, DOUBLE, DOC("X scale factor")),
    ARG(scale_y, DOUBLE, DOC("Y scale factor"))
);

FUNCTION(image_draw_rect, DOC("Draw a rectangular area from the image."), VOIDRET,
    ARG(image, "Image_Ptr", DOC("Image to draw")),
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(src_x, DOUBLE, DOC("Source X coordinate")),
    ARG(src_y, DOUBLE, DOC("Source Y coordinate")),
    ARG(src_width, DOUBLE, DOC("Source width")),
    ARG(src_height, DOUBLE, DOC("Source height"))
);

FUNCTION(image_draw_rect_ex, DOC("Draw a rectangular area from the image."), VOIDRET,
    ARG(image, "Image_Ptr", DOC("Image to draw")),
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(src_x, DOUBLE, DOC("Source X coordinate")),
    ARG(src_y, DOUBLE, DOC("Source Y coordinate")),
    ARG(src_width, DOUBLE, DOC("Source width")),
    ARG(src_height, DOUBLE, DOC("Source height")),
    ARG(angle, DOUBLE, DOC("Rotation angle")),
    ARG(origin_x, DOUBLE, DOC("Origin X coordinate")),
    ARG(origin_y, DOUBLE, DOC("Origin Y coordinate")),
    ARG(scale_x, DOUBLE, DOC("X scale factor")),
    ARG(scale_y, DOUBLE, DOC("Y scale factor"))
);

FUNCTION(image_get_width, DOC("Get the width of the image."), RET(INT32, DOC("Image width")),
    ARG(image, "Image_Ptr", DOC("Image"))
);

FUNCTION(image_get_height, DOC("Get the height of the image."), RET(INT32, DOC("Image height")),
    ARG(image, "Image_Ptr", DOC("Image"))
);

FUNCTION(image_new_canvas, DOC("Create a canvas image with given width and height."), RET("Image_Ptr", DOC("Canvas image")),
    ARG(width, INT32, DOC("Canvas width")),
    ARG(height, INT32, DOC("Canvas height"))
);

FUNCTION(image_set_canvas, DOC("Set the effective canvas image. All draw operations will go to this canvas until it's reset."), VOIDRET,
    ARG(canvas_image, "Image_Ptr", DOC("Canvas image"))
);

FUNCTION(reset_canvas, DOC("Reset the drawing target, back to screen."), VOIDRET);

FUNCTION(image_is_canvas, DOC("Check if the image was created as a canvas."), RET(INT32, DOC("1 if canvas, 0 otherwise")),
    ARG(image, "Image_Ptr", DOC("Image"))
);

FUNCTION(capture_image, DOC("Note: slow. Capture render image. This is a slow operation!"), RET("Image_Ptr", DOC("Captured image")),
    ARG(x, INT32, DOC("X coordinate")),
    ARG(y, INT32, DOC("Y coordinate")),
    ARG(w, INT32, DOC("Width")),
    ARG(h, INT32, DOC("Height"))
);

FUNCTION(imagebatch_cleanup, DOC("Free ImageBatch resources"), VOIDRET,
    ARG(imagebatch, "ImageBatch_Ptr", DOC("ImageBatch to cleanup"))
);

FUNCTION(imagebatch_new, DOC("Create an image batch"), RET("ImageBatch_Ptr", DOC("New image batch")),
    ARG(image, "Image_Ptr", DOC("Base image"))
);

FUNCTION(imagebatch_reset, DOC("Reset the image batch, remove all added rects."), VOIDRET,
    ARG(imagebatch, "ImageBatch_Ptr", DOC("ImageBatch"))
);

FUNCTION(imagebatch_add_rect, DOC("Add a recta to the image batch (from it's initial image)."), VOIDRET,
    ARG(imagebatch, "ImageBatch_Ptr", DOC("ImageBatch")),
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate")),
    ARG(dest_width, DOUBLE, DOC("Destination width")),
    ARG(dest_height, DOUBLE, DOC("Destination height")),
    ARG(src_x, DOUBLE, DOC("Source X coordinate")),
    ARG(src_y, DOUBLE, DOC("Source Y coordinate")),
    ARG(src_width, DOUBLE, DOC("Source width")),
    ARG(src_height, DOUBLE, DOC("Source height"))
);

FUNCTION(imagebatch_get_rect_count, DOC("Get the number of rects in the image batch."), RET(INT32, DOC("Number of rects")),
    ARG(imagebatch, "ImageBatch_Ptr", DOC("ImageBatch"))
);

FUNCTION(imagebatch_draw, DOC("Draw the image batch."), VOIDRET,
    ARG(imagebatch, "ImageBatch_Ptr", DOC("ImageBatch"))
);

FUNCTION(font_cleanup, DOC("Free Font resources"), VOIDRET,
    ARG(font, "Font_Ptr", DOC("Font to cleanup"))
);

FUNCTION(font_load, DOC("Load the font specified in the path, and set the initial size."), RET("Font_Ptr", DOC("Loaded font")),
    ARG(font_path, STRING, DOC("Path to font file")),
    ARG(size, DOUBLE, DOC("Font size"))
);

FUNCTION(font_set, DOC("Set the effective font to be used in the drawing operations."), VOIDRET,
    ARG(font, "Font_Ptr", DOC("Font"))
);

FUNCTION(draw_text, DOC("Draw a text line."), VOIDRET,
    ARG(text, STRING, DOC("Text to draw")),
    ARG(dest_x, DOUBLE, DOC("Destination X coordinate")),
    ARG(dest_y, DOUBLE, DOC("Destination Y coordinate"))
);

FUNCTION(get_text_width, DOC("Get the width of the given text line."), RET(INT32, DOC("Text width")),
    ARG(text, STRING, DOC("Text"))
);

FUNCTION(get_text_height, DOC("Get the height of the given text line."), RET(INT32, DOC("Text height")),
    ARG(text, STRING, DOC("Text"))
);

FUNCTION(get_monitor_count, DOC("Get the number of currently connected monitors."), RET(INT32, DOC("Number of monitors")));

FUNCTION(get_monitor_name, DOC("Get the name of the monitor at the index"), RET(STRING, DOC("Monitor name")),
    ARG(index, INT32, DOC("Monitor index"))
);

FUNCTION(get_monitor_width, DOC("Get the width of the monitor at the index"), RET(INT32, DOC("Monitor width")),
    ARG(index, INT32, DOC("Monitor index"))
);

FUNCTION(get_monitor_height, DOC("Get the height of the monitor at the index"), RET(INT32, DOC("Monitor height")),
    ARG(index, INT32, DOC("Monitor index"))
);

FUNCTION(set_window_monitor, DOC("Set the window's initial monitor to the indexed value. Must be set before the window is opened."), VOIDRET,
    ARG(index, INT32, DOC("Monitor index"))
);

FUNCTION(set_window_resizable, DOC("Set the window resizable flag to the given value. Must be set before the window is opened."), VOIDRET,
    ARG(resizable, INT32, DOC("Resizable flag (1 for true, 0 for false)"))
);

FUNCTION(set_window_minsize, DOC("Set the window's minimum possible size."), VOIDRET,
    ARG(width, INT32, DOC("Minimum width")),
    ARG(height, INT32, DOC("Minimum height"))
);

FUNCTION(set_window_size, DOC("Set the window's size."), VOIDRET,
    ARG(width, INT32, DOC("Width")),
    ARG(height, INT32, DOC("Height"))
);

FUNCTION(get_window_width, DOC("Get the width of the window."), RET(INT32, DOC("Width of the window in pixels")));

FUNCTION(get_window_height, DOC("Get the height of the window."), RET(INT32, DOC("Height of the window in pixels")));

FUNCTION(set_window_position, DOC("Set the window's position."), VOIDRET,
    ARG(x, INT32, DOC("X coordinate")),
    ARG(y, INT32, DOC("Y coordinate"))
);

FUNCTION(set_fullscreen, DOC("Set the window to fullscreen, or windowed mode."), VOIDRET,
    ARG(fullscreen, INT32, DOC("Fullscreen flag (1 for true, 0 for false)"))
);

FUNCTION(is_fullscreen, DOC("Check if the window is set to fullscreen."), RET(INT32, DOC("1 if fullscreen, 0 otherwise")));

FUNCTION(set_window_title, DOC("Set the window's title."), VOIDRET,
    ARG(title, STRING, DOC("Window title"))
);

FUNCTION(set_window_vsync, DOC("Set the window vsync flag to the given value."), VOIDRET,
    ARG(vsync, INT32, DOC("Vsync flag (1 for true, 0 for false)"))
);

FUNCTION(is_window_vsync, DOC("Check if the window vsync flag is set."), RET(INT32, DOC("1 if vsync, 0 otherwise")));

FUNCTION(set_window_icon_file, DOC("Set the window icon."), VOIDRET,
    ARG(icon_path, STRING, DOC("Path to icon file"))
);

FUNCTION(set_window_margins, DOC("Set the window margins. Margins are ignored and no drawing can be made there."), VOIDRET,
    ARG(left, INT32, DOC("Left margin")),
    ARG(right, INT32, DOC("Right margin")),
    ARG(top, INT32, DOC("Top margin")),
    ARG(bottom, INT32, DOC("Bottom margin"))
);

FUNCTION(set_window_paddings, DOC("Set the window paddings. Paddings are can be drawn on."), VOIDRET,
    ARG(left, INT32, DOC("Left padding")),
    ARG(right, INT32, DOC("Right padding")),
    ARG(top, INT32, DOC("Top padding")),
    ARG(bottom, INT32, DOC("Bottom padding"))
);

FUNCTION(is_key_down, DOC("Check if the given key is down."), RET(INT32, DOC("1 if down, 0 otherwise")),
    ARG(key, "KeyboardKey", DOC("Key to check"))
);

FUNCTION(is_key_pressed, DOC("Check if the given key is pressed."), RET(INT32, DOC("1 if pressed, 0 otherwise")),
    ARG(key, "KeyboardKey", DOC("Key to check"))
);

FUNCTION(is_key_released, DOC("Check if the given key is released."), RET(INT32, DOC("1 if released, 0 otherwise")),
    ARG(key, "KeyboardKey", DOC("Key to check"))
);

FUNCTION(is_key_repeat, DOC("Check if the given key is repeated."), RET(INT32, DOC("1 if repeated, 0 otherwise")),
    ARG(key, "KeyboardKey", DOC("Key to check"))
);

FUNCTION(get_pressed_keys, DOC("Get the list of pressed keys."), RET("KeyList", DOC("List of pressed keys")));

FUNCTION(get_textinput, DOC("Get all keyboard text input from last frame. Output is utf8 encoded."), RET(STRING, DOC("Text input")));

FUNCTION(is_mouse_down, DOC("Check if the given mouse button is down."), RET(INT32, DOC("1 if down, 0 otherwise")),
    ARG(mouse_button, "MouseButton", DOC("Mouse button to check"))
);

FUNCTION(is_mouse_pressed, DOC("Check if the given mouse button is pressed."), RET(INT32, DOC("1 if pressed, 0 otherwise")),
    ARG(mouse_button, "MouseButton", DOC("Mouse button to check"))
);

FUNCTION(is_mouse_released, DOC("Check if the given mouse button is released."), RET(INT32, DOC("1 if released, 0 otherwise")),
    ARG(mouse_button, "MouseButton", DOC("Mouse button to check"))
);

FUNCTION(get_mouse_x, DOC("Get the mouse x position."), RET(INT32, DOC("Mouse X position")));

FUNCTION(get_mouse_y, DOC("Get the mouse y position."), RET(INT32, DOC("Mouse Y position")));

FUNCTION(get_gamepad_count, DOC("Get the number of gamepads."), RET(INT32, DOC("Number of gamepads")));

FUNCTION(get_gamepad_name, DOC("Get the name of the gamepad at the given index."), RET(STRING, DOC("Gamepad name")),
    ARG(index, INT32, DOC("Gamepad index"))
);

FUNCTION(is_gamepad_down, DOC("Check if the given button of the gamepad at the given index is down."), RET(INT32, DOC("1 if down, 0 otherwise")),
    ARG(index, INT32, DOC("Gamepad index")),
    ARG(gamepad_button, "GamepadButton", DOC("Button to check"))
);

FUNCTION(is_gamepad_pressed, DOC("Check if the given button of the gamepad at the given index is pressed."), RET(INT32, DOC("1 if pressed, 0 otherwise")),
    ARG(index, INT32, DOC("Gamepad index")),
    ARG(gamepad_button, "GamepadButton", DOC("Button to check"))
);

FUNCTION(is_gamepad_released, DOC("Check if the given button of the gamepad at the given index is released."), RET(INT32, DOC("1 if released, 0 otherwise")),
    ARG(index, INT32, DOC("Gamepad index")),
    ARG(gamepad_button, "GamepadButton", DOC("Button to check"))
);

FUNCTION(get_gamepad_axis, DOC("Get the given axis of the gamepad at the given index."), RET(DOUBLE, DOC("Axis value")),
    ARG(index, INT32, DOC("Gamepad index")),
    ARG(gamepad_axis, "GamepadAxis", DOC("Axis to check"))
);

FUNCTION(set_mastervolume, DOC("Set the master volume."), VOIDRET,
    ARG(mastervolume, DOUBLE, DOC("Master volume value"))
);

FUNCTION(get_mastervolume, DOC("Get the master volume."), RET(DOUBLE, DOC("Master volume value")));

FUNCTION(music_cleanup, DOC("Free Music resources"), VOIDRET,
    ARG(music, "Music_Ptr", DOC("Music to cleanup"))
);

FUNCTION(music_load, DOC("Load the music specified in the path."), RET("Music_Ptr", DOC("Loaded music")),
    ARG(music_path, STRING, DOC("Path to music file"))
);

FUNCTION(music_play, DOC("Play the music."), VOIDRET,
    ARG(music, "Music_Ptr", DOC("Music to play"))
);

FUNCTION(music_pause, DOC("Pause the music."), VOIDRET,
    ARG(music, "Music_Ptr", DOC("Music to pause"))
);

FUNCTION(music_resume, DOC("Resume the music."), VOIDRET,
    ARG(music, "Music_Ptr", DOC("Music to resume"))
);

FUNCTION(music_stop, DOC("Stop the music."), VOIDRET,
    ARG(music, "Music_Ptr", DOC("Music to stop"))
);

FUNCTION(music_is_playing, DOC("Check if the given music is playing."), RET(INT32, DOC("1 if playing, 0 otherwise")),
    ARG(music, "Music_Ptr", DOC("Music"))
);

FUNCTION(music_get_length, DOC("Get the length of the given music object in seconds."), RET(DOUBLE, DOC("Music length")),
    ARG(music, "Music_Ptr", DOC("Music"))
);

FUNCTION(music_get_length_played, DOC("Get the already played length of the given music object in seconds."), RET(DOUBLE, DOC("Played length")),
    ARG(music, "Music_Ptr", DOC("Music"))
);

FUNCTION(music_seek, DOC("Move the music time played to the given value."), VOIDRET,
    ARG(music, "Music_Ptr", DOC("Music")),
    ARG(secs, DOUBLE, DOC("Time in seconds"))
);

FUNCTION(music_set_volume, DOC("Set the volume of the given music object."), VOIDRET,
    ARG(music, "Music_Ptr", DOC("Music")),
    ARG(volume, DOUBLE, DOC("Volume value"))
);

FUNCTION(music_set_pan, DOC("Set the pan of the given music object."), VOIDRET,
    ARG(music, "Music_Ptr", DOC("Music")),
    ARG(pan, DOUBLE, DOC("Pan value"))
);

FUNCTION(music_set_pitch, DOC("Set the pitch of the given music object."), VOIDRET,
    ARG(music, "Music_Ptr", DOC("Music")),
    ARG(pitch, DOUBLE, DOC("Pitch value"))
);

FUNCTION(music_get_volume, DOC("Get the volume of the given music object."), RET(DOUBLE, DOC("Volume value")),
    ARG(music, "Music_Ptr", DOC("Music"))
);

FUNCTION(music_get_pan, DOC("Get the pan of the given music object."), RET(DOUBLE, DOC("Pan value")),
    ARG(music, "Music_Ptr", DOC("Music"))
);

FUNCTION(music_get_pitch, DOC("Get the pitch of the given music object."), RET(DOUBLE, DOC("Pitch value")),
    ARG(music, "Music_Ptr", DOC("Music"))
);

FUNCTION(sound_cleanup, DOC("Free Sound resources"), VOIDRET,
    ARG(sound, "Sound_Ptr", DOC("Sound to cleanup"))
);

FUNCTION(sound_load, DOC("Load the sound specified in the path."), RET("Sound_Ptr", DOC("Loaded sound")),
    ARG(sound_path, STRING, DOC("Path to sound file"))
);

FUNCTION(sound_clone, DOC("Clone the sound specified in the path."), RET("Sound_Ptr", DOC("Cloned sound")),
    ARG(orig, "Sound_Ptr", DOC("Original sound"))
);

FUNCTION(sound_play, DOC("Play the sound."), VOIDRET,
    ARG(sound, "Sound_Ptr", DOC("Sound to play"))
);

FUNCTION(sound_pause, DOC("Pause the sound."), VOIDRET,
    ARG(sound, "Sound_Ptr", DOC("Sound to pause"))
);

FUNCTION(sound_resume, DOC("Resume the sound."), VOIDRET,
    ARG(sound, "Sound_Ptr", DOC("Sound to resume"))
);

FUNCTION(sound_stop, DOC("Stop the sound."), VOIDRET,
    ARG(sound, "Sound_Ptr", DOC("Sound to stop"))
);

FUNCTION(sound_is_playing, DOC("Check if the given sound is playing."), RET(INT32, DOC("1 if playing, 0 otherwise")),
    ARG(sound, "Sound_Ptr", DOC("Sound"))
);

FUNCTION(sound_set_volume, DOC("Set the volume of the given sound object."), VOIDRET,
    ARG(sound, "Sound_Ptr", DOC("Sound")),
    ARG(volume, DOUBLE, DOC("Volume value"))
);

FUNCTION(sound_set_pan, DOC("Set the pan of the given sound object."), VOIDRET,
    ARG(sound, "Sound_Ptr", DOC("Sound")),
    ARG(pan, DOUBLE, DOC("Pan value"))
);

FUNCTION(sound_set_pitch, DOC("Set the pitch of the given sound object."), VOIDRET,
    ARG(sound, "Sound_Ptr", DOC("Sound")),
    ARG(pitch, DOUBLE, DOC("Pitch value"))
);

FUNCTION(sound_get_volume, DOC("Get the volume of the given sound object."), RET(DOUBLE, DOC("Volume value")),
    ARG(sound, "Sound_Ptr", DOC("Sound"))
);

FUNCTION(sound_get_pan, DOC("Get the pan of the given sound object."), RET(DOUBLE, DOC("Pan value")),
    ARG(sound, "Sound_Ptr", DOC("Sound"))
);

FUNCTION(sound_get_pitch, DOC("Get the pitch of the given sound object."), RET(DOUBLE, DOC("Pitch value")),
    ARG(sound, "Sound_Ptr", DOC("Sound"))
);

FUNCTION(load_textfile, DOC("Load the file in the path."), RET(STRING, DOC("File contents")),
    ARG(file_path, STRING, DOC("Path to text file"))
);

FUNCTION(save_textfile, DOC("Append the text to the file in the path. Override if the file exists. Create if it doesn't exist."), VOIDRET,
    ARG(file_path, STRING, DOC("Path to text file")),
    ARG(data, STRING, DOC("Text data"))
);

FUNCTION(save_textfile_append, DOC("Append the text to the file in the path. Append at the end if the file exists. Create if it doesn't exist."), VOIDRET,
    ARG(file_path, STRING, DOC("Path to text file")),
    ARG(data, STRING, DOC("Text data"))
);

FUNCTION(push_matrix, DOC("Push the transform matrix."), VOIDRET);

FUNCTION(pop_matrix, DOC("Pop the transform matrix."), VOIDRET);

FUNCTION(reset_matrix, DOC("Reset the transformation matrix (load identity matrix.)"), VOIDRET);

FUNCTION(translate, DOC("Apply translation (changes transform matrix.)"), VOIDRET,
    ARG(delta_x, DOUBLE, DOC("X translation")),
    ARG(delta_y, DOUBLE, DOC("Y translation"))
);

FUNCTION(rotate, DOC("Apply rotation (changes transform matrix.)"), VOIDRET,
    ARG(angle, DOUBLE, DOC("Rotation angle"))
);

FUNCTION(rotate_at, DOC("Apply rotation at the given location (changes transform matrix.)"), VOIDRET,
    ARG(angle, DOUBLE, DOC("Rotation angle")),
    ARG(x, DOUBLE, DOC("X coordinate")),
    ARG(y, DOUBLE, DOC("Y coordinate"))
);

FUNCTION(scale, DOC("Apply scaling (changes transform matrix.)"), VOIDRET,
    ARG(scale_x, DOUBLE, DOC("X scale factor")),
    ARG(scale_y, DOUBLE, DOC("Y scale factor"))
);

FUNCTION(scale_at, DOC("Apply scaling at the given location (changes transform matrix.)"), VOIDRET,
    ARG(scale_x, DOUBLE, DOC("X scale factor")),
    ARG(scale_y, DOUBLE, DOC("Y scale factor")),
    ARG(x, DOUBLE, DOC("X coordinate")),
    ARG(y, DOUBLE, DOC("Y coordinate"))
);

FUNCTION(set_default_blendmode, DOC("Set the default blendmode."), VOIDRET,
    ARG(blendmode, "BlendMode", DOC("Blend mode"))
);

FUNCTION(set_blendmode, DOC("Set the effective blendmode."), VOIDRET,
    ARG(blendmode, "BlendMode", DOC("Blend mode"))
);

FUNCTION(reset_blendmode, DOC("Reset the blendmode value to its default value."), VOIDRET);

FUNCTION(set_default_filtermode, DOC("Set the default filtermode."), VOIDRET,
    ARG(filtermode, "FilterMode", DOC("Filter mode"))
);

FUNCTION(set_filtermode, DOC("Set the effective filtermode."), VOIDRET,
    ARG(filtermode, "FilterMode", DOC("Filter mode"))
);

FUNCTION(reset_filtermode, DOC("Reset the filtermode value to its default value."), VOIDRET);

FUNCTION(shaderbuilder_cleanup, DOC("Free ShaderBuilder resources"), VOIDRET,
    ARG(shaderbuilder, "ShaderBuilder_Ptr", DOC("ShaderBuilder to cleanup"))
);

FUNCTION(shaderbuilder_new, DOC("Create a ShaderBuilder object."), RET("ShaderBuilder_Ptr", DOC("New ShaderBuilder")));

FUNCTION(shaderbuilder_uniform, DOC("Add uniform definition to the shaderbuilder"), VOIDRET,
    ARG(shaderbuilder, "ShaderBuilder_Ptr", DOC("ShaderBuilder")),
    ARG(uniform_name, STRING, DOC("Uniform name")),
    ARG(uniform_type, "UniformType", DOC("Uniform type"))
);

FUNCTION(shaderbuilder_vertex, DOC("Add vertex code to the shaderbuilder"), VOIDRET,
    ARG(shaderbuilder, "ShaderBuilder_Ptr", DOC("ShaderBuilder")),
    ARG(vertex_code, STRING, DOC("Vertex shader code"))
);

FUNCTION(shaderbuilder_fragment, DOC("Add fragment to the shaderbuilder"), VOIDRET,
    ARG(shaderbuilder, "ShaderBuilder_Ptr", DOC("ShaderBuilder")),
    ARG(fragment_code, STRING, DOC("Fragment shader code"))
);

FUNCTION(shaderbuilder_build, DOC("Add fragment to the shaderbuilder"), RET("Shader_Ptr", DOC("Built shader")),
    ARG(shaderbuilder, "ShaderBuilder_Ptr", DOC("ShaderBuilder"))
);

FUNCTION(shader_cleanup, DOC("Free Shader resources"), VOIDRET,
    ARG(shader, "Shader_Ptr", DOC("Shader to cleanup"))
);

FUNCTION(shader_set, DOC("Set the custom shader and use it for consequent calls."), VOIDRET,
    ARG(shader, "Shader_Ptr", DOC("Shader"))
);

FUNCTION(reset_shader, DOC("Reset the shader, back to framework defaults."), VOIDRET);

FUNCTION(shader_set_uniform_float, DOC("Set the specified uniform."), VOIDRET,
    ARG(shader, "Shader_Ptr", DOC("Shader")),
    ARG(uniform_name, STRING, DOC("Uniform name")),
    ARG(uniform_value, FLOAT, DOC("Uniform value"))
);

FUNCTION(shader_set_uniform_floatvec4, DOC("Set the specified uniform."), VOIDRET,
    ARG(shader, "Shader_Ptr", DOC("Shader")),
    ARG(uniform_name, STRING, DOC("Uniform name")),
    ARG(uniform_value, "FloatVec4", DOC("Uniform value"))
);

FUNCTION(shader_set_uniform_image, DOC("Set the specified uniform."), VOIDRET,
    ARG(shader, "Shader_Ptr", DOC("Shader")),
    ARG(uniform_name, STRING, DOC("Uniform name")),
    ARG(uniform_value, "Image_Ptr", DOC("Uniform value"))
);

FUNCTION(shader_reset_uniform, DOC("Reset the specified uniform."), VOIDRET,
    ARG(shader, "Shader_Ptr", DOC("Shader")),
    ARG(uniform_name, STRING, DOC("Uniform name"))
);

FUNCTION(world_new, DOC("Create a new physics world."), RET("World_Ptr", DOC("New world")));

FUNCTION(world_cleanup, DOC("Cleanup (delete) world and all contained objects."), VOIDRET,
    ARG(world, "World_Ptr", DOC("World to cleanup"))
);

FUNCTION(world_set_gravity, DOC("Set worlds gravity values. By default, gravity is (0, 0)."), VOIDRET,
    ARG(world, "World_Ptr", DOC("World")),
    ARG(x, DOUBLE, DOC("X gravity")),
    ARG(y, DOUBLE, DOC("Y gravity"))
);

FUNCTION(world_update, DOC("Update world."), VOIDRET,
    ARG(world, "World_Ptr", DOC("World")),
    ARG(step_size, DOUBLE, DOC("Time step size"))
);

FUNCTION(body_new, DOC("Create a new physics body."), RET("Body_Ptr", DOC("New body")),
    ARG(world, "World_Ptr", DOC("World"))
);

FUNCTION(body_cleanup, DOC("Cleanup (delete) a body."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body to cleanup"))
);

FUNCTION(body_set_position, DOC("Set body's position."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(x, DOUBLE, DOC("X coordinate")),
    ARG(y, DOUBLE, DOC("Y coordinate"))
);

FUNCTION(body_get_position, DOC("Get body's position."), RET("Vec2", DOC("Position vector")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(body_set_rotation, DOC("Set body's rotation (angle)."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(angle, DOUBLE, DOC("Rotation angle"))
);

FUNCTION(body_get_rotation, DOC("Get body's rotation (angle)."), RET(DOUBLE, DOC("Rotation angle")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(body_set_linear_vel, DOC("Set body's linear velocity."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(x, DOUBLE, DOC("X velocity")),
    ARG(y, DOUBLE, DOC("Y velocity"))
);

FUNCTION(body_get_linear_vel, DOC("Get body's linear velocity."), RET("Vec2", DOC("Velocity vector")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(body_set_angular_vel, DOC("Set body's angular velocity."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(z, DOUBLE, DOC("Angular velocity"))
);

FUNCTION(body_get_angular_vel, DOC("Get body's angular velocity."), RET(DOUBLE, DOC("Angular velocity")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(body_set_mass_circle, DOC("Set body's (circular) mass."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(mass, DOUBLE, DOC("Mass value")),
    ARG(radius, DOUBLE, DOC("Radius"))
);

FUNCTION(body_set_mass_rect, DOC("Set body's (rectangular) mass."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(mass, DOUBLE, DOC("Mass value")),
    ARG(width, DOUBLE, DOC("Width")),
    ARG(height, DOUBLE, DOC("Height"))
);

FUNCTION(body_add_force, DOC("Add force to body."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(fx, DOUBLE, DOC("X force")),
    ARG(fy, DOUBLE, DOC("Y force"))
);

FUNCTION(body_get_force, DOC("Get body's linear."), RET("Vec2", DOC("Force vector")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(body_add_torque, DOC("Add torque to body."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(fz, DOUBLE, DOC("Torque value"))
);

FUNCTION(body_get_torque, DOC("Get body's torque."), RET(DOUBLE, DOC("Torque value")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(body_set_kinematic, DOC("Set body as 'kinematic'. False by default."), VOIDRET,
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(val, INT32, DOC("Kinematic flag (1 for true, 0 for false)"))
);

FUNCTION(body_is_kinematic, DOC("Is body set as 'kinematic'? False by default."), RET(INT32, DOC("1 if kinematic, 0 otherwise")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(space_new, DOC("Create a new Space for collisions"), RET("Space_Ptr", DOC("New space")));

FUNCTION(space_cleanup, DOC("Cleanup the Space (delete)."), VOIDRET,
    ARG(space, "Space_Ptr", DOC("Space to cleanup"))
);

FUNCTION(jointgroup_new, DOC("Create a new JointGroup."), RET("JointGroup_Ptr", DOC("New joint group")));

FUNCTION(jointgroup_cleanup, DOC("Cleanup the JointGroup (delete)."), VOIDRET,
    ARG(jointgroup, "JointGroup_Ptr", DOC("JointGroup to cleanup"))
);

FUNCTION(coll_update_check, DOC("Check collisions for current frame"), VOIDRET,
    ARG(world, "World_Ptr", DOC("World")),
    ARG(space, "Space_Ptr", DOC("Space")),
    ARG(jointgroup, "JointGroup_Ptr", DOC("JointGroup"))
);

FUNCTION(coll_update_correct, DOC("Correct angular drift for 2D"), VOIDRET,
    ARG(space, "Space_Ptr", DOC("Space"))
);

FUNCTION(body_get_collision_count, DOC("Get the number of collisions for this body."), RET(INT32, DOC("Collision count")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(body_get_collision_data_at, DOC("Get the data of for collision for this body at given index. Indexes start with 0."), RET("CollisionData", DOC("Collision data")),
    ARG(body, "Body_Ptr", DOC("Body")),
    ARG(index, INT32, DOC("Collision index"))
);

FUNCTION(joint_cleanup, DOC("Cleanup (delete) given joint."), VOIDRET,
    ARG(joint, "Joint_Ptr", DOC("Joint to cleanup"))
);

FUNCTION(joint_is_hinge, DOC("Check if the Joint is of class 'hinge'."), RET(INT32, DOC("1 if hinge, 0 otherwise")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_is_slider, DOC("Check if the Joint is of class 'slider'."), RET(INT32, DOC("1 if slider, 0 otherwise")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_is_fixed, DOC("Check if the Joint is of class 'fixed'."), RET(INT32, DOC("1 if fixed, 0 otherwise")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_get_body, DOC("Get the body at 'index' for the given joint."), RET("Body_Ptr", DOC("Body")),
    ARG(joint, "Joint_Ptr", DOC("Joint")),
    ARG(index, INT32, DOC("Body index (0 or 1)"))
);

FUNCTION(joint_attach, DOC("Attach the given bodies with the joint."), VOIDRET,
    ARG(joint, "Joint_Ptr", DOC("Joint")),
    ARG(body1, "Body_Ptr", DOC("First body")),
    ARG(body2, "Body_Ptr", DOC("Second body"))
);

FUNCTION(joint_attach_one, DOC("Attach the given body with to the 'world'."), VOIDRET,
    ARG(joint, "Joint_Ptr", DOC("Joint")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(joint_new_hinge, DOC("Create a new Joint of class 'hinge'."), RET("Joint_Ptr", DOC("New hinge joint")),
    ARG(world, "World_Ptr", DOC("World")),
    ARG(jointgroup, "JointGroup_Ptr", DOC("JointGroup"))
);

FUNCTION(joint_new_slider, DOC("Create a new Joint of class 'slider'."), RET("Joint_Ptr", DOC("New slider joint")),
    ARG(world, "World_Ptr", DOC("World")),
    ARG(jointgroup, "JointGroup_Ptr", DOC("JointGroup"))
);

FUNCTION(joint_new_fixed, DOC("Create a new Joint of class 'fixed'."), RET("Joint_Ptr", DOC("New fixed joint")),
    ARG(world, "World_Ptr", DOC("World")),
    ARG(jointgroup, "JointGroup_Ptr", DOC("JointGroup"))
);

FUNCTION(joint_set_hinge_anchor, DOC("Set the anchor location for the given hinge joint."), VOIDRET,
    ARG(joint, "Joint_Ptr", DOC("Joint")),
    ARG(x, DOUBLE, DOC("X coordinate")),
    ARG(y, DOUBLE, DOC("Y coordinate"))
);

FUNCTION(joint_get_hinge_anchor1, DOC("Get the anchor location for the given hinge joint. From body1. Should be the same for body2."), RET("Vec2", DOC("Anchor position")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_get_hinge_anchor2, DOC("Get the anchor location for the given hinge joint. From body2. Should be the same for body1."), RET("Vec2", DOC("Anchor position")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_get_hinge_angle, DOC("Get the angle or the given hinge joint."), RET(DOUBLE, DOC("Hinge angle")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_get_hinge_angle_rate, DOC("Get the angle rate for the given hinge joint."), RET(DOUBLE, DOC("Angle rate")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_set_slider_axis, DOC("Set the axis for the given slider joint."), VOIDRET,
    ARG(joint, "Joint_Ptr", DOC("Joint")),
    ARG(x, DOUBLE, DOC("X axis")),
    ARG(y, DOUBLE, DOC("Y axis"))
);

FUNCTION(joint_get_slider_axis, DOC("Get the axis for the given slider joint."), RET("Vec2", DOC("Slider axis")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_get_slider_position, DOC("Get the slider position for the given slider joint."), RET(DOUBLE, DOC("Slider position")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_get_slider_position_rate, DOC("Get the slider position rate for the given slider joint."), RET(DOUBLE, DOC("Position rate")),
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(joint_set_fixed, DOC("Set the joint as 'fixed'."), VOIDRET,
    ARG(joint, "Joint_Ptr", DOC("Joint"))
);

FUNCTION(geom_cleanup, DOC("Cleanup the Geom (delete)."), VOIDRET,
    ARG(geom, "Geom_Ptr", DOC("Geom to cleanup"))
);

FUNCTION(geom_new_circle, DOC("Create a new Geom (for collision)."), RET("Geom_Ptr", DOC("New circle geom")),
    ARG(space, "Space_Ptr", DOC("Space")),
    ARG(radius, DOUBLE, DOC("Radius"))
);

FUNCTION(geom_new_rect, DOC("Create a new Geom (for collision)."), RET("Geom_Ptr", DOC("New rect geom")),
    ARG(space, "Space_Ptr", DOC("Space")),
    ARG(width, DOUBLE, DOC("Width")),
    ARG(height, DOUBLE, DOC("Height"))
);

FUNCTION(geom_is_circle, DOC("Is the Geom a 'circle'?"), RET(INT32, DOC("1 if circle, 0 otherwise")),
    ARG(geom, "Geom_Ptr", DOC("Geom"))
);

FUNCTION(geom_is_rect, DOC("Is the Geom a 'rect'?"), RET(INT32, DOC("1 if rect, 0 otherwise")),
    ARG(geom, "Geom_Ptr", DOC("Geom"))
);

FUNCTION(geom_set_circle_radius, DOC("Set the Geom's (circle) radius."), VOIDRET,
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(radius, DOUBLE, DOC("Radius"))
);

FUNCTION(geom_get_circle_radius, DOC("Get the Geom's (circle) radius."), RET(DOUBLE, DOC("Radius")),
    ARG(geom, "Geom_Ptr", DOC("Geom"))
);

FUNCTION(geom_set_rect_size, DOC("Set the Geom's (rect) width and height."), VOIDRET,
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(width, DOUBLE, DOC("Width")),
    ARG(height, DOUBLE, DOC("Height"))
);

FUNCTION(geom_get_rect_size, DOC("Get the Geom's (rect) width and height."), RET("Vec2", DOC("Size vector")),
    ARG(geom, "Geom_Ptr", DOC("Geom"))
);

FUNCTION(geom_get_circle_point_depth, DOC("Get the depth of the point in Geom's (circle). Positive inside, Negative outside, Zero on the surface."), RET(DOUBLE, DOC("Depth")),
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(x, DOUBLE, DOC("X coordinate")),
    ARG(y, DOUBLE, DOC("Y coordinate"))
);

FUNCTION(geom_get_rect_point_depth, DOC("Get the depth of the point in Geom's (rect). Positive inside, Negative outside, Zero on the surface."), RET(DOUBLE, DOC("Depth")),
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(x, DOUBLE, DOC("X coordinate")),
    ARG(y, DOUBLE, DOC("Y coordinate"))
);

FUNCTION(geom_get_AABB, DOC("Get the AABB rectangle coord associated with the Geom."), RET("AABB", DOC("AABB coordinates")),
    ARG(geom, "Geom_Ptr", DOC("Geom"))
);

FUNCTION(geom_set_body, DOC("Set a Geom for the Body."), VOIDRET,
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(body, "Body_Ptr", DOC("Body"))
);

FUNCTION(geom_get_body, DOC("Get the Body associated with the Geom."), RET("Body_Ptr", DOC("Associated body")),
    ARG(geom, "Geom_Ptr", DOC("Geom"))
);

FUNCTION(geom_set_position, DOC("Set geom's position."), VOIDRET,
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(x, DOUBLE, DOC("X coordinate")),
    ARG(y, DOUBLE, DOC("Y coordinate"))
);

FUNCTION(geom_get_position, DOC("Get geom's position."), RET("Vec2", DOC("Position vector")),
    ARG(geom, "Geom_Ptr", DOC("Geom"))
);

FUNCTION(geom_set_rotation, DOC("Set geom's rotation (angle)."), VOIDRET,
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(angle, DOUBLE, DOC("Rotation angle"))
);

FUNCTION(geom_get_rotation, DOC("Get geom's rotation (angle)."), RET(DOUBLE, DOC("Rotation angle")),
    ARG(geom, "Geom_Ptr", DOC("Geom"))
);

FUNCTION(geom_set_category_bit, DOC("Set the category bit for the Geom. Between 0 and 63."), VOIDRET,
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(category_bit, INT32, DOC("Category bit (0-63)"))
);

FUNCTION(geom_is_category_bit_set, DOC("Is the category bit associated with the Geom? (Between 0 and 63.)"), RET(INT32, DOC("1 if set, 0 otherwise")),
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(category_bit, INT32, DOC("Category bit (0-63)"))
);

FUNCTION(geom_set_collide_bit, DOC("Set the collide bit for the Geom. Between 0 and 63."), VOIDRET,
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(collide_bit, INT32, DOC("Collide bit (0-63)"))
);

FUNCTION(geom_is_collide_bit_set, DOC("Is the collide bit associated with the Geom? (Between 0 and 63.)"), RET(INT32, DOC("1 if set, 0 otherwise")),
    ARG(geom, "Geom_Ptr", DOC("Geom")),
    ARG(collide_bit, INT32, DOC("Collide bit (0-63)"))
);

// --- Module Definition ---

MODULE(lyte_core, DOC("lyte_core namespace. Direct API mappings to C library."),
    MODULE_ENUMS(&UniformType, &BlendMode, &FilterMode, &GamepadAxis, &GamepadButton, &MouseButton, &KeyboardKey),
    MODULE_PTRS(&Image_Ptr, &ImageBatch_Ptr, &Font_Ptr, &Music_Ptr, &Sound_Ptr, &Shader_Ptr, &ShaderBuilder_Ptr, &World_Ptr, &Body_Ptr, &Space_Ptr, &JointGroup_Ptr, &Geom_Ptr, &Joint_Ptr),
    MODULE_ARRAYS(&FloatVec4, &KeyboardKeyArray),
    MODULE_RECORDS(&Vec2, &KeyList, &CollisionData, &AABB),
    MODULE_FUNCTIONS(&quit, &cls, &set_color, &reset_color, &draw_point, &draw_line, &draw_triangle, &draw_triangle_line,
                     &draw_rect, &draw_rect_line, &draw_circle, &draw_circle_line, &draw_ellipse, &draw_ellipse_line,
                     &image_cleanup, &image_load, &image_draw, &image_draw_ex, &image_draw_rect, &image_draw_rect_ex,
                     &image_get_width, &image_get_height, &image_new_canvas, &image_set_canvas, &reset_canvas,
                     &image_is_canvas, &capture_image, &imagebatch_cleanup, &imagebatch_new, &imagebatch_reset,
                     &imagebatch_add_rect, &imagebatch_get_rect_count, &imagebatch_draw, &font_cleanup, &font_load,
                     &font_set, &draw_text, &get_text_width, &get_text_height, &get_monitor_count, &get_monitor_name,
                     &get_monitor_width, &get_monitor_height, &set_window_monitor, &set_window_resizable,
                     &set_window_minsize, &set_window_size, &get_window_width, &get_window_height, &set_window_position,
                     &set_fullscreen, &is_fullscreen, &set_window_title, &set_window_vsync, &is_window_vsync,
                     &set_window_icon_file, &set_window_margins, &set_window_paddings, &is_key_down, &is_key_pressed,
                     &is_key_released, &is_key_repeat, &get_pressed_keys, &get_textinput, &is_mouse_down,
                     &is_mouse_pressed, &is_mouse_released, &get_mouse_x, &get_mouse_y, &get_gamepad_count,
                     &get_gamepad_name, &is_gamepad_down, &is_gamepad_pressed, &is_gamepad_released, &get_gamepad_axis,
                     &set_mastervolume, &get_mastervolume, &music_cleanup, &music_load, &music_play, &music_pause,
                     &music_resume, &music_stop, &music_is_playing, &music_get_length, &music_get_length_played,
                     &music_seek, &music_set_volume, &music_set_pan, &music_set_pitch, &music_get_volume,
                     &music_get_pan, &music_get_pitch, &sound_cleanup, &sound_load, &sound_clone, &sound_play,
                     &sound_pause, &sound_resume, &sound_stop, &sound_is_playing, &sound_set_volume, &sound_set_pan,
                     &sound_set_pitch, &sound_get_volume, &sound_get_pan, &sound_get_pitch, &load_textfile,
                     &save_textfile, &save_textfile_append, &push_matrix, &pop_matrix, &reset_matrix, &translate,
                     &rotate, &rotate_at, &scale, &scale_at, &set_default_blendmode, &set_blendmode, &reset_blendmode,
                     &set_default_filtermode, &set_filtermode, &reset_filtermode, &shaderbuilder_cleanup,
                     &shaderbuilder_new, &shaderbuilder_uniform, &shaderbuilder_vertex, &shaderbuilder_fragment,
                     &shaderbuilder_build, &shader_cleanup, &shader_set, &reset_shader, &shader_set_uniform_float,
                     &shader_set_uniform_floatvec4, &shader_set_uniform_image, &shader_reset_uniform, &world_new,
                     &world_cleanup, &world_set_gravity, &world_update, &body_new, &body_cleanup, &body_set_position,
                     &body_get_position, &body_set_rotation, &body_get_rotation, &body_set_linear_vel,
                     &body_get_linear_vel, &body_set_angular_vel, &body_get_angular_vel, &body_set_mass_circle,
                     &body_set_mass_rect, &body_add_force, &body_get_force, &body_add_torque, &body_get_torque,
                     &body_set_kinematic, &body_is_kinematic, &space_new, &space_cleanup, &jointgroup_new,
                     &jointgroup_cleanup, &coll_update_check, &coll_update_correct, &body_get_collision_count,
                     &body_get_collision_data_at, &joint_cleanup, &joint_is_hinge, &joint_is_slider, &joint_is_fixed,
                     &joint_get_body, &joint_attach, &joint_attach_one, &joint_new_hinge, &joint_new_slider,
                     &joint_new_fixed, &joint_set_hinge_anchor, &joint_get_hinge_anchor1, &joint_get_hinge_anchor2,
                     &joint_get_hinge_angle, &joint_get_hinge_angle_rate, &joint_set_slider_axis,
                     &joint_get_slider_axis, &joint_get_slider_position, &joint_get_slider_position_rate,
                     &joint_set_fixed, &geom_cleanup, &geom_new_circle, &geom_new_rect, &geom_is_circle,
                     &geom_is_rect, &geom_set_circle_radius, &geom_get_circle_radius, &geom_set_rect_size,
                     &geom_get_rect_size, &geom_get_circle_point_depth, &geom_get_rect_point_depth, &geom_get_AABB,
                     &geom_set_body, &geom_get_body, &geom_set_position, &geom_get_position, &geom_set_rotation,
                     &geom_get_rotation, &geom_set_category_bit, &geom_is_category_bit_set, &geom_set_collide_bit,
                     &geom_is_collide_bit_set)
);