#pragma once

#include "Common.h"
#include "Behaviour.h"

#define STATIC 0
#define DYNAMIC 1

struct Transform {
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 rot = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);

};

class GameObject
{
public:
	GameObject();
	GameObject(glm::vec3 position);
	~GameObject();

	void InitPhysics(int type);

	Behaviour* GetBehaviour() { return m_behaviour; }
	Transform& GetTransform() { return transform; }

	void SetBehaviour(Behaviour* beh) { m_behaviour = beh; m_behaviour->texture = textureID; m_behaviour->Parent = this; }

	GLuint textureID = NULL;

	btCollisionShape* Collider = NULL;
	btRigidBody* Rigidbody = NULL;

protected:
	Transform transform;
	Behaviour* m_behaviour = nullptr;
};

