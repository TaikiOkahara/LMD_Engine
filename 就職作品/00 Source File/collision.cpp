#include "collision.h"
#include "renderer.h"

void Collision::Init(D3DXVECTOR3 size, D3DXVECTOR3 center)
{
	//頂点の座標
	D3DXVECTOR3* vertex = new D3DXVECTOR3[24];

	m_vCenter = center;
	m_fXsize = size.x/2;
	m_fYsize = size.y/2;
	m_fZsize = size.z/2;

	{
		m_pVertex = new D3DXVECTOR3[8];

		m_pVertex[0] = D3DXVECTOR3(-m_fXsize, m_fYsize,-m_fZsize) + center;
		m_pVertex[1] = D3DXVECTOR3( m_fXsize, m_fYsize,-m_fZsize) + center;
		m_pVertex[2] = D3DXVECTOR3( m_fXsize,-m_fYsize,-m_fZsize) + center;
		m_pVertex[3] = D3DXVECTOR3(-m_fXsize,-m_fYsize,-m_fZsize) + center;
		m_pVertex[4] = D3DXVECTOR3(-m_fXsize, m_fYsize, m_fZsize) + center;
		m_pVertex[5] = D3DXVECTOR3( m_fXsize, m_fYsize, m_fZsize) + center;
		m_pVertex[6] = D3DXVECTOR3( m_fXsize,-m_fYsize, m_fZsize) + center;
		m_pVertex[7] = D3DXVECTOR3(-m_fXsize,-m_fYsize, m_fZsize) + center;
		
	}
	{
		m_pNormal = new D3DXVECTOR3[6];

		m_pNormal[0] = D3DXVECTOR3( 0, 0,-1);
		m_pNormal[1] = D3DXVECTOR3( 0, 0, 1);
		m_pNormal[2] = D3DXVECTOR3( 0, 1, 0);
		m_pNormal[3] = D3DXVECTOR3( 0,-1, 0);
		m_pNormal[4] = D3DXVECTOR3(-1, 0, 0);
		m_pNormal[5] = D3DXVECTOR3( 1, 0, 0);
	}

	{
		vertex[0] = D3DXVECTOR3(-m_fXsize, m_fYsize, -m_fZsize);
		vertex[1] = D3DXVECTOR3( m_fXsize, m_fYsize, -m_fZsize);

		vertex[2] = D3DXVECTOR3( m_fXsize,  m_fYsize, -m_fZsize);
		vertex[3] = D3DXVECTOR3( m_fXsize, -m_fYsize, -m_fZsize);

		vertex[4] = D3DXVECTOR3( m_fXsize,-m_fYsize, -m_fZsize);
		vertex[5] = D3DXVECTOR3(-m_fXsize,-m_fYsize, -m_fZsize);

		vertex[6] = D3DXVECTOR3(-m_fXsize,-m_fYsize, -m_fZsize);
		vertex[7] = D3DXVECTOR3(-m_fXsize, m_fYsize, -m_fZsize);
	}
	{
		vertex[8] = D3DXVECTOR3(-m_fXsize, m_fYsize, m_fZsize);
		vertex[9] = D3DXVECTOR3( m_fXsize, m_fYsize, m_fZsize);

		vertex[10] = D3DXVECTOR3( m_fXsize, m_fYsize, m_fZsize);
		vertex[11] = D3DXVECTOR3( m_fXsize,-m_fYsize, m_fZsize);

		vertex[12] = D3DXVECTOR3( m_fXsize,-m_fYsize, m_fZsize);
		vertex[13] = D3DXVECTOR3(-m_fXsize,-m_fYsize, m_fZsize);

		vertex[14] = D3DXVECTOR3(-m_fXsize, -m_fYsize, m_fZsize);
		vertex[15] = D3DXVECTOR3(-m_fXsize, m_fYsize, m_fZsize);
	}
	{
		vertex[16] = D3DXVECTOR3(-m_fXsize, m_fYsize, -m_fZsize);
		vertex[17] = D3DXVECTOR3(-m_fXsize, m_fYsize, m_fZsize);

		vertex[18] = D3DXVECTOR3(m_fXsize, m_fYsize, -m_fZsize);
		vertex[19] = D3DXVECTOR3(m_fXsize, m_fYsize, m_fZsize);

		vertex[20] = D3DXVECTOR3(m_fXsize, -m_fYsize, -m_fZsize);
		vertex[21] = D3DXVECTOR3(m_fXsize, -m_fYsize, m_fZsize);

		vertex[22] = D3DXVECTOR3(-m_fXsize, -m_fYsize, -m_fZsize);
		vertex[23] = D3DXVECTOR3(-m_fXsize, -m_fYsize, m_fZsize);
	}

	for (int i = 0; i < 24; i++)
	{
		vertex[i] += center;
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(D3DXVECTOR3) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertex[0];	
	RENDERER::m_pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);

	delete[]  vertex;

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, ARRAYSIZE(layout), "CollisionVertexShader.cso");
	RENDERER::CreateVertexShader(&m_pInstanceVertexShader, &m_pVertexLayout, layout, ARRAYSIZE(layout), "CollisionInstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "CollisionPixelShader.cso");
}

void Collision::Uninit()
{
	delete[] m_pVertex;
	delete[] m_pNormal;
	
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pInstanceVertexShader);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
}

void Collision::Draw()
{
	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	RENDERER::m_pDeviceContext->Draw(24, 0);

	RENDERER::m_pDeviceContext->VSSetShader(NULL, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(NULL, NULL, 0);
}

void Collision::Update()
{

}

void Collision::DrawInstance(const unsigned int instanceCount)
{
	RENDERER::m_pDeviceContext->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	RENDERER::m_pDeviceContext->DrawInstanced(24,instanceCount, 0,0);

	RENDERER::m_pDeviceContext->VSSetShader(NULL, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(NULL, NULL, 0);
}