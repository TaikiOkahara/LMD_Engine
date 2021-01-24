/*---------------------------------------
*�@calculation.h
* �ėp�I�Ȍv�Z�֐��Q
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "instanceObject.h"

//���_3��UV���W�R�Ɩ@�������Ƃɐڐ����v�Z����B�Ԃ�4�����x�N�g����w�͍����E��n���̔��f�t���O�B
void CalcTangent(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3,D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR3 normal,D3DXVECTOR3 Tangent,D3DXVECTOR3 Binormal);

float Det(D3DXVECTOR3 vector0, D3DXVECTOR3 vector1, D3DXVECTOR3 vector2);

bool CulcRay(D3DXVECTOR3 origin, D3DXVECTOR3 ray, D3DXVECTOR3 v0, D3DXVECTOR3 edge1, D3DXVECTOR3 edge2);

//OBB���m�̓����蔻��
bool ColOBBs(CGameObject* obb1, CGameObject* obb2);
bool ColOBBs(CGameObject* obb1, CInstanceGameObject* obb2,int index);