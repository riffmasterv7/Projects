#include <math_v7.h>

#include "gles.h"

#include "../app_test/CCityDemo.h"
#include "../app_test/camera.h"

#include "../app_test/pad.h"
#include "../app_test/sphere_camera_control.h"
#include "../app_test/model.h"
#include "../app_test/render.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

static const Vertex_p2f s_P2F_STRIP_VERTEX_ARRAY[] = {
	{ {-1.0f,  1.0f } },
	{ {-1.0f, -1.0f } },
	{ { 1.0f,  1.0f } },
	{ { 1.0f, -1.0f } }
};

static const uint32 s_P2F_STRIP_VERTEX_COUNT = sizeof(s_P2F_STRIP_VERTEX_ARRAY) / sizeof(s_P2F_STRIP_VERTEX_ARRAY[0]);

//////////////////////////////////////////////////////////////////////////

static const Vertex_p2f_t2f s_P2F_T2F_STRIP_VERTEX_ARRAY[] = {
	{ {-1.0f,  1.0f }, { 0.0f, 1.0f } },
	{ {-1.0f, -1.0f }, { 0.0f, 0.0f } },
	{ { 1.0f,  1.0f }, { 1.0f, 1.0f } },
	{ { 1.0f, -1.0f }, { 1.0f, 0.0f } }
};

static const uint32 s_P2F_T2F_STRIP_VERTEX_COUNT = sizeof(s_P2F_T2F_STRIP_VERTEX_ARRAY) / sizeof(s_P2F_T2F_STRIP_VERTEX_ARRAY[0]);

//////////////////////////////////////////////////////////////////////////

// cube ///////////////////////////////////////////////////////////////////////
//	 v6----- v5
//	/|		/|
//  v1------v0|
//  | |	  | |
//  | |v7---|-|v4
//  |/		|/
//  v2------v3

static const Vertex_p3f_c4b s_XZ_PLANE_STRIP_VERTEX_ARRAY[] = {

	{	{-100.0f, 0.0f, -100.0f	},	{ 200, 127, 127, 255}  },
	{	{-100.0f, 0.0f,  100.0f	},	{ 127, 200, 127, 255}  },
	{	{ 100.0f, 0.0f, -100.0f	},	{ 127, 127, 200, 255}  },
	{	{ 100.0f, 0.0f,  100.0f	},	{ 180, 150, 180, 255}  }
};

static const int32 s_XZ_PLANE_STRIP_VERTEX_COUNT = 4;


static const Vertex_3f s_HOUSE_VERTICES[] = {	

	{0.5f, 1.0f, 0.5f}, {-0.5f, 1.0f, 0.5f},  {-0.5f, 0.0f, 0.5f},  {0.5f, 1.0f, 0.5f}, {-0.5f, 0.0f, 0.5f}, {0.5f, 0.0f, 0.5f},		  // v0-v1-v2-v3

	{0.5f, 1.0f, 0.5f},  {0.5f, 0.0f, 0.5f},	{0.5f, 0.0f,-0.5f}, {0.5f, 1.0f, 0.5f}, {0.5f, 0.0f,-0.5f}, {0.5f, 1.0f,-0.5f},		  // v0-v3-v4-v5

	{0.5f, 1.0f, 0.5f},  {0.5f, 1.0f,-0.5f},  {-0.5f, 1.0f,-0.5f}, {0.5f, 1.0f, 0.5f}, {-0.5f, 1.0f,-0.5f}, {-0.5f, 1.0f, 0.5f},		  // v0-v5-v6-v1

	{-0.5f, 1.0f, 0.5f}, {-0.5f, 1.0f,-0.5f},  {-0.5f, 0.0f,-0.5f}, {-0.5f, 1.0f, 0.5f},  {-0.5f, 0.0f,-0.5f}, {-0.5f, 0.0f, 0.5f},	 // v1-v6-v7-v2

	{0.5f, 0.0f,-0.5f}, {-0.5f, 0.0f,-0.5f},  {-0.5f, 1.0f,-0.5f}, { 0.5f, 0.0f,-0.5f}, {-0.5f, 1.0f,-0.5f}, {0.5f, 1.0f,-0.5f} };	// v4-v7-v6-v5

