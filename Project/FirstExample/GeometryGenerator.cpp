
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\gtc\matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "math.h"
#include "GeometryGenerator.h"

void GeometryGenerator::CreateRubiksCube(float sideLength, GLuint *rubiksVAO)
{
	glGenVertexArrays(1, rubiksVAO);
	glBindVertexArray(*rubiksVAO);
	// RUBIKS VERTICES VBO -----------------------------------------------------------
	float rubiks_vertices[] =
	{
		-(sideLength / 2),  (sideLength / 2),  (sideLength / 2), // 0
		-(sideLength / 2),  (sideLength / 2),  (sideLength / 2), // 1
		-(sideLength / 2),  (sideLength / 2),  (sideLength / 2), // 2

		 (sideLength / 2),  (sideLength / 2),  (sideLength / 2), // 3
		 (sideLength / 2),  (sideLength / 2),  (sideLength / 2), // 4
		 (sideLength / 2),  (sideLength / 2),  (sideLength / 2), // 5

		-(sideLength / 2), -(sideLength / 2),  (sideLength / 2), // 6
		-(sideLength / 2), -(sideLength / 2),  (sideLength / 2), // 7
		-(sideLength / 2), -(sideLength / 2),  (sideLength / 2), // 8

		 (sideLength / 2), -(sideLength / 2),  (sideLength / 2), // 9
		 (sideLength / 2), -(sideLength / 2),  (sideLength / 2), // 10
		 (sideLength / 2), -(sideLength / 2),  (sideLength / 2), // 11

		-(sideLength / 2),  (sideLength / 2), -(sideLength / 2), // 12
		-(sideLength / 2),  (sideLength / 2), -(sideLength / 2), // 13
		-(sideLength / 2),  (sideLength / 2), -(sideLength / 2), // 14

		 (sideLength / 2),  (sideLength / 2), -(sideLength / 2), // 15
		 (sideLength / 2),  (sideLength / 2), -(sideLength / 2), // 16
		 (sideLength / 2),  (sideLength / 2), -(sideLength / 2), // 17

		-(sideLength / 2), -(sideLength / 2), -(sideLength / 2), // 18
		-(sideLength / 2), -(sideLength / 2), -(sideLength / 2), // 19
		-(sideLength / 2), -(sideLength / 2), -(sideLength / 2), // 20

		 (sideLength / 2), -(sideLength / 2), -(sideLength / 2), // 21
		 (sideLength / 2), -(sideLength / 2), -(sideLength / 2), // 22
		 (sideLength / 2), -(sideLength / 2), -(sideLength / 2)	 // 23
	};

	GLuint rubiks_points_vbo = 0;
	glGenBuffers(1, &rubiks_points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rubiks_points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rubiks_vertices), rubiks_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF RUBIKS VERTICES VBO ----------------------------------------------------

	// RUBIKS TEXTURES VBO -------------------------------------------------------------
	float rubiks_textures[] =
	{
		0.25, 0.333333, // 0
		0.25, 0.333333, // 1
		0.25, 0.333333, // 2
		0.50, 0.333333, // 3
		0.50, 0.333333, // 4
		0.50, 0.333333, // 5
		0.25, 0.666666, // 6
		0.25, 0.666666, // 7
		0.25, 0.666666, // 8
		0.50, 0.666666, // 9
		0.50, 0.666666, // 10
		0.50, 0.666666, // 11
		0.25, 0.000000, // 12
		0.00, 0.333333, // 13
		1.00, 0.333333, // 14
		0.50, 0.000000, // 15
		0.75, 0.333333, // 16
		0.75, 0.333333, // 17
		1.00, 0.666666, // 18
		0.25, 1.000000, // 19
		0.00, 0.666666, // 20
		0.75, 0.666666, // 21
		0.75, 0.666666, // 22
		0.50, 1.000000  // 23
	};

	GLuint rubiks_textures_vbo = 0;
	glGenBuffers(1, &rubiks_textures_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rubiks_textures_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rubiks_textures), rubiks_textures, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF CUBE TEXTURES VBO ------------------------------------------------------

	// CUBE INDEX LIST IBO ---------------------------------------------------------
	GLushort rubiks_index_array[] = {
		// FRONT
		0, 3, 9,
		9, 6, 0,

		// BACK
		18, 22, 17,
		17, 14, 18,

		// LEFT
		7, 20, 13,
		13, 2, 7,

		// RIGHT
		10, 4, 16,
		16, 21, 10,

		// TOP
		1, 12, 15,
		15, 5, 1,

		// BOTTOM
		11, 23, 19,
		19, 8, 11 };

	GLuint rubiks_index_ibo = 0;
	glGenBuffers(1, &rubiks_index_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rubiks_index_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rubiks_index_array), rubiks_index_array, GL_STATIC_DRAW);
	// END OF CUBE INDEX LIST IBO --------------------------------------------------
}

