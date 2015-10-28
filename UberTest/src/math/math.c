
#include <math.h>

#include "../math_v7.h"

//////////////////////////////////////////////////////////////////////////

float FUNC_CALL_TYPE deadlySin(float rad) {

	float _PI = D_PI;
	float x, y;
	int32 m;

	/// Clamps input angle to -180~180
	x = rad * D_1_DOUBLE_PI;
	D_F2I_CAST(x, m);
	x = rad - (float)(m) * D_DOUBLE_PI;
	D_F_GET_SIGN(x, m);
	D_F_SET_SIGN(_PI, m);
	x = _PI - x;

	/// Calculate function value
	y = x * (D_B + D_C * fAbs(x));
	y = y * (D_Q + D_P * fAbs(y));

	return y;
}

//////////////////////////////////////////////////////////////////////////

float FUNC_CALL_TYPE deadlyCos(float rad) {

	float _PI = D_PI;
	float x, y;
	int32 m;

	rad += D_PI_2;

	/// Clamps input angle to -180~180
	x = rad * D_1_DOUBLE_PI;
	D_F2I_CAST(x, m);
	x = rad - (float)(m) * D_DOUBLE_PI;
	D_F_GET_SIGN(x, m);
	D_F_SET_SIGN(_PI, m);
	x = _PI - x;

	/// Calculate function value
	y = x * (D_B + D_C * fAbs(x));
	y = y * (D_Q + D_P * fAbs(y));

	return y;
}

//////////////////////////////////////////////////////////////////////////

void FUNC_CALL_TYPE deadlySinCos(float * s, float * c, float rad) {

	float _PI = D_PI;
	float ts, tc, x;
	int m;

	/// Calculate sin	
	x = rad * D_1_DOUBLE_PI;
	D_F2I_CAST(x, m);
	x = rad - (float)(m) * D_DOUBLE_PI;
	D_F_GET_SIGN(x, m);
	D_F_SET_SIGN(_PI, m);
	x = _PI - x;

	ts = x * (D_B + D_C * fAbs(x));
	ts = ts * (D_Q + D_P * fAbs(ts));

	/// Calculate cos
	x = (x + D_PI_2);
	x -= (x > D_PI) ? D_DOUBLE_PI : 0.0f;
	x = -x;

	tc = x * (D_B + D_C * fAbs(x));
	tc = tc * (D_Q + D_P * fAbs(tc));

	*s = ts;
	*c = tc;
}

//////////////////////////////////////////////////////////////////////////

float FUNC_CALL_TYPE deadlyTan(float rad) {

	float _PI = D_PI;
	float ts, tc, x;
	int m;

	/// Calculate sin	
	x = rad * D_1_DOUBLE_PI;
	D_F2I_CAST(x, m);
	x = rad - (float)(m) * D_DOUBLE_PI;
	D_F_GET_SIGN(x, m);
	D_F_SET_SIGN(_PI, m);
	x = _PI - x;

	ts = x * (D_B + D_C * fAbs(x));
	ts = ts * (D_Q + D_P * fAbs(ts));

	/// Calculate cos
	x = (x + D_PI_2);
	x -= (x > D_PI) ? D_DOUBLE_PI : 0.0f;
	x = -x;

	tc = x * (D_B + D_C * fAbs(x));
	tc = tc * (D_Q + D_P * fAbs(tc));

	return ts / tc;
}

//////////////////////////////////////////////////////////////////////////
/// Spherical (integer)

#define D_USE_FSINCOS

void FUNC_CALL_TYPE sphi2DecartXZ(float4 dec, int32 lon, int32 lat, float rad) {

   float sinLon, cosLon;
   float sinLat, cosLat;
   float lonf = D_SPHI2RAD(lon);
   float latf = D_SPHI2RAD(lat);


#if !defined (D_USE_FSINCOS)
   deadlySinCosEx(&sinLon, &cosLon, lonf);
   deadlySinCosEx(&sinLat, &cosLat, latf);
#else
   fSinCos(&sinLon, &cosLon, lonf);
   fSinCos(&sinLat, &cosLat, latf);
#endif

   dec[0] = sinLon * cosLat;
   dec[1] = sinLat;
   dec[2] = cosLon * cosLat;

   vecScale(dec, dec, rad);

   dec[3] = 1.0f;
}

void FUNC_CALL_TYPE decart2SphiXZ(float4 dec, int32 * pLon, int32 * pLat, float * pRad) {

   float lonf;
   float latf;

   *pRad = vecMag(dec);

   lonf = D_RAD2DEG(atan2f(dec[0], dec[2]));
   latf = D_RAD2DEG(asinf(dec[1] / *pRad));

   lonf = D_DEG2SPHI(lonf);
   latf = D_DEG2SPHI(latf);

   D_F2I_CAST(lonf, *pLon);
   D_F2I_CAST(latf, *pLat);
}


