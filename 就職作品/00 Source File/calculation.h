/*---------------------------------------
*　calculation.h
* 汎用的な計算関数群
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"

//頂点3つとUV座標３つと法線をもとに接線を計算する。返す4次元ベクトルのwは左か右手系かの判断フラグ。
void CalcTangent(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3,D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR3 normal,D3DXVECTOR3 Tangent,D3DXVECTOR3 Binormal);

float Det(D3DXVECTOR3 vector0, D3DXVECTOR3 vector1, D3DXVECTOR3 vector2);

bool CulcRay(D3DXVECTOR3 origin, D3DXVECTOR3 ray, D3DXVECTOR3 v0, D3DXVECTOR3 edge1, D3DXVECTOR3 edge2);

//OBB同士の当たり判定
bool ColOBBs(CGameObject* obb1, CGameObject* obb2);
bool ColOBBs(CGameObject* obb1, CInstanceGameObject* obb2,int index);