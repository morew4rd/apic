#ifndef APIC_DEF_H
#define APIC_DEF_H

#ifndef APIC_SKIP_SHORTCUTS
#define F_       APIC_FIELD_
#define F        APIC_FIELD
#define FA       APIC_FIELD_ARRAY
#define FA_      APIC_FIELD_ARRAY_
#define A_       APIC_ARG_
#define A        APIC_ARG
#define N_       APIC_ENUMENTRY_
#define N        APIC_ENUMENTRY

#define STRUCT_  APIC_STRUCT_
#define STRUCT   APIC_STRUCT
#define UNION_   APIC_UNION_
#define UNION    APIC_UNION
#define ENUM_    APIC_ENUM_
#define ENUM     APIC_ENUM

#define FUNC_    APIC_FUNC_
#define FUNC     APIC_FUNC

#define ALIAS_   APIC_ALIAS_
#define ALIAS    APIC_ALIAS

#define PTR      APIC_PTR

#define FUNCPTR_ APIC_FUNCPTR_
#define FUNCPTR  APIC_FUNCPTR

#define STRUCTS  APIC_STRUCTS
#define UNIONS   APIC_UNIONS
#define ENUMS    APIC_ENUMS
#define FUNCS    APIC_FUNCS
#define FUNCPTRS APIC_FUNCPTRS
#define ALIASES  APIC_ALIASES

#define EXPORTS_ APIC_PUBLIC_EXPORTS_
#define EXPORTS  APIC_PUBLIC_EXPORTS

#endif // APIC_DEFINE_SHORTCUTS


#ifndef APIC_REFLECT

/* ----------------- Header Mode ---------------- */

// Definition macros
#define APIC_FIELD_(name, type, doc) type name;
#define APIC_FIELD(name, type) APIC_FIELD_(name, type, "")
#define APIC_FIELD_ARRAY_(name, type, count, doc) type name[count];
#define APIC_FIELD_ARRAY(name, type, count) APIC_FIELD_ARRAY_(name, type, count, "")
#define APIC_ALIAS_(name, type, doc) typedef type name;
#define APIC_ALIAS(name, type) APIC_ALIAS_(name, type, "")
#define APIC_ARG_(name, type, doc) type name
#define APIC_ARG(name, type) APIC_ARG_(name, type, "")
#define APIC_ENUMENTRY_(n,v,s,doc) n = v
#define APIC_ENUMENTRY(n,v,s) APIC_ENUMENTRY_(n,v,s,"")

/* --------------- Pointer Helpers --------------- */
#define APIC_PTR(TYPE) TYPE*

/* ------------------ apic_Struct Macros ----------------- */
#define APIC_STRUCT_(name, docstr, ...) \
    typedef struct name { _APIC_CONCAT_FIELDS(__VA_ARGS__) } name
#define APIC_STRUCT(name, ...) APIC_STRUCT_(name, "", __VA_ARGS__)

/* ------------------ apic_Union Macros ------------------ */
#define APIC_UNION_(name, docstr, ...) \
    typedef union name { _APIC_CONCAT_FIELDS(__VA_ARGS__) } name
#define APIC_UNION(name, ...) APIC_UNION_(name, "", __VA_ARGS__)

/* ---------------- apic_Function Macros ---------------- */
#define APIC_FUNC_(name, docstr, ret, ...) ret name(__VA_ARGS__)
#define APIC_FUNC(name, ret, ...) APIC_FUNC_(name, "", ret, __VA_ARGS__)

/* ----------------- apic_FuncPtr Macros ----------------- */
#define APIC_FUNCPTR_(name, docstr, ret, ...) typedef ret (*name)(__VA_ARGS__)
#define APIC_FUNCPTR(name, ret, ...) APIC_FUNCPTR_(name, "", ret, __VA_ARGS__)

/* ------------------ apic_Enum Macros ------------------ */
#define APIC_ENUM_(name, docstr, ...) typedef enum name { __VA_ARGS__ } name
#define APIC_ENUM(name, ...) APIC_ENUM_(name, "", __VA_ARGS__)

/* --------------- apic_Exports Macros --------------- */
#define APIC_PUBLIC_EXPORTS_(...)
#define APIC_PUBLIC_EXPORTS(...)

#define APIC_STRUCTS(...)
#define APIC_UNIONS(...)
#define APIC_FUNCS(...)
#define APIC_FUNCPTRS(...)
#define APIC_ENUMS(...)
#define APIC_ALIASES(...)


