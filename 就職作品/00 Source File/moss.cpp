/*---------------------------------------
*@moss.cpp
*
*@authorFOkahara Taiki
----------------------------------------*/
#include "director.h"
#include "renderer.h"
#include "moss.h"
#include "Imgui11.h"
#include "input.h"


void CMoss::Init()
{
	
	m_pField = new Tile();
	m_pField->Init("Moss//TexturesCom_Snow_Fresh_512_albedo.dds", "Moss//TexturesCom_Snow_Fresh_512_normal.dds", "Moss//TexturesCom_Snow_Fresh_512_roughness.dds",100,100,1.0f,NULL);
	
	m_Collision.Set(D3DXVECTOR3(100.0f, 1.0f, 100.0f), D3DXVECTOR3(0, 0, 0));
}

void CMoss::Uninit()
{
	m_pField->Uninit();
	delete m_pField;
}

void CMoss::Update()
{
}

void CMoss::Draw()
{

	SetWorldMatrix();

	RENDERER::GetDeviceContext()->VSSetShader(m_pCommonVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);

	
	m_pField->Draw();

	if (m_EnableCollision)
		m_Collision.Draw();
}

void CMoss::Imgui()
{
}