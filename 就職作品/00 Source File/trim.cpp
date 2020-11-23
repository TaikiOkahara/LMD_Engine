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


	m_pMesh->LoadModel("../02 Visual File//Trim//trim.fbx");
	m_pMesh->LoadTexture("../02 Visual File//Trim");

	m_Transform.rotation = D3DXVECTOR3(0.0f, D3DX_PI/2, 0.0f);
	
	m_Collision.Init(D3DXVECTOR3(1, 1, 1), D3DXVECTOR3(0, 0, 0));

	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");

	



	D3DXVECTOR3 scale, verticalRot, horizonalRot;

	scale = m_Transform.scale;
	verticalRot = m_Transform.rotation;
	horizonalRot = m_Transform.rotation + D3DXVECTOR3(0,D3DX_PI/2,0);



	for (int i = 0; i < 6; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0,0,i * 5.0f),verticalRot, scale });
	}
	for (int i = 0; i < 6; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-5.0f,0,i * 5.0f),verticalRot, scale });
	}

	//スタート地点横
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-8.8f, 0, 0),horizonalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-1.2f, 0, 0),horizonalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-5.0f,0, -5.1),verticalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0,0, -5.1),verticalRot, scale });

	//スタート地点囲い
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-0.15f, 0, -4.95),horizonalRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-9.85f,0,-4.95),horizonalRot, scale });







	InitInstance();
}

void CTrim::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();

	m_Collision.Uninit();

	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}

void CTrim::Update()
{
	UpdateInstance();

}

void CTrim::Draw()
{
	DrawInstance();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


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

		ImGui::Checkbox("isEnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}