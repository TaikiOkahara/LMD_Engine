#pragma once
#include "director.h"

class Collision
{
private:
	

	D3DXVECTOR3* m_pVertex;//—§•û‘Ì‚È‚Ì‚Å8’¸“_
	D3DXVECTOR3* m_pNormal;
	
	float m_fXsize, m_fYsize, m_fZsize;
	D3DXVECTOR3 m_vCenter;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11VertexShader* m_pInstanceVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pVertexLayout = nullptr;
public:

	void Init(D3DXVECTOR3 size, D3DXVECTOR3 center);
	void Uninit();
	void Draw();
	void Update();
	void DrawInstance(const unsigned int instanceCount);


	D3DXVECTOR3* GetVertex(){return m_pVertex;}
	D3DXVECTOR3* GetNormal(){return m_pNormal;}
	D3DXVECTOR3  GetPosition() { return m_vCenter; }

	float  GetXsize() { return m_fXsize; }
	float  GetYsize() { return m_fYsize; }
	float  GetZsize() { return m_fZsize; }
};
