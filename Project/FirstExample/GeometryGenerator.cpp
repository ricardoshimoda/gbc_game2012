
#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "math.h"
#include "GeometryGenerator.h"

#define PI 3.14159265

float GeometryGenerator::epsilonCorrector(float value) {
	return (value < 3 * FLT_EPSILON && value > -3 * FLT_EPSILON) ? 0.0f : value;
}

void GeometryGenerator::crossProduct(float vect_A[], float vect_B[], float cross_P[])
{
	cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
	cross_P[1] = vect_A[0] * vect_B[2] - vect_A[2] * vect_B[0];
	cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
	float length = sqrt(cross_P[0] * cross_P[0] + cross_P[1] * cross_P[1] + cross_P[2] * cross_P[2]);
	cross_P[0] = epsilonCorrector(cross_P[0] / length);
	cross_P[1] = epsilonCorrector(cross_P[1] / length);
	cross_P[2] = epsilonCorrector(cross_P[2] / length);
}

void GeometryGenerator::normalGenerator(float *vertices, GLushort *ibo, float *normals, int size) {
	for (int i = 0; i < size; i += 3) {
		/*
		 * Obtain triangle
		 */
		int left = ibo[i];
		int center = ibo[i + 1];
		int right = ibo[i + 2];

		/*
		 * Obtain vertices
		 */
		int vertexIndex = left * 3;
		float leftArray[] = { vertices[vertexIndex],vertices[vertexIndex + 1],vertices[vertexIndex + 2] };
		vertexIndex = center * 3;
		float centerArray[] = { vertices[vertexIndex], vertices[vertexIndex + 1],vertices[vertexIndex + 2] };
		vertexIndex = right * 3;
		float rightArray[] = { vertices[vertexIndex], vertices[vertexIndex + 1],vertices[vertexIndex + 2] };

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
		normals[vertexIndex] = normal[0];
		normals[vertexIndex + 1] = normal[1];
		normals[vertexIndex + 2] = normal[2];

		/* For Center */
		A[0] = leftArray[0] - centerArray[0];
		A[1] = leftArray[1] - centerArray[1];
		A[2] = leftArray[2] - centerArray[2];
		B[0] = rightArray[0] - centerArray[0];
		B[1] = rightArray[1] - centerArray[1];
		B[2] = rightArray[2] - centerArray[2];
		crossProduct(A, B, normal);
		vertexIndex = center * 3;
		normals[vertexIndex] = normal[0];
		normals[vertexIndex + 1] = normal[1];
		normals[vertexIndex + 2] = normal[2];

		/* For Right */
		A[0] = centerArray[0] - rightArray[0];
		A[1] = centerArray[1] - rightArray[1];
		A[2] = centerArray[2] - rightArray[2];
		B[0] = leftArray[0] - rightArray[0];
		B[1] = leftArray[1] - rightArray[1];
		B[2] = leftArray[2] - rightArray[2];
		crossProduct(A, B, normal);
		vertexIndex = right * 3;
		normals[vertexIndex] = normal[0];
		normals[vertexIndex + 1] = normal[1];
		normals[vertexIndex + 2] = normal[2];
	}
}

