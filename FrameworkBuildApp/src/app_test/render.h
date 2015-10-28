#ifndef __RENDER__
#define __RENDER__

#include <types_v7.h>

typedef struct _Vertex_p3f_c4b		Vertex_p3f_c4b;
typedef struct _Vertex_p2f			Vertex_p2f;
typedef struct _Vertex_p2f_t2f		Vertex_p2f_t2f;
typedef struct _MicroCityGroup		MicroCityGroup;
typedef struct _PointGroup		PointGroup;

typedef struct _CameraControl		CameraControl;

int32 rdrCreateShaders();
void rdrDestroyShaders();

void rdrDrawCityGroup(CameraControl * camCtrl, MicroCityGroup * cityGrp);

void rdrDrawPointGroup(CameraControl * camCtrl, PointGroup * pointGrp);

void rdrSetShader_p2f_screen();
void rdrSetColor_p2f_screen(float color[3]);
void rdrDrawArrays_p2f_screen(Vertex_p2f * pVertex, uint32 vertexCount, uint32 primitive);

void rdrSetShader_p2f_t2f_screen();
void rdrSetAlpha_p2f_t2f_screen(float alpha);
void rdrDrawArrays_p2f_t2f_screen(const Vertex_p2f_t2f * pVertex, uint32 vertexCount, uint32 primitive, float alpha);
void rdrDraw_p3f_c4b(CameraControl * pCamCtrl, const Vertex_p3f_c4b * pVertex, uint32 vertexCount, uint32 primitive);

void rdrDrawArrays_fxaa(const Vertex_p2f_t2f * pVertex, uint32 vertexCount, uint32 primitive, float rcpX, float rcpY);

#endif /**/