void GeometryGenerator::CreateStar(GLuint *starVAO)
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

	// STAR TEXTURES VBO -------------------------------------------------------------
	float rubiks_textures[] =
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

	GLuint rubiks_textures_vbo = 0;
	glGenBuffers(1, &rubiks_textures_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rubiks_textures_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rubiks_textures), rubiks_textures, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF STAR TEXTURES VBO ------------------------------------------------------

	// STAR INDEX LIST IBO ---------------------------------------------------------
	GLushort star_index_array[] =
	{
		// FRONT FACE
			// SQUARE MIDDLE
			16, 2, 6,
			15, 7, 11,

			// TRIANGLE NORTH
			0, 1, 17,

			// TRIANGLE EAST
			3, 4, 5,

			// TRIANGLE SOUTH
			8, 9, 10,

			// TRIANGLE WEST
			12, 13, 14,

			// THE SIDES
				// NORTH EAST RECTANGLES
				19, 20, 21,
				21, 18, 19,

				23, 24, 25,
				25, 22, 23,

				26, 27, 29,
				29, 28, 26,

				30, 31, 33,
				33, 32, 30,

				// SOUTH EAST RECTANGLES
				34, 35, 37,
				37, 36, 34,

				38, 39, 41,
				41, 40, 38,

				42, 43, 45,
				45, 44, 42,

				48, 46, 47,
				47, 49, 48,

				// SOUTH WEST RECTANGLES
				50, 52, 53,
				53, 51, 50,

				54, 55, 57,
				57, 56, 54,

				58, 59, 61,
				61, 60, 58,

				62, 63, 65,
				65, 64, 62,

				// NORTH WEST RECTANGLES
				68, 66, 67,
				67, 69, 68,

				70, 71, 73,
				73, 72, 70,

				74, 75, 77,
				77, 76, 74,

				78, 79, 81,
				81, 80, 78,

				// BACK FACE
					// SQUARE MIDDLE
					88, 84, 98,
					89, 97, 93,

					// NORTH TRIANGLE
					83, 82, 99,

					// EAST TRIANGLE
					87, 86, 85,

					// SOUTH TRIANGLE
					90, 92, 91,

					// WEST TRIANGLE
					94, 96, 95
	};

	GLuint star_index_ibo = 0;
	glGenBuffers(1, &star_index_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_index_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(star_index_array), star_index_array, GL_STATIC_DRAW);
	// END OF STAR INDEX LIST IBO --------------------------------------------------
}

void GeometryGenerator::CreateCube(float sideLength, GLuint *cubeVAO)
{
	glGenVertexArrays(1, cubeVAO);
	glBindVertexArray(*cubeVAO);
	// CUBE VERTICES VBO -----------------------------------------------------------
	float cube_vertices[] =
	{
		-(sideLength / 2),  (sideLength / 2),  (sideLength / 2),
		 (sideLength / 2),  (sideLength / 2),  (sideLength / 2),
		-(sideLength / 2), -(sideLength / 2),  (sideLength / 2),
		 (sideLength / 2), -(sideLength / 2),  (sideLength / 2),
		-(sideLength / 2),  (sideLength / 2), -(sideLength / 2),
		-(sideLength / 2), -(sideLength / 2), -(sideLength / 2),
		 (sideLength / 2), -(sideLength / 2), -(sideLength / 2),
		 (sideLength / 2),  (sideLength / 2), -(sideLength / 2)
	};

	GLuint cube_points_vbo = 0;
	glGenBuffers(1, &cube_points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF CUBE VERTICES VBO ----------------------------------------------------

	// CUBE COLORS VBO -------------------------------------------------------------
	float cube_colours[] = {
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0 };

	GLuint cube_colours_vbo = 0;
	glGenBuffers(1, &cube_colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colours), cube_colours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	// END OF CUBE COLORS VBO ------------------------------------------------------

	// CUBE INDEX LIST IBO ---------------------------------------------------------
	GLushort cube_index_array[] = {
		// FRONT
		0, 1, 2,
		1, 3, 2,

		// BACK
		5, 7, 4,
		5, 6, 7,

		// LEFT
		2, 4, 0,
		2, 5, 4,

		// RIGHT
		3, 1, 7,
		3, 7, 6,

		// TOP
		4, 1, 0,
		4, 7, 1,

		// BOTTOM
		5, 2, 3,
		5, 3, 6 };

	GLuint cube_index_ibo = 0;
	glGenBuffers(1, &cube_index_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_index_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index_array), cube_index_array, GL_STATIC_DRAW);
	// END OF CUBE INDEX LIST IBO --------------------------------------------------
}

