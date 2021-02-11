/*---------------------------------------
*　demoGargoyler.cpp
*
*@author：Okahara Taiki
----------------------------------------*/
#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "demoGargoyle.h"
#include "Imgui11.h"
#include "input.h"

void CDemoGargoyle::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//Gargoyle//GargoylePBR_NoTex.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Gargoyle");

	m_Collision.Set(D3DXVECTOR3(0.6, 2, 0.6), D3DXVECTOR3(0, 1, 0));

	m_Transform.scale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);

	D3DXVECTOR3 scale, rot;
	scale = m_Transform.scale;
	rot = m_Transform.rotation;

	const float interval = 0.4f;//配置間隔
	const int size = 150;


	for (int x = 0; x < size; x++)
	{
		for (int z = 0; z < size; z++)
		{
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3((-size/2 * interval) + interval * x,0,(-size/2 * interval) + interval * z),rot,scale });
		}
	}


	InitInstance();
}

void CDemoGargoyle::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
}

void CDemoGargoyle::Update()
{
	UpdateInstance();
}

void CDemoGargoyle::Draw()
{
	DrawInstance();

	RENDERER::GetDeviceContext()->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CDemoGargoyle::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Gargoyle", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}