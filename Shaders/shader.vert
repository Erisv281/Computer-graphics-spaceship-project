#version 150

in  vec3 in_Position;
in vec3 in_Normal;

// Matrices
uniform mat4 projection;
uniform mat4 model_world;	// this is model to world matrix. 
uniform mat4 lookat;

// Outs
out vec3 exNormal;

// Textures
in vec2 inTexCoord;
out vec2 texCoord;



void main(void)
{
	exNormal = mat3(lookat) * in_Normal;
	gl_Position =  projection * lookat * model_world * vec4(in_Position, 1.0);

	texCoord = inTexCoord;
}

