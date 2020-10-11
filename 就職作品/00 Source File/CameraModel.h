#pragma once

class CAMERA_MODEL
{
	//頂点の構造体
	struct GRID_3D
	{
		D3DXVECTOR3 Pos; //位置
	};
	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;
	};

public:
	CAMERA_MODEL();
	~CAMERA_MODEL();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR3		m_Pos;// 地面の位置
	D3DXVECTOR3		m_Scale;//
	D3DXVECTOR3		m_Rotate;
private:

	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext* m_pDeviceContext11 = NULL;

	ID3D11InputLayout* m_pVertexLayout = NULL;
	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11PixelShader* m_pPixelShader = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;
	//↓モデルごと	
	ID3D11Buffer* m_pVertexBuffer = NULL;	// 頂点バッファへのポインタ


	int g_NumVertex;
	int g_NumIndex;

	float FarClip = 10.0f;
	float NearClip = 0.1f;
	float Aspect = 1280 / 720;
	float Angle = D3DX_PI/4;
};