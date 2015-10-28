#ifndef __TERRAIN_MODEL__
#define __TERRAIN_MODEL__

#include <types_v7.h>
#include "CPointVertexBuffer.h"

//////////////////////////////////////////////////////////////////////////

typedef struct _Vertex_p3f_c4b {
	float position[3];
	uint8 color[4];
} Vertex_p3f_c4b;

typedef struct _Vertex_p2f {
	float position[2];
} Vertex_p2f;

typedef struct _Vertex_p2f_t2f {
	float position[2];
	float texcoord[2];
} Vertex_p2f_t2f;

typedef struct _Vertex_3f {
	float attrib[3];
} Vertex_3f;

typedef struct _Vertex4f {
	float attrib[4];
} Vertex4f;

typedef struct _MicroCityGroup {
	Vertex_3f * pPosition;
	Vertex_3f * pNormal;
	uint32 vertexCount;
	uint32 positionBuffId;
	uint32 normalBuffId;
} MicroCityGroup;

typedef struct _MicroCity {

	MicroCityGroup opaque;
	MicroCityGroup transparent;
	
	float minBuildingHeight;
	float maxBuildingHeight;
	float * pHeight;
	uint32 areaWidth;
	uint32 areaHeight;
	uint32 buildingCount;

	vec4 transparencyPlane;
	uint32 transparencyRadius;
	float transparencyAlpha;
	uint32 transparencyUsageFlags;
	float transparencyHeightMult;

} MicroCity;



typedef struct _PointGroup 
{
	SPointVertex * pVertices;
	CPointVertexBuffer * vertexBuffer;
	CVertexSetup * vertexSetup;

	uint32 vertexCount;

	vec4 normal;
	vec4 tangent;
	vec4 bitangent;
	float width;

} PointGroup;

typedef struct _BoxMicroCity 
{
	PointGroup points;

	float minBuildingHeight;
	float maxBuildingHeight;
	uint32 areaWidth;
	uint32 areaHeight;

} BoxMicroCity;

void mdlCreateSphere(float radius, float vertexSpacing, Vertex_p3f_c4b ** ppVertices, uint32 * pVertexCount);

void mdlCreateCity(MicroCity * city, const Vertex_3f * pBuildingPosition, const Vertex_3f * pBuildingNormal, uint32 vertexCount);
void mdlUpdateCity(MicroCity * city, const Vertex_3f * pBuildingPosition, const Vertex_3f * pBuildingNormal, uint32 vertexCount, float4 transparencyPlane, float4 minBoxP, float4 maxBoxP);
void mdlDestroyCity(MicroCity * city);

void mdlCreatePointCity(BoxMicroCity * city);
void mdlDestroyPointCity(BoxMicroCity * city);

//////////////////////////////////////////////////////////////////////////

#endif/*__TERRAIN_MODEL__*/