#pragma once
#include "gameobject.h"
#include "StaticMesh.h"
#include "light.h"


class CSceneLight : public CGameObject
{
public:
	CSceneLight(){}
	~CSceneLight(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};
