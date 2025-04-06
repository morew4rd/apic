#ifndef APIC_DEF_H
#define APIC_DEF_H

#ifdef APIC_REFLECT
/* ----------------- Reflection Mode ---------------- */
typedef struct Field { const char *name, *type, *doc; } Field;
typedef struct Var { const char *kind; const char *name, *type, *doc; } Var;
typedef struct Typedef { const char *kind; const char *name, *type, *doc; } Typedef;
typedef struct Struct { const char *kind; const char *name, *doc; Field *fields; int count; } Struct;
typedef struct Union { const char *kind; const char *name, *doc; Field *fields; int count; } Union;
typedef struct Arg { const char *name, *type, *doc; } Arg;
typedef struct Func { const char *kind; const char *name, *doc; const char *ret; Arg *args; int count; } Func;
typedef struct Lambda { const char *kind; const char *name, *doc; const char *ret; Arg *args; int count; } Lambda;
typedef struct EnumEntry { const char *name; int value; const char *str, *doc; } EnumEntry;
typedef struct Enum { const char *name, *doc; EnumEntry *entries; int count; } Enum;
typedef struct Exports {
    const char *name, *doc;
    Struct **structs;
    Union **unions;
    Func **funcs;
    Lambda **lambdas;
    Enum **enums;
    Var **vars;
    Typedef **typedefs;
    int struct_count;
    int union_count;
    int func_count;
    int lambda_count;
    int enum_count;
    int var_count;
    int typedef_count;
} Exports;


/* ----------------- Field Macros ----------------- */
#define F_(n,t,d) {#n, #t, d}
#define F(n,t) F_(n,t, "")
#define FA_(n,t,c,d) {#n, #t "[" #c "]", d}
#define FA(n,t,c) FA_(n,t,c, "")

/* ---------------- Variable Macros ---------------- */
#define VAR_(n,t,d) static Var n = {"var", #n, #t, d}
#define VAR(n,t) VAR_(n,t, "")

/* ---------------- Typedef Macros ---------------- */
#define ALIAS_(n,t,d) static Typedef n = {"typedef", #n, #t, d}
#define ALIAS(n,t) ALIAS_(n,t, "")

/* ---------------- Argument Macros --------------- */
#define A_(n,t,d) {#n, #t, d}
#define A(n,t) A_(n,t, "")

/* ---------------- Enum Value Macros -------------- */
#define N_(n,v,s,d) {#n, v, s, d}
#define N(n,v,s) N_(n,v,s, "")

/* --------------- Pointer Helpers --------------- */
#define PTR(TYPE) TYPE*

