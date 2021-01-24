/*---------------------------------------
*　gargoyle.h
* ガーゴイルの銅像オブジェクトクラス
*@author：Okahara Taiki
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