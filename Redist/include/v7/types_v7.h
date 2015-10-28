#ifndef __TYPES_V7__
#define __TYPES_V7__

#if defined(_WIN32) && defined(_MSC_VER)

#	define ALIGN_PREFIX(bytes) __declspec(align(bytes))
#	define ALIGN_POSTFIX(bytes)

#	define FUNC_DEF_INLINE __inline
#	define FUNC_DEF_EXTERN_INLINE extern __inline

#elif defined(__GNUC__)

#	define ALIGN_PREFIX(bytes)
#	define ALIGN_POSTFIX(bytes) __attribute__ ((aligned(bytes)))

#	if defined (_DEBUG)
#		define FUNC_DEF_EXTERN_INLINE extern __inline
#		define FUNC_DEF_INLINE static __inline
#	else
#		define FUNC_DEF_EXTERN_INLINE extern __inline
#		define FUNC_DEF_INLINE static __inline
#	endif

#else

#	define ALIGN_PREFIX(bytes)
#	define ALIGN_POSTFIX(bytes)
#	error UNKNOWN COMPILER AND OS

#endif

#define ALIGN16_PREFIX ALIGN_PREFIX(16)
#define ALIGN16_POSTFIX ALIGN_POSTFIX(16)

#define FUNC_CALL_TYPE __fastcall
#define FUNC_DEF_EXTERN extern

typedef signed long long int64;
typedef signed int int32;
typedef signed short int16;
typedef signed char int8;

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

typedef float float4[4];
typedef int32 int32_4[4];
typedef float float44[4][4];

typedef float ALIGN16_PREFIX vec4[4] ALIGN16_POSTFIX;
typedef int32 ALIGN16_PREFIX ivec4[4] ALIGN16_POSTFIX;
typedef float ALIGN16_PREFIX mtx[4][4] ALIGN16_POSTFIX;
typedef float ALIGN16_PREFIX quat[4] ALIGN16_POSTFIX;

typedef union _intf {
	float f;
	int32 i;
} intf;



#endif//__TYPES__