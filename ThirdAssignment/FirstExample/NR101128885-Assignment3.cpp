
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
int numberOfSquares = 1;
float rotationAngleCube1 = 45.0f;
float rotationAngleCube2 = -90.0f;
float rotationSpeed = 0.2f;
const int screenSize = 848;
float halfCubeSide = 0.65f;

float halfPyramidSide = 0.65f;
float pyramidHeight = 2.0f;

float cameraX = 0.0f;
float cameraY = 5.0f; /* Just a nicer perspective than 3,3,4, you know */
float cameraZ = 10.0f;
float cameraStep = 0.1f;

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)

GLuint gVAO;
GLuint gVAOPyramid;

GLuint MatrixID;
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
	float points[] = {
		/*
		 * This is the first set - from 0 to 7
		 */
		-halfCubeSide, -halfCubeSide,  halfCubeSide,	// 0,1                            
		 halfCubeSide, -halfCubeSide,  halfCubeSide,	// 1,1
		 halfCubeSide,  halfCubeSide,  halfCubeSide,	// 1,0
		-halfCubeSide,  halfCubeSide,  halfCubeSide,	// 0,0
		-halfCubeSide, -halfCubeSide, -halfCubeSide,	// 0,1
		 halfCubeSide, -halfCubeSide, -halfCubeSide,	// 1,1
		 halfCubeSide,  halfCubeSide, -halfCubeSide,	// 1,0
		-halfCubeSide,  halfCubeSide, -halfCubeSide,	// 0,0
		/*
		 * This is the second set - from 8 to 15
		 */
		-halfCubeSide, -halfCubeSide,  halfCubeSide,	// 0,1
		 halfCubeSide, -halfCubeSide,  halfCubeSide,	// 1,1
		 halfCubeSide,  halfCubeSide,  halfCubeSide,	// 1,0
		-halfCubeSide,  halfCubeSide,  halfCubeSide,	// 0,0
		-halfCubeSide, -halfCubeSide, -halfCubeSide,	// 0,1
		 halfCubeSide, -halfCubeSide, -halfCubeSide,	// 1,1
		 halfCubeSide,  halfCubeSide, -halfCubeSide,	// 1,0
		-halfCubeSide,  halfCubeSide, -halfCubeSide,	// 0,0
		/*
		 * This is the third set - from 16 to 23
		 */
		-halfCubeSide, -halfCubeSide,  halfCubeSide,	// 0,1
		 halfCubeSide, -halfCubeSide,  halfCubeSide,	// 1,1
		 halfCubeSide,  halfCubeSide,  halfCubeSide,	// 1,0
		-halfCubeSide,  halfCubeSide,  halfCubeSide,	// 0,0
		-halfCubeSide, -halfCubeSide, -halfCubeSide,	// 0,1
		 halfCubeSide, -halfCubeSide, -halfCubeSide,	// 1,1
		 halfCubeSide,  halfCubeSide, -halfCubeSide,	// 1,0
		-halfCubeSide,  halfCubeSide, -halfCubeSide		// 0,0
	};

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/*
	 * Coloring it all black
	 */
	float colors[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	/*
	 * now we establish the array for mapping the texture
	 */
	/*
	 * This is the array for uv mapping - not a rubik, just unrolled stuff
	 */
	/*
	float texMapCube[] = {
		0.f,1.f, // Front_1
		1.f,1.f, // Front_2
		1.f,0.f, // Front_3
		0.f,0.f, // Front_4

		0.f,1.f, // Back_1 
		1.f,1.f, // Back_2
		1.f,0.f, // Back_3
		0.f,0.f, // Back_4

		0.f,1.f, // Bottom_1 
		0.f,0.f, // Botton_4
		1.f,1.f, // Top_2
		0.f,1.f, // Top_1
		1.f,1.f, // Bottom_2
		1.f,0.f, // Bottom_3
		1.f,0.f, // Top_3
		0.f,0.f, // top_4

		0.f,0.f, // Left_4
		0.f,1.f, // Right_1
		0.f,0.f, // Right_4
		0.f,1.f, // Left_1
		1.f,0.f, // Left_3
		1.f,1.f, // Right_2
		1.f,0.f, // Right_3
		1.f,1.f  // Left_2
	};
	*/

	float texMapCube[] = {
		0.25f,1.f, // 00
		0.5f,1.f, // 01
		0.5f,2.0/3.0f, // 02
		0.25f,2.0/3.0f, // 03

		0.25f,0.f, // 04 
		0.5f,0.f, // 05
		0.5f,1.0/3.0f, // 06
		0.25f,1.0/3.0f, // 07

		1.0f,2.0/3.0f, // 08 
		0.75f,2.0/3.0f, // 09

		0.5f,2.0/3.0f, // 10
		0.25f,2.0/3.0f, // 11
		1.f,1.0/3.0f, // 12
		0.75f,1.0/3.0f, // 13
		0.5f,1.0/3.0f, // 14
		0.25f,1.0/3.0f, // 15

		0.f,2.0/3.f, // 16
		0.75f,2.0/3.f, // 17
		0.5f,2.0/3.f, // 18
		0.25f,2.0/3.f, // 19
		0.f,1.0/3.0f, // 20
		0.75f,1.0/3.f, // 21
		0.5f,1.0/3.f, // 22
		0.25f,1.0/3.f  // 23
	};


	GLuint texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(texMapCube), texMapCube, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	/*
	 * Faces have been defined counter-clockwise (just because I knid of prefer this way)
	 */
	GLushort cube_index_array[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// back
		4, 7, 6,
		6, 5, 4,
		// top
		11, 10, 14,
		14, 15, 11, 
		// bottom
		8, 12, 13,
		13, 9, 8,
		// right
		17, 21, 22,
		22, 18, 17,
		// left
		19, 23, 20,
		20, 16, 19
	};

	GLuint cube_IBO;
	glGenBuffers(1, &cube_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index_array), cube_index_array, GL_STATIC_DRAW);

	/*
	 * Changing the VBO, now we are going for the pyramids
	 */
	glGenVertexArrays(1, &gVAOPyramid);
	glBindVertexArray(gVAOPyramid);
	float pyramidPoints[] = {
		-halfPyramidSide, 0.0f,  halfPyramidSide, // 00-FRONT_1
		 halfPyramidSide, 0.0f,  halfPyramidSide, // 01-FRONT_2
		 halfPyramidSide, 0.0f, -halfPyramidSide, // 02-BACK_1
		-halfPyramidSide, 0.0f, -halfPyramidSide, // 03-BACK_2
		 0.0f, pyramidHeight, 0.0f,				  // 04-TOP_1
		-halfPyramidSide, 0.0f,  halfPyramidSide, // 05-LEFT_2
		 halfPyramidSide, 0.0f,  halfPyramidSide, // 06-RIGHT_1
		 halfPyramidSide, 0.0f, -halfPyramidSide, // 07-RIGHT_2
		-halfPyramidSide, 0.0f, -halfPyramidSide, // 08-LEFT_1
		 0.0f, pyramidHeight, 0.0f,				  // 09-TOP_2
		-halfPyramidSide, 0.0f,  halfPyramidSide, // 10-BOTTOM_1
		 halfPyramidSide, 0.0f,  halfPyramidSide, // 11-BOTTOM_2
		 halfPyramidSide, 0.0f, -halfPyramidSide, // 12-BOTTOM_3
		-halfPyramidSide, 0.0f, -halfPyramidSide  // 13-BOTTOM_4
	};

	GLuint pyramid_points_vbo = 0;
	glGenBuffers(1, &pyramid_points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_points_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPoints), pyramidPoints, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	float pyramid_colors[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	GLuint pyramid_colors_vbo = 0;
	glGenBuffers(1, &pyramid_colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_colors_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_colors), pyramid_colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	float texMapPyramid[] = {
		// Front and back
		0.f, 1.f,	// 00
		1.f, 1.f,	// 01
		0.f, 1.f,	// 02
		1.f, 1.f,	// 03
		0.5f, 0.f, // 04 

		1.f,1.f, // 05
		0.f,1.f, // 06
		1.f,1.f, // 07
		0.f,1.f, // 08 
		0.5f,0.f, // 09

		0.f, 0.f, // 10
		0.f,1.f, // 11
		1.f,1.f, // 12
		1.f,0.f // 13
	};

	GLuint pyramid_texture_vbo = 0;
	glGenBuffers(1, &pyramid_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_texture_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(texMapPyramid), texMapPyramid, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	/*
	 * Faces have been defined counter-clockwise (just because I kind of prefer this way)
	 */
	
	GLushort pyramid_index_array[] = {
		// front
		0,1,4,
		// back
		2,3,4,
		// left
		8,5,9,
		// right
		6,7,9,
		// bottom
		10,13,12,
		12,11,10
	};

	GLuint pyramid_IBO;
	glGenBuffers(1, &pyramid_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramid_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_index_array), pyramid_index_array, GL_STATIC_DRAW);
	
	/*
	 * Reads the information of textures
	 */
	GLint widthRubik, heightRubik, widthBonus, heightBonus;
	unsigned char* imageRubik = SOIL_load_image("rubiksTexture.png", &widthRubik, &heightRubik, 0, SOIL_LOAD_RGB);
	unsigned char* imageBonus = SOIL_load_image("bonusTexture.png", &widthBonus, &heightBonus, 0, SOIL_LOAD_RGB);

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

	GLuint pyramid_tex = 0;
	glGenTextures(1, &pyramid_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pyramid_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthBonus, heightBonus, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBonus);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 1);

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

