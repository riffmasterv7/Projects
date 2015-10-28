
#include <gles.h>
#include <CShaderPipeline.h>

#include "../app_test/render.h"
#include "../app_test/camera.h"
#include "../app_test/model.h"

#include <math_v7.h>
#include <buffer.h>


#define P2F_SCREEN_SHADER "p2f_screen_es2"
#define P2F_T2F_SCREEN_SHADER "p2f_t2f_screen_es2"
#define CITY_SHADER "city_es2"
#define P3F_C4B_SHADER "p3f_c4b_es2"
#define FXAA_SHADER "fxaa2"
#define BOX_PIPELINE "box"


static int32 s_p2f_screen_scale_UniformId = -1;
static int32 s_p2f_screen_color_UniformId = -1;
static int32 s_p2f_t2f_screen_alpha_UniformId = -1;
static int32 s_city_light_dir_UniformId = -1;
static int32 s_city_mvp_matrix_UniformId = -1;
static int32 s_city_normal_matrix_UniformId = -1;
static int32 s_p3f_c4b_MVPUniformId = -1;
static int32 s_fxaa_RTRcpSizeUniformId = -1;

static CShaderPipeline * p2fScreenPipeline;
static CShaderPipeline * p2ft2fScreenPipeline;
static CShaderPipeline * cityPipeline;
static CShaderPipeline * p3fc4bPipeline;
static CShaderPipeline * fxaa2Pipeline;

static CShaderPipeline * boxPipeline;
static int32 s_boxMVPUniformId = -1;
static int32 s_boxNORUniformId = -1;
static int32 s_boxTangentUniformId = -1;
static int32 s_boxNormalUniformId = -1;
static int32 s_boxBitangentUniformId = -1;
static int32 s_boxWidthUniformId = -1;

void bindPipeline(CShaderPipeline * pipeline)
{
	static uint32 s_enabledAttribs = 0;

	uint32 attribIndex;
	for (attribIndex = s_enabledAttribs; attribIndex < pipeline->getActiveAttributes(); ++attribIndex) {
		glEnableVertexAttribArray(attribIndex);
	}
	for (attribIndex = pipeline->getActiveAttributes(); attribIndex < s_enabledAttribs; ++attribIndex) {
		glDisableVertexAttribArray(attribIndex);
	}

	s_enabledAttribs = pipeline->getActiveAttributes();

	glUseProgram(pipeline->getId());
}


int32 rdrCreateShaders() {

	boxPipeline = new CShaderPipeline(BOX_PIPELINE);

	if (boxPipeline) {

		bindPipeline(boxPipeline);

		s_boxMVPUniformId = glGetUniformLocation(boxPipeline->getId(), "u_mvpMatrix");
		s_boxNORUniformId = glGetUniformLocation(boxPipeline->getId(), "u_normalMatrix");
		s_boxNormalUniformId = glGetUniformLocation(boxPipeline->getId(), "u_normal");
		s_boxTangentUniformId = glGetUniformLocation(boxPipeline->getId(), "u_tangent");
		s_boxBitangentUniformId = glGetUniformLocation(boxPipeline->getId(), "u_bitangent");
		s_boxWidthUniformId = glGetUniformLocation(boxPipeline->getId(), "u_width");
	}
	
	p2fScreenPipeline = new CShaderPipeline(P2F_SCREEN_SHADER);

	if (p2fScreenPipeline) {

		bindPipeline(p2fScreenPipeline);
		s_p2f_screen_color_UniformId = glGetUniformLocation(p2fScreenPipeline->getId(), "u_color");
		s_p2f_screen_scale_UniformId = glGetUniformLocation(p2fScreenPipeline->getId(), "u_scale");
	}

	p2ft2fScreenPipeline = new CShaderPipeline(P2F_T2F_SCREEN_SHADER);

	if (p2ft2fScreenPipeline) {

		bindPipeline(p2ft2fScreenPipeline);
		s_p2f_t2f_screen_alpha_UniformId = glGetUniformLocation(p2ft2fScreenPipeline->getId(), "u_alpha");
		glUniform1i(glGetUniformLocation(p2ft2fScreenPipeline->getId(), "u_texture"), 0);
	}

	cityPipeline = new CShaderPipeline(CITY_SHADER);

	if (cityPipeline) {

		bindPipeline(cityPipeline);
		s_city_light_dir_UniformId = glGetUniformLocation(cityPipeline->getId(), "u_lightDir");
		s_city_mvp_matrix_UniformId = glGetUniformLocation(cityPipeline->getId(), "u_mvpMatrix");
		s_city_normal_matrix_UniformId = glGetUniformLocation(cityPipeline->getId(), "u_normalMatrix");
	}

	p3fc4bPipeline = new CShaderPipeline(P3F_C4B_SHADER);
	if (p3fc4bPipeline) {

		bindPipeline(p3fc4bPipeline);
		s_p3f_c4b_MVPUniformId = glGetUniformLocation(p3fc4bPipeline->getId(), "u_mvp");
	}

	fxaa2Pipeline = new CShaderPipeline(FXAA_SHADER);
	if (fxaa2Pipeline) {

		bindPipeline(fxaa2Pipeline);
		glUniform1i(glGetUniformLocation(fxaa2Pipeline->getId(), "u_texture"), 0);
		s_fxaa_RTRcpSizeUniformId = glGetUniformLocation(fxaa2Pipeline->getId(), "u_rtRcpSize");
	}

	return 1;
}

void rdrSetShader_p2f_screen() {
	bindPipeline(p2fScreenPipeline);
}

