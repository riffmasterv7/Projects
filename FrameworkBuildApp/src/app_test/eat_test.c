
#include <gfx.h>


#include <math_v7.h>

#include "../app_test/test.h"
#include "../app_test/pad.h"
#include "../app_test/model.h"
#include "../app_test/render.h"


//////////////////////////////////////////////////////////////////////////

static const Vertex_EAT_TEST s_EAT_TEST_VERTEX_ARRAY_0[] = {
	{ {-1.0f, 0.5f},	{ 0, 0, 0 } },
	{ {-1.0f, -0.5f},	{ 0, 0, 0 } },
	{ {-0.8f, 0.5f},	{ 0, 0, 1 } },
	{ {-0.8f, -0.5f},	{ 0, 0, 1 } },
	{ {-0.8f, 0.5f},	{ 0, 1, 0 } },
	{ {-0.8f, -0.5f},	{ 0, 1, 0 } },
	{ {-0.6f, 0.5f},	{ 0, 1, 1 } },
	{ {-0.6f, -0.5f},	{ 0, 1, 1 } },
	{ {-0.6f, 0.5f},	{ 0, 2, 0 } },
	{ {-0.6f, -0.5f},	{ 0, 2, 0 } },
	{ {-0.4f, 0.5f},	{ 0, 2, 1 } },
	{ {-0.4f, -0.5f},	{ 0, 2, 1 } },

	{ {-0.4f, 0.5f},	{ 1, 0, 0 } },
	{ {-0.4f, -0.5f},	{ 1, 0, 0 } },
	{ {-0.2f, 0.5f},	{ 1, 0, 1 } },
	{ {-0.2f, -0.5f},	{ 1, 0, 1 } },
	{ {-0.2f, 0.5f},	{ 1, 1, 0 } },
	{ {-0.2f, -0.5f},	{ 1, 1, 0 } },
	{ { 0.0f, 0.5f},	{ 1, 1, 1 } },
	{ { 0.0f, -0.5f},	{ 1, 1, 1 } },

	{ { 0.0f, 0.5f},	{ 2, 0, 0 } },
	{ { 0.0f, -0.5f},	{ 2, 0, 0 } },
	{ { 0.2f, 0.5f},	{ 2, 0, 1 } },
	{ { 0.2f, -0.5f},	{ 2, 0, 1 } }
};

static const uint32 s_EAT_TEST_VERTEX_0_COUNT = sizeof(s_EAT_TEST_VERTEX_ARRAY_0) / sizeof(s_EAT_TEST_VERTEX_ARRAY_0[0]);

static const uint8 s_EAT_TEST_INDEX_ARRAY_0[] = {
	0, 1, 2, 3,  3, 4,  
	4, 5, 6, 7,  7, 8,
	8, 9,10,11, 11,12,

   12,13,14,15, 15,16,
   16,17,18,19, 19,20,

   20,21,22,23//, 23,24,
};

static const uint32 s_EAT_TEST_INDEX_0_COUNT = sizeof(s_EAT_TEST_INDEX_ARRAY_0) / sizeof(s_EAT_TEST_INDEX_ARRAY_0[0]);

static ElementArray s_EAT_TEST_ELEMENT_ARRAY_0 = {
	0, GL_TRIANGLE_STRIP, sizeof(s_EAT_TEST_INDEX_ARRAY_0) / sizeof(s_EAT_TEST_INDEX_ARRAY_0[0]), GL_UNSIGNED_BYTE
};

// static const Vertex_p2f s_RAINBOW_STRIP_VERTEX_ARRAY[] = {
// 	{ {-1.0f,  0.5f } },
// 	{ {-1.0f, -0.5f } },
//	 { {-0.3f,  0.5f } },
//	 { {-0.3f, -0.5f } },
// 
//	 { { 0.3f,  0.5f } },
//	 { { 0.3f, -0.5f } },
//	 { { 1.0f,  0.5f } },
//	 { { 1.0f, -0.5f } }
// };

#define D_SCALE 0.3f

// static const Vertex_p2f s_RAINBOW_STRIP_VERTEX_ARRAY[] = {
//	 { {-1.0f * D_SCALE,  0.5f * D_SCALE } },
//	 { {-1.0f * D_SCALE, -0.5f * D_SCALE } },
//	 { {-0.3f * D_SCALE,  0.5f * D_SCALE } },
//	 { {-0.3f * D_SCALE, -0.5f * D_SCALE } },
// 
//	 { { 0.3f * D_SCALE,  0.5f * D_SCALE } },
//	 { { 0.3f * D_SCALE, -0.5f * D_SCALE } },
//	 { { 1.0f * D_SCALE,  0.5f * D_SCALE } },
//	 { { 1.0f * D_SCALE, -0.5f * D_SCALE } }
// };

