#include "director.h"
#include "gameobject.h"
#include "InstanceObject.h"
#include "calculation.h"
#include "collision.h"

//FLOAT LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0);

//頂点3つとUV座標３つと法線をもとに接線を計算する。返す4次元ベクトルのwは左か右手系かの判断用フラグ。
void CalcTangent(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3,
	D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR3 normal, D3DXVECTOR3 Tangent, D3DXVECTOR3 Binormal)
{
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 bitangent;

	D3DXVECTOR3 edge1 = v2 - v1;//ベクトル
	D3DXVECTOR3 edge2 = v3 - v1;//ベクトル
	D3DXVec3Normalize(&edge1, &edge1);//法線ベクトル
	D3DXVec3Normalize(&edge2, &edge2);//法線ベクトル
	D3DXVECTOR2 uvEdge1 = uv2 - uv1;//UVのベクトル
	D3DXVECTOR2 uvEdge2 = uv3 - uv1;//UVのベクトル
	D3DXVec2Normalize(&uvEdge1, &uvEdge1);//UVの法線ベクトル
	D3DXVec2Normalize(&uvEdge2, &uvEdge2);//UVの法線ベクトル

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

	// レイが平面と平行でないかチェック
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


// 分離軸に投影された軸成分から投影線分長を算出
FLOAT LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	FLOAT r1 = fabsf(D3DXVec3Dot(Sep, e1));
	FLOAT r2 = fabsf(D3DXVec3Dot(Sep, e2));
	FLOAT r3 = e3 ? (fabsf(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}


// OBB v.s. OBB
bool ColOBBs(CGameObject* obb1, CGameObject* obb2)
{
	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	D3DXVECTOR3 NAe1 = obb1->GetForward(), Ae1 = NAe1 * obb1->GetCollision()->GetZsize();
	D3DXVECTOR3 NAe2 = obb1->GetUp(), Ae2 = NAe2 * obb1->GetCollision()->GetYsize();
	D3DXVECTOR3 NAe3 = obb1->GetRight(), Ae3 = NAe3 * obb1->GetCollision()->GetXsize();
	D3DXVECTOR3 NBe1 = obb2->GetForward(), Be1 = NBe1 * obb2->GetCollision()->GetZsize();
	D3DXVECTOR3 NBe2 = obb2->GetUp(), Be2 = NBe2 * obb2->GetCollision()->GetYsize();
	D3DXVECTOR3 NBe3 = obb2->GetRight(), Be3 = NBe3 * obb2->GetCollision()->GetXsize();
	D3DXVECTOR3 Interval = obb1->GetPosition() - obb2->GetPosition();

	// 分離軸 : Ae1
	FLOAT rA = D3DXVec3Length(&Ae1);
	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	FLOAT L = fabsf(D3DXVec3Dot(&Interval, &NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	 // 分離軸 : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabsf(D3DXVec3Dot(&Interval, &NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabsf(D3DXVec3Dot(&Interval, &NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be1);
	L = fabsf(D3DXVec3Dot(&Interval, &NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be2);
	L = fabsf(D3DXVec3Dot(&Interval, &NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be3);
	L = fabsf(D3DXVec3Dot(&Interval, &NBe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}


// OBB v.s. OBB
bool ColOBBs(CGameObject* obb1, CInstanceGameObject* obb2,int index)
{
	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	D3DXVECTOR3 NAeZ = obb1->GetForward(), AeZ = NAeZ * obb1->GetCollision()->GetZsize() * obb1->GetScale().z;
	D3DXVECTOR3 NAeY = obb1->GetUp(), AeY = NAeY * obb1->GetCollision()->GetYsize() * obb1->GetScale().y;
	D3DXVECTOR3 NAeX = obb1->GetRight(), AeX = NAeX * obb1->GetCollision()->GetXsize() * obb1->GetScale().x;
	D3DXVECTOR3 NBeZ = obb2->GetForward(index), BeZ = NBeZ * obb2->GetCollision()->GetZsize() * obb2->GetScale(index).z;
	D3DXVECTOR3 NBeY = obb2->GetUp(index), BeY = NBeY * obb2->GetCollision()->GetYsize() * obb2->GetScale(index).y;
	D3DXVECTOR3 NBeX = obb2->GetRight(index), BeX = NBeX * obb2->GetCollision()->GetXsize() * obb2->GetScale(index).x;
	D3DXVECTOR3 Interval = obb1->GetPosition() + obb1->GetCollision()->GetPosition() - obb2->GetPosition(index) - obb2->GetCollision()->GetPosition();



	// 分離軸 : AeZ
	FLOAT rA = D3DXVec3Length(&AeZ);
	FLOAT rB = LenSegOnSeparateAxis(&NAeZ, &BeZ, &BeY, &BeX);
	FLOAT L = fabsf(D3DXVec3Dot(&Interval, &NAeZ));
	if (L > rA + rB)
		return false; // 衝突していない

	 // 分離軸 : AeY
	rA = D3DXVec3Length(&AeY);
	rB = LenSegOnSeparateAxis(&NAeY, &BeZ, &BeY, &BeX);
	L = fabsf(D3DXVec3Dot(&Interval, &NAeY));
	if (L > rA + rB)
		return false;

	// 分離軸 : AeX
	rA = D3DXVec3Length(&AeX);
	rB = LenSegOnSeparateAxis(&NAeX, &BeZ, &BeY, &BeX);
	L = fabsf(D3DXVec3Dot(&Interval, &NAeX));
	if (L > rA + rB)
		return false;

	// 分離軸 : BeZ
	rA = LenSegOnSeparateAxis(&NBeZ, &AeZ, &AeY, &AeX);
	rB = D3DXVec3Length(&BeZ);
	L = fabsf(D3DXVec3Dot(&Interval, &NBeZ));
	if (L > rA + rB)
		return false;

	// 分離軸 : BeY
	rA = LenSegOnSeparateAxis(&NBeY, &AeZ, &AeY, &AeX);
	rB = D3DXVec3Length(&BeY);
	L = fabsf(D3DXVec3Dot(&Interval, &NBeY));
	if (L > rA + rB)
		return false;

	// 分離軸 : BeX
	rA = LenSegOnSeparateAxis(&NBeX, &AeZ, &AeY, &AeX);
	rB = D3DXVec3Length(&BeX);
	L = fabsf(D3DXVec3Dot(&Interval, &NBeX));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAeZ, &NBeZ);
	rA = LenSegOnSeparateAxis(&Cross, &AeY, &AeX);
	rB = LenSegOnSeparateAxis(&Cross, &BeY, &BeX);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &NAeZ, &NBeY);
	rA = LenSegOnSeparateAxis(&Cross, &AeY, &AeX);
	rB = LenSegOnSeparateAxis(&Cross, &BeZ, &BeX);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &NAeZ, &NBeX);
	rA = LenSegOnSeparateAxis(&Cross, &AeY, &AeX);
	rB = LenSegOnSeparateAxis(&Cross, &BeZ, &BeY);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &NAeY, &NBeZ);
	rA = LenSegOnSeparateAxis(&Cross, &AeZ, &AeX);
	rB = LenSegOnSeparateAxis(&Cross, &BeY, &BeX);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &NAeY, &NBeY);
	rA = LenSegOnSeparateAxis(&Cross, &AeZ, &AeX);
	rB = LenSegOnSeparateAxis(&Cross, &BeZ, &BeX);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &NAeY, &NBeX);
	rA = LenSegOnSeparateAxis(&Cross, &AeZ, &AeX);
	rB = LenSegOnSeparateAxis(&Cross, &BeZ, &BeY);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &NAeX, &NBeZ);
	rA = LenSegOnSeparateAxis(&Cross, &AeZ, &AeY);
	rB = LenSegOnSeparateAxis(&Cross, &BeY, &BeX);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &NAeX, &NBeY);
	rA = LenSegOnSeparateAxis(&Cross, &AeZ, &AeY);
	rB = LenSegOnSeparateAxis(&Cross, &BeZ, &BeX);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &NAeX, &NBeX);
	rA = LenSegOnSeparateAxis(&Cross, &AeZ, &AeY);
	rB = LenSegOnSeparateAxis(&Cross, &BeZ, &BeY);
	L = fabsf(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;


	D3DXVECTOR3  length = obb1->GetPosition() + obb1->GetCollision()->GetPosition() - (obb2->GetPosition(index) + obb2->GetCollision()->GetPosition());

	D3DXVec3Length(&length);




	// 分離平面が存在しないので「衝突している」
	return true;
}


