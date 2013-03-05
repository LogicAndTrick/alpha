#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in float pointage;

smooth out float age;

uniform mat4 viewport;

void main()
{
	gl_Position = viewport * vec4(position.x, position.y, 0.0, 1.0);
    age = pointage;
}
