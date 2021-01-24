#pragma once
#include "director.h"

class Collision
{
private:
	

	D3DXVECTOR3* m_pVertex;//óßï˚ëÃÇ»ÇÃÇ≈8í∏ì_
	D3DXVECTOR3* m_pNormal;
	
	float m_fXsize, m_fYsize, m_fZsize;
	D3DXVECTOR3 m_vCenter;//íÜêSì_

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	static ID3D11VertexShader* m_pVertexShader;
	static ID3D11VertexShader* m_pInstanceVertexShader;
	static ID3D11PixelShader* m_pPixelShader;
	static ID3D11InputLayout* m_pVertexLayout;
public:
	~Collision()
	{
		delete[] m_pVertex;
		delete[] m_pNormal;

		SAFE_RELEASE(m_pVertexBuffer);
	}
	static void Load();
	static void Unload();


	void Set(D3DXVECTOR3 size, D3DXVECTOR3 center);
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
