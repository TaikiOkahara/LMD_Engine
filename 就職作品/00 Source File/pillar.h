#pragma once
#include "InstanceObject.h"
#include "collision.h"
#include "StaticMesh.h"

class CPillar : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;

public:
	CPillar() {}
	~CPillar() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};