#version 330

uniform sampler2D sampler;

smooth in vec4 colour;
smooth in vec2 texture;

out vec4 finalColour;

void main()
{
    finalColour = colour;
    finalColour = texture2D(sampler, texture);
}
