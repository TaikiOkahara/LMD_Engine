/*---------------------------------------
*�@cullingDemo.h
*
* �J�����O���ʂ��m�F����V�[��
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "scene.h"


class CullingDemo : public CScene
{
private:
	bool timeStop = false;
	//int  deferredType = 0;
	bool frustumEnable = false;
	//float ambientOcclusionPower = 0.0f;
	//BOOL gBufferRenderEnable = false;
	//GAME_STAGE m_CurStage;
public:

	void Init();
	void UnInit();
	void Update();
	void Imgui();
};
