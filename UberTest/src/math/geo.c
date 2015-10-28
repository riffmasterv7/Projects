
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "geo.h"

#define D_USE_FSINCOS

void FUNC_CALL_TYPE geo2DecartXZ(float4 dec, int32 lon, int32 lat, float rad) {

	float sinLon, cosLon;
	float sinLat, cosLat;
	float lonf = D_GEO2RAD(lon);
	float latf = D_GEO2RAD(lat);
	

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

void FUNC_CALL_TYPE decart2GeoXZ(float4 dec, int32 * pLon, int32 * pLat, float * pRad) {

	float lonf;
	float latf;

	*pRad = vecMag(dec);

	lonf = D_RAD2DEG(atan2f(dec[0], dec[2]));
	latf = D_RAD2DEG(asinf(dec[1] / *pRad));

	lonf = D_DEG2GEO(lonf);
	latf = D_DEG2GEO(latf);

//  	lonf = D_RAD2GEO(atan2f(dec[0], dec[2]));
//  	latf = D_RAD2GEO(asinf(dec[1] / *pRad));

	D_F2I_CAST(lonf, *pLon);
	D_F2I_CAST(latf, *pLat);
}