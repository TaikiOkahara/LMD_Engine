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
	m_pMesh->LoadModel("../02 Visual File//Pillar//pillarPBR.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Pillar");



	m_Collision.Init(D3DXVECTOR3(1.0f, 4.5f, 1.0f), D3DXVECTOR3(0, 2.25f, 0));




	D3DXVECTOR3 scale, rot;
	scale = m_Transform.scale;
	rot = m_Transform.rotation;



	for (int i = 0; i < 6; i++)
	{
		//‰E’Œ
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-0.5f, 0.0f,5.0f + 5.0f*i),rot, scale });

		//¶’Œ
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-4.5f, 0.0f,5.0f + 5.0f*i),rot, scale });
	}

	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(4.5f, 0.0f,-5.5f),rot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-9.5f, 0.0f,-5.5f),rot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(4.5f, 0.0f,-14.5f),rot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-9.5f, 0.0f,-14.5f),rot, scale });


	InitInstance();
}

void CPillar::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
	m_Collision.Uninit();
}

void CPillar::Update()
{
	UpdateInstance();

}

void CPillar::Draw()
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

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}