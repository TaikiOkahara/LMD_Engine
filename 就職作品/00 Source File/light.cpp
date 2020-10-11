//#include "director.h"
//#include "renderer.h"
//#include "light.h"
//
//D_LIGHT Light::m_DirectionalData;
//P_LIGHT Light::m_PointData;
//std::vector<D3DXVECTOR3*>	Light::PLList_Pos;
//std::vector<float*>			Light::PLList_Intensity;
//std::vector<float*>			Light::PLList_Range;
//std::vector<float*>			Light::PLList_Rate;
//
//
//
////
////
////
//void Light::Init()
//{
//
//	//　ディレクショナル
//	m_DirectionalData.LightDir = D3DXVECTOR4(0, 0, 0, 0);
//	m_DirectionalData.Intensity = D3DXVECTOR4(0, 0, 0, 0);
//	//m_DirectionalData.Eye = D3DXVECTOR4(0, 0, 0, 1);
//	//　ポイントライト
//	m_PointData.g_fNumLight = D3DXVECTOR4(LIGHT_NUM, 0, 0, 1);
//
//	
//	for (int i = 0; i < m_PointData.g_fNumLight.x; i++)
//	{
//		m_PointData.g_vLightPos[i] = D3DXVECTOR4(0, 0, 0, 1);
//		m_PointData.g_fIntensity[i] = D3DXVECTOR4(0, 0, 0, 1);
//		m_PointData.g_fRange[i] = D3DXVECTOR4(1, 0, 0, 1);
//		m_PointData.g_fAttRate[i] = D3DXVECTOR4(2, 0, 0, 1);
//	}
//
//}
////
////
////
//void Light::Uninit()
//{
//	PLList_Pos.clear();
//	PLList_Intensity.clear();
//	PLList_Range.clear();
//	PLList_Rate.clear();
//
//}
////
////
////
//void Light::Update()
//{
//
//	for (int i = 0; i < PLList_Pos.size(); i++)
//	{
//		m_PointData.g_vLightPos[i] =D3DXVECTOR4(*PLList_Pos[i],1);
//
//		m_PointData.g_fIntensity[i].x = *PLList_Intensity[i];
//		m_PointData.g_fRange[i].x = *PLList_Range[i];
//		m_PointData.g_fAttRate[i].x = *PLList_Rate[i];
//	}
//
//}
//
//void Light::SetDirectionalLight(D3DXVECTOR4 direction, float intensity)
//{
//	//　ディレクショナル
//	m_DirectionalData.LightDir = direction;
//	m_DirectionalData.Intensity = D3DXVECTOR4(intensity, 0, 0, 0);
//
//}
////
////
////
//void Light::SetPointLight(D3DXVECTOR3* pos,float* intensity,float* range,float* rate)
//{
//	if (PLList_Pos.size() > LIGHT_NUM)
//	{
//		// 10個以上は入れられない
//		return;
//	}
//
//	PLList_Pos.push_back(pos);
//	PLList_Intensity.push_back(intensity);
//	PLList_Range.push_back(range);
//	PLList_Rate.push_back(rate);
//
//	return;
//}
////
////
////
//D_LIGHT Light::GetDirectionalData()
//{
//	return m_DirectionalData;
//}
////
//P_LIGHT Light::GetPointData()
//{
//	return m_PointData;
//}
//
