// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//         Mike Cameron-Jones (disconnect/connect capability)
//
// object3d.cpp
//
// This file defines a class for generic 3D objects



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "object3d.h"
#include "constants.h"
#include "camera.h"
#include "utility.h"
#include "platform.h"





// -----------------------------------------------------------------------------------------
// global variables
// -----------------------------------------------------------------------------------------
extern cameraClass		gCamera;
extern unsigned long	gPolygonCount;			// counts how many polygons get displayed each frame
extern ProgramMode		gProgramMode;			// current execution mode
extern bool				gShowLocalAxes;			// if true, draw local axes (2x2x2) as object is drawn
extern bool				gShowFaceNormals;		// if true, draw face normals
extern bool             gWireFrameOnly;
extern bool				gAlwaysDrawVertices;
extern float			gCurrentFrameTime;		// the time (since the animation started) for the frame being animated




// -----------------------------------------------------------------------------------------
// default constructor
// -----------------------------------------------------------------------------------------
object3d::object3d()
{
	vertexCount = 0;						// no vertices
	faceCount = 0;							// no faces
	polygonCount = 0;						// no polygons

	setDeformation(1.0, 1.0, 1.0);			// default deformation (none)
	setScale(1.0);							// default uniform scale
	setOffset(0, 0, 0);                     // default pre-rotation translation
	setRotation();							// default rotation
	setPosition(0, 0, 0);                   // default post-rotation translation
	setColour(1.0, 1.0, 1.0, 1.0);			// default colour (white)
	setSpecularColour(1.0, 1.0, 1.0, 1.0);	// default specular colour (white)
	setEmissiveColour(0.0, 0.0, 0.0, 1.0);	// default emissive colour (black)
	
	theShininess = 25;						// default shininess

	diffuseShading = true;					// default to per-face normals for lighting
	invisible = false;						// default to visible
	
	parent = 0;								// no parentage yet
	parentChangeTime = 0;
	
	autoBindToTexture = true;				// default to autobinding
	theTexture = theTexture2 = 0;			// no textures
	textureScaleX = textureScaleY = texture2ScaleX = texture2ScaleY = 1.0;
	
	theShader = 0;							// no shader
	
	listName = 0;							// no call list
	
	objectSerial = nextSerial++;			// give this object a serial number and a name
	setName("object");
	
	useConnectMatrix = false;				// no matrix to interpose
	connectMatrix.init();
}



// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------
object3d::~object3d()
{
}




// -----------------------------------------------------------------------------------------
// setName
// -----------------------------------------------------------------------------------------
void object3d::setName(string theName)
{
	ostringstream s;
	s << theName << "_" << objectSerial;
	objectName = s.str();
}




// -----------------------------------------------------------------------------------------
// setPosition
// -----------------------------------------------------------------------------------------
// sets the object's position (it's post-rotation translation values)
// -----------------------------------------------------------------------------------------
void object3d::setPosition(float x, float y, float z)
{
	position.set(x, y, z);
}

void object3d::setPosition(vectr &v)
{
	position = v;
}




// -----------------------------------------------------------------------------------------
// getPosition
// -----------------------------------------------------------------------------------------
// reads back the object's position (it's post-rotation translation values)
// -----------------------------------------------------------------------------------------
void object3d::getPosition(float &x, float &y, float &z)
{
	x = position.data[0];
	y = position.data[1];
	z = position.data[2];
}

vectr object3d::getPosition(void)
{
	return position;
}



// -----------------------------------------------------------------------------------------
// setOffset
// -----------------------------------------------------------------------------------------
// sets the object's geometry offset (it's pre-rotation translation values)
// -----------------------------------------------------------------------------------------
void object3d::setOffset(float x, float y, float z)
{
	offset.set(x, y, z);
}

void object3d::setOffset(vectr &v)
{
	offset = v;
}




