
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <math_v7.h>
#include <buffer.h>

#include "../app_test/model.h"

uint32 testAABBPlaneIntesectBehind(float4 plane, float4 boxMinP, float4 boxMaxP) {
	
	vec4 center;
	vec4 extent;
	vec4 absPlane;
	float e;
	float s;

	vecAdd(center, boxMaxP, boxMinP);
	vecSub(extent, boxMaxP, boxMinP);
	vecScale(center, center, 0.5f);
	vecScale(extent, extent, 0.5f);
	vecAbs(absPlane, plane);
	e = vecDot(absPlane, extent);
	s = vecDot(plane, center) + plane[3];

	if (s - e > 0.0f) {

		return 0;
	}

	return 1;
}

void mdlCreateCity(MicroCity * city, const Vertex_3f * pBuildingPosition, const Vertex_3f * pBuildingNormal, uint32 vertexCount) {

	int32 transparentVertexIndex = 0; 
	int32 opaqueVertexIndex = 0; 

	int32 overallVertexCount = 0;
	int32 overallBytes = 0;

	int32 transparentVertexCount = 0; 
	int32 opaqueVertexCount = 0; 

	int32 i = 0, j = 0, k = 0, h = 0;

	int32 iend = city->areaHeight / 2;
	int32 jend = city->areaWidth / 2;
	int32 istart = -iend;
	int32 jstart = -jend;
	//iend += 1;
	//jend += 1;

	//////////////////////////////////////////////////////////////////////////
	city->buildingCount = city->areaWidth * city->areaHeight;
	city->pHeight = (float *)malloc(sizeof(float) * city->buildingCount);
	srand((uint32)(pBuildingPosition));


	h = 0;
	for (i = istart; i < iend; ++i) {
		for (j = jstart; j < jend; ++j) {

			float xshift = (float)j;

			uint32 iheight = (uint32)rand() % (uint32)(city->maxBuildingHeight);
			city->pHeight[h++] = (float)iheight + city->minBuildingHeight;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	overallVertexCount = city->buildingCount * vertexCount;
	overallBytes = overallVertexCount * sizeof(Vertex_3f);

	//////////////////////////////////////////////////////////////////////////
	h = 0;
	for (i = istart; i < iend; ++i) {
		for (j = jstart; j < jend; ++j) {
			uint32 length = (uint32)(i * i + j * j);

			float height = city->pHeight[h++];

			if (length < city->transparencyRadius * city->transparencyRadius) {
				transparentVertexCount += vertexCount;
			} else {
				opaqueVertexCount += vertexCount;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	{
		uint32 opaqueBuildingBytes = sizeof(Vertex_3f) * opaqueVertexCount;
		uint32 transparentBuildingBytes = sizeof(Vertex_3f) * transparentVertexCount;

		city->opaque.vertexCount = opaqueVertexCount;
		city->opaque.pPosition = (Vertex_3f *)malloc(opaqueBuildingBytes);
		city->opaque.pNormal = (Vertex_3f *)malloc(opaqueBuildingBytes);

		if (transparentBuildingBytes) {
			city->transparent.vertexCount = transparentVertexCount;
			city->transparent.pPosition = (Vertex_3f *)malloc(transparentBuildingBytes);
			city->transparent.pNormal = (Vertex_3f *)malloc(transparentBuildingBytes);
		}

		h = 0;
		for (i = istart; i < iend; ++i) {

			float zshift = (float)i;
			for (j = jstart; j < jend; ++j) {

				float xshift = (float)j;
				float height = city->pHeight[h++];
				uint32 length = (uint32)(i * i + j * j);

				if (length < city->transparencyRadius * city->transparencyRadius) {

					//height *= city->transparencyHeightMult;

					for (k = 0; k < vertexCount; ++k) {
						city->transparent.pPosition[transparentVertexIndex + k].attrib[0] = pBuildingPosition[k].attrib[0] + xshift;
						city->transparent.pPosition[transparentVertexIndex + k].attrib[1] = pBuildingPosition[k].attrib[1] * height;
						city->transparent.pPosition[transparentVertexIndex + k].attrib[2] = pBuildingPosition[k].attrib[2] + zshift;

						city->transparent.pNormal[transparentVertexIndex + k].attrib[0] = pBuildingNormal[k].attrib[0];
						city->transparent.pNormal[transparentVertexIndex + k].attrib[1] = pBuildingNormal[k].attrib[1];
						city->transparent.pNormal[transparentVertexIndex + k].attrib[2] = pBuildingNormal[k].attrib[2];
					}
					transparentVertexIndex += vertexCount; 
				} else {

					for (k = 0; k < vertexCount; ++k) {
						city->opaque.pPosition[opaqueVertexIndex + k].attrib[0] = pBuildingPosition[k].attrib[0] + xshift;
						city->opaque.pPosition[opaqueVertexIndex + k].attrib[1] = pBuildingPosition[k].attrib[1] * height;
						city->opaque.pPosition[opaqueVertexIndex + k].attrib[2] = pBuildingPosition[k].attrib[2] + zshift;

						city->opaque.pNormal[opaqueVertexIndex + k].attrib[0] = pBuildingNormal[k].attrib[0];
						city->opaque.pNormal[opaqueVertexIndex + k].attrib[1] = pBuildingNormal[k].attrib[1];
						city->opaque.pNormal[opaqueVertexIndex + k].attrib[2] = pBuildingNormal[k].attrib[2];
					}
					opaqueVertexIndex += vertexCount; 
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////

		{
			city->opaque.positionBuffId = bufferCreate();
			bufferDynamicVertexData(city->opaque.positionBuffId, 0, overallBytes);

			city->opaque.normalBuffId = bufferCreate();
			bufferDynamicVertexData(city->opaque.normalBuffId, 0, overallBytes);

			city->transparent.positionBuffId = bufferCreate();
			bufferDynamicVertexData(city->transparent.positionBuffId, 0, overallBytes);

			city->transparent.normalBuffId = bufferCreate();
			bufferDynamicVertexData(city->transparent.normalBuffId, 0, overallBytes);
		}

		//////////////////////////////////////////////////////////////////////////

		{
			if (opaqueBuildingBytes) {

				bufferVertexSubData(city->opaque.positionBuffId, city->opaque.pPosition, opaqueBuildingBytes, 0);
				bufferVertexSubData(city->opaque.normalBuffId, city->opaque.pNormal, opaqueBuildingBytes, 0);
			}

			if (transparentBuildingBytes) {

				bufferVertexSubData(city->transparent.positionBuffId, city->transparent.pPosition, transparentBuildingBytes, 0);
				bufferVertexSubData(city->transparent.normalBuffId, city->transparent.pNormal, transparentBuildingBytes, 0);
			}
		}
	}


	if (0) {

		/// Test

		uint32 buildingBytes = sizeof(Vertex_3f) * vertexCount;

		city->opaque.vertexCount = vertexCount;

		city->opaque.pPosition = (Vertex_3f *)malloc(buildingBytes);
		city->opaque.pNormal = (Vertex_3f *)malloc(buildingBytes);

		memcpy(city->opaque.pPosition, pBuildingPosition, buildingBytes);
		memcpy(city->opaque.pNormal, pBuildingNormal, buildingBytes);

		city->opaque.positionBuffId = bufferCreate();
		bufferStaticVertexData(city->opaque.positionBuffId, city->opaque.pPosition, buildingBytes);

		city->opaque.normalBuffId = bufferCreate();
		bufferStaticVertexData(city->opaque.normalBuffId, city->opaque.pNormal, buildingBytes);
	}
}

void mdlUpdateCity(MicroCity * city, const Vertex_3f * pBuildingPosition, const Vertex_3f * pBuildingNormal, uint32 vertexCount
					  , float4 transparencyPlane, float4 boxMinP, float4 boxMaxP) {

	int32 transparentVertexIndex = 0; 
	int32 opaqueVertexIndex = 0; 

	int32 transparentVertexCount = 0; 
	int32 opaqueVertexCount = 0; 

	int32 i = 0, j = 0, k = 0, h = 0;

	int32 iend = city->areaHeight / 2;
	int32 jend = city->areaWidth / 2;
	int32 istart = -iend;
	int32 jstart = -jend;
	//iend += 1;
	//jend += 1;

	//////////////////////////////////////////////////////////////////////////

	h = 0;
	for (i = istart; i < iend; ++i) {

		float zshift = (float)i;

		for (j = jstart; j < jend; ++j) {

			vec4 realBoxMinP;
			vec4 realBoxMaxP;
			float xshift = (float)j;
			uint32 length = (uint32)(i * i + j * j);
			float height = city->pHeight[h++];

			realBoxMinP[0] = boxMinP[0] + xshift;
			realBoxMinP[1] = boxMinP[1];
			realBoxMinP[2] = boxMinP[2] + zshift;

			realBoxMaxP[0] = boxMaxP[0] + xshift;
			realBoxMaxP[1] = boxMaxP[1] * height;
			realBoxMaxP[2] = boxMaxP[2] + zshift;

			if ( length < city->transparencyRadius * city->transparencyRadius 
			  || testAABBPlaneIntesectBehind(transparencyPlane, realBoxMinP, realBoxMaxP)
				) {

				transparentVertexCount += vertexCount;
			} else {

				opaqueVertexCount += vertexCount;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	{
		uint32 opaqueBuildingBytes = sizeof(Vertex_3f) * opaqueVertexCount;
		uint32 transparentBuildingBytes = sizeof(Vertex_3f) * transparentVertexCount;

		if (city->opaque.vertexCount != opaqueVertexCount) {

			if (city->opaque.vertexCount) {

				free(city->opaque.pPosition);
				free(city->opaque.pNormal);
				city->opaque.pPosition = 0;
				city->opaque.pNormal = 0;
			}

			city->opaque.vertexCount = opaqueVertexCount;

			if (opaqueBuildingBytes) {

				city->opaque.pPosition = (Vertex_3f *)malloc(opaqueBuildingBytes);
				city->opaque.pNormal = (Vertex_3f *)malloc(opaqueBuildingBytes);
			}
		}

		if (city->transparent.vertexCount != transparentVertexCount) {

			if (city->transparent.vertexCount) {

				free(city->transparent.pPosition);
				free(city->transparent.pNormal);
				city->transparent.pPosition = 0;
				city->transparent.pNormal = 0;
			}

			city->transparent.vertexCount = transparentVertexCount;

			if (transparentBuildingBytes) {

				city->transparent.pPosition = (Vertex_3f *)malloc(transparentBuildingBytes);
				city->transparent.pNormal = (Vertex_3f *)malloc(transparentBuildingBytes);
			}
		}

		h = 0;
		for (i = istart; i < iend; ++i) {

			float zshift = (float)i;

			for (j = jstart; j < jend; ++j) {

				vec4 realBoxMinP;
				vec4 realBoxMaxP;
				float xshift = (float)j;
				uint32 length = (uint32)(i * i + j * j);
				float height = city->pHeight[h++];

				realBoxMinP[0] = boxMinP[0] + xshift;
				realBoxMinP[1] = boxMinP[1];
				realBoxMinP[2] = boxMinP[2] + zshift;

				realBoxMaxP[0] = boxMaxP[0] + xshift;
				realBoxMaxP[1] = boxMaxP[1] * height;
				realBoxMaxP[2] = boxMaxP[2] + zshift;

				if (length < city->transparencyRadius * city->transparencyRadius 
				 || testAABBPlaneIntesectBehind(transparencyPlane, realBoxMinP, realBoxMaxP)
					) {

					//height *= city->transparencyHeightMult;

					for (k = 0; k < vertexCount; ++k) {

						city->transparent.pPosition[transparentVertexIndex + k].attrib[0] = pBuildingPosition[k].attrib[0] + xshift;
						city->transparent.pPosition[transparentVertexIndex + k].attrib[1] = pBuildingPosition[k].attrib[1] * height;
						city->transparent.pPosition[transparentVertexIndex + k].attrib[2] = pBuildingPosition[k].attrib[2] + zshift;

						city->transparent.pNormal[transparentVertexIndex + k].attrib[0] = pBuildingNormal[k].attrib[0];
						city->transparent.pNormal[transparentVertexIndex + k].attrib[1] = pBuildingNormal[k].attrib[1];
						city->transparent.pNormal[transparentVertexIndex + k].attrib[2] = pBuildingNormal[k].attrib[2];
					}
					transparentVertexIndex += vertexCount; 

				} else {

					for (k = 0; k < vertexCount; ++k) {

						city->opaque.pPosition[opaqueVertexIndex + k].attrib[0] = pBuildingPosition[k].attrib[0] + xshift;
						city->opaque.pPosition[opaqueVertexIndex + k].attrib[1] = pBuildingPosition[k].attrib[1] * height;
						city->opaque.pPosition[opaqueVertexIndex + k].attrib[2] = pBuildingPosition[k].attrib[2] + zshift;

						city->opaque.pNormal[opaqueVertexIndex + k].attrib[0] = pBuildingNormal[k].attrib[0];
						city->opaque.pNormal[opaqueVertexIndex + k].attrib[1] = pBuildingNormal[k].attrib[1];
						city->opaque.pNormal[opaqueVertexIndex + k].attrib[2] = pBuildingNormal[k].attrib[2];
					}
					opaqueVertexIndex += vertexCount; 
				}
			}
		}

		if (opaqueBuildingBytes) {

			bufferBindVertex(city->opaque.positionBuffId);
			bufferVertexSubData(city->opaque.positionBuffId, city->opaque.pPosition, opaqueBuildingBytes, 0);
			bufferBindVertex(city->opaque.normalBuffId);
			bufferVertexSubData(city->opaque.normalBuffId, city->opaque.pNormal, opaqueBuildingBytes, 0);
		}

		if (transparentBuildingBytes) {

			bufferBindVertex(city->transparent.positionBuffId);
			bufferVertexSubData(city->transparent.positionBuffId, city->transparent.pPosition, transparentBuildingBytes, 0);
			bufferBindVertex(city->transparent.normalBuffId);
			bufferVertexSubData(city->transparent.normalBuffId, city->transparent.pNormal, transparentBuildingBytes, 0);
		}

	}
}

void mdlDestroyCity(MicroCity * city) {

	if (city->pHeight) {
		free(city->pHeight);
	}

	if (city->opaque.pNormal) {
		free(city->opaque.pNormal);
	}

	if (city->opaque.pPosition) {
		free(city->opaque.pPosition);
	}

	bufferDelete(city->opaque.positionBuffId);
	bufferDelete(city->opaque.normalBuffId);

	if (city->transparent.pNormal) {
		free(city->transparent.pNormal);
	}

	if (city->transparent.pPosition) {
		free(city->transparent.pPosition);
	}

	bufferDelete(city->transparent.positionBuffId);
	bufferDelete(city->transparent.normalBuffId);
}

//////////////////////////////////////////////////////////////////////////

void mdlCreatePointCity(BoxMicroCity * city)
{
	uint32 buildingCount = city->areaWidth * city->areaHeight;

	SPointVertex * vertices = new SPointVertex[buildingCount]; 

	city->points.vertexCount = buildingCount;

	float posX = -0.5f * (float)city->areaWidth;

	int32 pointIndex = 0;
	for (int32 i = 0; i < city->areaWidth; ++i)
	{
		float posZ = -0.5f * (float)city->areaHeight;

		for (int32 j = 0; j < city->areaHeight; ++j)
		{
			uint32 iheight = (uint32)rand() % (uint32)(city->maxBuildingHeight) + city->minBuildingHeight; 
			SPointVertex& vertex = vertices[pointIndex++];

			vertex.position[0] = posX;
			vertex.position[1] = 0.0f;
			vertex.position[2] = posZ;
			vertex.position[3] = (float)iheight;

			posZ += 1.0f;
		}

		posX += 1.0f;
	}

	city->points.vertexBuffer = new CPointVertexBuffer(vertices, sizeof(SPointVertex) * buildingCount);
	
	city->points.vertexSetup = new CVertexSetup;
	city->points.vertexSetup->init(city->points.vertexBuffer, 0);

	delete [] vertices;

	vecCopy(city->points.normal, g_VEC4_AXIS_Y);
	vecCopy(city->points.tangent, g_VEC4_AXIS_X);
	vecCopy(city->points.bitangent, g_VEC4_AXIS_Z);
	city->points.width = 1.0f;
}

void mdlDestroyPointCity(BoxMicroCity * city)
{
	delete city->points.vertexSetup;
	delete city->points.vertexBuffer;
}


//////////////////////////////////////////////////////////////////////////

void mdlCreateSphere(float radius, float vertexSpacing, Vertex_p3f_c4b ** ppVertices, uint32 * pVertexCount) {

	int32 n;
	float a;
	float b;
	n = 0;

	*pVertexCount = (uint32)( (180.0f / vertexSpacing) * (360.0f / vertexSpacing) * 4.0f);
	*ppVertices = (Vertex_p3f_c4b *)malloc(sizeof(Vertex_p3f_c4b) * (*pVertexCount));

	for (b = 0.0f; b <= 180.0f - vertexSpacing; b += vertexSpacing){

		for (a = 0.0f; a <= 360.0f - vertexSpacing; a += vertexSpacing){

			(*ppVertices)[n].position[0] = radius * fSin((a) / 180.0f * D_PI) * fSin((b) / 180.0f * D_PI);
			(*ppVertices)[n].position[2] = radius * fCos((a) / 180.0f * D_PI) * fSin((b) / 180.0f * D_PI);
			(*ppVertices)[n].position[1] = radius * fCos((b) / 180.0f * D_PI);
			(*ppVertices)[n].color[0]	 = (2 * b) / 360.0f * 255.0f;
			(*ppVertices)[n].color[1]	 = (a) / 360.0f * 255.0f;
			(*ppVertices)[n].color[2]	 = ((float)n / (float)(*pVertexCount));
			(*ppVertices)[n].color[3]	= 255;

			n++;

			(*ppVertices)[n].position[0] = radius * fSin((a) / 180.0f * D_PI) * fSin((b + vertexSpacing) / 180.0f * D_PI);
			(*ppVertices)[n].position[2] = radius * fCos((a) / 180.0f * D_PI) * fSin((b + vertexSpacing) / 180.0f * D_PI);
			(*ppVertices)[n].position[1] = radius * fCos((b + vertexSpacing) / 180.0f * D_PI);
			(*ppVertices)[n].color[0]	 = (2 * (b + vertexSpacing)) / 360.0f * 255.0f;
			(*ppVertices)[n].color[1]	 = (a) / 360.0f * 255.0f;
			(*ppVertices)[n].color[2]	 = ((float)n / (float)(*pVertexCount)) / 255.0f;
			(*ppVertices)[n].color[3]	= 255;

			n++;

			(*ppVertices)[n].position[0]	= radius * fSin((a + vertexSpacing) / 180.0f * D_PI) * fSin((b) / 180.0f * D_PI);
			(*ppVertices)[n].position[2]	= radius * fCos((a + vertexSpacing) / 180.0f * D_PI) * fSin((b) / 180.0f * D_PI);
			(*ppVertices)[n].position[1]	= radius * fCos((b) / 180.0f * D_PI);
			(*ppVertices)[n].color[0]		= (2 * b) / 360.0f * 255.0f;
			(*ppVertices)[n].color[1]		= (a + vertexSpacing) / 360.0f * 255.0f;
			(*ppVertices)[n].color[2]	 = ((float)n / (float)(*pVertexCount)) / 255.0f;
			(*ppVertices)[n].color[3]	= 255;

			n++;

			(*ppVertices)[n].position[0]	= radius * fSin((a + vertexSpacing) / 180.0f * D_PI) * fSin((b + vertexSpacing) / 180.0f * D_PI);
			(*ppVertices)[n].position[2]	= radius * fCos((a + vertexSpacing) / 180.0f * D_PI) * fSin((b + vertexSpacing) / 180.0f * D_PI);
			(*ppVertices)[n].position[1]	= radius * fCos((b + vertexSpacing) / 180.0f * D_PI);
			(*ppVertices)[n].color[0]		= (2 * (b + vertexSpacing)) / 360.0f * 255.0f;
			(*ppVertices)[n].color[1]		= (a + vertexSpacing) / 360.0f * 255.0f;
			(*ppVertices)[n].color[2]	 = ((float)n / (float)(*pVertexCount)) / 255.0f;
			(*ppVertices)[n].color[3]	= 255;

			n++;
		}
	}
}
