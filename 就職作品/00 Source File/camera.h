/*---------------------------------------
*Å@camera.h
*
*@authorÅFOkahara Taiki
----------------------------------------*/
#pragma once
#include "gameobject.h"
#include "instanceObject.h"

class CCamera : public CGameObject
{
private:
	
	D3DXVECTOR3 m_Target;//íçéãì_
	D3DXVECTOR3 m_Distance;
	D3DXVECTOR3 m_OffsetPosition;

	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjMatrix;

	
	float m_Angle;
	float m_Aspect;
	float m_Near;
	float m_Far;

	bool m_CameraControl = true;
	
	D3DXVECTOR3 CameraRayIntersect(CInstanceGameObject* object);
public:

	void Init();
	void Update();
	void Uninit();
	void Draw();
	void Imgui();


	D3DXVECTOR4 m_CullingWPos[4];
	D3DXVECTOR3 GetTarget() {return m_Target;}
};

