#include "base.h"
#include "renderer.h"
#include "camera.h"
#include "cubeMap.h"
#include "player.h"

void CCubeMap::Init()
{
	

	//D3DXCreateCubeTextureFromFile(RENDERER::m_pDevice,tex_front.c_str(),)
	/*SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFileA(RENDERER::m_pDevice, tex_front.c_str(), NULL, NULL, &m_pTextureSRV, NULL);*/

	/*VERTEX_3D* pVtx = new VERTEX_3D[4];


	pVtx[0] = VERTEX_3D(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,0,0))

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vrData;
	vrData.pSysMem = pVtx;
	RENDERER::m_pDevice->CreateBuffer(&bd, &vrData, &m_pVertexBuffer);


	delete[] pVtx;*/


	//CCamera* camera = Base::GetScene()->GetGameObject<CCamera>(0);

	VERTEX_3D vertex[36];

	{
		vertex[0].Position = D3DXVECTOR3(-1, 1, 1);
		vertex[0].Normal = D3DXVECTOR3(0, 0, -1);
		vertex[0].Tangent = vertex[0].Binormal = D3DXVECTOR3(0, 0, 0);
		vertex[0].TexturePos = D3DXVECTOR2(0, 0);

		vertex[1].Position = D3DXVECTOR3(1, 1, 1);
		vertex[1].Normal = D3DXVECTOR3(0, 0, -1);
		vertex[1].Tangent = vertex[1].Binormal = D3DXVECTOR3(0, 0, 0);
		vertex[1].TexturePos = D3DXVECTOR2(1, 0);

		vertex[2].Position = D3DXVECTOR3(-1, -1, 1);
		vertex[2].Normal = D3DXVECTOR3(0, 0, -1);
		vertex[2].Tangent = vertex[2].Binormal = D3DXVECTOR3(0, 0, 0);
		vertex[2].TexturePos = D3DXVECTOR2(0, 1);

		vertex[3].Position = D3DXVECTOR3(-1, -1, 1);
		vertex[3].Normal = D3DXVECTOR3(0, 0, -1);
		vertex[3].Tangent = vertex[3].Binormal = D3DXVECTOR3(0, 0, 0);
		vertex[3].TexturePos = D3DXVECTOR2(0, 1);

		vertex[4].Position = D3DXVECTOR3(1, 1, 1);
		vertex[4].Normal = D3DXVECTOR3(0, 0, -1);
		vertex[4].Tangent = vertex[4].Binormal = D3DXVECTOR3(0, 0, 0);
		vertex[4].TexturePos = D3DXVECTOR2(1, 0);

		vertex[5].Position = D3DXVECTOR3(1, -1, 1);
		vertex[5].Normal = D3DXVECTOR3(0, 0, -1);
		vertex[5].Tangent = vertex[5].Binormal = D3DXVECTOR3(0, 0, 0);
		vertex[5].TexturePos = D3DXVECTOR2(1, 1);
	}
	{
		vertex[6].Position = D3DXVECTOR3(1, 1, -1);
		vertex[6].Normal = D3DXVECTOR3(0, 0, 1);
		vertex[6].Tangent = vertex[6].Binormal = D3DXVECTOR3(1, 0, 0);
		vertex[6].TexturePos = D3DXVECTOR2(0, 0);

		vertex[7].Position = D3DXVECTOR3(-1, 1, -1);
		vertex[7].Normal = D3DXVECTOR3(0, 0, 1);
		vertex[7].Tangent = vertex[7].Binormal = D3DXVECTOR3(1, 0, 0);
		vertex[7].TexturePos = D3DXVECTOR2(1, 0);

		vertex[8].Position = D3DXVECTOR3(1, -1, -1);
		vertex[8].Normal = D3DXVECTOR3(0, 0, 1);
		vertex[8].Tangent = vertex[8].Binormal = D3DXVECTOR3(1, 0, 0);
		vertex[8].TexturePos = D3DXVECTOR2(0, 1);

		vertex[9].Position = D3DXVECTOR3(1, -1, -1);
		vertex[9].Normal = D3DXVECTOR3(0, 0, 1);
		vertex[9].Tangent = vertex[9].Binormal = D3DXVECTOR3(1, 0, 0);
		vertex[9].TexturePos = D3DXVECTOR2(0, 1);

		vertex[10].Position = D3DXVECTOR3(-1, 1, -1);
		vertex[10].Normal = D3DXVECTOR3(0, 0, 1);
		vertex[10].Tangent = vertex[10].Binormal = D3DXVECTOR3(1, 0, 0);
		vertex[10].TexturePos = D3DXVECTOR2(1, 0);

		vertex[11].Position = D3DXVECTOR3(-1, -1, -1);
		vertex[11].Normal = D3DXVECTOR3(0, 0, 1);
		vertex[11].Tangent = vertex[11].Binormal = D3DXVECTOR3(1, 0, 0);
		vertex[11].TexturePos = D3DXVECTOR2(1, 1);
	}
	{
		vertex[12].Position = D3DXVECTOR3(-1,-1, 1);
		vertex[12].Normal = D3DXVECTOR3(0, 1, 0);
		vertex[12].Tangent = vertex[12].Binormal = D3DXVECTOR3(3, 0, 0);
		vertex[12].TexturePos = D3DXVECTOR2(0, 0);

		vertex[13].Position = D3DXVECTOR3(1,-1, 1);
		vertex[13].Normal = D3DXVECTOR3(0, 1, 0);
		vertex[13].Tangent = vertex[13].Binormal = D3DXVECTOR3(3, 0, 0);
		vertex[13].TexturePos = D3DXVECTOR2(1, 0);

		vertex[14].Position = D3DXVECTOR3(-1,-1, -1);
		vertex[14].Normal = D3DXVECTOR3(0, 1, 0);
		vertex[14].Tangent = vertex[14].Binormal = D3DXVECTOR3(3, 0, 0);
		vertex[14].TexturePos = D3DXVECTOR2(0, 1);

		vertex[15].Position = D3DXVECTOR3(-1,-1, -1);
		vertex[15].Normal = D3DXVECTOR3(0, 1, 0);
		vertex[15].Tangent = vertex[15].Binormal = D3DXVECTOR3(3, 0, 0);
		vertex[15].TexturePos = D3DXVECTOR2(0, 1);

		vertex[16].Position = D3DXVECTOR3(1,-1, 1);
		vertex[16].Normal = D3DXVECTOR3(0, 1, 0);
		vertex[16].Tangent = vertex[16].Binormal = D3DXVECTOR3(3, 0, 0);
		vertex[16].TexturePos = D3DXVECTOR2(1, 0);

		vertex[17].Position = D3DXVECTOR3(1,-1, -1);
		vertex[17].Normal = D3DXVECTOR3(0, 1, 0);
		vertex[17].Tangent = vertex[17].Binormal = D3DXVECTOR3(3, 0, 0);
		vertex[17].TexturePos = D3DXVECTOR2(1, 1);
	}
	{
		vertex[18].Position = D3DXVECTOR3(1, 1, 1);
		vertex[18].Normal = D3DXVECTOR3(0, -1, 0);
		vertex[18].Tangent = vertex[18].Binormal = D3DXVECTOR3(2, 0, 0);
		vertex[18].TexturePos = D3DXVECTOR2(1, 1);

		vertex[19].Position = D3DXVECTOR3(-1, 1, 1);
		vertex[19].Normal = D3DXVECTOR3(0, -1, 0);
		vertex[19].Tangent = vertex[19].Binormal = D3DXVECTOR3(2, 0, 0);
		vertex[19].TexturePos = D3DXVECTOR2(0, 1);

		vertex[20].Position = D3DXVECTOR3(1, 1, -1);
		vertex[20].Normal = D3DXVECTOR3(0, -1, 0);
		vertex[20].Tangent = vertex[20].Binormal = D3DXVECTOR3(2, 0, 0);
		vertex[20].TexturePos = D3DXVECTOR2(1, 0);

		vertex[21].Position = D3DXVECTOR3(1, 1, -1);
		vertex[21].Normal = D3DXVECTOR3(0, -1, 0);
		vertex[21].Tangent = vertex[21].Binormal = D3DXVECTOR3(2, 0, 0);
		vertex[21].TexturePos = D3DXVECTOR2(1, 0);

		vertex[22].Position = D3DXVECTOR3(-1,1, 1);
		vertex[22].Normal = D3DXVECTOR3(0, -1, 0);
		vertex[22].Tangent = vertex[22].Binormal = D3DXVECTOR3(2, 0, 0);
		vertex[22].TexturePos = D3DXVECTOR2(0, 1);

		vertex[23].Position = D3DXVECTOR3(-1, 1, -1);
		vertex[23].Normal = D3DXVECTOR3(0, -1, 0);
		vertex[23].Tangent = vertex[23].Binormal = D3DXVECTOR3(2, 0, 0);
		vertex[23].TexturePos = D3DXVECTOR2(0, 0);
	}
	{
		vertex[24].Position = D3DXVECTOR3(1, 1, 1);
		vertex[24].Normal = D3DXVECTOR3(-1, 0, 0);
		vertex[24].Tangent = vertex[24].Binormal = D3DXVECTOR3(4, 0, 0);
		vertex[24].TexturePos = D3DXVECTOR2(0, 0);

		vertex[25].Position = D3DXVECTOR3(1, 1, -1);
		vertex[25].Normal = D3DXVECTOR3(-1, 0, 0);
		vertex[25].Tangent = vertex[25].Binormal = D3DXVECTOR3(4, 0, 0);
		vertex[25].TexturePos = D3DXVECTOR2(1, 0);

		vertex[26].Position = D3DXVECTOR3(1, -1, 1);
		vertex[26].Normal = D3DXVECTOR3(-1, 0, 0);
		vertex[26].Tangent = vertex[26].Binormal = D3DXVECTOR3(4, 0, 0);
		vertex[26].TexturePos = D3DXVECTOR2(0, 1);

		vertex[27].Position = D3DXVECTOR3(1, -1, 1);
		vertex[27].Normal = D3DXVECTOR3(-1, 0, 0);
		vertex[27].Tangent = vertex[27].Binormal = D3DXVECTOR3(4, 0, 0);
		vertex[27].TexturePos = D3DXVECTOR2(0, 1);

		vertex[28].Position = D3DXVECTOR3(1, 1, -1);
		vertex[28].Normal = D3DXVECTOR3(-1, 0, 0);
		vertex[28].Tangent = vertex[28].Binormal = D3DXVECTOR3(4, 0, 0);
		vertex[28].TexturePos = D3DXVECTOR2(1, 0);

		vertex[29].Position = D3DXVECTOR3(1, -1, -1);
		vertex[29].Normal = D3DXVECTOR3(-1, 0, 0);
		vertex[29].Tangent = vertex[29].Binormal = D3DXVECTOR3(4, 0, 0);
		vertex[29].TexturePos = D3DXVECTOR2(1, 1);
	}
	{
		vertex[30].Position = D3DXVECTOR3(-1, 1, -1);
		vertex[30].Normal = D3DXVECTOR3(1, 0, 0);
		vertex[30].Tangent = vertex[30].Binormal = D3DXVECTOR3(5, 0, 0);
		vertex[30].TexturePos = D3DXVECTOR2(0, 0);

		vertex[31].Position = D3DXVECTOR3(-1, 1, 1);
		vertex[31].Normal = D3DXVECTOR3(1, 0, 0);
		vertex[31].Tangent = vertex[31].Binormal = D3DXVECTOR3(5, 0, 0);
		vertex[31].TexturePos = D3DXVECTOR2(1, 0);

		vertex[32].Position = D3DXVECTOR3(-1, -1, -1);
		vertex[32].Normal = D3DXVECTOR3(1, 0, 0);
		vertex[32].Tangent = vertex[32].Binormal = D3DXVECTOR3(5, 0, 0);
		vertex[32].TexturePos = D3DXVECTOR2(0, 1);

		vertex[33].Position = D3DXVECTOR3(-1, -1, -1);
		vertex[33].Normal = D3DXVECTOR3(1, 0, 0);
		vertex[33].Tangent = vertex[33].Binormal = D3DXVECTOR3(5, 0, 0);
		vertex[33].TexturePos = D3DXVECTOR2(0, 1);

		vertex[34].Position = D3DXVECTOR3(-1, 1, 1);
		vertex[34].Normal = D3DXVECTOR3(1, 0, 0);
		vertex[34].Tangent = vertex[34].Binormal = D3DXVECTOR3(5, 0, 0);
		vertex[34].TexturePos = D3DXVECTOR2(1, 0);

		vertex[35].Position = D3DXVECTOR3(-1, -1, 1);
		vertex[35].Normal = D3DXVECTOR3(1, 0, 0);
		vertex[35].Tangent = vertex[35].Binormal = D3DXVECTOR3(5, 0, 0);
		vertex[35].TexturePos = D3DXVECTOR2(1, 1);
	}

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 36;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	RENDERER::m_pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);


	//テクスチャ読み込み
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, "Sky//DarkStorm4K_f.dds", NULL,NULL, &m_pTexture_f, NULL);//Front
	assert(m_pTexture_f);
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, "Sky//DarkStorm4K_b.dds", NULL,NULL, &m_pTexture_b, NULL);//Back
	assert(m_pTexture_b);
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, "Sky//DarkStorm4K_u.dds", NULL,NULL, &m_pTexture_u, NULL);//Up
	assert(m_pTexture_u);
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, "Sky//DarkStorm4K_d.dds", NULL,NULL, &m_pTexture_d, NULL);//Down
	assert(m_pTexture_d);
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, "Sky//DarkStorm4K_r.dds", NULL,NULL, &m_pTexture_r, NULL);//Right
	assert(m_pTexture_r);
	SetVisualDirectory();
	D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, "Sky//DarkStorm4K_l.dds", NULL,NULL, &m_pTexture_l, NULL);//Left
	assert(m_pTexture_l);


	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	 };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, 5, "cubeMapVS.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "cubeMapPS.cso");


	m_Transform.position = D3DXVECTOR3(-1, 2, 0);
	m_Transform.scale = D3DXVECTOR3(500, 500, 500);
}

void CCubeMap::Uninit()
{
	m_pVertexBuffer->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	m_pVertexLayout->Release();

	m_pTexture_f->Release();
	m_pTexture_b->Release();
	m_pTexture_u->Release();
	m_pTexture_d->Release();
	m_pTexture_r->Release();
	m_pTexture_l->Release();

}

void CCubeMap::Update()
{
	CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>();


	m_Transform.position = player->GetPosition();
}

void CCubeMap::Draw()
{
	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//マトリクス設定
	SetWorldMatrix();


	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//テクスチャ設定
	RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture_f);
	RENDERER::m_pDeviceContext->PSSetShaderResources(1, 1, &m_pTexture_b);
	RENDERER::m_pDeviceContext->PSSetShaderResources(2, 1, &m_pTexture_u);
	RENDERER::m_pDeviceContext->PSSetShaderResources(3, 1, &m_pTexture_d);
	RENDERER::m_pDeviceContext->PSSetShaderResources(4, 1, &m_pTexture_r);
	RENDERER::m_pDeviceContext->PSSetShaderResources(5, 1, &m_pTexture_l);


	RENDERER::m_pDeviceContext->Draw(36, 0);
}

void CCubeMap::Imgui()
{
}
