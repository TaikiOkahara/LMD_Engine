#pragma once
#include "StaticMesh.h"
#include "effect.h"


class CDirectionalLight : public CEffect
{
private:
	D3DXVECTOR4 m_DLightDir;
	D3DXVECTOR4 m_DLightPos;
	D3DXVECTOR4 m_DLightCol;

	TRANSFORM m_Transform;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	bool m_Enable = true;
public:
	CDirectionalLight(){}
	~CDirectionalLight(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};