int GeometryGenerator::CreateStar(GLuint *starVAO)
{
	glGenVertexArrays(1, starVAO);
	glBindVertexArray(*starVAO);
	// STAR VERTICES VBO -----------------------------------------------------------
	float star_vertices[] =
	{
		// FRONT FACE
		0,     0.5,     0.25, // 0
		0.146, 0.354,   0.25, // 1
		0.354, 0.354,   0.25, // 2
		0.354, 0.146,   0.25, // 3
		0.5,   0,       0.25, // 4
		0.354, -0.146,  0.25, // 5
		0.354, -0.354,  0.25, // 6
		0.354, -0.354,  0.25, // 7
		0.146, -0.354,  0.25, // 8
		0,     -0.5,    0.25, // 9
		-0.146, -0.354, 0.25, // 10
		-0.354, -0.354, 0.25, // 11
		-0.354, -0.146, 0.25, // 12
		-0.5,   0,      0.25, // 13
		-0.354, 0.146,  0.25, // 14
		-0.354, 0.354,  0.25, // 15
		-0.354, 0.354,  0.25, // 16
		-0.146, 0.354,  0.25, // 17

		// THE SIDES
		// NOTH EAST RECTANGLES
		0,     0.5,     0.25, // 18
		0,     0.5,    -0.25, // 19
		0.146, 0.354,  -0.25, // 20
		0.146, 0.354,   0.25, // 21
		0.146, 0.354,   0.25, // 22
		0.146, 0.354,  -0.25, // 23
		0.354, 0.354,  -0.25, // 24
		0.354, 0.354,   0.25, // 25
		0.354, 0.354,   0.25, // 26
		0.354, 0.354,  -0.25, // 27
		0.354, 0.146,   0.25, // 28
		0.354, 0.146,  -0.25, // 29
		0.354, 0.146,  -0.25, // 30
		0.5,   0,      -0.25, // 31
		0.354, 0.146,   0.25, // 32
		0.5,   0,       0.25, // 33

		// SOUTH EAST RECTANGLES
		0.5,   0,       0.25, // 34
		0.5,   0,      -0.25, // 35
		0.354, -0.146,  0.25, // 36
		0.354, -0.146, -0.25, // 37
		0.354, -0.146,  0.25, // 38
		0.354, -0.146, -0.25, // 39
		0.354, -0.354,  0.25, // 40
		0.354, -0.354, -0.25, // 41
		0.146, -0.354,  0.25, // 42
		0.354, -0.354,  0.25, // 43
		0.146, -0.354, -0.25, // 44
		0.354, -0.354, -0.25, // 45
		0.146, -0.354,  0.25, // 46
		0.146, -0.354, -0.25, // 47
		0,     -0.5,    0.25, // 48
		0,     -0.5,   -0.25, // 49

		// SOUTH WEST RECTANGLES
		-0.146, -0.354, 0.25, // 50
		-0.146, -0.354,-0.25, // 51
		0,     -0.5,    0.25, // 52
		0,     -0.5,   -0.25, // 53
		-0.354, -0.354, 0.25, // 54
		-0.146, -0.354, 0.25, // 55
		-0.354, -0.354,-0.25, // 56
		-0.146, -0.354,-0.25, // 57
		-0.354, -0.146,-0.25, // 58
		-0.354, -0.146, 0.25, // 59
		-0.354, -0.354,-0.25, // 60
		-0.354, -0.354, 0.25, // 61
		-0.5,   0,      0.25, // 62
		-0.354, -0.146, 0.25, // 63
		-0.5,   0,     -0.25, // 64
		-0.354, -0.146,-0.25, // 65

		// NORTH WEST RECTANGLES
		-0.354, 0.146, -0.25, // 66
		-0.354, 0.146,  0.25, // 67
		-0.5,   0,     -0.25, // 68
		-0.5,   0,      0.25, // 69
		-0.354, 0.354, -0.25, // 70
		-0.354, 0.354,  0.25, // 71
		-0.354, 0.146, -0.25, // 72
		-0.354, 0.146,  0.25, // 73
		-0.354, 0.354, -0.25, // 74
		-0.146, 0.354, -0.25, // 75
		-0.354, 0.354,  0.25, // 76
		-0.146, 0.354,  0.25, // 77
		-0.146, 0.354, -0.25, // 78
		0,     0.5,    -0.25, // 79
		-0.146, 0.354,  0.25, // 80
		0,     0.5,     0.25, // 81

		// BACK FACE
		0,     0.5,    -0.25, // 82
		0.146, 0.354,  -0.25, // 83
		0.354, 0.354,  -0.25, // 84
		0.354, 0.146,  -0.25, // 85
		0.5,   0,      -0.25, // 86
		0.354, -0.146, -0.25, // 87
		0.354, -0.354, -0.25, // 88
		0.354, -0.354, -0.25, // 89
		0.146, -0.354, -0.25, // 90
		0,     -0.5,   -0.25, // 91
		-0.146, -0.354,-0.25, // 92
		-0.354, -0.354,-0.25, // 93
		-0.354, -0.146,-0.25, // 94
		-0.5,   0,     -0.25, // 95
		-0.354, 0.146, -0.25, // 96
		-0.354, 0.354, -0.25, // 97
		-0.354, 0.354, -0.25, // 98
		-0.146, 0.354, -0.25, // 99
	};

	GLuint star_points_vbo = 0;
	glGenBuffers(1, &star_points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, star_points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(star_vertices), star_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF STAR VERTICES VBO ----------------------------------------------------

	// STAR COLOR VBO -------------------------------------------------------------
	float *star_color = new float[300];
	for (int i = 0; i < 300; i++)
	{
		// Blue
		star_color[i] = i % 3 == 2 ? 1.0f : 0.0f;
	}
	GLuint star_color_vbo = 0;
	glGenBuffers(1, &star_color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, star_color_vbo);
	glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(float), star_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	// END OF STAR COLOR VBO ----------------------------------------------------


	// STAR TEXTURES VBO -------------------------------------------------------------
	float star_texture[] =
	{
		// FRONT FACE
		0.5, 0,   // 0
		1, 1, 	  // 1
		1, 0, 	  // 2
		0, 0, 	  // 3
		1, 0.5,   // 4
		0, 1, 	  // 5
		1, 1, 	  // 6
		1, 1, 	  // 7
		1, 0, 	  // 8
		0.5, 1,   // 9
		0, 0,     // 10
		0, 1,     // 11
		1,1,      // 12
		0, 0.5,   // 13
		1, 0,     // 14
		0, 0,     // 15
		0, 0,     // 16
		0, 1,     // 17

		// THE SIDES
		// NORTH EAST RECTANGLES
		0,1, // 18
		0,0, // 19
		1,0, // 20
		1,1, // 21
		0,1, // 22
		0,0, // 23
		1,0, // 24
		1,1, // 25
		0,0, // 26
		1,0, // 27
		0,1, // 28
		1,1, // 29
		0,0, // 30
		1,0, // 31
		0,1, // 32
		1,1, // 33

		// SOUTH EAST RECTANGLES
		0,0, // 34
		1,0, // 35
		0,1, // 36
		1,1, // 37
		0,0, // 38
		1,0, // 39
		0,1, // 40
		1,1, // 41
		0,0, // 42
		1,0, // 43
		0,1, // 44
		1,1, // 45
		0,0, // 46
		1,0, // 47
		0,1, // 48
		1,1, // 49

		// SOUTH WEST RECTANGLES
		0,0, // 50
		0,1, // 51
		1,0, // 52
		1,1, // 53
		0,0, // 54
		1,0, // 55
		0,1, // 56
		1,1, // 57
		0,0, // 58
		1,0, // 59
		0,1, // 60
		1,1, // 61
		0,0, // 62
		1,0, // 63
		0,1, // 64
		1,1, // 65

		// NORTH WEST RECTANGLES
		0,0, // 66 
		0,1, // 67
		1,0, // 68
		1,1, // 69
		0,0, // 70
		1,0, // 71
		0,1, // 72
		1,1, // 73
		0,0, // 74
		1,0, // 75
		0,1, // 76
		1,1, // 77
		0,0, // 78
		1,0, // 79
		0,1, // 80
		1,1, // 81

		// BACK FACE
		0.5, 0,   // 82
		1, 1, 	  // 83
		1, 0, 	  // 84
		0, 0, 	  // 85
		1, 0.5,   // 86
		0, 1, 	  // 87
		1, 1, 	  // 88
		1, 1, 	  // 89
		1, 0, 	  // 90
		0.5, 1,   // 91
		0, 0,     // 92
		0, 1,     // 93
		1,1,      // 94
		0, 0.5,   // 95
		1, 0,     // 96
		0, 0,     // 97
		0, 0,     // 98
		0, 1,     // 99
	};

	GLuint star_texture_vbo = 0;
	glGenBuffers(1, &star_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, star_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(star_texture), star_texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF STAR TEXTURES VBO ------------------------------------------------------

	// STAR INDEX LIST IBO ---------------------------------------------------------
	GLushort star_index_array[] =
	{
		6,2,16,
		11,7,15,
		17,1,0,
		5,4,3,
		10,9,8,
		14,13,12,
		21,20,19,
		19,18,21,
		25,24,23,
		23,22,25,
		29,27,26,
		26,28,29,
		33,31,30,
		30,32,33,
		37,35,34,
		34,36,37,
		41,39,38,
		38,40,41,
		45,43,42,
		42,44,45,
		47,46,48,
		48,49,47,
		53,52,50,
		50,51,53,
		57,55,54,
		54,56,57,
		61,59,58,
		58,60,61,
		65,63,62,
		62,64,65,
		67,66,68,
		68,69,67,
		73,71,70,
		70,72,73,
		77,75,74,
		74,76,77,
		81,79,78,
		78,80,81,
		98,84,88,
		93,97,89,
		99,82,83,
		85,86,87,
		91,92,90,
		95,96,94
	};

	GLuint star_index_ibo = 0;
	glGenBuffers(1, &star_index_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_index_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(star_index_array), star_index_array, GL_STATIC_DRAW);
	// END OF STAR INDEX LIST IBO --------------------------------------------------

	// STAR NORMALS VBO ---------------------------------------------------------
	float *star_normals = new float[sizeof(star_vertices) / sizeof(float)];
	normalGenerator(star_vertices, star_index_array, star_normals, sizeof(star_index_ibo) / sizeof(GLushort));
	GLuint star_normal_vbo = 0;
	glGenBuffers(1, &star_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, star_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(float), star_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	// END OF STAR NORMALS VBO ---------------------------------------------------------

	return sizeof(star_index_array) / sizeof(GLushort);
}

int GeometryGenerator::CreateWedge(GLuint *wedgeVAO)
{
	glGenVertexArrays(1, wedgeVAO);
	glBindVertexArray(*wedgeVAO);
	// WEDGE VERTICES VBO -----------------------------------------------------------
	float wedge_vertices[] =
	{
		0,  1, -1, // 0
		0,  1,  0, // 1
		0,  0, -1, // 2
		0,  0,  0, // 3
		0,  1,  0, // 4
		0,  0,  0, // 5
		1,  0,  0, // 6
		0,  0,  0, // 7
		1,  0,  0, // 8
		0,  0, -1, // 9
		1,  0, -1, // 10
		0,  1,  0, // 11
		0,  1, -1, // 12
		1,  0,  0, // 13
		1,  0, -1, // 14
		0,  1, -1, // 15
		0,  0, -1, // 16
		1,  0, -1, // 17
	};
	// Centering X, Y and Z
	for (int i = 0; i < 18 * 3; i++) {
		if (i % 3 == 0 || i % 3 == 1) {
			wedge_vertices[i] -= 0.5;
		}
		else {
			wedge_vertices[i] += 0.5;

		}

	}


	GLuint wedge_points_vbo = 0;
	glGenBuffers(1, &wedge_points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, wedge_points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wedge_vertices), wedge_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF WEDGE VERTICES VBO ----------------------------------------------------

	// WEDGE TEXTURES VBO -------------------------------------------------------------
	float wedge_textures[] =
	{
		0, 0, // 0
		1, 0, // 1
		0, 1, // 2
		1, 1, // 3
		0, 0, // 4
		0, 1, // 5
		1, 1, // 6
		0, 0, // 7
		1, 0, // 8
		0, 1, // 9
		1, 1, // 10
		0, 0, // 11
		1, 0, // 12
		0, 1, // 13
		1, 1, // 14
		0, 0, // 15
		1, 0, // 16
		1, 1, // 17
	};

	GLuint wedge_textures_vbo = 0;
	glGenBuffers(1, &wedge_textures_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, wedge_textures_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wedge_textures), wedge_textures, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF WEDGE TEXTURES VBO ------------------------------------------------------

	// WEDGE INDEX LIST IBO ---------------------------------------------------------
	GLushort wedge_index_array[] =
	{
		3,1,0,
		0,2,3,
		5,6,4,
		10,8,7,
		7,9,10,
		13,14,12,
		12,11,13,
		17,16,15,
	};

	GLuint wedge_index_ibo = 0;
	glGenBuffers(1, &wedge_index_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wedge_index_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wedge_index_array), wedge_index_array, GL_STATIC_DRAW);
	// END OF WEDGE INDEX LIST IBO --------------------------------------------------

	// WEDGE NORMALS VBO ---------------------------------------------------------
	float *wedge_normals = new float[sizeof(wedge_vertices) / sizeof(float)];
	normalGenerator(wedge_vertices, wedge_index_array, wedge_normals, sizeof(wedge_index_array) / sizeof(GLushort));
	GLuint wegde_normal_vbo = 0;
	glGenBuffers(1, &wegde_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, wegde_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(float), wedge_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	// END OF WEDGE NORMALS VBO ---------------------------------------------------------

	return sizeof(wedge_index_array) / sizeof(GLushort);
}

int GeometryGenerator::CreateTriPrism(GLuint *triPrismVAO)
{
	glGenVertexArrays(1, triPrismVAO);
	glBindVertexArray(*triPrismVAO);
	// TRIPRISM VERTICES VBO -----------------------------------------------------------
	float triPrism_vertices[] =
	{
		0,    0,      0,     // 0
		0.5,  0.866,  -0.289,// 1
		1,    0,      0,     // 2
		0,    0,      0,     // 3
		1,    0,      0,     // 4
		0.5,  0,      -0.866,// 5
		0.5,  0.866,  -0.289,// 6
		0.5,  0,      -0.866,// 7
		1,    0,      0,     // 8
		0.5,  0,      -0.866,// 9
		0.5,  0.866,  -0.289,// 10
		0,    0,      0,     // 11
	};

	// Centering X and Y
	for (int i = 0; i < 12 * 3; i++) {
		if (i % 3 == 0 || i % 3 == 1) {
			triPrism_vertices[i] -= 0.5;
		}
		else {
			triPrism_vertices[i] += 0.5;
		}

	}

	GLuint triPrism_points_vbo = 0;
	glGenBuffers(1, &triPrism_points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triPrism_points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triPrism_vertices), triPrism_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF TRIPRISM VERTICES VBO ----------------------------------------------------

	// TRIPRISM TEXTURES VBO -------------------------------------------------------------
	float triPrism_textures[] =
	{
		0,   1, // 0
		0.5, 0, // 1
		1,   1, // 2
		0,   0, // 3
		1,   0, // 4
		0.5, 1, // 5
		0, 0.5, // 6
		1,   0, // 7
		1,   1, // 8
		0,   0, // 9
		1, 0.5, // 10
		0,   1, // 11
	};

	GLuint triPrism_textures_vbo = 0;
	glGenBuffers(1, &triPrism_textures_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triPrism_textures_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triPrism_textures), triPrism_textures, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF TRIPRISM TEXTURES VBO ------------------------------------------------------

	// TRIPRISM INDEX LIST IBO ---------------------------------------------------------
	GLushort triPrism_index_array[] =
	{
		2,1,0,
		5,4,3,
		8,7,6,
		11,10,9
	};

	GLuint triPrism_index_ibo = 0;
	glGenBuffers(1, &triPrism_index_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triPrism_index_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triPrism_index_array), triPrism_index_array, GL_STATIC_DRAW);
	// END OF TRIPRISM INDEX LIST IBO --------------------------------------------------

	// TRIPRISM NORMALS VBO ---------------------------------------------------------
	float *triPrism_normals = new float[sizeof(triPrism_vertices) / sizeof(float)];
	normalGenerator(triPrism_vertices, triPrism_index_array, triPrism_normals, sizeof(triPrism_index_array) / sizeof(GLushort));
	GLuint triPrism_normal_vbo = 0;
	glGenBuffers(1, &triPrism_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triPrism_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), triPrism_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	// END OF TRIPRISM NORMALS VBO ---------------------------------------------------------

	return sizeof(triPrism_index_array) / sizeof(GLushort);
}

int GeometryGenerator::CreateCube(GLuint *cubeVAO) { 

	glGenVertexArrays(1, cubeVAO);
	glBindVertexArray(*cubeVAO);

	// CUBE VERTICES VBO -----------------------------------------------------------
	float halfCubeSide = 0.5;
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
	// END OF CUBE VERTICES VBO ----------------------------------------------------

	// CUBE COLOURS VBO -------------------------------------------------------------
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
	// END OF CUBE COLOURS VBO -------------------------------------------------------------

	// CUBE TEXTURES VBO -------------------------------------------------------------
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

	GLuint texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texMapCube), texMapCube, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF CUBE TEXTURES VBO ------------------------------------------------------

	// CUBE INDEX LIST IBO ---------------------------------------------------------
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
	// END OF CUBE INDEX LIST IBO --------------------------------------------------

	// CUBE NORMALS VBO ---------------------------------------------------------
	float *cube_normals = new float[sizeof(points) / sizeof(float)];
	normalGenerator(points, cube_index_array, cube_normals, sizeof(cube_index_array) / sizeof(GLushort));
	GLuint cube_normal_vbo = 0;
	glGenBuffers(1, &cube_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), cube_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	// END OF CUBE NORMALS VBO ---------------------------------------------------------

	return sizeof(cube_index_array) / sizeof(GLushort);
}

int GeometryGenerator::CreatePyramid(GLuint *pyramidVAO) {
	glGenVertexArrays(1, pyramidVAO);
	glBindVertexArray(*pyramidVAO);

	// PYRAMID VERTICES VBO -----------------------------------------------------------
	float halfPyramidSide = 0.5f;
	float pyramidHeight = 1.0f;

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
		-halfPyramidSide, 0.0f, -halfPyramidSide, // 13-BOTTOM_4
		 0.0f, pyramidHeight, 0.0f,				  // 04-TOP_3
		 0.0f, pyramidHeight, 0.0f				  // 04-TOP_4
	};
	/*
	 Centering the pyramid
	*/
	for (int i = 0; i < sizeof(pyramidPoints) / sizeof(float); i++) {
		if (i % 3 == 1) {
			pyramidPoints[i] -= pyramidHeight/2;
		}
	}

	GLuint pyramid_points_vbo = 0;
	glGenBuffers(1, &pyramid_points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPoints), pyramidPoints, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// END OF PYRAMID VERTICES VBO ----------------------------------------------------

	// PYRAMID COLOURS VBO -------------------------------------------------------------
	float pyramid_colors[] = {
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

	GLuint pyramid_colors_vbo = 0;
	glGenBuffers(1, &pyramid_colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_colors), pyramid_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	// END OF PYRAMID COLOURS VBO -------------------------------------------------------------

	// PYRAMID TEXTURES VBO -------------------------------------------------------------
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
		1.f,0.f, // 13
		0.5f, 0.f, // 14
		0.5f, 0.f // 15
	};

	GLuint pyramid_texture_vbo = 0;
	glGenBuffers(1, &pyramid_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texMapPyramid), texMapPyramid, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF PYRAMID TEXTURES VBO ------------------------------------------------------

	// PYRAMID INDEX LIST IBO ---------------------------------------------------------
	GLushort pyramid_index_array[] = {
		// front
		0,1,4,
		// back
		2,3,14,
		// left
		8,5,9,
		// right
		6,7,15,
		// bottom
		10,13,12,
		12,11,10
	};

	GLuint pyramid_IBO;
	glGenBuffers(1, &pyramid_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramid_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_index_array), pyramid_index_array, GL_STATIC_DRAW);
	// END OF PYRAMID INDEX LIST IBO --------------------------------------------------

	// PYRAMID NORMALS VBO ---------------------------------------------------------
	float *pyramid_normals = new float[sizeof(pyramidPoints) / sizeof(float)];
	normalGenerator(pyramidPoints, pyramid_index_array, pyramid_normals, sizeof(pyramid_index_array) / sizeof(GLushort));
	GLuint pyramid_normal_vbo = 0;
	glGenBuffers(1, &pyramid_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), pyramid_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	// END OF PYRAMID NORMALS VBO ---------------------------------------------------------

	return sizeof(pyramid_index_array) / sizeof(GLushort);



}