// -----------------------------------------------------------------------------------------
// getOffset
// -----------------------------------------------------------------------------------------
// reads back the object's geometry offset (it's pre-rotation translation values)
// -----------------------------------------------------------------------------------------
void object3d::getOffset(float &x, float &y, float &z)
{
	x = offset.data[0];
	y = offset.data[1];
	z = offset.data[2];
}

vectr object3d::getOffset(void)
{
	return offset;
}



// -----------------------------------------------------------------------------------------
// setRotation
// -----------------------------------------------------------------------------------------
// set the object's rotation values
// -----------------------------------------------------------------------------------------
void object3d::setRotation(char axis1, float angle1, char axis2, float angle2, char axis3, float angle3)
{

	axis1 = tolower(axis1);
	axis2 = tolower(axis2);
	axis3 = tolower(axis3);
	
	if (axis2 == '\0') axis2 = (axis1 == 'x') ? 'y':'x';
	
	if (axis3 == '\0')
	{
		if (axis1 == 'x')			axis3 = (axis2 == 'y') ? 'z':'y';
		else if (axis1 == 'y')		axis3 = (axis2 == 'x') ? 'z':'x';
		else /* (axis1 == 'z') */	axis3 = (axis2 == 'x') ? 'y':'x';
	}

	if ((axis1 < 'x') || (axis1 > 'z') ||
		(axis2 < 'x') || (axis2 > 'z') ||
		(axis3 < 'x') || (axis3 > 'z'))
			fatal ("illegal axis specified in setRotation() call (must be x, y or z)");


	// build up an int as ddd where each digit is (0, 1, or 2) and represents (x, y, or z)
	int axesInOrder = ((axis1 - 'x') * 100) + ((axis2 - 'x') * 10) + (axis3 - 'x');
	int rotOrder;
	
	// remap the order of the axes to match the order the rotations should be applied
	switch (axesInOrder)
	{
		case  12: rotOrder =  12; break;
		case  21: rotOrder =  21; break;
		case 102: rotOrder = 102; break;
		case 120: rotOrder = 201; break;
		case 210: rotOrder = 210; break;
		case 201: rotOrder = 120; break;
		default: fatal ("setRotation() called with duplicated axis parameter!");
	}

	rotationOrder.set(rotOrder / 100, (rotOrder / 10) % 10, rotOrder % 10);

	rotation.data[0] = angle1;
	rotation.data[1] = angle2;
	rotation.data[2] = angle3;

}




// -----------------------------------------------------------------------------------------
// setScale
// -----------------------------------------------------------------------------------------
// set the object's scale value (for uniform scaling)
// -----------------------------------------------------------------------------------------
void object3d::setScale(float s)
{
	if (s == 0.0) fatal ("setScale called with a value of 0");
	scale = s;
}





// -----------------------------------------------------------------------------------------
// setDeformation
// -----------------------------------------------------------------------------------------
// set the object's deformation values (for non-uniform scaling)
// -----------------------------------------------------------------------------------------
void object3d::setDeformation(float x, float y, float z)
{
	if ((x == 0.0) || (y == 0.0) || (z == 0.0)) fatal ("setDeformation called with a value of 0");
	deformation.set(x, y, z);
}





// -----------------------------------------------------------------------------------------
// setColour, getColour
// -----------------------------------------------------------------------------------------
// set the material colour of the object
// -----------------------------------------------------------------------------------------
void object3d::setColour(float red, float green, float blue)	
{
	theColour[0] = red;
	theColour[1] = green;
	theColour[2] = blue;
	theColour[3] = 1.0;
	objectOpacity = false;
}

void object3d::setColour(float red, float green, float blue, float alpha)	
{
	theColour[0] = red;
	theColour[1] = green;
	theColour[2] = blue;
	theColour[3] = alpha;
	objectOpacity = (alpha < 1.0) ? kTransparentMode:kOpaqueMode;
}

void object3d::getColour(float &r, float &g, float &b, float &a)	
{
	r = theColour[0];
	g = theColour[1];
	b = theColour[2];
	a = theColour[3];
}

