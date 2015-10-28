
#include "../app_test/camera.h"

#include <math_v7.h>

void cameraInitialize(Camera * camera) {
	mtxUnit(camera->worldMtx);
	mtxUnit(camera->viewMtx);
	mtxUnit(camera->viewProjMtx);
	mtxUnit(camera->projMtx);
	mtxUnit(camera->normalMtx);

	vecZero4(camera->target);
	vecZero4(camera->position);
	vecZero4(camera->up);

	camera->position[0] = 1.0f;
	camera->position[1] = 1.0f;
	camera->position[2] = 1.0f;

	camera->aspect = 0.0f;
	camera->fov = 0.0f;
	camera->zfar = 0.0f;
	camera->znear = 0.0f;
}

void cameraUpdate(Camera * camera) {

	mtxMakeLookAtRH(camera->viewMtx, camera->position, camera->target, camera->up);
	mtxMakePerspectiveRH(camera->projMtx, D_DEG2RAD(camera->fov), camera->aspect, camera->znear, camera->zfar);

	mtxMul(camera->viewProjMtx, camera->viewMtx, camera->projMtx);
	mtxInvertFull(camera->worldMtx, camera->viewMtx);

	mtxCopy(camera->normalMtx, camera->viewMtx);

	vecZero4(camera->normalMtx[3]);
	camera->normalMtx[0][3] = 0.0f;
	camera->normalMtx[1][3] = 0.0f;
	camera->normalMtx[2][3] = 0.0f;

	camera->up[0] = camera->viewMtx[0][1];
	camera->up[1] = camera->viewMtx[1][1];
	camera->up[2] = camera->viewMtx[2][1];
	camera->up[3] = 1.0f;
}

void cameraSetView(Camera * camera, float4 pos, float4 target, float4 up) {
	vecCopy(camera->position, pos);
	vecCopy(camera->target, target);
	vecCopy(camera->up, up);
}

void cameraSetProj(Camera * camera, float aspect, float znear, float zfar, float fov) {
	camera->fov = fov;
	camera->aspect = aspect;
	camera->znear = znear;
	camera->zfar = zfar;
}

void cameraSetScale(Camera * camera, float world2pixel, float pixel2world) {

	camera->world2pixelScale = world2pixel;
	camera->pixel2worldScale = pixel2world;
}

