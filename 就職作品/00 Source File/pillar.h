/*---------------------------------------
*Å@pillar.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"
#include "collision.h"
#include "StaticMesh.h"

class CPillar : public CInstanceGameObject
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