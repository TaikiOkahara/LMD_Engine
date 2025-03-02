#include "director.h"
#include "tile.h"
#include "Scene.h"
#include "calculation.h"


void Tile::MakeVertexField(const HeightMask pMask[])
{

	VERTEX_3D* pVtx = new VERTEX_3D[m_iNumVertex];

	m_Vtx.resize((m_iXCount + 1));
	for (size_t i = 0; i < m_iXCount + 1; i++)
	{
		m_Vtx[i].resize(m_iYCount + 1);
	}


	const float offset_x = m_iXCount * m_fTileSize / 2;//中心点
	const float offset_y = m_iYCount * m_fTileSize / 2;//中心点

	

	//バーテックス
	for (int x = 0; x <= m_iXCount; x++) {

		for (int z = 0; z <= m_iYCount; z++) {

			
			pVtx[x * (m_iYCount + 1) + z].Position = D3DXVECTOR3((x - (m_iXCount / 2)) * m_fTileSize, 0, (z - (m_iYCount / 2)) * -m_fTileSize);
			m_Vtx[x][z] = pVtx[x * (m_iYCount + 1) + z].Position;


			pVtx[x * (m_iYCount + 1) + z].TexturePos.x = (float)m_iXCount - x;
			pVtx[x * (m_iYCount + 1) + z].TexturePos.y = (float)m_iYCount - z;
			pVtx[x * (m_iYCount + 1) + z].Normal = D3DXVECTOR3(0, 1, 0);
			pVtx[x * (m_iYCount + 1) + z].Tangent = D3DXVECTOR3(1, 0, 0);
			pVtx[x * (m_iYCount + 1) + z].Binormal = D3DXVECTOR3(0, 0, 1);


			//高さ設定
			float height = 0;
			if (pMask == NULL)
				continue;
			for (int i = 0;pMask[i].height != NULL; i++)
			{
				if (x == pMask[i].vertexNumX
					&& z == pMask[i].vertexNumY)
				{
					height = pMask[i].height;

					pVtx[x * (m_iYCount + 1) + z].Position.y = height;
					m_Vtx[x][z].y = height;
				}
			}

			
		}
	}
	

	//インデックス
	WORD* pIdx = new WORD[m_iNumIndex];
	int idx_cnt = 0;
	for (int x = 0; x < m_iXCount; x++)
	{
		for (int z = 0; z < (m_iYCount + 1); z++)
		{
			pIdx[idx_cnt] = x * (m_iYCount + 1) + z;
			idx_cnt++;
			pIdx[idx_cnt] = (x + 1) * (m_iYCount + 1) + z;
			idx_cnt++;
		}
		if (x == (m_iXCount - 1))
			break;

		pIdx[idx_cnt] = (x + 1) * (m_iYCount + 1) + m_iYCount;
		idx_cnt++;
		pIdx[idx_cnt] = (x + 1) * (m_iYCount + 1);
		idx_cnt++;
	}
	

	
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 	m_iNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vrData;
	vrData.pSysMem = pVtx;
	RENDERER::GetDevice()->CreateBuffer(&bd, &vrData, &m_pVertexBuffer);
	

	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * m_iNumIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA idData;
	idData.pSysMem = pIdx;
	RENDERER::GetDevice()->CreateBuffer(&bd, &idData, &m_pIndexBuffer);

	
	delete[] pVtx;
	delete[] pIdx;
		
	
	
	//　カラーテクスチャ―作成
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFileA(RENDERER::GetDevice(), m_sTexture_Name.c_str(), NULL, NULL, &m_pTextureSRV, NULL);

	//　ノーマルテクスチャ―作成
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFileA(RENDERER::GetDevice(), m_sNormalTexture_Name.c_str(), NULL, NULL, &m_pNormalTextureSRV, NULL);

	//　RMAクスチャ―作成
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFileA(RENDERER::GetDevice(), m_sMRATexture_Name.c_str(), NULL, NULL, &m_pMRATextureSRV, NULL);


}

void Tile::Init(std::string Tex_name,std::string Nor_name,std::string RM_name, int tile_X_count, int tile_Y_count, float tile_size,const HeightMask pMask[])
{

	m_sTexture_Name = Tex_name;
	m_sNormalTexture_Name = Nor_name;
	m_sMRATexture_Name = RM_name;
	m_iNumVertex = (tile_X_count + 1) * (tile_Y_count + 1);
	m_iNumIndex = (tile_X_count + 1) * 2 * tile_Y_count + (tile_Y_count - 1) * 2;
	m_iNumPrimitive = (tile_X_count * tile_Y_count) * 2 + (tile_Y_count - 1) * 4;
	m_iXCount = tile_X_count;
	m_iYCount = tile_Y_count;
	m_fTileSize = tile_size;


	// 頂点情報の作成
	MakeVertexField(pMask);
}

void Tile::Uninit()
{
	m_Vtx.clear();

	m_pIndexBuffer->Release();
	m_pVertexBuffer->Release();

	m_pTextureSRV->Release();
	m_pNormalTextureSRV->Release();
	m_pMRATextureSRV->Release();

}

void Tile::Update(){}

void Tile::Draw()
{

	RENDERER::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	RENDERER::GetDeviceContext()->PSSetShaderResources(0, 1, &m_pTextureSRV);
	RENDERER::GetDeviceContext()->PSSetShaderResources(1, 1, &m_pNormalTextureSRV);
	RENDERER::GetDeviceContext()->PSSetShaderResources(2, 1, &m_pMRATextureSRV);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	RENDERER::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	RENDERER::GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	RENDERER::GetDeviceContext()->DrawIndexed(m_iNumIndex,0, 0);
}

void Tile::DrawInstanced(const unsigned int instanceCount)
{

	RENDERER::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	RENDERER::GetDeviceContext()->PSSetShaderResources(0, 1, &m_pTextureSRV);
	RENDERER::GetDeviceContext()->PSSetShaderResources(1, 1, &m_pNormalTextureSRV);
	RENDERER::GetDeviceContext()->PSSetShaderResources(2, 1, &m_pMRATextureSRV);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	RENDERER::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	RENDERER::GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	RENDERER::GetDeviceContext()->DrawIndexedInstanced(m_iNumIndex, instanceCount, 0, 0, 0);
}

//高さ取得関数(任意の座標のtileの高さを計算)
float Tile::GetHeight(D3DXVECTOR3 Position)
{
	int x, z;

	//ブロック番号算出
	x = (int)(Position.x / m_fTileSize) + (m_iXCount)/2;
	z = (int)(Position.z / -m_fTileSize) + (m_iYCount)/2;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	pos0 = m_Vtx[x][z];
	pos1 = m_Vtx[x + 1][z];
	pos2 = m_Vtx[x][z + 1];
	pos3 = m_Vtx[x + 1][z + 1];

	D3DXVECTOR3 v12, v1p, c;

	v12 = pos2 - pos1;
	v1p = Position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;

	if (c.y > 0.0f) {
		//左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else {
		//右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	//高さ取得
	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;

	if (abs(Position.y - py) > 10)//変化が急なとき
	{
		int a = 0;
	}

	return py;
}

void Tile::SetHeight(HeightMask mask)
{

	m_Vtx[mask.vertexNumX][mask.vertexNumY].y = mask.height;
}
