
/* Configuration header file.
   Modify as needed
*/

#ifndef _TargConfig_h
#define _TargConfig_h

#include <com/tholusi/esp8266/cplusplus/includes.hpp>
//#include <com/espressif/esp8266/include/c_types.h>
//#include <com/espressif/esp8266/include/mem.h>
//#include <com/tholusi/esp8266/prototypes/osapi.h>

/* Define one of B_LITTLE_ENDIAN or B_BIG_ENDIAN */
#define B_LITTLE_ENDIAN

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* Must return 32-bit aligned address */
#define baMalloc(s)        os_malloc(s)
#define baRealloc(m, s)    NULL //realloc(m, s)  /* as above */
#define baFree(m)          os_free(m)

#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#include <winsock2.h>
#define millisleep(milliseconds) Sleep(milliseconds)
#else
#ifndef millisleep
#define millisleep(ms) do {                   \
        os_delay_us(ms * 1000);   \
   } while(0)
#endif
#endif


#ifndef INTEGRAL_TYPES
#define INTEGRAL_TYPES
//#if (__STDC_VERSION__ >= 199901L) || defined( __GNUC__)
//#include <stdint.h>
typedef uint8_t            U8;
typedef int8_t             S8;
typedef uint16_t           U16;
typedef int16_t            S16;
typedef uint32_t           U32;
typedef int32_t            S32;
typedef uint64_t           U64;
typedef sint64_t           S64;

typedef unsigned int     uintptr_t;
//#else
//typedef unsigned char      U8;
//typedef signed   char      S8;
//typedef unsigned short     U16;
//typedef signed   short     S16;
//typedef unsigned long      U32;
//typedef signed   long      S32;
//typedef unsigned long long U64;
//typedef signed   long long S64;
//#endif
#endif

typedef U8 BaBool;


#define BA_API

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <ctype.h>

//#ifndef NDEBUG
//#include <assert.h>
//#define baAssert assert
//#else
    //#define baAssert(x)
//#endif

/* This prints an "Assertion failed" message and aborts.  */
void __assert_fail (__const char *__assertion, __const char *__file, unsigned int __line, __const char *__function);

#define __assert_fail(__assertion, __file, __line, __function)  os_printf("%s:%d: %s: Assertion `%s' failed.\n", __file, __line, (__function), (__assertion)); while(true) {}

#define __STRING(x)	#x

#if defined __cplusplus
# define __ASSERT_VOID_CAST static_cast<void>
#else
# define __ASSERT_VOID_CAST (void)
#endif

//# if defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
//#   define __ASSERT_FUNCTION	__PRETTY_FUNCTION__
//# else
//#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#   define __ASSERT_FUNCTION	__func__
//#  else
//#   define __ASSERT_FUNCTION	((__const char *) 0)
//#  endif
//# endif
#define baAssert(expr)

# define xbaAssert(expr)							\
  ((expr)								\
   ? __ASSERT_VOID_CAST (0)						\
   : __assert_fail (__STRING(expr), __FILE__, __LINE__, __ASSERT_FUNCTION))

# ifdef	__USE_GNU
#  define assert_perror(errnum)						\
  (!(errnum)								\
   ? __ASSERT_VOID_CAST (0)						\
   : __assert_perror_fail ((errnum), __FILE__, __LINE__, __ASSERT_FUNCTION))
# endif

#endif  /* _TargConfig_h */
