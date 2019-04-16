
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
		star_color[i] = i % 3 == 2? 1.0f:0.0f;
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
	float *star_normals = new float[300];
	normalGenerator(star_vertices, star_index_array, star_normals, sizeof(star_index_array)/sizeof(GLushort));
	GLuint star_normal_vbo = 0;
	glGenBuffers(1, &star_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, star_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(float), star_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	// END OF STAR NORMALS VBO ---------------------------------------------------------

	return sizeof(star_index_array);
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
	return sizeof(wedge_index_array);
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
	float *triPrism_normals = new float[12];
	normalGenerator(triPrism_vertices, triPrism_index_array, triPrism_normals, 12);
	GLuint triPrism_normal_vbo = 0;
	glGenBuffers(1, &triPrism_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triPrism_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), triPrism_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	// END OF STAR NORMALS VBO ---------------------------------------------------------

	return sizeof(triPrism_index_array);
}

int GeometryGenerator::CreateCylinder(GLuint *cylinderVAO) {

	glGenVertexArrays(1, cylinderVAO);
	glBindVertexArray(*cylinderVAO);

	// CYLINDER VERTICES VBO -----------------------------------------------------------
	float radius = 0.5f;
	int degrees = 120;
	float halfHeight = 2.0f;

	int baseTriangles = 360 / degrees;
	int triangles = baseTriangles * 2 * 2;
	int vertices = triangles * 3;
	int coordinates = vertices * 3;
	float *points = new float[coordinates];
	float *p = points;

	for (int currentAngle = 0; currentAngle < 360; currentAngle += degrees) {

		float x1 = radius * cos(currentAngle * PI / 180);
		float x2 = radius * cos((currentAngle + degrees) * PI / 180);
		float z1 = radius * sin(currentAngle * PI / 180);
		float z2 = radius * sin((currentAngle + degrees) * PI / 180);

		/* TOP Origin */
		*p = 0.0f; p++; *p = halfHeight; p++; *p = 0.0f; p++;
		/* A */
		*p = x1; p++; *p = halfHeight; p++; *p = z1; p++;
		/* B */
		*p = x2; p++; *p = halfHeight; p++; *p = z2; p++;

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

	for (int i = 0; i < coordinates; i+=3) {
		std::cout << "Coordinates x:" << points[i] << " y: " << points[i + 1] << " z: " << points[i + 2] << std::endl;
	}

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates * sizeof(float), points, GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, coordinates, colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// END OF CYLINDER COLOURS VBO ------------------------------------------------------

	// CYLINDER TEXTURES VBO -------------------------------------------------------------
	float *texMap = new float[triangles * 3 * 2];
	float *tm = texMap;
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
	glBufferData(GL_ARRAY_BUFFER, triangles * 3 * 2, texMap, GL_STATIC_DRAW);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices, index_array, GL_STATIC_DRAW);

	// END OF CYLINDER INDEX LIST IBO --------------------------------------------------

	// CYLINDER NORMALS VBO ---------------------------------------------------------
	float *normals = new float[coordinates];
	normalGenerator(points, index_array, normals, vertices);
	GLuint normal_vbo = 0;
	glGenBuffers(1, &normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, coordinates, normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	// END OF CYLINDER VBO ---------------------------------------------------------

	return vertices;
}