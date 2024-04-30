#version 150

out vec4 out_Color;
in vec3 exNormal;
const float kd = 0.7f;

// Textures
in vec2 texCoord;
uniform sampler2D texUnit;

void main(void)
{
	// Diffuse light
	const vec3 light = vec3(0.58, 0.58, 0.58); // Given in VIEW coordinates!
	float diffuse;
	
	// Diffuse
	diffuse = kd * clamp(dot(light, normalize(exNormal)), 0, 1);
	diffuse = max(0.0, diffuse); // No negative light


	out_Color = texture(texUnit, texCoord);	// Set textures
}
