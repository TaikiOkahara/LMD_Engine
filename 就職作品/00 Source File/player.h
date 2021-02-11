/*---------------------------------------
*　player.h
*
*@author：Okahara Taiki
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

	bool m_EnableCollision = true;//当たり判定実行可否


	//影情報------------------------------

	//影地面テクスチャ
	Tile* m_pTile = nullptr;
	ID3D11VertexShader* m_TileVertexShader = nullptr;
	ID3D11PixelShader* m_TilePixelShader = nullptr;
	TRANSFORM m_pTileTransform;
	//------------------------------------

	//プレイヤー当たり判定関数
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

