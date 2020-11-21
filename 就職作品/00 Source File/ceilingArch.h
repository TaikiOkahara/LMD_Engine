#pragma once
#include "InstanceObject.h"

class CCeilingArch : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

public:
	CCeilingArch() {}
	~CCeilingArch() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};