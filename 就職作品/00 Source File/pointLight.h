/*---------------------------------------
*　pointlight.h
*
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"
#include "StaticMesh.h"

class CPointLight : public CInstanceGameObject
{
private:
	POINTLIGHT m_PointLight[LIGHT_MAX];//ポイントライト追加情報
	StaticMesh* m_pMesh = nullptr;

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;

	bool m_EnablePlayerPointLight = true;
	bool m_FlashEnable = false;//ロウソクの揺らめき有無
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();

	POINTLIGHT GetPointLight(int index) { return m_PointLight[index]; }
};
