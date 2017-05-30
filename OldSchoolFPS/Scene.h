#pragma once
#include "GameObject.h"

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<GameObject> GetObjects() { return m_sceneObjects; }

private:
	std::vector<GameObject> m_sceneObjects;
};

