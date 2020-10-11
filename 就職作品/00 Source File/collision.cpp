#include "collision.h"
#include "renderer.h"

void Collision::Init()
{
	//頂点の座標
	LineVertex vertex[2];
	vertex[0].pos = D3DXVECTOR3(0.0f, -3.0f, 0.0f);
	vertex[0].tex = D3DXVECTOR2(0, 0);
	vertex[1].pos = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
	vertex[1].tex = D3DXVECTOR2(0, 0);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LineVertex) * 2;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertex[0];	
	RENDERER::m_pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, ARRAYSIZE(layout), "CollisionVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "CollisionPixelShader.cso");
}

void Collision::Uninit()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
}

void Collision::Draw()
{
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1,1,1);
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0,0,0);

	//　マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	RENDERER::SetWorldMatrix(world);


	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	UINT stride = sizeof(LineVertex);
	UINT offset = 0;
	RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	RENDERER::m_pDeviceContext->Draw(2, 0);

	RENDERER::m_pDeviceContext->VSSetShader(NULL, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(NULL, NULL, 0);
	//RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Collision::Update()
{

}