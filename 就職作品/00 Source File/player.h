/*---------------------------------------
*�@player.h
*
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "gameobject.h"
#include "skeletalMesh.h"
#include "collision.h"
#include "instanceObject.h"
#include "tile.h"



class CPlayer : public CGameObject
{
private:
	CAnimationModel* m_AnimModel;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11BlendState* m_pPointLightBlendState = nullptr;
	ID3D11DepthStencilState* m_pBuckBuffer_DSTexState = nullptr;

	D3DXVECTOR3 m_OldPosition;
	D3DXVECTOR3 m_OldForward;

	bool m_EnableCollision = true;//�����蔻����s��


	//�e���------------------------------

	//�e�n�ʃe�N�X�`��
	Tile* m_pTile = nullptr;
	ID3D11VertexShader* m_TileVertexShader = nullptr;
	ID3D11PixelShader* m_TilePixelShader = nullptr;
	TRANSFORM m_pTileTransform;
	//------------------------------------

	//�v���C���[�����蔻��֐�
	D3DXVECTOR3 RayIntersect(CInstanceGameObject* object,int index);
	D3DXVECTOR3 LenOBBToPoint(CInstanceGameObject& obj, D3DXVECTOR3& p,float length);
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawShadow();
	void Imgui();
};

