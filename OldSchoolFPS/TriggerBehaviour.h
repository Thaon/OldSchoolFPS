#pragma once

#include "Common.h"
#include "Behaviour.h"

class TriggerBehaviour : public Behaviour
{
public:

	int size = 2;
	bool m_colliding = false;

	virtual void Start()
	{
	}

	virtual void Draw()
	{
		if (m_colliding)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			glutSolidCube(size / 2);
		}
		else
		{
		}
	}

	virtual void Update(float deltaTime, Camera* cam)
	{
		float dis = glm::distance(cam->GetPos(), Parent->GetTransform().pos);

		if (dis < size)
			m_colliding = true;
		else
			m_colliding = false;
	}
};