void object3d::setColour(float aColour[])
{
	theColour[0] = aColour[0];
	theColour[1] = aColour[1];
	theColour[2] = aColour[2];
	theColour[3] = aColour[3];
	
	objectOpacity = (theColour[3] < 1.0) ? kTransparentMode:kOpaqueMode;
}




// -----------------------------------------------------------------------------------------
// setSpecularColour
// -----------------------------------------------------------------------------------------
// set the specular colour of the object
// -----------------------------------------------------------------------------------------
void object3d::setSpecularColour(float red, float green, float blue, float alpha)
{
	theSpecularColour[0] = red;
	theSpecularColour[1] = green;
	theSpecularColour[2] = blue;
	theSpecularColour[3] = alpha;
}

void object3d::setSpecularColour(float aColour[])
{
	theSpecularColour[0] = aColour[0];
	theSpecularColour[1] = aColour[1];
	theSpecularColour[2] = aColour[2];
	theSpecularColour[3] = aColour[3];
}




// -----------------------------------------------------------------------------------------
// setEmissiveColour
// -----------------------------------------------------------------------------------------
// set the emissive colour of the object
// -----------------------------------------------------------------------------------------
void object3d::setEmissiveColour(float red, float green, float blue, float alpha)
{
	theEmissiveColour[0] = red;
	theEmissiveColour[1] = green;
	theEmissiveColour[2] = blue;
	theEmissiveColour[3] = alpha;
}

void object3d::setEmissiveColour(float aColour[])
{
	theEmissiveColour[0] = aColour[0];
	theEmissiveColour[1] = aColour[1];
	theEmissiveColour[2] = aColour[2];
	theEmissiveColour[3] = aColour[3];
}




// -----------------------------------------------------------------------------------------
// makeInsideFaces
// -----------------------------------------------------------------------------------------
// duplicates all vertices (inside and outside faces can't share vertices or the vertex
// normals will be wrong)
// duplicates all faces, then modifies the duplicated faces so that THEY use the 
// duplicated vertices, switching the winding order of the vertices so the normals point
// inside instead of outside
// this function is used when a shape should be transparent, and saves the hassle of 
// creating 2 sets of faces in the constructor
// -----------------------------------------------------------------------------------------
void object3d::makeInsideFaces(void)
{
	// can't do anything for objects that have no faces or vertices, and objects that have a display list
	if ((vertexCount < 1) || (faceCount < 1))
		return;
	
	// if the object has a display list, adding inside face geometry is going to invalidate it, so 
	// delete and remove the link to it
	if (listName) 
	{
		glDeleteLists(listName, 1);
		listName = 0;
	}

	// duplicate all of the vertices
	vertices.insert(vertices.end(), vertices.begin(), vertices.end());
	
	// duplicate all of the faces
	faces.insert(faces.end(), faces.begin(), faces.end());
	
	// rearrange the order of the vertices so that the faces are "inside out", and 
	// use the duplicated vertices in the upper half of the array
	for (unsigned int i=0; i < faceCount; i++)
	{
		if (faces[i].vertices == 3)
			faces[i+faceCount].init(faces[i].vertex(0) + vertexCount, faces[i].vertex(2) + vertexCount, faces[i].vertex(1) + vertexCount);
		else
			faces[i+faceCount].init(faces[i].vertex(0) + vertexCount, faces[i].vertex(3) + vertexCount, faces[i].vertex(2) + vertexCount, faces[i].vertex(1) + vertexCount);
	}
	
	// correct the face and polygon counts
	faceCount *= 2;
	polygonCount *= 2;

	// correct the vertexCount
	vertexCount *= 2;
		
	// now recalculate the normals for the new faces & vertices
	calculateNormals();

}




