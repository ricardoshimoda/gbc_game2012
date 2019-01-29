
//****************************************************************************
// NR-101128885-Assignment1.cpp by Ricardo Nakasako (C) 2018 All Rights Reserved.
//
// Assignment 1 submission.
//
// Description:
// Click run to see the results.
//
//*****************************************************************************
///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "math.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <list>

#include<iostream>

int numberOfSquares = 10;
int rotationAngle = 45;

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers = 2 };
enum Attrib_IDs { vPosition = 0 };

GLuint Buffers[NumBuffers];

const int screenSize = 848;
const GLuint NumVertices = 4;
GLfloat scaleStep = sqrt(2) / 2;
GLfloat scale = 0.9;
GLfloat angle = 45.0f;
GLfloat vertices[NumVertices][2];
//GLfloat vertices[NumVertices][4];
GLfloat verticeColors[NumVertices][4];

GLint MatrixId;
glm::mat4 ModelMatrix;

list<glm::mat4> modelList;


void push() {
	modelList.push_back(ModelMatrix);
}

void pop() {
	ModelMatrix = modelList.back();
	modelList.pop_back();
}

GLuint program;
void init(void)
{

	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	//Generating two buffers, one is used to store the coordinates of the vertices
	//The other one is not used. Just wanted to show that we can allocate as many as buffers, some of which might left unused.
	glGenBuffers(2, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	
	
	//Drawing first square 
	vertices[0][0] = -0.9;
	vertices[0][1] = -0.9;

	vertices[1][0] = 0.9;
	vertices[1][1] = -0.9;

	vertices[2][0] = 0.9;
	vertices[2][1] = 0.9;

	vertices[3][0] = -0.9;
	vertices[3][1] = 0.9;


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

}
/*
void transformObject(float scale, glm::vec3 rotationAxis, float
	rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}
*/
//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.58f, 0.44f, 0.49f, 1.0f);


	int i = 0;
	angle = 0.0f;
	scale = 1.0f;
	do {
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
		//Changing vertices colours
		float r =  (float)rand() / RAND_MAX;
		float g =  (float)rand() / RAND_MAX;
		float b =  (float)rand() / RAND_MAX;

		for (int i = 0; i < NumVertices; i++)
		{
			verticeColors[i][0] = r;
			verticeColors[i][1] = g;
			verticeColors[i][2] = b;
			verticeColors[i][3] = 1.0f;

		}

		//Pushing the coordinates of the vertices into the buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticeColors), verticeColors, GL_STATIC_DRAW);

		MatrixId = glGetUniformLocation(program, "Model");
		ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale));
		ModelMatrix = glm::rotate(ModelMatrix,
			glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		angle += rotationAngle;
		scale *= scaleStep;


		//Ordering the GPU to start the pipeline
		glDrawArrays(GL_LINE_LOOP, 0, NumVertices);
		//pop();
		i++;

	} while (i < 10);
	glFlush();

}

void idle()
{
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

int
main(int argc, char** argv)
{
	std::cout << "Number of squares to be created" << std::endl;
	std::cin >> numberOfSquares;
	std::cout << "Desired rotation angle" << std::endl;
	std::cin >> rotationAngle;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(screenSize, screenSize);
	glutCreateWindow("Nakasako, Ricardo, 101128885");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();
	
	

}