// apic_Argument counting (supports up to 32 arguments)
#define _APIC_COUNT_ARGS(...) _APIC_COUNT_ARGS_IMPL(__VA_ARGS__,\
    32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,\
    15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
    #define _APIC_COUNT_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,\
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,\
    _25,_26,_27,_28,_29,_30,_31,_32,N,...) N

    // Helper for macro expansion
    #define _APIC_CONCAT(a,b) a##b
    #define _APIC_CONCAT_FIELDS_IMPL(N, ...) _APIC_CONCAT(_APIC_CONCAT_FIELDS_, N)(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS(...) _APIC_CONCAT_FIELDS_IMPL(_APIC_COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

    // Field concatenators (up to 32 fields)
    #define _APIC_CONCAT_FIELDS_0()
    #define _APIC_CONCAT_FIELDS_1(a) a
    #define _APIC_CONCAT_FIELDS_2(a,b) a b
    #define _APIC_CONCAT_FIELDS_3(a,b,c) a b c
    #define _APIC_CONCAT_FIELDS_4(a,b,c,d) a b c d
    #define _APIC_CONCAT_FIELDS_5(a,b,c,d,e) a b c d e
    #define _APIC_CONCAT_FIELDS_6(a,b,c,d,e,f) a b c d e f
    #define _APIC_CONCAT_FIELDS_7(a,b,c,d,e,f,g) a b c d e f g
    #define _APIC_CONCAT_FIELDS_8(a,b,c,d,e,f,g,h) a b c d e f g h
    #define _APIC_CONCAT_FIELDS_9(a,b,c,d,e,f,g,h,i) a b c d e f g h i
    #define _APIC_CONCAT_FIELDS_10(a,b,c,d,e,f,g,h,i,j) a b c d e f g h i j
    #define _APIC_CONCAT_FIELDS_11(a,b,c,d,e,f,g,h,i,j,k) a b c d e f g h i j k
    #define _APIC_CONCAT_FIELDS_12(a,b,c,d,e,f,g,h,i,j,k,l) a b c d e f g h i j k l
    #define _APIC_CONCAT_FIELDS_13(a,b,c,d,e,f,g,h,i,j,k,l,m) a b c d e f g h i j k l m
    #define _APIC_CONCAT_FIELDS_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) a b c d e f g h i j k l m n
    #define _APIC_CONCAT_FIELDS_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) a b c d e f g h i j k l m n o
    #define _APIC_CONCAT_FIELDS_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) a b c d e f g h i j k l m n o p
    #define _APIC_CONCAT_FIELDS_17(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q) a b c d e f g h i j k l m n o p q
    #define _APIC_CONCAT_FIELDS_18(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r) a b c d e f g h i j k l m n o p q r
    #define _APIC_CONCAT_FIELDS_19(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s) a b c d e f g h i j k l m n o p q r s
    #define _APIC_CONCAT_FIELDS_20(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t) a b c d e f g h i j k l m n o p q r s t
    #define _APIC_CONCAT_FIELDS_21(a,...) a _APIC_CONCAT_FIELDS_20(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_22(a,...) a _APIC_CONCAT_FIELDS_21(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_23(a,...) a _APIC_CONCAT_FIELDS_22(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_24(a,...) a _APIC_CONCAT_FIELDS_23(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_25(a,...) a _APIC_CONCAT_FIELDS_24(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_26(a,...) a _APIC_CONCAT_FIELDS_25(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_27(a,...) a _APIC_CONCAT_FIELDS_26(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_28(a,...) a _APIC_CONCAT_FIELDS_27(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_29(a,...) a _APIC_CONCAT_FIELDS_28(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_30(a,...) a _APIC_CONCAT_FIELDS_29(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_31(a,...) a _APIC_CONCAT_FIELDS_30(__VA_ARGS__)
    #define _APIC_CONCAT_FIELDS_32(a,...) a _APIC_CONCAT_FIELDS_31(__VA_ARGS__)


#else // APIC_REFLECT