void rdrSetShader_p2f_t2f_screen() {
	bindPipeline(p2ft2fScreenPipeline);
}

void rdrSetColor_p2f_screen(float color[3]) {
	glUniform3f(s_p2f_screen_color_UniformId, color[0], color[1], color[2]);
}

void rdrSetAlpha_p2f_t2f_screen(float alpha) {
	glUniform1f(s_p2f_t2f_screen_alpha_UniformId, alpha);
}

void rdrDestroyShaders() {
	delete p2fScreenPipeline;
	delete p2ft2fScreenPipeline;
	delete p3fc4bPipeline;
	delete cityPipeline;
	delete fxaa2Pipeline;

	delete boxPipeline;
}

void rdrDrawArrays_p2f_screen(Vertex_p2f * pVertex, uint32 vertexCount, uint32 primitive) {

	glVertexAttribPointer(0, 2, GL_FLOAT,				GL_FALSE,	sizeof(Vertex_p2f), &pVertex->position[0]);
	glDrawArrays(primitive, 0, vertexCount);
}

void rdrDrawArrays_p2f_t2f_screen(const Vertex_p2f_t2f * pVertex, uint32 vertexCount, uint32 primitive, float alpha) {

	bindPipeline(p2ft2fScreenPipeline);
	glUniform1f(s_p2f_t2f_screen_alpha_UniformId, alpha);
	bufferBindVertex(0);
	glVertexAttribPointer(0, 2, GL_FLOAT,				GL_FALSE,	sizeof(Vertex_p2f_t2f), &pVertex->position[0]);
	glVertexAttribPointer(1, 2, GL_FLOAT,				GL_FALSE,	sizeof(Vertex_p2f_t2f), &pVertex->texcoord[0]);
	glDrawArrays(primitive, 0, vertexCount);
}

void rdrDraw_p3f_c4b(CameraControl * pCamCtrl, const Vertex_p3f_c4b * pVertex, uint32 vertexCount, uint32 primitive) {

	bindPipeline(p3fc4bPipeline);

	glUniformMatrix4fv(s_p3f_c4b_MVPUniformId, 1, 0, (float *)&pCamCtrl->camera.viewProjMtx[0][0]);

	bufferBindVertex(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,			GL_FALSE,	sizeof(Vertex_p3f_c4b), &pVertex->position);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE,	GL_TRUE,	sizeof(Vertex_p3f_c4b), &pVertex->color);
	glDrawArrays(primitive, 0, vertexCount);
}

void rdrDrawCityGroup(CameraControl * camCtrl, MicroCityGroup * cityGrp) {

	vec4 lightPos = {1.0f, 3.0f, 1.0f, 1.0f};
	vec4 lightDir;

	if (cityGrp->vertexCount) {

		vecNormalize(lightDir, lightPos);

		bindPipeline(cityPipeline);

		glUniform3f(s_city_light_dir_UniformId, lightDir[0], lightDir[1], lightDir[2]);
		glUniformMatrix4fv(s_city_mvp_matrix_UniformId, 1, 0, &camCtrl->camera.viewProjMtx[0][0]);
		glUniformMatrix4fv(s_city_normal_matrix_UniformId, 1, 0, &camCtrl->camera.normalMtx[0][0]);
		
		bufferBindVertex(cityGrp->positionBuffId);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,	sizeof(Vertex_3f), 0);
		bufferBindVertex(cityGrp->normalBuffId);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,	sizeof(Vertex_3f), 0);

		glDrawArrays(GL_TRIANGLES, 0, cityGrp->vertexCount);

		bufferBindVertex(0);
	}
}

void rdrDrawPointGroup(CameraControl * camCtrl, PointGroup * pointGrp)
{
	if (pointGrp->vertexCount)
	{
		CVertexSetup::bind(pointGrp->vertexSetup);

		bindPipeline(boxPipeline);

		glUniform3f(s_boxNormalUniformId, pointGrp->normal[0], pointGrp->normal[1], pointGrp->normal[2]);
		glUniform3f(s_boxTangentUniformId, pointGrp->tangent[0], pointGrp->tangent[1], pointGrp->tangent[2]);
		glUniform3f(s_boxBitangentUniformId, pointGrp->bitangent[0], pointGrp->bitangent[1], pointGrp->bitangent[2]);
		glUniform1f(s_boxWidthUniformId, pointGrp->width);

		glUniformMatrix4fv(s_boxMVPUniformId, 1, 0, &camCtrl->camera.viewProjMtx[0][0]);
		glUniformMatrix4fv(s_boxNORUniformId, 1, 0, &camCtrl->camera.normalMtx[0][0]);

		
		//CVertexBuffer::bind(pointGrp->vertexBuffer);
		//pointGrp->vertexBuffer->setup();

		glDrawArrays(GL_POINTS, 0, pointGrp->vertexCount);

		//CVertexBuffer::bind(0);
		CVertexSetup::bind(0);
	}
}

void rdrDrawArrays_fxaa(const Vertex_p2f_t2f * pVertex, uint32 vertexCount, uint32 primitive, float rcpX, float rcpY) {

	bindPipeline(fxaa2Pipeline);
	glUniform2f(s_fxaa_RTRcpSizeUniformId, rcpX, rcpY);
	bufferBindVertex(0);
	glVertexAttribPointer(0, 2, GL_FLOAT,				GL_FALSE,	sizeof(Vertex_p2f_t2f), &pVertex->position[0]);
	glVertexAttribPointer(1, 2, GL_FLOAT,				GL_FALSE,	sizeof(Vertex_p2f_t2f), &pVertex->texcoord[0]);
	glDrawArrays(primitive, 0, vertexCount);
}




