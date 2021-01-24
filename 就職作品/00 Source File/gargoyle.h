/*---------------------------------------
*�@gargoyle.h
* �K�[�S�C���̓����I�u�W�F�N�g�N���X
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"
#include "StaticMesh.h"

class CGargoyle : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};