void drawCubes(int numOfSquares) {

	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	View = glm::lookAt(
		glm::vec3(cameraX, cameraY, cameraZ),		// Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0),			// Camera Looking at the origin
		glm::vec3(0, 1, 0)			// Head is up (set to 0,-1,0 to look upside-down)
	);
	glBindVertexArray(gVAO);
	transformObject(1, Y_AXIS, rotationAngleCube1, glm::vec3(-3.0, 0.0, 0.0));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	transformObject(1, Y_AXIS, rotationAngleCube2, glm::vec3(3.0, 0.0, 0.0));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	rotationAngleCube1 += rotationSpeed;
	if (rotationAngleCube1 == 360)
		rotationAngleCube1 = 0;

	rotationAngleCube2 -= rotationSpeed;
	if (rotationAngleCube2 == 0)
		rotationAngleCube2 = 360;

	/*
	-- This draws the plane
	transformDistortedObject(4.0f, 0.2f, 4.0f, Y_AXIS, 0, glm::vec3(0, -1.00, 0));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	*/
	//-- This draws the pyramid
	glUniform1i(glGetUniformLocation(program, "texture0"), 1);
	glBindVertexArray(gVAOPyramid);
	transformObject(1.0f, Y_AXIS, 0, glm::vec3(0, 0.0, 0));// pyramid in the middle
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0);
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
