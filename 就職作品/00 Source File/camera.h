/*「CAMERA.h」=============================================
　　製作者：岡原大起　	(-"-)
=============================================================*/
#pragma once
#include "gameobject.h"

//
//
//
class CCamera : public CGameObject
{
public:
	CCamera(){}	//　コンストラクタ	
	~CCamera(){}	//　デストラクタ


	void Init();//　初期化
	void Update();//　更新
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

