/*---------------------------------------
*Å@ceilingArch.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"

class CCeilingArch : public CInstanceGameObject
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