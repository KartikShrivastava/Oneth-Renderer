#version 330 core

in vec2 t_texCoords;

uniform sampler2D u_texture;

out vec4 o_color;

const float offset = 1.0/300.0;

void main(){
	//no effect
	//o_color = texture(u_texture, t_texCoords);

	//invert effect
	//o_color = vec4(vec3(1.0 - texture(u_texture, t_texCoords)), 1.0);

	//grayscale effect
	//o_color = texture(u_texture, t_texCoords);
	//float average = (o_color.r + o_color.g + o_color.b) / 3.0;
	//float average = 0.2126 * o_color.r + 0.7152 * o_color.g + 0.0722 * o_color.b;	//weighted average for better results
	//o_color = vec4(average, average, average, 1.0);

	//kernel effect
	vec2 offsets[] = {
		vec2(-offset,	 offset),
		vec2( 0.0,		 offset),
		vec2( offset,	 offset),
		vec2(-offset,	 0.0),
		vec2( 0.0,		 0.0),
		vec2( offset,	 0.0),
		vec2(-offset,	-offset),
		vec2( 0.0,		-offset),
		vec2( offset,	-offset)
	};

	float kernel[] = {
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	};

	vec3 samples[9];
	vec3 col = vec3(0.0);

	for(int i=0; i<9; ++i){
		samples[i] = texture(u_texture, t_texCoords+offsets[i]).rgb;
		col += samples[i] * kernel[i];
	}

	o_color = vec4(col,1.0);
}