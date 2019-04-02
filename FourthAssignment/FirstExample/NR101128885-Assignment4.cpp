
//****************************************************************************
// NR-101128885-Assignment3.cpp by Ricardo Nakasako (C) 2019 All Rights Reserved.
//
// Assignment 3 submission.
//
// Description:
// Click run to see the results.
//
//*****************************************************************************
///////////////////////////////////////////////////////////////////////
//
// Rubik and the other one
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
#include <SOIL.h>
#include <list>

#include<iostream>

/*
 * Constants for solving the first problem
 */
int numberOfSquares = 7;
float sideSize = 1.0f;
float rotationAngleCube1 = 0.0f;
float rotationAngleCube2 = -90.0f;
float rotationSpeed = 0.2f;
const int screenSize = 848;
float halfCubeSide = 0.65f;

float halfPyramidSide = 0.65f;
float pyramidHeight = 2.0f;

float cameraX = 1.0f;
float cameraY = 2.0f; /* Just a nicer perspective than 3,3,4, you know */
float cameraZ = 1.0f;
float cameraStep = 0.1f;

float lightX = 0.0f;
float lightY = 0.0f;
float lightZ = 5.0f;
float lightStep = 0.3f;
glm::vec3 lightColor = glm::vec3(1,1,1);

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)

GLuint gVAO;
GLuint gVAOPyramid;

GLuint MatrixID;
GLuint ViewID;
GLuint ModelID;
GLuint LightPosID;
GLuint LightColID;

glm::mat4 MVP;
glm::mat4 View;
glm::mat4 Projection;
GLuint colours_vbo[5];
GLuint program;

/****************************************************************************************/


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


	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(program, "MVP");
	ViewID = glGetUniformLocation(program, "V");
	ModelID = glGetUniformLocation(program, "M");
	LightPosID = glGetUniformLocation(program, "lightColor");
	LightColID = glGetUniformLocation(program, "lightPosition");



	// First thing - setting the camera in perspective mode
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f); // In world coordinates

