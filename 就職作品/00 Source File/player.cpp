#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"

#include "camera.h"
#include "wall.h"
#include "pillar.h"
#include "player.h"
#include <typeinfo>
#include "calculation.h"


//
//
//
void CPlayer::Init()
{
	
	m_AnimModel = new CAnimationModel();

	m_AnimModel->LoadModel("../02 Visual File//Akai//Akai_Idle.fbx", D3DXVECTOR3(0,2,0));
	m_AnimModel->LoadTexture("../02 Visual File//Akai//texture.ini");
	m_AnimModel->LoadAnimation("../02 Visual File//Akai//Akai_Run.fbx", "Run");
	m_AnimModel->LoadAnimation("../02 Visual File//Akai//Akai_Idle.fbx", "Idle");

	m_Position = D3DXVECTOR3(-2.5f, 0.01f, -3.5f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.008f, 0.008f, 0.008f);
	//m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEWEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_VertexShader,&m_VertexLayout,layout,7,"SkeletalVertexShader.cso");
	RENDERER::CreatePixelShader(&m_PixelShader, "SkeletalPixelShader.cso");

	D3DXQuaternionIdentity(&m_Quaternion);
	m_OldPosition = m_Position;
	m_OldForward = GetForward();
	m_OldForward.y = 0;

}
//
//
//
void CPlayer::Uninit()
{

	m_AnimModel->Unload();
	delete m_AnimModel;
}
//
//
//
void CPlayer::Update()
{


	CCamera* camera = Base::GetScene()->GetGameObject<CCamera>(0);

	D3DXVECTOR3 cameraright = camera->GetRight();
	cameraright.y = 0;
	D3DXVECTOR3 cameraforward = camera->GetForward();
	cameraforward.y = 0;
	D3DXVECTOR3 camerarotation = camera->GetRotation();
	camerarotation.x = 0;
	camerarotation.z = 0;
	



	D3DXVECTOR3 position(0,0,0);
	float rotation = 0;

	if (Keyboard_IsPress(DIK_W))
	{
		position += cameraforward;
		rotation = camerarotation.y + 2 * D3DX_PI;
		if (Keyboard_IsPress(DIK_A)){
			position -= cameraright;
			rotation = camerarotation.y + 2 * D3DX_PI - D3DX_PI/4;
		}
		else if (Keyboard_IsPress(DIK_D)) {
			position += cameraright;
			rotation = camerarotation.y + 2 * D3DX_PI + D3DX_PI / 4;
		}
	}
	else if (Keyboard_IsPress(DIK_S))
	{
		position -= cameraforward;
		rotation = camerarotation.y + D3DX_PI;
		if (Keyboard_IsPress(DIK_A)) {
			position -= cameraright;
			rotation = camerarotation.y + D3DX_PI + D3DX_PI / 4;
		}
		else if (Keyboard_IsPress(DIK_D)) {
			position += cameraright;
			rotation = camerarotation.y + D3DX_PI - D3DX_PI / 4;
		}
	}
	else if (Keyboard_IsPress(DIK_A))
	{
		position -= cameraright;
		rotation = camerarotation.y -D3DX_PI/2;
	}
	else if (Keyboard_IsPress(DIK_D))
	{
		position += cameraright;
		rotation = camerarotation.y + D3DX_PI/2;
	}
	

	D3DXVec3Normalize(&position, &position);
	m_Position += position * 0.1f;
	
	if (rotation != 0){
		m_Rotation.y = rotation;
	}
	
	//壁との当たり判定
	CWall* wall = Base::GetScene()->GetGameObject<CWall>(1);
	m_hit = LenOBBToPoint(*wall,m_Position,3.0f);
	CPillar* pillar = Base::GetScene()->GetGameObject<CPillar>(1);
	m_hit = LenOBBToPoint(*pillar,m_Position,1.5f);

	
	


	if (Keyboard_IsPress(DIK_W) || 
		Keyboard_IsPress(DIK_A) ||
		Keyboard_IsPress(DIK_S) ||
		Keyboard_IsPress(DIK_D))
		m_AnimModel->Update("Run", m_Frame);
	else
		m_AnimModel->Update("Idle", m_Frame);

	

	m_OldPosition = m_Position;
	m_Frame++;
}
//
//
//
void CPlayer::Draw()
{
	//　マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	//影
	/*D3DXMATRIX shadow;
	D3DXPLANE plane;
	D3DXPlaneFromPointNormal(&plane, &D3DXVECTOR3(0, 0.01f, 0), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixShadow(&shadow, &D3DXVECTOR4(-5, 5, 0, 1), &plane);
	world = world * shadow;*/

	RENDERER::SetWorldMatrix(world);

	RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_VertexLayout);


	m_AnimModel->Draw();

}

