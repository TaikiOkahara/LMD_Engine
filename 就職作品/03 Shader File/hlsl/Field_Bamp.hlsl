//ディフューズ色がテクスチャー色に置き換わる　

//グローバル
Texture2D g_texColor: register(t0);
Texture2D g_texNormal: register(t1);
SamplerState g_samLinear : register(s0);

//グローバル
cbuffer per_WorldMatrix: register(b0)
{
	matrix g_mW;//ワールド行列
	matrix g_mWIT;//ワールド行列の逆行列の転置行列
	matrix g_mWVP; //ワールドから射影までの変換行列
};

cbuffer per_Material: register(b1)
{
	float4 g_Ambient;//アンビエント光
	float4 g_Diffuse;//拡散反射(色）
	float4 g_Specular;//鏡面反射
};

//　ディレクショナルライト + カメラ位置
cbuffer per_Light : register(b2)
{
	float4 g_vLight;//ライトの方向ベクトル
	float4 g_fIntensity;//明るさ
	float4 g_vEye;//視点、カメラ位置
}

//　ポイントライト
cbuffer per_Light_2 : register(b3)
{
	float4  g_vLightPos[10];		//	座標

	//　xしか使用していないのに毎回float4呼ぶのはめんどくさい
	//　ｃBufferに変換するときにそれぞれの軸に割り当てれば、効率がいい
	float4	g_fIntensity_point[10];	//　明るさ
	float4	g_Range[10];			//　範囲
	float4	g_AttRate[10];			//　減衰率

	float4	g_fNumLight;			//　ライト総数

	//float4	Dummy = 0;
}

//構造体
struct VS_OUTPUT
{
	float4 Pos			: SV_POSITION;
	float3 WorldPos		: POSITION;
	float3 Light		: TEXCOORD0;//　ディレクショナルライト
	float3 Normal		: TEXCOORD1;
	float3 HalfVector	: TEXCOORD2;
	float3 EyeVector	: TEXCOORD3;
	float2 Tex			: TEXCOORD4;
};

//
//バーテックスシェーダー
//　引数がFbxVertexの要素と同じ
VS_OUTPUT VS(
	float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD,
	float4 Tangent : TANGENT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 worldPos = mul(Pos, g_mW).xyz;
	float3 LightVector = normalize(g_vLight).xyz;//
	float3 EyeVector = normalize(g_vEye.xyz - worldPos);
	float3 HalfVector = normalize(LightVector + EyeVector);//　ノーマルマッピングには必須

	float3 T = mul(Tangent.xyz, (float3x3)g_mWIT);
	float3 N = mul(Normal.xyz, (float3x3)g_mWIT);
	float3 B = cross(N, T) * Tangent.w;

	float3x3 TangentToObject = float3x3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z);//　ノーマルマッピングには必須
	output.Normal = mul(Normal,g_mW);
	// ローカル座標 * ワールド座標変換行列
	output.WorldPos = mul(Pos, g_mW);
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;
	output.HalfVector = mul(HalfVector, TangentToObject);
	output.Light = mul(LightVector, TangentToObject);

	return output;
}
//
//
//
float4 PLight(float4 Ambient,float4 Diffuse, float4 Specular,float3 Pos,float3 LightDir,float3 LPos,float range,float rate,float Intensity,float3 Normal,float2 Tex, float3 EyeVector)
{

	
	float NL_2 = saturate(dot(Normal, LightDir));//
	//　ポイントライト分さらに乗じる
	//float4 diffuse = (Diffuse)*NL_2;
	float4 diffuse = (Diffuse / 2 + g_texColor.Sample(g_samLinear, Tex)/2)*NL_2;

	//鏡面反射光　項
	float3 Reflect = normalize(2 * NL_2 * Normal -LightDir);
	float4 specular = pow(saturate(dot(Reflect, EyeVector)), 4)*Specular;

	float4 ambient = Ambient;


	//フォンモデル最終色　３つの項の合計
	float4 Color = ambient + diffuse + specular;

	Color *= Intensity;

	//減衰
	float Distance = length(LPos - Pos)/ range;// / 100;//ここで割る値によって範囲を変更できる
	Color *= pow(saturate(1 / Distance), rate);//適当に1単位から減衰開始。ここの値で減衰率を変更できる

	return Color;
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 HalfVector = normalize(input.HalfVector);
	float3 Light = normalize(input.Light);
	float3 Normal = normalize(g_texNormal.Sample(g_samLinear, input.Tex).rgb * 2.0f - 1.0f);
	float NH = saturate(dot(Normal, HalfVector));
	//　鏡面反射光　項
	float NL = saturate(dot(Normal, Light));

	//float Power = (NL == 0.0f) ? 0.0f : pow(NH,4);

	//　ディレクショナルライト
	float4 Diffuse = ((g_Diffuse *NL) / 2);	
	float4 Ambient = g_Ambient;		
	//float4 Specular = g_Specular; *Power;
	float3 Reflect = normalize(2 * NL * Normal - Light);
	float4 Specular = pow(saturate(dot(Reflect, input.EyeVector)), 4) * g_Specular;
	
	float4 DirectionalColor = Ambient + Diffuse + Specular;
	DirectionalColor *= g_fIntensity.x;

	//　ポイントライト
	float3 WorldPos = normalize(input.WorldPos);
	float4 PointColor;
	for (int i = 0; i < g_fNumLight.x; i++)
	{		
		float3 LightDir = (normalize(g_vLightPos[i] - WorldPos));
		PointColor += PLight(g_Ambient, g_Diffuse, g_Specular, input.WorldPos,LightDir, g_vLightPos[i], g_Range[i].x, g_AttRate[i].x, g_fIntensity_point[i].x,input.Normal,input.Tex,input.EyeVector);
	}


	//　今はディレクショナルライトとポイントライトの結果を合成しただけで、
	//　ポイントライトのほうはノーマルマッピングされていない
	//　理想はその二つのライトの合成されたものからノーマルマッピングしたい
	//
	float4 FinalColor = PointColor + DirectionalColor;//　ポイントライトとディレクショナルライトを合成


	return FinalColor;
}