/* ----------------- Reflection Mode ---------------- */
typedef struct apic_Field { const char *name, *type, *doc; } apic_Field;
typedef struct apic_Alias { const char *kind; const char *name, *type, *doc; } apic_Alias;
typedef struct apic_Struct { const char *kind; const char *name, *doc; apic_Field *fields; int count; } apic_Struct;
typedef struct apic_Union { const char *kind; const char *name, *doc; apic_Field *fields; int count; } apic_Union;
typedef struct apic_Arg { const char *name, *type, *doc; } apic_Arg;
typedef struct apic_Func { const char *kind; const char *name, *doc; const char *ret; apic_Arg *args; int count; } apic_Func;
typedef struct apic_FuncPtr { const char *kind; const char *name, *doc; const char *ret; apic_Arg *args; int count; } apic_FuncPtr;
typedef struct apic_apic_EnumEntry { const char *name; int value; const char *str, *doc; } apic_apic_EnumEntry;
typedef struct apic_Enum { const char *name, *doc; apic_apic_EnumEntry *entries; int count; } apic_Enum;
typedef struct apic_Exports {
    const char *name, *doc;
    apic_Struct **structs;
    apic_Union **unions;
    apic_Func **funcs;
    apic_FuncPtr **funcptrs;
    apic_Enum **enums;
    apic_Alias **aliases;
    int struct_count;
    int union_count;
    int func_count;
    int funcptr_count;
    int enum_count;
    int typedef_count;
} apic_Exports;


/* ----------------- apic_Field Macros ----------------- */
#define APIC_FIELD_(n,t,d) {#n, #t, d}
#define APIC_FIELD(n,t) APIC_FIELD_(n,t, "")
#define APIC_FIELD_ARRAY_(n,t,c,d) {#n, #t "[" #c "]", d}
#define APIC_FIELD_ARRAY(n,t,c) APIC_FIELD_ARRAY_(n,t,c, "")

/* ---------------- apic_Alias Macros ---------------- */
#define APIC_ALIAS_(n,t,d) static apic_Alias n = {"typedef", #n, #t, d}
#define APIC_ALIAS(n,t) APIC_ALIAS_(n,t, "")

/* ---------------- apic_Argument Macros --------------- */
#define APIC_ARG_(n,t,d) {#n, #t, d}
#define APIC_ARG(n,t) APIC_ARG_(n,t, "")

/* ---------------- apic_Enum Value Macros -------------- */
#define APIC_ENUMENTRY_(n,v,s,d) {#n, v, s, d}
#define APIC_ENUMENTRY(n,v,s) APIC_ENUMENTRY_(n,v,s, "")

/* --------------- Pointer Helpers --------------- */
#define APIC_PTR(TYPE) TYPE*

