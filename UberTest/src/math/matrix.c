
#include <memory.h>

#include "../math_v7.h"

#if defined(USE_SSE)
#include <xmmintrin.h>
#include <emmintrin.h>
#endif

//////////////////////////////////////////////////////////////////////////

const mtx g_MTX_UNIT = {
	{ 1.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f }
};

const mtx g_MTX_ZERO = {
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f }
};

//////////////////////////////////////////////////////////////////////////

#define D_MTX_CPY(to, from) \
	_mm_store_ps(to[0], _mm_load_ps(from[0])); \
 	_mm_store_ps(to[1], _mm_load_ps(from[1])); \
 	_mm_store_ps(to[2], _mm_load_ps(from[2])); \
 	_mm_store_ps(to[3], _mm_load_ps(from[3]));

#define D_MTX_CPY1(to, from) \
	{ \
		__m128 xmm0, xmm1, xmm2, xmm3; \
		xmm0 = _mm_load_ps(from[0]); \
		xmm1 = _mm_load_ps(from[1]); \
		xmm2 = _mm_load_ps(from[2]); \
		xmm3 = _mm_load_ps(from[3]); \
		_mm_store_ps(to[0], xmm0); \
		_mm_store_ps(to[1], xmm1); \
		_mm_store_ps(to[2], xmm2); \
		_mm_store_ps(to[3], xmm3); \
	}

#define D_MTX_CPY2(to1, from1, to2, from2) \
{ \
	__m128 xmm0, xmm1, xmm2, xmm3; \
	__m128 xmm4, xmm5, xmm6, xmm7; \
	xmm0 = _mm_load_ps(from1[0]); \
	xmm1 = _mm_load_ps(from1[1]); \
	xmm2 = _mm_load_ps(from1[2]); \
	xmm3 = _mm_load_ps(from1[3]); \
	xmm4 = _mm_load_ps(from2[0]); \
	xmm5 = _mm_load_ps(from2[1]); \
	xmm6 = _mm_load_ps(from2[2]); \
	xmm7 = _mm_load_ps(from2[3]); \
	_mm_store_ps(to1[0], xmm0); \
	_mm_store_ps(to1[1], xmm1); \
	_mm_store_ps(to1[2], xmm2); \
	_mm_store_ps(to1[3], xmm3); \
	_mm_store_ps(to2[0], xmm4); \
	_mm_store_ps(to2[1], xmm5); \
	_mm_store_ps(to2[2], xmm6); \
	_mm_store_ps(to2[3], xmm7); \
	}

//////////////////////////////////////////////////////////////////////////

void FUNC_CALL_TYPE mtxUnit(float44 m) {
#if !defined(USE_SSE)
	memcpy(m, g_MTX_UNIT, sizeof(float44));
#else
	D_MTX_CPY(m, g_MTX_UNIT);
#endif
}

void FUNC_CALL_TYPE mtxZero(float44 m) {
#if !defined(USE_SSE)
	memcpy(m, g_MTX_ZERO, sizeof(float44));
#else
	D_MTX_CPY(m, g_MTX_ZERO);
#endif
}

void FUNC_CALL_TYPE mtxCopy(float44 dst, float44 src) {
#if !defined(USE_SSE)
	memcpy(dst, src, sizeof(float44));
#else
	D_MTX_CPY(dst, src);
#endif
}

void FUNC_CALL_TYPE mtxAdd(float44 res, float44 a, float44 b) {
	float * ar = (float *)&res[0];
	float * aa = (float *)&a[0];
	float * ab = (float *)&b[0];
	int32 i = 0;
	for (; i < 16; ++i) {
		ar[i] = aa[i] + ab[i];
	}
}

void FUNC_CALL_TYPE mtxSub(float44 res, float44 a, float44 b) {
	float * ar = (float *)&res[0];
	float * aa = (float *)&a[0];
	float * ab = (float *)&b[0];
	int32 i = 0;
	for (; i < 16; ++i) {
		ar[i] = aa[i] - ab[i];
	}
}

