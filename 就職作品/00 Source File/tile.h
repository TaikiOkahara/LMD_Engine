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
	void DrawInstanced(UINT instanceCount);

	/*D3DXVECTOR3 GetNormal() { return D3DXVECTOR3(0, 1, 0); }*/
private:
	std::string m_Texture_Name;
	std::string m_NormalTexture_Name;
	

	void MakeVertexField();
	
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	
	ID3D11Texture2D* m_pNormalTexture  = nullptr;


	ID3D11ShaderResourceView* m_pTextureSRV = nullptr;
	ID3D11ShaderResourceView* m_pNormalTextureSRV = nullptr;

	ID3D11RasterizerState* m_pRasterizerState = nullptr;

	int m_NumVertex;
	int m_NumIndex;
	int	m_NumPrimitive;
	int m_XCount;
	int m_YCount;
	float m_TileSize;
};