// -----------------------------------------------------------------------------------------
// buildModelViewMatrix
// -----------------------------------------------------------------------------------------
// have OpenGL build the model view matrix (recursively), starting at the top parent
// of the group, descending until finally reaching the child for which we were originally
// called
// -----------------------------------------------------------------------------------------
void object3d::buildModelViewMatrix(int depth)
{
	if (parent)
		parent->buildModelViewMatrix(depth + 1);
	
	if(useConnectMatrix)
		connectMatrix.multiplyIntoOpenGL();
	
	glTranslatef(position.data[0], position.data[1], position.data[2]);
	
	for (int i=2; i>=0; i--)
	{
		glRotatef(rotation.data[i],
					(int)rotationOrder.data[0] == i ? 1.0:0.0,
					(int)rotationOrder.data[1] == i ? 1.0:0.0,
					(int)rotationOrder.data[2] == i ? 1.0:0.0);
	}

	glTranslatef(offset.data[0], offset.data[1], offset.data[2]);	
	
	glScalef(scale, scale, scale);
	
	if (depth == 0)
	{
		// only apply the deformation scale values if we're at the bottom
		// of the call chain (that is, a parent's deformation is not passed on to the children)
		glScalef(deformation.data[0], deformation.data[1], deformation.data[2]);
	}
}





// -----------------------------------------------------------------------------------------
// buildTransformMatrix
// -----------------------------------------------------------------------------------------
// build a composite transform matrix (recursively), starting at the top parent
// of the group, descending until finally reaching the child for which we were originally
// called
// ignores the deformation and scale values as they should not affect things at all
// -----------------------------------------------------------------------------------------
void object3d::buildTransformMatrix(int depth, matrix &finalMatrix)
{
	matrix tempMatrix;
	
	if (parent)
		parent->buildTransformMatrix(depth + 1, finalMatrix);
	
	if(useConnectMatrix)
		finalMatrix = finalMatrix * connectMatrix;
	
	// post rotation translation first
	tempMatrix.setTranslate(position.data[0], position.data[1], position.data[2]);
	finalMatrix = finalMatrix * tempMatrix;
	
	// then rotation, according to the rotation order
	for (int i=2; i>=0; i--)
	{
		if		((int)rotationOrder.data[0] == i)	tempMatrix.setRotateX (rotation.data[i]);
		else if ((int)rotationOrder.data[1] == i)	tempMatrix.setRotateY (rotation.data[i]);
		else if ((int)rotationOrder.data[2] == i)	tempMatrix.setRotateZ (rotation.data[i]);
		
		finalMatrix = finalMatrix * tempMatrix;
	}


	// then apply the pre-rotation translation
	tempMatrix.setTranslate(offset.data[0], offset.data[1], offset.data[2]);
	finalMatrix = finalMatrix * tempMatrix;
	

	// now apply uniform scale
	tempMatrix.setScale(scale);
	finalMatrix = finalMatrix * tempMatrix;

	
	if (depth == 0)
	{
		// for child only, apply deformation
		tempMatrix.setScale(deformation.data[0], deformation.data[1], deformation.data[2]);
		finalMatrix = finalMatrix * tempMatrix;
	}
}





// -----------------------------------------------------------------------------------------
// calculateDistanceToCamera
// -----------------------------------------------------------------------------------------
// calculate and store the distance from this object's origin to the camera
// -----------------------------------------------------------------------------------------
void object3d::calculateDistanceToCamera()
{
	matrix		distCalcTemp;							// a temporary matrix used to calculate the distance to the camera
	vectr		objectPosn;
	vectr		cameraPosn = gCamera.getPosition();
	
	distCalcTemp.init();								// set temp matrix to the identity matrix
	buildTransformMatrix(0, distCalcTemp);				// build a transform matrix
	globalTransform = distCalcTemp;						// save this value for later
	objectPosn =  globalTransform * objectPosn;			// apply the transform to (0, 0, 0) to get the object's world position
	
	objectPosn = objectPosn - cameraPosn;				// calculate the distance between the vectors as a vector
	distanceToCamera = objectPosn.length();				// work out the length of the vector
}





