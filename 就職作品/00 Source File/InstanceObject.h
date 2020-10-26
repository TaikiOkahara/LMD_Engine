#pragma once
#include "gameobject.h"
#include "tile.h"
#include "collision.h"
#include "StaticMesh.h"

class CInstanceGameObject : public CGameObject
{
private:

	//構造体バッファとビュー
	ID3D11Buffer* m_pMatrixBuffer = nullptr;
	ID3D11ShaderResourceView* m_pMatrixBufferSRV = nullptr;
	//スタック操作を行うバッファとビュー
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	ID3D11UnorderedAccessView* m_pIndexBufferUAV = nullptr;

	ID3D11Buffer* m_pCounterBuffer = nullptr;
	ID3D11ComputeShader* m_pComputeShader = nullptr;
	
protected:
	struct VECTOR
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 rotation;
		D3DXVECTOR3 scale;
	};
	
	std::vector<VECTOR> m_Vector;
	

	UINT m_MeshCount = 0;


	void InitInstance();
	void UpdateInstance();
	void UninitInstance();
	void DrawInstance();

public:
	CInstanceGameObject()
	{}
	~CInstanceGameObject(){}

	//オーバーロード
	D3DXVECTOR3  GetPosition(int index) { return m_Vector[index].position; }
	D3DXVECTOR3  GetRotation(int index) { return m_Vector[index].rotation; }
	D3DXVECTOR3  GetScale(int index) { return m_Vector[index].scale; }
	D3DXVECTOR3 GetForward(int index)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Vector[index].rotation.y,
			m_Vector[index].rotation.x,
			m_Vector[index].rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	D3DXVECTOR3 GetRight(int index)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Vector[index].rotation.y,
			m_Vector[index].rotation.x,
			m_Vector[index].rotation.z);

		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}
	D3DXVECTOR3 GetUp(int index)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Vector[index].rotation.y,
			m_Vector[index].rotation.x,
			m_Vector[index].rotation.z);

		D3DXVECTOR3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}


	int GetMeshCount() { return m_MeshCount; }
};
