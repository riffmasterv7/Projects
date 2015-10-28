
#include <app_test/test.h>

#include <gfx/gfx.h>
#include <gfx/shader.h>

#include <math/math.h>

typedef struct _SVertex {
	float position[3];
	uint8 color[4];
} SVertex;

static ShaderSource s_vsrc = {
	"precision highp float;\n"
	"uniform mat4 u_mvp;" \
	"attribute vec3 a_attrib0;" \
	"attribute vec4 a_attrib1;" \
	"varying vec4 v_color;" \
	"void main() { " \
	"gl_Position = u_mvp * vec4(a_attrib0, 1.0);" \
	"v_color = a_attrib1;"
	"}",
	0
};

static ShaderSource s_fsrc = {
	"precision highp float;\n"
	"varying vec4 v_color;" \
	"void main() { " \
	"gl_FragColor = v_color;" \
	"}",
	0
};

static ShaderSource s_vSrcOutline = {
	"precision highp float;\n"
	"uniform mat4 u_mvp;" \
	"attribute vec3 a_attrib0;" \
	"uniform float u_lineWidth;"\
	"varying vec2 v_position;"\
	"void main() { " \
	"gl_Position = u_mvp * vec4(a_attrib0, 1.0);" \
	"gl_PointSize = u_lineWidth;"\
	"v_position = gl_Position.xy;"\
	"}",
	0
};

static ShaderSource s_fSrcOutline = {
	"precision highp float;\n"
	"uniform vec4 u_viewPort;" \
	"uniform float u_lineWidth;"\
	"uniform vec4 u_color;" \
	"varying vec2 v_position;"\
	"void main() { " \
	"vec2 pos = (v_position.xy * gl_FragCoord.w + vec2(1.0)) * 0.5 * u_viewPort.zw + u_viewPort.xy;" \
	"vec2 delta = abs(pos - gl_FragCoord.xy);"\
	"float a = clamp(u_lineWidth * 0.5 - delta.x - delta.y, 0.0, 1.0);"\
	"gl_FragColor = vec4(u_color.rgb, u_color.a * a);" \
	"}",
	0
};

static SVertex s_vertices[] = {
	{ { 1.0f,  1.0f, 1.0f }, { 133, 0, 0, 255 } },
	{ { 1.0f, -1.0f, 1.0f }, { 133, 0, 0, 255 } },
	{ {-1.0f,  1.0f, 1.0f }, { 133, 0, 0, 255 } },
	{ {-1.0f, -1.0f, 1.0f }, { 133, 0, 0, 255 } },

	{ { 1.0f,  1.0f, -1.0f }, { 0, 0, 133, 255 } },
	{ { 1.0f, -1.0f, -1.0f }, { 0, 0, 133, 255 } },
	{ {-1.0f,  1.0f, -1.0f }, { 0, 0, 133, 255 } },
	{ {-1.0f, -1.0f, -1.0f }, { 0, 0, 133, 255 } }   
};

static uint8 s_indices[] = 
{
	0, 3, 1, 0, 2, 3, //FRONT
	4, 7, 5, 4, 6, 7, //BACK
	4, 1, 5, 4, 0, 1, //RIGHT
	2, 7, 3, 2, 6, 7, //LEFT
	4, 2, 0, 4, 6, 2, //TOP
	1, 7, 5, 1, 3, 7  //BOTTOM
};

static uint8 s_outlineIndices[] = 
{
	0, 1, 0, 2, 2, 3, 3, 1,
	4, 5, 4, 6, 6, 7, 7, 5,
	0, 4, 2, 6, 3, 7, 1, 5
};

static uint8 s_outlinePointIndices[] = 
{
	0, 1, 2, 3, 4, 5, 6, 7
};

static uint8 s_vertexCount = sizeof(s_vertices) / sizeof(SVertex);
static uint8 s_indexCount = sizeof(s_indices) / sizeof(uint8);

static ShaderId s_modelShaderId = 0;
static int32 s_modelMVPUniformId = 0;