void FUNC_CALL_TYPE mtxMul(float44 res, float44 a, float44 b) {
#if !defined(USE_SSE)
	mtx m;

	m[0][0] = a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0] + a[0][3]*b[3][0];
	m[0][1] = a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1] + a[0][3]*b[3][1];
	m[0][2] = a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2] + a[0][3]*b[3][2];
	m[0][3] = a[0][0]*b[0][3] + a[0][1]*b[1][3] + a[0][2]*b[2][3] + a[0][3]*b[3][3];

	m[1][0] = a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0] + a[1][3]*b[3][0];
	m[1][1] = a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1] + a[1][3]*b[3][1];
	m[1][2] = a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2] + a[1][3]*b[3][2];
	m[1][3] = a[1][0]*b[0][3] + a[1][1]*b[1][3] + a[1][2]*b[2][3] + a[1][3]*b[3][3];

	m[2][0] = a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0] + a[2][3]*b[3][0];
	m[2][1] = a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1] + a[2][3]*b[3][1];
	m[2][2] = a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2] + a[2][3]*b[3][2];
	m[2][3] = a[2][0]*b[0][3] + a[2][1]*b[1][3] + a[2][2]*b[2][3] + a[2][3]*b[3][3];

	m[3][0] = a[3][0]*b[0][0] + a[3][1]*b[1][0] + a[3][2]*b[2][0] + a[3][3]*b[3][0];
	m[3][1] = a[3][0]*b[0][1] + a[3][1]*b[1][1] + a[3][2]*b[2][1] + a[3][3]*b[3][1];
	m[3][2] = a[3][0]*b[0][2] + a[3][1]*b[1][2] + a[3][2]*b[2][2] + a[3][3]*b[3][2];
	m[3][3] = a[3][0]*b[0][3] + a[3][1]*b[1][3] + a[3][2]*b[2][3] + a[3][3]*b[3][3];

	mtxCopy(res, m);
