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

}

void CFloor::Uninit()
{
	m_pField->Uninit();
	delete m_pField;
}

void CFloor::Update()
{
}

void CFloor::Draw()
{
	SetWorldMatrix();

	
	RENDERER::GetDeviceContext()->VSSetShader(m_pCommonVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);

	m_pField->Draw();

	
	if (m_EnableCollision)
		m_Collision.Draw();
	
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

		ImGui::End();
	}
}