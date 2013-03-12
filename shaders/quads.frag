#version 330

smooth in vec4 colour;

out vec4 finalColour;

uniform float time;
uniform mat4 viewport;

void main()
{
    finalColour = colour;
}
