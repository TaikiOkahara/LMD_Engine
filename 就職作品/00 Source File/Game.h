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
	bool m_TimeStop = false;
	int  m_DeferredType = 0;
	bool m_EnableCulling = false;
	BOOL m_GBufferRenderEnable = false;
	BOOL m_ShadowEnable = true;
	GAME_STAGE m_CurStage;

	FLOAT m_aoEnable = 1;
	FLOAT m_aoHemRedius = 0.03f;
	FLOAT m_aoZfar = 1000.0f;
	FLOAT m_aoPower = 10.0f;


public:

	void Init();
	void UnInit();
	void Update();
	void Imgui();
};