// -----------------------------------------------------------------------------------------
// setParent()
// -----------------------------------------------------------------------------------------
// sets the parent of the current object to be another object
// -----------------------------------------------------------------------------------------
void object3d::setParent(object3d *theParent)			// set an object's parent
{
	parent = theParent;
	parentChangeTime = gCurrentFrameTime;				// record the time, so we can flash updates
}




// -----------------------------------------------------------------------------------------
// attachToParentAt()
// -----------------------------------------------------------------------------------------
// sets the parent of the current object to be another object, and sets the current 
// object's "position" 
// -----------------------------------------------------------------------------------------
void object3d::attachToParentAt(object3d *theParent, float x, float y, float z)
{
	parent = theParent;
	parentChangeTime = gCurrentFrameTime;
    setPosition(x, y, z); 
}






// -----------------------------------------------------------------------------------------
// draw()
// -----------------------------------------------------------------------------------------
// draws the shape
// -----------------------------------------------------------------------------------------
void object3d::draw(bool drawMode)
{
	// ignore this call for invisible objects (and their children)
	if (invisible) return;

	// build a matrix to size, orient, and position the object
	// this is all done inside the OpenG/L engine - in the future, we may be able to
	// just send the globalTransform matrix to OpenGL
	glLoadIdentity();
	gCamera.tellGL();
	buildModelViewMatrix(0);


	// tell OpenG/L about the object characteristics
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, theColour);
	glMaterialfv (GL_FRONT, GL_SPECULAR, theSpecularColour);
	glMaterialfv (GL_FRONT, GL_EMISSION, theEmissiveColour);
	glMaterialf (GL_FRONT, GL_SHININESS, theShininess);	


	// give shaders preference to textures
	if (theShader)
		theShader->activate();
	
	// if autobind is on, and a texture has been defined, bind to it and set the default wrap and filter options
	// note - we only bind to the first texture (multitexture will require more complex privateDraw)
	else if (autoBindToTexture && theTexture && !gWireFrameOnly)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, theTexture->id());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, theTexture->bestMagFilter());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, theTexture->bestMinFilter());
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glScalef(textureScaleX, textureScaleY, 1.0);
		glMatrixMode(GL_MODELVIEW);
	}

	// set the shading model 
	if (diffuseShading)
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);


	// call the "real" draw method if we're in the appropriate drawing mode
	if (drawMode == objectOpacity) 
	{
		// draw the local axes if necessary
		if (gShowLocalAxes)
		{
			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
			glColor4f(1.0, 0.0, 0.0, 1.0);
			glVertex3f(-2.0, 0, 0); glVertex3f(2, 0, 0);
			glColor4f(0.0, 1.0, 0.0, 1.0);
			glVertex3f(0.0, -2, 0); glVertex3f(0, 2, 0);
			glColor4f(0.0, 0.0, 1.0, 1.0);
			glVertex3f(0.0, 0, -2); glVertex3f(0, 0, 2);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			glEnd();
			glEnable(GL_LIGHTING);
		}

		// count the number of polygons to be drawn
		gPolygonCount += polygonCount;
		
		privateDraw(drawMode);
	}
	
	
	// if we turned on texturing, we turn it off
	if (theShader)
		shaderprogram::deactivate();

	// if we turned on texturing, we turn it off
	else if (autoBindToTexture && theTexture && !gWireFrameOnly)
		glDisable(GL_TEXTURE_2D);
	
}