// Camera matrix
	View = glm::lookAt(
		glm::vec3(cameraX, cameraY, cameraZ),		// Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0),			// Camera Looking at the origin
		glm::vec3(0, 1, 0)			// Head is up (set to 0,-1,0 to look upside-down)
	);

	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	float step = sideSize / numberOfSquares;
	float startPoint = -sideSize / 2;
	int pointArraySize = (numberOfSquares + 1) * (numberOfSquares + 1) * 3;
	//float points[] = {
	//	-0.5,-0.5, 0.0,
	//	-0.5, 0.0, 0.0,
	//	-0.5, 0.5, 0.0,

	//	 0.0,-0.5, 0.0,
	//	 0.0, 0.0, 0.0,
	//	 0.0, 0.5, 0.0,

	//	 0.5,-0.5, 0.0,
	//	 0.5, 0.0, 0.0,
	//	 0.5, 0.5, 0.0
	//};

	float *points = new float [pointArraySize];
	float *currentPointer;
	currentPointer = points;
	for (int i = 0; i < numberOfSquares + 1; i++) {
		for (int j = 0; j < numberOfSquares + 1; j++) {
			if (j == 0) {
				*currentPointer = startPoint;
			}
			else if (j == numberOfSquares) {
				*currentPointer = -startPoint;
			}
			else {
				*currentPointer = startPoint + j * step;
			}
			currentPointer++;

			/* - But who is Y? Y is dead baby */
			*currentPointer = 0.0f;
			currentPointer++;

			if (i == 0) {
				*currentPointer = startPoint;
			}
			else if (i == numberOfSquares) {
				*currentPointer = -startPoint;
			}
			else {
				*currentPointer = startPoint + i * step;
			}
			currentPointer++;

		}
	}

	//currentPointer = points;
	//for (int i = 0; i < numberOfSquares + 1; i++) {
	//	for (int j = 0; j < numberOfSquares + 1; j++) {
	//		cout << "Point: (" << i <<"," << j <<") " << *currentPointer << endl;
	//		currentPointer++;
	//		cout << "Point: (" << i << "," << j << ") " << *currentPointer << endl;
	//		currentPointer++;
	//		cout << "Point: (" << i << "," << j << ") " << *currentPointer << endl;
	//		currentPointer++;
	//	}
	//}
	

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, pointArraySize * sizeof(float), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/*
	 * Coloring it all black
	 */
	//float colors[] = {
	//	0.0f,0.0f,0.0f,
	//	0.0f,0.0f,0.0f,
	//	0.0f,0.0f,0.0f,

	//	0.0f,0.0f,0.0f,
	//	0.0f,0.0f,0.0f,
	//	0.0f,0.0f,0.0f,

	//	0.0f,0.0f,0.0f,
	//	0.0f,0.0f,0.0f,
	//	0.0f,0.0f,0.0f
	//};
	float *colors = new float[pointArraySize];
	for (int i = 0; i < pointArraySize; i++) {
		colors[i] = 0.0f;
	}


	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);

	glBufferData(GL_ARRAY_BUFFER, pointArraySize * sizeof(float), colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	/*
	 * now we establish the array for mapping the texture - isn't this kinda like the other one?
	 */
	//float texMapCube[] = {
	//	0.0, 0.0,
	//	0.0, 0.5,
	//	0.0, 1.0,

	//	0.5,0.0,
	//	0.5,0.5,
	//	0.5,1.0,

	//	1.0,0.0,
	//	1.0,0.5,
	//	1.0,1.0
	//};
	int uvMappingSize = (numberOfSquares + 1) * (numberOfSquares + 1) * 2;
	float *texMapCube = new float[uvMappingSize]; 
	currentPointer = texMapCube;
	for (int i = 0; i < (numberOfSquares + 1); i++) {
		for (int j = 0; j < (numberOfSquares + 1); j++) {
			if (i == 0) {
				*currentPointer = 0.0;
			}
			else if (i == numberOfSquares) {
				*currentPointer = 1.0;
			}
			else {
				*currentPointer = i * step;
			}
			currentPointer++;

			if (j == 0) {
				*currentPointer = 0.0;
			}
			else if (j == numberOfSquares) {
				*currentPointer = 1.0;
			}
			else {
				*currentPointer = j * step;
			}
			currentPointer++;
		}
	}
	/*
	// Testing to see if the UV mapping corresponds to what was necessary
	currentPointer = texMapCube;
	for (int i = 0; i < numberOfSquares + 1; i++) {
		for (int j = 0; j < numberOfSquares + 1; j++) {
			std::cout << "UVMappingX: (" << i << "," << j << ") " << *currentPointer << endl;
			currentPointer++;
			std::cout << "UVMappingY: (" << i << "," << j << ") " << *currentPointer << endl;
			currentPointer++;
		}
	}
	*/	

	GLuint texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);

	glBufferData(GL_ARRAY_BUFFER, uvMappingSize * sizeof(float), texMapCube, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	float *normalMapCube = new float[pointArraySize]; 
	for (int i = 0; i < pointArraySize; i++) {
		if (i % 3 == 1) {
			normalMapCube[i] = 1.0f;
		}
		else {
			normalMapCube[i] = 0.0f;
		}
	}
	/*
	// Testing to see of the normal mapping was created successfully
	currentPointer = normalMapCube;
	for (int i = 0; i < numberOfSquares + 1; i++) {
		for (int j = 0; j < numberOfSquares + 1; j++) {
			cout << "NormalX: (" << i << "," << j << ") " << *currentPointer << endl;
			currentPointer++;
			cout << "NormalY: (" << i << "," << j << ") " << *currentPointer << endl;
			currentPointer++;
			cout << "NormalZ: (" << i << "," << j << ") " << *currentPointer << endl;
			currentPointer++;
		}
	}
	*/

	GLuint normal_vbo = 0;
	glGenBuffers(1, &normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);

	glBufferData(GL_ARRAY_BUFFER, pointArraySize * sizeof(float), normalMapCube, GL_STATIC_DRAW);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);


	/*
	 * Faces face face face face face face
	 */
	
	//GLushort cube_index_array[] = {
	//	0,3,1,
	//	1,3,4,
	//	1,4,2,
	//	2,4,5,
	//	3,6,4,
	//	4,6,7,
	//	4,7,5,
	//	5,7,8
	//
	//};
	int numberOfTriangles = numberOfSquares * numberOfSquares * 2 * 3;
	GLushort *cube_index_array = new GLushort[numberOfTriangles];
	GLushort *shortPointer;
	shortPointer = cube_index_array;
	for (int i = 0; i < numberOfSquares; i++) {
		for (int j = 0; j < numberOfSquares; j++) {
			GLushort base = j + (numberOfSquares + 1) * i;
			*shortPointer = base;
			shortPointer++;
			*shortPointer = base + numberOfSquares + 1;
			shortPointer++;
			*shortPointer = base + 1;
			shortPointer++;
			*shortPointer = base + 1;
			shortPointer++;
			*shortPointer = base + numberOfSquares + 1;
			shortPointer++;
			*shortPointer = base + numberOfSquares + 2;
			shortPointer++;
		}
	}

	/*
	// Testing to see if the ibo was created correctly
	shortPointer = cube_index_array;
	for (int i = 0; i < numberOfSquares; i++) {
		for (int j = 0; j < numberOfSquares; j++) {
			GLushort base = j + (numberOfSquares + 1) * i;
			cout << "(" << *shortPointer << ",";
			shortPointer++;
			cout << *shortPointer << ",";
			shortPointer++;
			cout << *shortPointer << ")" << endl;
			shortPointer++;

			cout << "(" << *shortPointer << ",";
			shortPointer++;
			cout << *shortPointer << ",";
			shortPointer++;
			cout << *shortPointer << ")" << endl;
			shortPointer++;
		}
	}
	*/

	GLuint cube_IBO;
	glGenBuffers(1, &cube_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfTriangles * sizeof(GLushort), cube_index_array, GL_STATIC_DRAW);
	
	/*
	 * Reads the information of textures
	 */
	GLint widthRubik, heightRubik, widthBonus, heightBonus;
	unsigned char* imageRubik = SOIL_load_image("wicker.jpg", &widthRubik, &heightRubik, 0, SOIL_LOAD_RGB);

	// Setup rubik's texture
	GLuint cube_tex = 0;
	glGenTextures(1, &cube_tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthRubik, heightRubik, 0, GL_RGB, GL_UNSIGNED_BYTE, imageRubik);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

	// Restablishes Rubik texture as the current one

}

