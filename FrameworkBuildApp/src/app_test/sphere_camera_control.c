
#include <math.h>

#include "../app_test/camera.h"
#include "../app_test/sphere_camera_control.h"
#include "../app_test/pad.h"

#include <math_v7.h>

typedef struct _SphereCameraControlCache {

	vec4 decartPos;

	int32 longitude;
	int32 latitude;
	float altitude;
	float radius;

	float pitch; /// Pitch angle in degrees
	float heading; /// Heading angle in degrees

} SphereCameraControlCache;

static SphereCameraControlCache s_CONTROL_CACHE;

void sphereCameraControlInitialize(float radius) {

	s_CONTROL_CACHE.altitude = 0.0f;
	s_CONTROL_CACHE.radius = radius;
	s_CONTROL_CACHE.heading = 0.0f;
	s_CONTROL_CACHE.latitude = 0;
	s_CONTROL_CACHE.longitude = 0;
	s_CONTROL_CACHE.pitch = 0.0f;
}

void sphereCameraControlUpdate(void * param) {

	CameraControl * test = (CameraControl *)param;

	vec4 target = {0.0f, 0.0f, 0.0f, 1.0f};
	vec4 up = {0.0f, 1.0f, 0.0f, 1.0f};

	int32 step = (int32)(65536.0f * 150.0f);

	if (g_PAD.state & E_PAD_BUTTON_A) {
		s_CONTROL_CACHE.longitude -= step;
	}
	if (g_PAD.state & E_PAD_BUTTON_B) {
		s_CONTROL_CACHE.longitude += step;
	}
	if (g_PAD.state & E_PAD_BUTTON_C) {
		if (D_SPHI2DEG(s_CONTROL_CACHE.latitude) > -85.0f) {
			s_CONTROL_CACHE.latitude -= step;
		}
	}
	if (g_PAD.state & E_PAD_BUTTON_D) {
		if (D_SPHI2DEG(s_CONTROL_CACHE.latitude) < 85.0f) {
			s_CONTROL_CACHE.latitude += step;
		}
	}
	if (g_PAD.state & E_PAD_BUTTON_X) {
		s_CONTROL_CACHE.radius -= s_CONTROL_CACHE.radius * 0.005f;
	}
	if (g_PAD.state & E_PAD_BUTTON_Y) {
		s_CONTROL_CACHE.radius += s_CONTROL_CACHE.radius * 0.005f;
	}

	if (g_PAD.state != E_PAD_RELEASED)
	{
		sphi2DecartXZ(s_CONTROL_CACHE.decartPos
					, s_CONTROL_CACHE.longitude
					, s_CONTROL_CACHE.latitude
					, s_CONTROL_CACHE.radius);

// 		{
// 			int32 lon;
// 			int32 lat;
// 			float radius;
// 
// 			decart2GeoXZ(s_CONTROL_CACHE.decartPos, &lon, &lat, &radius);
// 
// 			printf(" Lon %d		 Lat %d \n", s_CONTROL_CACHE.longitude - lon, s_CONTROL_CACHE.latitude - lat );
// 
// 			lon = lon;
// 		}
	}

	cameraSetView(&test->camera, s_CONTROL_CACHE.decartPos, target, up);
	cameraSetProj(&test->camera, test->viewSize[0] / test->viewSize[1], s_CONTROL_CACHE.radius / 10.f, 10.0f + s_CONTROL_CACHE.radius * 2.0f, 60.0f);
	
	{
		//float world2PixelScale = 1.0f;//(1.0f / s_CONTROL_CACHE.radius) * (1.0f / tanf(D_DEG2RAD(test->camera.fov * 0.5f))) * (test->viewSize[1] * 0.5f);
		//float pixel2WorldScale = (s_CONTROL_CACHE.radius * test->camera.fov * test->camera.aspect) / test->viewSize[0];
// 		float world2PixelScale = (1.0f / s_CONTROL_CACHE.radius) * (1.0f / tanf(D_DEG2RAD(test->camera.fov * test->camera.aspect * 0.5f))) * (test->viewSize[0] * 0.5f);
//		 float pixel2WorldScale = 1.0f / world2PixelScale;
		float world2PixelScale = (1.0f / s_CONTROL_CACHE.radius) * (1.0f / tanf(D_DEG2RAD(test->camera.fov * 0.5f))) * (test->viewSize[1] * 0.5f);
		float pixel2WorldScale = 1.0f / world2PixelScale;
		cameraSetScale(&test->camera, world2PixelScale, pixel2WorldScale);
	}


	/// Right one, needed from proper scale calculation
// 	{
// 		float radius;
// 		float z = s_CONTROL_CACHE.altitude - D_EARTH_RADIUS;
// 		radius = (D_EARTH_RADIUS / z) * (1.0f / deadlyTanEx(D_DEG2RAD(test->camera.fov * 0.5f))) * (test->viewSize[1] * 0.5f);
// 
// 		//printf("radius1 %4.8f \n", radius);
// 	}
}