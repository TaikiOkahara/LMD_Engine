/*---------------------------------------
*　ceiling.cpp
*
*@author：Okahara Taiki
----------------------------------------*/
#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "ceiling.h"
#include "Imgui11.h"
#include "input.h"


void CCeiling::Init()
{
	m_pMesh = new StaticMesh();


	m_pMesh->LoadModel("../02 Visual File//Ceiling//ceilingPBR_NoTex.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Ceiling");

	m_Transform.position = D3DXVECTOR3(-0.5f, 3.5f, 2.5f);

	m_Collision.Set(D3DXVECTOR3(5, 0.2, 5), D3DXVECTOR3(-2.0f, 0, 0));
	

	//スタート地点前方の5つの天井
	TRANSFORM vector;
	for (int i = 0; i < 7; i++)
	{
		vector.position = D3DXVECTOR3(m_Transform.position.x, m_Transform.position.y, m_Transform.position.z + i * 5);
		vector.rotation = m_Transform.rotation;
		vector.scale = m_Transform.scale;
		
		m_TransformList.push_back(vector);
	}

	InitInstance();
	
}

void CCeiling::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
}

void CCeiling::Update()
{
	UpdateInstance();
}

void CCeiling::Draw()
{
	DrawInstance();

	RENDERER::GetDeviceContext()->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);

	m_pMesh->DrawInstanced(m_MeshCount);

	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CCeiling::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Ceiling", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}