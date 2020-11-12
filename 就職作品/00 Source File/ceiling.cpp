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


	m_pMesh->LoadModel("../02 Visual File//Ceiling//ceiling.fbx");
	m_pMesh->LoadTexture("");

	m_Transform.position = D3DXVECTOR3(-0.5f, 3.5f, 2.5f);


	
	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");


	VECTOR vector;
	for (int i = 0; i < 6; i++)
	{
		vector.position = D3DXVECTOR3(m_Transform.position.x, m_Transform.position.y, m_Transform.position.z + i * 5);
		vector.rotation = m_Transform.rotation;
		vector.scale = m_Transform.scale;
		
		m_Vector.push_back(vector);
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
}

void CCeiling::Update()
{

}

void CCeiling::Draw()
{
	DrawInstance();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);
}