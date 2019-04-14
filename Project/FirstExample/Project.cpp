
//****************************************************************************
// Project.cpp by Greasy Beacon (C) 2019 All Rights Reserved.
//
// Greasy Bacon is
// Ekin Karayalcin - 101116005 
// Ricardo Shimoda Nakasako - 10128885
//
// Final Project submission.
//
// Description:
// Click run to see the results.
//
//*****************************************************************************
///////////////////////////////////////////////////////////////////////
//
// Wowza
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
#include "GeometryGenerator.h"
#include<iostream>

float rotationAngle = 45.0f;
float rotationSpeed = 0.2f;

const int screenSize = 848;


float cameraX = 0.0f;
float cameraY = 5.0f; /* Just a nicer perspective than 3,3,4, you know */
float cameraZ = 10.0f;
float cameraStep = 0.1f;

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)

GLuint program;

// list of VAOs
GLuint cubeVAO, pyramidVAO, rubiksVAO, starVAO, wedgeVAO, triPrismVAO, gVAOPyramid;

GLuint MatrixID;
glm::mat4 MVP;
glm::mat4 View;
glm::mat4 Projection;

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

	GeometryGenerator gg;
	gg.CreateRubiksCube(1.3, &rubiksVAO);
	gg.CreatePyramid(&pyramidVAO);
	gg.CreateStar(&starVAO);
	gg.CreateWedge(&wedgeVAO);
	gg.CreateTriPrism(&triPrismVAO);
	gg.CreatePyramid(&pyramidVAO);

	/*
	 * Reads the information of the sole texture
	 */
	GLint widthBonus, heightBonus;

	unsigned char* imageBonus = SOIL_load_image("bonusTexture.png", &widthBonus, &heightBonus, 0, SOIL_LOAD_RGBA);
	GLuint pyramid_tex = 0;
	glGenTextures(1, &pyramid_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pyramid_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthBonus, heightBonus, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBonus);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

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

void drawSolids() {

	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	View = glm::lookAt(
		glm::vec3(cameraX, cameraY, cameraZ),		// Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0),			// Camera Looking at the origin
		glm::vec3(0, 1, 0)			// Head is up (set to 0,-1,0 to look upside-down)
	);

	//cubeVAO, pyramidVAO, rubiksVAO, starVAO, wedgeVAO, triPrismVAO, gVAOPyramid
	glBindVertexArray(cubeVAO);
	transformObject(1, Y_AXIS, rotationAngle, glm::vec3(-3.0, 0.0, 0.0));
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
	drawSolids();
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
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glewExperimental = true;
	glewInit();    //Initializes the glew and prepares the drawing pipeline.

	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardFun);
	glutIdleFunc(idle);
	glutMainLoop();
}
