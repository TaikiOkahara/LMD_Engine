/*「CAMERA.cpp」=============================================
　　製作者：岡原大起　	(-"-)
=============================================================*/
#include "base.h"
#include "director.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "wall.h"
#include "pillar.h"
#include "Imgui11.h"
#include "calculation.h"
#include "ceiling.h"
#include "floor.h"

//　初期化
void CCamera::Init()
{
	
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

	CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>();

	
	m_OffsetPosition = player->GetPosition();
	m_OffsetPosition.y = 0.0f;

	m_Target = player->GetPosition() +D3DXVECTOR3(0, 1, 0);


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


	//D3DXVECTOR3 pos;
	m_Transform.position = offset + m_OffsetPosition;




	//カメラとプレイヤーの間に障害物があった場合、カメラ位置を移動させる
	if (m_CameraControl)
	{
		CWall* wall = Base::GetScene()->GetGameObject<CWall>();
		if (wall != NULL)
		{
			m_Transform.position = CameraRayIntersect(wall);
		}
		CPillar* pillar = Base::GetScene()->GetGameObject<CPillar>();
		if (pillar != NULL)
		{
			m_Transform.position = CameraRayIntersect(pillar);
		}
		CCeiling* ceiling = Base::GetScene()->GetGameObject<CCeiling>();
		if (ceiling != NULL)
		{
			m_Transform.position = CameraRayIntersect(ceiling);
		}
		/*CFloor* floor = Base::GetScene()->GetGameObject<CFloor>();
		if (floor != NULL)
		{
			m_Transform.position = CameraRayIntersect(floor);
		}*/

		
	}


	//　マトリクス設定

	D3DXMATRIX viewOldMatrix;
	D3DXMATRIX projOldMatrix;

	viewOldMatrix = m_ViewMatrix;
	projOldMatrix = m_ProjMatrix;


	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Transform.position, &m_Target, &D3DXVECTOR3(0, 1, 0));
	//RENDERER::SetViewMatrix(m_ViewMatrix);
	RENDERER::m_ConstantBufferList.GetStruct<ViewBuffer>()->Set(m_ViewMatrix,viewOldMatrix);

	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_Angle, m_Aspect, m_Near, m_Far);
	RENDERER::m_ConstantBufferList.GetStruct<ProjBuffer>()->Set(m_ProjMatrix,projOldMatrix);


	EYE eye;
	eye.eyePos = D3DXVECTOR4(m_Transform.position.x, m_Transform.position.y, m_Transform.position.z, 0);
	
	eye.worldCamera[0] = m_CullingWPos[0];
	eye.worldCamera[1] = m_CullingWPos[1];
	eye.worldCamera[2] = m_CullingWPos[2];
	eye.worldCamera[3] = m_CullingWPos[3];
	
	
	RENDERER::m_ConstantBufferList.GetStruct<EyeBuffer>()->Set(eye);

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
//
//
//
void CCamera::Draw()
{
}




