#version 330 core

in vec2 t_tex;
in vec3 t_worldPos;
in vec3 t_normal;

uniform sampler2D u_textureWood;
uniform sampler2D u_textureYayi;
uniform vec3 u_lightPos;
uniform vec4 u_lightColor;
uniform float u_mixValue;

out vec4 o_col;

void main(){
	float ambientStrength = 0.1;
	vec4 ambient = ambientStrength * u_lightColor;

	vec3 lightDir = normalize(u_lightPos - t_worldPos);
	vec4 diffuse = max(dot(normalize(t_normal),lightDir), 0.0) * u_lightColor;

	o_col = (ambient+diffuse) * mix(texture(u_textureWood, t_tex), texture(u_textureYayi, vec2(1.0-t_tex.x,t_tex.y)), u_mixValue);
}