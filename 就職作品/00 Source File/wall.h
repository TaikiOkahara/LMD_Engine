#pragma once
#include "InstanceObject.h"
#include "tile.h"

class CWall : public CInstanceGameObject
{
private:
	Tile* m_pWall = nullptr;
	int m_wallCount = 0;


	std::vector<D3DXVECTOR3> m_posList;
	std::vector<D3DXVECTOR3> m_rotList;

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
	int GetWallCount() { return m_wallCount; }
};