#version 330

uniform sampler2D texture;

smooth in vec2 textureCoords;

out vec4 colour;

void main(void)
{
    colour = texture2D(texture, textureCoords);
}