// normal array
static const Vertex_3f s_HOUSE_NORMALS[] = {{0,0,1},  {0,0,1},  {0,0,1}, {0,0,1},{0,0,1}, {0,0,1},				 // v0-v1-v2-v3
{1,0,0},  {1,0,0},  {1,0,0}, {1,0,0}, {1,0,0},{1,0,0},				 // v0-v3-v4-v5
{0,1,0},  {0,1,0},  {0,1,0}, {0,1,0},	{0,1,0},{0,1,0},			  // v0-v5-v6-v1
{-1,0,0},  {-1,0,0}, {-1,0,0},  {-1,0,0},	{-1,0,0}, {-1,0,0},		  // v1-v6-v7-v2
{0,0,-1},  {0,0,-1},  {0,0,-1}, {0,0,-1},{0,0,-1}, {0,0,-1} };		  // v4-v7-v6-v5

static const uint32 s_HOUSE_VERTEX_COUNT = sizeof(s_HOUSE_VERTICES) / sizeof(s_HOUSE_VERTICES[0]);

//////////////////////////////////////////////////////////////////////////

static uint32 s_AUX_FBUFF_ID[2] = { 0, 0 };
static uint32 s_AUX_TEX_ID[2] = { 0, 0 };

static uint32 s_SCRN_FBUFF_ID = 0;
static uint32 s_SCRN_TEX_ID = 0;

static uint32 s_SHARED_RBUFF_DEPTH_ID = 0;

static vec4 s_VIEWPORT;
static vec4 s_TRANSPARENCY_PLANE;
static vec4 s_BOX_MIN_POINT = {-0.5f, 0.0f,-0.5f, 1.0f};
static vec4 s_BOX_MAX_POINT = { 0.5f, 1.0f, 0.5f, 1.0f};

CameraControl g_CAMERA_CONTROL;

MicroCity s_MICRO_CITY;

BoxMicroCity s_BOX_MICRO_CITY;

static int32 s_ENABLE_FXAA = 0;

extern CameraControl g_CAMERA_CONTROL;

namespace
{
	void utilUpdateTransparencyPlane(float4 plane, float4 up, float4 eyePos, float shift) {

		vec4 point = {0.0f, 0.0f, 0.0f, 1.0f};
		vec4 upVec;
		vec4 shiftVec;

		///vecCopy(point, eyePos);

		vecNormalize(upVec, up);
		vecScale(shiftVec, upVec, shift);
		vecAdd(point, shiftVec, point);

		vecCopy(plane, upVec);
		plane[3] = -vecDot(point, upVec);	
	}
}

CCityDemo::CCityDemo()
	: CDemo()
	, mGraphicsContext()
{
}

CCityDemo::~CCityDemo()
{

}

