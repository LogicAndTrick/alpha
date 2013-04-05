#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

const vec2 offsets[4] = vec2[]
(
  vec2(-1.0,  1.0),
  vec2(-1.0, -1.0),
  vec2( 1.0,  1.0),
  vec2( 1.0, -1.0)
);

smooth in vec4 g_position[];
smooth in vec2 g_velocity[];
smooth in vec4 g_colour[];
smooth in vec2 g_size[];
smooth in float g_age[];

smooth out vec4 f_position;
smooth out vec4 f_fragment;
smooth out vec2 f_velocity;
smooth out vec4 f_colour;
smooth out vec2 f_size;
smooth out float f_age;

void main()
{
    f_position        = g_position[0];
    f_velocity        = g_velocity[0];
    f_colour          = g_colour[0];
    f_size = g_size[0];
    f_age = g_age[0];

    for (int i = 0; i < 4; ++i) {
        f_fragment = gl_Position = vec4(g_position[0].xy + (g_size[0] * offsets[i]), 0.0, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
