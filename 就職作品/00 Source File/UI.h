/*「UI.h」=============================================
　・GUI、ビルボードの実装クラス
　　製作者：岡原大起　	(-"-)
=============================================================*/
#pragma once

//UIナンバー
enum UI_TYPE
{
	BILLBOARD,
	GUI,

	TYPE_MAX
};


//
//
//　クラス
class UI
{
public:
	//頂点の構造体
	struct UIVertex
	{
		D3DXVECTOR3 Pos; //位置
		D3DXVECTOR2 UV; //テクスチャー座標
	};
	//
	struct UI_SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;

	};
public:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Scale;
	float m_Yaw, m_Pitch, m_Roll;
	
	HRESULT Draw();	//	 GUI
	HRESULT Draw(D3DXMATRIX& mView, D3DXMATRIX& mProj);//　ビルボード
	HRESULT Init(LPSTR,D3DXVECTOR2 pos);//　GUI	
	HRESULT Init(LPSTR,D3DXVECTOR3 pos);//　ビルボード

	UI();	//　コンストラクタ
	~UI();	//　デストラクタ
private:

	HWND m_hWnd;//　ウィンドウハンドル
	ID3D11Device* m_pDevice;//　デバイスポインタ
	ID3D11DeviceContext* m_pDeviceContext;//　デバイス環境ポインタ
	//　バックバッファ
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	//　バッファ
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11SamplerState* m_pSampler;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（用意するのはリソースビューだけでいい）

	//　メンバ関数
	HRESULT InitShader(LPSTR , LPSTR , LPSTR , void** , ID3DBlob** );
	HRESULT InitModel();
	VOID RenderSprite(D3DXMATRIX);
	int m_Type;//UIの種類（GUIか、ビルボードか）
};