void GeometryGenerator::CreatePyramid(GLuint *pyramidVAO)
{
	glGenVertexArrays(1, pyramidVAO);
	glBindVertexArray(*pyramidVAO);
	// PYRAMID VERTICES VBO -----------------------------------------------------------
	float pyramid_vertices[] =
	{
		-0.45, -0.45, 0.45,  // 0
		-0.45, -0.45, 0.45,  // 1
		-0.45, -0.45, 0.45,  // 2

		0.45,  -0.45, 0.45,  // 3
		0.45,  -0.45, 0.45,	 // 4
		0.45,  -0.45, 0.45,	 // 5

		-0.45, -0.45, -0.45, // 6
		-0.45, -0.45, -0.45, // 7
		-0.45, -0.45, -0.45, // 8

		0.45,  -0.45, -0.45, // 9
		0.45,  -0.45, -0.45, // 10
		0.45,  -0.45, -0.45, // 11

		0,     0.45,  0,     // 12
		0,     0.45,  0,	 // 13
		0,     0.45,  0,	 // 14
		0,     0.45,  0      // 15
	};

	GLuint pyramid_points_vbo = 0;
	glGenBuffers(1, &pyramid_points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// END OF PYRAMID VERTICES VBO ----------------------------------------------------

	// PYRAMID TEXTURES VBO -------------------------------------------------------------
	float pyramid_colours[] =
	{
		0.0, 1.0, // 0
		1.0, 1.0, // 1
		0.0, 0.0, // 2

		1.0, 1.0, // 3
		0.0, 1.0, // 4
		1.0, 0.0, // 5

		0.0, 0.0, // 6
		1.0, 0.0, // 7
		0.0, 1.0, // 8

		1.0, 0.0, // 9
		1.0, 1.0, // 10
		0.0, 0.0, // 11

		0.5, 1.0, // 12
		0.0, 0.5, // 13
		0.5, 0.0, // 14
		1.0, 0.5  // 15
	};

	GLuint pyramid_colours_vbo = 0;
	glGenBuffers(1, &pyramid_colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_colours), pyramid_colours, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	// END OF PYRAMID TEXTURES VBO ------------------------------------------------------

	// PYRAMID INDEX LIST IBO ---------------------------------------------------------
	GLushort pyramid_index_array[] = {
		// BOTTOM
		3, 9, 6,
		6, 0, 3,

		// FRONT
		5, 2, 12,

		// RIGHT
		15, 11, 4,

		// BACK
		10, 14, 8,

		// LEFT
		7, 13, 1 };

	GLuint pyramid_index_ibo = 0;
	glGenBuffers(1, &pyramid_index_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramid_index_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_index_array), pyramid_index_array, GL_STATIC_DRAW);
	// END OF PYRAMID INDEX LIST IBO --------------------------------------------------
}

void GeometryGenerator::CreateWedge(GLuint *wedgeVAO)
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
		0, 1, 3,
		3, 2, 0,

		4, 6, 5,

		7, 8, 10,
		10, 9, 7,

		12, 14, 13,
		13, 11, 12,

		15, 16, 17,
	};

	GLuint wedge_index_ibo = 0;
	glGenBuffers(1, &wedge_index_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wedge_index_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wedge_index_array), wedge_index_array, GL_STATIC_DRAW);
	// END OF WEDGE INDEX LIST IBO --------------------------------------------------
}

void GeometryGenerator::CreateTriPrism(GLuint *triPrismVAO)
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
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,
	};

	GLuint triPrism_index_ibo = 0;
	glGenBuffers(1, &triPrism_index_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triPrism_index_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triPrism_index_array), triPrism_index_array, GL_STATIC_DRAW);
	// END OF TRIPRISM INDEX LIST IBO --------------------------------------------------
}

void GeometryGenerator::CreatePyramid(GLuint *pyramidVAO) {

	float halfPyramidSide = 0.5f;
	float pyramidHeight = 1.0f;

	glGenVertexArrays(1, pyramidVAO);
	glBindVertexArray(*pyramidVAO);
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
}