int GeometryGenerator::CreateHexagon(GLuint *hexagonVAO) {

	glGenVertexArrays(1, hexagonVAO);
	glBindVertexArray(*hexagonVAO);

	// HEXAGON VERTICES VBO -----------------------------------------------------------
	float halfSideSize = 0.5f;
	float sdDist = halfSideSize * sqrt(3)/2; // side * sin(60)

	float points[] = {
		// TOP_LID
		-halfSideSize,			halfSideSize, halfSideSize,
		 halfSideSize,			halfSideSize, halfSideSize,
		 halfSideSize + sdDist,	halfSideSize, 0,
		 halfSideSize,			halfSideSize, -halfSideSize,
		-halfSideSize,			halfSideSize, -halfSideSize,
		-halfSideSize - sdDist,	halfSideSize, 0,

		 // BOTTOM_LID
		-halfSideSize,		   -halfSideSize, halfSideSize,
		 halfSideSize,		   -halfSideSize, halfSideSize,
		 halfSideSize + sdDist,-halfSideSize, 0,
		 halfSideSize,		   -halfSideSize, -halfSideSize,
		-halfSideSize,		   -halfSideSize, -halfSideSize,
		-halfSideSize - sdDist,-halfSideSize, 0,

		// SIDES
		-halfSideSize,			halfSideSize, halfSideSize,
 	    -halfSideSize,		   -halfSideSize, halfSideSize,
		 halfSideSize,			halfSideSize, halfSideSize,
 		 halfSideSize,		   -halfSideSize, halfSideSize,
		 halfSideSize + sdDist,	halfSideSize, 0,
		 halfSideSize + sdDist,-halfSideSize, 0,
		 halfSideSize,			halfSideSize, -halfSideSize,
		 halfSideSize,		   -halfSideSize, -halfSideSize,
		-halfSideSize,			halfSideSize, -halfSideSize,
	    -halfSideSize,		   -halfSideSize, -halfSideSize,
		-halfSideSize - sdDist,	halfSideSize, 0,
	    -halfSideSize - sdDist,-halfSideSize, 0,

	};

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF HEXAGON VERTICES VBO ----------------------------------------------------

	// HEXAGON COLOURS VBO -------------------------------------------------------------

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
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// END OF HEXAGON COLOURS VBO ------------------------------------------------------

	// HEXAGON TEXTURES VBO -------------------------------------------------------------
	float texMapHexagon[] = {
		/* TOP */
		0,  1,
		0,  0,
		0,  1,
		0.5,0.5,
		0,  1,
		0,  0,
		/* BOTTOM  */
		0,  1,
		0,  0,
		0,  1,
		0.5,0.5,
		0,  1,
		0,  0,
		/* SIDES */
		0,  0,
		0,  1,
		1,  0,
		1,  1,
		0,  0,
		0,  1,
		1,  0,
		1,  1,
		0,  0,
		0,  1,
		1,  0,
		1,  1
	};

	GLuint hexagon_texture_vbo = 0;
	glGenBuffers(1, &hexagon_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, hexagon_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texMapHexagon), texMapHexagon, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF HEXAGON TEXTURES VBO ------------------------------------------------------

	// HEXAGON INDEX LIST IBO ---------------------------------------------------------

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
		0, 3, 1,
		1, 3, 2,
		5, 3, 0,
		5, 4, 3,
		// sides
		0, 1, 3,
		3, 2, 0,
		2, 3, 5,
		5, 4, 2,
		4, 5, 7,
		7, 6, 4,
		6, 7, 9,
		9, 8, 6,
		8, 9, 11,
		11, 10, 8,
		10, 11, 1,
		1, 0, 10
	};
	for (int i = 4 * 3; i < 8 * 3; i++) {
		penta_index_array[i] += 6;
	}

	for (int i = 8 * 3; i < sizeof(penta_index_array) / sizeof(GLushort); i++) {
		penta_index_array[i] += 12;
	}

	GLuint penta_IBO;
	glGenBuffers(1, &penta_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, penta_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(penta_index_array), penta_index_array, GL_STATIC_DRAW);

	// END OF HEXAGON INDEX LIST IBO --------------------------------------------------

	// HEXAGON NORMALS VBO ---------------------------------------------------------
	float *hexagon_normals = new float[sizeof(points) / sizeof(float)];
	normalGenerator(points, penta_index_array, hexagon_normals, sizeof(penta_index_array) / sizeof(GLushort));
	GLuint hexagon_normal_vbo = 0;
	glGenBuffers(1, &hexagon_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, hexagon_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), hexagon_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	// END OF HEXAGON NORMALS VBO ---------------------------------------------------------

	return sizeof(penta_index_array) / sizeof(GLushort);
}

