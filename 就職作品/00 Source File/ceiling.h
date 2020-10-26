#pragma once
#include "InstanceObject.h"

class CCeiling : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

public:
	CCeiling() {}
	~CCeiling() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};