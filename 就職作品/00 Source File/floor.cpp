/*---------------------------------------
*@floor.cpp
*
*@authorFOkahara Taiki
----------------------------------------*/
#include "director.h"
#include "renderer.h"
#include "floor.h"
#include "Imgui11.h"
#include "input.h"


void CFloor::Init()
{
	
	m_pField = new Tile();
	m_pField->Init("BrickFloor//T_BrickFloor_Clean_A.dds", "BrickFloor//T_BrickFloor_Clean_N.dds", "BrickFloor//T_BrickFloor_Clean_C.dds",100,100,1.0f,g_FloorMask);
	
	m_Collision.Set(D3DXVECTOR3(100.0f, 1.0f, 100.0f), D3DXVECTOR3(0, 0, 0));

	m_Transform = TRANSFORM{ D3DXVECTOR3(0, 0.0f,0),D3DXVECTOR3(0, 0, 0.0f), D3DXVECTOR3(1,1,1) };
	//m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0, 0.0f,0),D3DXVECTOR3(0, 0, 0.0f), D3DXVECTOR3(1,1,1) });


	//RENDERER::CreateVertexShader(&m_VertexShader, nullptr, nullptr, 0, "commonVS.cso");
	//RENDERER::CreatePixelShader(&m_PixelShader, "tilePS.cso");


	//InitInstance();
	//UpdateInstance();
}

void CFloor::Uninit()
{
	m_pField->Uninit();
	delete m_pField;

	//UninitInstance();
}

void CFloor::Update()
{
}

void CFloor::Draw()
{

	//DrawInstance();

	SetWorldMatrix();

	//RENDERER::GetDeviceContext()->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->VSSetShader(m_pCommonVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);

	//RENDERER::SetRasterizerState(D3D11_CULL_MODE::D3D11_CULL_BACK, D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);

	//m_pField->DrawInstanced(m_MeshCount);
	m_pField->Draw();

	//RENDERER::SetRasterizerState(D3D11_CULL_MODE::D3D11_CULL_NONE, D3D11_FILL_MODE::D3D11_FILL_SOLID);
	if (m_EnableCollision)
		m_Collision.Draw();
		//m_Collision.DrawInstance(m_MeshCount);
}

void CFloor::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Floor", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		//ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}