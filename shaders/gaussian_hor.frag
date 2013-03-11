#version 330

uniform sampler2D texture;

smooth in vec2 textureCoords;

out vec4 colour;

//uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
//uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

uniform float offset[3] = float[]( 0.0, 0.002, 0.006 );
uniform float weight[3] = float[]( 0.23, 0.32, 0.07 );

void main(void)
{
	colour = texture2D(texture, vec2(textureCoords)) * weight[0];
	for (int i=1; i<3; i++) {
		colour += texture2D(texture, (vec2(textureCoords) + vec2(offset[i], 0.0))) * weight[i];
		colour += texture2D(texture, (vec2(textureCoords) - vec2(offset[i], 0.0))) * weight[i];
	}
}
