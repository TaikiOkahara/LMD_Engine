#pragma once
#include "gameobject.h"
#include "skeletalMesh.h"
#include "collision.h"
#include "InstanceObject.h"
#include "tile.h"



class CPlayer : public CGameObject
{
	



private:
	CAnimationModel* m_AnimModel;

	//---------------------------------
	//CCondition* m_CurCondition;
	//CCondition* m_NexCondition;

	////std::map<std::string, CCondition*> m_ConditionList;
	//float m_blendTime = 1.0f;
	//const float m_blendSpeed = 0.1f;//���ŌŒ�l

	//unsigned int m_CurLevel;

	//void SetCondition(CCondition* set)
	//{
	//	if (m_CurCondition == nullptr)
	//	{
	//		m_CurCondition = m_NexCondition = set;
	//		m_CurLevel = m_CurCondition->GetLevel();
	//		return;
	//	}

	//	if (m_CurCondition->MotionLock())
	//		return;

	//	if (typeid(*m_CurCondition) == typeid(set))
	//		return;

	//	//�ʏ탂�[�V�����u�����h
	//	//m_NexCondition->FrameReset();
	//	m_NexCondition = set;

	//}
	//void AnimUpdate()
	//{
	//	m_CurCondition->FrameUpdate();

	//	//�u�����f�B���O
	//	if (m_CurCondition == m_NexCondition)
	//		return;


	//	//���̍Ō�̃t���[���Ǝ��̃A�j���[�V�����̃t���[�����u�����h

	//	if (!m_CurCondition->MotionLock())
	//	{
	//		//m_CurLevel = m_NexCondition->GetLevel();


	//		unsigned int frame_0 = m_CurCondition->GetFrameMax();
	//		unsigned int frame_1 = m_NexCondition->GetCurFrame();

	//		//m_AnimModel->SetAnimation(frame_0,frame_1,)

	//	}
	//	else
	//	{
	//		//���̎c��̃t���[���Ǝ��̃A�j���[�V�����̃t���[�����u�����h
	//		m_blendTime -= m_blendSpeed;
	//		if (m_blendTime <= 0)
	//		{
	//			m_blendTime = 1.0f;
	//			m_CurCondition = m_NexCondition;
	//		}


	//		unsigned int frame_0 = m_CurCondition->GetCurFrame();
	//		unsigned int frame_1 = m_NexCondition->GetCurFrame();




	//		if (m_CurCondition->GetLowerLock())
	//		{

	//		}

	//	}
	//}
	//void LoadAnimation(std::string file_name, std::string anim_name,unsigned int level)
	///*{
	//	m_AnimModel->LoadAnimation(file_name.c_str(), anim_name.c_str());
	//	m_ConditionList[anim_name.c_str()] = CCondition(level,);
	//}*/
	//---------------------------------

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	ID3D11BlendState* m_pPointLightBlendState;
	ID3D11DepthStencilState* m_pBuckBuffer_DSTexState;
	//ID3D11RasterizerState* m_pCommonRasterizerState;
	D3DXVECTOR3 RayIntersect(CInstanceGameObject* object,int index);
	D3DXVECTOR3 LenOBBToPoint(CInstanceGameObject& obj, D3DXVECTOR3& p,float length);
	bool m_hit;
	
	D3DXVECTOR3 m_OldPosition;
	D3DXVECTOR3 m_OldForward;

	bool m_EnableHit = false;

	//�e�p�n�ʃe�N�X�`��
	Tile* m_pTile = nullptr;
	ID3D11VertexShader* m_TileVertexShader;
	ID3D11PixelShader* m_TilePixelShader;

	TRANSFORM m_pTileTransform;

public:
	CPlayer(){}
	~CPlayer(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawShadow();
	void Imgui();

};

