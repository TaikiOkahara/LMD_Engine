#pragma once
#include "InstanceObject.h"
#include "collision.h"

class CPots : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;
	//Collision m_Collision;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;


public:
	CPots() {}
	~CPots() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};