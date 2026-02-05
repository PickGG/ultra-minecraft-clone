#version 330 core

out vec4 color;

in vec2 texture_coord;
uniform sampler2D atlas;

void main()
{
	//color = vec4(1.0f, 0.7f, 0.2f, 1.0f);
	color = texture(atlas, texture_coord);
}