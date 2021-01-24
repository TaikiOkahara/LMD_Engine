/*---------------------------------------
*　edge.cpp
*
*@author：Okahara Taiki
----------------------------------------*/
#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "edge.h"
#include "Imgui11.h"
#include "input.h"

void CEdge::Init()
{
	
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//Edge//edge_PBR_NoTex.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Edge");

	m_Transform.scale = D3DXVECTOR3(0.85f, 0.85f, 0.85f);

	m_Collision.Set(D3DXVECTOR3(5, 0.65, 0.65), D3DXVECTOR3(0, 0, 0));
	
	D3DXVECTOR3 vertical = D3DXVECTOR3(0, D3DX_PI/2, 0);
	D3DXVECTOR3 horizonal = D3DXVECTOR3(0, 0, 0);
	{
		//スタート地点囲い
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(5.0f, 5.0f, -7.4f),vertical,m_Transform.scale});
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(5.0f, 5.0f, -12.5f),vertical,m_Transform.scale});
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(-10.0f, 5.0f,-7.4f),vertical,m_Transform.scale});
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(-10.0f, 5.0f,-12.5f),vertical,m_Transform.scale});


		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(2.5f, 5.0f,-15.0f),horizonal,m_Transform.scale});
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(-2.5f, 5.0f,-15.0f),horizonal,m_Transform.scale});
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(-7.5f, 5.0f,-15.0f),horizonal,m_Transform.scale});

		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(-2.5f, 5.0f, 0.0f),horizonal,m_Transform.scale});

		//道左右
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(2.5f, 5.0f, -5.0f),horizonal,m_Transform.scale});
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(-7.5f, 5.0f, -5.0f),horizonal,m_Transform.scale});

		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(0.0f, 5.0f, -2.5f),vertical,m_Transform.scale});
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(-5.0f, 5.0f, -2.5f),vertical,m_Transform.scale});
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(0.0f, 5.0f, 2.6f),vertical,m_Transform.scale});
		m_TransformList.push_back(TRANSFORM{D3DXVECTOR3(-5.0f, 5.0f, 2.6f),vertical,m_Transform.scale});

	}

	InitInstance();
}


void CEdge::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
}


void CEdge::Update()
{
	UpdateInstance();

}


void CEdge::Draw()
{
	DrawInstance();


	
	RENDERER::GetDeviceContext()->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CEdge::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Edge", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}