static ShaderId s_outlineShaderId = 0;
static int32 s_outlineMVPUniformId = 0;
static int32 s_outlineColorUniformId = 0;
static int32 s_outlineLineWidthUniformId = 0;
static int32 s_outlineViewPortUniformId = 0;
static uint8 s_outlineIndexCount = sizeof(s_outlineIndices) / sizeof(uint8);
static uint8 s_outlinePointIndexCount = sizeof(s_outlinePointIndices) / sizeof(uint8);
static float s_outlineWidth = 5.0f;

//#define D_TEST_VBO_LEAK
#if defined(D_TEST_VBO_LEAK)

static int32 s_buffers[4] = {0};

void createVBOs() {

	glGenBuffers(4, &s_buffers[0]);

	if (s_buffers[0]) {
		glBindBuffer(GL_ARRAY_BUFFER, s_buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, s_vertexCount * sizeof(SVertex), &s_vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	if (s_buffers[1]) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_indexCount * sizeof(uint8), &s_indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (s_buffers[2]) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_buffers[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_outlineIndexCount * sizeof(uint8), &s_outlineIndices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (s_buffers[3]) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_buffers[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_outlinePointIndexCount * sizeof(uint8), &s_outlinePointIndices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	
}

void destroyVBOs() {
	glDeleteBuffers(4, &s_buffers[0]);
}

#endif

void testBeginFrame() {


	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void testEndFrame() {

   //glFinish();
}

void testDrawFrame() {

	static float y = 3.0f;

	int32 error = 0;
	int32 i = 0;
	Matrixa modelMatrix0;
	Matrixa modelMatrix1;
	Matrixa modelMatrix2;
	Matrixa viewMatrix;
	Matrixa modelViewProjectionMatrix;
	Matrixa modelViewMatrix;
	Matrixa projectionMatrix;
	Vector4a outlineColor = {1.0f, 1.0f, 1.0f, 1.0f};
	Vector4a axis = {0.0f, 1.0f, 0.0f, 1.0f};
	Vector4a shift = {0.0, 0.0f, 0.0f, 1.0f};
	Vector4a target = {0.0f, -1.0f, 0.0f, 1.0f};
	Vector4a eye = { 0.0f, 12.0f, 14.0f, 1.0f };
	Vector4a up = {0.0f, 1.0f, 0.0f, 1.0f};

	mtxMakePerspectiveRH(projectionMatrix, D_DEG2RAD(60.0f), 800.0f / 600.0f, 0.01f, 100.0f);
	mtxMakeLookAtRH(viewMatrix, eye, target, up);

	mtxUnit(modelMatrix1);
	mtxMakeRotationY(modelMatrix1, D_DEG2RAD(y));

	mtxUnit(modelMatrix0);
	//mtxMakeTranslation(modelMatrix0, shift);

	mtxMul(modelMatrix2, modelMatrix1, modelMatrix0);
	mtxMul(modelViewMatrix, modelMatrix2, viewMatrix);
	mtxMul(modelViewProjectionMatrix, modelViewMatrix, projectionMatrix);

	//////////////////////////////////////////////////////////////////////////

#if defined(D_TEST_VBO_LEAK)

	for (i = 0; i < 1; ++i) {

		createVBOs();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		shaderBind(s_modelShaderId);

		glStencilFunc( GL_ALWAYS, 1, -1 );
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );

		glUniformMatrix4fv(s_modelMVPUniformId, 1, 0, (float *)&modelViewProjectionMatrix[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, s_buffers[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT,           0,       sizeof(SVertex), (const void *)0 );
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE,   GL_TRUE, sizeof(SVertex), (const void *)12 );
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_buffers[1]);
		glDrawElements(GL_TRIANGLES, s_indexCount, GL_UNSIGNED_BYTE, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		

		shaderUnbind();

		//////////////////////////////////////////////////////////////////////////

		shaderBind(s_outlineShaderId);

		glStencilFunc( GL_NOTEQUAL, 1, -1 );
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		glLineWidth(s_outlineWidth * 2.0f);
		//glPointSize(s_outlineWidth * 2.0f);

		glUniformMatrix4fv(s_outlineMVPUniformId, 1, 0, (float *)&modelViewProjectionMatrix[0][0]);
		glUniform4fv(s_outlineColorUniformId, 1, outlineColor);
		glUniform1f(s_outlineLineWidthUniformId, s_outlineWidth * 2.0f);

		glBindBuffer(GL_ARRAY_BUFFER, s_buffers[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT,           0,       sizeof(SVertex), 0 );
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_buffers[2]);
		glDrawElements(GL_LINES, s_outlineIndexCount, GL_UNSIGNED_BYTE, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_buffers[3]);
		glDrawElements(GL_POINTS, s_outlinePointIndexCount, GL_UNSIGNED_BYTE, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		shaderUnbind();

		//////////////////////////////////////////////////////////////////////////
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		destroyVBOs();
	}


#else

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	shaderBind(s_modelShaderId);

	glStencilFunc( GL_ALWAYS, 1, -1 );
	glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );

	glUniformMatrix4fv(s_modelMVPUniformId, 1, 0, (float *)&modelViewProjectionMatrix[0][0]);

	glVertexAttribPointer(0, 3, GL_FLOAT,           0,       sizeof(SVertex), s_vertices[0].position );
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE,   GL_TRUE, sizeof(SVertex), s_vertices[0].color );
	glDrawElements(GL_TRIANGLES, s_indexCount, GL_UNSIGNED_BYTE, s_indices);

	shaderUnbind();

	//////////////////////////////////////////////////////////////////////////

	shaderBind(s_outlineShaderId);

	glStencilFunc( GL_NOTEQUAL, 1, -1 );
	glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
	glLineWidth(s_outlineWidth * 2.0f);
	//glPointSize(s_outlineWidth * 2.0f);

	glUniformMatrix4fv(s_outlineMVPUniformId, 1, 0, (float *)&modelViewProjectionMatrix[0][0]);
	glUniform4fv(s_outlineColorUniformId, 1, outlineColor);
	glUniform1f(s_outlineLineWidthUniformId, s_outlineWidth * 2.0f);

	glVertexAttribPointer(0, 3, GL_FLOAT,           0,       sizeof(SVertex), s_vertices[0].position );

	glDrawElements(GL_LINES, s_outlineIndexCount, GL_UNSIGNED_BYTE, s_outlineIndices);
	glDrawElements(GL_POINTS, s_outlinePointIndexCount, GL_UNSIGNED_BYTE, s_outlinePointIndices);

	shaderUnbind();

	//////////////////////////////////////////////////////////////////////////
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

#endif


	y += 0.75f;

}

void testUpdate() {

}

void testInitialize(int32 width, int32 height) {

	Vector4a viewPort = {0.0f, 0.0f, width, height};

#if defined (D_GFX_OPENGL)
	glewInit();
	glClearDepth(1.0f);
#elif defined (D_GFX_OPENGL_ES_2)
	glClearDepthf(1.0f);
#endif

	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearStencil(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	s_modelShaderId = shaderCreate(&s_vsrc, &s_fsrc);
	s_modelMVPUniformId = glGetUniformLocation(s_modelShaderId, "u_mvp");

	s_outlineShaderId = shaderCreate(&s_vSrcOutline, &s_fSrcOutline);
	if (s_outlineShaderId) {

		s_outlineMVPUniformId = glGetUniformLocation(s_outlineShaderId, "u_mvp");
		s_outlineColorUniformId = glGetUniformLocation(s_outlineShaderId, "u_color");
		s_outlineLineWidthUniformId = glGetUniformLocation(s_outlineShaderId, "u_lineWidth");
		s_outlineViewPortUniformId = glGetUniformLocation(s_outlineShaderId, "u_viewPort");

		shaderBind(s_outlineShaderId);
		glUniform4fv(s_outlineViewPortUniformId, 1, viewPort);
		shaderUnbind();
	}
}

void testDeinitialize() {

	shaderDelete(s_modelShaderId);
	shaderDelete(s_outlineShaderId);
}