//インスタンスオブジェクト専用
//3次元OBBと点の最短距離算出関数
FLOAT CPlayer:: LenOBBToPoint(CInstanceGameObject& obj, D3DXVECTOR3& p,float length)
{
	D3DXVECTOR3 Vec(0, 0, 0);   // 最終的に長さを求めるベクトル

	//最初は1個から
	for(int i = 0;i < obj.GetMeshCount();i++)
	{
		//プレイヤーの半径4.0f以内にいないものは排除する
		float len = D3DXVec3Length(&D3DXVECTOR3(obj.GetPosition(i) - m_Position));
		if (len > length)
		{
			continue;
		}









		D3DXVECTOR3 pos = obj.GetPosition(i) + obj.GetCollision()->GetPosition();

		FLOAT L = obj.GetCollision()->GetXsize();
		if (L > 0)// L=0は計算できない
		{
			
			D3DXVECTOR3 rot = obj.GetRight(i);
			FLOAT s = D3DXVec3Dot(&(p - pos), &rot) / L;

			// sの値から、はみ出した部分があればそのベクトルを加算
			s = fabs(s);
			if (s > 1)
				Vec += (1 - s) * L * rot;   // はみ出した部分のベクトル算出

			
		}
		L = obj.GetCollision()->GetYsize();
		if (L > 0)// L=0は計算できない
		{
			D3DXVECTOR3 rot = obj.GetUp(i);
			FLOAT s = D3DXVec3Dot(&(p - pos), &rot) / L;

			// sの値から、はみ出した部分があればそのベクトルを加算
			s = fabs(s);
			if (s > 1)
				Vec += (1 - s) * L * rot;   // はみ出した部分のベクトル算出


		}
		L = obj.GetCollision()->GetZsize();
		if (L > 0)// L=0は計算できない
		{
			D3DXVECTOR3 rot = obj.GetForward(i);
			FLOAT s = D3DXVec3Dot(&(p - pos), &rot) / L;

			// sの値から、はみ出した部分があればそのベクトルを加算
			s = fabs(s);
			if (s > 1)
				Vec += (1 - s) * L * rot;   // はみ出した部分のベクトル算出

		}

		if (D3DXVec3Length(&Vec) <= 0)
		{
			m_Position = RayIntersect(&obj, i);
		}
		else
		{
			Vec = D3DXVECTOR3(0, 0, 0);
		}
	}
	



	return D3DXVec3Length(&Vec);   // 長さを出力
}

//壁に平行なベクトルを計算する関数
D3DXVECTOR3 CPlayer::RayIntersect(CInstanceGameObject* object, int index)
{

	D3DXVECTOR3 playerPos = m_Position;//プレイヤー
	D3DXVECTOR3 oldPlayerPos = m_OldPosition;//オブジェクトの中心
	D3DXVECTOR3 origin = m_OldPosition;//レイを飛ばす原点
	D3DXVECTOR3 ray = playerPos - oldPlayerPos;
	D3DXVec3Normalize(&ray, &ray);

	D3DXVECTOR3 returnPos = m_OldPosition;
	D3DXVECTOR3 returnNormal;//壁ずりに使う

	//D3DXVECTOR3 offsetVertex[8];

	D3DXVECTOR3 vertex[8];
	D3DXVECTOR3 normal[6];

	//オフセットを取得
	for (int i = 0; i < 8; i++)
	{
		vertex[i] = object->GetCollision()->GetVertex()[i];
	}
	for (int i = 0; i < 6; i++)
	{
		normal[i] = object->GetCollision()->GetNormal()[i];
	}

	{

		//マトリクス
		D3DXMATRIX mWorld, mScale, mRot, mTrans;
		D3DXMatrixScaling(&mScale, object->GetScale(index).x, object->GetScale(index).y, object->GetScale(index).z);
		D3DXMatrixRotationYawPitchRoll(&mRot, object->GetRotation(index).y, object->GetRotation(index).x, object->GetRotation(index).z);
		D3DXMatrixTranslation(&mTrans, object->GetPosition(index).x, object->GetPosition(index).y, object->GetPosition(index).z);
		mWorld = mScale * mRot * mTrans;

		//頂点をワールド座標変換
		for (int i = 0; i < 8; i++)
		{
			D3DXVec3TransformCoord(&vertex[i], &vertex[i], &mWorld);
		}
		//法線をワールド座標変換
		for (int i = 0; i < 6; i++)
		{
			D3DXVec3TransformCoord(&normal[i], &normal[i], &mRot);
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

			//面の逆法線側にプレイヤーがいない場合は省く
			//float nor = D3DXPlaneDotCoord(&plane[face], &oldPlayerPos);
			//if (nor >= 0)
			//{
			//	

			//}
			float nor = D3DXPlaneDotCoord(&plane[face], &oldPlayerPos);
			float nor2 = D3DXPlaneDotCoord(&plane[face], &playerPos);
			if (nor >= -0.01f)//ちょっと甘め
			{
				if (nor2 < 0)
				{
					D3DXVECTOR3  hit;
					bool  boolHit;
					boolHit = D3DXPlaneIntersectLine(&hit, &plane[face], &oldPlayerPos, &playerPos);

					if (boolHit != NULL)
					{
						//交点から、壁ずり判定を行う
						D3DXVECTOR3 f = D3DXVECTOR3(playerPos - oldPlayerPos);
						D3DXVECTOR3 out = (f - D3DXVec3Dot(&f, &normal[face]) * normal[face]);
						//D3DXVec3Normalize(&out, &(f - D3DXVec3Dot(&f, &normal[face]) * normal[face]));
						returnPos = hit + out;// *0.1f;
						
					}
					
				}
				
			}


		}
	}
			
	
	return returnPos;

}

void CPlayer::Imgui()
{
	static bool show_player_winow = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (Keyboard_IsTrigger(DIK_F1))
		show_player_winow = !show_player_winow;

	if (show_player_winow)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Player", &lw_is_open, lw_flag);

		
		ImGui::Text("HitDistance : %.3f", m_hit);

		ImGui::InputFloat3("Position", m_Position, 1);
		ImGui::InputFloat3("Rotation", m_Rotation, 1);

		ImGui::End();
	}
}