//***************************************************************************
// Ekin Karayalcin.
//
// Assignment 3 submission.
//
// Description:
//   Draws a rotating rubix cube with textures applied.
//*****************************************************************************

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
#define PI 3.14159265

GLuint MatrixID, ViewID, ModelID;
glm::mat4 MVP, View, Projection;
/*
GLuint cube_points_vbo, cube_colours_vbo, ;
GLuint pyramid_points_vbo, pyramid_colours_vbo, ;
GLuint rubiks_points_vbo, rubiks_textures_vbo;
GLuint star_points_vbo, star_textures_vbo, ;
GLuint triPrism_points_vbo, triPrism_textures_vbo, ;
GLuint wedge_points_vbo, wedge_textures_vbo, ;

*/
GLuint program;

/*
All the VAOs
*/
GLuint cubeVAO, pyramidVAO, rubiksVAO, starVAO, wedgeVAO, triPrismVAO, cylinderVAO;

/*
All the textures
*/
GLuint wedge_tex, triPrism_tex, star_tex, pyramid_tex, cube_tex;
int starIndexSize, cylinderIndexSize;

float cameraX = 0, cameraY = 1, cameraZ = 8;
float cameraSpeed = 1;
float rotAngle = 0;

GLint width, height;
unsigned char* image;

struct Light {
	GLuint colorHandle;
	GLuint posHandle;
	GLuint strengthHandle;
	GLuint falloffStartHandle;
	GLuint falloffEndHandle;
};

Light   pointLights[2];

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
	starIndexSize = gg.CreateStar(&starVAO);

	PassTextureToGPU("bonusTexture.png", wedge_tex);
	gg.CreateWedge(&wedgeVAO);

	PassTextureToGPU("bonusTexture.png", triPrism_tex);
	gg.CreateTriPrism(&triPrismVAO);


	cylinderIndexSize = gg.CreateCylinder(&cylinderVAO);

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
	glBindVertexArray(wedgeVAO);
	glBindTexture(GL_TEXTURE_2D, wedge_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(-2, 0, 0));
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(triPrismVAO);
	glBindTexture(GL_TEXTURE_2D, triPrism_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(0, 0, 0));
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(starVAO);
	glBindTexture(GL_TEXTURE_2D, star_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(2, 0, 0));
	glDrawElements(GL_TRIANGLES, starIndexSize, GL_UNSIGNED_SHORT, 0);
	*/
	glBindVertexArray(cylinderVAO);
	glBindTexture(GL_TEXTURE_2D, star_tex);
	transformObject(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), rotAngle, glm::vec3(0, 0, 0));
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);


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
	glutCreateWindow("Karayalcin Ekin 101116005");
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