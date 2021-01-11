#pragma once
#include "gameobject.h"
#include "tile.h"
#include "collision.h"

class CInstanceGameObject : public CGameObject
{
private:
	static ID3D11ComputeShader* m_pComputeShader;

	//構造体バッファとビュー
	ID3D11Buffer* m_pMatrixBuffer = nullptr;
	ID3D11ShaderResourceView* m_pMatrixBufferSRV = nullptr;
	//スタック操作を行うバッファとビュー
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	ID3D11UnorderedAccessView* m_pIndexBufferUAV = nullptr;
	//インデックス参照バッファとビュー
	ID3D11Buffer* m_pOutIndexBuffer = nullptr;
	ID3D11ShaderResourceView* m_pOutIndexBufferSRV = nullptr;


	ID3D11Buffer* m_pCounterBuffer = nullptr;
	ID3D11Buffer* m_pCullingBuffer = nullptr;
	
protected:
	
	std::vector<TRANSFORM> m_TransformList;
	

	UINT m_MeshCount = 0;
	UINT m_MeshMax = 0;
	//CULLING culling;

	UINT m_CullingCount;
	D3DXVECTOR4 m_CullingPos[8];

	void InitInstance();
	void UpdateInstance();
	void UninitInstance();
	void DrawInstance();

	static ID3D11VertexShader* m_pInstanceVertexShader;

public:
	/*CInstanceGameObject()
	{}*/
	~CInstanceGameObject()
	{
		for (int i = 0; i < m_TransformList.size(); i++)
		{
			std::vector<TRANSFORM>().swap(m_TransformList);
			m_TransformList.clear();
		}
	}

	//オーバーロード
	D3DXVECTOR3  GetPosition(int index) { return m_TransformList[index].position; }
	D3DXVECTOR3  GetRotation(int index) { return m_TransformList[index].rotation; }
	D3DXVECTOR3  GetScale(int index) { return m_TransformList[index].scale; }
	D3DXVECTOR3 GetForward(int index)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_TransformList[index].rotation.y,
			m_TransformList[index].rotation.x,
			m_TransformList[index].rotation.z);

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
			m_TransformList[index].rotation.y,
			m_TransformList[index].rotation.x,
			m_TransformList[index].rotation.z);

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
			m_TransformList[index].rotation.y,
			m_TransformList[index].rotation.x,
			m_TransformList[index].rotation.z);

		D3DXVECTOR3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}


	int GetMeshCount() { return m_MeshCount; }
	int GetMeshMax() { return m_MeshMax; }

	static void Load();
	static void Unload();
};
