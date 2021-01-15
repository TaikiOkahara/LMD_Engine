#pragma once
#include "InstanceObject.h"
#include "StaticMesh.h"

class CTable : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;

public:
	CTable() {}
	~CTable() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};