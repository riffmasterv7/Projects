
#include "../math_v7.h"

#if defined(USE_SSE)
#include <xmmintrin.h>
#include <emmintrin.h>
#endif

//////////////////////////////////////////////////////////////////////////

const vec4 g_VEC4_ZERO3 = {0.0f, 0.0f, 0.0f, 1.0f};
const vec4 g_VEC4_ZERO4 = {0.0f, 0.0f, 0.0f, 0.0f};
const vec4 g_VEC4_NEG3 = {-1.0f, -1.0f, -1.0f, 1.0f};
const vec4 g_VEC4_NEG4 = {-1.0f, -1.0f, -1.0f, -1.0f};
const vec4 g_VEC4_UNIT = {1.0f, 1.0f, 1.0f, 1.0f};
const vec4 g_VEC4_AXIS_X = {1.0f, 0.0f, 0.0f, 0.0f};
const vec4 g_VEC4_AXIS_Y = {0.0f, 1.0f, 0.0f, 0.0f};
const vec4 g_VEC4_AXIS_Z = {0.0f, 0.0f, 1.0f, 0.0f};

//////////////////////////////////////////////////////////////////////////

float FUNC_CALL_TYPE  vecDot(float4 v1, float4 v2) {
#if !defined(USE_SSE)
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2;

	xmm0 = _mm_load_ps(v1);
	xmm0 =_mm_mul_ps(xmm0, _mm_load_ps(v2));

	xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 2, 0, 1));
	xmm2 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 0, 1, 2));
	xmm0 = _mm_add_ss(xmm0, xmm1);
	xmm0 = _mm_add_ss(xmm0, xmm2);

	return *(float *)&xmm0;
#endif
}

float FUNC_CALL_TYPE  vecDot4(float4 v1, float4 v2) {
#if !defined(USE_SSE)
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
#else
	__m128 xmm0;
	__m128 xmm1;
	xmm0 = _mm_load_ps(v1);
	xmm0 =_mm_mul_ps(xmm0, _mm_load_ps(v2));

	xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(1, 0, 3, 2));
	xmm0 = _mm_add_ps(xmm0, xmm1);
	xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 2, 0, 1));
	xmm0 = _mm_add_ss(xmm0, xmm1);

	return *(float *)&xmm0;
#endif
}

void FUNC_CALL_TYPE  vecCrossRH(float4 res, float4 v1, float4 v2) {
#if !defined(USE_SSE)
	res[0] = v1[1] * v2[2] - v1[2] * v2[1];
	res[1] = v1[2] * v2[0] - v1[0] * v2[2];
	res[2] = v1[0] * v2[1] - v1[1] * v2[0];
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2; 
	__m128 xmm3;

	xmm0 = _mm_load_ps(v1);
	xmm1 = _mm_load_ps(v2);

	xmm2 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 0, 2, 1));
	xmm3 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 1, 0, 2));

	xmm2 = _mm_mul_ps(xmm2, xmm3);
	xmm0 = _mm_mul_ps(xmm0, xmm3);

	xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 1, 0, 2));
	_mm_store_ps(res, _mm_sub_ps(xmm2, xmm1));
#endif
}

void FUNC_CALL_TYPE  vecCrossLH(float4 res, float4 v1, float4 v2) {
#if !defined(USE_SSE)
	res[0] = v1[2] * v2[1] - v1[1] * v2[2];
	res[1] = v1[0] * v2[2] - v1[2] * v2[0];
	res[2] = v1[1] * v2[0] - v1[0] * v2[1];
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2; 
	__m128 xmm3;

	xmm0 = _mm_load_ps(v1);
	xmm1 = _mm_load_ps(v2);

	xmm2 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 0, 2, 1));
	xmm3 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 1, 0, 2));

	xmm2 = _mm_mul_ps(xmm2, xmm3);
	xmm0 = _mm_mul_ps(xmm0, xmm3);

	xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 1, 0, 2));
	_mm_store_ps(res, _mm_sub_ps(xmm1, xmm2));
