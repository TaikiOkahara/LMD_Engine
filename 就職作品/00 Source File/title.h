#pragma once
#include "scene.h"

class Title : public CScene
{
private:
	bool timeStop = false;
	int  deferredType = 0;
	bool frustumEnable = false;
	float ambientOcclusionPower = 0.0f;

public:

	void Init();
	void UnInit();
	void Update();
	void Imgui();
};
