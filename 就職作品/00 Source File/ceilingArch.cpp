#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "ceilingArch.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CCeilingArch::Init()
{
	m_pMesh = new StaticMesh();


	m_pMesh->LoadModel("../02 Visual File//CeilingArch//ceilingArchPBR.fbx");
	m_pMesh->LoadTexture("");

	m_Transform.position = D3DXVECTOR3(0.0f, 0.0f, 2.5f);

	m_Collision.Init(D3DXVECTOR3(1, 1, 1), D3DXVECTOR3(0, 0, 0));
	
	
	TRANSFORM vector;
	//‰E
	for (int i = 0; i < 7; i++)
	{
		vector.position = D3DXVECTOR3(0.0f, 3.5f, 2.5f + i * 5);
		vector.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vector.scale = m_Transform.scale;
		
		m_TransformList.push_back(vector);
	}
	//¶
	for (int i = 0; i < 7; i++)
	{
		vector.position = D3DXVECTOR3(-5.0f, 3.5f, 2.5f + i * 5);
		vector.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vector.scale = m_Transform.scale;

		m_TransformList.push_back(vector);
	}
	//‰¡
	for (int i = 0; i < 7; i++)
	{
		vector.position = D3DXVECTOR3(-2.5f, 3.5f, 5.0f + i * 5);
		vector.rotation = D3DXVECTOR3(0.0f, D3DX_PI/2, 0.0f);
		vector.scale = m_Transform.scale;

		m_TransformList.push_back(vector);
	}
	

	InitInstance();
}

void CCeilingArch::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();

	m_Collision.Uninit();

}

void CCeilingArch::Update()
{
	UpdateInstance();

}

void CCeilingArch::Draw()
{
	DrawInstance();

	//RENDERER::m_pDeviceContext->VSSetShader(RENDERER::m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CCeilingArch::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("CeilingArch", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}