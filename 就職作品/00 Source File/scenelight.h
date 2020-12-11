#pragma once
#include "gameobject.h"
#include "StaticMesh.h"


class CSceneLight : public CGameObject
{
private:
	DIRECTIONALLIGHT m_DirectionalLight;

	StaticMesh* m_pMesh;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};
