/*---------------------------------------
*　projectionShadow.h
*
* プレイヤーの投影テクスチャを作成するクラス
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "gameobject.h"

class CProjShadow : public CGameObject
{
private:
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;

	ID3D11InputLayout* m_pVertexLayout = nullptr;

	bool m_Enable = true;

	ID3D11Texture2D* m_pShadowTexture = nullptr;
	ID3D11ShaderResourceView* m_pShadowShaderResourceView = nullptr;
public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};