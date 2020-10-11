//グローバル
cbuffer global: register(b0)
{
	matrix g_WVP; //ワールドから射影までの変換行列
};
//構造体
struct PS_INPUT
{
	float4 Normal : SV_POSITION;
};

//
//
//バーテックスシェーダー
PS_INPUT VS( float4 Pos : POSITION ) 
{
	PS_INPUT Out;
	Out.Normal =mul(Pos,g_WVP);
	return Out;
}
//
//
//ピクセルシェーダー
float4 PS( PS_INPUT Input ) : SV_Target
{
	return float4(0,1,0,1);
}