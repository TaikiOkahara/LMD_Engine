#pragma once
#include "director.h"

class Collision
{
private:
	struct LineVertex
	{
		D3DXVECTOR3 pos;
	};

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

};
