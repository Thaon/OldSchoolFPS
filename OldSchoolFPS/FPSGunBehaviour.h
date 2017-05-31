#pragma once

#include "Common.h"
#include "Behaviour.h"

class FPSGunBehaviour
{
public:

	int size = 1;
	GLuint texture = NULL;

	virtual void Start()
	{
	}

	virtual void Draw(glm::vec3 pos)
	{
		glPushMatrix();

		glTranslatef(pos.x, pos.y, pos.z);

		glColor3f(1.0f, 1.0f, 1.0f);

		float modelview[16];
		int i, j;

		// save the current modelview matrix
		glPushMatrix();

		// get the current modelview matrix
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

		// undo all rotations
		// beware all scaling is lost as well 
		for (i = 0; i<3; i++)
			for (j = 0; j<3; j++) {
				if (i == j)
					modelview[i * 4 + j] = 1.0;
				else
					modelview[i * 4 + j] = 0.0;
			}

		// set the modelview with no rotations and scaling
		glLoadMatrixf(modelview);

		glEnable(GL_TEXTURE_2D);

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0);

		glBindTexture(GL_TEXTURE_2D, texture);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, -size, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size, -size, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, 0);
		glEnd();

		glScalef(.1f, .1f, .1f);

		glPopMatrix();

		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);

		glColor3f(1.0f, 1.0f, 1.0f);


		glPopMatrix();
	}
};