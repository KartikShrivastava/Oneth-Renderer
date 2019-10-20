#version 330 core

float near = 0.1;
float far = 100;

in vec2 t_texCoords;

uniform sampler2D u_texture_diffuse1;

out vec4 o_Color;

void main() {
    vec4 col = texture(u_texture_diffuse1, t_texCoords);
	
	float depth = 2*gl_FragCoord.z - 1.0;
	float linearDepth = (2 * near * far) / (-depth * (far - near) + near + far);
	linearDepth = linearDepth / far;
	o_Color = vec4(vec3(linearDepth), 1.0);
}