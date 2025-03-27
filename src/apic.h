#ifndef APIC_H
#define APIC_H


#include <stddef.h>
#include <stdio.h>
#include <string.h>

// --- Primitive Types ---
#define INT32    "int32"
#define INT64    "int64"
#define FLOAT    "float"
#define DOUBLE   "double"
#define ISIZE    "isize"
#define USIZE    "usize"
#define BYTE     "byte"
#define STRING   "string"
#define VOID     "void"

// --- Core Structures ---
typedef struct Field {
    const char *name;
    const char *type;
    const char *doc;
} Field;

typedef struct Choice {
    const char *name;
    const char *type;
    const char *doc;
} Choice;

typedef struct Record {
    const char *name;
    const char *doc;
    const Field *fields;
    size_t fields_count;
} Record;

typedef struct Variant {
    const char *name;
    const char *doc;
    const Choice *choices;
    size_t choices_count;
} Variant;

typedef struct ArrayType {
    const char *name;
    const char *item_type;
    size_t size;
    const char *doc;
} ArrayType;

typedef struct PtrType {
    const char *name;
    const char *pointee_type;
    const char *doc;
} PtrType;

typedef struct Arg {
    const char *name;
    const char *type;
    const char *doc;
} Arg;

typedef struct Ret {
    const char *type;
    const char *doc;
} Ret;

typedef struct Function {
    const char *name;
    const char *doc;
    const Arg *inputs;
    size_t inputs_count;
    const Ret *output;
} Function;

typedef struct EnumField {
    const char *name;
    int value;
    const char *str_value;
    const char *doc;
} EnumField;

typedef struct Enum {
    const char *name;
    const char *doc;
    const EnumField *fields;
    size_t fields_count;
} Enum;

typedef struct Module {
    const char *name;
    const char *doc;
    const Record * const *records;
    const Variant * const *variants;
    const ArrayType * const *arrays;
    const PtrType * const *ptrs;
    const Function * const *functions;
    const Enum * const *enums;
    size_t records_count;
    size_t variants_count;
    size_t arrays_count;
    size_t ptrs_count;
    size_t functions_count;
    size_t enums_count;
} Module;


// --- Macros ---
#define CHOICE(NAME, TYPE, DOC_STR) \
    (Choice){.name=#NAME, .type=(TYPE), .doc=(DOC_STR)}

#define RET(TYPE, DOC_STR) \
    (Ret){.type=(TYPE), .doc=(DOC_STR)}

#define VOIDRET RET(VOID, NODOC)

#define DOC(str) str

#define NODOC NULL

#define RECORD(NAME, DOC_STR, ...) \
    static const Record NAME = (Record){ \
        .name=#NAME, .doc=(DOC_STR), \
        .fields=(Field[]){__VA_ARGS__}, \
        .fields_count=sizeof((Field[]){__VA_ARGS__})/sizeof(Field) \
    }

#define VARIANT(NAME, DOC_STR, ...) \
    static const Variant NAME = (Variant){ \
        .name=#NAME, .doc=(DOC_STR), \
        .choices=(Choice[]){__VA_ARGS__}, \
        .choices_count=sizeof((Choice[]){__VA_ARGS__})/sizeof(Choice) \
    }

#define ARRAY_TYPE(NAME, ITEM_TYPE, SIZE, DOC_STR) \
    static const ArrayType NAME = (ArrayType){.name=(#NAME), .item_type=(ITEM_TYPE), .size=(SIZE), .doc=(DOC_STR)}

#define PTR_TYPE(NAME, POINTEE_TYPE, DOC_STR) \
    static const PtrType NAME = (PtrType){.name=(#NAME), .pointee_type=(POINTEE_TYPE), .doc=(DOC_STR)}

#define FIELD(NAME, TYPE, DOC_STR) \
    (Field){.name=#NAME, .type=(TYPE), .doc=(DOC_STR)}

#define ARG(NAME, TYPE, DOC_STR) \
    (Arg){.name=#NAME, .type=(TYPE), .doc=(DOC_STR)}

#define ENUM_FIELD(NAME, VAL, STR, DOC_STR) \
    (EnumField){.name=#NAME, .value=(VAL), .str_value=(STR), .doc=(DOC_STR)}

#define ENUM(NAME, DOC_STR, ...) \
    static const Enum NAME = (Enum){ \
        .name=#NAME, .doc=(DOC_STR), \
        .fields=(EnumField[]){__VA_ARGS__}, \
        .fields_count=sizeof((EnumField[]){__VA_ARGS__})/sizeof(EnumField) \
    }

#define FUNCTION(NAME, DOC_STR, RET, ...) \
    static const Function NAME = (Function){ \
        .name=#NAME, .doc=(DOC_STR), \
        .inputs=(Arg[]){__VA_ARGS__}, \
        .inputs_count=sizeof((Arg[]){__VA_ARGS__})/sizeof(Arg), \
        .output = &(RET) \
    }

#define MODULE_RECORDS(...) \
    .records = (const Record*[]){__VA_ARGS__}, \
    .records_count = sizeof((const Record*[]){__VA_ARGS__})/sizeof(const Record*)

#define MODULE_VARIANTS(...) \
    .variants = (const Variant*[]){__VA_ARGS__}, \
    .variants_count = sizeof((const Variant*[]){__VA_ARGS__})/sizeof(const Variant*)

#define MODULE_ARRAYS(...) \
    .arrays = (const ArrayType*[]){__VA_ARGS__}, \
    .arrays_count = sizeof((const ArrayType*[]){__VA_ARGS__})/sizeof(const ArrayType*)

#define MODULE_PTRS(...) \
    .ptrs = (const PtrType*[]){__VA_ARGS__}, \
    .ptrs_count = sizeof((const PtrType*[]){__VA_ARGS__})/sizeof(const PtrType*)

#define MODULE_FUNCTIONS(...) \
    .functions = (const Function*[]){__VA_ARGS__}, \
    .functions_count = sizeof((const Function*[]){__VA_ARGS__})/sizeof(const Function*)

#define MODULE_ENUMS(...) \
    .enums = (const Enum*[]){__VA_ARGS__}, \
    .enums_count = sizeof((const Enum*[]){__VA_ARGS__})/sizeof(const Enum*)

#define MODULE(NAME, DOC_STR, ...) \
    static const Module NAME = (Module){ \
        .name=#NAME, .doc=(DOC_STR), \
        __VA_ARGS__ \
    }

// functions
int enum_str_to_int(const Enum* en, const char* str);
const char* enum_int_to_str(const Enum* en, int value);
void pretty_print_module(const Module* mod);
void generate_c_header(const Module* mod);
void generate_lua_bindings(const Module* mod);

#endif // APIC_H
