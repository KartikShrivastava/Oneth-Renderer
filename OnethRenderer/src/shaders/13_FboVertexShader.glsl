#version 330 core

layout (location=0) in vec2 i_pos;
layout (location=1) in vec2 i_texCoords;

out vec2 t_texCoords;

void main(){
	gl_Position = vec4(i_pos.xy, 0.0, 1.0);
	t_texCoords = i_texCoords;
}