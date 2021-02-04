/*---------------------------------------
*@halfArch.cpp
*
*@authorFOkahara Taiki
----------------------------------------*/
#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "halfArch.h"
#include "Imgui11.h"
#include "input.h"

void CHalfArch::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//HalfArch//halfArchPBR_NoTex.fbx");
	m_pMesh->LoadTexture("../02 Visual File//HalfArch//texture");

	m_Transform.scale = D3DXVECTOR3(0.8f, 0.8f, 0.8f);
	m_Transform.rotation = D3DXVECTOR3(0, D3DX_PI / 2, 0);

	m_Collision.Set(D3DXVECTOR3(1.0f, 4.5f, 1.0f), D3DXVECTOR3(0, 2.25f, 0));


	D3DXVECTOR3 scale, frontRot,backRot;
	scale = m_Transform.scale;
	frontRot = D3DXVECTOR3(0, D3DX_PI / 2, 0);
	backRot = D3DXVECTOR3(0, -D3DX_PI / 2, 0);

	
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3( -12.5f, 3.0f,30.5f),frontRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3( -12.5f, 3.0f,34.5f), backRot, scale });


	InitInstance();
	UpdateInstance();
}

void CHalfArch::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
}

void CHalfArch::Update()
{
}

void CHalfArch::Draw()
{
	DrawInstance();

	RENDERER::GetDeviceContext()->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CHalfArch::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("HalfArch", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}