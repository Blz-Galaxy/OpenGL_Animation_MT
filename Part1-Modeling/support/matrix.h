// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// matrix.h
//
// This class defines a matrix class for 4x4 matrices



#ifndef _MATRIX_H
#define _MATRIX_H



class vectr;							// "preannounce" the vectr class so we can have
										// the vectr class as a friend of the matrix class
	


// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------

class matrix
{
		
	public:
		float	data[4][4];									// the matrix cells

				matrix();									// default constructor
		virtual	~matrix();									// destructor
		
		void set(float, float, float, float,				// initialiser
				 float, float, float, float, 
				 float, float, float, float, 
				 float, float, float, float);
				 
		void init(void);									// builds an identity matrix
		void setTranslate(float tx, float ty, float tz);	// init as translation matrix
		void setScale(float s);								// init as uniform scale matrix
		void setScale(float sx, float sy, float sz);		// init as nonuniform scale matrix
		void setRotateX(float r);							// init as x-axis rotation matrix
		void setRotateY(float r);							// init as y-axis rotation matrix
		void setRotateZ(float r);							// init as z-axis rotation matrix

		void print(void);
		
		matrix simplisticInverse(void);						// simplistic attempt to invert
		void multiplyIntoOpenGL();							// multiply into current OpenGL matrix

		// friend functions
		friend matrix	operator*(matrix &a, matrix &b);	// multiply 2 matrices
		friend vectr	operator*(matrix &a, vectr &b);		// multiply matrix by vectr
		
};


#endif // _MATRIX_H

