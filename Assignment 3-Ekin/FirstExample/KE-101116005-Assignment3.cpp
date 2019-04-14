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

GLuint MatrixID;
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
GLuint cubeVAO, pyramidVAO, rubiksVAO, starVAO, wedgeVAO, triPrismVAO;

/*
All the textures
*/
GLuint wedge_tex, triPrism_tex, star_tex, pyramid_tex, cube_tex;
int starIndexSize;

float cameraX = 0, cameraY = 1, cameraZ = 8;
float cameraSpeed = 1;
float rotAngle = 0;

GLint width, height;
unsigned char* image;


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