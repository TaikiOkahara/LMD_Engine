#pragma once
#include "InstanceObject.h"
#include "StaticMesh.h"

class CGargoyle : public CInstanceGameObject
{
private:
	//Tile* m_pMesh = nullptr;
	StaticMesh* m_pMesh = nullptr;

public:
	CGargoyle() {}
	~CGargoyle() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};