#endif
}

void FUNC_CALL_TYPE  vecAdd(float4 res, float4 v1, float4 v2) {
#if !defined(USE_SSE)
	res[0] = v1[0] + v2[0];
	res[1] = v1[1] + v2[1];
	res[2] = v1[2] + v2[2];
	res[3] = v1[3] + v2[3];
#else
	_mm_store_ps(res, _mm_add_ps(_mm_load_ps(v1), _mm_load_ps(v2)));
#endif
}

void FUNC_CALL_TYPE  vecSub(float4 res, float4 v1, float4 v2) {
#if !defined(USE_SSE)
	res[0] = v1[0] - v2[0];
	res[1] = v1[1] - v2[1];
	res[2] = v1[2] - v2[2];
	res[3] = v1[3] - v2[3];
#else
	_mm_store_ps(res, _mm_sub_ps(_mm_load_ps(v1), _mm_load_ps(v2)));
#endif
}

void FUNC_CALL_TYPE  vecMul(float4 res, float4 v1, float4 v2) {
#if !defined(USE_SSE)
	res[0] = v1[0] * v2[0];
	res[1] = v1[1] * v2[1];
	res[2] = v1[2] * v2[2];
	res[3] = v1[3] * v2[3];
#else
	_mm_store_ps(res, _mm_mul_ps(_mm_load_ps(v1), _mm_load_ps(v2)));
#endif
}

void FUNC_CALL_TYPE  vecScale(float4 res, float4 v, float f) {
#if !defined(USE_SSE)
	res[0] = v[0] * f;
	res[1] = v[1] * f;
	res[2] = v[2] * f;
#else
	__m128 xmm0 = _mm_load_ss(&f);
	_mm_store_ps(res, _mm_mul_ps(_mm_load_ps(v), _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(0, 0, 0, 0))));
#endif
}

void FUNC_CALL_TYPE  vecCopy(float4 to, float4 from) {
#if !defined(USE_SSE)
	to[0] = from[0];
	to[1] = from[1];
	to[2] = from[2];
	to[3] = from[3];
#else
	_mm_store_ps(to, _mm_load_ps(from));
#endif
}

void FUNC_CALL_TYPE  vecSwap(float4 to, float4 from) {
#if !defined(USE_SSE)
	float t0 = from[0];
	float t1 = from[1];
	float t2 = from[2];
	float t3 = from[3];
	to[0] = t0;
	to[1] = t1;
	to[2] = t2;
	to[3] = t3;
#else
	__m128 xmm1 = _mm_load_ps(from);
	__m128 xmm0 = _mm_load_ps(to);
	_mm_store_ps(from, xmm0);
	_mm_store_ps(to  , xmm1);
#endif
}

void FUNC_CALL_TYPE  vecZero(float4 v) {
#if !defined(USE_SSE)
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
	v[3] = 1.0f;
#else
	__m128 xmm0 = _mm_load_ps(g_VEC4_ZERO3);
	_mm_store_ps(v, xmm0);
#endif
}

void FUNC_CALL_TYPE  vecZero4(float4 v) {
#if !defined(USE_SSE)
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
	v[3] = 0.0f;
#else
	__m128 xmm0 = _mm_load_ps(g_VEC4_ZERO4);
	_mm_store_ps(v, xmm0);
#endif
}

void FUNC_CALL_TYPE  vecDiv(float4 res, float4 v1, float4 v2) {
#if !defined(USE_SSE)
	res[0] = v1[0] / v2[0];
	res[1] = v1[1] / v2[1];
	res[2] = v1[2] / v2[2];
	res[3] = v1[3] / v2[3];
#else
	_mm_store_ps(res, _mm_div_ps(_mm_load_ps(v1), _mm_load_ps(v2)));
#endif
}

