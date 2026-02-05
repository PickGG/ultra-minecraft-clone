#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 in_texture_coord;

uniform mat4 projection_view;
out vec2 texture_coord;

void main()
{
    gl_Position = projection_view * vec4(position.x, position.y, position.z, 1.0);
    texture_coord = in_texture_coord;
}