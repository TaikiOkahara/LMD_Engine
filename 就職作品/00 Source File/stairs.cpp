/*---------------------------------------
*@stairs.cpp
*
*@authorFOkahara Taiki
----------------------------------------*/
#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "stairs.h"
#include "Imgui11.h"
#include "input.h"


void CStairs::Init()
{
	
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//Stairs//stairsPBR_NoTex.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Stairs//texture");


	m_Transform.position = D3DXVECTOR3(-7.0f, 0.0f, 32.5f);
	m_Transform.rotation = D3DXVECTOR3(0.0f, -D3DX_PI/2, 0.0f);

	m_Collision.Set(D3DXVECTOR3(1, 1, 1), D3DXVECTOR3(0, 0, 0));
	
	
	m_TransformList.push_back(TRANSFORM{ m_Transform.position,m_Transform.rotation,m_Transform.scale });
	m_TransformList.push_back(TRANSFORM{ m_Transform.position + D3DXVECTOR3(-3.0f,2,0),m_Transform.rotation,m_Transform.scale });


	InitInstance();
}

void CStairs::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
}

void CStairs::Update()
{
	UpdateInstance();

}

void CStairs::Draw()
{
	DrawInstance();


	RENDERER::GetDeviceContext()->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CStairs::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Stairs", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}