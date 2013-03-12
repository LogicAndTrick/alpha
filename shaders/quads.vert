#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 vertexColour;

smooth out vec4 colour;

uniform float time;
uniform mat4 viewport;

void main()
{
	gl_Position = viewport * vec4(vec2(200,200) + position.xy * 100, 0.0, 1.0);
    colour = vertexColour;
}
