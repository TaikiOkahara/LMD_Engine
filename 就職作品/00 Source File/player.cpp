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
#include "pointLight.h"

//
//
//
void CPlayer::Init()
{
	
	m_AnimModel = new CAnimationModel();

	m_AnimModel->LoadModel("../02 Visual File//Knight//KnightPBR.fbx", D3DXVECTOR3(0,0.1f,0));
	
	m_AnimModel->LoadTexture();
	m_AnimModel->LoadAnimation("../02 Visual File//Knight//Run_Blender.fbx", "Run");
	m_AnimModel->LoadAnimation("../02 Visual File//Knight//Idle_Blender.fbx", "Idle");

	m_Transform.position = D3DXVECTOR3(-2.5f, 0.01f, -3.5f);
	m_Transform.scale = D3DXVECTOR3(0.8f, 0.8f, 0.8f);

	m_Collision.Init(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0, 0, 0));


	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEWEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_VertexShader,&m_VertexLayout,layout,7,"SkeletalVertexShader.cso");
	RENDERER::CreatePixelShader(&m_PixelShader, "SkeletalPixelShader.cso");
	RENDERER::CreatePixelShader(&m_ShadowPixelShader, "PlayerShadowPixelShader.cso");


	m_OldPosition = m_Transform.position;
	m_OldForward = GetForward();
	m_OldForward.y = 0;

	//D3D11_BLEND_DESC dblend;
	//ZeroMemory(&dblend, sizeof(D3D11_BLEND_DESC));
	//dblend.IndependentBlendEnable = false;
	//dblend.AlphaToCoverageEnable = false;
	//dblend.RenderTarget[0].BlendEnable = true;
	//dblend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;//メッシュのレンダリングイメージ
	//dblend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;//レンダーターゲットサーファスのイメージ
	//dblend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	////dblend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	////dblend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	////dblend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//dblend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	//if (FAILED(RENDERER::m_pDevice->CreateBlendState(&dblend, &m_pPointLightBlendState)))
	//{
	//	return ;
	//}

	////通常用ラスタライズ設定
	//D3D11_RASTERIZER_DESC rdc;
	//ZeroMemory(&rdc, sizeof(rdc));
	//rdc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	//rdc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	////rdc.FrontCounterClockwise = FALSE;//デフォルト
	//rdc.DepthClipEnable = FALSE;

	//rdc.MultisampleEnable = FALSE;

	//RENDERER::m_pDevice->CreateRasterizerState(&rdc, &m_pCommonRasterizerState);
	//m_pDeviceContext->RSSetState(m_pCommonRasterizerState);

	//D3D11_DEPTH_STENCIL_DESC dc;
	//ZeroMemory(&dc, sizeof(dc));
	//dc.DepthEnable = true;

	//dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
	////dc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;
	//dc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	////dc.DepthFunc=D3D11_COMPARISON_LESS;
	////dc.StencilEnable=false;
	//
	//if (FAILED(RENDERER::m_pDevice->CreateDepthStencilState(&dc, &m_pBuckBuffer_DSTexState)))
	//{
	//	return;// E_FAIL;
	//}
	//深度ステンシルステートを適用
	//m_pDeviceContext->OMSetDepthStencilState(m_pBuckBuffer_DSTexState,NULL);

}
//
//
//
void CPlayer::Uninit()
{
	m_AnimModel->Unload();
	delete m_AnimModel;

	m_Collision.Uninit();

	m_VertexShader->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();
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

	if (CInput::KeyPress(DIK_W))
	{
		position += cameraforward;
		rotation = camerarotation.y + 2 * D3DX_PI;
		if (CInput::KeyPress(DIK_A)){
			position -= cameraright;
			rotation = camerarotation.y + 2 * D3DX_PI - D3DX_PI/4;
		}
		else if (CInput::KeyPress(DIK_D)) {
			position += cameraright;
			rotation = camerarotation.y + 2 * D3DX_PI + D3DX_PI / 4;
		}
	}
	else if (CInput::KeyPress(DIK_S))
	{
		position -= cameraforward;
		rotation = camerarotation.y + D3DX_PI;
		if (CInput::KeyPress(DIK_A)) {
			position -= cameraright;
			rotation = camerarotation.y + D3DX_PI + D3DX_PI / 4;
		}
		else if (CInput::KeyPress(DIK_D)) {
			position += cameraright;
			rotation = camerarotation.y + D3DX_PI - D3DX_PI / 4;
		}
	}
	else if (CInput::KeyPress(DIK_A))
	{
		position -= cameraright;
		rotation = camerarotation.y -D3DX_PI/2;
	}
	else if (CInput::KeyPress(DIK_D))
	{
		position += cameraright;
		rotation = camerarotation.y + D3DX_PI/2;
	}
	

	D3DXVec3Normalize(&position, &position);
	m_Transform.position += position * 0.08f;
	
	if (rotation != 0){
		m_Transform.rotation.y = rotation;
	}
	
	//壁との当たり判定
	if (m_EnableHit)
	{
		CWall* wall = Base::GetScene()->GetGameObject<CWall>(1);
		m_Transform.position = LenOBBToPoint(*wall, m_Transform.position, 7.0f);
		CPillar* pillar = Base::GetScene()->GetGameObject<CPillar>(1);
		m_Transform.position = LenOBBToPoint(*pillar, m_Transform.position, 2.0f);
	}
	

	
	m_Collision.Update();


	/*bool ishit = false;
	for (int i = 0; i < wall->GetMeshCount(); i++)
	{
		ishit = ColOBBs(this, wall, i);

		if (ishit)
			break;
	}
	m_hit = ishit;*/



	if (CInput::KeyPress(DIK_W) || 
		CInput::KeyPress(DIK_A) ||
		CInput::KeyPress(DIK_S) ||
		CInput::KeyPress(DIK_D))
		m_AnimModel->Update("Run", m_AnimModel->m_iFrame);
	else
		m_AnimModel->Update("Idle", m_AnimModel->m_iFrame);

	

	m_OldPosition = m_Transform.position;
	m_AnimModel->m_iFrame++;
}
//
//
//
void CPlayer::Draw()
{
	RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_VertexLayout);

	WORLDMATRIX worldMatrix;
	//モデル
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Transform.scale.x / 100.0f, m_Transform.scale.y / 100.0f, m_Transform.scale.z / 100.0f);//アニメーションデータをスケール変更できないため、/100をして調節
	D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y - D3DX_PI, m_Transform.rotation.x, m_Transform.rotation.z);
	D3DXMatrixTranslation(&trans, m_Transform.position.x, m_Transform.position.y, m_Transform.position.z);
	world = scale * rot * trans;
	
	worldMatrix.worldMatrix = world;
	RENDERER::SetWorldMatrix(worldMatrix);

	m_AnimModel->Draw();


	//影
	//{
	//	//ブレンドステート
	//	float blend[4] = { 1,1,1,1 };
	//	RENDERER::m_pDeviceContext->OMSetBlendState(m_pPointLightBlendState, blend, 0xffffffff);
	//	RENDERER::m_pDeviceContext->OMSetDepthStencilState(m_pBuckBuffer_DSTexState, NULL);
	//	//RENDERER::m_pDeviceContext->RSSetState(m_pCommonRasterizerState);

	//	RENDERER::m_pDeviceContext->PSSetShader(m_ShadowPixelShader, NULL, 0);
	//	CPointLight* pointLight = Base::GetScene()->GetGameObject<CPointLight>(2);
	//	int lightNum = pointLight->GetMeshCount();
	//	for (int i = 0; i < lightNum; i++)
	//	{
	//		D3DXVECTOR3 lightPos = pointLight->GetPosition(i);
	//		float distance;
	//		distance = D3DXVec3Length(&D3DXVECTOR3(lightPos - m_Transform.position));
	//		if (distance > pointLight->GetScale(i).x)
	//			continue;

	//		D3DXMATRIX shadow;
	//		D3DXPLANE plane;

	//		D3DXPlaneFromPointNormal(&plane, &D3DXVECTOR3(0, 0.01f, 0), &D3DXVECTOR3(0, 1, 0));
	//		D3DXMatrixShadow(&shadow, &D3DXVECTOR4(lightPos.x, lightPos.y, lightPos.z, 1), &plane);
	//		worldMatrix.worldMatrix = world * shadow;
	//		//WORLDMATRIXのInverseにポイントライトの座標とサイズをマトリクスを入れる
	//		D3DXMatrixTranslation(&worldMatrix.worldInverseMatrix, lightPos.x, lightPos.y, lightPos.z);
	//		//サイズ(どこでもいいが31)
	//		worldMatrix.worldInverseMatrix._31 = pointLight->GetScale(i).x;


	//		RENDERER::SetWorldMatrix(worldMatrix);

	//		m_AnimModel->Draw();
	//	}

	//	//ブレンドステート

	//	RENDERER::m_pDeviceContext->OMSetBlendState(nullptr, blend, 0xffffffff);
	//	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	//}
	


	//コリジョン	
	D3DXMatrixScaling(&scale, m_Transform.scale.x, m_Transform.scale.y, m_Transform.scale.z);
	world = scale * rot * trans;
	worldMatrix.worldMatrix = world;
	RENDERER::SetWorldMatrix(worldMatrix);

	if (m_EnableCollision)
		m_Collision.Draw();
}

