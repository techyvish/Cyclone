//
//  Geometry.cpp
//  PhysicsEngine
//
//  Created by Vishal Patel on 9/06/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef  __GEOMETRY_H__
#define  __GEOMETRY_H__

#include "Geometry.h"
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#define PI_ 3.14159265358979323846

//-------------------------------------------------------------------------------------------------------------
static void PlotSpherePoints(GLfloat radius, GLint stacks, GLint slices, GLfloat* v, GLfloat* n)
{
    
	GLint i, j; 
	GLfloat slicestep, stackstep;
    
	stackstep = ((GLfloat)PI_) / stacks;
	slicestep = 2.0f * ((GLfloat)PI_) / slices;
    
	for (i = 0; i < stacks; ++i)		
	{
		GLfloat a = i * stackstep;
		GLfloat b = a + stackstep;
        
		GLfloat s0 =  (GLfloat)sin(a);
		GLfloat s1 =  (GLfloat)sin(b);
        
		GLfloat c0 =  (GLfloat)cos(a);
		GLfloat c1 =  (GLfloat)cos(b);
        
		for (j = 0; j <= slices; ++j)		
		{
			GLfloat c = j * slicestep;
			GLfloat x = (GLfloat)cos(c);
			GLfloat y = (GLfloat)sin(c);
            
			*n = x * s0;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = y * s0;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = c0;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = x * s1;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = y * s1;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = c1;
			*v = *n * radius;
            
			n++;
			v++;
            
		}
	}
}


static void glutSolidSphere(GLfloat radius, GLint slices, GLint stacks) 
{
	GLint i, triangles; 
	static GLfloat* v, *n;
	static GLfloat parms[3];
    
	if (v) 
	{
		if (parms[0] != radius || parms[1] != slices || parms[2] != stacks) 
		{
			free(v); 
			free(n);
            
			n = v = 0;
            
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, 0);
		}
	}
    
	if (!v) 
	{
		parms[0] = radius; 
		parms[1] = (GLfloat)slices; 
		parms[2] = (GLfloat)stacks;
        
		v = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof *v);
		n = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof *n);
        
		PlotSpherePoints(radius, stacks, slices, v, n);
        
	}
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glNormalPointer(GL_FLOAT, 0, n);
    
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
    
	triangles = (slices + 1) * 2;
    
	for(i = 0; i < stacks; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, i * triangles, triangles);
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
    
}




static void glutWireSphere(GLfloat radius, GLint slices, GLint stacks) 
{
	GLint i, j, f; 
	static GLfloat* v, *n;
	static GLfloat parms[3];
    
	if (v) 
	{
		if (parms[0] != radius || parms[1] != slices || parms[2] != stacks) 
		{
			free(v); 
			free(n);
            
			n = v = 0;
            
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, 0);
		}
	}
    
	if (!v) 
	{
		parms[0] = radius; 
		parms[1] = (GLfloat)slices; 
		parms[2] = (GLfloat)stacks;
        
		v = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof *v);
		n = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof *n);
        
		PlotSpherePoints(radius, stacks, slices, v, n);
        
	}
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glNormalPointer(GL_FLOAT, 0, n);
    
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
    
	for(i = 0; i < stacks; ++i)
	{
		f = i * (slices + 1);
        
		for (j = 0; j <= slices; ++j)
			glDrawArrays(GL_LINE_LOOP, (f + j)*2, 3);
	}
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
    
}



//-------------------------------------------------------------------------------------------------------------
static GLfloat boxvec[6][3] =
{
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, -1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0}
};

static GLushort boxndex [12][3] = 
{
	{0, 1, 2},
	{0, 2, 3},
	{3, 2, 6},
	{3, 6, 7},
	{6, 4, 7},
	{6, 5, 4},
	{4, 5, 1},
	{4, 1, 0},
	{2, 1, 5},
	{2, 5, 6},
	{3, 7, 4},
	{3, 4, 0}
};

static GLushort wireboxndex[6][4] = 
{
	{0, 1, 2, 3},
	{3, 2, 6, 7},
	{7, 6, 5, 4},
	{4, 5, 1, 0},
	{5, 6, 2, 1},
	{7, 4, 0, 3}
};


static void		   //x						 y						 z
glutSolidBox(GLfloat Width, GLfloat Depth, GLfloat Height)
{
	int i;
	GLfloat v[8][3];
    
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = - Width/ 2.0f;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = Width / 2.0f;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -Depth / 2.0f;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = Depth / 2.0f;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -Height / 2.0f;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = Height / 2.0f;
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glEnableClientState (GL_VERTEX_ARRAY);
    
	for (i = 0; i < 6; i++)
	{
		glNormal3f(boxvec[i][0], boxvec[i][1], boxvec[i][2]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, boxndex[i*2]);
	}
    
	glDisableClientState (GL_VERTEX_ARRAY);
}


static void       //x						 y						 z
glutWireBox(GLfloat Width, GLfloat Depth, GLfloat Height)
{
	GLfloat v[8][3];
	int i;
    
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = - Width/ 2.0f;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = Width / 2.0f;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -Depth / 2.0f;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = Depth / 2.0f;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -Height / 2.0f;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = Height / 2.0f;
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glEnableClientState (GL_VERTEX_ARRAY);
    
	for ( i = 0; i < 6; i++)
	{
		glNormal3f(boxvec[i][0], boxvec[i][1], boxvec[i][2]);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, wireboxndex[i]);
	}
	glDisableClientState (GL_VERTEX_ARRAY);
}

#endif