static const Vertex_p2f s_RAINBOW_STRIP_VERTEX_ARRAY[] = {
	{ {-1.0f * D_SCALE,  0.5f * D_SCALE } },
	{ {-1.0f * D_SCALE, -0.5f * D_SCALE } },
	{ { 1.0f * D_SCALE,  0.5f * D_SCALE } },
	{ { 1.0f * D_SCALE, -0.5f * D_SCALE } }
};

static const uint32 s_RAINBOW_STRIP_VERTEX_COUNT = sizeof(s_RAINBOW_STRIP_VERTEX_ARRAY) / sizeof(s_RAINBOW_STRIP_VERTEX_ARRAY[0]);

//////////////////////////////////////////////////////////////////////////

typedef struct _SElementArrayTestEntry {

	ElementArray * elementArray;
	uint32 elementsLimit;
	uint32 elementType;
	float color[3];

} SElementArrayTestEntry;

static SElementArrayTestEntry s_TEST_ENTRY_LIST[] = {
/*	x		 Limit		 Type					  y				 */
	{ 0,		 250, GL_UNSIGNED_BYTE, {64.0f, 0.0f, 0.0f} },
	{ 0,	  25000, GL_UNSIGNED_BYTE, {96.0f, 0.0f, 0.0f} },
	{ 0,	  50000, GL_UNSIGNED_BYTE, {128.0f, 0.0f, 0.0f} },
	{ 0,	  60000, GL_UNSIGNED_BYTE, {144.0f, 32.0f, 0.0f} },
	{ 0,	  62000, GL_UNSIGNED_BYTE, {144.0f, 96.0f, 0.0f} },
	{ 0,	  64000, GL_UNSIGNED_BYTE, {144.0f, 160.0f, 0.0f} },
	{ 0,	  65535, GL_UNSIGNED_BYTE, {160.0f, 0.0f, 0.0f} },
	{ 0,	  65538, GL_UNSIGNED_BYTE, {192.0f, 0.0f, 0.0f} },
	{ 0,	  66000, GL_UNSIGNED_BYTE, {255.0f, 0.0f, 0.0f} },

	{ 0,		 250, GL_UNSIGNED_SHORT, {0.0f, 64.0f, 0.0f} },
	{ 0,	  25000, GL_UNSIGNED_SHORT, {0.0f, 96.0f, 0.0f} },
	{ 0,	  50000, GL_UNSIGNED_SHORT, {0.0f, 128.0f, 0.0f} },
	{ 0,	  60000, GL_UNSIGNED_SHORT, {0.0f, 144.0f, 32.0f} },
	{ 0,	  62000, GL_UNSIGNED_SHORT, {0.0f, 144.0f, 96.0f} },
	{ 0,	  64000, GL_UNSIGNED_SHORT, {0.0f, 144.0f, 160.0f} },
	{ 0,	  65535, GL_UNSIGNED_SHORT, {0.0f, 160.0f, 0.0f} },
	{ 0,	  65538, GL_UNSIGNED_SHORT, {0.0f, 192.0f, 0.0f} },
	{ 0,	  66000, GL_UNSIGNED_SHORT, {0.0f, 255.0f, 0.0f} },

	{ 0,		 250, GL_UNSIGNED_INT, {0.0f, 0.0f, 64.0f} },
	{ 0,	  25000, GL_UNSIGNED_INT, {0.0f, 0.0f, 96.0f} },
	{ 0,	  50000, GL_UNSIGNED_INT, {0.0f, 0.0f, 128.0f} },
	{ 0,	  65535, GL_UNSIGNED_INT, {0.0f, 0.0f, 160.0f} },
	{ 0,	  65538, GL_UNSIGNED_INT, {0.0f, 0.0f, 192.0f} },
	{ 0,	  66000, GL_UNSIGNED_INT, {0.0f, 0.0f, 255.0f} },
};

static const uint32 s_TEST_ENTRY_COUNT = sizeof(s_TEST_ENTRY_LIST) / sizeof(s_TEST_ENTRY_LIST[0]);
static uint32 s_CURRENT_TEST_INDEX = 0;

void testBeginFrame() {

	glClear(GL_COLOR_BUFFER_BIT);
}

