
#ifndef __GEODETICAL_MATH__
#define __GEODETICAL_MATH__

#include "../math_v7.h"

#define D_MAX_INT			   2147483648.0f
#define D_1_MAX_INT			0.00000000046566128730774f
#define D_MAX_INT_OVER_PI	683565275.4172766467f
#define D_MAX_INT_OVER_180	11930464.708333f

#define D_SPHI2RAD(geo) ((float)(geo) * (D_1_MAX_INT * D_PI))
#define D_SPHI2DEG(geo) ((float)(geo) * (D_1_MAX_INT * 180.0f))
#define D_RAD2SPHI(rad) ((rad) * D_MAX_INT_OVER_PI)
#define D_DEG2SPHI(deg) ((deg) * D_MAX_INT_OVER_180)

//////////////////////////////////////////////////////////////////////////
/// Spherical (integer)
FUNC_DEF_EXTERN void FUNC_CALL_TYPE sphi2DecartXZ(float4 dec, int32 lon, int32 lat, float radius);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE decart2SphiXZ(float4 dec, int32 * pLon, int32 * pLat, float * pRadius);

#endif
