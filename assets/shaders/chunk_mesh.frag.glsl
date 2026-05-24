#version 330 core

out vec4 color;

in vec2 texture_coord;
uniform sampler2D atlas;

void main()
{
	color = texture(atlas, texture_coord);
	if(color.a < 0.01)
		discard;
}