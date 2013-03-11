#version 330

uniform sampler2D texture;

smooth in vec2 textureCoords;

out vec4 colour;

void main(void)
{
    vec4 c = texture2D(texture, textureCoords);
    colour = vec4(1, 1, 1, c.x);
}
