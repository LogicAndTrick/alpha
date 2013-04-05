#version 330

smooth in vec4 f_position;
smooth in vec4 f_fragment;
smooth in vec2 f_velocity;
smooth in vec4 f_colour;
smooth in vec2 f_size;
smooth in float f_age;

out vec4 colour;

void main()
{
    vec2 diff = abs((f_fragment - f_position).xy) / (f_size.xy);
    float val = clamp(1 - sqrt(diff.x * diff.x + diff.y * diff.y), 0, 1);
    float dist = val * (1 - f_age);
    colour = vec4(dist, dist, dist, 1);
    if (val < 0.3) colour.w = 0;
}
