#version 330 core

in vec2 t_texCoords;

uniform sampler2D u_texture_diffuse1;

out vec4 o_Color;

void main()
{    
    o_Color = texture(u_texture_diffuse1, t_texCoords);
}