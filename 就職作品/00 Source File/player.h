#pragma once
#include "gameobject.h"
#include "skeletalMesh.h"
#include "collision.h"
#include "InstanceObject.h"

class CPlayer : public CGameObject
{
private:
	CAnimationModel* m_AnimModel;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11PixelShader* m_ShadowPixelShader;

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
public:
	CPlayer(){}
	~CPlayer(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};