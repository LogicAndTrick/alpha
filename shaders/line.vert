#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in float pointtime;

smooth out float age;

uniform float time;
uniform mat4 viewport;

void main()
{
	gl_Position = viewport * vec4(position.xy, 1.0, 1.0);
    age = time - pointtime;
}
