#include "DIRECTOR.h"
#include "CameraModel.h"
#include "Scene.h"

//#define	M_CAM_POS_V_X		(0.0f)					// 視点初期位置(X座標)
//#define	M_CAM_POS_V_Y		(100.0f)				// 視点初期位置(Y座標)
//#define	M_CAM_POS_V_Z		(-200.0f)				// 視点初期位置(Z座標)
//#define	M_CAM_POS_R_X		(10.0f)					// 注視点初期位置(X座標)
//#define	M_CAM_POS_R_Y		(100.0f)				// 注視点初期位置(Y座標)
//#define	M_CAM_POS_R_Z		(10.0f)					// 注視点初期位置(Z座標)
//#define	M_VIEW_ANGLE		(D3DX_PI/4)				// ビュー平面の視野角
//#define	M_VIEW_ASPECT		((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)	// ビュー平面のアスペクト比
//#define	M_VIEW_NEAR_Z		(0.1f)											// ビュー平面のNearZ値
//#define	M_VIEW_FAR_Z		(100.0f)										// ビュー平面のFarZ値
//
//#define M_VALUE_MOVE_CAMERA		(0.1f)	//視点移動距離

static D3DXVECTOR2	model_trans_pos;
// 頂点情報の作成





CAMERA_MODEL::CAMERA_MODEL()
{
	ZeroMemory(this, sizeof(GRID));
	m_Pos = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(1, 1, 1);
	m_Rotate = D3DXVECTOR3(0, 0, 0);
}
//
CAMERA_MODEL::~CAMERA_MODEL()
{

	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);

	SAFE_RELEASE(m_pVertexBuffer);
}
//
//
//
HRESULT CAMERA_MODEL::Init()
{
	m_pDevice = Scene::m_pD3d->m_pDevice;
	m_pDeviceContext11 = Scene::m_pD3d->m_pDeviceContext;


	
	g_NumVertex = 16;

	GRID_3D	 pVtx[] =
	{
		m_Pos,

	};
	




	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(GRID_3D) * g_NumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
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
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}


	


	return S_OK;

}
//
//
//
void CAMERA_MODEL::Uninit()
{

}
//
//
//
void CAMERA_MODEL::Update()
{

}
//
//
//
void CAMERA_MODEL::Draw()
{
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;



	//使用するシェーダーの登録	
	m_pDeviceContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext11->PSSetShader(m_pPixelShader, NULL, 0);

	D3DXMATRIX mWorld, mTran, mYaw, mPitch, mRoll, mScale;

	D3DXMatrixScaling(&mScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationY(&mYaw, m_Rotate.y);
	D3DXMatrixRotationX(&mPitch, m_Rotate.x);
	D3DXMatrixRotationZ(&mRoll, m_Rotate.z);
	D3DXMatrixTranslation(&mTran, m_Pos.x, m_Pos.y, m_Pos.z);
	mWorld = mScale * mYaw * mPitch * mRoll * mTran;

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
	
	//m_pDeviceContext11->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (SUCCEEDED(m_pDeviceContext11->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{

		GRID_3D* pVtx = (GRID_3D*)pData.pData;

		pVtx[0].Pos = m_Pos;
		//pVtx[1].Pos = D3DXVECTOR3(0, 5, 0);
		pVtx[1].Pos =D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip),

		pVtx[2].Pos = m_Pos;
		pVtx[3].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[4].Pos = m_Pos;
		pVtx[5].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[6].Pos = m_Pos;
		pVtx[7].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		//--------------------------------------------------
		pVtx[8].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		pVtx[9].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[10].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		pVtx[11].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[12].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		pVtx[13].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[14].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		pVtx[15].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&pVtx), sizeof(pVtx));
		m_pDeviceContext11->Unmap(m_pVertexBuffer, 0);
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
	m_pDeviceContext11->Draw(g_NumVertex, 0);
}