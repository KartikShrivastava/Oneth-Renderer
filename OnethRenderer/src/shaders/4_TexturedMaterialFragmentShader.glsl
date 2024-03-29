#version 330 core

struct Material{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 t_texCoords;
in vec3 t_worldPos;
in vec3 t_normal;

uniform Material u_material;
uniform Light u_light;
uniform vec3 u_camPos;

out vec4 o_color;

void main(){
	vec3 ambient = u_light.ambient * (texture(u_material.diffuseMap, t_texCoords).rgb);

	vec3 normal = normalize(t_normal);
	vec3 lightDir = normalize(u_light.position - t_worldPos);
	vec3 diffuse = u_light.diffuse * ( max(dot(lightDir,normal), 0.0) * (texture(u_material.diffuseMap, t_texCoords).rgb) );

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = normalize(u_camPos - t_worldPos);
	vec3 specular = u_light.specular * (pow(max(dot(reflectDir,viewDir), 0.0), u_material.shininess) * (texture(u_material.specularMap, t_texCoords).rgb));
	
	vec3 emission = 0.9 * texture(u_material.emissionMap, t_texCoords).rgb;

	vec3 result = ambient + diffuse + specular + emission;
	o_color = vec4(result, 1.0);
}
