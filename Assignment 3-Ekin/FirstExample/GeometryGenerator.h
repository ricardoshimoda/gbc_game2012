#pragma once
#ifndef GEOMETRY_GENERATOR
#define GEOMETRY_GENERATOR


class GeometryGenerator {
public:
	int CreateStar(GLuint *starVAO);
	int CreateWedge(GLuint *wedgeVAO);
	int CreateTriPrism(GLuint *triPrismVAO);
	int CreateCylinder(GLuint *cylinderVAO);
private:
	void crossProduct(float vect_A[], float vect_B[], float cross_P[])
	{
		cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
		cross_P[1] = vect_A[0] * vect_B[2] - vect_A[2] * vect_B[0];
		cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
		float length = sqrt(cross_P[0] * cross_P[0] + cross_P[1] * cross_P[1] + cross_P[2] * cross_P[2]);
		cross_P[0] = cross_P[0] / length;
		cross_P[1] = cross_P[1] / length;
		cross_P[2] = cross_P[2] / length;
	}
	void normalGenerator(float *vertices, GLushort *ibo, float *normals, int size) {
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
			crossProduct (B, A, normal);
			vertexIndex = left * 3;
			normals[vertexIndex] = normal[0];
			normals[vertexIndex+1] = normal[1];
			normals[vertexIndex+1] = normal[2];

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
			normals[vertexIndex+1] = normal[1];
			normals[vertexIndex+2] = normal[2];

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
			normals[vertexIndex+1] = normal[1];
			normals[vertexIndex+2] = normal[2];
		}
	}
};

#endif // GAME_HPP
