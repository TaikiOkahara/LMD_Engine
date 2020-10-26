#include "director.h"
#include "renderer.h"
#include "StaticMesh.h"
#include "trim.h"


//
//
//
void CTrim::Init()
{
	m_pMesh = new StaticMesh();


	m_pMesh->LoadModel("../02 Visual File//Trim//trim.fbx");
	//m_pMesh->LoadModel("../02 Visual File//Chandelier//chandelier.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Trim");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, D3DX_PI/2, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	

	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");

	


	VECTOR vector;
	vector.scale = m_Scale;
	vector.rotation = m_Rotation;
	for (int i = 0; i < 4; i++)
	{
		//　マトリクス設定
		vector.position = D3DXVECTOR3(m_Position.x - 0.2f, m_Position.y, m_Position.z + i * 5);
		m_Vector.push_back(vector);

	}
	for (int i = 0; i < 4; i++)
	{
		vector.position = D3DXVECTOR3(m_Position.x - 4.7f, m_Position.y, m_Position.z + i * 5);
		m_Vector.push_back(vector);
	}

	InitInstance();
	UpdateInstance();//処理速度が落ちるかもだからInitに置いてる
}

void CTrim::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();


	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}

void CTrim::Update()
{

}

void CTrim::Draw()
{
	DrawInstance();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);
}