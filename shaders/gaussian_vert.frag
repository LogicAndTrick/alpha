#version 330

uniform sampler2D texture;

smooth in vec2 textureCoords;

out vec4 colour;

//uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
//uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

uniform float offset[4] = float[]( 0.0, 0.004, 0.012, 0.02 );
uniform float weight[4] = float[]( 0.25, 0.35, 0.07, 0.03 );

void main(void)
{
	colour = texture2D(texture, vec2(textureCoords)) * weight[0];
	for (int i=1; i<4; i++) {
		colour += texture2D(texture, (vec2(textureCoords) + vec2(0.0, offset[i]))) * weight[i];
		colour += texture2D(texture, (vec2(textureCoords) - vec2(0.0, offset[i]))) * weight[i];
	}
    colour.w = 1;
}
