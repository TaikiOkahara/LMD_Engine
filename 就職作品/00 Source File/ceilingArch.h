#pragma once
#include "InstanceObject.h"

class CCeilingArch : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;

public:
	CCeilingArch() {}
	~CCeilingArch() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};