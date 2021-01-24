/*---------------------------------------
*Å@fog.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "gameobject.h"
#include "collision.h"
#include "effect.h"




class CFogEffect : public CEffect
{
private:
	FOG m_Fog;

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;

	ID3D11ShaderResourceView* m_FogTexture = nullptr;
	bool m_Enable = true;
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};
