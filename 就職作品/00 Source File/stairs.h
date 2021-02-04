/*---------------------------------------
*Å@stairs.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"
#include "StaticMesh.h"

class CStairs : public CInstanceGameObject
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