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
    // vec2 diff = abs((f_fragment - f_position).xy) / (f_size.xy);
    // float val = clamp(1 - sqrt(diff.x * diff.x + diff.y * diff.y), 0, 1);
    // float dist = val * (1 - f_age);
    // colour = vec4(dist, dist, dist, 1) * f_colour;
    // if (val < 0.3) colour.w = 0;

    vec2 dir = vec2(f_velocity.y, -f_velocity.x);
    vec2 pos = (f_fragment - f_position).xy;
    float val = clamp(1 - abs(dot(normalize(dir), pos)) / f_size.x * 2, 0, 1);
    float dist = val * (1 - f_age);
    colour = vec4(dist, dist, dist, 1) * f_colour;
    // if (val < 0.3) colour.w = 0;

    // vec2 np = (f_fragment - f_position).xy * f_velocity;
    // float u = (np.x + np.y) / pow(length(f_velocity), 2);
    // vec2 pos = f_position.xy + u * f_velocity;
    // vec2 diff = abs((f_fragment.xy - pos).xy) / (f_size.xy);
    // float val = clamp(1 - sqrt(diff.x * diff.x + diff.y * diff.y), 0, 1);
    // float dist = val;// * (1 - f_age);
    // colour = vec4(dist, dist, dist, 1) * f_colour;
    // if (u < -0.8) colour.w = 0;

    //float dist = length(f_fragment.xy - (f_position.xy + u * f_velocity)) * 100;
    //colour = vec4(dist, dist, dist, 1) * f_colour;
}
