// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// vectr.cpp
//
// Class definition for a vector



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "vectr.h"
#include "matrix.h"




// -----------------------------------------------------------------------------------------
// vectr constructor
// -----------------------------------------------------------------------------------------

vectr::vectr()
{
	set(0, 0, 0);
}




// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------

vectr::~vectr()
{
}




// -----------------------------------------------------------------------------------------
// set
// -----------------------------------------------------------------------------------------
// set the fields of the vector
// -----------------------------------------------------------------------------------------

void vectr::set(vectr v)
{
	data[0] = v.data[0];
	data[1] = v.data[1];
	data[2] = v.data[2];
	data[3] = v.data[3];
}

void vectr::set(float x, float y, float z)
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
	data[3] = 1.0;
}

void vectr::set(float x, float y, float z, float w)
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
	data[3] = w;
}




// -----------------------------------------------------------------------------------------
// add
// -----------------------------------------------------------------------------------------
// add offset to a vectr
// -----------------------------------------------------------------------------------------

void vectr::add(float x, float y, float z)
{
	data[0] += x;
	data[1] += y;
	data[2] += z;
}




// -----------------------------------------------------------------------------------------
// calculate the length of a vector
// -----------------------------------------------------------------------------------------

float vectr::length(void) 
{
	return (float)sqrt(	(data[0] * data[0]) + 
						(data[1] * data[1]) + 
						(data[2] * data[2])); 
}




// -----------------------------------------------------------------------------------------
// reduce the vector to unit length (normalise)
// -----------------------------------------------------------------------------------------

void vectr::reduceToUnit(void) 
{ 
	// calculate the length of the vector 
	float len = this->length(); 

	// prevent div by zero
	if (len == 0.0f) return; 

	// dividing each element by the length will result in a unit normal vector. 
	data[0] /= len; 
	data[1] /= len; 
	data[2] /= len; 
}





// -----------------------------------------------------------------------------------------
// vector printer (for debugging purposes)
// we can use this to display the contents of a vector
// -----------------------------------------------------------------------------------------

void vectr::print()
{
	for (int i=0; i<4; i++)
		cout << data[i] << ",";
	cout << endl << endl;
}




// -----------------------------------------------------------------------------------------
// add two vectrs
// -----------------------------------------------------------------------------------------

vectr operator+(vectr &a, vectr &b)
{
	vectr temp;
	temp.data[0] = a.data[0] + b.data[0];
	temp.data[1] = a.data[1] + b.data[1];
	temp.data[2] = a.data[2] + b.data[2];
	return temp;
}



// -----------------------------------------------------------------------------------------
// subtract one vector from another
// -----------------------------------------------------------------------------------------

vectr operator-(vectr &a, vectr &b)
{
	vectr temp;
	temp.data[0] = a.data[0] - b.data[0];
	temp.data[1] = a.data[1] - b.data[1];
	temp.data[2] = a.data[2] - b.data[2];
	return temp;
}



// --------------------------------------------------------------------------------
// multiply a matrix by a vector (gives a vector result)
// --------------------------------------------------------------------------------

vectr operator*(matrix &a, vectr &b)
{
	vectr	temp;

	for (int i=0; i<4; i++)
		temp.data[i] =	(a.data[i][0] * b.data[0]) +
						(a.data[i][1] * b.data[1]) +
						(a.data[i][2] * b.data[2]) +
						(a.data[i][3] * b.data[3]); 
	return temp;

}



// --------------------------------------------------------------------------------
// scale a vector by a float (gives a vector result)
// --------------------------------------------------------------------------------

vectr operator*(vectr &a, float b)
{
	vectr	temp;

	for (int i=0; i<4; i++)
		temp.data[i] = a.data[i] * b;
	return temp;

}



// --------------------------------------------------------------------------------
// calculate the cross product of two vectrs
// --------------------------------------------------------------------------------

vectr crossProduct(vectr &a, vectr &b)
{
	vectr	temp;
	
	static const int x = 0; 
	static const int y = 1; 
	static const int z = 2; 

	temp.data[x] = a.data[y]*b.data[z] - a.data[z]*b.data[y]; 
	temp.data[y] = a.data[z]*b.data[x] - a.data[x]*b.data[z]; 
	temp.data[z] = a.data[x]*b.data[y] - a.data[y]*b.data[x]; 

	return temp;

}




// --------------------------------------------------------------------------------
// calculate the dot product of two vectrs
// --------------------------------------------------------------------------------

float dotProduct(vectr &a, vectr &b)
{
	float temp;
	
	static const int x = 0; 
	static const int y = 1; 
	static const int z = 2; 

	temp = (a.data[x] * b.data[x]) + (a.data[y] * b.data[y]) + (a.data[z] * b.data[z]); 

	return temp;
}


