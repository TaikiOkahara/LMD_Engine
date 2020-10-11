#pragma once
#include "gameobject.h"
#include "tile.h"
#include "StaticMesh.h"

class CInstanceGameObject : public CGameObject
{
protected:

	ID3D11ComputeShader* m_pComputeShader = nullptr;

	UINT m_MeshCount = 0;
	std::vector<D3DXMATRIX> m_MatrixList;

	//�\���̃o�b�t�@�ƃr���[
	ID3D11Buffer* m_pMatrixBuffer = nullptr;
	ID3D11ShaderResourceView* m_pMatrixBufferSRV = nullptr;
	//�X�^�b�N������s���o�b�t�@�ƃr���[
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	ID3D11UnorderedAccessView* m_pIndexBufferUAV = nullptr;

	ID3D11Buffer* m_pCounterBuffer = nullptr;

	void InitInstance();
	void UpdateInstance();
	void UninitInstance();
	void DrawInstance();
public:
	CInstanceGameObject(){}
	~CInstanceGameObject(){}
};
