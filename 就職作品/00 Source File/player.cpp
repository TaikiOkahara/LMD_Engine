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

	SetVisualDirectory("../02 Visual File//Knight//");
	m_AnimModel->LoadModel("KnightPBR_NoTex.fbx", D3DXVECTOR3(0,0.1f,0));
	m_AnimModel->LoadTexture("texture");
	
	m_AnimModel->LoadAnimation("Run.fbx", "Run",48,false,1.0f);
	m_AnimModel->LoadAnimation("Slash.fbx", "Slash",122,true,1.0f);
	m_AnimModel->LoadAnimation("Sword_And_Shield_Idle_152.fbx", "Idle",152,false,1.0f);
	m_AnimModel->LoadAnimation("Sword_And_Shield_Walk_65.fbx", "Walk",65,false,1.0f);
	m_AnimModel->LoadAnimation("Kick_71.fbx", "Kick",71,true,1.0f);
	m_AnimModel->LoadAnimation("Slash_146.fbx", "Slash2",146, true,1.3f);
	m_AnimModel->LoadAnimation("Slash2_100.fbx", "Slash3",100, true,1.3f);
	m_AnimModel->LoadAnimation("Sword_And_Shield_Slash_3_90.fbx", "Slash4",90, true,1.3f);
	m_AnimModel->LoadAnimation("Standing_Melee_Combo_Attack_Ver_2_252.fbx", "Slash5",252, true,1.3f);
	
	//m_AnimModel->SetLockAnimation("Walk");


	//m_ConditionList["idle"] = new CCondition(PRIORITY_LEVEL::LEVEL_1, 0, 1, false);
	
	//m_AnimModel->LoadAnimation("../02 Visual File//Knight//Sword_And_Shield_Attack.fbx", "Attack");

	m_Transform.position = D3DXVECTOR3(-2.5f, 0.01f, -3.5f);
	m_Transform.scale = D3DXVECTOR3(0.8f, 0.8f, 0.8f);

	m_Collision.Init(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0, 0, 0));


	//�@���̓��C�A�E�g����
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEWEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//�V�F�[�_�[�쐬
	RENDERER::CreateVertexShader(&m_VertexShader,&m_VertexLayout,layout,7,"skeletalVS.cso");
	RENDERER::CreatePixelShader(&m_PixelShader, "skeletalPS.cso");


	m_OldPosition = m_Transform.position;
	m_OldForward = GetForward();
	m_OldForward.y = 0;



	m_pTile = new Tile();


	m_pTile->Init("Wall//T_StoneWall_A.dds", "Wall//T_StoneWall_N.dds", "Wall//T_StoneWall_C.dds", 1, 1, 30);
	RENDERER::CreateVertexShader(&m_TileVertexShader,nullptr, nullptr,0,"playerShadowVS.cso");
	RENDERER::CreatePixelShader(&m_TilePixelShader, "playerShadowPS.cso");



}
//
//
//
void CPlayer::Uninit()
{
	m_AnimModel->Unload();
	delete m_AnimModel;

	

	m_pTile->Uninit();
	delete m_pTile;

	m_Collision.Uninit();

	m_VertexShader->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();

	m_TileVertexShader->Release();
	m_TilePixelShader->Release();


	delete m_pPointLightBlendState;
	delete m_pBuckBuffer_DSTexState;
}
//
//
//
void CPlayer::Update()
{


	CCamera* camera = Base::GetScene()->GetGameObject<CCamera>();


	D3DXVECTOR3 cameraright = camera->GetRight();
	cameraright.y = 0;
	D3DXVECTOR3 cameraforward = camera->GetForward();
	cameraforward.y = 0;
	D3DXVECTOR3 camerarotation = camera->GetRotation();
	camerarotation.x = 0;
	camerarotation.z = 0;
	



	D3DXVECTOR3 position(0,0,0);
	float rotation = 0;

	if(m_AnimModel->GetMotionLock())
	{
		//�ړ��Ȃ�
	}
	else if (CInput::KeyPress(DIK_W))
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

	float moveSpeed = 0.03f;

	if (CInput::KeyPress(DIK_SPACE))
	{
		moveSpeed = 0.08f;
	}

	m_Transform.position += position * moveSpeed;
	
	if (rotation != 0){
		m_Transform.rotation.y = rotation;
	}
	
	//�ǂƂ̓����蔻��
	if (m_EnableHit)
	{

		CWall* wall = Base::GetScene()->GetGameObject<CWall>();
		if(wall != NULL)
		{
			m_Transform.position = LenOBBToPoint(*wall, m_Transform.position, 7.0f);
		}
		CPillar* pillar = Base::GetScene()->GetGameObject<CPillar>();
		if (pillar != NULL)
		{
			m_Transform.position = LenOBBToPoint(*pillar, m_Transform.position, 5.0f);
		}

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

	if (CInput::KeyTrigger(DIK_RETURN)) 
	{
		m_AnimModel->SetAnimation("Slash", false);
	}
	else if (CInput::KeyTrigger(DIK_F7))
	{

		m_AnimModel->SetAnimation("Slash2", false);
	}
	else if (CInput::KeyTrigger(DIK_F6))
	{

		m_AnimModel->SetAnimation("Slash3", false);
	}
	else if (CInput::KeyTrigger(DIK_F5))
	{

		m_AnimModel->SetAnimation("Slash5", false);
	}
	else if (CInput::KeyTrigger(DIK_F4))
	{

		m_AnimModel->SetAnimation("Slash4", false);
	}
	else if (CInput::KeyPress(DIK_W) ||
		CInput::KeyPress(DIK_A) ||
		CInput::KeyPress(DIK_S) ||
		CInput::KeyPress(DIK_D))
	{
		if (CInput::KeyPress(DIK_SPACE))
			m_AnimModel->SetAnimation("Run", false);
		else
			m_AnimModel->SetAnimation("Walk",false);

		
	}
	else
	{
		m_AnimModel->SetAnimation("Idle",false);

	}

	

	m_AnimModel->Update();
	


	m_OldPosition = m_Transform.position;
}
//
//
//
void CPlayer::Draw()
{

	RENDERER::ShadowDraw();

	D3DXMATRIX world, scale, rot, trans;

	D3DXMatrixScaling(&scale, 1, 1, 1);
	D3DXMatrixRotationYawPitchRoll(&rot, 0, 0, 0);
	D3DXMatrixTranslation(&trans, m_Transform.position.x, m_Transform.position.y +  0.01f, m_Transform.position.z);//�e�͏����n�ʂ��痣��
	world = scale * rot * trans;

	RENDERER::m_ConstantBufferList.GetStruct<WorldBuffer>()->Set(world);
	RENDERER::m_pDeviceContext->VSSetShader(m_TileVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_TilePixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pCommonVertexLayout);

	m_pTile->Draw();

	RENDERER::CommonDraw();

	RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_VertexLayout);

	//���f��
	D3DXMatrixScaling(&scale, m_Transform.scale.x / 100.0f, m_Transform.scale.y / 100.0f, m_Transform.scale.z / 100.0f);//�A�j���[�V�����f�[�^���X�P�[���ύX�ł��Ȃ����߁A/100�����Ē���
	D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y - D3DX_PI, m_Transform.rotation.x, m_Transform.rotation.z);
	D3DXMatrixTranslation(&trans, m_Transform.position.x, m_Transform.position.y, m_Transform.position.z);
	world = scale * rot * trans;
	
	RENDERER::m_ConstantBufferList.GetStruct<WorldBuffer>()->Set(world);


	m_AnimModel->Draw();


	//�R���W����	

	if (m_EnableCollision)
	{
		D3DXMatrixScaling(&scale, m_Transform.scale.x, m_Transform.scale.y, m_Transform.scale.z);
		world = scale * rot * trans;
		RENDERER::m_ConstantBufferList.GetStruct<WorldBuffer>()->Set(world);
		m_Collision.Draw();
	}
}