bool CCityDemo::create(void * nativeDisplayId, void * nativeWindowId, uint32 canvasWidth, uint32 canvasHeight)
{
	if (CGraphicsContext::createDisplay(nativeDisplayId, nativeWindowId, mGraphicsContext)
	 && CGraphicsContext::createContext(mGraphicsContext, 0) 
	   )
	{
		CGraphicsContext::activate(mGraphicsContext);

		vec4 viewPort = {0.0f, 0.0f, canvasWidth, canvasHeight};

		g_CAMERA_CONTROL.control = sphereCameraControlUpdate;
		g_CAMERA_CONTROL.viewSize[0] = canvasWidth;
		g_CAMERA_CONTROL.viewSize[1] = canvasHeight;

		sphereCameraControlInitialize(120.0f);
		cameraInitialize(&g_CAMERA_CONTROL.camera);

		padInitialize(0);

		vecCopy(s_VIEWPORT, viewPort);
		glViewport(0, 0, s_VIEWPORT[2], s_VIEWPORT[3]);

		glClearDepthf(1.0f);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

		glDisable(GL_CULL_FACE);
		//glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		glDepthMask(GL_TRUE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glDisable(GL_STENCIL_TEST);

		//glDisable(GL_BLEND);
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);
		glDisableVertexAttribArray(7);

		if (!rdrCreateShaders()) 
		{

			return false;
		}

		//////////////////////////////////////////////////////////////////////////

		glGenTextures(1, &s_SCRN_TEX_ID);
		glGenTextures(2, s_AUX_TEX_ID);
		glGenFramebuffers(1, &s_SCRN_FBUFF_ID);
		glGenFramebuffers(2, &s_AUX_FBUFF_ID[0]);
		glGenRenderbuffers(1, &s_SHARED_RBUFF_DEPTH_ID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_SCRN_TEX_ID);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_VIEWPORT[2], s_VIEWPORT[3], 0,	GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_AUX_TEX_ID[0]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_VIEWPORT[2], s_VIEWPORT[3], 0,	GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_AUX_TEX_ID[1]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_VIEWPORT[2], s_VIEWPORT[3], 0,	GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		//////////////////////////////////////////////////////////////////////////

		glBindRenderbuffer(GL_RENDERBUFFER, s_SHARED_RBUFF_DEPTH_ID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, s_VIEWPORT[2], s_VIEWPORT[3]);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		{
			GLenum status;

			glBindFramebuffer(GL_FRAMEBUFFER, s_AUX_FBUFF_ID[0]);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, s_SHARED_RBUFF_DEPTH_ID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_AUX_TEX_ID[0], 0);

			status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {

				return false;
			}

			//////////////////////////////////////////////////////////////////////////

			glBindFramebuffer(GL_FRAMEBUFFER, s_AUX_FBUFF_ID[1]);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, s_SHARED_RBUFF_DEPTH_ID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_AUX_TEX_ID[1], 0);

			status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {

				return false;
			}

			//////////////////////////////////////////////////////////////////////////

			glBindFramebuffer(GL_FRAMEBUFFER, s_SCRN_FBUFF_ID);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_SCRN_TEX_ID, 0);

			status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {

				return false;
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//////////////////////////////////////////////////////////////////////////
		/*
		s_MICRO_CITY.transparencyHeightMult = 0.67f;
		s_MICRO_CITY.minBuildingHeight = 1.0f;
		s_MICRO_CITY.maxBuildingHeight = 7.0f;
		s_MICRO_CITY.areaHeight = 70;
		s_MICRO_CITY.areaWidth = 70;
		s_MICRO_CITY.transparencyRadius = 12;
		s_MICRO_CITY.transparencyUsageFlags = 3;
		s_MICRO_CITY.transparencyAlpha = 0.41f;

		mdlCreateCity(&s_MICRO_CITY, s_HOUSE_VERTICES, s_HOUSE_NORMALS, s_HOUSE_VERTEX_COUNT);
		*/

		s_BOX_MICRO_CITY.minBuildingHeight = 1.0f;
		s_BOX_MICRO_CITY.maxBuildingHeight = 12.0f;
		s_BOX_MICRO_CITY.areaHeight = 2000;
		s_BOX_MICRO_CITY.areaWidth = 2000;

		mdlCreatePointCity(&s_BOX_MICRO_CITY);

		return true;
	}

	return false;
}

