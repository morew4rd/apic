#include "apic.h"

// --- Example API definition ---

ARRAY_TYPE(int32_buffer, INT32, 10,
    DOC("Array of 10 int32 elements"));

PTR_TYPE(node_ptr, "Node",
    DOC("Pointer to linked list node"));

RECORD(vec_f, DOC("2D vector"),
    FIELD(x, FLOAT, DOC("X coordinate")),
    FIELD(y, FLOAT, DOC("Y coordinate"))
);

RECORD(circle, NODOC,
    FIELD(radius, FLOAT, DOC("Radius"))
);

RECORD(rectangle, NODOC,
    FIELD(width, FLOAT, DOC("Width")),
    FIELD(height, FLOAT, DOC("Height"))
);

RECORD(Node, DOC("Linked list node"),
    FIELD(data, "int32_buffer", DOC("Data storage")),
    FIELD(next, "node_ptr", DOC("Next node pointer"))
);

VARIANT(shape, DOC("Geometric shape"),
    CHOICE(circle, "circle", DOC("Circle shape")),
    CHOICE(rectangle, "rectangle", DOC("Rectangle shape"))
);

FUNCTION(draw_shape, DOC("Draws a geometric shape"),
    RET(INT32, DOC("Status code")),
    ARG(shape, "shape", DOC("Shape to draw")),
    ARG(color, STRING, DOC("Color name"))
);

PTR_TYPE(shape_ptr, "shape", DOC("shape ptr"));

FUNCTION(draw_shape_ptr, DOC("Draws a geometric shape via ptr"),
    RET(INT32, DOC("Status code")),
    ARG(shape_ptr, "shape_ptr", DOC("Shape pointer to draw")),
    ARG(color, STRING, DOC("Color name"))
);

FUNCTION(log_message, DOC("Logs a message"),
    VOIDRET,
    ARG(message, STRING, DOC("Message to log"))
);

ENUM(ErrorCode, DOC("API error codes"),
    ENUM_FIELD(OK, 0, "Success", DOC("Operation succeeded")),
    ENUM_FIELD(NOT_FOUND, 1, "Not Found", DOC("Resource not found")),
    ENUM_FIELD(INVALID_INPUT, 2, "Invalid Input", DOC("Bad request parameters"))
);

MODULE(geometry, DOC("Complete geometry module with all components"),
    MODULE_RECORDS(&vec_f, &circle, &rectangle, &Node),
    MODULE_VARIANTS(&shape),
    MODULE_ARRAYS(&int32_buffer),
    MODULE_PTRS(&node_ptr, &shape_ptr),
    MODULE_FUNCTIONS(&draw_shape, &draw_shape_ptr, &log_message),
    MODULE_ENUMS(&ErrorCode)
);