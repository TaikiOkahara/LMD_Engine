#pragma once
#include "InstanceObject.h"
#include "collision.h"

class CChandelier : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;
	//Collision m_Collision;

public:
	CChandelier() {}
	~CChandelier() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};