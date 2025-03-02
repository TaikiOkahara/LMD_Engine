#pragma once
#include "director.h"
#include "renderer.h"

struct HeightMask
{
	int vertexNumX;
	int vertexNumY;
	float height;
};

class Tile
{
private:
	void MakeVertexField(const HeightMask pMask[]);


	std::string m_sTexture_Name;
	std::string m_sNormalTexture_Name;
	std::string m_sMRATexture_Name;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;


	ID3D11ShaderResourceView* m_pTextureSRV = nullptr;
	ID3D11ShaderResourceView* m_pNormalTextureSRV = nullptr;
	ID3D11ShaderResourceView* m_pMRATextureSRV = nullptr;


	std::vector<std::vector<D3DXVECTOR3>> m_Vtx;

	int m_iNumVertex;
	int m_iNumIndex;
	int	m_iNumPrimitive;
	int m_iXCount;
	int m_iYCount;
	float m_fTileSize;
public:
	void Init(std::string Tex_name, std::string Nor_name, std::string RM_name, int tile_X_count, int tile_Y_count,float tile_Xsize,const  HeightMask pMask[]);
	void Uninit();
	void Update();
	void Draw();
	void DrawInstanced(const unsigned int instanceCount);

	//高さ取得関数(任意の座標のtileの高さを計算)
	float GetHeight(D3DXVECTOR3 Position);

	void SetHeight(HeightMask);
};
