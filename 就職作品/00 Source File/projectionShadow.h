#pragma once
#include "gameobject.h"

class CProjShadow : public CGameObject
{
private:
	
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pVertexLayout;

	bool m_Enable = true;
public:
	CProjShadow() {}
	~CProjShadow() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};