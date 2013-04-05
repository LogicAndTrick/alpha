#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec4 vertexColour;

smooth out vec4 colour;
smooth out vec2 texture;

uniform float time;
uniform mat4 viewport;

void main()
{
	gl_Position = viewport * vec4(position.xy, 0.0, 1.0);
    texture = textureCoordinates;
    colour = vertexColour;
}
