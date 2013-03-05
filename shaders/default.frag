#version 330

smooth in float age;

out vec4 colour;

void main(void)
{
	colour = vec4(1, 1, 1, age);
}
