/*---------------------------------------
*　floor.h
*
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "gameobject.h"
#include "instanceObject.h"
#include "tile.h"



class CFloor : public CGameObject
{
private:
	Tile* m_pField = nullptr;

	/*ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;*/

public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();

	//任意の座標で地面の高さを計算する関数
	float GetHeight(D3DXVECTOR3 pos) { return m_pField->GetHeight(pos); }
};




static const HeightMask g_FloorMask[] =
{
	HeightMask{37,15,4},
	HeightMask{37,16,4},
	HeightMask{37,17,4},
	HeightMask{37,18,4},
	HeightMask{37,19,4},
	HeightMask{37,20,4},
	HeightMask{38,15,3.333},
	HeightMask{38,16,3.333},
	HeightMask{38,17,3.333},
	HeightMask{38,18,3.333},
	HeightMask{38,19,3.333},
	HeightMask{38,20,3.333},
	HeightMask{39,15,2.666},
	HeightMask{39,16,2.666},
	HeightMask{39,17,2.666},
	HeightMask{39,18,2.666},
	HeightMask{39,19,2.666},
	HeightMask{39,20,2.666},
	HeightMask{40,15,2},
	HeightMask{40,16,2},
	HeightMask{40,17,2},
	HeightMask{40,18,2},
	HeightMask{40,19,2},
	HeightMask{40,20,2},
	HeightMask{41,15,1.333},
	HeightMask{41,16,1.333},
	HeightMask{41,17,1.333},
	HeightMask{41,18,1.333},
	HeightMask{41,19,1.333},
	HeightMask{41,20,1.333},
	HeightMask{42,15,0.666},
	HeightMask{42,16,0.666},
	HeightMask{42,17,0.666},
	HeightMask{42,18,0.666},
	HeightMask{42,19,0.666},
	HeightMask{42,20,0.666},
	
	HeightMask{NULL,NULL,NULL}
};