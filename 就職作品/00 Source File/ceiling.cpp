#include "director.h"
#include "renderer.h"
#include "StaticMesh.h"
#include "ceiling.h"


//
//
//
void CCeiling::Init()
{
	m_pMesh = new StaticMesh();


	m_pMesh->LoadModel("../02 Visual File//Ceiling//Ceiling.fbx");
	m_pMesh->LoadTexture("");

	m_Position = D3DXVECTOR3(-2.5f, 4.0f, 0.5f);
	m_Rotation = D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f);
	//m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, 5, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");

	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

	for (int i = 0; i < 6; i++)
	{

		//　マトリクス設定
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z + i * 5);
		world = scale * rot * trans;
		D3DXMatrixTranspose(&world, &world);

		m_MatrixList.push_back(world);
	}
	

	InitInstance();
	UpdateInstance();//処理速度が落ちるかもだからInitに置いてる
}

void CCeiling::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();


	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
}

void CCeiling::Update()
{

}

void CCeiling::Draw()
{
	DrawInstance();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);
}