/*�uCAMERA.h�v=============================================
�@�@����ҁF������N�@	(-"-)
=============================================================*/
#pragma once
#include "gameobject.h"

//
//
//
class CCamera : public CGameObject
{
public:
	CCamera(){}	//�@�R���X�g���N�^	
	~CCamera(){}	//�@�f�X�g���N�^


	void Init();//�@������
	void Update();//�@�X�V
	void Uninit();
	void Draw();
	void Imgui();


private:

	D3DXVECTOR3 m_Target;
	D3DXVECTOR3 m_Distance;

	D3DXVECTOR3 m_OffsetPosition;

	float m_Angle;
	float m_Aspect;
	float m_Near;
	float m_Far;

	void GetDistance(D3DXVECTOR3 RayStart, D3DXVECTOR3 RayEnd);
};

