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
	//LIGHT m_Light;

	//D_LIGHT GetDLight() { return m_Light.GetDirectionalData(); }
	//P_LIGHT GetPLight() { return m_Light.GetPointData(); }
private:
	//static D_LIGHT g_DLight;

	
	D3DXVECTOR4 m_DL_Intensity;


	D3DXVECTOR3 m_PL;
	float m_PL_Intensity = 1.0f;
	float m_PL_Range = 1.0f;
	float m_PL_Rate = 4.0f;

};
