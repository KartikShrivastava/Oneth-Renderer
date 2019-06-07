#version 430 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 col;
layout (location = 2) in vec2 tex;

uniform mat4 u_mTWMat;

out vec4 f_col;
out vec2 f_tex;

void main(){
	gl_Position = u_mTWMat * pos;
	f_col = col;
	f_tex = tex;
}