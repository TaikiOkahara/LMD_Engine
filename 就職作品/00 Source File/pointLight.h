#pragma once
#include "gameobject.h"

class CPointLight : public CGameObject
{
private:

	//std::vector<D3DXVECTOR4> m_PointList;
	POINTLIGHT m_PointLight;
	StaticMesh* m_pMesh = nullptr;

	//int m_PointCount;

	//構造体バッファとビュー
	ID3D11Buffer* m_pPointLightBuffer = nullptr;
	ID3D11ShaderResourceView* m_pPointLightBufferSRV = nullptr;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pVertexLayout;


public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};
