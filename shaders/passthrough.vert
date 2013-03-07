#version 330

layout(location = 0) in vec2 position;

smooth out vec2 textureCoords;

void main(void)
{
	gl_Position = vec4(position, 0.0, 1.0);
    textureCoords = vec2((position.x + 1) / 2.0, (position.y + 1) / 2.0);
}