#else

	mtx m;
	__m128 b0, b1, b2, b3;
	__m128 a0_2, a1_3;
	__m128 temp1, temp2;

	b0 = _mm_load_ps(b[0]);
	b1 = _mm_load_ps(b[1]);
	b2 = _mm_load_ps(b[2]);
	b3 = _mm_load_ps(b[3]);

	//////////////////////////////////////////////////////////////////////////

	a0_2 = _mm_load_ps(a[0]);
	a1_3 = _mm_load_ps(a[1]);

	/// row0
	temp1 = _mm_shuffle_ps(a0_2, a0_2, _MM_SHUFFLE(0, 0, 0, 0));
	temp2 = _mm_shuffle_ps(a0_2, a0_2, _MM_SHUFFLE(1, 1, 1, 1));
	temp1 = _mm_mul_ps(b0, temp1);
	temp2 = _mm_mul_ps(b1, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	temp2 = _mm_shuffle_ps(a0_2, a0_2, _MM_SHUFFLE(2, 2, 2, 2));
	temp2 = _mm_mul_ps(b2, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	temp2 = _mm_shuffle_ps(a0_2, a0_2, _MM_SHUFFLE(3, 3, 3, 3));
	temp2 = _mm_mul_ps(b3, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	_mm_store_ps(m[0], temp1);

	/// row1
	temp1 = _mm_shuffle_ps(a1_3, a1_3, _MM_SHUFFLE(0, 0, 0, 0));
	temp2 = _mm_shuffle_ps(a1_3, a1_3, _MM_SHUFFLE(1, 1, 1, 1));
	temp1 = _mm_mul_ps(b0, temp1);
	temp2 = _mm_mul_ps(b1, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	temp2 = _mm_shuffle_ps(a1_3, a1_3, _MM_SHUFFLE(2, 2, 2, 2));
	temp2 = _mm_mul_ps(b2, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	temp2 = _mm_shuffle_ps(a1_3, a1_3, _MM_SHUFFLE(3, 3, 3, 3));
	temp2 = _mm_mul_ps(b3, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	_mm_store_ps(m[1], temp1);

	//////////////////////////////////////////////////////////////////////////
	a0_2 = _mm_load_ps(a[2]);
	a1_3 = _mm_load_ps(a[3]);

	/// row2
	temp1 = _mm_shuffle_ps(a0_2, a0_2, _MM_SHUFFLE(0, 0, 0, 0));
	temp2 = _mm_shuffle_ps(a0_2, a0_2, _MM_SHUFFLE(1, 1, 1, 1));
	temp1 = _mm_mul_ps(b0, temp1);
	temp2 = _mm_mul_ps(b1, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	temp2 = _mm_shuffle_ps(a0_2, a0_2, _MM_SHUFFLE(2, 2, 2, 2));
	temp2 = _mm_mul_ps(b2, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	temp2 = _mm_shuffle_ps(a0_2, a0_2, _MM_SHUFFLE(3, 3, 3, 3));
	temp2 = _mm_mul_ps(b3, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	_mm_store_ps(m[2], temp1);

	/// row3
	temp1 = _mm_shuffle_ps(a1_3, a1_3, _MM_SHUFFLE(0, 0, 0, 0));
	temp2 = _mm_shuffle_ps(a1_3, a1_3, _MM_SHUFFLE(1, 1, 1, 1));
	temp1 = _mm_mul_ps(b0, temp1);
	temp2 = _mm_mul_ps(b1, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	temp2 = _mm_shuffle_ps(a1_3, a1_3, _MM_SHUFFLE(2, 2, 2, 2));
	temp2 = _mm_mul_ps(b2, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	temp2 = _mm_shuffle_ps(a1_3, a1_3, _MM_SHUFFLE(3, 3, 3, 3));
	temp2 = _mm_mul_ps(b3, temp2);
	temp1 = _mm_add_ps(temp1, temp2);
	_mm_store_ps(m[3], temp1);

	D_MTX_CPY(res, m);

#endif
}

void FUNC_CALL_TYPE mtxApply(float4 dst, float44 m, float4 src) {

#if !defined(USE_SSE)
	float x = src[0];
	float y = src[1];
	float z = src[2];
	float w = src[3];

	dst[0] = x * m[0][0] + y * m[1][0] + z * m[2][0] + w * m[3][0];
	dst[1] = x * m[0][1] + y * m[1][1] + z * m[2][1] + w * m[3][1];
	dst[2] = x * m[0][2] + y * m[1][2] + z * m[2][2] + w * m[3][2];
	dst[3] = x * m[0][3] + y * m[1][3] + z * m[2][3] + w * m[3][3];
#else
	__m128 v0, v1, v2, v3;
	__m128 m0, m1, m2, m3;

	v0 = _mm_load_ps(src);
	m0 = _mm_load_ps(m[0]);
	m1 = _mm_load_ps(m[1]);
	m2 = _mm_load_ps(m[2]);
	m3 = _mm_load_ps(m[3]);

	v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(1, 1, 1, 1));
	v2 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(2, 2, 2, 2));
	v3 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3, 3, 3, 3));
	v0 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0, 0, 0, 0));

	v0 = _mm_mul_ps(v0, m0);
	v1 = _mm_mul_ps(v1, m1);
	v2 = _mm_mul_ps(v2, m2);
	v3 = _mm_mul_ps(v3, m3);

	v0 = _mm_add_ps(v0, v1);
	v0 = _mm_add_ps(v0, v2);
	v0 = _mm_add_ps(v0, v3);

	_mm_store_ps(dst, v0);
#endif
}

void FUNC_CALL_TYPE mtxTranspose(float44 res, float44 m) {
	float t;
	res[0][0] = m[0][0];
	res[1][1] = m[1][1];
	res[2][2] = m[2][2];
	res[3][3] = m[3][3];
	t = m[0][1]; res[0][1] = m[1][0]; res[1][0] = t;
	t = m[0][2]; res[0][2] = m[2][0]; res[2][0] = t;
	t = m[0][3]; res[0][3] = m[3][0]; res[3][0] = t;
	t = m[1][2]; res[1][2] = m[2][1]; res[2][1] = t;
	t = m[1][3]; res[1][3] = m[3][1]; res[3][1] = t;
	t = m[2][3]; res[2][3] = m[3][2]; res[3][2] = t;
}

void FUNC_CALL_TYPE mtxInvertFull(float44 res, float44 m) {
#if !defined(USE_SSE)
	mtx src;
	int32 row;
	int32 col;
	int32 j;

	memcpy(res, g_MTX_UNIT, sizeof(float44));
	memcpy(src, m, sizeof(float44));

	for (row = 0; row < 4; ++row) {

		float e = src[row][row];

		if (e != 1.0f) {

			float invE = 1.0f / e;

			for (col = row; col < 4; ++col) {
				src[row][col] *= invE;
			}

// 			src[row][0] *= invE;
// 			src[row][1] *= invE;
// 			src[row][2] *= invE;
// 			src[row][3] *= invE;

			res[row][0] *= invE;
			res[row][1] *= invE;
			res[row][2] *= invE;
			res[row][3] *= invE;
		}

		for (j = 0; j < 4; ++j) {

			if (j != row) {

				float e = src[j][row];
				if (e == 0.0f) continue;
				e = -e;

				for (col = row; col < 4; ++col) {

					src[j][col] = src[j][col] + e * src[row][col];
				}

// 				src[j][0] = src[j][0] + e * src[row][0];
// 				src[j][1] = src[j][1] + e * src[row][1];
// 				src[j][2] = src[j][2] + e * src[row][2];
// 				src[j][3] = src[j][3] + e * src[row][3];

				res[j][0] = res[j][0] + e * res[row][0];
				res[j][1] = res[j][1] + e * res[row][1];
				res[j][2] = res[j][2] + e * res[row][2];
				res[j][3] = res[j][3] + e * res[row][3];
			}
		}
	}
#else
	mtx src;
	int32 row;
	int32 j;
	__m128 xmm0;
	__m128 xmm1;
	__m128 xmm2;
	__m128 xmm3;
	__m128 xmm4;
	__m128 xmm7;

	xmm7 = _mm_load_ps(g_VEC4_NEG4);

//	D_MTX_CPY(res, g_MTX_UNIT);
//	D_MTX_CPY(src, m);

//	D_MTX_CPY2(res, g_MTX_UNIT, src, m);

	/// Slower
// 	_mm_store_ps(res[0], _mm_load_ps(g_MTX_UNIT[0]));
// 	_mm_store_ps(res[1], _mm_load_ps(g_MTX_UNIT[1]));
// 	_mm_store_ps(res[2], _mm_load_ps(g_MTX_UNIT[2]));
// 	_mm_store_ps(res[3], _mm_load_ps(g_MTX_UNIT[3]));
//
// 	_mm_store_ps(src[0], _mm_load_ps(m[0]));
// 	_mm_store_ps(src[1], _mm_load_ps(m[1]));
// 	_mm_store_ps(src[2], _mm_load_ps(m[2]));
// 	_mm_store_ps(src[3], _mm_load_ps(m[3]));

	for (row = 0; row < 4; ++row) {

		xmm0 = _mm_load_ss(&src[row][row]);
		xmm0 = _mm_rcp_ss(xmm0);
		xmm0 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(0, 0, 0, 0));

		xmm1 = _mm_load_ps(src[row]);
		xmm2 = _mm_load_ps(res[row]);
		xmm1 = _mm_mul_ps(xmm1, xmm0);
		xmm2 = _mm_mul_ps(xmm2, xmm0);
		_mm_store_ps(src[row], xmm1);
		_mm_store_ps(res[row], xmm2);

		for (j = 0; j < 4; ++j) {

			if (j != row) {
				if (src[j][row] == 0.0f) continue;
				xmm0 = _mm_load_ss(&src[j][row]);
				xmm0 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(0, 0, 0, 0));
				xmm0 = _mm_mul_ps(xmm0, xmm7);

// 				xmm1 = _mm_load_ps(src[row]);
// 				xmm2 = _mm_load_ps(res[row]);
				xmm3 = _mm_add_ps(_mm_load_ps(src[j]), _mm_mul_ps(xmm1, xmm0));
				xmm4 = _mm_add_ps(_mm_load_ps(res[j]), _mm_mul_ps(xmm2, xmm0));
				_mm_store_ps(src[j], xmm3);
				_mm_store_ps(res[j], xmm4);
			}
		}
	}

#endif
}

void FUNC_CALL_TYPE mtxInvertFast(float44 res, float44 m) { }

void FUNC_CALL_TYPE mtxMakeScaling(float44 m, float4 scale) {
	mtxUnit(m);
	m[0][0] = scale[0];
	m[1][1] = scale[1];
	m[2][2] = scale[2];
}

void FUNC_CALL_TYPE mtxMakeTranslation(float44 m, float4 translate) {
	mtxUnit(m);
	memcpy(m[3], translate, sizeof(float4));
}

void FUNC_CALL_TYPE mtxMakeRotationX(float44 m, float radians) {

   float sint;
   float cost;
   mtxUnit(m);
   fSinCos(&sint, &cost, radians);

	m[1][1] = cost; m[1][2] = -sint;
	m[2][1] = sint; m[2][2] = cost;

}

void FUNC_CALL_TYPE mtxMakeRotationY(float44 m, float radians) {

   float sint;
   float cost;
   mtxUnit(m);
   fSinCos(&sint, &cost, radians);

	m[0][0] = cost; m[0][2] = sint;
	m[2][0] = -sint; m[2][2] = cost;
}

void FUNC_CALL_TYPE mtxMakeRotationZ(float44 m, float radians) {


	float sint;
	float cost;
	mtxUnit(m);
    fSinCos(&sint, &cost, radians);

	m[0][0] = cost; m[0][1] = -sint;
	m[1][0] = sint; m[1][1] = cost;
}

void FUNC_CALL_TYPE mtxMakeRotationAxis(float44 m, float4 normal, float radians) {

   float sint;
   float cost;
   float t;
   float t_x;
   float t_y;
   float t_z;
   float t_x_y;
   float t_y_z;
   float t_x_z;
   float s_x;
   float s_y;
   float s_z;

   fSinCos(&sint, &cost, radians);
	t = 1.0f - cost;
	t_x = t * normal[0];
	t_y = t * normal[1];
	t_z = t * normal[2];
	t_x_y = t_x * normal[1];
	t_y_z = t_y * normal[2];
	t_x_z = t_z * normal[0];
	s_x = sint * normal[0];
	s_y = sint * normal[1];
	s_z = sint * normal[2];

	m[0][0] = t_x * normal[0] + cost;
	m[0][1] = t_x_y + s_z;
	m[0][2] = t_x_z - s_y;
	m[0][3] = 0.0f;

	m[1][0] = t_x_y - s_z;
	m[1][1] = t_y * normal[1] + cost;
	m[1][2] = t_y_z + s_x;
	m[1][3] = 0.0f;

	m[2][0] = t_x_z + s_y;
	m[2][1] = t_y_z - s_x;
	m[2][2] = t_z * normal[2] + cost;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 0.0f;

}

void FUNC_CALL_TYPE mtxMakePerspectiveLH(float44 m, float vertFOVRad, float aspect, float zn, float zf) {

   float tanv;
   {
      float sinv;
      float cosv;
      //fSinCos(&sinv, &cosv, vertFOVRad * 0.5f);
      fSinCos(&sinv, &cosv, (vertFOVRad + D_PI) * 0.5f);
      tanv = sinv / cosv;
   }
   {
   //	float yScale = 1.0f / tanv;
	   float yScale = -tanv;
	   float xScale = yScale / aspect;
	   float zScale = zf / (zf - zn);
	   mtxZero(m);
	   m[0][0] = xScale;
	   m[1][1] = yScale;
	   m[2][2] = zScale;
	   m[2][3] = 1.0f;
	   m[3][2] = zn * -zScale;
   }
}

void FUNC_CALL_TYPE mtxMakePerspectiveRH(float44 m, float vertFOVRad, float aspect, float zn, float zf) {

   float tanv;
   {
      float sinv;
      float cosv;
      //fSinCos(&sinv, &cosv, vertFOVRad * 0.5f);
      fSinCos(&sinv, &cosv, (vertFOVRad + D_PI) * 0.5f);
      tanv = sinv / cosv;
   }
   {
      //	float yScale = 1.0f / tanv;
      float yScale = -tanv;
      float xScale = yScale / aspect;
      float zScale = zf / (zn - zf); /// *
      mtxZero(m);
      m[0][0] = xScale;
      m[1][1] = yScale;
      m[2][2] = zScale;
      m[2][3] = -1.0f;				 /// *
      m[3][2] = zn * zScale;
   }

}

void FUNC_CALL_TYPE mtxMakeLookAtLH(float44 m, float4 eye, float4 target, float4 up) {
	vec4 xAxis;
	vec4 yAxis;
	vec4 zAxis;

	mtxUnit(m);

	vecSub(zAxis, target, eye);
	vecNormalize(zAxis, zAxis);

	vecCrossLH(xAxis, zAxis, up);
	vecNormalize(xAxis, xAxis);

	vecCrossLH(yAxis, xAxis, zAxis);

	m[0][0] = xAxis[0];
	m[1][0] = xAxis[1];
	m[2][0] = xAxis[2];
	m[3][0] = -vecDot(xAxis, eye);

	m[0][1] = yAxis[0];
	m[1][1] = yAxis[1];
	m[2][1] = yAxis[2];
	m[3][1] = -vecDot(yAxis, eye);

	m[0][2] = zAxis[0];
	m[1][2] = zAxis[1];
	m[2][2] = zAxis[2];
	m[3][2] = -vecDot(zAxis, eye);
}

void FUNC_CALL_TYPE mtxMakeLookAtRH(float44 m, float4 eye, float4 target, float4 up) {

	vec4 xAxis;
	vec4 yAxis;
	vec4 zAxis;

	mtxUnit(m);

	vecSub(zAxis, eye, target);
	vecNormalize(zAxis, zAxis);

	vecCrossRH(xAxis, up, zAxis);
	vecNormalize(xAxis, xAxis);

	vecCrossRH(yAxis, zAxis, xAxis);

	m[0][0] = xAxis[0];
	m[1][0] = xAxis[1];
	m[2][0] = xAxis[2];
	m[3][0] = -vecDot(xAxis, eye);

	m[0][1] = yAxis[0];
	m[1][1] = yAxis[1];
	m[2][1] = yAxis[2];
	m[3][1] = -vecDot(yAxis, eye);

	m[0][2] = zAxis[0];
	m[1][2] = zAxis[1];
	m[2][2] = zAxis[2];
	m[3][2] = -vecDot(zAxis, eye);
}

void FUNC_CALL_TYPE mtxMakeOrthoLH(float44 m, float w, float h, float zn, float zf) {

	float zScale = 1.0f / (zn - zf);
	mtxUnit(m);

	m[0][0] = 2.0f / w;
	m[1][1] = 2.0f / h;
	m[2][2] = zScale;
	m[3][2] = -zn * zScale;
}

void FUNC_CALL_TYPE mtxMakeOrthoRH(float44 m, float w, float h, float zn, float zf) {

	float zScale = 1.0f / (zn - zf);
	mtxUnit(m);

	m[0][0] = 2.0f / w;
	m[1][1] = 2.0f / h;
	m[2][2] = zScale;
	m[3][2] = zn * zScale;
}
