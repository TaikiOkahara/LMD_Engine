#include "DIRECTOR.h"
#include "grid.h"
#include "Scene.h"

#define	GRID_SIZE		(10.0f)						// 地面の幅
#define GRID_LINE		(10)
#define VERTEX_NUM		((GRID_LINE * 2) * 2 + 4)

GRID::GRID()
{

	ZeroMemory(this, sizeof(GRID));
	m_Pos = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(1, 1, 1);
	m_Yaw = 0;
	m_Pitch = 0;
	m_Roll = 0;
}

GRID::~GRID()
{
	//SAFE_DELETE(pD3d);
	//SAFE_DELETE(m_pDeviceContext11);
	
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);

	SAFE_RELEASE(m_pVertexBuffer);
	//SAFE_RELEASE(m_pTexture);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT GRID::Init()
{
	m_pDevice = Scene::m_pD3d->m_pDevice;
	m_pDeviceContext11 = Scene::m_pD3d->m_pDeviceContext;
	

	// 頂点情報の作成


	
	g_NumVertex = VERTEX_NUM;

	GRID_3D	 pVtx[(GRID_LINE * 2) * 2 + 4];//(+4は最後の2本)
	for (int x = 0; x < GRID_LINE + 1; x++)
	{
		for (int z = 0; z < 2; z++)
		{
			pVtx[(x * 2) + z].Pos.x = m_Pos.x + (GRID_SIZE / 2) - ((GRID_SIZE / GRID_LINE * 1.0f) * x);
			pVtx[(x * 2) + z].Pos.y = m_Pos.y;
			pVtx[(x * 2) + z].Pos.z = m_Pos.z - (GRID_SIZE / 2) + (GRID_SIZE * z);
		}
	}
	for (int z = GRID_LINE + 1; z < (GRID_LINE * 2) + 2; z++)
	{
		for (int x = 0; x < 2; x++)
		{
			pVtx[(z * 2) + x].Pos.x = m_Pos.x - (GRID_SIZE / 2) + (GRID_SIZE * x);
			pVtx[(z * 2) + x].Pos.y = m_Pos.y;
			pVtx[(z * 2) + x].Pos.z = m_Pos.z + (GRID_SIZE/GRID_LINE) + (GRID_SIZE / 2) - ((GRID_SIZE / GRID_LINE * 1.0f) * (z - GRID_LINE));
		}
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(GRID_3D) * 	g_NumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vrData;
	vrData.pSysMem = pVtx;


	if (FAILED(m_pDevice->CreateBuffer(&bd,				// 頂点データ用に確保するバッファサイズ(バイト単位)
		&vrData,			// 頂点バッファの使用法　
		&m_pVertexBuffer	// 頂点バッファインターフェースへのポインタ
	)))					// NULLに設定
	{
		return E_FAIL;
	}

	//バーテックスバッファーをセット
	UINT stride = sizeof(GRID_3D);
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	SetShaderDirectory();
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"Line3D.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"Line3D.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;//D3D11_USAGE_DEFAULT


	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void GRID::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void GRID::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void GRID::Draw()
{

	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;

	//使用するシェーダーの登録	
	m_pDeviceContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext11->PSSetShader(m_pPixelShader, NULL, 0);

	D3DXMATRIX mWorld, mTran, mYaw, mPitch, mRoll, mScale;

	D3DXMatrixScaling(&mScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationY(&mYaw, m_Yaw);
	D3DXMatrixRotationX(&mPitch, m_Pitch);
	D3DXMatrixRotationZ(&mRoll, m_Roll);
	D3DXMatrixTranslation(&mTran, m_Pos.x, m_Pos.y, m_Pos.z);
	mWorld = mScale * mYaw*mPitch*mRoll*mTran;

	//ワールド、カメラ、射影行列を渡す
	CAMERA_DATA camera_data = Scene::m_Camera.GetCameraData();

	D3DXMATRIX m = mWorld * camera_data.View * camera_data.Projection;
	D3DXMatrixTranspose(&m, &m);

	SIMPLESHADER_CONSTANT_BUFFER scb;
	if (SUCCEEDED(m_pDeviceContext11->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		
		scb.mWVP = m;



		memcpy_s(pData.pData, pData.RowPitch, (void*)(&scb), sizeof(scb));
		m_pDeviceContext11->Unmap(m_pConstantBuffer, 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウトをセット
	m_pDeviceContext11->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	m_pDeviceContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	UINT stride = sizeof(GRID_3D);
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//m_pDeviceContext11->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//プリミティブをレンダリング
	m_pDeviceContext11->Draw(g_NumVertex,0);

}