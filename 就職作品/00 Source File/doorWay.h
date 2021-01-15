#pragma once
#include "InstanceObject.h"
#include "StaticMesh.h"

class CDoorWay : public CInstanceGameObject
{
private:
	//Tile* m_pMesh = nullptr;
	StaticMesh* m_pMesh = nullptr;

public:
	CDoorWay() {}
	~CDoorWay() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};