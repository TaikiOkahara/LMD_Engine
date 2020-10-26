#pragma once
#include "Scene.h"

class Game : public Scene
{
public:
	Game(){}
	~Game(){}

	void Init();
	void UnInit();
	void Update();
	void Imgui();
};
