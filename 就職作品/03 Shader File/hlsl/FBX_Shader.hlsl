//グローバル
Texture2D g_texColor: register(t0);
Texture2D g_texNormal: register(t1);
SamplerState g_samLinear : register(s0);

#define PI 3.1415926535897

cbuffer WorldBuffer : register(b0)
{
	matrix g_mWorld;//ワールド行列
	matrix g_mWIT;//ワールドの逆の転置行列
	//matrix g_mWVP;//ワールドから射影までの変換行列
}

cbuffer ViwBuffer : register(b1)
{
	matrix g_mView;//ワールド行列
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix g_mProj;//ワールド行列
}

cbuffer MaterialBuffer: register(b3)
{
	float4 g_Ambient;//アンビエント光
	float4 g_Diffuse;//拡散反射(色）
	float4 g_Specular;//鏡面反射
}

//　ディレクショナルライト
cbuffer DirectionalLightBuffer : register(b4)
{
	float4 g_vLight;//ライトの方向ベクトル
	float4 g_fIntensity;//明るさ	
	float4 g_vEye;//視点、カメラ位置
}

//　ポイントライト
cbuffer PointLightBuffer : register(b5)
{
	float4  g_vLightPos[10];		//	座標

	float4	g_fIntensity_point[10];	//　明るさ
	float4	g_Range[10];			//　範囲
	float4	g_AttRate[10];			//　減衰率

	float4	g_fNumLight;			//　ライト総数

	//float Dummy;
}

struct VS_OUTPUT
{
	float4 Pos			: SV_POSITION;
	float3 WorldPos		: POSITION;
	float3 Light		: TEXCOORD0;//	
	float3 Normal		: TEXCOORD1;//	法線
	float3 HalfVector	: TEXCOORD2;//　ノーマルマップに必要（接線？）
	//float3 EyeVector	: TEXCOORD3;//　カメラ視点
	float2 Tex			: TEXCOORD4;
	float3 LightTanDir  : TEXCOORD5;
};


VS_OUTPUT VS(
	float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD,
	float4 Tangent : TANGENT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	matrix WVP;
	WVP = mul(g_mWorld, g_mView);
	WVP = mul(WVP, g_mProj);


	//　射影変換
	float3 worldPos = mul(Pos, g_mWorld).xyz;

	
	float3 LightVector = -g_vLight.xyz;
	
	float3 EyeVector = normalize(g_vEye.xyz - worldPos);
	//float3 EyeVector = normalize(g_vEye.xyz);

	float3 HalfVector = normalize(LightVector + EyeVector);//鏡面反射光のためのハーフベクトル

	float3 T = mul(Tangent.xyz, (float3x3)g_mWIT);
	float3 N = mul(Normal.xyz, (float3x3)g_mWIT);
	float3 B = cross(N, T) * Tangent.w;	//

	float3x3 TangentToObject = float3x3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z);
	//　法線をモデルの姿勢に合わせる（モデルが回転すれば法線も回転させる必要があるため）
	output.Normal = Normal.xyz;// normalize(mul(Normal, g_mWorld));

	
	output.WorldPos = mul(Pos, g_mWorld).xyz;
	output.Pos = mul(Pos, WVP);
	output.Tex = Tex;

	//VSよりPSのほうが圧倒的に処理負荷が高いので、毎回行う計算はできるだけVS内で行う
	//法線の正規化もCPU側で事前に行っておく

	

	output.HalfVector = HalfVector;
	//output.HalfVector = mul(HalfVector, TangentToObject);
	output.Light = LightVector;
	
	output.LightTanDir = mul(LightVector, TangentToObject);
	
	return output;
}

//
//
//
float4 PLight(float4 Ambient, float4 Diffuse, float4 Specular, float3 Pos, float3 LightDir, float3 LPos, float range, float rate, float Intensity, float3 Normal, float2 Tex, float3 EyeVector)
{


	float NL_2 = saturate(dot(Normal, LightDir));//
	//　ポイントライト分さらに乗じる
	//float4 diffuse = (Diffuse)*NL_2;
	float4 diffuse = (Diffuse / 2 + g_texColor.Sample(g_samLinear, Tex) / 2)*NL_2;

	//鏡面反射光　項
	float3 Reflect = normalize(2 * NL_2 * Normal - LightDir);
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

	float3 Normal = normalize(g_texNormal.Sample(g_samLinear, input.Tex).rgb * 2.0f - 1.0f);
	//clip(length(Normal) - 0.1);
	
	float3 Bright = (dot(input.LightTanDir,Normal));
	Bright = max(0.0f, Bright);
	
	float NH = saturate(dot(Normal, HalfVector));
	//　拡散反射光

	//float NL = saturate(dot(input.Normal,input.Light));
	float NL = saturate(dot(input.Normal, input.Light));
	NL = NL * 0.9f + 0.1f;
	//float NV = saturate(dot(Normal,input.EyeVector));
	//float VH = saturate(dot(input.EyeVector, input.HaltVector));
	//float LV = saturate(dot(Light, input.EyeVector));

	
	float3 LightColor = float3(1, 1, 1);
	float3 irradistance = NL * LightColor;
	//float3 irradistance = NL * LightColor;
	//return float4(irradistance, 1);

	//　鏡面反射光
	float Power = (NL == 0.0f) ? 0.0f : pow(NH,1);//この4は、鏡面反射強度。0から10まで。大きいほど広がりがシャープ
	//Eye,Half

	//float3 Diffuse = (g_Diffuse/2 + g_texColor.Sample(g_samLinear, input.Tex)/2);//鏡面反射光は今は消してる
	float3 Diffuse = (g_texColor.Sample(g_samLinear, input.Tex).xyz);//鏡面反射光は今は消してる
	float3 Specular;// = Diffuse.xyz;

	Diffuse = Diffuse * irradistance;// / PI;
	

	float4 Ambient = g_Ambient;			//　ディレクショナルライト
	Specular = g_Specular.xyz * Power;
	
	return float4(Diffuse, 1);


	//float4 DirectionalColor = Ambient;// +Diffuse + Specular;

	////DirectionalColor *= g_fIntensity.x;//　今はなしにする	

	//float3 WorldPos = normalize(input.WorldPos);
	//float4 PointColor;
	//for (int i = 0; i < g_fNumLight.x; i++)
	//{
	//	float3 LightDir = normalize(g_vLightPos[i].xyz - WorldPos);
	//	PointColor += PLight(g_Ambient, g_Diffuse, g_Specular, input.WorldPos,LightDir, g_vLightPos[i].xyz, g_Range[i].x, g_AttRate[i].x, g_fIntensity_point[i].x,Normal,input.Tex,input.EyeVector);
	//}


	////　今はディレクショナルライトとポイントライトの結果を合成しただけで、
	////　ポイントライトのほうはノーマルマッピングされていない
	////　理想はその二つのライトの合成されたものからノーマルマッピングしたい
	////
	//float4 FinalColor = PointColor + DirectionalColor;//　ポイントライトとディレクショナルライトを合成


	//return FinalColor;
}
