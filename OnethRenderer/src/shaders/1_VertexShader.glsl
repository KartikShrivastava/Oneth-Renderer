#version 430 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 tex;

uniform mat4 u_mvp;

out vec2 f_tex;

void main(){
	gl_Position = u_mvp * pos;
	f_tex = tex;
}