//#include <string>
#include "director.h"
#include "tile.h"
#include "Scene.h"
#include "calculation.h"

// マクロ定義
//#define BLOCK_XY	50	//ブロックの数
//#define BLOCK_SIZE  1.0f//ブロックのサイズ



//
//
// 頂点の作成
void Tile::MakeVertexField()
{

	VERTEX_3D* pVtx = new VERTEX_3D[m_NumVertex];



	float fTexSizeX = 1.0f;
	float fTexSizeZ = 1.0f;

	const float offset_x = m_XCount * m_TileSize / 2;
	const float offset_y = m_YCount * m_TileSize / 2;

	// 頂点座標の設定
	for (int z = 0; z <= m_YCount; z++) {

		for (int x = 0; x <= m_XCount; x++) {

			pVtx[z * (m_XCount + 1) + x].Position = D3DXVECTOR3(-x * (m_TileSize) + offset_x, 0, z * (m_TileSize) - offset_y);
									  
			pVtx[z * (m_XCount + 1) + x].TexturePos.x = (float)x;
			pVtx[z * (m_XCount + 1) + x].TexturePos.y = (float)z;
			pVtx[z * (m_XCount + 1) + x].Normal = D3DXVECTOR3(0, 1, 0);
			pVtx[z * (m_XCount + 1) + x].Tangent = D3DXVECTOR3(1, 0, 0);
			pVtx[z * (m_XCount + 1) + x].Binormal = D3DXVECTOR3(0, 0, 1);
		}
	}
	

	//インデックス
	WORD* pIdx = new WORD[m_NumIndex];
	int idx_cnt = 0;
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < (m_XCount + 1); x++)
		{
			pIdx[idx_cnt] = (m_XCount + 1) * y + (m_XCount + 1) + x;
			idx_cnt++;
			pIdx[idx_cnt] = (m_XCount + 1) * y + x;
			idx_cnt++;
		}
		if (y < (m_YCount - 1))
		{
			pIdx[idx_cnt] = (m_XCount + 1)*y + m_XCount;
			idx_cnt++;
			pIdx[idx_cnt] = (m_XCount + 1)*(y + 2);
			idx_cnt++;
		}
	}
	//　接線
	//for (int poly_num = 0; poly_num < m_NumPrimitive; poly_num++)
	//{
	//	D3DXVECTOR3 vTangent;
	//	D3DXVECTOR3 vBinormal;

	//	CalcTangent(
	//		pVtx[pIdx[poly_num    ]].Position,pVtx[pIdx[poly_num + 1]].Position,pVtx[pIdx[poly_num + 2]].Position,
	//		pVtx[pIdx[poly_num    ]].TexturePos,pVtx[pIdx[poly_num + 1]].TexturePos,pVtx[pIdx[poly_num + 2]].TexturePos,
	//		pVtx[pIdx[poly_num]].Normal,
	//		vTangent,vBinormal);

	//	//if (pVtx[pIdx[poly_num]].Tangent == D3DXVECTOR4(0,0,0,0))
	//		
	//	pVtx[pIdx[poly_num]].Tangent = vTangent;
	//	pVtx[pIdx[poly_num]].Binormal = vBinormal;
	//	
	//	//if (pVtx[pIdx[poly_num + 1]].Tangent == D3DXVECTOR4(0, 0, 0, 0))
	//	
	//	pVtx[pIdx[poly_num + 1]].Tangent = vTangent;
	//	pVtx[pIdx[poly_num + 1]].Binormal = vBinormal;
	//	
	//	//if (pVtx[pIdx[poly_num + 2]].Tangent == D3DXVECTOR4(0, 0, 0, 0))
	//		
	//	pVtx[pIdx[poly_num + 2]].Tangent = vTangent;
	//	pVtx[pIdx[poly_num + 2]].Binormal = vBinormal;
	//	
	//	
	//}

	
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 	m_NumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vrData;
	vrData.pSysMem = pVtx;
	RENDERER::m_pDevice->CreateBuffer(&bd, &vrData, &m_pVertexBuffer);
	

	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * m_NumIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA idData;
	idData.pSysMem = pIdx;
	RENDERER::m_pDevice->CreateBuffer(&bd, &idData, &m_pIndexBuffer);

	
	delete[] pVtx;
	delete[] pIdx;
		
	

	//ノーマルテクスチャ―作成
	SetVisualDirectory();
	D3DX11CreateTextureFromFileA(RENDERER::m_pDevice, m_NormalTexture_Name.c_str(), NULL, NULL, (ID3D11Resource**)&m_pNormalTexture, NULL);


	//ノーマルマップテクスチャーのシェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	D3D11_TEXTURE2D_DESC desc;
	m_pNormalTexture->GetDesc( &desc );
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	HRESULT hr = RENDERER::m_pDevice->CreateShaderResourceView(m_pNormalTexture, &srvDesc, &m_pNormalTextureSRV);

	
	//　カラーテクスチャ―作成
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFileA(RENDERER::m_pDevice, m_Texture_Name.c_str(), NULL, NULL, &m_pTextureSRV, NULL);

	
	//　ノーマルテクスチャ―作成
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFileA(RENDERER::m_pDevice, m_NormalTexture_Name.c_str(), NULL, NULL, &m_pNormalTextureSRV, NULL);
	
}

void Tile::Init(std::string Tex_name,std::string Nor_name, int tile_X_count, int tile_Y_count, float tile_size)
{

	m_Material.Ambient = D3DXVECTOR4(0.1, 0.1, 0.1, 1);
	m_Material.Diffuse = D3DXVECTOR4(0.8, 0.8, 0.8, 1);
	m_Material.Specular = D3DXVECTOR4(0.2, 0.2, 0.2, 1);

	m_Texture_Name = Tex_name;
	m_NormalTexture_Name = Nor_name;
	m_NumVertex = (tile_X_count + 1) * (tile_Y_count + 1);
	m_NumIndex = (tile_X_count + 1) * 2 * tile_Y_count + (tile_Y_count - 1) * 2;
	m_NumPrimitive = (tile_X_count * tile_Y_count) * 2 + (tile_Y_count - 1) * 4;
	m_XCount = tile_X_count;
	m_YCount = tile_Y_count;
	m_TileSize = tile_size;


	// 頂点情報の作成
	MakeVertexField();
}

void Tile::Uninit(){}

void Tile::Update(){}

void Tile::Draw()
{

	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	RENDERER::SetMaterial(m_Material);

	RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	RENDERER::m_pDeviceContext->PSSetShaderResources(1, 1, &m_pNormalTextureSRV);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	RENDERER::m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	RENDERER::m_pDeviceContext->DrawIndexed(m_NumIndex,0, 0);
}

void Tile::DrawInstanced(UINT instanceCount)
{

	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	RENDERER::SetMaterial(m_Material);

	RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	RENDERER::m_pDeviceContext->PSSetShaderResources(1, 1, &m_pNormalTextureSRV);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	RENDERER::m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	RENDERER::m_pDeviceContext->DrawIndexedInstanced(m_NumIndex, instanceCount, 0, 0, 0);
}