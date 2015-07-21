// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// object3d.h
//
// This file declares a class for generic 3D objects


#ifndef _object3d_H
#define _object3d_H




// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "constants.h"
#include "vectr.h"
#include "vertex.h"
#include "matrix.h"
#include "face.h"
#include "texture.h"
#include "shaderprogram.h"





// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class object3d
{
// =================================================
// Data Items
// =================================================
	protected:
	
		// -------------------------------------------------
		// face and vertex data
		// -------------------------------------------------
		unsigned int		vertexCount;				// the number of vertices
		unsigned int		faceCount;					// the number of faces
		unsigned int		polygonCount;				// the number of onscreen polygons

		vector<vertex>		vertices;					// an array to store the vertex data
		vector<face>		faces;						// an array to store the face data


		// -------------------------------------------------
		// position, orientation, etc
		// -------------------------------------------------
		vectr		rotation;							// axis rotations to orient the object (x, y, z)
		vectr		rotationOrder;						// the order that the rotations are applied (stored as a vector)
														// for each value, 0.0=x, 1.0=y, 2.0=z
		vectr		offset;                             // this is pre-rotation translation
		vectr		position;                           // this is post-rotation translation
		vectr		deformation;						// scale values to specify deformation (pre-rotation), not passed to children
		float		scale;								// uniform scale (pre-rotation), passed from parents to children
		
		matrix		globalTransform;					// a matrix that represents the transform required to put the object in the correct location in world space
		float		distanceToCamera;					// the distance from this object's origin to the camera's origin
	

		// -------------------------------------------------
		// visual attributes
		// -------------------------------------------------
		float		theColour[4];						// the material colour of the object (RGBA format)
		float		theSpecularColour[4];				// the specular colour of the object (RGBA format)
		float		theEmissiveColour[4];				// the emissive colour of the object (RGBA format)
		float		theShininess;						// the shininess of the material (int, 0-128)
		
		bool		diffuseShading;						// true = per face normals
		bool		invisible;							// true = don't draw
		bool		objectOpacity;						// true if the shape is transparent (set automatically)


		// -------------------------------------------------
		// textures & shaders
		// -------------------------------------------------
		bool		autoBindToTexture;					// if TRUE, draw() will bind to the texture before calling privateDraw()
		texture		*theTexture;						// points to this object's primary texture (if there is one)
		float		textureScaleX;						// x-texture scale
		float		textureScaleY;						// y-texture scale
		texture		*theTexture2;						// points to this object's secondary texture (if there is one)
		float		texture2ScaleX;
		float		texture2ScaleY;
	
		shaderprogram	*theShader;						// experimental alternative to textures
		

		// -------------------------------------------------
		// relationships
		// -------------------------------------------------
		object3d	*parent;							// points to the parent object of this object
		float		parentChangeTime;					// set when a parent has changed (useful for parentage debugging)


		// -------------------------------------------------
		// miscellaneous
		// -------------------------------------------------
		GLuint		listName;							// the OpenGL display list that holds this shape
														// (for highly optimised drawing)

		string		objectName;							// give objects names to make the easier to debug/identify
		int			objectSerial;						// each object is created with a unique serial number
		static int	nextSerial;

		
		// --------------------------------------------------
		// connectedness
		// --------------------------------------------------
		bool		useConnectMatrix;					// interpose stored matrix?
		matrix		connectMatrix;						// stored matrix to interpose, in appropriate format for own ops






// =================================================
// Member Functions
// =================================================

	public:
		// -------------------------------------------------
		// basic methods
		// -------------------------------------------------
						object3d();								// default constructor
		virtual			~object3d();							// destructor


		// -------------------------------------------------
		// face and vertex data
		// -------------------------------------------------
		unsigned int	getVertexCount()						// return the number of vertices in the object
						{ return vertexCount; }
		unsigned int	getFaceCount()							// return the number of faces in the object
						{ return faceCount; }

		void			calculateNormals();						// used for shading
		void			distanceSortFaces();					// sort the faces based on distance from the camera

		void			makeInsideFaces(void);					// create "inside-out" versions of all faces (for translucency)


		// -------------------------------------------------
		// position, orientation, etc
		// -------------------------------------------------
		void			setPosition(float x, float y, float z);                 // get/set location in space
		void			setPosition(vectr &loc);                                // (applied after rotation)
		void			getPosition(float &x, float &y, float &z);
		vectr			getPosition(void);

		void			setOffset(float x, float y, float z);                   // get/set geometry offset from origin
		void			setOffset(vectr &loc);                                  // (applied before rotation)
		void			getOffset(float &x, float &y, float &z);
		vectr			getOffset(void);

		void			setRotation(char axis1 = 'x', float angle1 = 0.0, char axis2 = '\0', float angle2 = 0.0, char axis3 = '\0', float angle3 = 0.0);
	
		void			setScale(float s);                                      // set the object's scale

		void			setDeformation(float x, float y, float z);              // non-uniform scaling
		
		void			calculateDistanceToCamera();							// calculate and store the distance from this
                                                                                // object's origin to the camera
		float			getDistanceToCamera()	{ return distanceToCamera; }	// return the distance to the camera
		

		// -------------------------------------------------
		// visual attributes
		// -------------------------------------------------
		void			setColour(float r, float g, float b);					// set the material colour of the object
		void			setColour(float r, float g, float b, float a);			// set the material colour & transparency
		void			getColour(float &r, float &g, float &b, float &a);		// read back the material colour
		void			setColour(float colour[]);

		void			setSpecularColour(float r, float g, float b, float a);	// set the specular colour of the object
		void			setSpecularColour(float colour[]);

		void			setEmissiveColour(float r, float g, float b, float a);	// set the emissive colour of the object
		void			setEmissiveColour(float colour[]);


		void			setShininess(float s)	{ theShininess = s; }
		float			getShininess()			{ return theShininess; }

		virtual void	hide(void)				{ invisible = true; }
		virtual void	show(void)				{ invisible = false; }
		
		bool			isInvisible(void)		{ return invisible; }
		bool			isTranparent(void)		{ return objectOpacity; }
		void			makeTransparent(void)	{ objectOpacity = kTransparentMode; }
		void			makeOpaque(void)		{ objectOpacity = kOpaqueMode; }
		
		void			useDiffuseShading(void)	{ diffuseShading = true; }
		void			useSmoothShading(void)	{ diffuseShading = false; }
					

		// -------------------------------------------------
		// textures
		// -------------------------------------------------
		void			setTexture(texture *t)						// associate a texture with an object
						{ theTexture = t; }
		void			setTexture2(texture *t)
						{ theTexture2 = t; }
		
		void			setTexture(texture *t, float x, float y)	// ditto with scale
						{ theTexture = t; textureScaleX = x, textureScaleY = y; }					
		void			setTexture2(texture *t, float x, float y)
						{ theTexture2 = t; texture2ScaleX = x, texture2ScaleY = y; }

		void			setShader(shaderprogram *s)
						{ theShader = s; }


		// -------------------------------------------------
		// relationships
		// -------------------------------------------------
		void			setParent(object3d *theParent);			// set an object's parent
        void            attachToParentAt(object3d *theParent, float x, float y, float z);
			
		object3d		*getParent(void)						// return an object's parent
						{ return parent; }

		void			drawParentLink();
		


		// -------------------------------------------------
		// drawing
		// -------------------------------------------------
		void			draw(bool drawMode);						// draw the object (don't override)	
		virtual void	privateDraw(bool drawMode);					// really draw the object (override in derived classes if necessary)




		// -------------------------------------------------
		// building matrices
		// -------------------------------------------------
		void			buildModelViewMatrix(int depth);	// builds the OpenGL modelview matrix to put this object
														// into world space (used by drawing functions)
		void			buildTransformMatrix(int depth, matrix &finalMatrix);	
														// builds a local modelview matrix (essentially the same
														// as the OpenGL version, but used for calculating distance
														// to camera)

		// -------------------------------------------------
		// naming/serial number
		// -------------------------------------------------
		void			setName(string theName);
				
		string			name(void)
						{ return objectName; }
		
		static void		initNextSerial(void)
						{ nextSerial = 0; }

		int				getSerial(void)
						{ return objectSerial; }


		// -------------------------------------------------
		// connectedness
		// -------------------------------------------------
		void			connect(object3d *newParent);				// connect to parent
		void			disconnect();								// disconnect from parent
		void			resetConnectMatrix();						// reset connect matrix
		
};



#endif //  _object3d_H

