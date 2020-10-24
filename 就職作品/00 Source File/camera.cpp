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
#include "makeTangent.h"

//　初期化
void CCamera::Init()
{
	D3DXVECTOR3 Eye(0, 1, -4);
	D3DXVECTOR3 LookAt(0, -2, 0);

	m_Position = D3DXVECTOR3(0, 0, 0);
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

	m_Target = player->GetPosition() +D3DXVECTOR3(0, 1, 0);


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
		if (m_Rotation.x >= (D3DX_PI /180) * 70)
			m_Rotation.x = (D3DX_PI / 180) * 70;
	}

	if (Keyboard_IsPress(DIK_Q)) {
		m_Distance.z -= 0.05f;
	}
	else if (Keyboard_IsPress(DIK_E)) {
		m_Distance.z += 0.05f;
	}
	

	D3DXVECTOR3 offset = m_Distance;

	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXVec3TransformCoord(&offset, &offset, &rot);


	//D3DXVECTOR3 pos;
	m_Position = offset + m_OffsetPosition;



	//カメラとプレイヤーの間に障害物があった場合、カメラ位置を移動させる
	CWall* wall = Base::GetScene()->GetGameObject<CWall>(1);
	m_Position = RayIntersect(wall);
	CPillar* pillar = Base::GetScene()->GetGameObject<CPillar>(1);
	m_Position = RayIntersect(pillar);
}
//
//
//
void CCamera::Draw()
{
	//　マトリクス設定


	

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


float Det(D3DXVECTOR3 vector0, D3DXVECTOR3 vector1, D3DXVECTOR3 vector2)
{
	return (
		    (vector0.x * vector1.y * vector2.z)
		  + (vector0.y * vector1.z * vector2.x)
		  + (vector0.z * vector1.x * vector2.y)
		  - (vector0.x * vector1.z * vector2.y)
		  - (vector0.y * vector1.x * vector2.z)
		  - (vector0.z * vector1.y * vector2.x));
}

bool CulcRay(D3DXVECTOR3 origin,D3DXVECTOR3 ray,D3DXVECTOR3 v0,D3DXVECTOR3 edge1, D3DXVECTOR3 edge2) 
{
	float denominator = Det(edge1, edge2, ray);

	// レイが平面と平行でないかチェック
	if (denominator <= 0) {
		return false;
	}

	D3DXVECTOR3 d = origin - v0;

	float u = Det(d, edge2, ray) / denominator;

	if ((u >= 0) && (u <= 1))
	{
		float v = Det(edge1, d, ray) / denominator;
		if ((v >= 0) && (u + v <= 1))
		{
			float t = Det(edge1, edge2, d) / denominator;

			return true;
		}

	}
	return false;
}


D3DXVECTOR3 CCamera::RayIntersect(CInstanceGameObject* object)
{

	CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>(1);


	D3DXVECTOR3 playerPos = player->GetPosition() + D3DXVECTOR3(0, 1, 0);//プレイヤーの中心
	D3DXVECTOR3 cameraPos = m_Position;
	D3DXVECTOR3 origin = m_Position;
	D3DXVECTOR3 ray = playerPos - cameraPos;
	D3DXVec3Normalize(&ray, &ray);

	D3DXVECTOR3 returnPos = m_Position;

	D3DXVECTOR3 offsetVertex[8];

	//オフセットを取得
	for (int i = 0; i < 8; i++)
	{
		offsetVertex[i] = object->GetCollision()->GetVertex()[i];
	}

	int count = object->GetMeshCount();
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
	static bool show_camera_winow = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (Keyboard_IsTrigger(DIK_F1))
		show_camera_winow = !show_camera_winow;

	if (show_camera_winow)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Camera", &lw_is_open, lw_flag);

		/*if(hit)
			ImGui::InputFloat3("hit!!", hitpos,1);*/
		
		ImGui::InputFloat3("Position", m_Position, 1);
		ImGui::InputFloat3("Rotation", m_Rotation, 1);
		ImGui::InputFloat3("Target", m_Target, 1);
		ImGui::InputFloat3("Distance", m_Distance, 1);

		ImGui::End();
	}
}