/*---------------------------------------
*Å@moss.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "gameobject.h"
#include "instanceObject.h"
#include "tile.h"



class CMoss : public CGameObject
{
private:
	Tile* m_pField = nullptr;

public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};