// -----------------------------------------------------------------------------------------
// privateDraw()
// -----------------------------------------------------------------------------------------
// send the object's vertex information down the OpenGL pipeline
//
// if the object is already recorded into an OpenGL display list, then
// just execute that list, otherwise draw all of the faces individually.
//
// drawing the faces individually is probably the least efficient way to send
// the data to OpenGL but it provides a suitable fallback for objects which don't
// have their own optimised draw functions
// -----------------------------------------------------------------------------------------
void object3d::privateDraw(bool drawMode)
{

	// if the shape has been precompiled into a display list, we should try to play it
	// unless the user has put the system into "show face normals" mode (in which case, 
	// we should only play the list if there is no geometry for us to draw manually)
	if ((listName != 0) && ((!gShowFaceNormals) || (faceCount == 0)))
	{
		glCallList(listName);
		return;
	}

	// it looks like we have to draw the shape ourselves...
	// this will be non-optimised draw, since we have no knowledge of how
	// the faces are linked to make surfaces
	
	
	// if no faces are defined yet for the shape, draw just the vertices as large dots
	if ((faceCount == 0) || gAlwaysDrawVertices)
	{
		glPointSize(5.0);
		glBegin(GL_POINTS);
		for (unsigned int i=0; i<vertexCount; i++)
			glVertex3fv(vertices[i].coordinate.data);
		glEnd();
		glPointSize(1.0);
		if (faceCount == 0) return;
	}


	// if the shape is transparent, depth sort the faces so the more distant faces
	// get drawn first (we don't need to do this for opaque objects)
	if (drawMode == kTransparentMode)
		distanceSortFaces();
	
	
	
	// draw the faces
	// note that we do this in two passes - first we draw the quads, and then we draw the triangles
	// by splitting like this, we can draw all quads between a single glBegin/glEnd pair, and
	// similarly we can draw all triangles between a single glBegin/glEnd pair 
	// this should provide a significant performace boost over drawing each face in order,
	// and sending a glBegin/glEnd for each face
	
	// draw the quads
	glBegin(GL_QUADS);
	for (unsigned int i=0; i<faceCount; i++)
	{
		if (faces[i].vertices == 4)
		{
		// send the first normal (the only normal if this shape has flat faces)
		if (diffuseShading) glNormal3fv(faces[i].faceNormal.data); 
		else glNormal3fv(vertices[faces[i].vertex(0)].normal.data);
		
		// first vertex
		glVertex3fv(vertices[faces[i].vertex(0)].coordinate.data);	

		// second normal & vertex
		if (!diffuseShading) glNormal3fv(vertices[faces[i].vertex(1)].normal.data);
		glVertex3fv(vertices[faces[i].vertex(1)].coordinate.data);
		
		// third normal & vertex
		if (!diffuseShading) glNormal3fv(vertices[faces[i].vertex(2)].normal.data);
		glVertex3fv(vertices[faces[i].vertex(2)].coordinate.data);

		// fourth normal & vertex
		if (!diffuseShading) glNormal3fv(vertices[faces[i].vertex(3)].normal.data);
		glVertex3fv(vertices[faces[i].vertex(3)].coordinate.data);
		}
	}
	glEnd();
	
	
	// draw the triangles
	glBegin(GL_TRIANGLES);
	for (unsigned int i=0; i<faceCount; i++)
	{
		if (faces[i].vertices == 3)
		{
			// send the first normal (the only normal if this shape has flat faces)
			if (diffuseShading) glNormal3fv(faces[i].faceNormal.data); 
			else glNormal3fv(vertices[faces[i].vertex(0)].normal.data);
			
			// first vertex
			glVertex3fv(vertices[faces[i].vertex(0)].coordinate.data);	

			// second normal & vertex
			if (!diffuseShading) glNormal3fv(vertices[faces[i].vertex(1)].normal.data);
			glVertex3fv(vertices[faces[i].vertex(1)].coordinate.data);
			
			// third normal & vertex
			if (!diffuseShading) glNormal3fv(vertices[faces[i].vertex(2)].normal.data);
			glVertex3fv(vertices[faces[i].vertex(2)].coordinate.data);
		}
	}
	glEnd();


	// finally, draw the face normals if this feature is turned on
	if (gShowFaceNormals)
	{ 
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor4f(0.0, 0.0, 1.0, 1.0);
		for (unsigned int i=0; i<faceCount; i++)
		{
			vectr temp = faces[i].centroid;
			for (int j=0; j<3; j++) 
				temp.data[j] = temp.data[j] + (faces[i].faceNormal.data[j]/5.0);
			glVertex3fv(faces[i].centroid.data);
			glVertex3fv(temp.data);
		}
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glEnd();
		glEnable(GL_LIGHTING);
	}

}





