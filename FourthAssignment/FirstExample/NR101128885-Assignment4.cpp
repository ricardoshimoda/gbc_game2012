
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
// Lighting
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
const int screenSize = 848;

float cameraX = 0.0f;
float cameraY = 5.0f; /* Just a nicer perspective than 3,3,4, you know */
float cameraZ = 0.1f;
float cameraStep = 0.1f;

/*
 * Light sources
 */

/*
float lightX = 0.0f;
float lightY = 0.1f;
float lightZ = 0.0f;
float lightStep = 0.05f;
glm::vec3 lightColor = glm::vec3(1,1,1);
struct PointLight {
	GLuint posHandle;
	GLuint colorHandle;
	GLuint strengthHandle;
};
struct DirectLight {
	GLuint dirHandle;
	GLuint colorHandle;
	GLuint strengthHandle;
};
*/
glm::vec3 currentLightPos;


struct Light {
	GLuint colorHandle;
	GLuint posHandle;
	GLuint strengthHandle;
	GLuint falloffStartHandle;
	GLuint falloffEndHandle;
};

Light   pointLights[2];

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)

GLuint gVAO;
GLuint gVAOPyramid;

GLuint MatrixID;
GLuint ViewID;
GLuint ModelID;

glm::mat4 MVP;
glm::mat4 View;
glm::mat4 Projection;
GLuint colours_vbo[5];
GLuint program;

/****************************************************************************************/

