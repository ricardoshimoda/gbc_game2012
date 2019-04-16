#pragma once
#ifndef GEOMETRY_GENERATOR
#define GEOMETRY_GENERATOR


class GeometryGenerator {
public:
	int CreateStar(GLuint *starVAO);
	int CreateWedge(GLuint *wedgeVAO);
	int CreateTriPrism(GLuint *triPrismVAO);
	int CreateCube(GLuint *cubeVAO);  // YES
	int CreatePyramid(GLuint *pyramidVAO);
	int CreateHexagon(GLuint *hexagonVAO);
	int CreateSimsIndicator(GLuint *simsIndicatorVAO);
	int CreateCylinder(GLuint *cylinderVAO); // YES
	int CreateSphere(GLuint *sphereVAO); //YES
	int CreateCone(GLuint *coneVAO);
private:
	void crossProduct(float vect_A[], float vect_B[], float cross_P[]);
	void normalGenerator(float *vertices, GLushort *ibo, float *normals, int size);
	float epsilonCorrector(float value);
};

#endif // GAME_HPP
