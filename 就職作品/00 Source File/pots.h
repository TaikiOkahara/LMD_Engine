#pragma once
#include "InstanceObject.h"
#include "collision.h"

class CPots : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;
	//Collision m_Collision;

public:
	CPots() {}
	~CPots() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};