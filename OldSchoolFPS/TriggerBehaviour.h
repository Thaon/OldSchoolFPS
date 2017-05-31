#pragma once

#include "Common.h"
#include "TimedMessage.h"
#include "Behaviour.h"

class TriggerBehaviour : public Behaviour
{
public:

	int size = 2;
	bool m_colliding = false;
	bool m_isActive = true;
	void *font = GLUT_STROKE_ROMAN;
	TimedMessage* m_message = NULL;

	virtual void Start()
	{
	}

	virtual void Draw()
	{
		if (m_colliding && m_isActive)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			glutSolidCube(size / 2);
			if (m_message == NULL)
			{
				m_message  = new TimedMessage((char*)m_messageToShow.c_str(), font, 800, 600, 2);
				m_isActive = false;
			}
		}
		else
		{
		}
	}

	virtual void Update(float deltaTime, Camera* cam)
	{
		float dis = glm::distance(cam->GetPos(), Parent->GetTransform().pos);

		if (m_message != NULL)
		{
			if (m_message->m_markForDeletion)
			{
				delete m_message;
				m_message = NULL;
			}
			else
			m_message->DrawMessage();
		}

		if (dis < size)
			m_colliding = true;
		else
			m_colliding = false;
	}
};