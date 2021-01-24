/*---------------------------------------
*　gameobject.h
*
* 画面に表示される全てのオブジェクト箱のクラスを継承(camera、pointlightを含む)
* 現在のシーンを取得してからお互いのオブジェクトクラスを取得する(GatGameObject関数)
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "director.h"
#include "renderer.h"
#include "collision.h"

class CGameObject
{
protected:

	TRANSFORM		m_Transform;
	D3DXMATRIX		m_WorldMatrix;

	D3DXQUATERNION  m_Quaternion;
	bool			m_Destroy = false;
	bool			m_EnableCollision = false;

	Collision		m_Collision;
	
	static ID3D11VertexShader* m_pCommonVertexShader;
	static ID3D11PixelShader* m_pCommonPixelShader;
	static ID3D11InputLayout* m_pCommonVertexLayout;

	//マトリクス設定を省略する関数
	void SetWorldMatrix();
public:
	CGameObject() {}
	virtual ~CGameObject() {}


	static void Load();
	static void Unload();
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Imgui(){}

	D3DXVECTOR3 GetPosition() { return m_Transform.position; }
	D3DXVECTOR3 GetRotation() { return m_Transform.rotation; }
	D3DXVECTOR3 GetScale()	  { return m_Transform.scale; }
	Collision* GetCollision() { return &m_Collision; }

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
};

