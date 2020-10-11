#pragma once
#include "director.h"

class Collision
{
private:
	struct LineVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
	};

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pVertexLayout = nullptr;
public:
	Collision(){}
	~Collision(){}

	void Init();
	void Uninit();
	void Draw();
	void Update();


};
