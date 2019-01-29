
//****************************************************************************
// NR-101128885-Assignment2.cpp by Ricardo Nakasako (C) 2018 All Rights Reserved.
//
// Assignment 2 submission.
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

/*
 * Constants for solving the first problem
 */
int numberOfSquares = 10;
int rotationAngle = 45;
const int screenSize = 848;

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)

GLuint gVAO;
GLuint MatrixID;
glm::mat4 MVP;
glm::mat4 View;
glm::mat4 Projection;
GLuint colours_vbo[5];
GLuint program;

/****************************************************************************************/

void recolorSquare() {
	float r = (float)rand() / RAND_MAX;
	float g = (float)rand() / RAND_MAX;
	float b = (float)rand() / RAND_MAX;

	float colours[] = {
		r, g,  b,
		r, g,  b,
		r, g,  b,
		r, g,  b
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
}

void generateRandomColoredSquares(int numOfColors) {
	srand(time(NULL));
	glGenBuffers(numOfColors, colours_vbo);

	for (int i = 0; i < numOfColors; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, colours_vbo[i]);
		recolorSquare();
	}
}

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
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(program, "MVP");

	Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordinates

// Camera matrix
	View = glm::lookAt(
		glm::vec3(0, 0, -1.0f), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	float points[] = {
		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
	};
	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	generateRandomColoredSquares(5);

}

void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));

	MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}

void drawSquares(int numOfSquares) {
	float x = 19.0f;
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo[0]);
	transformObject(x, Z_AXIS, 0, glm::vec3(0, 0, 0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	int colorIndex = 1;
	for (int i = 1; i<numOfSquares; i++){
        x /=2;
        x = sqrt((x*x) + (x*x));
        float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45° per second
        //float angle = 45;
        transformObject(x, Z_AXIS, i*angle, glm::vec3(0,0,0));
        glBindBuffer(GL_ARRAY_BUFFER, colours_vbo[colorIndex++]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        glLineWidth(5.0f);
        glDrawArrays(GL_LINE_LOOP, 0, 4);

        if(colorIndex == 5)
            colorIndex  = 1;
    }

}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.5f, 0.9f, 0.0f);
	drawSquares(numberOfSquares);
	glFlush();
}

void idle() {
	glutPostRedisplay();
}


//---------------------------------------------------------------------
//
// main
//


void keyboardFun(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		generateRandomColoredSquares(5);
		// call a function
		break;
	case 'b':
		// call a function
		break;

	default:
		break;
	}
}

int main(int argc, char** argv)
{
	std::cout << "Number of squares to be created" << std::endl;
	std::cin >> numberOfSquares;
	std::cout << "Desired rotation angle" << std::endl;
	std::cin >> rotationAngle;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(screenSize, screenSize);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Nakasako, Ricardo, 101128885");

	glewExperimental = true;
	glewInit();    //Initializes the glew and prepares the drawing pipeline.

	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardFun);
	glutIdleFunc(idle);
	glutMainLoop();
}