/* ------------------ Struct Macros ----------------- */
#define STRUCT_(name, docstr, ...) \
    static Field name##_fields[] = {__VA_ARGS__}; \
    Struct name = {"struct", #name, docstr, name##_fields, sizeof(name##_fields)/sizeof(Field)}
#define STRUCT(name, ...) STRUCT_(name, "", __VA_ARGS__)

/* ------------------ Union Macros ------------------ */
#define UNION_(name, docstr, ...) \
    static Field name##_fields[] = {__VA_ARGS__}; \
    Union name = {"union", #name, docstr, name##_fields, sizeof(name##_fields)/sizeof(Field)}
#define UNION(name, ...) UNION_(name, "", __VA_ARGS__)

/* ---------------- Function Macros ---------------- */
#define FUNC_(name, docstr, ret, ...) \
    static Arg name##_args[] = {__VA_ARGS__}; \
    Func name = {"function", #name, docstr, #ret, name##_args, sizeof(name##_args)/sizeof(Arg)}
#define FUNC(name, ret, ...) FUNC_(name, "", ret, __VA_ARGS__)

/* ----------------- Lambda Macros ----------------- */
#define LAMBDA_(name, docstr, ret, ...) \
    static Arg name##_args[] = {__VA_ARGS__}; \
    Lambda name = {"lambda", #name, docstr, #ret, name##_args, sizeof(name##_args)/sizeof(Arg)}
#define LAMBDA(name, ret, ...) LAMBDA_(name, "", ret, __VA_ARGS__)

/* ------------------ Enum Macros ------------------ */
#define ENUM_(name, docstr, ...) \
    static EnumEntry name##_entries[] = {__VA_ARGS__}; \
    Enum name = {#name, docstr, name##_entries, sizeof(name##_entries)/sizeof(EnumEntry)}
#define ENUM(name, ...) ENUM_(name, "", __VA_ARGS__)

/* --------------- Exports Macros --------------- */
#define STRUCTS(...) .structs = (Struct*[]){__VA_ARGS__}, .struct_count = sizeof((Struct*[]){__VA_ARGS__})/sizeof(Struct*)
#define UNIONS(...) .unions = (Union*[]){__VA_ARGS__}, .union_count = sizeof((Union*[]){__VA_ARGS__})/sizeof(Union*)
#define FUNCS(...) .funcs = (Func*[]){__VA_ARGS__}, .func_count = sizeof((Func*[]){__VA_ARGS__})/sizeof(Func*)
#define LAMBDAS(...) .lambdas = (Lambda*[]){__VA_ARGS__}, .lambda_count = sizeof((Lambda*[]){__VA_ARGS__})/sizeof(Lambda*)
#define ENUMS(...) .enums = (Enum*[]){__VA_ARGS__}, .enum_count = sizeof((Enum*[]){__VA_ARGS__})/sizeof(Enum*)
#define VARS(...) .vars = (Var*[]){__VA_ARGS__}, .var_count = sizeof((Var*[]){__VA_ARGS__})/sizeof(Var*)
#define TYPEDEFS(...) .typedefs = (Typedef*[]){__VA_ARGS__}, .typedef_count = sizeof((Typedef*[]){__VA_ARGS__})/sizeof(Typedef*)

#define EXPORTS_(n, d, ...) Exports n = {.name = #n, .doc = d, __VA_ARGS__}
#define EXPORTS(n, ...) EXPORTS_(n, "", __VA_ARGS__)

#else // APIC_REFLECT
/* ----------------- Header Mode ---------------- */
// Argument counting (supports up to 32 arguments)
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__,\
    32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,\
    15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
    #define COUNT_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,\
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,\
    _25,_26,_27,_28,_29,_30,_31,_32,N,...) N

    // Helper for macro expansion
    #define CONCAT(a,b) a##b
    #define CONCAT_FIELDS_IMPL(N, ...) CONCAT(CONCAT_FIELDS_, N)(__VA_ARGS__)
    #define CONCAT_FIELDS(...) CONCAT_FIELDS_IMPL(COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

    // Field concatenators (up to 32 fields)
    #define CONCAT_FIELDS_0()
    #define CONCAT_FIELDS_1(a) a
    #define CONCAT_FIELDS_2(a,b) a b
    #define CONCAT_FIELDS_3(a,b,c) a b c
    #define CONCAT_FIELDS_4(a,b,c,d) a b c d
    #define CONCAT_FIELDS_5(a,b,c,d,e) a b c d e
    #define CONCAT_FIELDS_6(a,b,c,d,e,f) a b c d e f
    #define CONCAT_FIELDS_7(a,b,c,d,e,f,g) a b c d e f g
    #define CONCAT_FIELDS_8(a,b,c,d,e,f,g,h) a b c d e f g h
    #define CONCAT_FIELDS_9(a,b,c,d,e,f,g,h,i) a b c d e f g h i
    #define CONCAT_FIELDS_10(a,b,c,d,e,f,g,h,i,j) a b c d e f g h i j
    #define CONCAT_FIELDS_11(a,b,c,d,e,f,g,h,i,j,k) a b c d e f g h i j k
    #define CONCAT_FIELDS_12(a,b,c,d,e,f,g,h,i,j,k,l) a b c d e f g h i j k l
    #define CONCAT_FIELDS_13(a,b,c,d,e,f,g,h,i,j,k,l,m) a b c d e f g h i j k l m
    #define CONCAT_FIELDS_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) a b c d e f g h i j k l m n
    #define CONCAT_FIELDS_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) a b c d e f g h i j k l m n o
    #define CONCAT_FIELDS_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) a b c d e f g h i j k l m n o p
    #define CONCAT_FIELDS_17(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q) a b c d e f g h i j k l m n o p q
    #define CONCAT_FIELDS_18(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r) a b c d e f g h i j k l m n o p q r
    #define CONCAT_FIELDS_19(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s) a b c d e f g h i j k l m n o p q r s
    #define CONCAT_FIELDS_20(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t) a b c d e f g h i j k l m n o p q r s t
    #define CONCAT_FIELDS_21(a,...) a CONCAT_FIELDS_20(__VA_ARGS__)
    #define CONCAT_FIELDS_22(a,...) a CONCAT_FIELDS_21(__VA_ARGS__)
    #define CONCAT_FIELDS_23(a,...) a CONCAT_FIELDS_22(__VA_ARGS__)
    #define CONCAT_FIELDS_24(a,...) a CONCAT_FIELDS_23(__VA_ARGS__)
    #define CONCAT_FIELDS_25(a,...) a CONCAT_FIELDS_24(__VA_ARGS__)
    #define CONCAT_FIELDS_26(a,...) a CONCAT_FIELDS_25(__VA_ARGS__)
    #define CONCAT_FIELDS_27(a,...) a CONCAT_FIELDS_26(__VA_ARGS__)
    #define CONCAT_FIELDS_28(a,...) a CONCAT_FIELDS_27(__VA_ARGS__)
    #define CONCAT_FIELDS_29(a,...) a CONCAT_FIELDS_28(__VA_ARGS__)
    #define CONCAT_FIELDS_30(a,...) a CONCAT_FIELDS_29(__VA_ARGS__)
    #define CONCAT_FIELDS_31(a,...) a CONCAT_FIELDS_30(__VA_ARGS__)
    #define CONCAT_FIELDS_32(a,...) a CONCAT_FIELDS_31(__VA_ARGS__)

// Definition macros
#define F_(name, type, doc) type name;
#define F(name, type) F_(name, type, "")
#define FA_(name, type, count, doc) type name[count];
#define FA(name, type, count) FA_(name, type, count, "")
#define VAR_(name, type, doc) type name;
#define VAR(name, type) VAR_(name, type, "")
#define ALIAS_(name, type, doc) typedef type name;
#define ALIAS(name, type) ALIAS_(name, type, "")
#define A_(name, type, doc) type name
#define A(name, type) A_(name, type, "")
#define N_(n,v,s,doc) n = v
#define N(n,v,s) N_(n,v,s,"")

/* --------------- Pointer Helpers --------------- */
#define PTR(TYPE) TYPE*

/* ------------------ Struct Macros ----------------- */
#define STRUCT_(name, docstr, ...) \
    typedef struct name { CONCAT_FIELDS(__VA_ARGS__) } name
#define STRUCT(name, ...) STRUCT_(name, "", __VA_ARGS__)

/* ------------------ Union Macros ------------------ */
#define UNION_(name, docstr, ...) \
    typedef union name { CONCAT_FIELDS(__VA_ARGS__) } name
#define UNION(name, ...) UNION_(name, "", __VA_ARGS__)

/* ---------------- Function Macros ---------------- */
#define FUNC_(name, docstr, ret, ...) ret name(__VA_ARGS__)
#define FUNC(name, ret, ...) FUNC_(name, "", ret, __VA_ARGS__)

/* ----------------- Lambda Macros ----------------- */
#define LAMBDA_(name, docstr, ret, ...) typedef ret (*name)(__VA_ARGS__)
#define LAMBDA(name, ret, ...) LAMBDA_(name, "", ret, __VA_ARGS__)

/* ------------------ Enum Macros ------------------ */
#define ENUM_(name, docstr, ...) typedef enum name { __VA_ARGS__ } name
#define ENUM(name, ...) ENUM_(name, "", __VA_ARGS__)

/* --------------- Exports Macros --------------- */
#define EXPORTS_(...)
#define EXPORTS(...)

#define STRUCTS(...)
#define UNIONS(...)
#define FUNCS(...)
#define LAMBDAS(...)
#define ENUMS(...)
#define VARS(...)
#define TYPEDEFS(...)

#endif // APIC_REFLECT

#endif // APIC_DEF_H