//インスタンスオブジェクト専用
//3次元OBBと点の最短距離算出関数
D3DXVECTOR3 CPlayer:: LenOBBToPoint(CInstanceGameObject& obj, D3DXVECTOR3& p,float length)
{
	D3DXVECTOR3 Vec(0,0,0);   // 最終的に長さを求めるベクトル
	D3DXVECTOR3 position = m_Transform.position;

	//最初は1個から
	for(unsigned int i = 0;i < obj.GetMeshMax();i++)
	{
		//length以内にいないものは排除する
		float len = D3DXVec3Length(&D3DXVECTOR3(obj.GetPosition(i) + obj.GetCollision()->GetPosition() - m_Transform.position));
		if (len > length)
		{
			continue;
		}



		D3DXVECTOR3 pos = obj.GetPosition(i) + obj.GetCollision()->GetPosition();

		FLOAT LenX = obj.GetCollision()->GetXsize();
		FLOAT LenY = obj.GetCollision()->GetYsize();
		FLOAT LenZ = obj.GetCollision()->GetZsize();
		if (LenX > 0)// L=0は計算できない
		{
			D3DXVECTOR3 rot = obj.GetRight(i);
			FLOAT s = D3DXVec3Dot(&(p - pos), &rot) / LenX;

			// sの値から、はみ出した部分があればそのベクトルを加算
			s = fabs(s);
			if (s > 1)
				Vec += (1 - s) * LenX * rot;   // はみ出した部分のベクトル算出
		}

		if (LenY > 0)// L=0は計算できない
		{
			D3DXVECTOR3 rot = obj.GetUp(i);
			FLOAT s = D3DXVec3Dot(&(p - pos), &rot) / LenY;

			// sの値から、はみ出した部分があればそのベクトルを加算
			s = fabs(s);
			if (s > 1)
				Vec += (1 - s) * LenY * rot;   // はみ出した部分のベクトル算出
		}
		if (LenZ > 0)// L=0は計算できない
		{
			D3DXVECTOR3 rot = obj.GetForward(i);
			FLOAT s = D3DXVec3Dot(&(p - pos), &rot) / LenZ;

			// sの値から、はみ出した部分があればそのベクトルを加算
			s = fabs(s);
			if (s > 1)
				Vec += (1 - s) * LenZ * rot;   // はみ出した部分のベクトル算出
		}

		
		if (D3DXVec3Length(&Vec) <= 0)
			position = RayIntersect(&obj, i);



		Vec = D3DXVECTOR3(0, 0, 0);
	}
	
	return position;
}

//壁に平行なベクトルを計算する関数
//前回の座標から今の座標へ伸ばしたベクトルとボックスの当たり判定
D3DXVECTOR3 CPlayer::RayIntersect(CInstanceGameObject* object, int index)
{
	D3DXVECTOR3 playerPos = m_Transform.position;//プレイヤー
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

	if (CInput::KeyTrigger(DIK_F1))
		show_player_winow = !show_player_winow;

	if (show_player_winow)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Player", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);
		ImGui::Checkbox("EnableHit", &m_EnableHit);
		
		
		/*if(m_hit)
			ImGui::Text("Hit!!!");*/

		ImGui::Text("Position : %f,%f,%f", m_Transform.position.x,m_Transform.position.y,m_Transform.position.z);
		ImGui::Text("Rotation : %f,%f,%f", m_Transform.rotation.x,m_Transform.rotation.y,m_Transform.rotation.z);

		ImGui::End();
	}
}