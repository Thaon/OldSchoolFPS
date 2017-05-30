#pragma once

#include "Common.h"
#include "Behaviour.h"

class CubeBehaviour : public Behaviour
{
public:

	int size = 2;

	virtual void Start()
	{
		Parent->Collider = new btBoxShape(btVector3(2,2,2));
		Parent->InitPhysics(STATIC);
	}

	virtual void Draw()
	{
		if (texture != NULL)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
			DrawCubeTextured();
			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			glutSolidCube(size);
		}

		glColor3f(1.0f, 1.0f, 1.0f);
	}

	void DrawCubeTextured()
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		// draw a cube (6 quadrilaterals)
		glBegin(GL_QUADS);				// start drawing the cube.

										// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);	// Top Left Of The Texture and Quad

																	// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);	// Bottom Left Of The Texture and Quad

																	// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);	// Top Right Of The Texture and Quad

																	// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);	// Bottom Right Of The Texture and Quad

																	// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);	// Bottom Left Of The Texture and Quad

																	// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);	// Top Left Of The Texture and Quad

		glEnd();					// Done Drawing The Cube

	}
};