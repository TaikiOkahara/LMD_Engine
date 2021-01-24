/*---------------------------------------
*@table.cpp
*
*@authorFOkahara Taiki
----------------------------------------*/
#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "table.h"
#include "Imgui11.h"
#include "input.h"


void CTable::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//Table//TablePBR_NoTex.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Table");

	m_Collision.Set(D3DXVECTOR3(2.0, 1, 2.0), D3DXVECTOR3(0, 0.5f, 0));	
	
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(3.0f,0,-12.5f),m_Transform.rotation,m_Transform.scale });
	}

	InitInstance();
}

void CTable::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
}

void CTable::Update()
{
	UpdateInstance();
}

void CTable::Draw()
{
	DrawInstance();
	
	RENDERER::GetDeviceContext()->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CTable::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Table", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}