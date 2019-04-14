//*********************************************************************************
// 3D Printer OS.cpp by GH (C) 4023 All Rights Reserved.
//
// Main operating system file for the ships 3D printer.
// Author: Ricardo Shimoda Nakasako - 101128885 
//
// Description:
//   Running this software will print the contents that are rendered on the screen
//*********************************************************************************


#include "stdlib.h"
#include "iostream"

#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"


#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
void keyOperations();

GLuint gVAO;
GLuint gVAOIndicator;
GLuint MatrixID;
glm::mat4 MVP;
glm::mat4 View;
glm::mat4 Projection;

glm::vec3 currentCamPos;
glm::vec3 currentCamVel;

float rotationSpeed = 0.2f;
float rotationGreenIndicator = 0.0f;

float upAndDown = 0.1;
float upAndDownStep = 0.01;
bool directionUp = true;


int frame = 0, currentTime, timebase = 0;
float deltaTime = 0;
bool keyStates[256] = {}; // Create an array of boolean values of length 256 (0-255)

void init(void) {
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
	{ GL_FRAGMENT_SHADER, "triangles.frag" },
	{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);    //My Pipeline is set up
							

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(program, "MVP");

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	currentCamPos = glm::vec3(0.0f, 0.0f, -4.0f);
	currentCamVel = glm::vec3(0.0f);

	// Camera matrix
	View = glm::lookAt(
		currentCamPos, // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// this is the VAO for the first solid
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	float sdDist = 10 * sqrt(3);
	float points[] = {
		-5.0f, 5.0f,              3.0f,
		 5.0f, 5.0f,              3.0f,
		10.0f, 5.0f - sdDist/2,   3.0f,
		 5.0f, 5 - sdDist,        3.0f,
		-5.0f, 5 - sdDist,        3.0f,
	   -10.0f, 5 - sdDist/2,        3.0f,

		-5.0f, 5.0f,             0.0f,
		 5.0f, 5.0f,             0.0f,
		10.0f, 5.0f - sdDist/2,  0.0f,
		 5.0f, 5 - sdDist,       0.0f,
		-5.0f, 5 - sdDist,       0.0f,
	   -10.0f, 5 - sdDist/2,     0.0f
	};

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	float colors[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	/*
	* Faces have been defined counter-clockwise (just because I knid of prefer this way)
	*/
	GLushort penta_index_array[] = {
		// top
		0, 1, 3,
		1, 2, 3,
		0, 3, 5,
		3, 4, 5,
		//bottom
		0+6,3+6,1+6,
		1+6,3+6,2+6,
		5+6,3+6,0+6,
		5+6,4+6,3+6,
		// sides
		6,7,1,
		1,0,6,
		7,8,2,
		2,1,7,
		8,9,3,
		3,2,8,
		9,10,4,
		4,3,9,
		10,11,5,
		5,4,10,
		11,6,0,
		0,5,11
	};

	GLuint penta_IBO;
	glGenBuffers(1, &penta_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, penta_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(penta_index_array), penta_index_array, GL_STATIC_DRAW);



	glGenVertexArrays(1, &gVAOIndicator);
	glBindVertexArray(gVAOIndicator);


	//OMG OMG OMG OMG OMG OMG
	float sdDist2 = 5 * sqrt(3) / 2;
	float newYOrigin = 5 - 5 * sqrt(3) - sdDist2;
	float indicator[] = {
		-2.5f, newYOrigin + sdDist2,				0.0f,//0
		 2.5f, newYOrigin + sdDist2,				0.0f,//1
	     3.75f, newYOrigin,   0.0f,//2
		 2.5f, newYOrigin - sdDist2,			0.0f,//3
		-2.5f, newYOrigin - sdDist2,			0.0f,//4
	   -3.75f, newYOrigin,      0.0f,//5
	   // upperpoint
	   0.0f, newYOrigin,   10.0f,//6
	   // debbie downer
	   0.0f, newYOrigin,  -10.0f//7
	};

	GLuint indicator_vbo = 0;
	glGenBuffers(1, &indicator_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, indicator_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(indicator), indicator, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
	float indicatorCol[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	GLuint indicator_colors_vbo = 0;
	glGenBuffers(1, &indicator_colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, indicator_colors_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(indicatorCol), indicatorCol, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLushort indicator_index_array[] = {
		// upper faces plateau
		0, 1, 6,
		1, 2, 6,
		2, 3, 6,
		3, 4, 6,
		4, 5, 6,
		5, 0, 6,
		// down faces plateau
		1, 0, 7,
		2, 1, 7,
		3, 2, 7,
		4, 3, 7,
		5, 4, 7,
		0, 5, 7
	};

	GLuint indicator_IBO;
	glGenBuffers(1, &indicator_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicator_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicator_index_array), indicator_index_array, GL_STATIC_DRAW);

	// One of my team members said I should start defining my objects here. No one is going to see these comments right?
}

void transformObject(glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, scale);

	MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

}

void transformOtherRotationObject(glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 secondaryRotationAxis, float secondaryRotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::rotate(Model, glm::radians(secondaryRotationAngle), secondaryRotationAxis);
	Model = glm::scale(Model, scale);

	MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

}

//---------------------------------------------------------------------
//
// display
//
void display(void) {
	keyOperations();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	//glBindBuffer(GL_ARRAY_BUFFER, indicator_vbo);

	glBindVertexArray(gVAO);
	transformObject(glm::vec3(0.2f, 0.2f, 0.2f), X_AXIS, 90, glm::vec3(0.0, -1.25, 0.0));
	glDrawElements(GL_TRIANGLES, 20 * 3 , GL_UNSIGNED_SHORT, 0);
	
	glBindVertexArray(gVAOIndicator);
	transformOtherRotationObject(glm::vec3(0.1f, 0.1f, 0.1f), X_AXIS, 90, Y_AXIS, rotationGreenIndicator, glm::vec3(0, upAndDown, 0/*upAndDown*/));
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_SHORT, 0);

	rotationGreenIndicator += rotationSpeed;
	if (rotationGreenIndicator >= 360)
		rotationGreenIndicator = 0;

	if (directionUp) {
		upAndDown += upAndDownStep;
		if (upAndDown >= 0.1) {
			directionUp = false;
		}		
	}
	else {
		upAndDown -= upAndDownStep;
		if (upAndDown <= -0.1) {
			directionUp = true;
		}
	}


	glutSwapBuffers();
	deltaTime = (glutGet(GLUT_ELAPSED_TIME) - currentTime) / 1000.0f;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
}

void keyDown(unsigned char key, int x, int y) {
	keyStates[key] = true; // Set the state of the current key to pressed
}

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false; // Release the state of the current key to pressed
}

void keyOperations(void) {
	float cameraSpeed = 10.0f;
	if (keyStates['w']) { // If the 'w' key has been pressed
		currentCamPos.z -= cameraSpeed * (deltaTime);
	}
	if (keyStates['s']) { // If the 's' key has been pressed
		currentCamPos.z += cameraSpeed * (deltaTime);
	}
	if (keyStates['a']) { // If the 'a' key has been pressed
		currentCamPos.x -= cameraSpeed * (deltaTime);
	}
	if (keyStates['d']) { // If the 'd' key has been pressed
		currentCamPos.x += cameraSpeed * (deltaTime);
	}
	if (keyStates['r']) { // If the 'r' key has been pressed
		currentCamPos.y += cameraSpeed * (deltaTime);
	}
	if (keyStates['f']) { // If the 'f' key has been pressed
		currentCamPos.y -= cameraSpeed * (deltaTime);
	}
	View = glm::lookAt(
		currentCamPos, // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
}

void keyboardFun(unsigned char key, int x, int y) {
	float cameraSpeed = 10.0f;
	switch (key) {
	case 'w':
		// call a function
		currentCamPos.z -= cameraSpeed * (deltaTime);
		std::cout << deltaTime << std::endl;
		break;
	case 's':
		currentCamPos.z += cameraSpeed * (deltaTime);
		// call a function
		break;
	case 'a':
		currentCamPos.x -= cameraSpeed * (deltaTime);
		// call a function
		break;
	case 'd':
		currentCamPos.x += cameraSpeed * (deltaTime);
		// call a function
		break;
	case 'r':
		currentCamPos.y += cameraSpeed * (deltaTime);
		// call a function
		break;
	case 'f':
		currentCamPos.y -= cameraSpeed * (deltaTime);
		// call a function
		break;

	default:
		break;
	}

}

void idle() {

}

void Timer(int id) {
	glutPostRedisplay();
	glutTimerFunc(15, Timer, 0);
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Ricardo Shimoda Nakasako - 101128885");

	glewExperimental = true;
	glewInit();    //Initializes the glew and prepares the drawing pipeline.

	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_DEPTH_TEST);
	init();

	glutTimerFunc(15, Timer, 0);

	glutDisplayFunc(display);

	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glutIdleFunc(idle);

	glutMainLoop();
}




