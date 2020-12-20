#include "director.h"
#include "renderer.h"
#include "wall.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CWall::Init()
{
	
	m_pWall = new Tile();
	m_pWall->Init("Wall//T_StoneWall_A.BMP", "Wall//T_StoneWall_N.BMP", "Wall//T_StoneWall_C.BMP", 2,2, 2.5f);

	m_Collision.Init(D3DXVECTOR3(5.5f, 0.5f, 5.5f), D3DXVECTOR3(0, 0, 0));


	m_Transform.rotation = D3DXVECTOR3(D3DX_PI/2,D3DX_PI/2, 0.0f);

	
	//シェーダー作成
	RENDERER::CreatePixelShader(&m_pPixelShader, "tilePS.cso");
	
	
	


			
	D3DXVECTOR3 scale;
	scale = m_Transform.scale;


	D3DXVECTOR3 forwardRot, backRot, leftRot, rightRot,topRot;

	forwardRot = m_Transform.rotation + D3DXVECTOR3(0,-D3DX_PI/2,0);
	backRot = m_Transform.rotation + D3DXVECTOR3(0,D3DX_PI/2,0);
	leftRot = m_Transform.rotation + D3DXVECTOR3(0,D3DX_PI,0);
	rightRot = m_Transform.rotation + D3DXVECTOR3(0,0,0);
	topRot = m_Transform.rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);


	{//スタート地点囲い壁

		{//スタート地点壁

			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(2.5f, 2.5f, -5.0f),backRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f, 2.5f, -5.0f),backRot, scale });
		}	
		{//スタート地点横壁

			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(5.0f, 2.5f, -7.5f),leftRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f, 2.5f, -7.5f),rightRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(5.0f, 2.5f, -12.5f),leftRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f, 2.5f, -12.5f),rightRot, scale });
		}
		{//スタート地点後ろ壁

			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(2.5f, 2.5f, -15.0f),forwardRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f, 2.5f, -15.0f),forwardRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f, 2.5f, -15.0f),forwardRot, scale });	
		}
		{//天井
			
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(2.5f, 5.0f, -12.5f),topRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(2.5f, 5.0f, -7.5f),topRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f, 5.0f, -12.5f),topRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f, 5.0f, -7.5f),topRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f, 5.0f, -12.5f),topRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f, 5.0f, -7.5f),topRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f, 5.0f, -2.5f),topRot, scale });
		}
		
	}
	
	//横の壁
	for (int i = 0; i < 8; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0, 2.5f, -2.5f + i * 5.0f),leftRot, scale });
	}
	for (int i = 0; i < 7; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-5.0f, 2.5f, -2.5f + i * 5.0f),rightRot, scale });
	}
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f,2.5f,30.0f),forwardRot,scale });


	//奥の壁
	for (int i = 0; i < 8; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0.0f - i * 5.0f, 2.5f, 35.0f),backRot, scale });
	}



	InitInstance();
	
}
//
//
//
void CWall::Uninit()
{
	m_pWall->Uninit();
	delete m_pWall;

	UninitInstance();
	m_Collision.Uninit();
	SAFE_RELEASE(m_pPixelShader);
}
//
//
//
void CWall::Update()
{
	UpdateInstance();
}
//
//
//
void CWall::Draw()
{
	DrawInstance();
	

	//RENDERER::m_pDeviceContext->VSSetShader(RENDERER::m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pCommonVertexLayout);

	m_pWall->DrawInstanced(m_MeshCount);

	if(m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CWall::Imgui()
{
	static bool show = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Wall", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);

		ImGui::End();
	}
}