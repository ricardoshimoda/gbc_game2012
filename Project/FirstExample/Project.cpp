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
GLuint blackTex, bushTex, darkerGreyTex, darkGreyTex, greyTex, goldTex, grassTex, treeTex, waterTex, woodTex, redTex;

GLint width, height;
unsigned char* image;
/************ END TEXTURES ************/

/************ CAMERA POSITION ************/
glm::vec3 cameraPosition(-4.0f, 105.0f, 89.0f);
float cameraStep = 0.1f;
float cameraX = 85.0f, cameraY = 51.0f, cameraZ = 40.0f;
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

	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10000.0f);
	View = glm::lookAt
	(
		glm::vec3(cameraX, cameraY, cameraZ), // position of the camera (positive Z-Axis is outwwards of the screen)
		glm::vec3(0, 0, 0), // lookat position (what the camera is looking at)
		glm::vec3(0, 1, 0)  // up vector of the camera (orientation/rotation of the camera)
	);


	PassTextureToGPU("black.jpg", blackTex);
	PassTextureToGPU("red.jpg", redTex);
	PassTextureToGPU("Bush.png", bushTex);
	PassTextureToGPU("DarkerGrey.jpg", darkerGreyTex);
	PassTextureToGPU("DarkGrey.jpg", darkGreyTex);
	PassTextureToGPU("Gold.jpg", goldTex);
	PassTextureToGPU("Grass.jpg", grassTex);
	PassTextureToGPU("Grey.jpg", greyTex);
	PassTextureToGPU("Tree.jpg", treeTex);
	PassTextureToGPU("Water.jpg", waterTex);
	PassTextureToGPU("Wood.png", woodTex);

	GeometryGenerator gg;
	starTriangles = gg.CreateStar(&starVAO);
	cubeTriangles = gg.CreateCube(&cubeVAO);
	simsIndicatorTriangles = gg.CreateSimsIndicator(&simsIndicatorVAO);
	cylinderTriangles = gg.CreateCylinder(&cylinderVAO);
	coneTriangles = gg.CreateCone(&coneVAO);
	sphereTriangles = gg.CreateSphere(&sphereVAO);
	pyramidTriangles = gg.CreatePyramid(&pyramidVAO);

	wedgeTriangles = gg.CreateWedge(&wedgeVAO);
	triPrismTriangles = gg.CreateTriPrism(&triPrismVAO);
	hexagonTriangles = gg.CreateHexagon(&hexagonVAO);

	initLights();
}

