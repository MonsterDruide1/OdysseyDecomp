#ifndef _NN_FEATURES_H
#define _NN_FEATURES_H
/* libc++ (and or libc++abi) checks for GLIBC
 *
 * for now pretend MUSL is GLIBC 2.17
 * Perhaps someday MUSL will have version information
 * and libc++ will check for it
 *
 * rename original MUSL header to features_musl.h
 * and include it here
 */
#include <features_musl.h>
#ifndef __GLIBC_PREREQ
#define __MUSL_GLIBC__ 2
#define __MUSL_GLIBC_MINOR__ 17
#define __GLIBC_PREREQ(maj, min) ((__MUSL_GLIBC__ << 16) + __MUSL_GLIBC_MINOR__ >= ((maj) << 16) + (min))
#endif
#define __MUSL__ 0x10110
#define __NNMUSL__ 1
#endif


