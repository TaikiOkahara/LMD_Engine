#pragma once
#include "InstanceObject.h"

class CTrim : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pVertexLayout;
public:
	CTrim() {}
	~CTrim() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};