void transformObject(glm::vec3 scale, glm::vec3 rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle.x), X_AXIS);
	Model = glm::rotate(Model, glm::radians(rotationAngle.y), Y_AXIS);
	Model = glm::rotate(Model, glm::radians(rotationAngle.z), Z_AXIS);
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

	// LAKE
	glBindVertexArray(cubeVAO);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	transformObject(glm::vec3(75, 9.26, 75), glm::vec3(0, 0, 0), glm::vec3(0, -8.4, 0));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);

	// GRASS
	glBindTexture(GL_TEXTURE_2D, grassTex);
	transformObject(glm::vec3(50, 9.26, 50), glm::vec3(0, 0, 0), glm::vec3(0, -4.53, 0));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(150, 9.26, 50), glm::vec3(0, 0, 0), glm::vec3(0, -4.53, 60.3));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(150, 9.26, 50), glm::vec3(0, 0, 0), glm::vec3(0, -4.53, -59.2));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(38.52, 9.26, 100), glm::vec3(0, 0, 0), glm::vec3(55.74, -4.53, 0));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(38.52, 9.26, 100), glm::vec3(0, 0, 0), glm::vec3(-55.56, -4.53, 0));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);

	// Bridge
	glBindTexture(GL_TEXTURE_2D, woodTex);
	transformObject(glm::vec3(19.05, 1, 8.33), glm::vec3(0, 0, 0), glm::vec3(30.24, 0.39, 0));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);

	// Walls
	glBindTexture(GL_TEXTURE_2D, greyTex);
	transformObject(glm::vec3(27.86, 1.76, 44.744), glm::vec3(0, 180, 90), glm::vec3(-22.27, 2, 0.144));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(27.86, 1.76, 44.744), glm::vec3(0, 0, 90), glm::vec3(20.74, 2, 0.144));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(27.86, 1.76, 44.744), glm::vec3(0, 90, 90), glm::vec3(-0.72, 2, -21.68));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(27.86, 1.76, 44.744), glm::vec3(0, 270, 90), glm::vec3(-0.72, 2, 21.49));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);

	// Central Cube
	transformObject(glm::vec3(23, 15, 23), glm::vec3(0, 180, 90), glm::vec3(0, 4.4, 0));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);

	// Portal
	glBindTexture(GL_TEXTURE_2D, blackTex);
	transformObject(glm::vec3(19.05, 1.95, 8.32), glm::vec3(0, 0, 0), glm::vec3(20.76, -2.4, 0));
	glDrawElements(GL_TRIANGLES, cubeTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(cylinderVAO);
	transformObject(glm::vec3(8.38, 0.96, 8.38), glm::vec3(0, 0, 90), glm::vec3(20.76, 7.02, 0.011));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);

	// Towers
	glBindTexture(GL_TEXTURE_2D, darkGreyTex);
	transformObject(glm::vec3(4, 40, 4), glm::vec3(0, 0, 0), glm::vec3(20.8, 2.73, -21.5));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(4, 40, 4), glm::vec3(0, 0, 0), glm::vec3(-22.21, 2.73, -21.5));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(4, 40, 4), glm::vec3(0, 0, 0), glm::vec3(20.8, 2.73, 21.67));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(4, 40, 4), glm::vec3(0, 0, 0), glm::vec3(-20.19, 2.73, 21.67));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);

	glBindTexture(GL_TEXTURE_2D, darkerGreyTex);
	transformObject(glm::vec3(5, 0.3, 5), glm::vec3(0, 0, 0), glm::vec3(20.8, 22.83, -21.5));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(5, 0.3, 5), glm::vec3(0, 0, 0), glm::vec3(-22.21, 22.83, -21.5));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(5, 0.3, 5), glm::vec3(0, 0, 0), glm::vec3(20.8, 22.83, 21.67));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(5, 0.3, 5), glm::vec3(0, 0, 0), glm::vec3(-20.19, 22.83, 21.67));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);

	glBindTexture(GL_TEXTURE_2D, woodTex);
	transformObject(glm::vec3(1, 10, 1), glm::vec3(0, 0, 0), glm::vec3(45, 2, -20));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(1, 10, 1), glm::vec3(0, 0, 0), glm::vec3(45, 2, -10));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(1, 10, 1), glm::vec3(0, 0, 0), glm::vec3(45, 2, 0));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(1, 10, 1), glm::vec3(0, 0, 0), glm::vec3(45, 2, 10));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(1, 10, 1), glm::vec3(0, 0, 0), glm::vec3(45, 2, 20));
	glDrawElements(GL_TRIANGLES, cylinderTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(coneVAO);
	glBindTexture(GL_TEXTURE_2D, bushTex);
	transformObject(glm::vec3(5, 10, 5), glm::vec3(0, 0, 0), glm::vec3(45, 10, -20));
	glDrawElements(GL_TRIANGLES, coneTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(5, 10, 5), glm::vec3(0, 0, 0), glm::vec3(45, 10, -10));
	glDrawElements(GL_TRIANGLES, coneTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(5, 10, 5), glm::vec3(0, 0, 0), glm::vec3(45, 10, 0));
	glDrawElements(GL_TRIANGLES, coneTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(5, 10, 5), glm::vec3(0, 0, 0), glm::vec3(45, 10, 10));
	glDrawElements(GL_TRIANGLES, coneTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(5, 10, 5), glm::vec3(0, 0, 0), glm::vec3(45, 10, 20));
	glDrawElements(GL_TRIANGLES, coneTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(sphereVAO);
	glBindTexture(GL_TEXTURE_2D, darkGreyTex);
	transformObject(glm::vec3(15, 15, 15), glm::vec3(0, 0, 0), glm::vec3(0, 11.91, 0));
	glDrawElements(GL_TRIANGLES, sphereTriangles, GL_UNSIGNED_SHORT, 0);


	glBindVertexArray(simsIndicatorVAO);
	glBindTexture(GL_TEXTURE_2D, goldTex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(90, 0, 0), glm::vec3(21.55, 5.9, -11));
	glDrawElements(GL_TRIANGLES, simsIndicatorTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(90, 0, 0), glm::vec3(21.55, 5.9, 11));
	glDrawElements(GL_TRIANGLES, simsIndicatorTriangles, GL_UNSIGNED_SHORT, 0);


	glBindVertexArray(starVAO);
	glBindTexture(GL_TEXTURE_2D, goldTex);
	transformObject(glm::vec3(15, 15, 1), glm::vec3(0, 90, 0), glm::vec3(21.55, 5.9, 0));
	glDrawElements(GL_TRIANGLES, starTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(pyramidVAO);
	transformObject(glm::vec3(3, 5, 3), glm::vec3(0, 0, 0), glm::vec3(20.8, 24.83, -21.5));
	glDrawElements(GL_TRIANGLES, pyramidTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(3, 5, 3), glm::vec3(0, 0, 0), glm::vec3(-22.21, 24.83, -21.5));
	glDrawElements(GL_TRIANGLES, pyramidTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(3, 5, 3), glm::vec3(0, 0, 0), glm::vec3(20.8, 24.83, 21.67));
	glDrawElements(GL_TRIANGLES, pyramidTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(3, 5, 3), glm::vec3(0, 0, 0), glm::vec3(-20.19, 24.83, 21.67));
	glDrawElements(GL_TRIANGLES, pyramidTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(hexagonVAO);
	transformObject(glm::vec3(50, 5, 50), glm::vec3(0, 0, 0), glm::vec3(0, 40, 0));
	glDrawElements(GL_TRIANGLES, hexagonTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(wedgeVAO);
	glBindTexture(GL_TEXTURE_2D, redTex);
	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(39.35, 0.26, 5.51));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(41.35, 0.26, 5.51));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(43.35, 0.26, 5.51));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(45.35, 0.26, 5.51));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(47.35, 0.26, 5.51));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);

	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(39.35, 0.26, -2.58));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(41.35, 0.26, -2.58));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(43.35, 0.26, -2.58));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(45.35, 0.26, -2.58));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);
	transformObject(glm::vec3(2, 2, 2), glm::vec3(0, 90, 0), glm::vec3(47.35, 0.26, -2.58));
	glDrawElements(GL_TRIANGLES, wedgeTriangles, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(triPrismVAO);
	transformObject(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 20, 0));
	glDrawElements(GL_TRIANGLES, triPrismTriangles, GL_UNSIGNED_SHORT, 0);

	/*

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

	glBindVertexArray(sphereVAO);
	glBindTexture(GL_TEXTURE_2D, hexagon_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, rotAngle, 0), glm::vec3(0, 0, 0));
	glDrawElements(GL_TRIANGLES, sphereTriangles, GL_UNSIGNED_SHORT, 0);
	*/


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
