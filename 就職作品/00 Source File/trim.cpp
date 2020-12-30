#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "trim.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CTrim::Init()
{
	m_pMesh = new StaticMesh();


	m_pMesh->LoadModel("../02 Visual File//Trim//trimPBR_NoTex.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Trim");


	m_Collision.Init(D3DXVECTOR3(5.0f, 0.5f, 0.5f), D3DXVECTOR3(0 ,0, 0));

	

	D3DXVECTOR3 scale, verticalRot, horizonalRot;

	scale = m_Transform.scale;
	verticalRot = D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f);;
	horizonalRot = m_Transform.rotation;//D3DXVECTOR3(0,D3DX_PI,0);



	for (int i = 0; i < 6; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0,0,i * 5.0f + 2.5f),verticalRot, scale });
	}
	for (int i = 0; i < 6; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-5.0f,0,i * 5.0f + 2.5f),verticalRot, scale });
	}

	//スタート地点横
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-6.3f, 0, 0),horizonalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3( 1.3f, 0, 0),horizonalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-5.0f,0, -2.6),verticalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0,0, -2.6),verticalRot, scale });

	//スタート地点囲い
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(2.35f, 0, -4.95),horizonalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.35f,0,-4.95),horizonalRot, scale });

	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f,0, -7.5),verticalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(5,0, -7.5),verticalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f,0, -12.5),verticalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(5,0, -12.5),verticalRot, scale });

	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f,0, -15),horizonalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f,0, -15),horizonalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(2.5,0, -15),horizonalRot, scale });



	InitInstance();
}

void CTrim::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();

	m_Collision.Uninit();

}

void CTrim::Update()
{
	UpdateInstance();

}

void CTrim::Draw()
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

void CTrim::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Trim", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}