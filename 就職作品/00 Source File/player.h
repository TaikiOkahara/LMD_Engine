#pragma once
#include "gameobject.h"
#include "SkeletalMesh.h"
#include "collision.h"
#include "InstanceObject.h"

class CPlayer : public CGameObject
{
private:
	CAnimationModel* m_AnimModel;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11InputLayout* m_VertexLayout;

	D3DXVECTOR3 RayIntersect(CInstanceGameObject* object,int index);
	FLOAT LenOBBToPoint(CInstanceGameObject& obj, D3DXVECTOR3& p,float length);
	float m_hit;
	int m_Frame = 0;
	D3DXVECTOR3 m_OldPosition;
	D3DXVECTOR3 m_OldForward;
public:
	CPlayer(){}
	~CPlayer(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};