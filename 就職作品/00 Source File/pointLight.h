#pragma once
#include "gameobject.h"

class CPointLight : public CGameObject
{
private:

	//std::vector<D3DXVECTOR4> m_PointList;
	POINTLIGHT m_PointLight;

	//int m_PointCount;

	//�\���̃o�b�t�@�ƃr���[
	ID3D11Buffer* m_pPointLightBuffer = nullptr;
	ID3D11ShaderResourceView* m_pPointLightBufferSRV = nullptr;

	StaticMesh* m_pMesh = nullptr;
	
	ID3D11RasterizerState* m_pBackCullingRasterizerState;
	ID3D11BlendState* m_pBackCullingBlendState;
public:

	/*CPointLight() {}
	~CPointLight() {}*/


	//����Init��GameObject�N���X��Init�̍Ō�ɓ����
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};
