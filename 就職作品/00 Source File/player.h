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



	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	ID3D11BlendState* m_pPointLightBlendState;
	ID3D11DepthStencilState* m_pBuckBuffer_DSTexState;

	D3DXVECTOR3 RayIntersect(CInstanceGameObject* object,int index);
	D3DXVECTOR3 LenOBBToPoint(CInstanceGameObject& obj, D3DXVECTOR3& p,float length);
	bool m_hit;
	
	D3DXVECTOR3 m_OldPosition;
	D3DXVECTOR3 m_OldForward;

	bool m_EnableHit = false;

	//影用地面テクスチャ
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