D3DXVECTOR3 CCamera::CameraRayIntersect(CInstanceGameObject* object)
{

	CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>();


	D3DXVECTOR3 playerPos = player->GetPosition() + D3DXVECTOR3(0, 1, 0);//プレイヤーの中心
	D3DXVECTOR3 cameraPos = m_Transform.position;
	D3DXVECTOR3 origin = m_Transform.position;
	D3DXVECTOR3 ray = playerPos - cameraPos;
	D3DXVec3Normalize(&ray, &ray);

	D3DXVECTOR3 returnPos = m_Transform.position;

	D3DXVECTOR3 offsetVertex[8];

	//オフセットを取得
	for (int i = 0; i < 8; i++)
	{
		offsetVertex[i] = object->GetCollision()->GetVertex()[i];
	}

	int count = object->GetMeshMax();
	for (int objnum = 0; objnum < count; objnum++)
	{
		D3DXVECTOR3 vertex[8];
		//マトリクス
		D3DXMATRIX mWorld, mScale, mRot, mTrans;
		D3DXMatrixScaling(&mScale, object->GetScale(objnum).x, object->GetScale(objnum).y, object->GetScale(objnum).z);
		D3DXMatrixRotationYawPitchRoll(&mRot, object->GetRotation(objnum).y, object->GetRotation(objnum).x, object->GetRotation(objnum).z);
		D3DXMatrixTranslation(&mTrans, object->GetPosition(objnum).x, object->GetPosition(objnum).y, object->GetPosition(objnum).z);
		mWorld = mScale * mRot * mTrans;

		//頂点をワールド座標変換
		for (int i = 0; i < 8; i++)
		{
			vertex[i] = offsetVertex[i];
			D3DXVec3TransformCoord(&vertex[i], &vertex[i], &mWorld);
		}
		//平面を生成
		D3DXPLANE plane[6];

		D3DXPlaneFromPoints(&plane[0], &vertex[0], &vertex[1], &vertex[2]);
		D3DXPlaneFromPoints(&plane[1], &vertex[5], &vertex[4], &vertex[7]);
		D3DXPlaneFromPoints(&plane[2], &vertex[4], &vertex[5], &vertex[1]);
		D3DXPlaneFromPoints(&plane[3], &vertex[6], &vertex[7], &vertex[3]);
		D3DXPlaneFromPoints(&plane[4], &vertex[4], &vertex[0], &vertex[3]);
		D3DXPlaneFromPoints(&plane[5], &vertex[1], &vertex[5], &vertex[6]);

		

		//平面との衝突を取得
		for (int face = 0; face < 6; face++)
		{

			//面の法線側にプレイヤーがいない場合は省く
			float nor = D3DXPlaneDotCoord(&plane[face], &playerPos);
			if (nor < 0) continue;

			D3DXVECTOR3 hit;
			D3DXPlaneIntersectLine(&hit, &plane[face], &cameraPos, &playerPos);

			if (hit != NULL)
			{

				D3DXVECTOR3	v1, v2, v3, v4, v5, v6;
				switch (face)
				{
				case 0:

					v1 = vertex[0];
					v2 = vertex[1];
					v3 = vertex[2];
						 
					v4 = vertex[0];
					v5 = vertex[2];
					v6 = vertex[3];
						 
					break;
				case 1:	 
						 
					v1 = vertex[5];
					v2 = vertex[4];
					v3 = vertex[7];
						 
					v4 = vertex[5];
					v5 = vertex[7];
					v6 = vertex[6];
						 
					break;
				case 2:	 
						 
					v1 = vertex[4];
					v2 = vertex[5];
					v3 = vertex[1];
						 
					v4 = vertex[4];
					v5 = vertex[1];
					v6 = vertex[0];
						 
					break;
				case 3:	 
						 
					v1 = vertex[3];
					v2 = vertex[2];
					v3 = vertex[6];
						 
					v4 = vertex[3];
					v5 = vertex[6];
					v6 = vertex[7];
						 
					break;
				case 4:	 
						 
					v1 = vertex[4];
					v2 = vertex[0];
					v3 = vertex[3];
						 
					v4 = vertex[4];
					v5 = vertex[3];
					v6 = vertex[7];
						 
					break;
				case 5:	 
						 
					v1 = vertex[1];
					v2 = vertex[5];
					v3 = vertex[6];
						 
					v4 = vertex[1];
					v5 = vertex[6];
					v6 = vertex[2];

					break;
				default:

					break;
				}

				D3DXVECTOR3 edge1, edge2;

				edge1 = v2 - v1;
				edge2 = v3 - v1;


				if (CulcRay(origin, ray, v1, edge1, edge2))
				{

					D3DXVECTOR3 hitdistance = playerPos - hit;
					D3DXVECTOR3 returndistance = playerPos - returnPos;
					if (D3DXVec3Length(&returndistance) > D3DXVec3Length(&hitdistance))
					{
						returnPos = hit;
						continue;
					}

					
				}
				
				edge1 = v5 - v4;
				edge2 = v6 - v4;

				if (CulcRay(origin, ray, v4, edge1, edge2))
				{
					D3DXVECTOR3 hitdistance = playerPos - hit;
					D3DXVECTOR3 returndistance = playerPos - returnPos;
					if (D3DXVec3Length(&returndistance) > D3DXVec3Length(&hitdistance))
					{
						returnPos = hit;
						continue;
					}
				}
				

			}

		}

	}
	return returnPos;
	
}

void CCamera::Imgui()
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

		/*if(hit)
			ImGui::InputFloat3("hit!!", hitpos,1);*/
		ImGui::Checkbox("CameraControl", &m_CameraControl);


		ImGui::InputFloat3("Position", m_Transform.position, 1);
		ImGui::InputFloat3("Rotation", m_Transform.rotation, 1);
		ImGui::InputFloat3("Target", m_Target, 1);
		ImGui::InputFloat3("Distance", m_Distance, 1);

		ImGui::End();
	}
}