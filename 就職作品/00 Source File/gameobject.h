#pragma once
//#include "BASE.h"
#include "director.h"
#include "renderer.h"
#include "collision.h"

class CGameObject
{
public:
	CGameObject() {}
	virtual ~CGameObject() {}


	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Imgui(){}

	D3DXVECTOR3 GetPosition() { return m_Transform.position; }
	D3DXVECTOR3 GetRotation() { return m_Transform.rotation; }
	D3DXVECTOR3 GetScale()	  { return m_Transform.scale; }

	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Transform.rotation.y,
			m_Transform.rotation.x,
			m_Transform.rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Transform.rotation.y,
			m_Transform.rotation.x,
			m_Transform.rotation.z);

		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}
	D3DXVECTOR3 GetUp()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Transform.rotation.y,
			m_Transform.rotation.x,
			m_Transform.rotation.z);

		D3DXVECTOR3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}
	//マトリクス設定を省略する関数
	void SetWorldMatrix()
	{
		//　マトリクス設定
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_Transform.scale.x, m_Transform.scale.y, m_Transform.scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y, m_Transform.rotation.x, m_Transform.rotation.z);
		D3DXMatrixTranslation(&trans, m_Transform.position.x, m_Transform.position.y, m_Transform.position.z);
		world = scale * rot * trans;
		RENDERER::SetWorldMatrix(world);
	}
	void SetDestroy()
	{
		m_Destroy = true;
	}
	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();

			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	Collision* GetCollision() { return &m_Collision; }
protected:
	//D3DXVECTOR3		m_Position = D3DXVECTOR3(0,0,0);
	//D3DXVECTOR3		m_Rotation = D3DXVECTOR3(0, 0, 0);//Pitch,Yaw,Roll
	//D3DXVECTOR3		m_Scale = D3DXVECTOR3(1, 1, 1);
	TRANSFORM m_Transform;


	D3DXQUATERNION  m_Quaternion;
	bool			m_Destroy = false;
	D3DXMATRIX		m_WorldMatrix;
	Collision m_Collision;
};

