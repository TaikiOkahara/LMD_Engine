/*---------------------------------------
*�@projectionShadow.h
*
* �v���C���[�̓��e�e�N�X�`�����쐬����N���X
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "gameobject.h"

class CProjShadow : public CGameObject
{
private:
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;

	ID3D11InputLayout* m_pVertexLayout = nullptr;

	bool m_Enable = true;
public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};