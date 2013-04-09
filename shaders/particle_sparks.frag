#version 330

smooth in vec4 f_position;
smooth in vec4 f_fragment;
smooth in vec2 f_velocity;
smooth in vec4 f_colour;
smooth in vec2 f_size;
smooth in float f_age;

out vec4 colour;

float distance(vec2 dir, vec2 pos, float size)
{
    return clamp(1 - abs(dot(normalize(dir), pos)) / size * 2, 0, 1);
}

void main()
{
    vec2 dir = vec2(f_velocity.y, -f_velocity.x);
    vec2 pos = (f_fragment - f_position).xy;
    float dist = distance(dir, pos, f_size.x) * (1 - f_age);
    float dist2 = distance(f_velocity, pos, f_size.x) * (1 - f_age);
    dist = (dist * 0.5) + (dist2 * 0.5);
    //colour = vec4(dist, dist, dist, 1) * f_colour;
    //if (val < 0.3) colour.w = 0;
    colour = f_colour;
    colour.w = dist;
}