// -----------------------------------------------------------------------------------------
// drawParentLink()
// -----------------------------------------------------------------------------------------
// draws a solid black line between the object and its parent (debugging tool)
// -----------------------------------------------------------------------------------------
void object3d::drawParentLink()
{
	// ignore this call for invisible objects
	if (invisible) return;

	if (parent)
	{
		vectr a, b;
		a = globalTransform * a;
		b = parent->globalTransform * b;

		if ((gCurrentFrameTime > parentChangeTime + 1.0) || (gCurrentFrameTime < parentChangeTime))
		{
			glLineWidth(2.0);
			glColor3f(0.0, 0.0, 0.0);
		}
		else
		{
			glLineWidth(2.0 + (8.0 * (1.0 - (gCurrentFrameTime - parentChangeTime))));
			glColor3f(1.0, 0.0, 0.0);
		}
		
		glBegin(GL_LINES);
		glVertex3fv(a.data);
		glVertex3fv(b.data);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glEnd();

		glLineWidth(1);
	}
}





// --------------------------------------------------------------------------------
// calculateNormals()
// --------------------------------------------------------------------------------
// this function calculates the face and vertex normals for the object
// face normals are used for diffuse shading, and vertex normals are used for
// smooth (Gouraud) shading
// it also calculates the centroid for each face, which is used as the centre
// of the face for the purpose of depth sorting
// --------------------------------------------------------------------------------
void object3d::calculateNormals()
{

	// first, calculate the normal for each face 
	for (unsigned int i=0; i<faceCount; i++)
	{
		calculateNormal( faces[i].faceNormal,							// <--- receiver
							vertices[faces[i].vertex(0)].coordinate,	// <--- first vertex
							vertices[faces[i].vertex(1)].coordinate,	// <--- second vertex
							vertices[faces[i].vertex(2)].coordinate);	// <--- third vertex
	}
	
	
	// init the surface normals for each vertex to 0
	for (unsigned int i=0; i<vertexCount; i++)
		vertices[i].normal.set(0.0, 0.0, 0.0);


	// for each vertex, sum the surface normals of the surrounding faces
	for (unsigned int i=0; i<faceCount; i++)
		for (unsigned int j=0; j<faces[i].vertices; j++)
			vertices[faces[i].vert[j]].normal = vertices[faces[i].vert[j]].normal + faces[i].faceNormal;
	
	
	// now normalise the vertex normals
	for (unsigned int i=0; i<vertexCount; i++)
		vertices[i].normal.reduceToUnit();


	// calculate the centroid for each face
	for (unsigned int i=0; i<faceCount; i++)
	{
		vectr	temp;
		for (unsigned int f=0; f < faces[i].vertices; f++)
			temp = temp + vertices[faces[i].vertex(f)].coordinate;
		temp.data[0] /= faces[i].vertices;
		temp.data[1] /= faces[i].vertices;
		temp.data[2] /= faces[i].vertices;
		faces[i].centroid = temp;
	}
}




