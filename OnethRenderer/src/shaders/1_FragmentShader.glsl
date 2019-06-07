#version 430 core

in vec4 f_col;
in vec2 f_tex;

uniform sampler2D u_textureWood;
uniform sampler2D u_textureYayi;
uniform float u_mixValue;

out vec4 col;

void main(){
	col = mix(texture(u_textureWood, f_tex), texture(u_textureYayi, vec2(1.0-f_tex.x,f_tex.y)), u_mixValue);// * f_col;
	//col = texture(u_textureWood, f_tex);
}