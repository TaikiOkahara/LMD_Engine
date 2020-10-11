#pragma once
#include "gameobject.h"
#include "SkeletalMesh.h"
#include "collision.h"

class CPlayer : public CGameObject
{
private:
	CAnimationModel* m_AnimModel;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11InputLayout* m_VertexLayout;

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