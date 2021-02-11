/*---------------------------------------
*　edge.h
* 壁上の石囲いオブジェクトのクラス
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"
#include "StaticMesh.h"

class CEdge : public CInstanceGameObject
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