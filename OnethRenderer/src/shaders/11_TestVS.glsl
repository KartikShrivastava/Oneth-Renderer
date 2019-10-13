#version 330 core
layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texCoords;

uniform mat4 u_mvp;

out vec2 t_texCoords;

void main()
{
    gl_Position = u_mvp * vec4(i_pos, 1.0);
    t_texCoords = i_texCoords;    
}