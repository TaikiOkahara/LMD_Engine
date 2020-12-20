#pragma once
#include "InstanceObject.h"
#include "tile.h"
#include "collision.h"

class CWall : public CInstanceGameObject
{
private:
	Tile* m_pWall = nullptr;
	

	
	ID3D11PixelShader* m_pPixelShader;
public:
	CWall() {}
	~CWall() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
	
};