float FUNC_CALL_TYPE  vecMag(float4 v) {
#if !defined(USE_SSE)
	float magSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	return sqrt(magSq);
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2;
	__m128 xmm3;
	xmm0 = _mm_load_ps(v);
	xmm1 = _mm_mul_ps(xmm0, xmm0);
	xmm2 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 2, 0, 1));
	xmm3 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 0, 1, 2));
	xmm1 = _mm_add_ps(xmm1, xmm2);
	xmm1 = _mm_add_ps(xmm1, xmm3);
	xmm0 = _mm_sqrt_ss(xmm1);

	return *(float *)&xmm0;
#endif
}

float FUNC_CALL_TYPE  vecMagSq(float4 v) {
#if !defined(USE_SSE)
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2;
	__m128 xmm3;
	xmm0 = _mm_load_ps(v);
	xmm1 = _mm_mul_ps(xmm0, xmm0);
	xmm2 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 2, 0, 1));
	xmm3 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 0, 1, 2));
	xmm1 = _mm_add_ps(xmm1, xmm2);
	xmm0 = _mm_add_ps(xmm1, xmm3);

	return *(float *)&xmm0;
#endif
}

float FUNC_CALL_TYPE  vecMag4(float4 v) {
#if !defined(USE_SSE)
	float magSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
   return sqrt(magSq);
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2;
	xmm0 = _mm_load_ps(v);
	xmm0 = _mm_mul_ps(xmm0, xmm0);
	xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(1, 0, 3, 2));
	xmm0 = _mm_add_ps(xmm0, xmm1);
	xmm2 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 2, 0, 1));
	xmm1 = _mm_add_ss(xmm0, xmm2);
	xmm0 = _mm_sqrt_ss(xmm1);
	return *(float *)&xmm0;
#endif
}

float FUNC_CALL_TYPE  vecDist(float4 v1, float4 v2) {
#if !defined(USE_SSE)
	vec4 diff;
	diff[0] = v1[0] - v2[0];
	diff[1] = v1[1] - v2[1];
	diff[2] = v1[2] - v2[2];
	return sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2;
	__m128 xmm3;
	xmm0 = _mm_load_ps(v1);
	xmm1 = _mm_load_ps(v2);
	xmm0 = _mm_sub_ps(xmm0, xmm1);
	xmm1 = _mm_mul_ps(xmm0, xmm0);
	xmm2 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 2, 1, 1));
	xmm3 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 2, 1, 2));
	xmm0 = _mm_add_ss(xmm1, xmm2);
	xmm0 = _mm_add_ss(xmm0, xmm3);
	xmm0 = _mm_sqrt_ss(xmm0);
	return *(float *)&xmm0;
#endif
}

float FUNC_CALL_TYPE  vecDistSq(float4 v1, float4 v2) {
#if !defined(USE_SSE)
	vec4 diff;
	diff[0] = v1[0] - v2[0];
	diff[1] = v1[1] - v2[1];
	diff[2] = v1[2] - v2[2];
	return diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2;
	__m128 xmm3;
	xmm0 = _mm_load_ps(v1);
	xmm1 = _mm_load_ps(v2);
	xmm0 = _mm_sub_ps(xmm0, xmm1);
	xmm1 = _mm_mul_ps(xmm0, xmm0);
	xmm2 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 2, 1, 1));
	xmm3 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 2, 1, 2));
	xmm0 = _mm_add_ss(xmm1, xmm2);
	xmm0 = _mm_add_ss(xmm0, xmm3);
	return *(float *)&xmm0;
