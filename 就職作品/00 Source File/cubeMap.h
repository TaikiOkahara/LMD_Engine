#pragma once
#include "gameobject.h"

class CCubeMap : public CGameObject
{
private:

	ID3D11Buffer* m_pVertexBuffer = nullptr;

	ID3D11ShaderResourceView* m_pTexture_f = nullptr;
	ID3D11ShaderResourceView* m_pTexture_b = nullptr;
	ID3D11ShaderResourceView* m_pTexture_u = nullptr;
	ID3D11ShaderResourceView* m_pTexture_d = nullptr;
	ID3D11ShaderResourceView* m_pTexture_r = nullptr;
	ID3D11ShaderResourceView* m_pTexture_l = nullptr;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pVertexLayout;
public:
	CCubeMap(){}
	~CCubeMap (){}


	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};