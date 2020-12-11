#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "chandelier.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CChandelier::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//Chandelier//chandelier.fbx");
	m_pMesh->LoadTexture("");

	

	m_Collision.Init(D3DXVECTOR3(1.0f,1.0f,1.0f),D3DXVECTOR3(0,-1,0));



	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");

	

	TRANSFORM vector;

	vector.rotation = m_Transform.rotation;
	vector.scale = m_Transform.scale;

	//スタート地点
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f,5.0f,-10.0f),vector.rotation,vector.scale });


	for (int i = 0; i < 3; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f, 4.0f, 5.0f + 10.0f * i),vector.rotation,vector.scale });
	}
	//一番奥
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f,4.0f,32.5f),vector.rotation,vector.scale });



	InitInstance();
}

void CChandelier::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
	m_Collision.Uninit();

	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}

void CChandelier::Update()
{
	UpdateInstance();

}

void CChandelier::Draw()
{
	DrawInstance();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if(m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CChandelier::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Chandelier", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);
	

		ImGui::End();
	}
}