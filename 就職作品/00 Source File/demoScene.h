/*---------------------------------------
*　demoScene.h
*
* カリング効果を確認するシーン
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "scene.h"


class DemoScene : public CScene
{
private:
	bool m_TimeStop = false;
	
	bool m_EnableCulling = false;
	
	int m_modelCount = 0;
public:

	void Init();
	void UnInit();
	void Update();
	void Imgui();
};
