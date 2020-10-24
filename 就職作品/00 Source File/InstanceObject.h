#pragma once
#include "gameobject.h"
#include "tile.h"
#include "collision.h"
#include "StaticMesh.h"

class CInstanceGameObject : public CGameObject
{
private:
	//std::vector<D3DXMATRIX> m_MatrixList;
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
	/*std::vector<D3DXVECTOR3> m_vecPosition;
	std::vector<D3DXVECTOR3> m_vecRotation;
	std::vector<D3DXVECTOR3> m_vecScale;*/
	std::vector<VECTOR> m_Vector;
	Collision m_Collision;

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
	
	Collision* GetCollision() { return &m_Collision; }

	int GetMeshCount() { return m_MeshCount; }
};
