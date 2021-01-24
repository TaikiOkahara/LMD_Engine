/*---------------------------------------
*Å@wall.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"
#include "tile.h"
#include "collision.h"

class CWall : public CInstanceGameObject
{
private:
	Tile* m_pWall = nullptr;
	
	ID3D11PixelShader* m_pPixelShader = nullptr;
public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
	
};