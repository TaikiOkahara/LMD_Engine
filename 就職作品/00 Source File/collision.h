#pragma once
#include "director.h"

class Collision
{
private:
	struct LineVertex
	{
		D3DXVECTOR3 pos;
	};

	D3DXVECTOR3* m_pVertex;//—§•û‘Ì‚È‚Ì‚Å8’¸“_
	D3DXVECTOR3* m_pNormal;
	
	float m_Xsize, m_Ysize, m_Zsize;
	D3DXVECTOR3 m_Center;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11VertexShader* m_pInstanceVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pVertexLayout = nullptr;
public:
	Collision(){}
	~Collision(){}

	void Init(D3DXVECTOR3 size, D3DXVECTOR3 center);
	void Uninit();
	void Draw();
	void Update();
	void DrawInstance(UINT instanceCount);


	D3DXVECTOR3* GetVertex(){return m_pVertex;}
	D3DXVECTOR3* GetNormal(){return m_pNormal;}
	D3DXVECTOR3  GetPosition() { return m_Center; }

	float  GetXsize() { return m_Xsize; }
	float  GetYsize() { return m_Ysize; }
	float  GetZsize() { return m_Zsize; }
};
