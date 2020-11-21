#pragma once
#include "InstanceObject.h"
#include "collision.h"
#include "StaticMesh.h"

class CPillar : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;


public:
	CPillar() {}
	~CPillar() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};