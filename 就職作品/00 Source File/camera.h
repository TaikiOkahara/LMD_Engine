/*「CAMERA.h」=============================================
　　製作者：岡原大起　	(-"-)
=============================================================*/
#pragma once
#include "gameobject.h"
#include "InstanceObject.h"
//
//
//
class CCamera : public CGameObject
{
public:
	CCamera(){}
	~CCamera(){}


	void Init();
	void Update();
	void Uninit();
	void Draw();
	void Imgui();


	D3DXVECTOR4 m_CullingWPos[4];
private:
	
	D3DXVECTOR3 m_Target;
	D3DXVECTOR3 m_Distance;
	D3DXVECTOR3 m_OffsetPosition;

	/*D3DXMATRIX m_ProjMatrix;
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_OldViewMatrix;
	D3DXMATRIX m_OldProjMatrix;*/
	VIEWMATRIX m_ViewMatrix;
	PROJMATRIX m_ProjMatrix;


	float m_Angle;
	float m_Aspect;
	float m_Near;
	float m_Far;

	bool m_CameraControl = true;
	
	D3DXVECTOR3 CameraRayIntersect(CInstanceGameObject* object);
};

