// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//         Mike Cameron-Jones (inverse)
//
// matrix.cpp
//
// Class definitions for a matrix operations



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "matrix.h"
#include "utility.h"




// --------------------------------------------------------------------------------
// identity() - inits a matrix to the identity matrix
// --------------------------------------------------------------------------------

void matrix::init(void)
{
	int i;

	for (i=0; i<4; i++)
		data[i][0] = data[i][1] = data[i][2] = data [i][3] = 0.0;

	for (i=0; i<4; i++)
		data[i][i] = 1.0;

}



// --------------------------------------------------------------------------------
// matrix constructor (called automatically when a new matrix object is created)
// initialises the matrix as an identity matrix
// --------------------------------------------------------------------------------

matrix::matrix()
{
	init();
}



// --------------------------------------------------------------------------------
// destructor
// --------------------------------------------------------------------------------

matrix::~matrix()
{
}




// --------------------------------------------------------------------------------
// matrix initialiser
// we can call this to set the matrix to a specific set of values
// --------------------------------------------------------------------------------

void matrix::set(float r0c0, float r0c1, float r0c2, float r0c3,
				float r1c0, float r1c1, float r1c2, float r1c3,
				float r2c0, float r2c1, float r2c2, float r2c3,
				float r3c0, float r3c1, float r3c2, float r3c3)
{
	data[0][0] = r0c0; data[0][1] = r0c1; data[0][2] = r0c2; data[0][3] = r0c3;
	data[1][0] = r1c0; data[1][1] = r1c1; data[1][2] = r1c2; data[1][3] = r1c3;
	data[2][0] = r2c0; data[2][1] = r2c1; data[2][2] = r2c2; data[2][3] = r2c3;
	data[3][0] = r3c0; data[3][1] = r3c1; data[3][2] = r3c2; data[3][3] = r3c3;
}




// --------------------------------------------------------------------------------
// setTranslate
// builds a translation matrix
// --------------------------------------------------------------------------------

void matrix::setTranslate(float tx, float ty, float tz)
{
	init();					// reset to identity
	data[0][3] = tx;		// store the translation values
	data[1][3] = ty;
	data[2][3] = tz;
}




// --------------------------------------------------------------------------------
// setScale
// builds a uniform scaling matrix
// --------------------------------------------------------------------------------

void matrix::setScale(float s)
{
	init();					// reset to identity
	data[0][0] = s;			// store the same scaling factor for each axis
	data[1][1] = s;
	data[2][2] = s;
}




// --------------------------------------------------------------------------------
// setScale
// builds a non-uniform scaling matrix
// --------------------------------------------------------------------------------

void matrix::setScale(float sx, float sy, float sz)
{
	init();					// reset to identity
	data[0][0] = sx;		// store the scaling factor for each axis
	data[1][1] = sy;
	data[2][2] = sz;
}




// --------------------------------------------------------------------------------
// setRotateX
// builds an X-axis rotation matrix
// --------------------------------------------------------------------------------

void matrix::setRotateX(float rx)
{
	init();						// reset to identity
	data[1][1] = cos_d(rx);		// store the rotation values
	data[1][2] = -sin_d(rx);
	data[2][1] = sin_d(rx);
	data[2][2] = cos_d(rx);
}




// --------------------------------------------------------------------------------
// setRotateY
// builds an Y-axis rotation matrix
// --------------------------------------------------------------------------------

void matrix::setRotateY(float rx)
{
	init();						// reset to identity
	data[0][0] = cos_d(rx);		// store the rotation values
	data[0][2] = sin_d(rx);
	data[2][0] = -sin_d(rx);
	data[2][2] = cos_d(rx);
}




// --------------------------------------------------------------------------------
// setRotateZ
// builds an Z-axis rotation matrix
// --------------------------------------------------------------------------------

void matrix::setRotateZ(float rx)
{
	init();						// reset to identity
	data[0][0] = cos_d(rx);		// store the rotation values
	data[0][1] = -sin_d(rx);
	data[1][0] = sin_d(rx);
	data[1][1] = cos_d(rx);
}




// --------------------------------------------------------------------------------
// matrix printer (for debugging purposes)
// we can use this to display the contents of a matrix
// --------------------------------------------------------------------------------

void matrix::print(void)
{
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
			cout << data[i][j] << ",";
		cout << endl;
	}
	cout << endl;
}




// --------------------------------------------------------------------------------
// multiply two matrices
// --------------------------------------------------------------------------------

matrix operator*(matrix &a, matrix &b)
{
	matrix	temp;
	
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			temp.data[i][j] =  ((a.data[i][0] * b.data[0][j]) +
								(a.data[i][1] * b.data[1][j]) +
								(a.data[i][2] * b.data[2][j]) +
								(a.data[i][3] * b.data[3][j]));
	return temp;
}




// ---------------------------------------------------------------------------------
// simplistic inverse
//
// this assumes that matrix has special form in that last row is 0 0 0 1, as will be
// the case for matrices representing homogeneous coordinate transformations on 3D
// space where transformations arise from nothing more complex than rotations,
// translations and scalings, but is not true in general of 4 x 4 matrices, not even
// e.g. perspective projection matrices
//
// assumptions enable approach based on inverting upper left 3 x 3 using cofactors
// and working rest out from there -- could still have inaccuracies due to rounding
//
// assuming scalings are only uniform, could try alternative approach to inverting
// upper left 3 x 3 based on it being constant times orthogonal matrix
// ---------------------------------------------------------------------------------

matrix matrix::simplisticInverse()
{
	int cofactorIndices[3][2] = { {1, 2}, {0, 2}, {0, 1} };
	double temp[3][3];

	// get cofactors for upper left 3 x 3
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			temp[i][j] = (((i + j) % 2) ? -1.0 : 1.0) * 
							 ((double) data[cofactorIndices[i][0]][cofactorIndices[j][0]]
								* data[cofactorIndices[i][1]][cofactorIndices[j][1]]
							- (double) data[cofactorIndices[i][0]][cofactorIndices[j][1]]
								* data[cofactorIndices[i][1]][cofactorIndices[j][0]]);
		}
	}

	double determinant = 0;

	// get determinant
	for(int i = 0; i < 3; i++) determinant += data[i][0] * temp[i][0];
	
	matrix result;

	// evaluate last column, except last entry which is already initialised to 1
	for(int i = 0; i < 3; i++)
	{
		int j;
		double sum = 0;
		for(j = 0; j < 3; j++)
		{
			sum -= temp[j][i] * data[j][3];
		}
		result.data[i][3] = sum / determinant;
	}
	
	// upper left 3 x 3 is transpose of cofactors divided by determinant 
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			result.data[i][j] = temp[j][i] / determinant;
		}
	}
	
	return result;
}




// ---------------------------------------------------------------------------------
// multiply into OpenGL
//
// this copies into an array appropriate for use by OpenGL and then multiplies by it
// ---------------------------------------------------------------------------------

void matrix::multiplyIntoOpenGL()
{
	float a[16];						// need to put into this format to multiply into OpenGL
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			a[i + 4 * j] = data[i][j];
		}
	}
	glMultMatrixf(a);
}


