#pragma once
#include "InstanceObject.h"

class CCeiling : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;

public:
	CCeiling() {}
	~CCeiling() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};