/*---------------------------------------
*Å@demoDirectionalLight.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once

#include "effect.h"


class CDemoDirectionalLight : public CEffect
{
private:
	D3DXVECTOR4 m_DLightDir;
	D3DXVECTOR4 m_DLightPos;
	D3DXVECTOR4 m_DLightCol;

	TRANSFORM m_Transform;

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;

	bool m_Enable = true;
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};