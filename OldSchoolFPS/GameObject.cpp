#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::GameObject(glm::vec3 position)
{
	transform.pos = position;
}


GameObject::~GameObject()
{
}

void GameObject::InitPhysics(int type)
{
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(transform.pos.x, transform.pos.y, transform.pos.z)));

	if (type == STATIC)
	{

		btRigidBody::btRigidBodyConstructionInfo staticRigidBodyCI(0, motionState, Collider, btVector3(0, 0, 0));
		Rigidbody = new btRigidBody(staticRigidBodyCI);
	}
	else
	{
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		Collider->calculateLocalInertia(mass, fallInertia);

		btRigidBody::btRigidBodyConstructionInfo dynamicRigidBodyCI(mass, motionState, Collider, fallInertia);
		Rigidbody = new btRigidBody(dynamicRigidBodyCI);
		Rigidbody->setActivationState(DISABLE_DEACTIVATION);
	}
}
