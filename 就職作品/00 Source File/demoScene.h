/*---------------------------------------
*�@demoScene.h
*
* �J�����O���ʂ��m�F����V�[��
*@author�FOkahara Taiki
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
