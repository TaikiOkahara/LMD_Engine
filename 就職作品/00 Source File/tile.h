#pragma once
#include "director.h"
#include "renderer.h"

//
//
//
class Tile
{
public:

	MATERIAL m_Material;

	

	void Init(std::string Tex_name, std::string Nor_name,int tile_X_count, int tile_Y_count,float tile_Xsize);
	void Uninit();
	void Update();
	void Draw();
	void DrawInstanced(const unsigned int instanceCount);

private:
	void MakeVertexField();
	

	std::string m_sTexture_Name;
	std::string m_sNormalTexture_Name;
	
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	
	ID3D11Texture2D* m_pNormalTexture  = nullptr;


	ID3D11ShaderResourceView* m_pTextureSRV = nullptr;
	ID3D11ShaderResourceView* m_pNormalTextureSRV = nullptr;

	ID3D11RasterizerState* m_pRasterizerState = nullptr;

	int m_iNumVertex;
	int m_iNumIndex;
	int	m_iNumPrimitive;
	int m_iXCount;
	int m_iYCount;
	float m_fTileSize;
};
