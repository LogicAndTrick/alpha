#version 330

smooth in float age;

out vec4 colour;

const float MAX_TIME = 0.75;
const float TIME_FACTOR = 1 / MAX_TIME;

void main(void)
{
    float alpha = max(0, 1 - (TIME_FACTOR * age));
	colour = vec4(1, 1, 1, alpha);
}
