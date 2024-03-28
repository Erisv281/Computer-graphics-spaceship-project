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



// Models
// Todo remove this later
GLfloat vertices[] =
{
	-0.5f,-0.5f,0.0f,
	-0.5f,1.75f,0.0f,	// Changed
	0.5f,-0.5f,0.0f
};

// vertex array object
unsigned int vertexArrayObjID;

// vertex buffer objects
unsigned int vertexBufferObjID;

// Programs
GLuint program;

// Init stuff
void GLInits(){
    dumpInfo();

	glClearColor(0.2,0.2,0.5,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");
}

// Init shaders and compile
void loadShaders(){
    program = loadShaders("shader.vert", "shader.frag");
	printError("init shader");
}

void init(void)
{
	GLInits();
    loadShaders();	


	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	
	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	// Render triangle
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(600, 600);
	glutCreateWindow ("Project");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}
