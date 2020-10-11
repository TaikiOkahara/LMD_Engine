//#pragma once
//#include "renderer.h"
//#include "director.h"
//
//
//class Light
//{
//
//private:
//	
//	static D_LIGHT m_DirectionalData;
//	static P_LIGHT m_PointData;
//	static std::vector<D3DXVECTOR3*> PLList_Pos;
//	static std::vector<float*> PLList_Intensity;
//	static std::vector<float*> PLList_Range;
//	static std::vector<float*> PLList_Rate;
//
//public:
//	Light() {}
//	~Light(){}
//
//	static D_LIGHT GetDirectionalData();
//	static P_LIGHT GetPointData();
//
//	static void Init();	
//	static void Uninit();	
//	
//	static void Update();
//
//	static void SetDirectionalLight(D3DXVECTOR4 direction, float intensity);
//	static void SetPointLight(D3DXVECTOR3* pos,float* intensity,float* range,float* rate);
//};