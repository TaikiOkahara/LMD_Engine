#pragma once
#include "scene.h"

class Game : public CScene
{
public:
	Game(){}
	~Game(){}

	void Init();
	void UnInit();
	void Update();
	void Imgui();
};
