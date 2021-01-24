/*---------------------------------------
*Å@trim.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"

class CTrim : public CInstanceGameObject
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