#endif
}
void FUNC_CALL_TYPE  vecRcp(float4 res, float4 v) {
#if !defined(USE_SSE)
	res[0] = 1.0f / v[0];
	res[1] = 1.0f / v[1];
	res[2] = 1.0f / v[2];
	res[3] = 1.0f / v[3];
#else
	_mm_store_ps(res, _mm_rcp_ps(_mm_load_ps(v)));
#endif
}
void FUNC_CALL_TYPE  vecNeg(float4 res, float4 v) {
#if !defined(USE_SSE)
	res[0] = -v[0];
	res[1] = -v[1];
	res[2] = -v[2];
#else
	__m128 xmm0 = _mm_load_ps(v);
	_mm_store_ps(res, _mm_mul_ps(xmm0, _mm_load_ps(g_VEC4_NEG3)));
#endif
}

void FUNC_CALL_TYPE  vecNeg4(float4 res, float4 v) {
#if !defined(USE_SSE)
	res[0] = -v[0];
	res[1] = -v[1];
	res[2] = -v[2];
	res[3] = -v[3];
#else
	__m128 xmm0 = _mm_load_ps(v);
	_mm_store_ps(res, _mm_mul_ps(xmm0, _mm_load_ps(g_VEC4_NEG4)));
#endif
}

void FUNC_CALL_TYPE  vecNormalize(float4 res, float4 v) {
#if !defined(USE_SSE)
	vec4 diff;
	diff[0] = v[0] * v[0];
	diff[1] = v[1] * v[1];
	diff[2] = v[2] * v[2];
	diff[3] = 1.0f / sqrt(diff[0] + diff[1] + diff[2]);
	res[0] = v[0] * diff[3];
	res[1] = v[1] * diff[3];
	res[2] = v[2] * diff[3];
	res[3] = 0.0f;
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2;
	__m128 xmm3;
	xmm0 = _mm_load_ps(v);
	xmm1 = _mm_mul_ps(xmm0, xmm0);
	xmm2 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 2, 0, 1));
	xmm3 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 0, 1, 2));
	xmm1 = _mm_add_ss(xmm1, xmm2);
	xmm1 = _mm_add_ss(xmm1, xmm3);
	xmm2 = _mm_sqrt_ss(xmm1);
	xmm3 = _mm_shuffle_ps(xmm2, xmm2, _MM_SHUFFLE(0, 0, 0, 0));
	xmm1 = _mm_div_ps(xmm0, xmm3);
	_mm_store_ps(res, xmm1);
#endif
}
float FUNC_CALL_TYPE  vecNormalizeEx(float4 res, float4 v) {
#if !defined(USE_SSE)
	vec4 diff;
	diff[0] = v[0] * v[0];
	diff[1] = v[1] * v[1];
	diff[2] = v[2] * v[2];
   res[3] = sqrt(diff[0] + diff[1] + diff[2]);
	diff[3] = 1.0f / res[3];
	res[0] = diff[0] * diff[3];
	res[1] = diff[1] * diff[3];
	res[2] = diff[2] * diff[3];
	diff[3] = res[3];
	res[3] = 0.0f;

	return diff[3];
#else
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2;
	__m128 xmm3;
	xmm0 = _mm_load_ps(v);
	xmm1 = _mm_mul_ps(xmm0, xmm0);
	xmm2 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 2, 0, 1));
	xmm3 = _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 0, 1, 2));
	xmm1 = _mm_add_ps(xmm1, xmm2);
	xmm1 = _mm_add_ps(xmm1, xmm3);
	xmm2 = _mm_sqrt_ss(xmm1);
	xmm3 = _mm_shuffle_ps(xmm2, xmm2, _MM_SHUFFLE(0, 0, 0, 0));
	xmm1 = _mm_div_ps(xmm0, xmm3);
	_mm_store_ps(res, xmm1);
	return *(float *)&xmm2;
#endif
}

void FUNC_CALL_TYPE vecAbs(float4 res, float4 v) {
   
   const int32 maxInt32 = 0x7FFFFFFF;
   __m128 xmm0 = _mm_load_ps(v);
   __m128 xmm1 = _mm_and_ps(_mm_set1_ps(*(float *)&maxInt32), xmm0);
   _mm_store_ps(res, xmm1);
}

