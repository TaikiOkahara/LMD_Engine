/*---------------------------------------
*　demoCamera.h
*
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "gameobject.h"
#include "instanceObject.h"

class CDemoCamera : public CGameObject
{
private:
	
	D3DXVECTOR3 m_Target;//注視点
	D3DXVECTOR3 m_Distance;
	D3DXVECTOR3 m_OffsetPosition;

	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjMatrix;

	
	float m_Angle;
	float m_Aspect;
	float m_Near;
	float m_Far;

	bool m_CameraControl = true;
	
	//仮想カメラ情報

	TRANSFORM m_virtualTransform;

	D3DXVECTOR3 m_virtualCameraTarget;
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	D3DXVECTOR3 GetVirtualCameraForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_virtualTransform.rotation.y,
			m_virtualTransform.rotation.x,
			m_virtualTransform.rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
public:

	void Init();
	void Update();
	void Uninit();
	void Draw();
	void Imgui();


	D3DXVECTOR4 m_CullingWPos[4];
	D3DXVECTOR3 GetTarget() {return m_Target;}
};