void transformDistortedObject(float scaleX, float scaleY, float scaleZ, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scaleX, scaleY, scaleZ));

	MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
	glUniform3f(LightColID, lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(LightPosID, lightX, lightY, lightZ);
}

void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));

	MVP = Projection * View * Model;

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
	glUniform3f(LightColID, lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(LightPosID, lightX, lightY, lightZ);

}

void drawCubes(int numOfSquares) {

	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	View = glm::lookAt(
		glm::vec3(cameraX, cameraY, cameraZ),		// Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0),			// Camera Looking at the origin
		glm::vec3(0, 1, 0)			// Head is up (set to 0,-1,0 to look upside-down)
	);
	glBindVertexArray(gVAO);
	transformObject(1, Y_AXIS, rotationAngleCube1, glm::vec3(0.0, 0.0, 0.0));
	glDrawElements(GL_TRIANGLES, numberOfSquares * numberOfSquares * 2 * 3, GL_UNSIGNED_SHORT, 0);
	/*
	transformObject(1, Y_AXIS, rotationAngleCube2, glm::vec3(3.0, 0.0, 0.0));
	glDrawElements(GL_TRIANGLES, numberOfSquares * numberOfSquares * 2 * 3, GL_UNSIGNED_SHORT, 0);
	
	rotationAngleCube1 += rotationSpeed;
	if (rotationAngleCube1 == 360)
		rotationAngleCube1 = 0;

	rotationAngleCube2 -= rotationSpeed;
	if (rotationAngleCube2 == 0)
		rotationAngleCube2 = 360;
		*/
	/*
	-- This draws the plane
	transformDistortedObject(4.0f, 0.2f, 4.0f, Y_AXIS, 0, glm::vec3(0, -1.00, 0));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	*/
	//-- This draws the pyramid
	/*
	glUniform1i(glGetUniformLocation(program, "texture0"), 1);
	glBindVertexArray(gVAOPyramid);
	transformObject(1.0f, Y_AXIS, 0, glm::vec3(0, 0.0, 0));// pyramid in the middle
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0);*/
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.5f, 0.9f, 0.0f);
	drawCubes(numberOfSquares);
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
	/*
	“W” and “S” to move the camera in and out.
c. Use “A” and “D” to move the camera left and right.
d. Use “R” and “F” to move the camera up and down.
e. The camera shoul
	*/
	switch (key) {
	case 'w':
	case 'W':
		cameraZ -= cameraStep;
		//generateRandomColoredSquares(5);
		// call a function
		break;
	case 's':
	case 'S':
		cameraZ += cameraStep;
		break;
	case 'a':
	case 'A':
		cameraX -= cameraStep;
		//generateRandomColoredSquares(5);
		// call a function
		break;
	case 'd':
	case 'D':
		cameraX += cameraStep;
		break;
	case 'f':
	case 'F':
		cameraY -= cameraStep;
		//generateRandomColoredSquares(5);
		// call a function
		break;
	case 'r':
	case 'R':
		cameraY += cameraStep;
		break;
	case 'i':
	case 'I':
		lightY += lightStep;
		break;
	case 'k':
	case 'K':
		lightY -= lightStep;
		break;
	case 'j':
	case 'J':
		lightX -= lightStep;
		break;
	case 'l':
	case 'L':
		lightX += lightStep;
		break;

	default:
		break;
	}
}

int main(int argc, char** argv)
{
	//std::cout << "Number of cubes to be created" << std::endl;
	//std::cin >> numberOfSquares;
	//std::cout << "Desired rotation angle" << std::endl;
	//std::cin >> rotationAngle;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Nakasako, Ricardo, 101128885");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	//glewExperimental = true;
	glewInit();    //Initializes the glew and prepares the drawing pipeline.

	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardFun);
	glutIdleFunc(idle);
	glutMainLoop();
}
