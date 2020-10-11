#pragma once
#include "InstanceObject.h"

class CStage : public CInstanceGameObject
{
private:
	//Tile* m_pMesh = nullptr;
	StaticMesh* m_pMesh = nullptr;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pVertexLayout;
public:
	CStage() {}
	~CStage() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};