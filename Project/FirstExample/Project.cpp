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
// The Castle
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "math.h"
#include <SOIL.h>
#include "GeometryGenerator.h"

GLuint program;

/************ TRANSFORMATION MATRIX ************/
GLuint MatrixID, ViewID, ModelID;
glm::mat4 MVP, View, Projection;
/************ END TRANSFORMATION MATRIX ************/

/************ VAOs AND DRAWING VARIABLES ************/
GLuint cubeVAO,
       pyramidVAO, 
	   starVAO, 
	   wedgeVAO, 
	   triPrismVAO, 
	   hexagonVAO, 
	   simsIndicatorVAO, 
	   cylinderVAO,
	   sphereVAO,
	   coneVAO;

int cubeTriangles,
    pyramidTriangles,
	starTriangles,
	wedgeTriangles,
	triPrismTriangles,
	hexagonTriangles,
	simsIndicatorTriangles,
	cylinderTriangles,
	sphereTriangles,
	coneTriangles;

/************ END VAOs AND DRAWING VARIABLES ************/

/************ TEXTURES ************/
GLuint wedge_tex, triPrism_tex, star_tex, pyramid_tex, cube_tex, hexagon_tex;

GLint width, height;
unsigned char* image;
/************ END TEXTURES ************/

/************ CAMERA POSITION ************/
glm::vec3 cameraPosition(0.0f, 5.0f, 10.0f);
float cameraStep = 0.1f;
float cameraX = 0, cameraY = 1, cameraZ = 8;
float cameraSpeed = 1;
float rotAngle = 0;
/************ END CAMERA POSITION AND LOOK AT ************/

/************ CONSTANTS ************/
#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define ORIGIN glm::vec3(0,0,0)
/************ END CONSTANTS ************/


/************ LIGHTING ************/
struct Light {
	GLuint colorHandle;
	GLuint posHandle;
	GLuint strengthHandle;
	GLuint falloffStartHandle;
	GLuint falloffEndHandle;
};
Light   pointLights[2];
/************ END LIGHTING ************/

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
	glUniform3fv(pointLights[0].posHandle, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform1f(pointLights[0].strengthHandle, 1.0f);
	glUniform1f(pointLights[0].falloffStartHandle, 1.0f);
	glUniform1f(pointLights[0].falloffEndHandle, 50.0f);

	glUniform3fv(pointLights[1].colorHandle, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform3fv(pointLights[1].posHandle, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform1f(pointLights[1].strengthHandle, 1.0f);
	glUniform1f(pointLights[1].falloffStartHandle, 1.0f);
	glUniform1f(pointLights[1].falloffEndHandle, 50.0f);
}

void MyKeyboardFunc(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	case 'W':
		cameraZ -= cameraSpeed;
		break;
	case 'w':
		cameraZ -= cameraSpeed;
		break;
	case 'S':
		cameraZ += cameraSpeed;
		break;
	case 's':
		cameraZ += cameraSpeed;
		break;
	case 'A':
		cameraX -= cameraSpeed;
		break;
	case 'a':
		cameraX -= cameraSpeed;
		break;
	case 'D':
		cameraX += cameraSpeed;
		break;
	case 'd':
		cameraX += cameraSpeed;
		break;
	case 'F':
		cameraY -= cameraSpeed;
		break;
	case 'f':
		cameraY -= cameraSpeed;
		break;
	case 'R':
		cameraY += cameraSpeed;
		break;
	case 'r':
		cameraY += cameraSpeed;
		break;
	};
}

void PassTextureToGPU(char* fileName, GLuint& tex)
{
	image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGB);
	tex = 0;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
}

