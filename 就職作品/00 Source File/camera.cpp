/*「CAMERA.cpp」=============================================
　　製作者：岡原大起　	(-"-)
=============================================================*/
#include "base.h"
#include "director.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "Imgui11.h"


//　初期化
void CCamera::Init()
{
	D3DXVECTOR3 Eye(0, 1, -4);
	D3DXVECTOR3 LookAt(0, -2, 0);

	m_Rotation = D3DXVECTOR3(0, 0, 0);
	m_Distance = D3DXVECTOR3(0, 1.5, -5);

	m_Angle = D3DX_PI / 4;
	m_Aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	m_Near = 0.1f;
	m_Far = 1000.0f;
}
//
//
//
void CCamera::Uninit()
{
}
//
//
// 更新
void CCamera::Update()
{

	CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>(1);

	
	m_OffsetPosition = player->GetPosition();
	m_OffsetPosition.y = 0.0f;

	m_Target = player->GetPosition() + D3DXVECTOR3(0,2,0);


	if (Keyboard_IsPress(DIK_LEFTARROW)){	
		m_Rotation.y -= 0.05f;
	}
	else if (Keyboard_IsPress(DIK_RIGHTARROW)) {
		m_Rotation.y += 0.05f;
	}

	if (Keyboard_IsPress(DIK_UPARROW)) {
		m_Rotation.x -= 0.05f;
	}
	else if (Keyboard_IsPress(DIK_DOWNARROW)) {
		m_Rotation.x += 0.05f;
	}

	if (Keyboard_IsPress(DIK_Q)) {
		m_Distance.z -= 0.05f;
	}
	else if (Keyboard_IsPress(DIK_E)) {
		m_Distance.z += 0.05f;
	}
	
	//カメラとプレイヤーの間に障害物があった場合、カメラ位置を移動させる
}
//
//
//
void CCamera::Draw()
{
	//　マトリクス設定


	D3DXVECTOR3 offset = m_Distance;

	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXVec3TransformCoord(&offset, &offset, &rot);

	
	//D3DXVECTOR3 pos;
	m_Position = offset + m_OffsetPosition;

	D3DXMATRIX ViewMatrix;
	D3DXMATRIX ProjectionMatrix;
	D3DXMatrixLookAtLH(&ViewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0, 1, 0));
	RENDERER::SetViewMatrix(ViewMatrix);


	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix,m_Angle,m_Aspect,m_Near,m_Far);
	RENDERER::SetProjectionMatrix(ProjectionMatrix);

	EYE eye;
	eye.Eye = D3DXVECTOR4(m_Position.x, m_Position.y, m_Position.z, 0);
	RENDERER::SetEye(eye);
}

void CCamera::GetDistance(D3DXVECTOR3 RayStart, D3DXVECTOR3 RayEnd)
{
	bool hit = false;
	float dist = 0xffffffff;
	D3DXVECTOR3 direction = RayEnd - RayStart;
	D3DXVec3Normalize(&direction,&direction);

	
	//D3DXIntersect()

}


void CCamera::Imgui()
{
	static bool show_camera_winow = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (Keyboard_IsTrigger(DIK_F1))
		show_camera_winow = !show_camera_winow;

	if (show_camera_winow)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Camera", &lw_is_open, lw_flag);


		
		ImGui::InputFloat3("Position", m_Position, 1);
		ImGui::InputFloat3("Rotation", m_Rotation, 1);
		ImGui::InputFloat("Target", m_Target, 1);
		ImGui::InputFloat3("Distance", m_Distance, 1);

		ImGui::End();
	}
}