void CCityDemo::present()
{
	float line = 0.25f;

	padUpdate();

	g_CAMERA_CONTROL.control(&g_CAMERA_CONTROL);

	cameraUpdate(&g_CAMERA_CONTROL.camera);

	if (g_PAD.state == E_PAD_BUTTON_Z) 
	{
		s_ENABLE_FXAA = 1; 
	}

	if (g_PAD.state == E_PAD_BUTTON_W) 
	{
		s_ENABLE_FXAA = 0; 
	}

	line = (line * s_VIEWPORT[3] - s_VIEWPORT[3] * 0.5f) * g_CAMERA_CONTROL.camera.pixel2worldScale;

	utilUpdateTransparencyPlane(s_TRANSPARENCY_PLANE, g_CAMERA_CONTROL.camera.up, g_CAMERA_CONTROL.camera.position, line);
	
	/*
	mdlUpdateCity(&s_MICRO_CITY, s_HOUSE_VERTICES, s_HOUSE_NORMALS, s_HOUSE_VERTEX_COUNT
					, s_TRANSPARENCY_PLANE, s_BOX_MIN_POINT, s_BOX_MAX_POINT);
	*/

	glActiveTexture(GL_TEXTURE0);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glBindFramebuffer(GL_FRAMEBUFFER, s_AUX_FBUFF_ID[0]);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); /// 1
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	rdrDraw_p3f_c4b(&g_CAMERA_CONTROL, &s_XZ_PLANE_STRIP_VERTEX_ARRAY[0], s_XZ_PLANE_STRIP_VERTEX_COUNT, GL_TRIANGLE_STRIP);
	rdrDrawPointGroup(&g_CAMERA_CONTROL, &s_BOX_MICRO_CITY.points);  

	glBindFramebuffer(GL_FRAMEBUFFER, s_AUX_FBUFF_ID[1]);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f); /// 0
	glClear(GL_COLOR_BUFFER_BIT);

	//////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, s_SCRN_FBUFF_ID);
	//////////////////////////////////////////////////////////////////////////

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, s_AUX_TEX_ID[0]);
	rdrDrawArrays_p2f_t2f_screen(&s_P2F_T2F_STRIP_VERTEX_ARRAY[0], s_P2F_T2F_STRIP_VERTEX_COUNT, GL_TRIANGLE_STRIP, 1.0f);

	glBindTexture(GL_TEXTURE_2D, s_AUX_TEX_ID[1]);
	rdrDrawArrays_p2f_t2f_screen(&s_P2F_T2F_STRIP_VERTEX_ARRAY[0], s_P2F_T2F_STRIP_VERTEX_COUNT, GL_TRIANGLE_STRIP, s_MICRO_CITY.transparencyAlpha);

	//////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//////////////////////////////////////////////////////////////////////////

	glBindTexture(GL_TEXTURE_2D, s_SCRN_TEX_ID);
	if (s_ENABLE_FXAA) {
		rdrDrawArrays_fxaa(&s_P2F_T2F_STRIP_VERTEX_ARRAY[0], s_P2F_T2F_STRIP_VERTEX_COUNT, GL_TRIANGLE_STRIP, 1.0f / s_VIEWPORT[2], 1.0f / s_VIEWPORT[3]);
	} else {
		rdrDrawArrays_p2f_t2f_screen(&s_P2F_T2F_STRIP_VERTEX_ARRAY[0], s_P2F_T2F_STRIP_VERTEX_COUNT, GL_TRIANGLE_STRIP, 1.0f);
	}

	//////////////////////////////////////////////////////////////////////////
	/// Debug
	if (0)
	{	
		Vertex_p3f_c4b upVector[4];

		upVector[0].position[0] = 0.0f;
		upVector[0].position[1] = 0.0f;
		upVector[0].position[2] = 0.0f;
		upVector[0].color[0] = 255;
		upVector[0].color[1] = 0;
		upVector[0].color[2] = 0;
		upVector[0].color[3] = 255;

		upVector[1].position[0] = s_TRANSPARENCY_PLANE[0] * 10.0f;
		upVector[1].position[1] = s_TRANSPARENCY_PLANE[1] * 10.0f;
		upVector[1].position[2] = s_TRANSPARENCY_PLANE[2] * 10.0f;
		upVector[1].color[0] = 255;
		upVector[1].color[1] = 0;
		upVector[1].color[2] = 0;
		upVector[1].color[3] = 255;

		upVector[2].position[0] = 1.0f;
		upVector[2].position[1] = 0.0f;
		upVector[2].position[2] = 1.0f;
		upVector[2].color[0] = 0;
		upVector[2].color[1] = 255;
		upVector[2].color[2] = 0;
		upVector[2].color[3] = 255;

		upVector[3].position[0] = 1.0f;
		upVector[3].position[1] = 10.0f;
		upVector[3].position[2] = 1.0f;
		upVector[3].color[0] = 0;
		upVector[3].color[1] = 255;
		upVector[3].color[2] = 0;
		upVector[3].color[3] = 255;

		glLineWidth(3.0f);
		rdrDraw_p3f_c4b(&g_CAMERA_CONTROL, upVector, 4, GL_LINES);
		glLineWidth(1.0f);
	}

	CGraphicsContext::present(mGraphicsContext);
}

void CCityDemo::destroy()
{
	padDeinitialize();

	rdrDestroyShaders();

	mdlDestroyCity(&s_MICRO_CITY);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glDisableVertexAttribArray(7);

	glDeleteTextures(2, s_AUX_TEX_ID);
	glDeleteFramebuffers(2, s_AUX_FBUFF_ID);
	glDeleteTextures(1, &s_SCRN_TEX_ID);
	glDeleteFramebuffers(1, &s_SCRN_FBUFF_ID);

	glDeleteRenderbuffers(1, &s_SHARED_RBUFF_DEPTH_ID);

	CGraphicsContext::destroyContext(mGraphicsContext);
	CGraphicsContext::destroyDisplay(mGraphicsContext);
}

