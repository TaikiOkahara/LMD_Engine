/*---------------------------------------
*　game.h
*
* メインとなるシーン
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "scene.h"


enum GAME_STAGE
{
	DUNGEON = 0,
	OUTDOOR
};

class Game : public CScene
{
private:
	bool timeStop = false;
	int  deferredType = 0;
	bool frustumEnable = false;
	float ambientOcclusionPower = 0.0f;
	BOOL gBufferRenderEnable = false;
	GAME_STAGE m_CurStage;
public:

	void Init();
	void UnInit();
	void Update();
	void Imgui();
};
