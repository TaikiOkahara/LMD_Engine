/*---------------------------------------
*Å@chandelier.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"
#include "collision.h"

class CChandelier : public CInstanceGameObject
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