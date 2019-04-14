#pragma once
#ifndef GEOMETRY_GENERATOR
#define GEOMETRY_GENERATOR

#include "glm\glm.hpp"

class GeometryGenerator {
public:
	void CreateRubiksCube(float sideLength, GLuint *rubiksVAO);
	void CreateStar(GLuint *starVAO);
	void CreateCube(float sideLength, GLuint *cubeVAO);
	void CreatePyramid(GLuint *pyramidVAO);
	void CreateWedge(GLuint *wedgeVAO);
	void CreateTriPrism(GLuint *triPrismVAO);
	void CreatePyramid(GLuint *pyramidVAO);
};

#endif // GAME_HPP