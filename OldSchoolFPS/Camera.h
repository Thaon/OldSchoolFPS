#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Common.h"
#include "FPSGunBehaviour.h"

class Camera
{
public:
	Camera() {};

	void InitCamera()
	{
		/*
		Initialises the camera much like would normally happen in a constructor.
		Sets up the camera position, the forwards and up vectors and the projection and view matrices.
		*/
		this->m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->m_up = glm::vec3(0.0f, 1.0f, 0.0f);

		Collider = new btSphereShape(1);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(m_pos.x, m_pos.y, m_pos.z)));
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		Collider->calculateLocalInertia(mass, fallInertia);

		btRigidBody::btRigidBodyConstructionInfo dynamicRigidBodyCI(mass, motionState, Collider, fallInertia);
		Rigidbody = new btRigidBody(dynamicRigidBodyCI);
		Rigidbody->setFriction(1);
		Rigidbody->setActivationState(DISABLE_DEACTIVATION);
	}

	int oldMouseX, oldMouseY;

	int mouseX, mouseY;

	//glm::vec3 tempFrontVec = m_forward;
	FPSGunBehaviour m_gun;

	//Getter/Setter for m_pos
	void SetPos(glm::vec3& newPos) { m_pos = newPos; }
	glm::vec3 GetPos() { return m_pos; }

	//Move functions
	void MoveLeft() { m_pos += glm::cross(m_up, m_forward) * 0.1f; }//Moves camera left
	void MoveRight() { m_pos -= glm::cross(m_up, m_forward) * 0.1f; }//Moves camera right

	void MoveForward() { m_pos += m_forward * 0.1f; }//Moves camera forward
	void MoveBack() { m_pos -= m_forward * 0.1f; }//Moves camera back

	void MoveUp() { m_pos += m_up * 0.1f; }//Moves camera up
	void MoveDown() { m_pos -= m_up * 0.1f; }//Moves camera down

	void Move(glm::vec3 amount)
	{
		glm::vec3 forward = glm::normalize(m_forward);
		glm::vec3 right = glm::normalize(glm::cross(m_up, m_forward));

		glm::vec3 actualDir = glm::vec3(0, 0, 0);
		actualDir += forward * amount.z;
		actualDir += right * amount.x;

		btTransform trans;
		Rigidbody->getMotionState()->getWorldTransform(trans);

		btVector3 force = btVector3(actualDir.x, Rigidbody->getLinearVelocity().getY(), actualDir.z);
		Rigidbody->setLinearVelocity(force);
		//Rigidbody->applyCentralForce(force);
	}

	void Jump(float amt)
	{
		btVector3 force = btVector3(0, amt, 0);

		Rigidbody->applyCentralForce(force);
	}

	void UpdatePhysics()
	{
		btTransform trans;
		Rigidbody->getMotionState()->getWorldTransform(trans);
		SetPos(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	}

	void Refresh()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(m_pos.x, m_pos.y, m_pos.z, m_pos.x + m_forward.x, m_pos.y + m_forward.y, m_pos.z + m_forward.z, m_up.x, m_up.y, m_up.z);
		m_gun.Draw(m_pos + m_forward);
	}

	void Pitch(float angle)//Calculate the camera pitch when mouse moves.
	{
		glm::vec3 right = glm::normalize(glm::cross(m_up, m_forward));

		m_forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(m_forward, 0.0)));
		m_up = glm::normalize(glm::cross(m_forward, right));
	}

	void RotateY(float angle)//Calculate the camera rotation when mouse moves.
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		m_forward = glm::vec3(glm::normalize(rotation * glm::vec4(m_forward, 0.0)));
		m_up = glm::vec3(glm::normalize(rotation * glm::vec4(m_up, 0.0)));
	}

	glm::vec3 Forward() { return m_forward; }

	btCollisionShape* Collider = NULL;
	btRigidBody* Rigidbody = NULL;

private:

	glm::vec3 m_cameraTarget;



	glm::vec3 m_pos;
	glm::vec3 m_forward;
	glm::vec3 m_up;

};

#endif
