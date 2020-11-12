#pragma once
#include "scene.h"

class Game : public CScene
{
private:
	bool timeStop = false;
public:
	Game(){}
	~Game(){}

	void Init();
	void UnInit();
	void Update();
	void Imgui();
};
