#pragma once
#include "InstanceObject.h"
#include "collision.h"

class CChandelier : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;
	//Collision m_Collision;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;


	bool isEnableCollision = true;
public:
	CChandelier() {}
	~CChandelier() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};