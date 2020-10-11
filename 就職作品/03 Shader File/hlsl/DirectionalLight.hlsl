//グローバル
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);



cbuffer per_mesh:register(b0)
{
	matrix g_mW;//ワールド行列
	matrix g_mWVP; //ワールドから射影までの変換合成行列
};

cbuffer per_material:register(b1)
{
	float4 g_Ambient;//環境光
	float4 g_Diffuse; //拡散反射光
	float4 g_Specular;//鏡面反射光
};

//グローバル
cbuffer per_frame:register(b2)
{
	float4 g_vLightDir;//ディレクショナルライト情報（ライトの方向）
	float4 g_vEye;//カメラ位置
	float4 g_fIntensity;//ディレクショナルライト情報（ライトの明るさ）
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{	
	float4 Pos		: SV_POSITION;
	float3 Light	: TEXCOORD0;
	float3 Normal	: TEXCOORD1;
	//float3 HalfVetor : TEXCOORD2;
	float3 EyeVector: TEXCOORD2;
	float2 Tex		: TEXCOORD3;
};
//
//バーテックスシェーダー
//
VS_OUTPUT VS( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos=mul(Pos,g_mWVP);
	//法線をモデルの姿勢に合わせる(モデルが回転すれば法線も回転させる必要があるため）
	output.Normal=mul(Norm, (float3x3)g_mW);
	//ライト方向 　ここがポイント！！　ディレクショナルはどこでも同じ方向。位置は無関係
	output.Light=normalize(g_vLightDir);
	//視線ベクトル　ワールド空間上での頂点から視点へ向かうベクトル
	float3 PosWorld = mul(Pos,g_mW);
	output.EyeVector = normalize(g_vEye - PosWorld);
	//テクスチャー座標
	output.Tex=Tex;

	return output;
}

//
//ピクセルシェーダー
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	//環境光　項
	float4 ambient=g_Ambient;
	//拡散反射光　項
	float NL=saturate(dot(input.Normal, input.Light));
	float4 diffuse = (g_Diffuse/2+g_texColor.Sample( g_samLinear, input.Tex )/2)*NL;
	//鏡面反射光　項
	float3 reflect=normalize(2*NL*input.Normal-input.Light); 
	float4 specular=pow(saturate(dot(reflect,input.EyeVector)), 4)*g_Specular;
	//フォンモデル最終色　３つの項の合計
	float4 color=ambient+diffuse+specular;
	//ライト強度を反映
	color*=g_fIntensity.x;

	return color;
}