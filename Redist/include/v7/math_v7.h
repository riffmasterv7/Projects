#ifndef __MATH_V7__
#define __MATH_V7_

#include "types_v7.h"

#define USE_SSE
//#define USE_FAST_CAST

#if defined(USE_SSE)
#include <xmmintrin.h>
#include <emmintrin.h>
#endif

/* Definitions of useful mathematical constants
 * M_E		  - e
 * M_LOG2E	 - log2(e)
 * M_LOG10E	- log10(e)
 * M_LN2		- ln(2)
 * M_LN10	  - ln(10)
 * M_PI		 - pi
 * M_PI_2	  - pi/2
 * M_PI_4	  - pi/4
 * M_1_PI	  - 1/pi
 * M_2_PI	  - 2/pi
 * M_2_SQRTPI - 2/sqrt(pi)
 * M_SQRT2	 - sqrt(2)
 * M_SQRT1_2  - 1/sqrt(2)
 */

#define D_E				2.7182818284590f
#define D_LOG2E			1.4426950408889f
#define D_LOG10E		0.4342944819032f
#define D_LN2			0.6931471805599f
#define D_LN10			2.3025850929940f
#define D_PI			3.1415926535898f
#define D_DOUBLE_PI		6.2831853071796f
#define D_SQUARE_PI		9.8696044010894f
#define D_PI_2			1.5707963267949f
#define D_PI_4			0.7853981633974f
#define D_1_PI			0.3183098861838f
#define D_1_DOUBLE_PI	0.1591549430919f
#define D_1_SQUARE_PI	0.1013211836423f	
#define D_2_PI			0.6366197723676f
#define D_2_SQRTPI		1.1283791670955f
#define D_SQRT2			1.4142135623731f
#define D_SQRT1_2		0.7071067811865f
#define D_1_180			0.0055555555555f

//////////////////////////////////////////////////////////////////////////

const vec4 g_VEC4_ZERO3 = { 0.0f, 0.0f, 0.0f, 1.0f };
const vec4 g_VEC4_ZERO4 = { 0.0f, 0.0f, 0.0f, 0.0f };
const vec4 g_VEC4_NEG3 =  {-1.0f,-1.0f,-1.0f, 1.0f };
const vec4 g_VEC4_NEG4 =  {-1.0f,-1.0f,-1.0f,-1.0f };
const vec4 g_VEC4_UNIT =  { 1.0f, 1.0f, 1.0f, 1.0f };
const vec4 g_VEC4_AXIS_X ={ 1.0f, 0.0f, 0.0f, 1.0f };
const vec4 g_VEC4_AXIS_Y ={ 0.0f, 1.0f, 0.0f, 1.0f };
const vec4 g_VEC4_AXIS_Z ={ 0.0f, 0.0f, 1.0f, 1.0f };

extern const mtx g_MTX_UNIT;
extern const mtx g_MTX_ZERO; 

//////////////////////////////////////////////////////////////////////////

#define D_DEG2RAD(deg) ((deg) * D_1_180 * D_PI)
#define D_RAD2DEG(rad) ((rad) * D_1_PI * 180.0f)

/// Avoids 'strict-aliasing' warnings in GCC
#define D_F_GET_SIGN(__f, __sign) { intf __intf = { __f }; __sign = (__intf.i) & 0x80000000; }
#define D_F_SET_SIGN(__f, __sign) { intf __intf = { __f }; __intf.i &= 0x7FFFFFFF; __intf.i |= (__sign); __f = __intf.f; }
#define D_F_ABS(__f) { intf __intf = {__f}; __intf.i &= 0x7FFFFFFF; __f = __intf.f; }

#if defined(USE_FAST_CAST) && defined(USE_SSE)

#	define D_F2I_CAST(f, i) { __m128 __xmm0 = _mm_load_ss(&f); i = _mm_cvtt_ss2si(__xmm0);	}
#	define D_I2F_CAST(i, f) { __m128 __xmm0; __xmm0 = _mm_cvt_si2ss(__xmm0, i); f = *(float *)&__xmm0; }
#	define D_F_FLOOR(f) \
		{ \
			__m128 __xmm0 = _mm_load_ss(&f); \
			__xmm0 = _mm_cvt_si2ss(__xmm0, _mm_cvtt_ss2si(__xmm0)); \
			f = *(float *)&__xmm0; \
		}

#elif defined(USE_FAST_CAST)

#	if defined(_WIN32) && defined(_MSC_VER)

