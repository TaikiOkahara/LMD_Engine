#pragma once
//
//
//
class GRID
{
	//頂点の構造体
	struct GRID_3D
	{
		D3DXVECTOR3 Pos; //位置
	};
public:

	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;
	};
public:
	HRESULT Init();
	void	Uninit(void);
	void	Update(void);
	void	Draw();

	D3DXVECTOR3		m_Pos;// 地面の位置
	D3DXVECTOR3		m_Scale;//
	float m_Yaw, m_Pitch, m_Roll;

	GRID();
	~GRID();

private:

	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext *m_pDeviceContext11 = NULL;

	ID3D11InputLayout* m_pVertexLayout = NULL;
	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11PixelShader* m_pPixelShader = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;
	//↓モデルごと	
	ID3D11Buffer* m_pVertexBuffer = NULL;	// 頂点バッファへのポインタ
	//ID3D11Buffer* m_pIndexBuffer;	// 頂点バッファへのポインタ
	//ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー
	//ID3D11ShaderResourceView* m_pTexture;//テクスチャ


	int g_NumVertex;
	int g_NumIndex;
};