#version 330 core

//	i_	denotes attributes which are passed from CPU
//	t_	denotes attributes which are transferred between shaders
//	u_	denotes uniforms
//	o_	denotes attributes which frangment shader outputs

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec2 i_tex;
layout (location = 2) in vec3 i_normal;

uniform mat4 u_mvp;
uniform mat4 u_model;

out vec2 t_tex;
out vec3 t_worldPos;
out vec3 t_normal;

void main(){
	gl_Position = u_mvp * i_pos;
	t_worldPos = vec3(u_model * i_pos);
	t_tex = i_tex;
	t_normal = mat3(u_model) * i_normal;
}