#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"



VS_PL_OUT main(
	float4 Pos		: POSITION,
	float4 Normal	: NORMAL,
	float2 Tex		: TEXCOORD,
	float4 Tangent	: TANGENT,
	float4 Binormal : BINORMAL)
{
    VS_PL_OUT output = (VS_PL_OUT) 0;

    output.LightIndex = g_mWorld._33;//index��scale��z�l�ɓ��ꂢ�Ă���

    matrix world = g_mWorld;
    world._33 = world._22 = world._11;//scale�͂����W�ɓ����Ă���l��yz�ɂ����f������

	matrix WVP;
    WVP = mul(world, g_mView);
	WVP = mul(WVP, g_mProj);

	output.Pos = mul(Pos, WVP);
    output.WorldPos = mul(Pos, world);
    output.LightPos = float3(world._41, world._42, world._43); //�}�g���N�X����ʒu���W���擾
    output.WorldNormal = mul(Normal, world).xyz;
    output.LightRange = world._11;
    output.Tex = Tex;
	
	return output;

}
