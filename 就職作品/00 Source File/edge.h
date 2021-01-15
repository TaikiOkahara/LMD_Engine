#pragma once
#include "InstanceObject.h"
#include "StaticMesh.h"

class CEdge : public CInstanceGameObject
{
private:
	//Tile* m_pMesh = nullptr;
	StaticMesh* m_pMesh = nullptr;

public:
	CEdge() {}
	~CEdge() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};