int GeometryGenerator::CreateSimsIndicator(GLuint *simsIndicatorVAO) { 

	glGenVertexArrays(1, simsIndicatorVAO);
	glBindVertexArray(*simsIndicatorVAO);

	// SIMS INDICATOR VERTICES VBO -----------------------------------------------------------
	float halfSideSize = 0.5f;
	float sdDist = halfSideSize * sqrt(3) / 2; // side * sin(60)

	float points[] = {
		// BASE AND TOP
		 0,                     halfSideSize, 0,
		-halfSideSize,			0,            halfSideSize,
		 halfSideSize,			0,            halfSideSize,

		 0,                     halfSideSize, 0,
		 halfSideSize,			0,            halfSideSize,
		 halfSideSize + sdDist,	0,            0,

		 0,                     halfSideSize, 0,
		 halfSideSize + sdDist,	0,            0,
		 halfSideSize,			0,           -halfSideSize,

		 0,                     halfSideSize, 0,
		 halfSideSize,			0,           -halfSideSize,
		-halfSideSize,			0,           -halfSideSize,

		 0,                     halfSideSize, 0,
		-halfSideSize,			0,           -halfSideSize,
		-halfSideSize - sdDist,	0,            0,

		 0,                     halfSideSize, 0,
		-halfSideSize - sdDist,	0,            0,
		-halfSideSize,			0,            halfSideSize,

		// BASE AND DOWN
		 0,                    -halfSideSize, 0,
		 halfSideSize,			0,            halfSideSize,
		-halfSideSize,			0,            halfSideSize,

		 0,                    -halfSideSize, 0,
		 halfSideSize + sdDist,	0,            0,
		 halfSideSize,			0,            halfSideSize,

		 0,                    -halfSideSize, 0,
		 halfSideSize,			0,           -halfSideSize,
		 halfSideSize + sdDist,	0,            0,

		 0,                    -halfSideSize, 0,
		-halfSideSize,			0,           -halfSideSize,
		 halfSideSize,			0,           -halfSideSize,

		 0,                    -halfSideSize, 0,
		-halfSideSize - sdDist,	0,            0,
		-halfSideSize,			0,           -halfSideSize,

		 0,                    -halfSideSize, 0,
		-halfSideSize,			0,            halfSideSize,
		-halfSideSize - sdDist,	0,            0
	};

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF SIMS INDICATOR VERTICES VBO ----------------------------------------------------

	// SIMS INDICATOR COLOURS VBO -------------------------------------------------------------

	float *colors = new float[108];
	for (int i = 0; i < 108; i++) {
		if (i % 3 == 2) {
			colors[i] = 1.0f;
		}
	}
	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 108, colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// END OF SIMS INDICATOR COLOURS VBO ------------------------------------------------------

	// SIMS INDICATOR TEXTURES VBO -------------------------------------------------------------
	float *texMap = new float[36*2];
	for (int i = 0; i < 36; i++) {
		if (i % 3 == 0) {
			texMap[2 * i] = 0.5;
			texMap[2 * i + 1] = 0.5;
		}
		else if (i % 3 == 1) {
			texMap[2 * i] = 1;
			texMap[2 * i + 1] = 0;
		}
		else {
			texMap[2 * i] = 1;
			texMap[2 * i + 1] = 1;
		}
	}


	GLuint sims_indicator_texture_vbo = 0;
	glGenBuffers(1, &sims_indicator_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sims_indicator_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texMap), texMap, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF SIMS INDICATOR TEXTURES VBO ------------------------------------------------------

	// SIMS INDICATOR INDEX LIST IBO ---------------------------------------------------------

	GLushort index_array[] = {
		0,  1,  2,
		3,  4,  5,
		6,  7,  8,
		9,  10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26, 
		27, 28, 29,
		30, 31, 32,
		33, 34, 35
	};

	GLuint sims_indicator_IBO;
	glGenBuffers(1, &sims_indicator_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sims_indicator_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array), index_array, GL_STATIC_DRAW);

	// END OF SIMS INDICATOR INDEX LIST IBO --------------------------------------------------

	// SIMS INDICATOR NORMALS VBO ---------------------------------------------------------
	float *normals = new float[sizeof(points) / sizeof(float)];
	normalGenerator(points, index_array, normals, sizeof(index_array) / sizeof(GLushort));
	GLuint sims_indicator_normal_vbo = 0;
	glGenBuffers(1, &sims_indicator_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sims_indicator_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	// END OF HEXAGON NORMALS VBO ---------------------------------------------------------

	return sizeof(index_array) / sizeof(GLushort);
}

