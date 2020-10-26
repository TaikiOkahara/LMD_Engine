#pragma once
#include "InstanceObject.h"
#include "tile.h"
#include "collision.h"

class CWall : public CInstanceGameObject
{
private:
	Tile* m_pWall = nullptr;
	
	bool isEnableCollision = true;

	
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pVertexLayout;
public:
	CWall() {}
	~CWall() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
	
};