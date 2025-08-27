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
#define FE_0(WHAT, DELIM)
#define FE_1(WHAT, DELIM, X0, X1) WHAT(X0, X1)
#define FE_2(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_1(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_3(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_2(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_4(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_3(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_5(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_4(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_6(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_5(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_7(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_6(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_8(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_7(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_9(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_8(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_10(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_9(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_11(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_10(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_12(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_11(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_13(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_12(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_14(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_13(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_15(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_14(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_16(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_15(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_17(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_16(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_18(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_17(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_19(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_18(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_20(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_19(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_21(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_20(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_22(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_21(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_23(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_22(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_24(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_23(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_25(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_24(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_26(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_25(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_27(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_26(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_28(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_27(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_29(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_28(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_30(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_29(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_31(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_30(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_32(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_31(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_33(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_32(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_34(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_33(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_35(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_34(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_36(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_35(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_37(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_36(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_38(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_37(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_39(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_38(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_40(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_39(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_41(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_40(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_42(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_41(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_43(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_42(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_44(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_43(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_45(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_44(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_46(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_45(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_47(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_46(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_48(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_47(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_49(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_48(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_50(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_49(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_51(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_50(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_52(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_51(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_53(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_52(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_54(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_53(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_55(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_54(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_56(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_55(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_57(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_56(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_58(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_57(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_59(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_58(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_60(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_59(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_61(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_60(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_62(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_61(WHAT, DELIM, X0, __VA_ARGS__)
#define FE_63(WHAT, DELIM, X0, X1, ...) WHAT(X0, X1) DELIM() FE_62(WHAT, DELIM, X0, __VA_ARGS__)

#define FOR_EACH_DELIM(action, delim, x0, ...)                                                     \
    VFUNC(FE_, __VA_ARGS__)(action, delim, x0, __VA_ARGS__)

#define NO_DELIM()
#define FOR_EACH(action, x0, ...) VFUNC(FE_, __VA_ARGS__)(action, NO_DELIM, x0, __VA_ARGS__)

#define _FUNC_(name, n) name n
#define FOR_EACH_TUPL(action, ...) FOR_EACH(_FUNC_, action, __VA_ARGS__)
#define FOR_EACH_TUPL_DELIM(action, delim, ...) FOR_EACH_DELIM(_FUNC_, delim, action, __VA_ARGS__)

#define COMMA() ,
#define LOGICAL_OR() ||

#define DECL_MEMBER_VAR(type, name) type m##name;

#define PARAM(type, name) type p##name

#define DECL_GET(type, name)                                                                       \
    type get##name() const {                                                                       \
        return m##name;                                                                            \
    }

#define POINTER_PARAM(type, name) PARAM(type*, name)

#define PARAM_START_COMMA(type, name) , type p##name

#define CALL_PARAM(_, name) p##name

#define SET_MEMBER_PARAM(_, name) m##name = p##name;

#define DECL_MEMBER_VAR_MULTI(...) FOR_EACH_TUPL(DECL_MEMBER_VAR, __VA_ARGS__)
#define DECL_GET_MULTI(...) FOR_EACH_TUPL(DECL_GET, __VA_ARGS__)

#define PARAM_LIST(...) FOR_EACH_TUPL_DELIM(PARAM, COMMA, __VA_ARGS__)
#define POINTER_PARAM_LIST(...) FOR_EACH_TUPL_DELIM(POINTER_PARAM, COMMA, __VA_ARGS__)
#define CALL_PARAM_LIST(...) FOR_EACH_TUPL_DELIM(CALL_PARAM, COMMA, __VA_ARGS__)

#define SET_MEMBER_PARAM_MULTI(...) FOR_EACH_TUPL(SET_MEMBER_PARAM, __VA_ARGS__)