void init(void)
{
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};
	program = LoadShaders(shaders);
	glUseProgram(program);

	MatrixID = glGetUniformLocation(program, "MVP");
	ViewID = glGetUniformLocation(program, "V");
	ModelID = glGetUniformLocation(program, "M");

	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	View = glm::lookAt
	(
		glm::vec3(cameraX, cameraY, cameraZ), // position of the camera (positive Z-Axis is outwwards of the screen)
		glm::vec3(0, 0, 0), // lookat position (what the camera is looking at)
		glm::vec3(0, 1, 0)  // up vector of the camera (orientation/rotation of the camera)
	);

	GeometryGenerator gg;
	/*
	PassTextureToGPU("rubiksTexture.png", cube_tex);
	gg.CreateRubiksCube(1.3, &rubiksVAO);

	PassTextureToGPU("bonusTexture.png", pyramid_tex);
	gg.CreatePyramid(&pyramidVAO);
	*/

	PassTextureToGPU("bonusTexture.png", star_tex);
	starTriangles = gg.CreateStar(&starVAO);

	PassTextureToGPU("bonusTexture.png", wedge_tex);
	wedgeTriangles = gg.CreateWedge(&wedgeVAO);

	PassTextureToGPU("bonusTexture.png", triPrism_tex);
	triPrismTriangles = gg.CreateTriPrism(&triPrismVAO);

	PassTextureToGPU("bonusTexture.png", cube_tex);
	cubeTriangles = gg.CreateCube(&cubeVAO);

	PassTextureToGPU("bonusTexture.png", pyramid_tex);
	pyramidTriangles = gg.CreatePyramid(&pyramidVAO);

	PassTextureToGPU("wicker.jpg", hexagon_tex);
	hexagonTriangles = gg.CreateHexagon(&hexagonVAO);

	simsIndicatorTriangles = gg.CreateSimsIndicator(&simsIndicatorVAO);

	cylinderTriangles = gg.CreateCylinder(&cylinderVAO);

	coneTriangles = gg.CreateCone(&coneVAO);

	sphereTriangles = gg.CreateSphere(&sphereVAO);
	initLights();
}

void transformObject(glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, scale);

	MVP = Projection * View * Model;
	MatrixID = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2, 0.2, 0.2, 0.2);

	View = glm::lookAt
	(
		glm::vec3(cameraX, cameraY, cameraZ),	// position of the camera (positive Z-Axis is outwwards of the screen)
		glm::vec3(0, 0, 0),						// lookat position (what the camera is looking at)
		glm::vec3(0, 1, 0)						// up vector of the camera (orientation/rotation of the camera)
	);
	/*
	glBindVertexArray(cubeVAO);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(-4, 0, 0));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(wedgeVAO);
	glBindTexture(GL_TEXTURE_2D, wedge_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(-2, 0, 0));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(triPrismVAO);
	glBindTexture(GL_TEXTURE_2D, triPrism_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(0, 0, 0));
	glDrawElements(GL_TRIANGLES, triPrismTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(starVAO);
	glBindTexture(GL_TEXTURE_2D, star_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(2, 0, 0));
	glDrawElements(GL_TRIANGLES, starTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(pyramidVAO);
	glBindTexture(GL_TEXTURE_2D, pyramid_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(4, 0, 0));
	glDrawElements(GL_TRIANGLES, pyramidTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(hexagonVAO);
	glBindTexture(GL_TEXTURE_2D, hexagon_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(0, 2, 0));
	glDrawElements(GL_TRIANGLES, hexagonTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(simsIndicatorVAO);
	glBindTexture(GL_TEXTURE_2D, hexagon_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(0, 2, 0));
	glDrawElements(GL_TRIANGLES, hexagonTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(cylinderVAO);
	glBindTexture(GL_TEXTURE_2D, hexagon_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(0, 0, 0));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(coneVAO);
	glBindTexture(GL_TEXTURE_2D, hexagon_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(0, 0, 0));
	glDrawElements(GL_TRIANGLES, coneTriangles, GL_UNSIGNED_SHORT, 0);

	*/

	glBindVertexArray(sphereVAO);
	glBindTexture(GL_TEXTURE_2D, hexagon_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(0, 0, 0));
	glDrawElements(GL_TRIANGLES, sphereTriangles, GL_UNSIGNED_SHORT, 0);

	rotAngle += 0.75f;
	glutSwapBuffers();
}

void Timer(int id)
{
	glutPostRedisplay();
	glutTimerFunc(33, Timer, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Greasy Bacon - The Castle");
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glutKeyboardFunc(MyKeyboardFunc);

	init();
	glutDisplayFunc(display);
	glutTimerFunc(33, Timer, 0);
	glutMainLoop();
}
