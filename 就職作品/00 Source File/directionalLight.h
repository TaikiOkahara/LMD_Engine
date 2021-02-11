/*---------------------------------------
*Å@directionalLight.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once

#include "effect.h"


class CDirectionalLight : public CEffect
{
private:
	D3DXVECTOR4 m_Direction;
	D3DXVECTOR4 m_Position;
	D3DXVECTOR4 m_Color;

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