#pragma once
#include "InstanceObject.h"
#include "StaticMesh.h"

class CStage : public CInstanceGameObject
{
private:
	//Tile* m_pMesh = nullptr;
	StaticMesh* m_pMesh = nullptr;

public:
	CStage() {}
	~CStage() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};