#pragma once
#include "scene.h"

class Game : public CScene
{
private:
	bool timeStop = false;
	int  deferredType = 0;
public:
	Game(){}
	~Game(){}

	void Init();
	void UnInit();
	void Update();
	void Imgui();
};
