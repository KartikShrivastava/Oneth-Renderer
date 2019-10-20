#version 330 core

uniform sampler2D u_texture;

in vec2 t_texCoords;

out vec4 o_color;

void main(){
	vec4 texCol = texture(u_texture, t_texCoords);
	//if(texCol.a < 0.1)	//commented this after enabling blending
	//	discard;
	o_color = texCol;
}