#		define D_F2I_CAST(f, i) \
			__asm fld f \
			__asm fisttp i
#		define D_I2F_CAST(i, f) f = (float)(i);
#		define D_F_FLOOR(f) { int32 __i = (int)f; f = (float)__i; }

#	elif defined(_GNUC)

#		define	D_F2I_CAST(f, i) \
			__asm__ __volatile__ ("fisttpl %0" : "=m" (i) : "t" (f) : "st") ;
#		define D_I2F_CAST(i, f) f = (float)(i);
#		define D_F_FLOOR(f) { int32 __i = (int)f; f = (float)__i; }

#	else
#	error COMPILER OR OS NOT DEFINED
#	endif

#else
#	define D_F2I_CAST(f, i) i = (int32)f;
#	define D_I2F_CAST(i, f) f = (float)(i);
#	define D_F_FLOOR(f) { int32 __i = (int)f; f = (float)__i; }
#endif

#ifdef __cplusplus
extern "C" {
#endif
//////////////////////////////////////////////////////////////////////////
/// Vector
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecCopy(float4 to, const float4 from);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecSwap(float4 to, float4 from);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecZero(float4 v);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecZero4(float4 v);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE vecDot(float4 v1, float4 v2);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE vecDot4(float4 v1, float4 v2);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecCrossRH(float4 res, float4 v1, float4 v2);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecCrossLH(float4 res, float4 v1, float4 v2);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecAdd(float4 res, float4 v1, float4 v2);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecSub(float4 res, float4 v1, float4 v2);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecMul(float4 res, float4 v1, float4 v2);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecDiv(float4 res, float4 v1, float4 v2);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecScale(float4 res, float4 v, float f);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE vecMag(float4 v);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE vecMagSq(float4 v);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE vecMag4(float4 v);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE vecDist(float4 v1, float4 v2);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE vecDistSq(float4 v1, float4 v2);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecRcp(float4 res, float4 v);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecNeg(float4 res, float4 v);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecNeg4(float4 res, float4 v);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecNormalize(float4 res, float4 v);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE vecNormalizeEx(float4 res, float4 v);

FUNC_DEF_INLINE void FUNC_CALL_TYPE vecInit(float4 vec, float4 unaligned);
FUNC_DEF_INLINE void FUNC_CALL_TYPE vecInitVec3(float4 vec, float unaligned[3]);
FUNC_DEF_INLINE void FUNC_CALL_TYPE vecInitFloat3(float4 vec, float x, float y, float z);
FUNC_DEF_INLINE float FUNC_CALL_TYPE vecMax(float4 vec);
FUNC_DEF_INLINE float FUNC_CALL_TYPE vecMax4(float4 vec);
FUNC_DEF_INLINE float FUNC_CALL_TYPE vecMin(float4 vec);
FUNC_DEF_INLINE float FUNC_CALL_TYPE vecMin4(float4 vec);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE vecAbs(float4 res, float4 v);

//////////////////////////////////////////////////////////////////////////
/// Matrix
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxUnit(float44 m);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxZero(float44 m);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxCopy(float44 dst, float44 src);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxAdd(float44 res, float44 a, float44 b);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxSub(float44 res, float44 a, float44 b);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMul(float44 res, float44 a, float44 b);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxApply(float4 dst, float44 m, float4 src);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxTranspose(float44 res, float44 m);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxInvertFull(float44 res, float44 m);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxInvertFast(float44 res, float44 m);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeScaling(float44 m, float4 scale);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeTranslation(float44 m, float4 translate);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeRotationX(float44 m, float angleRad);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeRotationY(float44 m, float angleRad);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeRotationZ(float44 m, float angleRad);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeRotationAxis(float44 m, float4 normalizedAxis, float angleRad);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakePerspectiveLH(float44 m, float vertFOVRad, float aspect, float zn, float zf);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakePerspectiveRH(float44 m, float vertFOVRad, float aspect, float zn, float zf);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeLookAtLH(float44 m, float4 eye, float4 target, float4 up);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeLookAtRH(float44 m, float4 eye, float4 target, float4 up);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeOrthoLH(float44 m, float w, float h, float zn, float zf);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE mtxMakeOrthoRH(float44 m, float w, float h, float zn, float zf);

//////////////////////////////////////////////////////////////////////////
/// Quaternion
FUNC_DEF_EXTERN void FUNC_CALL_TYPE quatMul(float res, float4 a, float4 b);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE quatConj(float res, float4 a);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE quatDot(float4 v1, float4 v2);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE quatNormalize(float4 res, float4 v);

//////////////////////////////////////////////////////////////////////////
/// Spherical (integer)
#define D_MAX_INT			   2147483648.0f
#define D_1_MAX_INT			0.00000000046566128730774f
#define D_MAX_INT_OVER_PI	683565275.4172766467f
#define D_MAX_INT_OVER_180	11930464.708333f

#define D_SPHI2RAD(sphi) ((float)(sphi) * (D_1_MAX_INT * D_PI))
#define D_SPHI2DEG(sphi) ((float)(sphi) * (D_1_MAX_INT * 180.0f))
#define D_RAD2SPHI(rad) ((rad) * D_MAX_INT_OVER_PI)
#define D_DEG2SPHI(deg) ((deg) * D_MAX_INT_OVER_180)

FUNC_DEF_EXTERN void FUNC_CALL_TYPE sphi2DecartXZ(float4 dec, int32 lon, int32 lat, float radius);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE decart2SphiXZ(float4 dec, int32 * pLon, int32 * pLat, float * pRadius);

//////////////////////////////////////////////////////////////////////////
/// Common
FUNC_DEF_INLINE int32 FUNC_CALL_TYPE f2i(float f);
FUNC_DEF_INLINE float FUNC_CALL_TYPE i2f(int32 i);
FUNC_DEF_INLINE float FUNC_CALL_TYPE fFloor(float f);
FUNC_DEF_INLINE float FUNC_CALL_TYPE fAbs(float f);

FUNC_DEF_INLINE float FUNC_CALL_TYPE fMax(float f1, float f2);
FUNC_DEF_INLINE float FUNC_CALL_TYPE fMin(float f1, float f2);

FUNC_DEF_INLINE float FUNC_CALL_TYPE fSin(float rad);
FUNC_DEF_INLINE float FUNC_CALL_TYPE fCos(float rad);
FUNC_DEF_INLINE void FUNC_CALL_TYPE fSinCos(float * s, float * c, float radFromMinusPiToPi);
FUNC_DEF_INLINE float FUNC_CALL_TYPE deadlySinEx(float radFromMinusPiToPi);
FUNC_DEF_INLINE float FUNC_CALL_TYPE deadlyCosEx(float radFromMinusPiToPi);
FUNC_DEF_INLINE float FUNC_CALL_TYPE deadlyTanEx(float radFromMinusPiToPi);
FUNC_DEF_INLINE void FUNC_CALL_TYPE deadlySinCosEx(float * s, float * c, float radFromMinusPiToPi);

FUNC_DEF_EXTERN float FUNC_CALL_TYPE deadlySin(float rad);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE deadlyCos(float rad);
FUNC_DEF_EXTERN float FUNC_CALL_TYPE deadlyTan(float rad);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE deadlySinCos(float * s, float * c, float rad);

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE int32 FUNC_CALL_TYPE f2i(float f) {	
	int i;
	D_F2I_CAST(f, i);
	return i;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE i2f(int32 i) {	
	float f;
	D_I2F_CAST(i, f);
	return f;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE fAbs(float f) {
	D_F_ABS(f);
	return f;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE fMax(float f1, float f2) {
	return (f2 > f1) ? f2 : f1;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE fMin(float f1, float f2) {
	return (f2 > f1) ? f1 : f2;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE fFloor(float f) {
	float a = f;
	D_F_FLOOR(a);
	return a;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE fSin(float rad) {
	float f;
#if defined(_WIN32) && defined(_MSC_VER)
	__asm {
		fld rad
		fsin 
		fstp f
	}	
#elif defined(__GNUC__)
	__asm__ __volatile__ ("fsin" : "=t" (f) : "0" (rad));
#else

#endif
	return f;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE fCos(float rad) {
	float f;
#if defined(_WIN32) && defined(_MSC_VER)
	__asm {
		fld rad
		fcos
		fstp f
	}
#elif defined(__GNUC__)
	__asm__ __volatile__ ("fcos" : "=t" (f) : "0" (rad));
#else

#endif
	return f;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE void FUNC_CALL_TYPE fSinCos(float * s, float * c, float rad) {
#if defined(_WIN32) && defined(_MSC_VER)
	__asm {
		push eax
		fld rad
		fsincos
		mov eax, c
		fstp dword ptr [eax]
		mov eax, s
		fstp dword ptr [eax]
		pop eax
	}
#elif defined(__GNUC__)
// 	__asm__ __volatile__ (".intel_syntax noprefix
// 						push eax 
// 						fld rad  
// 						fsincos 
// 						mov eax, c 
// 						fstp dword ptr [eax] 
// 						mov eax, s 
// 						fstp dword ptr [eax] 
// 						pop eax\n"
// 						);
#else
#endif
}

//////////////////////////////////////////////////////////////////////////

#define D_B (4.0f * D_1_PI)
#define D_C (-4.0f * D_1_SQUARE_PI)
#define D_P 0.225f
#define D_Q 0.775f

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE deadlySinEx(float radFromMinusPiToPi) {

	/// Calculate the deadly sin
	float y = radFromMinusPiToPi * (D_B + D_C * fAbs(radFromMinusPiToPi));
	y = y * (D_Q + D_P * fAbs(y));

	return y;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE deadlyCosEx(float radFromMinusPiToPi) {

	float y;
	radFromMinusPiToPi += D_PI_2;
	radFromMinusPiToPi -= (radFromMinusPiToPi > D_PI) ? D_DOUBLE_PI : 0.0f;
	/// Calculate the deadly sin
	y = radFromMinusPiToPi * (D_B + D_C * fAbs(radFromMinusPiToPi));
	y = y * (D_Q + D_P * fAbs(y));

	return y;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE void FUNC_CALL_TYPE deadlySinCosEx(float * s, float * c, float radFromMinusPiToPi) {

	float ts, tc;
	/// Calculate sin
	ts = radFromMinusPiToPi * (D_B + D_C * fAbs(radFromMinusPiToPi));
	ts = ts * (D_Q + D_P * fAbs(ts));

	/// Calculate cos
	radFromMinusPiToPi += D_PI_2;
	radFromMinusPiToPi -= (radFromMinusPiToPi > D_PI) ? D_DOUBLE_PI : 0.0f;
	tc = radFromMinusPiToPi * (D_B + D_C * fAbs(radFromMinusPiToPi));
	tc = tc * (D_Q + D_P * fAbs(tc));

	*s = ts;
	*c = tc;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE float FUNC_CALL_TYPE deadlyTanEx(float radFromMinusPiToPi) {

	float s, c;
	/// Calculate sin
	s = radFromMinusPiToPi * (D_B + D_C * fAbs(radFromMinusPiToPi));
	s = s * (D_Q + D_P * fAbs(s));

	/// Calculate cos
	radFromMinusPiToPi += D_PI_2;
	radFromMinusPiToPi -= (radFromMinusPiToPi > D_PI) ? D_DOUBLE_PI : 0.0f;
	c = radFromMinusPiToPi * (D_B + D_C * fAbs(radFromMinusPiToPi));
	c = c * (D_Q + D_P * fAbs(c));

	return s / c;
}

//////////////////////////////////////////////////////////////////////////

FUNC_DEF_INLINE void FUNC_CALL_TYPE vecInit(float4 vec, float4 unaligned) {
	vec[0] = unaligned[0];
	vec[1] = unaligned[1];
	vec[2] = unaligned[2];
	vec[3] = unaligned[3];
}

FUNC_DEF_INLINE void FUNC_CALL_TYPE vecInitVec3(float4 vec, float unaligned[3]) {
	vec[0] = unaligned[0];
	vec[1] = unaligned[1];
	vec[2] = unaligned[2];
	vec[3] = 1.0f;
}

FUNC_DEF_INLINE void FUNC_CALL_TYPE vecInitFloat3(float4 vec, float x, float y, float z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
	vec[3] = 1.0f;
}

FUNC_DEF_INLINE float FUNC_CALL_TYPE vecMax(float4 vec) {
	return fMax(fMax(vec[0], vec[1]), vec[2]);
}

FUNC_DEF_INLINE float FUNC_CALL_TYPE vecMax4(float4 vec) {
	return fMax(fMax(fMax(vec[0], vec[1]), vec[2]), vec[3]);
}

FUNC_DEF_INLINE float FUNC_CALL_TYPE vecMin(float4 vec) {
	return fMin(fMin(vec[0], vec[1]), vec[2]);
}

FUNC_DEF_INLINE float FUNC_CALL_TYPE vecMin4(float4 vec) {
	return fMin(fMin(fMin(vec[0], vec[1]), vec[2]), vec[3]);
}

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////


#endif//__MATH__