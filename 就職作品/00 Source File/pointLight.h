#pragma once
#include "InstanceObject.h"
#include "StaticMesh.h"

class CPointLight : public CInstanceGameObject
{
private:

	//enum LightName
	//{
	//	PLAYER = 0,//�v���C���[�ɂ��Ă���
	//	CHANDELIER,//�V�����f���A
	//	WORLD,//��
	//};
	POINTLIGHT m_PointLight[LIGHT_MAX];//�|�C���g���C�g�ǉ����
	StaticMesh* m_pMesh = nullptr;

	
	//TRANSFORM* m_TransformList;
		 

	//�\���̃o�b�t�@�ƃr���[
	//ID3D11Buffer* m_pPointLightBuffer = nullptr;
	//ID3D11ShaderResourceView* m_pPointLightBufferSRV = nullptr;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	//ID3D11InputLayout* m_pVertexLayout;

	bool m_EnablePlayerPointLight = true;
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};
