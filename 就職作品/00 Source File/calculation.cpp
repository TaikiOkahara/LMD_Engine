#include "calculation.h"

//���_3��UV���W�R�Ɩ@�������Ƃɐڐ����v�Z����B�Ԃ�4�����x�N�g����w�͍����E��n���̔��f�p�t���O�B
void CalcTangent(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3,
	D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR3 normal, D3DXVECTOR3 Tangent, D3DXVECTOR3 Binormal)
{
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 bitangent;

	D3DXVECTOR3 edge1 = v2 - v1;//�x�N�g��
	D3DXVECTOR3 edge2 = v3 - v1;//�x�N�g��
	D3DXVec3Normalize(&edge1, &edge1);//�@���x�N�g��
	D3DXVec3Normalize(&edge2, &edge2);//�@���x�N�g��
	D3DXVECTOR2 uvEdge1 = uv2 - uv1;//UV�̃x�N�g��
	D3DXVECTOR2 uvEdge2 = uv3 - uv1;//UV�̃x�N�g��
	D3DXVec2Normalize(&uvEdge1, &uvEdge1);//UV�̖@���x�N�g��
	D3DXVec2Normalize(&uvEdge2, &uvEdge2);//UV�̖@���x�N�g��

	float det = (uvEdge1.x * uvEdge2.y) - (uvEdge1.y * uvEdge2.x);
	det = 1.0f / det;

	tangent.x = (uvEdge2.y * edge1.x - uvEdge1.y * edge2.x) * det;
	tangent.y = (uvEdge2.y * edge1.y - uvEdge1.y * edge2.y) * det;
	tangent.z = (uvEdge2.y * edge1.z - uvEdge1.y * edge2.z) * det;

	bitangent.x = (-uvEdge2.x * edge1.x + uvEdge1.x * edge2.x) * det;
	bitangent.y = (-uvEdge2.x * edge1.y + uvEdge1.x * edge2.y) * det;
	bitangent.z = (-uvEdge2.x * edge1.z + uvEdge1.x * edge2.z) * det;

	D3DXVec3Normalize(&tangent, &tangent);
	D3DXVec3Normalize(&bitangent, &bitangent);

	D3DXVECTOR3 BiNormal;
	D3DXVec3Cross(&BiNormal, &normal, &tangent);

	
	Tangent = tangent;
	Binormal = BiNormal;
}

float Det(D3DXVECTOR3 vector0, D3DXVECTOR3 vector1, D3DXVECTOR3 vector2)
{
	return (
		(vector0.x * vector1.y * vector2.z)
		+ (vector0.y * vector1.z * vector2.x)
		+ (vector0.z * vector1.x * vector2.y)
		- (vector0.x * vector1.z * vector2.y)
		- (vector0.y * vector1.x * vector2.z)
		- (vector0.z * vector1.y * vector2.x));
}

bool CulcRay(D3DXVECTOR3 origin, D3DXVECTOR3 ray, D3DXVECTOR3 v0, D3DXVECTOR3 edge1, D3DXVECTOR3 edge2)
{
	float denominator = Det(edge1, edge2, ray);

	// ���C�����ʂƕ��s�łȂ����`�F�b�N
	if (denominator <= 0) {
		return false;
	}

	D3DXVECTOR3 d = origin - v0;

	float u = Det(d, edge2, ray) / denominator;

	if ((u >= 0) && (u <= 1))
	{
		float v = Det(edge1, d, ray) / denominator;
		if ((v >= 0) && (u + v <= 1))
		{
			float t = Det(edge1, edge2, d) / denominator;

			return true;
		}

	}
	return false;
}