void CPlayer::DrawShadow()
{
	//���f��
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Transform.scale.x / 100.0f, m_Transform.scale.y / 100.0f, m_Transform.scale.z / 100.0f);//�A�j���[�V�����f�[�^���X�P�[���ύX�ł��Ȃ����߁A/100�����Ē���
	D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y - D3DX_PI, m_Transform.rotation.x, m_Transform.rotation.z);
	D3DXMatrixTranslation(&trans, m_Transform.position.x, m_Transform.position.y, m_Transform.position.z);
	world = scale * rot * trans;

	RENDERER::m_ConstantBufferList.GetStruct<WorldBuffer>()->Set(world);


	m_AnimModel->Draw();
}

//�C���X�^���X�I�u�W�F�N�g��p
//3����OBB�Ɠ_�̍ŒZ�����Z�o�֐�
D3DXVECTOR3 CPlayer:: LenOBBToPoint(CInstanceGameObject& obj, D3DXVECTOR3& p,float length)
{
	D3DXVECTOR3 Vec(0,0,0);   // �ŏI�I�ɒ��������߂�x�N�g��
	D3DXVECTOR3 position = m_Transform.position;

	//�ŏ���1����
	for(unsigned int i = 0;i < obj.GetMeshMax();i++)
	{
		//length�ȓ��ɂ��Ȃ����͔̂r������
		float len = D3DXVec3Length(&D3DXVECTOR3(obj.GetPosition(i) + obj.GetCollision()->GetPosition() - m_Transform.position));
		if (len > length)
		{
			continue;
		}



		D3DXVECTOR3 pos = obj.GetPosition(i) + obj.GetCollision()->GetPosition();

		FLOAT LenX = obj.GetCollision()->GetXsize();
		FLOAT LenY = obj.GetCollision()->GetYsize();
		FLOAT LenZ = obj.GetCollision()->GetZsize();
		if (LenX > 0)// L=0�͌v�Z�ł��Ȃ�
		{
			D3DXVECTOR3 rot = obj.GetRight(i);
			FLOAT s = D3DXVec3Dot(&(p - pos), &rot) / LenX;

			// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
			s = fabs(s);
			if (s > 1)
				Vec += (1 - s) * LenX * rot;   // �͂ݏo���������̃x�N�g���Z�o
		}

		if (LenY > 0)// L=0�͌v�Z�ł��Ȃ�
		{
			D3DXVECTOR3 rot = obj.GetUp(i);
			FLOAT s = D3DXVec3Dot(&(p - pos), &rot) / LenY;

			// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
			s = fabs(s);
			if (s > 1)
				Vec += (1 - s) * LenY * rot;   // �͂ݏo���������̃x�N�g���Z�o
		}
		if (LenZ > 0)// L=0�͌v�Z�ł��Ȃ�
		{
			D3DXVECTOR3 rot = obj.GetForward(i);
			FLOAT s = D3DXVec3Dot(&(p - pos), &rot) / LenZ;

			// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
			s = fabs(s);
			if (s > 1)
				Vec += (1 - s) * LenZ * rot;   // �͂ݏo���������̃x�N�g���Z�o
		}

		
		if (D3DXVec3Length(&Vec) <= 0)
			position = RayIntersect(&obj, i);



		Vec = D3DXVECTOR3(0, 0, 0);
	}
	
	return position;
}

