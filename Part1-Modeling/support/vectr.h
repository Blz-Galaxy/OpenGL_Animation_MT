// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// vectr.h
//
// This is a class used to represent a vector.  The core requirement is that
// this class provide storage for a 4 row vector to represent a point in
// 3D space.  A vectr can also be used to represent a normal.
//
// Do not confuse this class with the STL vector container!


#ifndef _VECTR_H
#define _VECTR_H



class matrix;							// "preannounce" the matrix class so we can have
										// the matrix class as a friend of the vectr class


// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class vectr
{
	public:
		float			data[4];							// the coordinate data for this vectr
		
						vectr();							// constructor
		virtual			~vectr();							// destructor

		void set(vectr);									// initialisers
		void set(float x, float y, float z);
		void set(float x, float y, float z, float w);

		void add(float x, float y, float z);				// add an offset to the vectr
		void reduceToUnit(void);							// normalise
		float length(void);									// calculate length of a vector
		
		void print(void);

		// friend functions
		friend vectr	operator+(vectr &a, vectr &b);		// add two vectrs
		friend vectr	operator-(vectr &a, vectr &b);		// subtract vectr b from vectr a
		friend vectr	operator*(matrix &a, vectr &b);		// multiply matrix by vectr
		friend vectr	operator*(vectr &a, float f);		// multiply a vectr by a float
		friend vectr	crossProduct(vectr &a, vectr &b);	// cross product of two vectrs
		friend float	dotProduct(vectr &a, vectr &b);		// dot product of two vectrs

};



#endif // _VECTR_H

