#version 330

uniform sampler2D texture;

out vec4 colour;

void main(void)
{
    colour = texture2D(texture, vec2(gl_FragCoord));
}
