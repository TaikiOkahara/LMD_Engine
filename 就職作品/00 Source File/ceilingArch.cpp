#include "director.h"
#include "renderer.h"
#include "StaticMesh.h"
#include "ceilingArch.h"


//
//
//
void CCeilingArch::Init()
{
	m_pMesh = new StaticMesh();


	m_pMesh->LoadModel("../02 Visual File//CeilingArch//ceilingArch.fbx");
	m_pMesh->LoadTexture("");

	m_Transform.position = D3DXVECTOR3(0.0f, 0.0f, 2.5f);


	
	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");


	VECTOR vector;
	//右
	for (int i = 0; i < 6; i++)
	{
		vector.position = D3DXVECTOR3(0.0f, 3.5f, 2.5f + i * 5);
		vector.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vector.scale = m_Transform.scale;
		
		m_Vector.push_back(vector);
	}
	//左
	for (int i = 0; i < 6; i++)
	{
		vector.position = D3DXVECTOR3(-5.0f, 3.5f, 2.5f + i * 5);
		vector.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vector.scale = m_Transform.scale;

		m_Vector.push_back(vector);
	}
	//横
	for (int i = 0; i < 6; i++)
	{
		vector.position = D3DXVECTOR3(-2.5f, 3.5f, 5.0f + i * 5);
		vector.rotation = D3DXVECTOR3(0.0f, D3DX_PI/2, 0.0f);
		vector.scale = m_Transform.scale;

		m_Vector.push_back(vector);
	}
	

	InitInstance();
	UpdateInstance();//処理速度が落ちるかもだからInitに置いてる
}

void CCeilingArch::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();


	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}

void CCeilingArch::Update()
{

}

void CCeilingArch::Draw()
{
	DrawInstance();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);
}