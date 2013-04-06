#version 330

layout(location = 0) in vec2 v_position;
layout(location = 1) in vec2 v_velocity;
layout(location = 2) in vec4 v_colour;
layout(location = 3) in vec3 v_sizeAgeLifespan;

smooth out vec4 g_position;
smooth out vec2 g_velocity;
smooth out vec4 g_colour;
smooth out vec2 g_size;
smooth out float g_age;

uniform float time;
uniform mat4 viewport;

void main()
{
    g_position        = viewport * vec4(v_position.xy, 0, 1);
    g_velocity        = vec2(v_velocity.x, -v_velocity.y);
    g_colour          = v_colour;
    g_age = v_sizeAgeLifespan.y / v_sizeAgeLifespan.z;
    g_size = (viewport * vec4(v_sizeAgeLifespan.xx, 0, 0)).xy;
}
