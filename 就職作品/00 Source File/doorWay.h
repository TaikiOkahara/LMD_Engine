/*---------------------------------------
*Å@doorWay.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"
#include "StaticMesh.h"

class CDoorWay : public CInstanceGameObject
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