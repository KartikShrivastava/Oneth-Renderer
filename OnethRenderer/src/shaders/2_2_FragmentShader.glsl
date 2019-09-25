#version 330 core

in vec3 t_worldPos;
in vec3 t_normal;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_camPos;
uniform Material material;

out vec4 col;

void main(){
	vec4 ambient = material.ambient * u_lightColor;

	vec3 normal = normalize(t_normal);
	vec3 lightDir = normalize(u_lightPos - t_worldPos);
	vec4 diffuse = material.diffuse * max(dot(normal, lightDir), 0.0) * u_lightColor;

	vec3 viewDir = normalize(u_camPos - t_worldPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec4 specular = material.specular * pow(max(dot(viewDir,reflectDir),0.0), shininess) * u_lightColor;

	vec3 res = ambient + diffuse + specular;
	col = vec4(res, 1.0);
}