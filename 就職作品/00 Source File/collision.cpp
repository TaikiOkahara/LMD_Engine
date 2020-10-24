#include "collision.h"
#include "renderer.h"

void Collision::Init(D3DXVECTOR3 size, D3DXVECTOR3 center)
{
	//頂点の座標
	LineVertex* vertex = new LineVertex[24];

	float xSize, ySize, zSize;
	xSize = size.x/2;
	ySize = size.y/2;
	zSize = size.z/2;

	{

		

		m_pVertex = new D3DXVECTOR3[8];

		m_pVertex[0] = D3DXVECTOR3(-xSize, ySize,-zSize);
		m_pVertex[1] = D3DXVECTOR3( xSize, ySize,-zSize);
		m_pVertex[2] = D3DXVECTOR3( xSize,-ySize,-zSize);
		m_pVertex[3] = D3DXVECTOR3(-xSize,-ySize,-zSize);
		m_pVertex[4] = D3DXVECTOR3(-xSize, ySize, zSize);
		m_pVertex[5] = D3DXVECTOR3( xSize, ySize, zSize);
		m_pVertex[6] = D3DXVECTOR3( xSize,-ySize, zSize);
		m_pVertex[7] = D3DXVECTOR3(-xSize,-ySize, zSize);


		
	}
	{
		vertex[0].pos = D3DXVECTOR3(-xSize, ySize, -zSize);
		vertex[1].pos = D3DXVECTOR3( xSize, ySize, -zSize);

		vertex[2].pos = D3DXVECTOR3( xSize,  ySize, -zSize);
		vertex[3].pos = D3DXVECTOR3( xSize, -ySize, -zSize);

		vertex[4].pos = D3DXVECTOR3( xSize,-ySize, -zSize);
		vertex[5].pos = D3DXVECTOR3(-xSize,-ySize, -zSize);

		vertex[6].pos = D3DXVECTOR3(-xSize,-ySize, -zSize);
		vertex[7].pos = D3DXVECTOR3(-xSize, ySize, -zSize);
	}
	{
		vertex[8].pos = D3DXVECTOR3(-xSize, ySize, zSize);
		vertex[9].pos = D3DXVECTOR3( xSize, ySize, zSize);

		vertex[10].pos = D3DXVECTOR3( xSize, ySize, zSize);
		vertex[11].pos = D3DXVECTOR3( xSize,-ySize, zSize);

		vertex[12].pos = D3DXVECTOR3( xSize,-ySize, zSize);
		vertex[13].pos = D3DXVECTOR3(-xSize,-ySize, zSize);

		vertex[14].pos = D3DXVECTOR3(-xSize, -ySize, zSize);
		vertex[15].pos = D3DXVECTOR3(-xSize, ySize, zSize);
	}
	{
		vertex[16].pos = D3DXVECTOR3(-xSize, ySize, -zSize);
		vertex[17].pos = D3DXVECTOR3(-xSize, ySize, zSize);

		vertex[18].pos = D3DXVECTOR3(xSize, ySize, -zSize);
		vertex[19].pos = D3DXVECTOR3(xSize, ySize, zSize);

		vertex[20].pos = D3DXVECTOR3(xSize, -ySize, -zSize);
		vertex[21].pos = D3DXVECTOR3(xSize, -ySize, zSize);

		vertex[22].pos = D3DXVECTOR3(-xSize, -ySize, -zSize);
		vertex[23].pos = D3DXVECTOR3(-xSize, -ySize, zSize);
	}

	for (int i = 0; i < 24; i++)
	{
		vertex[i].pos += center;
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LineVertex) * 24;
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
	
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pInstanceVertexShader);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
}

void Collision::Draw()
{
	//D3DXVECTOR3 m_Scale = D3DXVECTOR3(1,1,1);
	//D3DXVECTOR3 m_Position = D3DXVECTOR3(0,0,0);
	//D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0,0,0);

	////　マトリクス設定
	//D3DXMATRIX world, scale, rot, trans;
	//D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	//D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	//world = scale * rot * trans;

	//RENDERER::SetWorldMatrix(world);


	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	UINT stride = sizeof(LineVertex);
	UINT offset = 0;
	RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	RENDERER::m_pDeviceContext->Draw(24, 0);

	RENDERER::m_pDeviceContext->VSSetShader(NULL, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(NULL, NULL, 0);
	//RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Collision::Update()
{

}

void Collision::DrawInstance(UINT instanceCount)
{
	RENDERER::m_pDeviceContext->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	UINT stride = sizeof(LineVertex);
	UINT offset = 0;
	RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	RENDERER::m_pDeviceContext->DrawInstanced(24,instanceCount, 0,0);

	RENDERER::m_pDeviceContext->VSSetShader(NULL, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(NULL, NULL, 0);
}