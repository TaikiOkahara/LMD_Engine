/*---------------------------------------
*　demoCame.cpp
*
*@author：Okahara Taiki
----------------------------------------*/
#include "base.h"
#include "director.h"
#include "renderer.h"
#include "demoCamera.h"
#include "input.h"
//#include "player.h"
//#include "wall.h"
//#include "pillar.h"
#include "Imgui11.h"
#include "calculation.h"
//#include "ceiling.h"
#include "floor.h"


void CDemoCamera::Init()
{
	
	m_Distance = D3DXVECTOR3(0, 1.5, -5);

	m_Angle = D3DX_PI / 4;
	m_Aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	m_Near = 0.1f;
	m_Far = 1000.0f;
}


void CDemoCamera::Uninit()
{
}

void CDemoCamera::Update()
{

	/*CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>();*/

	
	m_OffsetPosition = D3DXVECTOR3(0, 0, 0);//player->GetPosition();

	if (CInput::KeyPress(DIK_A)) {
		m_Transform.position.x -= 1.0;
	}
	else if (CInput::KeyPress(DIK_D)) {
		m_Transform.position.x += 1.0;
	}
	else if (CInput::KeyPress(DIK_W)) {
		m_Transform.position.z += 1.0;
	}
	else if (CInput::KeyPress(DIK_S)) {
		m_Transform.position.z -= 1.0;
	}

	m_Target = m_Transform.position + D3DXVECTOR3(0,-1,0);// m_OffsetPosition + D3DXVECTOR3(0, 1, 0);


	if (CInput::KeyPress(DIK_LEFTARROW)){	
		m_Transform.rotation.y -= 0.05f;
	}
	else if (CInput::KeyPress(DIK_RIGHTARROW)) {
		m_Transform.rotation.y += 0.05f;
	}

	if (CInput::KeyPress(DIK_UPARROW)) {
		m_Transform.rotation.x -= 0.05f;
	}
	else if (CInput::KeyPress(DIK_DOWNARROW)) {
		m_Transform.rotation.x += 0.05f;
		if (m_Transform.rotation.x >= (D3DX_PI /180) * 70)
			m_Transform.rotation.x = (D3DX_PI / 180) * 70;
	}

	//注支店との距離更新
	if (CInput::KeyPress(DIK_Q)) {
		m_Distance.z -= 0.05f;
	}
	else if (CInput::KeyPress(DIK_E)) {
		m_Distance.z += 0.05f;
	}
	

	D3DXVECTOR3 offset = m_Distance;

	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y, m_Transform.rotation.x, m_Transform.rotation.z);
	D3DXVec3TransformCoord(&offset, &offset, &rot);



	m_Transform.position = offset + m_OffsetPosition;




	//カメラとプレイヤーの間に障害物があった場合、カメラ位置を移動させる
	/*if (m_CameraControl)
	{
		CFloor* floor = Base::GetScene()->GetGameObject<CFloor>();
		if (floor != NULL)
			m_Transform.position = CameraRayIntersect(floor);
	}*/



	D3DXMATRIX viewOldMatrix;
	D3DXMATRIX projOldMatrix;

	viewOldMatrix = m_ViewMatrix;
	projOldMatrix = m_ProjMatrix;


	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Transform.position, &m_Target, &D3DXVECTOR3(0, 1, 0));
	RENDERER::GetConstantList().GetStruct<ViewBuffer>()->Set(m_ViewMatrix,viewOldMatrix);

	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_Angle, m_Aspect, m_Near, m_Far);
	RENDERER::GetConstantList().GetStruct<ProjBuffer>()->Set(m_ProjMatrix,projOldMatrix);


	EYE eye;
	eye.eyePos = D3DXVECTOR4(m_Transform.position.x, m_Transform.position.y, m_Transform.position.z, 0);
	eye.worldCamera[0] = m_CullingWPos[0];
	eye.worldCamera[1] = m_CullingWPos[1];
	eye.worldCamera[2] = m_CullingWPos[2];
	eye.worldCamera[3] = m_CullingWPos[3];
	
	
	RENDERER::GetConstantList().GetStruct<EyeBuffer>()->Set(eye);



	//視錐台カリング用視錐台頂点計算
	D3DXMATRIX vp, invvp;

	vp = m_ViewMatrix * m_ProjMatrix;

	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(-1.0f,-1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3( 1.0f,-1.0f, 1.0f);
	
	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	m_CullingWPos[0] = D3DXVECTOR4(wpos[0].x,wpos[0].y,wpos[0].z,0);
	m_CullingWPos[1] = D3DXVECTOR4(wpos[1].x,wpos[1].y,wpos[1].z,0);
	m_CullingWPos[2] = D3DXVECTOR4(wpos[2].x,wpos[2].y,wpos[2].z,0);
	m_CullingWPos[3] = D3DXVECTOR4(wpos[3].x,wpos[3].y,wpos[3].z,0);
}


void CDemoCamera::Draw()
{
}


void CDemoCamera::Imgui()
{
	static bool show = true;

	

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Camera", &lw_is_open, lw_flag);

		ImGui::Checkbox("CameraControl", &m_CameraControl);


		ImGui::InputFloat3("Position", m_Transform.position, 1);
		ImGui::InputFloat3("Rotation", m_Transform.rotation, 1);
		ImGui::InputFloat3("Target", m_Target, 1);
		ImGui::InputFloat3("Distance", m_Distance, 1);

		ImGui::End();
	}
}