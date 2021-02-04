/*---------------------------------------
*�@wall.cpp
*
*@author�FOkahara Taiki
----------------------------------------*/
#include "director.h"
#include "renderer.h"
#include "wall.h"
#include "Imgui11.h"
#include "input.h"

void CWall::Init()
{
	
	m_pWall = new Tile();
	m_pWall->Init("Wall//T_StoneWall_A.dds", "Wall//T_StoneWall_N.dds", "Wall//T_StoneWall_C.dds", 2,2, 2.5f,NULL);

	m_Collision.Set(D3DXVECTOR3(5.5f, 0.5f, 5.5f), D3DXVECTOR3(0, 0, 0));


	m_Transform.rotation = D3DXVECTOR3(D3DX_PI/2,D3DX_PI/2, 0.0f);

	
	//�V�F�[�_�[�쐬
	RENDERER::CreatePixelShader(&m_pPixelShader, "tilePS.cso");

			
	D3DXVECTOR3 scale;
	scale = m_Transform.scale;

	D3DXVECTOR3 frontRot, backRot, leftRot, rightRot,topRot,downRot;
	frontRot = m_Transform.rotation + D3DXVECTOR3(0,-D3DX_PI/2,0);
	backRot = m_Transform.rotation + D3DXVECTOR3(0,D3DX_PI/2,0);
	leftRot = m_Transform.rotation + D3DXVECTOR3(0,D3DX_PI,0);
	rightRot = m_Transform.rotation + D3DXVECTOR3(0,0,0);
	topRot = m_Transform.rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);
	downRot = m_Transform.rotation + D3DXVECTOR3(-D3DX_PI / 2, 0, 0);


	{//�X�^�[�g�n�_�͂���

		{//�X�^�[�g�n�_��

			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(2.5f, 2.5f, -5.0f),backRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f, 2.5f, -5.0f),backRot, scale });
		}	
		{//�X�^�[�g�n�_����

			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(5.0f, 2.5f, -7.5f),leftRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f, 2.5f, -7.5f),rightRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(5.0f, 2.5f, -12.5f),leftRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f, 2.5f, -12.5f),rightRot, scale });
		}
		{//�X�^�[�g�n�_����

			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(2.5f, 2.5f, -15.0f),frontRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f, 2.5f, -15.0f),frontRot, scale });
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f, 2.5f, -15.0f),frontRot, scale });
		}		
	}
	
	//���̕�
	for (int i = 0; i < 8; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0, 2.5f, -2.5f + i * 5.0f),leftRot, scale });
	}
	for (int i = 0; i < 7; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-5.0f, 2.5f, -2.5f + i * 5.0f),rightRot, scale });
	}
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f,2.5f,30.0f),frontRot,scale });


	//���̕�
	for (int i = 0; i < 8; i++)
	{
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0.0f - i * 5.0f, 2.5f, 35.0f),backRot, scale });
	}

	//�K�i�O
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(- 5.25f, 7.0f, 32.5f),leftRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f, 8.5f, 32.5f),downRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-7.5f, 8.5f, 32.5f),downRot, scale });
	//�K�i�e
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(- 7.5f, 7.5f, 35.0f),backRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f, 7.5f, 35.0f),backRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(- 7.5f, 2.5f, 30.0f),frontRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f, 2.5f, 30.0f),frontRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(- 7.5f, 7.5f, 30.0f),frontRot, scale });
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-10.0f, 7.5f, 30.0f),frontRot, scale });

	//�K�i�O
	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-15.5f, 4.0f, 32.5f),topRot, scale });


	InitInstance();
	
}

void CWall::Uninit()
{
	m_pWall->Uninit();
	delete m_pWall;

	UninitInstance();
	SAFE_RELEASE(m_pPixelShader);
}

void CWall::Update()
{
	UpdateInstance();
}

void CWall::Draw()
{
	DrawInstance();
	
	RENDERER::GetDeviceContext()->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pCommonVertexLayout);

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