int GeometryGenerator::CreateCylinder(GLuint *cylinderVAO) {

	glGenVertexArrays(1, cylinderVAO);
	glBindVertexArray(*cylinderVAO);

	// CYLINDER VERTICES VBO -----------------------------------------------------------
	float radius = 0.5f;
	int degrees = 2;
	float halfHeight = 0.5f;

	int baseTriangles = 360 / degrees;
	int triangles = baseTriangles * 2 * 2;
	int vertices = triangles * 3;
	int coordinates = vertices * 3;
	float *points = new float[coordinates];
	float *p;
	p = points;
	for (int currentAngle = 0; currentAngle < 360; currentAngle += degrees) {

		float x1 = radius * cos(currentAngle * PI / 180);
		float x2 = radius * cos((currentAngle + degrees) * PI / 180);
		float z1 = radius * sin(currentAngle * PI / 180);
		float z2 = radius * sin((currentAngle + degrees) * PI / 180);

		/* TOP Origin */
		*p = 0.0f; p++; *p = halfHeight; p++; *p = 0.0f; p++;
		/* A */
		*p =x1; p++; *p = halfHeight; p++; *p = z1; p++;
		/* B */
		*p = x2; p++; *p = halfHeight; p++; *p= z2; p++;

		/* DOWN Origin */
		*p = 0.0f; p++; *p = -halfHeight; p++; *p = 0.0f; p++;
		/* A */
		*p = x2; p++; *p = -halfHeight; p++; *p = z2; p++;
		/* B */
		*p = x1; p++; *p = -halfHeight; p++; *p = z1; p++;

		/* SIDE 1 */
		/* A */
		*p = x2; p++; *p = halfHeight; p++; *p = z2; p++;
		/* B */
		*p = x1; p++; *p = halfHeight; p++; *p = z1; p++;
		/* C */
		*p = x1; p++; *p = -halfHeight; p++; *p = z1; p++;

		/* SIDE 2 */
		/* C */
		*p = x1; p++; *p = -halfHeight; p++; *p = z1; p++;
		/* D */
		*p = x2; p++; *p = -halfHeight; p++; *p = z2; p++;
		/* A */
		*p = x2; p++; *p = halfHeight; p++; *p = z2; p++;

	}

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates*sizeof(float), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF CYLINDER VERTICES VBO ----------------------------------------------------

	// CYLINDER COLOURS VBO -------------------------------------------------------------

	float *colors = new float[coordinates];
	for (int i = 0; i < coordinates; i++) {
		if (i % 3 == 2) {
			colors[i] = 1.0f;
		}
	}
	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates * sizeof(float), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// END OF CYLINDER COLOURS VBO ------------------------------------------------------

	// CYLINDER TEXTURES VBO -------------------------------------------------------------
	float *texMap = new float[triangles * 3 * 2];
	float *tm;
	tm = texMap;
	for (int i = 0; i < baseTriangles; i++) {
		/* Base TOP */
		*tm = 0.5f; tm++; *tm = 0.5f; tm++;
		*tm = 1.0f; tm++; *tm = 0.0f; tm++;
		*tm = 1.0f; tm++; *tm = 1.0f; tm++;
		/* Base DOWN */
		*tm = 0.5f; tm++; *tm = 0.5f; tm++;
		*tm = 1.0f; tm++; *tm = 1.0f; tm++;
		*tm = 1.0f; tm++; *tm = 0.0f; tm++;
		/* Side ONE */
		*tm = 0.0f; tm++; *tm = 0.0f; tm++;
		*tm = 0.0f; tm++; *tm = 1.0f; tm++;
		*tm = 1.0f; tm++; *tm = 1.0f; tm++;
		/* Side TWO */
		*tm = 1.0f; tm++; *tm = 1.0f; tm++;
		*tm = 1.0f; tm++; *tm = 0.0f; tm++;
		*tm = 0.0f; tm++; *tm = 0.0f; tm++;
	}


	GLuint texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, triangles * 3 * 2 * sizeof(float), texMap, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF CYLINDER TEXTURES VBO ------------------------------------------------------

	// CYLINDER INDEX LIST IBO ---------------------------------------------------------

	GLushort *index_array = new GLushort[vertices];
	for (int i = 0; i < vertices; i++) {
		index_array[i] = i;
	}

	GLuint cylinder_IBO;
	glGenBuffers(1, &cylinder_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinder_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices * sizeof(GLushort), index_array, GL_STATIC_DRAW);

	// END OF CYLINDER INDEX LIST IBO --------------------------------------------------

	// CYLINDER NORMALS VBO ---------------------------------------------------------
	float *normals = new float[coordinates];
	normalGenerator(points, index_array, normals, vertices);
	GLuint normal_vbo = 0;
	glGenBuffers(1, &normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	// END OF CYLINDER VBO ---------------------------------------------------------

	return vertices;
}

int GeometryGenerator::CreateCone(GLuint *coneVAO) { 
	glGenVertexArrays(1, coneVAO);
	glBindVertexArray(*coneVAO);

	// CONE VERTICES VBO -----------------------------------------------------------
	float radius = 0.5f;
	int degrees = 2;
	float halfHeight = 0.5f;

	int baseTriangles = 360 / degrees;
	int triangles = baseTriangles * 2;
	int vertices = triangles * 3;
	int coordinates = vertices * 3;
	float *points = new float[coordinates];
	float *p;
	p = points;
	for (int currentAngle = 0; currentAngle < 360; currentAngle += degrees) {

		float x1 = radius * cos(currentAngle * PI / 180);
		float x2 = radius * cos((currentAngle + degrees) * PI / 180);
		float z1 = radius * sin(currentAngle * PI / 180);
		float z2 = radius * sin((currentAngle + degrees) * PI / 180);

		/* DOWN Origin */
		*p = 0.0f; p++; *p = -halfHeight; p++; *p = 0.0f; p++;
		/* A */
		*p = x2; p++; *p = -halfHeight; p++; *p = z2; p++;
		/* B */
		*p = x1; p++; *p = -halfHeight; p++; *p = z1; p++;

		/* SIDE */
		/* A */
		*p = 0; p++; *p = halfHeight; p++; *p = 0; p++;
		/* B */
		*p = x1; p++; *p = -halfHeight; p++; *p = z1; p++;
		/* C */
		*p = x2; p++; *p = -halfHeight; p++; *p = z2; p++;

	}

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates * sizeof(float), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF CONE VERTICES VBO ----------------------------------------------------

	// CYLINDER CONE VBO -------------------------------------------------------------

	float *colors = new float[coordinates];
	for (int i = 0; i < coordinates; i++) {
		if (i % 3 == 2) {
			colors[i] = 1.0f;
		}
	}
	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates * sizeof(float), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// END OF CONE COLOURS VBO ------------------------------------------------------

	// CONE TEXTURES VBO -------------------------------------------------------------
	float *texMap = new float[triangles * 3 * 2];
	float *tm;
	tm = texMap;
	for (int i = 0; i < baseTriangles; i++) {
		/* DOWN */
		*tm = 0.5f; tm++; *tm = 0.5f; tm++;
		*tm = 1.0f; tm++; *tm = 1.0f; tm++;
		*tm = 1.0f; tm++; *tm = 0.0f; tm++;
		/* Side */
		*tm = 0.0f; tm++; *tm = 0.5f; tm++;
		*tm = 1.0f; tm++; *tm = 0.0f; tm++;
		*tm = 1.0f; tm++; *tm = 1.0f; tm++;
	}


	GLuint texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, triangles * 3 * 2 * sizeof(float), texMap, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF CONE TEXTURES VBO ------------------------------------------------------

	// CONE INDEX LIST IBO ---------------------------------------------------------

	GLushort *index_array = new GLushort[vertices];
	for (int i = 0; i < vertices; i++) {
		index_array[i] = i;
	}

	GLuint cone_IBO;
	glGenBuffers(1, &cone_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices * sizeof(GLushort), index_array, GL_STATIC_DRAW);

	// END OF CONE INDEX LIST IBO --------------------------------------------------

	// CONE NORMALS VBO ---------------------------------------------------------
	float *normals = new float[coordinates];
	normalGenerator(points, index_array, normals, vertices);
	GLuint normal_vbo = 0;
	glGenBuffers(1, &normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	// END OF CONE VBO ---------------------------------------------------------

	return vertices;
}

int GeometryGenerator::CreateSphere(GLuint *sphereVAO) {

	glGenVertexArrays(1, sphereVAO);
	glBindVertexArray(*sphereVAO);

	// SPHERE VERTICES VBO -----------------------------------------------------------
	float radius = 0.5f;
	int degrees = 10;
	float halfHeight = 0.5f;

	int baseTriangles = 360 / degrees;
	
	int triangles = baseTriangles * 2 /* TOP and BOTTOM */
		            + (baseTriangles - 2) * baseTriangles * 2; /* SIDES */
	int vertices = triangles * 3;
	int coordinates = vertices * 3;
	float *points = new float[coordinates];
	float *p;
	p = points;
	for (int longAngle = 0; longAngle < 360; longAngle += degrees) {
		for (int latAngle = 0;  latAngle < 360; latAngle += degrees) {
			float r1 = radius * sin(latAngle * PI / 180);
			float z1 = radius * cos(latAngle * PI / 180);
			float r1x1 = r1 * cos(longAngle * PI / 180);
			float r1x2 = r1 * cos((longAngle + degrees) * PI / 180);
			float r1z1 = r1 * sin(longAngle * PI / 180);
			float r1z2 = r1 * sin((longAngle + degrees) * PI / 180);

			float r2 = radius * sin((latAngle + degrees) * PI / 180);
			float z2 = radius * cos((latAngle + degrees) * PI / 180);
			float r2x1 = r2 * cos(longAngle * PI / 180);
			float r2x2 = r2 * cos((longAngle + degrees) * PI / 180);
			float r2z1 = r2 * sin(longAngle * PI / 180);
			float r2z2 = r2 * sin((longAngle + degrees) * PI / 180);

			if (latAngle == 0) {
				// TOP
				*p = 0.0f; p++; *p = radius; p++; *p = 0.0f; p++;
				*p = r2x1; p++; *p = z2; p++; *p = r2z1; p++;
				*p = r2x2; p++; *p = z2; p++; *p = r2z2; p++;
			}
			else if (360 - latAngle <= degrees) {
				// BOTTOM
				*p = 0.0f; p++; *p = -radius; p++; *p = 0.0f; p++;
				*p = r1x2; p++; *p = z1; p++; *p = r1z2; p++;
				*p = r1x1; p++; *p = z1; p++; *p = r1z1; p++;
			}
			else {
				*p = r1x2; p++; *p = z1; p++; *p = r1z2; p++;
				*p = r1x1; p++; *p = z1; p++; *p = r1z1; p++;
				*p = r2x1; p++; *p = z2; p++; *p = r2z1; p++;
				*p = r2x1; p++; *p = z2; p++; *p = r2z1; p++;
				*p = r2x2; p++; *p = z2; p++; *p = r2z2; p++;
				*p = r1x2; p++; *p = z1; p++; *p = r1z2; p++;
			}
		}
	}

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates * sizeof(float), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF SPHERE VERTICES VBO ----------------------------------------------------

	// SPHERE COLOURS VBO -------------------------------------------------------------

	float *colors = new float[coordinates];
	for (int i = 0; i < coordinates; i++) {
		if (i % 3 == 2) {
			colors[i] = 1.0f;
		}
	}
	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates * sizeof(float), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// END OF SPHERE COLOURS VBO ------------------------------------------------------

	// SPHERE TEXTURES VBO -------------------------------------------------------------
	float *texMap = new float[triangles * 3 * 2];
	float *tm;
	tm = texMap;
	for (int longAngle = 0; longAngle < 360; longAngle += degrees) {
		for (int latAngle = 0; latAngle < 360; latAngle += degrees) {
			if (latAngle == 0) {
				// TOP
				*tm = 0.5f; tm++; *tm = 0.0f; tm++;
				*tm = 0.0f; tm++; *tm = 1.0f; tm++;
				*tm = 1.0f; tm++; *tm = 1.0f; tm++;
			}
			else if (360 - latAngle <= degrees) {
				// BOTTOM
				*tm = 0.5f; tm++; *tm = 0.0f; tm++;
				*tm = 0.0f; tm++; *tm = 1.0f; tm++;
				*tm = 1.0f; tm++; *tm = 1.0f; tm++;
			}
			else {
				*tm = 0.0f; tm++; *tm = 0.0f; tm++;
				*tm = 0.0f; tm++; *tm = 1.0f; tm++;
				*tm = 1.0f; tm++; *tm = 1.0f; tm++;
				*tm = 1.0f; tm++; *tm = 1.0f; tm++;
				*tm = 1.0f; tm++; *tm = 0.0f; tm++;
				*tm = 0.0f; tm++; *tm = 0.0f; tm++;
			}

		}
	}

	GLuint texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, triangles * 3 * 2 * sizeof(float), texMap, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF SPHERE TEXTURES VBO ------------------------------------------------------

	// SPHERE INDEX LIST IBO ---------------------------------------------------------

	GLushort *index_array = new GLushort[vertices];
	for (int i = 0; i < vertices; i++) {
		index_array[i] = i;
	}

	GLuint cylinder_IBO;
	glGenBuffers(1, &cylinder_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinder_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices * sizeof(GLushort), index_array, GL_STATIC_DRAW);

	// END OF SPHERE INDEX LIST IBO --------------------------------------------------

	// SPHERE NORMALS VBO ---------------------------------------------------------
	float *normals = new float[coordinates];
	normalGenerator(points, index_array, normals, vertices);
	GLuint normal_vbo = 0;
	glGenBuffers(1, &normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	// END OF SPHERE NORMALS VBO ---------------------------------------------------------

	return vertices;
}
