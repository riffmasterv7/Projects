#ifndef __CAMERA__
#define __CAMERA__

#include <types_v7.h>

typedef struct _Frustum {
	vec4 point[8];
	vec4 normal[6];
} Frustum;

typedef struct _Camera {
	/// Inverted view matrix
	mtx worldMtx;
	mtx viewMtx;
	mtx projMtx;
	mtx viewProjMtx;
	mtx normalMtx;

	vec4 position;
	vec4 target;
	vec4 up;

	float aspect;
	float znear;
	float zfar;
	float fov;

	float world2pixelScale;
	float pixel2worldScale;

} Camera;

typedef void (* FP_cameraControl)(void * data);

typedef struct _CameraControl {

	Camera camera;
	FP_cameraControl control;
	float viewSize[2];

} CameraControl;

void cameraInitialize(Camera * camera);
void cameraUpdate(Camera * camera);
void cameraSetView(Camera * camera, float4 pos, float4 target, float4 up);
void cameraSetProj(Camera * camera, float aspect, float znear, float zfar, float fov);
void cameraSetScale(Camera * camera, float world2pixel, float pixel2world);


#endif /**/