#version 310 es

precision highp float;

#extension GL_EXT_gpu_shader5 : enable

layout(points) in;
layout(triangle_strip, max_vertices = 14) out;

uniform mat4 u_mvpMatrix;
uniform mat4 u_normalMatrix;

uniform vec3 u_normal;
uniform vec3 u_tangent;
uniform vec3 u_bitangent;
uniform float u_width;

in vec4 GS_PositionHeight[];

out vec3 FS_Color;
out vec3 FS_Normal;

void main() 
{
   vec3 verticalShift = u_normal * GS_PositionHeight[0].w;
   vec3 tangentShift = u_tangent * u_width * 0.5;
   vec3 bitangentShift = u_bitangent * u_width * 0.5;

   vec3 topPos = GS_PositionHeight[0].xyz + verticalShift;
   vec3 botPos = GS_PositionHeight[0].xyz;

   vec3 MP = -tangentShift + bitangentShift;
   vec3 MM = -tangentShift - bitangentShift;
   vec3 PP =  tangentShift + bitangentShift;
   vec3 PM =  tangentShift - bitangentShift;

   vec3 boxBot_MP_Normal = vec3(u_normalMatrix * vec4(normalize(MP), 1.0));
   vec3 boxBot_MM_Normal = vec3(u_normalMatrix * vec4(normalize(MM), 1.0));
   vec3 boxBot_PP_Normal = vec3(u_normalMatrix * vec4(normalize(PP), 1.0));
   vec3 boxBot_PM_Normal = vec3(u_normalMatrix * vec4(normalize(PM), 1.0));
   vec3 boxTop_MP_Normal = vec3(u_normalMatrix * vec4(normalize(MP + u_normal), 1.0));
   vec3 boxTop_MM_Normal = vec3(u_normalMatrix * vec4(normalize(MM + u_normal), 1.0));
   vec3 boxTop_PP_Normal = vec3(u_normalMatrix * vec4(normalize(PP + u_normal), 1.0));
   vec3 boxTop_PM_Normal = vec3(u_normalMatrix * vec4(normalize(PM + u_normal), 1.0));
   
   vec4 boxBot_MP = u_mvpMatrix * vec4(botPos + MP, 1.0);
   vec4 boxBot_MM = u_mvpMatrix * vec4(botPos + MM, 1.0);
   vec4 boxBot_PP = u_mvpMatrix * vec4(botPos + PP, 1.0);
   vec4 boxBot_PM = u_mvpMatrix * vec4(botPos + PM, 1.0);

   vec4 boxTop_MP = u_mvpMatrix * vec4(topPos + MP, 1.0);
   vec4 boxTop_MM = u_mvpMatrix * vec4(topPos + MM, 1.0);
   vec4 boxTop_PP = u_mvpMatrix * vec4(topPos + PP, 1.0);
   vec4 boxTop_PM = u_mvpMatrix * vec4(topPos + PM, 1.0);
   
   vec3 topColor = normalize(topPos);
   vec3 botColor = normalize(botPos);

   FS_Color = topColor;
   FS_Normal = boxTop_MP_Normal;
   gl_Position = boxTop_MP; EmitVertex();

   FS_Color = topColor;
   FS_Normal = boxTop_MM_Normal;
   gl_Position = boxTop_MM; EmitVertex();

   FS_Color = topColor;
   FS_Normal = boxTop_PP_Normal;
   gl_Position = boxTop_PP; EmitVertex();

   FS_Color = topColor;
   FS_Normal = boxTop_PM_Normal;
   gl_Position = boxTop_PM; EmitVertex();

   EndPrimitive();


   FS_Color = botColor;
   FS_Normal = boxBot_MP_Normal;
   gl_Position = boxBot_MP; EmitVertex();

   FS_Color = topColor;
   FS_Normal = boxTop_MP_Normal;
   gl_Position = boxTop_MP; EmitVertex();

   FS_Color = botColor;
   FS_Normal = boxBot_PP_Normal;
   gl_Position = boxBot_PP; EmitVertex();

   FS_Color = topColor;
   FS_Normal = boxTop_PP_Normal;
   gl_Position = boxTop_PP; EmitVertex();

   FS_Color = botColor;
   FS_Normal = boxBot_PM_Normal;
   gl_Position = boxBot_PM; EmitVertex();

   FS_Color = topColor;
   FS_Normal = boxTop_PM_Normal;
   gl_Position = boxTop_PM; EmitVertex();

   FS_Color = botColor;
   FS_Normal = boxBot_MM_Normal;
   gl_Position = boxBot_MM; EmitVertex();

   FS_Color = topColor;
   FS_Normal = boxTop_MM_Normal;
   gl_Position = boxTop_MM; EmitVertex();

   FS_Color = botColor;
   FS_Normal = boxBot_MP_Normal;
   gl_Position = boxBot_MP; EmitVertex();

   FS_Color = topColor;
   FS_Normal = boxTop_MP_Normal;
   gl_Position = boxTop_MP; EmitVertex();

   EndPrimitive();
}