//�ǂɕ��s�ȃx�N�g�����v�Z����֐�
//�O��̍��W���獡�̍��W�֐L�΂����x�N�g���ƃ{�b�N�X�̓����蔻��
D3DXVECTOR3 CPlayer::RayIntersect(CInstanceGameObject* object, int index)
{
	D3DXVECTOR3 playerPos = m_Transform.position;//�v���C���[
	D3DXVECTOR3 oldPlayerPos = m_OldPosition;//�I�u�W�F�N�g�̒��S
	D3DXVECTOR3 origin = m_OldPosition;//���C���΂����_
	D3DXVECTOR3 ray = playerPos - oldPlayerPos;
	D3DXVec3Normalize(&ray, &ray);

	D3DXVECTOR3 returnPos = m_OldPosition;
	D3DXVECTOR3 returnNormal;//�ǂ���Ɏg��

	//D3DXVECTOR3 offsetVertex[8];

	D3DXVECTOR3 vertex[8];
	D3DXVECTOR3 normal[6];

	//�I�t�Z�b�g���擾
	for (int i = 0; i < 8; i++)
	{
		vertex[i] = object->GetCollision()->GetVertex()[i];
	}
	for (int i = 0; i < 6; i++)
	{
		normal[i] = object->GetCollision()->GetNormal()[i];
	}

	{

		//�}�g���N�X
		D3DXMATRIX mWorld, mScale, mRot, mTrans;
		D3DXMatrixScaling(&mScale, object->GetScale(index).x, object->GetScale(index).y, object->GetScale(index).z);
		D3DXMatrixRotationYawPitchRoll(&mRot, object->GetRotation(index).y, object->GetRotation(index).x, object->GetRotation(index).z);
		D3DXMatrixTranslation(&mTrans, object->GetPosition(index).x, object->GetPosition(index).y, object->GetPosition(index).z);
		mWorld = mScale * mRot * mTrans;

		//���_�����[���h���W�ϊ�
		for (int i = 0; i < 8; i++)
		{
			D3DXVec3TransformCoord(&vertex[i], &vertex[i], &mWorld);
		}
		//�@�������[���h���W�ϊ�
		for (int i = 0; i < 6; i++)
		{
			D3DXVec3TransformCoord(&normal[i], &normal[i], &mRot);
		}



		//���ʂ𐶐�
		D3DXPLANE plane[6];

		D3DXPlaneFromPoints(&plane[0], &vertex[0], &vertex[1], &vertex[2]);
		D3DXPlaneFromPoints(&plane[1], &vertex[5], &vertex[4], &vertex[7]);
		D3DXPlaneFromPoints(&plane[2], &vertex[4], &vertex[5], &vertex[1]);
		D3DXPlaneFromPoints(&plane[3], &vertex[6], &vertex[7], &vertex[3]);
		D3DXPlaneFromPoints(&plane[4], &vertex[4], &vertex[0], &vertex[3]);
		D3DXPlaneFromPoints(&plane[5], &vertex[1], &vertex[5], &vertex[6]);





			//���ʂƂ̏Փ˂��擾
		for (int face = 0; face < 6; face++)
		{

			//�ʂ̋t�@�����Ƀv���C���[�����Ȃ��ꍇ�͏Ȃ�
			//float nor = D3DXPlaneDotCoord(&plane[face], &oldPlayerPos);
			//if (nor >= 0)
			//{
			//	

			//}
			float nor = D3DXPlaneDotCoord(&plane[face], &oldPlayerPos);
			float nor2 = D3DXPlaneDotCoord(&plane[face], &playerPos);
			if (nor >= -0.01f)//������ƊÂ�
			{
				if (nor2 < 0)
				{
					D3DXVECTOR3  hit;
					bool  boolHit;
					boolHit = D3DXPlaneIntersectLine(&hit, &plane[face], &oldPlayerPos, &playerPos);

					if (boolHit != NULL)
					{
						//��_����A�ǂ��蔻����s��
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