// --------------------------------------------------------------------------------
// distanceSortFaces()
// --------------------------------------------------------------------------------
// this function re-sorts the face vector of the object so that faces further from
// the camera are at the start of the vector, and faces closer to the camera are
// at the end of the vector
//
// this function exists to allow transparent objects to be drawn correctly, but
// since it reorders the face vector, it means that faces must be drawn one at a 
// time - it is not possible to use display lists, or strips of faces (for 
// optimisation) once this function has been called
// 
// this function requires that the object's globalTransform matrix is aready valid
// as this is applied to the centroid of each face to get that face's everage depth
//
// the global transform is set by the calculateDistanceToCamera function (a 
// member of this class), which is called in drawScene, before any drawing is done.
// --------------------------------------------------------------------------------
void object3d::distanceSortFaces()
{
	vectr		facePosn;
	vectr		cameraPosn = gCamera.getPosition();

	// start by calculating the distance fron the camera to each face
	for (unsigned int i=0; i<faceCount; i++)
	{
		// move the centroid of the face to it's location in world space 
		facePosn = globalTransform * faces[i].centroid;
		
		// calculate the distance between the vectors as a vector
		facePosn = facePosn - cameraPosn;
		
		// work out the length of the vector from the camera to that vertex
		faces[i].distanceToCamera = facePosn.length();
	}

	// now sort the face array based on the camera distance
	sort(faces.begin(), faces.end(), faceToCameraDistanceComparison);
}




// -----------------------------------------------------------------------------------
// disconnect()
// -----------------------------------------------------------------------------------
// this function disconnects an object from its parent
//
// the intent of the function is that when the disconnection occurs, the transform
// that was being applied at the moment of disconnection due to the parent's existence
// will be computed and cached so that it continues to be applied to the object after
// the disconnection so that the object remains where it was left by its parent (if 
// the object isn't moving of its own accord)
//
// there may be problems with numerical discrepancies between different calculations
// of the "same" matrix in OpenGL and KXC354 matrix class -- could alternatively
// do the matrix operations in OpenGL to try to reduce this
// -----------------------------------------------------------------------------------
void object3d::disconnect()
{
	matrix		distCalcTemp;					// a temporary matrix used to calculate the distance to the camera

	// trying to disconnect parentless object -- ignore
	if(!parent) return;						
	
	// get the transform being applied from above
	distCalcTemp.init();
	parent->buildTransformMatrix(1, distCalcTemp);
	
	// apply in front of existing connect matrix 
	connectMatrix = distCalcTemp * connectMatrix;
	
	setParent(0);
	useConnectMatrix = true;
}




// -----------------------------------------------------------------------------------
// connect()
// -----------------------------------------------------------------------------------
// this function connects an object to a (new) parent
//
// the intent of the function is that when the connection occurs, the transform
// that will be applied at the moment of connection due to the parent's existence
// will be computed and its inverse cached so that it can be applied to the object after
// the connection so that the object remains where it is relative to its parent (if 
// the object isn't moving of its own accord)
//
// there may be problems with numerical discrepancies between different calculations
// of the "same" matrix in OpenGL and KXC354 matrix class
//
// possible alternatives to approach here would include computing the inverse transform
// as a product of the inverses of the individual transforms instead of computing the
// transform and then inverting it, and / or doing matrix operations in OpenGL to
// reduce numerical discrepancies
// -----------------------------------------------------------------------------------
void object3d::connect(object3d *newParent)
{
	matrix		distCalcTemp;					// a temporary matrix used to calculate the distance to the camera

	// trying to connect parented object -- first disconnect
	if(parent) disconnect();
	
	// get the transform that will be applied when newParent is parent
	distCalcTemp.init();
	newParent->buildTransformMatrix(1, distCalcTemp);
	
	// invert it and pre-apply inverse to existing connect matrix
	matrix inv = distCalcTemp.simplisticInverse();
	connectMatrix = inv * connectMatrix;
		
	setParent(newParent);
	useConnectMatrix = true;
}




// -------------------------------------------------------------------------------------
// resetConnectMatrix()
// -------------------------------------------------------------------------------------
// resets the connectMatrix so that an object can be independent of any parents
//
// intended to be used in initScene as part of the re-initialisation of an object that may
// have had its connection status changed by animateScene during animation, so that
// the animation can be re-started correctly without quitting the program
// -------------------------------------------------------------------------------------
void object3d::resetConnectMatrix()
{
	connectMatrix.init();
	useConnectMatrix = false;
}	



int object3d::nextSerial = 0;

