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
	virtual void Update() {};
	virtual void Draw() {};
	virtual void InteractedByPlayer(Camera* cam) {};
};

