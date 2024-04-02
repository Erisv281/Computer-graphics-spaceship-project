// Project main file



// Includes
#include "GL_utilities.h"
#include "MicroGlut.h"
#define MAIN
#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include "LoadTGA.h"
#include <math.h>
#include <iostream>



// Lookat
// Todo fix later
vec3 cameraPoint;
vec3 lookatPoint;
mat4 look, modelView, projectionMatrix;



// Models
Model* world;


// World
#define kGroundSize 100.0f
vec3 vertices[] =
{
 vec3(-kGroundSize,0.0f,-kGroundSize),
 vec3(-kGroundSize,0.0f,kGroundSize),
 vec3(kGroundSize,-0.0f,-kGroundSize),
 vec3(kGroundSize,-0.0f,kGroundSize)
};

vec3 vertex_normals[] =
{
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f)
};

vec2 tex_coords[] =
{
  vec2(0.0f,0.0f),
  vec2(0.0f,20.0f),
  vec2(20.0f,0.0f), 
  vec2(20.0f,20.0f)
};
GLuint indices[] =
{
  0, 1, 2, 1, 3, 2
};

vec3 colors[] = 
{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};


// vertex array object
unsigned int vertexArrayObjID;

// vertex buffer objects
unsigned int vertexBufferObjID;

// Programs
GLuint program;

// Prototypes
void handleInputs();
void drawWorld();
void loadModels();
void initTextures();

// Todo fix later
void handleInputs(){
	// WS = Move camerapoint
	// ADQE = Move lookat and camerapoint
	// RF Move up and down. 
	float cameraSpeed = 0.25f;

	// Found in collision2-surfaces-multiobj-little-city.c by Ingemar
	if (glutKeyIsDown('w'))
		cameraPoint += lookatPoint * cameraSpeed;

	if (glutKeyIsDown('a'))
		lookatPoint = MultVec3(Ry(0.03), lookatPoint);

	if (glutKeyIsDown('d'))
		lookatPoint = MultVec3(Ry(-0.03), lookatPoint);

	if (glutKeyIsDown('s'))
		cameraPoint -= lookatPoint * cameraSpeed;

	if (glutKeyIsDown('q'))
		cameraPoint += MultVec3(Ry(M_PI/2), lookatPoint) * cameraSpeed;

	if (glutKeyIsDown('e'))
		cameraPoint += MultVec3(Ry(-M_PI/2), lookatPoint) * cameraSpeed;

	if (glutKeyIsDown('r'))
		cameraPoint.y += 0.1;

	if (glutKeyIsDown('f')){
		cameraPoint.y -= 0.1;
	}
	

	// send camera position to shader, camera pos already in world cooridnates. 
	//glUniform3f(glGetUniformLocation(program, well within the lab tim"camera_pos"), cameraPoint.x, cameraPoint.y, cameraPoint.z);

	// Lookat here
	look = lookAtv(cameraPoint, cameraPoint + lookatPoint, vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(program, "lookat"), 1, GL_TRUE, look.m);
}

void loadModels(){
	world = LoadDataToModel(vertices, vertex_normals, tex_coords, colors, indices, 4, 6);	// Or 4*3

	// Todo add more models here
}

void initTextures(){
	// todo add textures here. 
}

// Todo fix using other code
void drawWorld(){
	glUseProgram(program);
	mat4 trans = T(0.0f, 0, 0.0f);
	mat4 scale = S(500.0f, 0.5f, 500.0f);
	mat4 rotation = Ry(0);
	mat4 modification = trans * scale * rotation;
	mat4 total = modification;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, total.m);


	DrawModel(world, program, "in_Position", "in_Normal", "inTexCoord");

}



// Init stuff
void GLInits(){
    dumpInfo();

	glClearColor(0.2,0.2,0.5,0);
	printError("GL inits");
}

// Init shaders and compile
void loadShaders(){
    program = loadShaders("shader.vert", "shader.frag");
	printError("init shader");
}

void init(void)
{
	loadModels();
	initTextures();
	GLInits();
    loadShaders();	

	// Projection
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);

	// Lookat matrix init
	cameraPoint = vec3(-5.9, 2.6, 0.25);
	lookatPoint = vec3(2.46, 0, 1.4);
	vec3 newPoint = cameraPoint + lookatPoint;
	look = lookAtv(cameraPoint, newPoint, vec3(0, 1, 0));

	// Model-view
	modelView = IdentityMatrix();



	
	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	// Predisplay
	printError("pre display");
	glClear(GL_COLOR_BUFFER_BIT);	// clear the screen

	// Input handler
	handleInputs();
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, modelView.m);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);


	drawWorld();
	
	// Post display
	printError("display");
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);

	// Todo enable glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);	// Call before creating context

	glutInitWindowSize(600, 600);
	glutCreateWindow ("Project");

	// Animation
	glutRepeatingTimer(150);

	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}
