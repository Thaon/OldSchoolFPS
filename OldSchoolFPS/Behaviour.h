#pragma once

#include "Common.h"

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
};

