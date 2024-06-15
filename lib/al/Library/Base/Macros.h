#pragma once

// https://stackoverflow.com/a/26408195/9275661
#define __NARG__(...) __NARG_I_(__VA_ARGS__, __RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18,   \
                _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34,    \
                _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,    \
                _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, N, ...)           \
    N
#define __RSEQ_N()                                                                                 \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41,    \
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,    \
        18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__))

// https://stackoverflow.com/a/11994395/9275661
// adjusted to keep the first parameter the same for all "recursive" calls,
// used to pass the class name to NERVE_MAKE macros
#define FE_0(WHAT)
#define FE_1(WHAT, X0, X1) WHAT(X0, X1)
#define FE_2(WHAT, X0, X1, ...) WHAT(X0, X1) FE_1(WHAT, X0, __VA_ARGS__)
#define FE_3(WHAT, X0, X1, ...) WHAT(X0, X1) FE_2(WHAT, X0, __VA_ARGS__)
#define FE_4(WHAT, X0, X1, ...) WHAT(X0, X1) FE_3(WHAT, X0, __VA_ARGS__)
#define FE_5(WHAT, X0, X1, ...) WHAT(X0, X1) FE_4(WHAT, X0, __VA_ARGS__)
#define FE_6(WHAT, X0, X1, ...) WHAT(X0, X1) FE_5(WHAT, X0, __VA_ARGS__)
#define FE_7(WHAT, X0, X1, ...) WHAT(X0, X1) FE_6(WHAT, X0, __VA_ARGS__)
#define FE_8(WHAT, X0, X1, ...) WHAT(X0, X1) FE_7(WHAT, X0, __VA_ARGS__)
#define FE_9(WHAT, X0, X1, ...) WHAT(X0, X1) FE_8(WHAT, X0, __VA_ARGS__)
#define FE_10(WHAT, X0, X1, ...) WHAT(X0, X1) FE_9(WHAT, X0, __VA_ARGS__)
#define FE_11(WHAT, X0, X1, ...) WHAT(X0, X1) FE_10(WHAT, X0, __VA_ARGS__)
#define FE_12(WHAT, X0, X1, ...) WHAT(X0, X1) FE_11(WHAT, X0, __VA_ARGS__)
#define FE_13(WHAT, X0, X1, ...) WHAT(X0, X1) FE_12(WHAT, X0, __VA_ARGS__)
#define FE_14(WHAT, X0, X1, ...) WHAT(X0, X1) FE_13(WHAT, X0, __VA_ARGS__)
#define FE_15(WHAT, X0, X1, ...) WHAT(X0, X1) FE_14(WHAT, X0, __VA_ARGS__)
#define FE_16(WHAT, X0, X1, ...) WHAT(X0, X1) FE_15(WHAT, X0, __VA_ARGS__)
#define FE_17(WHAT, X0, X1, ...) WHAT(X0, X1) FE_16(WHAT, X0, __VA_ARGS__)
#define FE_18(WHAT, X0, X1, ...) WHAT(X0, X1) FE_17(WHAT, X0, __VA_ARGS__)
#define FE_19(WHAT, X0, X1, ...) WHAT(X0, X1) FE_18(WHAT, X0, __VA_ARGS__)
#define FE_20(WHAT, X0, X1, ...) WHAT(X0, X1) FE_19(WHAT, X0, __VA_ARGS__)
#define FE_21(WHAT, X0, X1, ...) WHAT(X0, X1) FE_20(WHAT, X0, __VA_ARGS__)
#define FE_22(WHAT, X0, X1, ...) WHAT(X0, X1) FE_21(WHAT, X0, __VA_ARGS__)
#define FE_23(WHAT, X0, X1, ...) WHAT(X0, X1) FE_22(WHAT, X0, __VA_ARGS__)
#define FE_24(WHAT, X0, X1, ...) WHAT(X0, X1) FE_23(WHAT, X0, __VA_ARGS__)
#define FE_25(WHAT, X0, X1, ...) WHAT(X0, X1) FE_24(WHAT, X0, __VA_ARGS__)
#define FE_26(WHAT, X0, X1, ...) WHAT(X0, X1) FE_25(WHAT, X0, __VA_ARGS__)
#define FE_27(WHAT, X0, X1, ...) WHAT(X0, X1) FE_26(WHAT, X0, __VA_ARGS__)
#define FE_28(WHAT, X0, X1, ...) WHAT(X0, X1) FE_27(WHAT, X0, __VA_ARGS__)
#define FE_29(WHAT, X0, X1, ...) WHAT(X0, X1) FE_28(WHAT, X0, __VA_ARGS__)
#define FE_30(WHAT, X0, X1, ...) WHAT(X0, X1) FE_29(WHAT, X0, __VA_ARGS__)
#define FE_31(WHAT, X0, X1, ...) WHAT(X0, X1) FE_30(WHAT, X0, __VA_ARGS__)
#define FE_32(WHAT, X0, X1, ...) WHAT(X0, X1) FE_31(WHAT, X0, __VA_ARGS__)
#define FE_33(WHAT, X0, X1, ...) WHAT(X0, X1) FE_32(WHAT, X0, __VA_ARGS__)
#define FE_34(WHAT, X0, X1, ...) WHAT(X0, X1) FE_33(WHAT, X0, __VA_ARGS__)
#define FE_35(WHAT, X0, X1, ...) WHAT(X0, X1) FE_34(WHAT, X0, __VA_ARGS__)
#define FE_36(WHAT, X0, X1, ...) WHAT(X0, X1) FE_35(WHAT, X0, __VA_ARGS__)
#define FE_37(WHAT, X0, X1, ...) WHAT(X0, X1) FE_36(WHAT, X0, __VA_ARGS__)
#define FE_38(WHAT, X0, X1, ...) WHAT(X0, X1) FE_37(WHAT, X0, __VA_ARGS__)
#define FE_39(WHAT, X0, X1, ...) WHAT(X0, X1) FE_38(WHAT, X0, __VA_ARGS__)
#define FE_40(WHAT, X0, X1, ...) WHAT(X0, X1) FE_39(WHAT, X0, __VA_ARGS__)
#define FE_41(WHAT, X0, X1, ...) WHAT(X0, X1) FE_40(WHAT, X0, __VA_ARGS__)
#define FE_42(WHAT, X0, X1, ...) WHAT(X0, X1) FE_41(WHAT, X0, __VA_ARGS__)
#define FE_43(WHAT, X0, X1, ...) WHAT(X0, X1) FE_42(WHAT, X0, __VA_ARGS__)
#define FE_44(WHAT, X0, X1, ...) WHAT(X0, X1) FE_43(WHAT, X0, __VA_ARGS__)
#define FE_45(WHAT, X0, X1, ...) WHAT(X0, X1) FE_44(WHAT, X0, __VA_ARGS__)
#define FE_46(WHAT, X0, X1, ...) WHAT(X0, X1) FE_45(WHAT, X0, __VA_ARGS__)
#define FE_47(WHAT, X0, X1, ...) WHAT(X0, X1) FE_46(WHAT, X0, __VA_ARGS__)
#define FE_48(WHAT, X0, X1, ...) WHAT(X0, X1) FE_47(WHAT, X0, __VA_ARGS__)
#define FE_49(WHAT, X0, X1, ...) WHAT(X0, X1) FE_48(WHAT, X0, __VA_ARGS__)
#define FE_50(WHAT, X0, X1, ...) WHAT(X0, X1) FE_49(WHAT, X0, __VA_ARGS__)
#define FE_51(WHAT, X0, X1, ...) WHAT(X0, X1) FE_50(WHAT, X0, __VA_ARGS__)
#define FE_52(WHAT, X0, X1, ...) WHAT(X0, X1) FE_51(WHAT, X0, __VA_ARGS__)
#define FE_53(WHAT, X0, X1, ...) WHAT(X0, X1) FE_52(WHAT, X0, __VA_ARGS__)
#define FE_54(WHAT, X0, X1, ...) WHAT(X0, X1) FE_53(WHAT, X0, __VA_ARGS__)
#define FE_55(WHAT, X0, X1, ...) WHAT(X0, X1) FE_54(WHAT, X0, __VA_ARGS__)
#define FE_56(WHAT, X0, X1, ...) WHAT(X0, X1) FE_55(WHAT, X0, __VA_ARGS__)
#define FE_57(WHAT, X0, X1, ...) WHAT(X0, X1) FE_56(WHAT, X0, __VA_ARGS__)
#define FE_58(WHAT, X0, X1, ...) WHAT(X0, X1) FE_57(WHAT, X0, __VA_ARGS__)
#define FE_59(WHAT, X0, X1, ...) WHAT(X0, X1) FE_58(WHAT, X0, __VA_ARGS__)
#define FE_60(WHAT, X0, X1, ...) WHAT(X0, X1) FE_59(WHAT, X0, __VA_ARGS__)
#define FE_61(WHAT, X0, X1, ...) WHAT(X0, X1) FE_60(WHAT, X0, __VA_ARGS__)
#define FE_62(WHAT, X0, X1, ...) WHAT(X0, X1) FE_61(WHAT, X0, __VA_ARGS__)
#define FE_63(WHAT, X0, X1, ...) WHAT(X0, X1) FE_62(WHAT, X0, __VA_ARGS__)

#define FOR_EACH(action, x0, ...) VFUNC(FE_, __VA_ARGS__)(action, x0, __VA_ARGS__)
