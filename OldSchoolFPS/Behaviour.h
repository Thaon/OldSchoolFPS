#pragma once

#include "Common.h"
#include "Camera.h"

class GameObject;

class Behaviour
{
public:
	Behaviour();
	~Behaviour();

	GameObject* Parent;

	GLuint texture = NULL;

	virtual void Start() {};
	virtual void Update(float deltaTime, Camera* cam) {};
	virtual void Draw() {};
	virtual void InteractedByPlayer(Camera* cam) {};
};

