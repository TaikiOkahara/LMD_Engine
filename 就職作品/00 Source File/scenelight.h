#pragma once
#include "gameobject.h"
#include "StaticMesh.h"


class CSceneLight : public CGameObject
{
private:
	DIRECTIONALLIGHT m_DirectionalLight;

public:
	CSceneLight(){}
	~CSceneLight(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};
