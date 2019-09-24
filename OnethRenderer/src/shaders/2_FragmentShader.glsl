#version 330 core

in vec3 t_worldPos;
in vec3 t_normal;

uniform vec4 u_objectColor;
uniform vec4 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_camPos;

out vec4 col;

void main(){
	float ambientStrength = 0.1;
	vec4 ambient = ambientStrength * u_lightColor;

	vec3 normal = normalize(t_normal);
	vec3 lightDir = normalize(u_lightPos - t_worldPos);
	vec4 diffuse = max(dot(normal, lightDir), 0.0) * u_lightColor;

	float specularStrength = 0.7;
	vec3 viewDir = normalize(u_camPos - t_worldPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec4 specular = specularStrength * pow(max(dot(viewDir,reflectDir),0.0), 128) * u_lightColor;

	col = (ambient + diffuse + specular) * u_objectColor;
}