#pragma once
#include "InstanceObject.h"

class CTrim : public CInstanceGameObject
{
private:
	StaticMesh* m_pMesh = nullptr;

public:
	CTrim() {}
	~CTrim() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};