#version 330 core

in vec2 t_texCoords;

uniform sampler2D u_text;

out vec4 o_color;

void main(){
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, t_texCoords).r);
	vec4 color = vec4(1.0, 1.0, 1.0, 1.0) * sampled;
	if(color.a < 0.1)
		discard;
	o_color = color;
}