void testEndFrame() {

	//glFinish();
}

void testDrawFrame() {

// 	{
// 		rdrDrawArrays_p2f_screen(&s_RAINBOW_STRIP_VERTEX_ARRAY, s_RAINBOW_STRIP_VERTEX_COUNT, GL_TRIANGLE_STRIP);
// 	}

// 	{
// 		rdrDrawElements_p2f_screen(&s_RAINBOW_STRIP_VERTEX_ARRAY[0], s_TEST_ENTRY_LIST[s_CURRENT_TEST_INDEX].elementArray);
// 	}
	float lprFrom[3] = { 0.0f, 0.0f, 0.02f };
	float lprTo[3] = { 2.0f, 0.0f, 0.05f };

	rdrDrawElements_EAT_TEST(&s_EAT_TEST_VERTEX_ARRAY_0[0], &s_EAT_TEST_ELEMENT_ARRAY_0, lprFrom, lprTo);
}

void testUpdate() {

	static uint32 pressed = 0;

	padUpdate();

	if (g_PAD.state & E_PAD_BUTTON_A)
	{
		pressed = 1;
	}

	if (pressed && (g_PAD.state == E_PAD_RELEASED))
	{
		++s_CURRENT_TEST_INDEX;
		s_CURRENT_TEST_INDEX %= s_TEST_ENTRY_COUNT;
		pressed = 0;
		rdrSetColor_p2f_screen(s_TEST_ENTRY_LIST[s_CURRENT_TEST_INDEX].color);
		printf("\nPass %d Type %d  Elements %d\n", 
			s_CURRENT_TEST_INDEX, 
			s_TEST_ENTRY_LIST[s_CURRENT_TEST_INDEX].elementType,
			s_TEST_ENTRY_LIST[s_CURRENT_TEST_INDEX].elementArray->count);
	}
}

int32 testInitialize(int32 width, int32 height) {

	vec4 viewPort = {0.0f, 0.0f, width, height};

	padInitialize(0);

	glViewport(0, 0, width, height);


#if defined (D_GFX_OPENGL)
	glewInit();
	glClearDepth(1.0f);
#elif defined (D_GFX_OPENGL_ES_2)
	glClearDepthf(1.0f);
#endif

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glDepthMask(GL_FALSE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);

	if (!rdrCreateShaders()) {

		return 0;
	}

	rdrSetShader_p2f_screen();
	rdrSetColor_p2f_screen(s_TEST_ENTRY_LIST[s_CURRENT_TEST_INDEX].color);
	//glReleaseShaderCompiler();

	{
		uint32 entry = 0;
		for (; entry < s_TEST_ENTRY_COUNT; ++entry) {
			mdlCreateElementArray(
				  s_RAINBOW_STRIP_VERTEX_COUNT
				, GL_TRIANGLE_STRIP
				, s_TEST_ENTRY_LIST[entry].elementsLimit
				, s_TEST_ENTRY_LIST[entry].elementType
				, &s_TEST_ENTRY_LIST[entry].elementArray
				);
		}
	}

	s_EAT_TEST_ELEMENT_ARRAY_0.data = &s_EAT_TEST_INDEX_ARRAY_0[0];

	/*
	{
		vec4 position = {10.0f, 20.0f, 30.0f, 1.0f};
		vec4 scale = { 10.0f, 10.0f, 10.0f, 1.0f };
		vec4 axis = { 0.7f, 2.0f, 3.0f, 1.0f };

		mtx translateMtx;
		mtx rotateMtx;
		mtx scaleMtx;

		mtx tmp;
		mtx res;
		float x;

		vecNormalize(axis, axis);
		axis[3] = 1.0f;
		mtxMakeScaling(scaleMtx, scale);
		mtxMakeTranslation(translateMtx, position);
		mtxMakeRotationAxis(rotateMtx, axis, D_DEG2RAD(90.0f));
		rotateMtx[3][3] = 1.0f;

		mtxMul(tmp, scaleMtx, rotateMtx);
		mtxMul(res, tmp, translateMtx);
		x = 0.0f;
	}
	*/

	return 1;
}

void testDeinitialize() {

	padDeinitialize();

	rdrDestroyShaders();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glDisableVertexAttribArray(7);

	{
		uint32 entry = 0;
		for (; entry < s_TEST_ENTRY_COUNT; ++entry) {
			mdlDestroyElementArray(&s_TEST_ENTRY_LIST[entry].elementArray);
		}
	}
}

