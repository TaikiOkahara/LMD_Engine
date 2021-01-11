#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "pots.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CPots::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//Pots//PotBPBR_NoTex.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Pots");



	m_Collision.Init(D3DXVECTOR3(0.5f,0.5f,0.5f),D3DXVECTOR3(0,0,0));

	

	{

		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-0.6f, 0.0f, -1.2f),D3DXVECTOR3(0.0f, 0.0f, 0.0f) ,D3DXVECTOR3(1.2, 1.5, 1.2) });
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-0.4f, 0.0f, -1.7f) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f) ,D3DXVECTOR3(1, 1, 1) });
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-0.75f, 0.0f, 8.5f) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f) ,D3DXVECTOR3(2.7, 3,2.7) });
	}

	InitInstance();
}

void CPots::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
	m_Collision.Uninit();
}

void CPots::Update()
{
	UpdateInstance();

}

void CPots::Draw()
{
	DrawInstance();

	//RENDERER::m_pDeviceContext->VSSetShader(RENDERER::m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if(m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CPots::Imgui()
{
	static bool show_pillar_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (CInput::KeyTrigger(DIK_F1))
		show_pillar_window = !show_pillar_window;

	if (show_pillar_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Pots", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount,m_MeshMax);
	

		ImGui::End();
	}
}