/* ------------------ apic_Struct Macros ----------------- */
#define APIC_STRUCT_(name, docstr, ...) \
    static apic_Field name##_fields[] = {__VA_ARGS__}; \
    apic_Struct name = {"struct", #name, docstr, name##_fields, sizeof(name##_fields)/sizeof(apic_Field)}
#define APIC_STRUCT(name, ...) APIC_STRUCT_(name, "", __VA_ARGS__)

/* ------------------ apic_Union Macros ------------------ */
#define APIC_UNION_(name, docstr, ...) \
    static apic_Field name##_fields[] = {__VA_ARGS__}; \
    apic_Union name = {"union", #name, docstr, name##_fields, sizeof(name##_fields)/sizeof(apic_Field)}
#define APIC_UNION(name, ...) APIC_UNION_(name, "", __VA_ARGS__)

/* ---------------- apic_Function Macros ---------------- */
#define APIC_FUNC_(name, docstr, ret, ...) \
    static apic_Arg name##_args[] = {__VA_ARGS__}; \
    apic_Func name = {"function", #name, docstr, #ret, name##_args, sizeof(name##_args)/sizeof(apic_Arg)}
#define APIC_FUNC(name, ret, ...) APIC_FUNC_(name, "", ret, __VA_ARGS__)

/* ----------------- apic_FuncPtr Macros ----------------- */
#define APIC_FUNCPTR_(name, docstr, ret, ...) \
    static apic_Arg name##_args[] = {__VA_ARGS__}; \
    apic_FuncPtr name = {"funcptr", #name, docstr, #ret, name##_args, sizeof(name##_args)/sizeof(apic_Arg)}
#define APIC_FUNCPTR(name, ret, ...) APIC_FUNCPTR_(name, "", ret, __VA_ARGS__)

/* ------------------ apic_Enum Macros ------------------ */
#define APIC_ENUM_(name, docstr, ...) \
    static apic_apic_EnumEntry name##_entries[] = {__VA_ARGS__}; \
    apic_Enum name = {#name, docstr, name##_entries, sizeof(name##_entries)/sizeof(apic_apic_EnumEntry)}
#define APIC_ENUM(name, ...) APIC_ENUM_(name, "", __VA_ARGS__)

/* --------------- apic_Exports Macros --------------- */
#define APIC_STRUCTS(...) .structs = (apic_Struct*[]){__VA_ARGS__}, .struct_count = sizeof((apic_Struct*[]){__VA_ARGS__})/sizeof(apic_Struct*)
#define APIC_UNIONS(...) .unions = (apic_Union*[]){__VA_ARGS__}, .union_count = sizeof((apic_Union*[]){__VA_ARGS__})/sizeof(apic_Union*)
#define APIC_FUNCS(...) .funcs = (apic_Func*[]){__VA_ARGS__}, .func_count = sizeof((apic_Func*[]){__VA_ARGS__})/sizeof(apic_Func*)
#define APIC_FUNCPTRS(...) .funcptrs = (apic_FuncPtr*[]){__VA_ARGS__}, .funcptr_count = sizeof((apic_FuncPtr*[]){__VA_ARGS__})/sizeof(apic_FuncPtr*)
#define APIC_ENUMS(...) .enums = (apic_Enum*[]){__VA_ARGS__}, .enum_count = sizeof((apic_Enum*[]){__VA_ARGS__})/sizeof(apic_Enum*)
#define APIC_ALIASES(...) .aliases = (apic_Alias*[]){__VA_ARGS__}, .typedef_count = sizeof((apic_Alias*[]){__VA_ARGS__})/sizeof(apic_Alias*)

#define APIC_PUBLIC_EXPORTS_(n, d, ...) apic_Exports n = {.name = #n, .doc = d, __VA_ARGS__}
#define APIC_PUBLIC_EXPORTS(n, ...) APIC_PUBLIC_EXPORTS_(n, "", __VA_ARGS__)


#ifdef APIC_REFLECT_PRETTYPRINT

#include <stdio.h>

static void apic_prettyprint(apic_Exports *exports) {
    printf("\n=============================================\n\n");
    printf("apic_Exports: %s\n\nDescription: %s\n\n", exports->name, exports->doc);

    printf("=== apic_Structs (%d) ===\n", exports->struct_count);
    for(int i = 0; i < exports->struct_count; i++) {
        apic_Struct *st = exports->structs[i];
        printf("%s: %s\napic_Fields (%d):\n", st->name, st->doc, st->count);
        for(int j = 0; j < st->count; j++) {
            apic_Field f = st->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < exports->struct_count-1) printf("\n");
    }

    printf("\n=== apic_Unions (%d) ===\n", exports->union_count);
    for(int i = 0; i < exports->union_count; i++) {
        apic_Union *u = exports->unions[i];
        printf("%s: %s\napic_Fields (%d):\n", u->name, u->doc, u->count);
        for(int j = 0; j < u->count; j++) {
            apic_Field f = u->fields[j];
            printf("  %s: %s (%s)\n", f.name, f.type, f.doc);
        }
        if(i < exports->union_count-1) printf("\n");
    }

    printf("\n=== apic_Functions (%d) ===\n", exports->func_count);
    for(int i = 0; i < exports->func_count; i++) {
        apic_Func *func = exports->funcs[i];
        printf("%s: %s\nReturns: %s\napic_Args (%d):\n",
               func->name, func->doc, func->ret, func->count);
        for(int j = 0; j < func->count; j++) {
            apic_Arg a = func->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < exports->func_count-1) printf("\n");
    }

    printf("\n=== apic_FuncPtrs (%d) ===\n", exports->funcptr_count);
    for(int i = 0; i < exports->funcptr_count; i++) {
        apic_FuncPtr *lam = exports->funcptrs[i];
        printf("%s: %s\nReturns: %s\napic_Args (%d):\n",
               lam->name, lam->doc, lam->ret, lam->count);
        for(int j = 0; j < lam->count; j++) {
            apic_Arg a = lam->args[j];
            printf("  %s: %s (%s)\n", a.name, a.type, a.doc);
        }
        if(i < exports->funcptr_count-1) printf("\n");
    }

    printf("\n=== apic_Enums (%d) ===\n", exports->enum_count);
    for(int i = 0; i < exports->enum_count; i++) {
        apic_Enum *en = exports->enums[i];
        printf("%s: %s\nEntries (%d):\n", en->name, en->doc, en->count);
        for(int j = 0; j < en->count; j++) {
            apic_apic_EnumEntry e = en->entries[j];
            printf("  %s = %d (%s, %s)\n", e.name, e.value, e.str, e.doc);
        }
        if(i < exports->enum_count-1) printf("\n");
    }


    printf("\n=== apic_Aliass (%d) ===\n", exports->typedef_count);
    for(int i = 0; i < exports->typedef_count; i++) {
        apic_Alias *t = exports->aliases[i];
        printf("%s = %s (%s)\n", t->name, t->type, t->doc);
    }
    printf("\n=============================================\n\n");
}

#endif // APIC_REFLECT_PRETTYPRINT

#endif // APIC_REFLECT

#endif // APIC_DEF_H
