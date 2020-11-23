#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "pillar.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CPillar::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//Pillar//pillar.fbx");
	m_pMesh->LoadTexture("");



	m_Collision.Init(D3DXVECTOR3(1.0f, 4.5f, 1.0f), D3DXVECTOR3(0, 2.25f, 0));



	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");




	D3DXVECTOR3 scale, rot;
	scale = m_Transform.scale;
	rot = m_Transform.rotation;



	for (int i = 0; i < 6; i++)
	{
		//右柱
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-0.5f, 0.0f,5.0f + 5.0f*i),rot, scale });

		//左柱
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-4.5f, 0.0f,5.0f + 5.0f*i),rot, scale });
	}

	

	InitInstance();
}

void CPillar::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
	m_Collision.Uninit();

	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}

void CPillar::Update()
{
	UpdateInstance();

}

void CPillar::Draw()
{
	DrawInstance();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CPillar::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Pillar", &lw_is_open, lw_flag);

		ImGui::Checkbox("isEnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}