void initLights() {
	// first, get handles
	pointLights[0].colorHandle = glGetUniformLocation(program, "pointLights[0].Color");
	pointLights[0].posHandle = glGetUniformLocation(program, "pointLights[0].Position");
	pointLights[0].strengthHandle = glGetUniformLocation(program, "pointLights[0].Strength");
	pointLights[0].falloffStartHandle = glGetUniformLocation(program, "pointLights[0].falloffStart");
	pointLights[0].falloffEndHandle = glGetUniformLocation(program, "pointLights[0].falloffEnd");

	pointLights[1].colorHandle = glGetUniformLocation(program, "pointLights[1].Color");
	pointLights[1].posHandle = glGetUniformLocation(program, "pointLights[1].Position");
	pointLights[1].strengthHandle = glGetUniformLocation(program, "pointLights[1].Strength");
	pointLights[1].falloffStartHandle = glGetUniformLocation(program, "pointLights[1].falloffStart");
	pointLights[1].falloffEndHandle = glGetUniformLocation(program, "pointLights[1].falloffEnd");

	// second, pass data
	glUniform3fv(pointLights[0].colorHandle, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	currentLightPos = glm::vec3(-0.3f, 0.1f, 0.2f);
	glUniform3fv(pointLights[0].posHandle, 1, glm::value_ptr(currentLightPos));
	glUniform1f(pointLights[0].strengthHandle, 1.0f);
	glUniform1f(pointLights[0].falloffStartHandle, 1.0f);
	glUniform1f(pointLights[0].falloffEndHandle, 50.0f);

	glUniform3fv(pointLights[1].colorHandle, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform3fv(pointLights[1].posHandle, 1, glm::value_ptr(glm::vec3(0.3f, 0.1f, 0.0f)));
	glUniform1f(pointLights[1].strengthHandle, 1.0f);
	glUniform1f(pointLights[1].falloffStartHandle, 1.0f);
	glUniform1f(pointLights[1].falloffEndHandle, 50.0f);
}

void crossProduct(float vect_A[], float vect_B[], float cross_P[])
{
	cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
	cross_P[1] = vect_A[0] * vect_B[2] - vect_A[2] * vect_B[0];
	cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];

	float length = sqrt(cross_P[0]* cross_P[0] + cross_P[1]* cross_P[1]+ cross_P[2]*cross_P[2]);
	cross_P[0] = cross_P[0] / length;
	cross_P[1] = cross_P[1] / length;
	cross_P[2] = cross_P[2] / length;

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


	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(program, "MVP");
	ViewID = glGetUniformLocation(program, "V");
	ModelID = glGetUniformLocation(program, "M");

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

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, pointArraySize * sizeof(float), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Color Array
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
	
	GLuint texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, uvMappingSize * sizeof(float), texMapCube, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	/*
	 * Faces face face face face face face
	 */
	
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

	GLuint cube_IBO;
	glGenBuffers(1, &cube_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfTriangles * sizeof(GLushort), cube_index_array, GL_STATIC_DRAW);

	float *normalMapCube = new float[numberOfTriangles];
	for (int i = 0; i < numberOfTriangles; i+=3) {


		/*
 * Obtain triangle
 */
		int left = cube_index_array[i];
		int center = cube_index_array[i + 1];
		int right = cube_index_array[i + 2];

		/*
		 * Obtain vertices
		 */
		int vertexIndex = left * 3;
		float leftArray[] = { points[vertexIndex],points[vertexIndex + 1],points[vertexIndex + 2] };
		vertexIndex = center * 3;
		float centerArray[] = { points[vertexIndex], points[vertexIndex + 1],points[vertexIndex + 2] };
		vertexIndex = right * 3;
		float rightArray[] = { points[vertexIndex], points[vertexIndex + 1],points[vertexIndex + 2] };

		/*
		 * Produce normals
		 */
		float *normal = new float[3];
		float *A = new float[3];
		float *B = new float[3];

		/* For Left */
		A[0] = centerArray[0] - leftArray[0];
		A[1] = centerArray[1] - leftArray[1];
		A[2] = centerArray[2] - leftArray[2];
		B[0] = rightArray[0] - leftArray[0];
		B[1] = rightArray[1] - leftArray[1];
		B[2] = rightArray[2] - leftArray[2];
		crossProduct(B, A, normal);
		vertexIndex = left * 3;
		normalMapCube[vertexIndex] = normal[0];
		normalMapCube[vertexIndex+1] = normal[1];
		normalMapCube[vertexIndex+2] = normal[2];

		/* For Center */
		A[0] = leftArray[0] - centerArray[0];
		A[1] = leftArray[1] - centerArray[1];
		A[2] = leftArray[2] - centerArray[2];
		B[0] = rightArray[0] - centerArray[0];
		B[1] = rightArray[1] - centerArray[1];
		B[2] = rightArray[2] - centerArray[2];
		crossProduct(A, B, normal);
		vertexIndex = center * 3;
		normalMapCube[vertexIndex] = normal[0];
		normalMapCube[vertexIndex+1] = normal[1];
		normalMapCube[vertexIndex+2] = normal[2];

		/* For Right */
		A[0] = centerArray[0] - rightArray[0];
		A[1] = centerArray[1] - rightArray[1];
		A[2] = centerArray[2] - rightArray[2];
		B[0] = leftArray[0] - rightArray[0];
		B[1] = leftArray[1] - rightArray[1];
		B[2] = leftArray[2] - rightArray[2];
		crossProduct(A, B, normal);
		vertexIndex = right * 3;
		normalMapCube[vertexIndex] = normal[0];
		normalMapCube[vertexIndex+1] = normal[1];
		normalMapCube[vertexIndex+2] = normal[2];
	}

	for (int i = 0; i < pointArraySize; i += 3) {
		cout << (i/3) << ":Normal (" << normalMapCube[i] <<"," << normalMapCube[i+1] <<"," << normalMapCube[i+2] <<")" << endl;
	}
	GLuint normal_vbo = 0;
	glGenBuffers(1, &normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, pointArraySize * sizeof(float), normalMapCube, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);



	
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

	initLights();

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

}

void drawCubes(int numOfSquares) {

	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	View = glm::lookAt(
		glm::vec3(cameraX, cameraY, cameraZ),		// Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0),			// Camera Looking at the origin
		glm::vec3(0, 1, 0)			// Head is up (set to 0,-1,0 to look upside-down)
	);
	glBindVertexArray(gVAO);
	transformObject(1, Y_AXIS, 0.0f, glm::vec3(0.0, 0.0, 0.0));
	glDrawElements(GL_TRIANGLES, numberOfSquares * numberOfSquares * 2 * 3, GL_UNSIGNED_SHORT, 0);

}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
		/*
	case 'i':
	case 'I':
		lightZ -= lightStep;
		break;
	case 'k':
	case 'K':
		lightZ += lightStep;
		break;
	case 'j':
	case 'J':
		lightX -= lightStep;
		break;
	case 'l':
	case 'L':
		lightX += lightStep;
		break;
		*/
	default:
		break;
	}
}

int main(int argc, char** argv)
{	
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
