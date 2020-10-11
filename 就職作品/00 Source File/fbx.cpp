/*「FBX.cpp」=============================================
　・Fbxファイル読み込みクラス
　　製作者：岡原大起　	(-"-)
=============================================================*/
#include <DirectXMath.h>
#include <fbxsdk.h>
#include "director.h"
#include "renderer.h"
#include "Scene.h"
#include "makeTangent.h"
#include "fbx.h"

void Replace(char search_char, char replace_char, char* buffer);
void Split(char split_char, char* buffer, std::vector<std::string>& out);




//
//
//
HRESULT Fbx::Uninit()
{
	//delete[] clusterDeformation;
	for (auto & mesh : m_MeshList)
	{
		//delete[] mesh.m_ClusterDeformation;
		//delete[] mesh.m_ClusterData;
		//delete[] mesh.m_Cluster;
		delete[] mesh.m_AnimVertices;
		//delete[] mesh.m_AnimCluster;
		//delete[] mesh.ClusterMatrix;
	}

	m_fbxMeshNames.clear();
	m_MeshList.clear();

	Sin_Materials().clear();
	Sin_MaterialLinks().clear();
	Sin_Textures().clear();
	Sin_NormalTextureLinks().clear();

	

	m_fbxScene->Destroy();
	m_fbxImporter->Destroy();
	m_fbxManager->Destroy();


	
	return S_OK;
}

//
//
//　初期化
HRESULT Fbx::Init(const char* file_name, bool isAnimation, int animStackNumber = 0)
{
	std::string fbx_name;
	fbx_name = FindFile("02 Visual File",file_name,".fbx");

	//TEXTUREINI FbxTexture;
	//LoadTextureFile(FbxTexture);

	m_fbxManager = FbxManager::Create();
	if (!m_fbxManager)return S_FALSE;

	m_fbxIos = FbxIOSettings::Create(m_fbxManager, IOSROOT);
	m_fbxManager->SetIOSettings(m_fbxIos);

	m_fbxImporter = FbxImporter::Create(m_fbxManager, "FInporter");
	if (!m_fbxImporter)return S_FALSE;

	// 初期化・読み込み
	m_fbxScene = FbxScene::Create(m_fbxManager, "FScene");

	SetVisualDirectory(file_name);
	//SetVisualDirectory();

	FbxString filename(fbx_name.c_str());
	m_fbxImporter->Initialize(filename.Buffer(), -1, m_fbxManager->GetIOSettings());
	
	m_fbxImporter->Import(m_fbxScene);

	
	//　すべての頂点を三角ポリゴンに変換	
	FbxGeometryConverter geometryConverter(m_fbxManager);//(自分で追加)
	// メッシュに使われているマテリアル単位でメッシュを分割する
	geometryConverter.SplitMeshesPerMaterial(m_fbxScene, true);//処理重重
	
	//geometryConverter.Triangulate(m_fbxScene, true);//処理重重
	//geometryConverter.RemoveBadPolygonsFromMeshes(m_fbxScene);

	

	int material_num = m_fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();

	for (int i = 0; i < material_num; i++){
		LoadMaterial(m_fbxScene,m_fbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
	}
	


	int meshCount = m_fbxScene->GetSrcObjectCount<FbxMesh>();	
	for (int i = 0; i < meshCount; ++i)	{
		CreateMesh(m_fbxScene->GetSrcObject<FbxMesh>(i),isAnimation);
		//LoadTextureFile(m_MeshList[i].FbxTexture,file_name);

		//for (int tex_num = 0; tex_num < m_MeshList[i].FbxTexture.TextureCount; tex_num++)
		//{
		//	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(
		//		RENDERER::m_pDevice,						// ID3D11Device
		//		m_MeshList[i].FbxTexture.TextureData[tex_num].TextureName,						// ファイル名(wchar_t型なので注意)
		//		nullptr,						// 通常のテクスチャデータ
		//		nullptr,
		//		&Sin_Textures()[m_MeshList[i].FbxTexture.TextureData[tex_num].TextureName],		// シェーダで使用することができるテクスチャデータ
		//		nullptr)))
		//	{
		//		//FbxFree(file_name);
		//		return false;
		//	}
		//}
		
	}

	/*for (int i = 0; i < meshCount; i++)
	{
	}*/
	
	LoadAnimation(isAnimation,AnimStackNumber);



	//　静的オブジェクトのみの処理
	//　アニメーションの場合動的なバッファが必要なのでDraw毎に生成する
	if (is_animation)
	{
		//int count = m_MeshList.size();
		int meshCount = m_fbxScene->GetSrcObjectCount<FbxMesh>();

		for (int mesh_num = 0; mesh_num < meshCount; mesh_num++)
		{
			FbxMesh* pMesh = m_fbxScene->GetSrcObject<FbxMesh>(mesh_num);

			int GetControlPointCount = pMesh->GetControlPointsCount();//頂点数
			int GetPolygonVertexCount = pMesh->GetPolygonVertexCount();//インデックス数


			//　頂点バッファ作成
			D3D11_BUFFER_DESC bd_vertex;
			bd_vertex.ByteWidth = sizeof(ANIMVERTEX_3D) * GetControlPointCount;
			bd_vertex.Usage = D3D11_USAGE_DEFAULT;
			bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd_vertex.CPUAccessFlags = 0;
			bd_vertex.MiscFlags = 0;
			bd_vertex.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sub_resource;
			sub_resource.pSysMem = &m_MeshList[mesh_num].m_AnimVertices[0];
			//sub_resource.SysMemPitch = 0;
			//sub_resource.SysMemSlicePitch = 0;

			RENDERER::m_pDevice->CreateBuffer(&bd_vertex, &sub_resource, &m_MeshList[mesh_num].m_VertexBuffer);

			//  インデックスデータの取り出しとバッファの設定
			D3D11_BUFFER_DESC bd_index;
			bd_index.ByteWidth = sizeof(UINT) * GetPolygonVertexCount;
			bd_index.Usage = D3D11_USAGE_DEFAULT;
			bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd_index.CPUAccessFlags = 0;
			bd_index.MiscFlags = 0;
			//bd_index.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA data_index;
			data_index.pSysMem = &m_MeshList[mesh_num].m_Indices[0];


			RENDERER::m_pDevice->CreateBuffer(&bd_index, &data_index, &m_MeshList[mesh_num].m_IndexBuffer);
		}

		//　入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC layout[]{
		{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "CLUSTER_INDEX",  0, DXGI_FORMAT_R32G32B32A32_UINT,   0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "CLUSTER_WEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }};


		//シェーダー作成
		RENDERER::CreateVertexShader("FBX_VertexShader.hlsl", "VS_FBX_ANIM", &m_VertexShader, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, layout, 6, &m_VertexLayout);
		RENDERER::CreatePixelShader("FBX_PixelShader.hlsl", "PS_FBX_ANIM", &m_PixelShader, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION);


	}
	else
	{
		//CreateVertexBuffer
		for (auto& mesh : m_MeshList)
		{
			int vertex3d_size = sizeof(VERTEX_3D);
			//頂点バッファ作成
			D3D11_BUFFER_DESC buffer_desc;
			buffer_desc.ByteWidth = sizeof(VERTEX_3D) * mesh.m_Vertices.size();	
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;			
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	
			buffer_desc.CPUAccessFlags = 0;						
			buffer_desc.MiscFlags = 0;							

			D3D11_SUBRESOURCE_DATA sub_resource;
			sub_resource.pSysMem = &mesh.m_Vertices[0];	// バッファの中身の設定
		
			
			RENDERER::m_pDevice->CreateBuffer(&buffer_desc, &sub_resource, &mesh.m_VertexBuffer);
		}
		//CreateinexBuffer
		for (auto& mesh : m_MeshList)
		{
			//インデックスバッファ作成
			D3D11_BUFFER_DESC buffer_desc;
			buffer_desc.ByteWidth = (UINT)sizeof(UINT) * mesh.m_Indices.size();	
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;							
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					
			buffer_desc.CPUAccessFlags = 0;									
			buffer_desc.MiscFlags = 0;											
						

			D3D11_SUBRESOURCE_DATA sub_resource;
			sub_resource.pSysMem = &mesh.m_Indices[0];							
							

			RENDERER::m_pDevice->CreateBuffer(&buffer_desc, &sub_resource, &mesh.m_IndexBuffer);
			
		}
		
		//　入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC layout[]{
		{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	 0,							  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };
		

		//シェーダー作成
		RENDERER::CreateVertexShader("FBX_VertexShader.hlsl", "VS_FBX", &m_VertexShader, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, layout, 4, &m_VertexLayout);
		RENDERER::CreatePixelShader("FBX_PixelShader.hlsl", "PS_FBX", &m_PixelShader, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION);
		
	}
	

	

	return S_OK;
}

//
//
//　引数のFbxSceneは今は全く必要ないからもう消してもいい
void Fbx::LoadMaterial(FbxScene* pScene,FbxSurfaceMaterial* material)
{

	FbxMaterial entry_material;
	enum class MaterialOrder
	{
		Ambient,
		Diffuse,
		Specular,
		//Bump,
		MaxOrder,
	};

	FbxDouble3 colors[(int)MaterialOrder::MaxOrder];//3
	FbxDouble factors[(int)MaterialOrder::MaxOrder];//3
	FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sAmbient);
	//	materialポインタの先にマテリアルがあれば、GetClassIDでsのマテリアルを取得
	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		const char* element_check_list[] =
		{
			FbxSurfaceMaterial::sAmbient,
			FbxSurfaceMaterial::sDiffuse,
			FbxSurfaceMaterial::sSpecular,
			//FbxSurfaceMaterial::sBump,
		};

		const char* factor_check_list[] =
		{
			FbxSurfaceMaterial::sAmbientFactor,
			FbxSurfaceMaterial::sDiffuseFactor,
			FbxSurfaceMaterial::sSpecularFactor,
			//FbxSurfaceMaterial::sBumpFactor,
		};

		//　下の”４”は上の配列の要素数
		for (int i = 0; i < (int)MaterialOrder::MaxOrder; i++)
		{
			prop = material->FindProperty(element_check_list[i]);
			if (prop.IsValid())
			{
				colors[i] = prop.Get<FbxDouble3>();

			}
			else
			{
				colors[i] = FbxDouble3(1.0, 1.0, 1.0);
			}

			prop = material->FindProperty(factor_check_list[i]);
			if (prop.IsValid())
			{
				factors[i] = prop.Get<FbxDouble>();
			}
			else
			{
				factors[i] = 1.0;
			}

			
		}
	}

	//　アンビエントプロパティを取得
	FbxDouble3 color = colors[(int)MaterialOrder::Ambient];//0
	FbxDouble factor = factors[(int)MaterialOrder::Ambient];//0
	entry_material.SetAmbient((float)color[0], (float)color[1], (float)color[2], (float)factor);

	//　デフューズプロパティ取得？初期化？
	color = colors[(int)MaterialOrder::Diffuse];//1
	factor = factors[(int)MaterialOrder::Diffuse];//1
	entry_material.SetDiffuse((float)color[0], (float)color[1], (float)color[2], (float)factor);		
	entry_material.Alpha = 1.0f;

	color = colors[(int)MaterialOrder::Specular];
	factor = factors[(int)MaterialOrder::Specular];
	entry_material.SetSpecular((float)color[0], (float)color[1], (float)color[2], (float)factor);

	Sin_Materials()[material->GetName()] = entry_material;

	// テクスチャ読み込み(シングル対応)	
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);// Diffuseプロパティを取得
	FbxFileTexture* texture = nullptr;
	std::string keyword;
	std::string normal_keyword;
	int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0)
	{
		// propからFbxFileTextureを取得	
		texture = prop.GetSrcObject<FbxFileTexture>(0);
	}
	else
	{
		// FbxLayeredTextureからFbxFileTextureを取得	
		int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layer_num > 0)
		{
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
	}


	if (texture != nullptr &&
		LoadTexture(texture, keyword,normal_keyword) == true)
	{
		// 読み込んだテクスチャとマテリアルの関係を覚えておく
		const char* tmp_char = material->GetName();
		Sin_MaterialLinks()[material->GetName()] = Sin_Textures()[keyword];

		//　ここに追加する
		if (normal_keyword != keyword)
		{
			Sin_NormalTextureLinks()[material->GetName()] =  Sin_Textures()[normal_keyword];
		}
	}


}
void Fbx::CreateMesh(FbxMesh* mesh,bool animation)
{
	MeshData mesh_data;
	LoadIndices(mesh_data, mesh,animation);
	LoadVertices(mesh_data, mesh,animation);
	LoadNormals(mesh_data, mesh,animation);
	LoadUV(mesh_data, mesh,animation);
	LoadColors(mesh_data, mesh);

	SetMaterialName(mesh_data, mesh);



	if (animation)
	{
		int* indices = mesh->GetPolygonVertices();
		for (int i = 0; i < mesh->GetPolygonCount(); i++)
		{
			int index_1 = indices[i * 3];
			int index_2 = indices[i * 3 + 1];
			int index_3 = indices[i * 3 + 2];

			D3DXVECTOR3 v1 = (D3DXVECTOR3)mesh_data.m_AnimVertices[mesh_data.m_Indices[index_1]].Position;
			D3DXVECTOR3 v2 = (D3DXVECTOR3)mesh_data.m_AnimVertices[mesh_data.m_Indices[index_2]].Position;
			D3DXVECTOR3 v3 = (D3DXVECTOR3)mesh_data.m_AnimVertices[mesh_data.m_Indices[index_3]].Position;

			D3DXVECTOR2 t1 = mesh_data.m_AnimVertices[mesh_data.m_Indices[index_1]].TexturePos;
			D3DXVECTOR2 t2 = mesh_data.m_AnimVertices[mesh_data.m_Indices[index_2]].TexturePos;
			D3DXVECTOR2 t3 = mesh_data.m_AnimVertices[mesh_data.m_Indices[index_3]].TexturePos;

			D3DXVECTOR3 vN =(
				(D3DXVECTOR3)mesh_data.m_AnimVertices[mesh_data.m_Indices[index_1]].Normal +
				(D3DXVECTOR3)mesh_data.m_AnimVertices[mesh_data.m_Indices[index_2]].Normal +
				(D3DXVECTOR3)mesh_data.m_AnimVertices[mesh_data.m_Indices[index_3]].Normal) / 3;
			D3DXVECTOR4 vTangent;

			vTangent = CalcTangent(v1, v2, v3, t1, t2, t3, vN);


			mesh_data.m_AnimVertices[mesh_data.m_Indices[index_1]].Tangent = vTangent;
			mesh_data.m_AnimVertices[mesh_data.m_Indices[index_2]].Tangent = vTangent;
			mesh_data.m_AnimVertices[mesh_data.m_Indices[index_3]].Tangent = vTangent;

		}
	}
	else
	{
		for (int i = 0; i < mesh->GetPolygonCount(); i++)
		{
			D3DXVECTOR3 v1 = (D3DXVECTOR3)mesh_data.m_Vertices[mesh_data.m_Indices[i * 3]].Position;
			D3DXVECTOR3 v2 = (D3DXVECTOR3)mesh_data.m_Vertices[mesh_data.m_Indices[i * 3 + 1]].Position;
			D3DXVECTOR3 v3 = (D3DXVECTOR3)mesh_data.m_Vertices[mesh_data.m_Indices[i * 3 + 2]].Position;

			D3DXVECTOR2 t1 = mesh_data.m_Vertices[mesh_data.m_Indices[i * 3]].TexturePos;
			D3DXVECTOR2 t2 = mesh_data.m_Vertices[mesh_data.m_Indices[i * 3 + 1]].TexturePos;
			D3DXVECTOR2 t3 = mesh_data.m_Vertices[mesh_data.m_Indices[i * 3 + 2]].TexturePos;

			D3DXVECTOR3 vN =
				((D3DXVECTOR3)mesh_data.m_Vertices[mesh_data.m_Indices[i * 3]].Normal +
					(D3DXVECTOR3)mesh_data.m_Vertices[mesh_data.m_Indices[i * 3 + 1]].Normal +
					(D3DXVECTOR3)mesh_data.m_Vertices[mesh_data.m_Indices[i * 3 + 2]].Normal) / 3;
			D3DXVECTOR4 vTangent;

			vTangent = CalcTangent(v1, v2, v3, t1, t2, t3, vN);


			mesh_data.m_Vertices[mesh_data.m_Indices[i * 3]].Tangent = vTangent;
			mesh_data.m_Vertices[mesh_data.m_Indices[i * 3 + 1]].Tangent = vTangent;
			mesh_data.m_Vertices[mesh_data.m_Indices[i * 3 + 2]].Tangent = vTangent;

		}
	}
	


	m_MeshList.push_back(mesh_data);
}
void Fbx::LoadIndices(MeshData& mesh_data, FbxMesh* mesh,bool animation)
{

	if (animation)
	{
		int index_count = mesh->GetPolygonVertexCount();
		int* indices = mesh->GetPolygonVertices();
		for (int i = 0; i < index_count; i++)
		{
			int index = indices[i];
			mesh_data.m_Indices.push_back(index);
		}
	}
	else
	{
		int polygon_count = mesh->GetPolygonCount();
		for (int i = 0; i < polygon_count; i++)
		{
			mesh_data.m_Indices.push_back(i * 3);
			mesh_data.m_Indices.push_back(i * 3 + 1);
			mesh_data.m_Indices.push_back(i * 3 + 2);
		}
	}	
}
void Fbx::LoadNormals(MeshData& mesh_data, FbxMesh* mesh,bool animation)
{
	FbxArray<FbxVector4> normals;
	// 法線リストの取得
	mesh->GetPolygonVertexNormals(normals);
	
	int polynum = mesh->GetPolygonCount();
	int* indices = mesh->GetPolygonVertices();

	if (animation)
	{
		for (int i = 0; i < normals.Size(); i++)
		{
			int index = indices[i];
			mesh_data.m_AnimVertices[index].Normal.x = (float)-normals[i][0];
			mesh_data.m_AnimVertices[index].Normal.y = (float)normals[i][1];
			mesh_data.m_AnimVertices[index].Normal.z = (float)normals[i][2];
		}
	}
	else
	{
		for (int i = 0; i < normals.Size(); i++)
		{
			int index = indices[i];
			mesh_data.m_Vertices[i].Normal.x = (float)-normals[i][0];
			mesh_data.m_Vertices[i].Normal.y = (float)normals[i][1];
			mesh_data.m_Vertices[i].Normal.z = (float)normals[i][2];
		}
	}
	
	
}
void Fbx::LoadColors(MeshData& mesh_data, FbxMesh* mesh)
{
	// 頂点カラーデータの数を確認
	int color_count = mesh->GetElementVertexColorCount();
	if (color_count == 0)
	{
		return;
	}

	// 頂点カラーデータの取得
	FbxGeometryElementVertexColor* vertex_colors = mesh->GetElementVertexColor(0);

	if (vertex_colors == nullptr)
	{
		return;
	}

	FbxLayerElement::EMappingMode mapping_mode = vertex_colors->GetMappingMode();
	FbxLayerElement::EReferenceMode reference_mode = vertex_colors->GetReferenceMode();

	if (mapping_mode == FbxLayerElement::eByPolygonVertex)
	{
		if (reference_mode == FbxLayerElement::eIndexToDirect)
		{
			// 頂点カラーバッファ取得
			FbxLayerElementArrayTemplate<FbxColor>& colors = vertex_colors->GetDirectArray();
			// 頂点カラーインデックスバッファ取得
			FbxLayerElementArrayTemplate<int>& indeces = vertex_colors->GetIndexArray();
			for (int i = 0; i < indeces.GetCount(); i++)
			{
				int id = indeces.GetAt(i);
				FbxColor color = colors.GetAt(id);
				//　残念ながらいらない
				//mesh_data.m_Vertices[i].Color.a = (float)color.mAlpha;
				//mesh_data.m_Vertices[i].Color.r = (float)color.mRed;
				//mesh_data.m_Vertices[i].Color.g = (float)color.mGreen;
				//mesh_data.m_Vertices[i].Color.b = (float)color.mBlue;
			}
		}
	}
}
void Fbx::LoadVertices(MeshData& mesh_data, FbxMesh* mesh,bool animation)
{
	FbxVector4* vertices = mesh->GetControlPoints();
	
	
	//int polygon_count = mesh->GetPolygonCount();
	

	if (animation)
	{
		int vertex_count = mesh->GetControlPointsCount();//頂点数

		//int* indices = mesh->GetPolygonVertices();
		mesh_data.m_AnimVertices = new ANIMVERTEX_3D[vertex_count];

		for (int i = 0; i < vertex_count; i++)
		{
			ANIMVERTEX_3D vertex;
			//int index = indices[i];

			// 頂点座標リストから座標を取得する
			vertex.Position.x = (float)-vertices[i][0];
			vertex.Position.z = (float)vertices[i][2];
			vertex.Position.y = (float)vertices[i][1];

			// 追加
			mesh_data.m_AnimVertices[i] = vertex;
		}
	}
	else
	{
		int polygon_vertex_count = mesh->GetPolygonVertexCount();//インデックス数
		int* indices = mesh->GetPolygonVertices();

		for (int i = 0; i < polygon_vertex_count; i++)
		{
			VERTEX_3D vertex;
			// インデックスバッファから頂点番号を取得
			int index = indices[i];

			// 頂点座標リストから座標を取得する
			vertex.Position.x = (float)-vertices[index][0];
			vertex.Position.y = (float)vertices[index][1];
			vertex.Position.z = (float)vertices[index][2];

			// 追加
			mesh_data.m_Vertices.push_back(vertex);
		}
	}


	
}
void Fbx::LoadUV(MeshData& mesh_data, FbxMesh* mesh, bool animation)
{
	FbxStringList uvset_names;
	// UVSetの名前リストを取得
	mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;
	
	// UVSetの名前からUVSetを取得する
	// 今回はマルチテクスチャには対応しないので最初の名前を使う
	mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), uv_buffer);
	
	

	//int vertex_size = mesh->GetPolygonVertexCount();

	if (animation)
	{
		int index_count = mesh->GetPolygonVertexCount();
		int* indices = mesh->GetPolygonVertices();
		for (int i = 0; i < index_count; i++)
		{
			int index = indices[i];
			FbxVector2& uv = uv_buffer[i];
			// そのまま使用して問題ない
			mesh_data.m_AnimVertices[index].TexturePos.x = (float)uv[0];
			mesh_data.m_AnimVertices[index].TexturePos.y = (float)(1.0f - uv[1]);
		}
	}
	else
	{
		int uv_size = uv_buffer.Size();

		for (int i = 0; i < uv_buffer.Size(); i++)
		{
			FbxVector2& uv = uv_buffer[i];
			// そのまま使用して問題ない
			mesh_data.m_Vertices[i].TexturePos.x = (float)uv[0];
			mesh_data.m_Vertices[i].TexturePos.y = (float)(1.0f - uv[1]);
		}
	}
	
}
void Fbx::LoadAnimation(bool isAnimation,int animStackNumber)
{
	if (!isAnimation) return;

	is_animation = true;
	FbxArray<FbxString*> AnimStackNameArray;
	m_fbxScene->FillAnimStackNameArray(AnimStackNameArray);
	AnimStackNumber = animStackNumber;
	FbxAnimStack* AnimationStack = m_fbxScene->FindMember<FbxAnimStack>(AnimStackNameArray[AnimStackNumber]->Buffer());
	m_fbxScene->SetCurrentAnimationStack(AnimationStack);

	FbxTakeInfo* takeInfo = m_fbxScene->GetTakeInfo(*(AnimStackNameArray[AnimStackNumber]));
	start = takeInfo->mLocalTimeSpan.GetStart();
	stop = takeInfo->mLocalTimeSpan.GetStop();
	FrameTime.SetTime(0, 0, 0, 1, 0, m_fbxScene->GetGlobalSettings().GetTimeMode());
	timeCount = start;

	for (int i = 0; i < m_fbxScene->GetRootNode()->GetChildCount(); i++) {

		if (m_fbxScene->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
			m_meshNode = m_fbxScene->GetRootNode()->GetChild(i);
			m_pMesh = m_meshNode->GetMesh();

			break;
		}
	}
	


	m_CurFrame = 0;
	m_MaxAnimFrame = 0;

	int count = 0;
	timeCount = 0;
	while (1){
		if (timeCount < stop){
			timeCount += FrameTime;
			count++;
		}
		else{break;}
	}

	m_MaxAnimFrame = count - 1;//0から始まるから
	timeCount = 0;

	int meshCount = m_fbxScene->GetSrcObjectCount<FbxMesh>();
	
	//for (int i = 0; i < meshCount; i++)
	//{
	//	FbxMesh* pMesh = m_fbxScene->GetSrcObject<FbxMesh>(i);
	//	FbxSkin* skinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);//タイプがSkinのものを取得
	//	int clusterCount = skinDeformer->GetClusterCount();//スキンのクラスタ数を取得

	//	//m_MeshList[i].m_AnimCluster = new Cluster[clusterCount];
	//}

	for (int mesh_num = 0; mesh_num < meshCount; mesh_num++)
	{
	
		for (int frame_num = 0; frame_num < m_MaxAnimFrame; frame_num++)
		{
		
		
			FbxMesh* pMesh = m_fbxScene->GetSrcObject<FbxMesh>(mesh_num);
			

			FbxSkin* skinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);//タイプがSkinのものを取得
			int clusterCount = skinDeformer->GetClusterCount();//スキンのクラスタ数を取得


			// <移動、回転、拡大のための行列を作成>
			FbxMatrix globalPosition = m_meshNode->EvaluateGlobalTransform(timeCount);
			FbxVector4 t0 = m_meshNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			FbxVector4 r0 = m_meshNode->GetGeometricRotation(FbxNode::eSourcePivot);
			FbxVector4 s0 = m_meshNode->GetGeometricScaling(FbxNode::eSourcePivot);
			FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);


			m_MeshList[mesh_num].ClusterAnimMatrix.emplace_back();

			// 各クラスタから各頂点に影響を与えるための行列作成
			for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {


				// クラスタ(ボーン)の取り出し
				FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);
				FbxMatrix	vertexTransformMatrix;
				FbxAMatrix	referenceGlobalInitPosition;	//初期座標
				FbxAMatrix	clusterGlobalInitPosition;	//クラスタグローバル初期座標
				FbxMatrix	clusterGlobalCurrentPosition; //クラスタグローバル現在座標
				FbxMatrix	clusterRelativeInitPosition;	//クラスタ初期座標
				FbxMatrix	clusterRelativeCurrentPositionInverse;//クラスタ現在座標

				cluster->GetTransformMatrix(referenceGlobalInitPosition);//多分Tポーズの状態取得
				referenceGlobalInitPosition *= geometryOffset;

				cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);//初期姿勢取得
				clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(timeCount);

				clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
				clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;
				vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;




				m_MeshList[mesh_num].ClusterAnimMatrix[frame_num].push_back(vertexTransformMatrix);//ClusterIndex順

			}
			timeCount += FrameTime;
		}
		timeCount = 0;
	}

	for (int mesh_num = 0; mesh_num < meshCount; mesh_num++)
	{
		FbxMesh* pMesh = m_fbxScene->GetSrcObject<FbxMesh>(mesh_num);
		FbxSkin* skinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);//タイプがSkinのものを取得
		int clusterCount = skinDeformer->GetClusterCount();//スキンのクラスタ数を取得

		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
			// クラスタ(ボーン)の取り出し
			FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);

			for (int cnt = 0; cnt < cluster->GetControlPointIndicesCount(); cnt++) {
				int index = cluster->GetControlPointIndices()[cnt];//このインデックスは頂点インデックスと同じ（だと思う）
				double weight = cluster->GetControlPointWeights()[cnt];


				for (int i = 0; i < 4; i++)
				{
					if (m_MeshList[mesh_num].m_AnimVertices[index].Weight[i] == 0.0f)//未格納検知
					{
						m_MeshList[mesh_num].m_AnimVertices[index].ClusterIndex[i] = clusterIndex;
						m_MeshList[mesh_num].m_AnimVertices[index].Weight[i] = (FLOAT)weight;
						break;
					}
				}
			}
		}	
	}
	

	//for (int mesh_num = 0; mesh_num < meshCount; mesh_num++)
	//{
	//	FbxMatrix* clusterDeformation;
	//	FbxMesh* pMesh = m_fbxScene->GetSrcObject<FbxMesh>(mesh_num);
	//	std::size_t GetControlPointCount = pMesh->GetPolygonVertexCount();//頂点数


	//	



	//	// <各頂点に掛けるための最終的な行列の配列>

	//	//clusterDeformation = new FbxMatrix[GetControlPointCount];
	//	//memset(clusterDeformation, 0, sizeof(FbxMatrix) * GetControlPointCount);


	//	//m_MeshList[mesh_num].m_ClusterData = new VertexWeight[GetControlPointCount];


	//	FbxSkin* skinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);//タイプがSkinのものを取得
	//	//FbxSkin* skinDeformer = (FbxSkin*)m_pMesh->GetDeformer(0, FbxDeformer::eSkin);//タイプがSkinのものを取得
	//	int clusterCount = skinDeformer->GetClusterCount();//スキンのクラスタ数を取得

	//	// 各クラスタから各頂点に影響を与えるための行列作成
	//	for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
	//		// クラスタ(ボーン)の取り出し
	//		FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);
	//		

	//		for (int cnt = 0; cnt < cluster->GetControlPointIndicesCount(); cnt++) {
	//			int index = cluster->GetControlPointIndices()[cnt];//このインデックスは頂点インデックスと同じ（だと思う）
	//			double weight = cluster->GetControlPointWeights()[cnt];
	//			//FbxMatrix influence = vertexTransformMatrix * weight;
	//			//clusterDeformation[index] += influence;
	//			//m_MeshList[mesh_num].m_ClusterData[index].Weight.push_back(weight);
	//			//m_MeshList[mesh_num].m_ClusterData[index].ClusterNum.push_back(clusterIndex);
	//		}
	//	}

	//	//m_MeshList[mesh_num].m_Cluster = new FbxMatrix[m_MaxAnimFrame][clusterCount];
	//	timeCount = 0;
	//	
	//	for (int frame_count = 0; frame_count < m_MaxAnimFrame; frame_count++ )
	//	{
	//		// <移動、回転、拡大のための行列を作成>
	//		FbxMatrix globalPosition = m_meshNode->EvaluateGlobalTransform(timeCount);
	//		FbxVector4 t0 = m_meshNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	//		FbxVector4 r0 = m_meshNode->GetGeometricRotation(FbxNode::eSourcePivot);
	//		FbxVector4 s0 = m_meshNode->GetGeometricScaling(FbxNode::eSourcePivot);
	//		FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);

	//		//m_MeshList[mesh_num].m_Cluster[frame_count].emplace_back();
	//		// 各クラスタから各頂点に影響を与えるための行列作成
	//		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
	//			// クラスタ(ボーン)の取り出し
	//			FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);
	//			FbxMatrix	vertexTransformMatrix;
	//			FbxAMatrix	referenceGlobalInitPosition;	//初期座標
	//			FbxAMatrix	clusterGlobalInitPosition;	//クラスタグローバル初期座標
	//			FbxMatrix	clusterGlobalCurrentPosition; //クラスタグローバル現在座標
	//			FbxMatrix	clusterRelativeInitPosition;	//クラスタ初期座標
	//			FbxMatrix	clusterRelativeCurrentPositionInverse;//クラスタ現在座標

	//			cluster->GetTransformMatrix(referenceGlobalInitPosition);//多分Tポーズの状態取得
	//			referenceGlobalInitPosition *= geometryOffset;

	//			cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);//初期姿勢取得
	//			clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(timeCount);

	//			clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
	//			clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;
	//			vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

	//			m_MeshList[mesh_num].m_Cluster.emplace_back();
	//			m_MeshList[mesh_num].m_Cluster[frame_count].push_back(vertexTransformMatrix);

	//		}
	//		timeCount += FrameTime;
	//	}
	//}

}

void Fbx::SetMaterialName(MeshData& mesh_data, FbxMesh* mesh)
{
	// マテリアルが無ければ終わり
	if (mesh->GetElementMaterialCount() == 0)
	{
		mesh_data.m_MaterialName = "";
		return;
	}

	// Mesh側のマテリアル情報を取得
	FbxLayerElementMaterial* material = mesh->GetElementMaterial(0);
	
	
	
	int mat_count = mesh->GetElementMaterialCount();//マテリアルの数
	int index = material->GetIndexArray().GetAt(0);
	int index_count = material->GetIndexArray().GetCount();
	FbxSurfaceMaterial* surface_material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

	if (surface_material != nullptr)
	{
		mesh_data.m_MaterialName = surface_material->GetName();
	}
	else
	{
		mesh_data.m_MaterialName = "";
	}
}
void Fbx::SetMaterialColor(FbxMaterial& material)
{
	MATERIAL* constant = &m_CBufferData_Material;
	constant->Diffuse = D3DXVECTOR4(material.Diffuse[0], material.Diffuse[1], material.Diffuse[2], material.Diffuse[3]);
	constant->Ambient = D3DXVECTOR4(material.Ambient[0], material.Ambient[1], material.Ambient[2], material.Ambient[3]);
	constant->Specular = D3DXVECTOR4(material.Diffuse[0], material.Diffuse[1], material.Diffuse[2], material.Diffuse[3]);//分からんからとりあえずNULL回避
}
void Replace(char search_char, char replace_char, char* buffer)
{
	int len = (int)strlen(buffer);

	for (int i = 0; i < len; i++)
	{
		if (buffer[i] == search_char)
		{
			buffer[i] = replace_char;
		}
	}
}
void Split(char split_char, char* buffer, std::vector<std::string>& out)
{
	int count = 0;
	if (buffer == nullptr)
	{
		return;
	}

	int start_point = 0;

	while (buffer[count] != '\0')
	{
		if (buffer[count] == split_char)
		{
			if (start_point != count)
			{
				char split_str[256] = { 0 };
				strncpy_s(split_str, 256, &buffer[start_point], count - start_point);
				out.emplace_back(split_str);
			}
			else
			{
				out.emplace_back("");
			}
			start_point = count + 1;
		}
		count++;
	}

	if (start_point != count)
	{
		char split_str[256] = { 0 };
		strncpy_s(split_str, 256, &buffer[start_point], count - start_point);
		out.emplace_back(split_str);
	}
}
bool Fbx::LoadTexture(FbxFileTexture* texture, std::string& keyword, std::string& normal_keyword)
{
	if (texture == nullptr)
	{
		return false;
	}

	// ファイル名を取得
	std::string file_path = texture->GetRelativeFileName();

	// ファイル分解
	
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, file_path.c_str(), sizeof(char) * 256);

	// 記号統一
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// 「/」で分解
	Split('/', buffer, split_list);


	// 文字化け対策
	char* file_name;
	size_t size = 0;
	//動的メモリ確保なのでFbxFreeで開放させなければいけない
	FbxUTF8ToAnsi(split_list[split_list.size() - 1].c_str(), file_name, &size);

	//一度テクスチャを読み込んでいれば、再び読み込む必要はない
	//マテリアルとの関係だけ設定させる
	if (Sin_Textures().count(file_name) > 0 &&
		Sin_Textures()[file_name] != nullptr)
	{
		keyword = file_name;
		normal_keyword = file_name;
		FbxFree(file_name);
		return true;
	}

	
	
	

	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(
		RENDERER::m_pDevice,						// ID3D11Device
		file_name,						// ファイル名(wchar_t型なので注意)
		nullptr,						// 通常のテクスチャデータ
		nullptr,
		&Sin_Textures()[file_name],		// シェーダで使用することができるテクスチャデータ
		nullptr)))	
	{
		FbxFree(file_name);
		return false;
	}
	keyword = file_name;
	normal_keyword = file_name;
	//----------------------------------------------------
	size_t normal_char_len = strlen(file_name);//残す文字数
	size_t erase_len = (1 + 7 + 1 + 3);//_deffuse.png(12文字)

	normal_char_len = normal_char_len - erase_len;
	std::string str_normal = "deffuse";

	std::string normal_char = file_name;
	std::string normal_tag = "_normal.png";
	normal_char.erase(normal_char_len, erase_len);
	std::string normal_name = normal_char + normal_tag;//　完成形


	if (str_normal.compare(0, strlen(file_name), file_name))
	{
		ID3D11Texture2D* pNormalTexture;
		if (FAILED(D3DX11CreateTextureFromFileA(RENDERER::m_pDevice, normal_name.c_str(), NULL, NULL, (ID3D11Resource**)&pNormalTexture, NULL)))
		{
			normal_keyword = file_name;
			FbxFree(file_name);
			return E_FAIL;
		}
		//ノーマルマップのテクスチャーのシェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		D3D11_TEXTURE2D_DESC desc;
		pNormalTexture->GetDesc(&desc);

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		HRESULT hr = RENDERER::m_pDevice->CreateShaderResourceView(pNormalTexture, &srvDesc, &Sin_Textures()[normal_name]);

	}
	normal_keyword = normal_name;
	FbxFree(file_name);
	return true;
}

//
//
//
void Fbx::Draw() {

	
	// ----- Animation -----
	if (is_animation)
	{
		
		timeCount += FrameTime;
		if (timeCount > stop) 
		{
			timeCount = start;		
		}
		m_CurFrame++;
		if (m_CurFrame >= m_MaxAnimFrame)
		{
			m_CurFrame = 0;
		}


		RenderAnimation();

	}
	else
	{
		RenderModel();
	}

}

//
//
//　描画
void Fbx::RenderModel()
{
	RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_VertexLayout);
	
	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT strides = sizeof(VERTEX_3D);
	UINT offsets = 0;
	for (auto& mesh : m_MeshList)
	{
		
		// IAに設定する頂点バッファの指定
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer, &strides, &offsets);
		RENDERER::m_pDeviceContext->IASetIndexBuffer(mesh.m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);


		SetMaterialColor(Sin_Materials()[mesh.m_MaterialName]);	
		RENDERER::SetMaterial(m_CBufferData_Material);


		// テクスチャ設定
		if (Sin_MaterialLinks().count(mesh.m_MaterialName) > 0)
		{
			ID3D11ShaderResourceView *tmp_Textures = Sin_MaterialLinks()[mesh.m_MaterialName];
			
			//RENDERER::m_pDeviceContext->PSSetSamplers(0, 1, &RENDERER::m_pSamplerState);
			RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &tmp_Textures);

			//　ノーマルマップあれば入れる
			if (Sin_NormalTextureLinks().count(mesh.m_MaterialName) > 0)
			{
				//RENDERER::m_pDeviceContext->PSSetSamplers(1, 1, &RENDERER::m_pSamplerState);
				//RENDERER::m_pDeviceContext->PSSetShaderResources(1, 1, &Sin_NormalTextureLinks()[mesh.m_MaterialName]);
			}
		}
		else
		{
			// Samplerの設定
			//RENDERER::m_pDeviceContext->PSSetSamplers(0, 1, &RENDERER::samplerState);
			// PixelShaderで使用するテクスチャの設定
			//RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, nullptr);
			
			
		}	
		


		// 描画
		RENDERER::m_pDeviceContext->DrawIndexed(mesh.m_Indices.size(),0,	0);	
	}
}


void Fbx::RenderAnimation() {

	RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_VertexLayout);
	

	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int Count = m_MeshList.size();
	for (int mesh_num = 0; mesh_num < Count; mesh_num++)
	{
		FbxMesh* pMesh = m_fbxScene->GetSrcObject<FbxMesh>(mesh_num);
		int GetPolygonVertexCount = pMesh->GetPolygonVertexCount();//インデックス数
		int GetControlPointCount = pMesh->GetControlPointsCount();//頂点数

		FbxSkin* skinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);//タイプがSkinのものを取得
		int clusterCount = skinDeformer->GetClusterCount();//スキンのクラスタ数を取得

		// <移動、回転、拡大のための行列を作成>
		//FbxMatrix globalPosition = m_meshNode->EvaluateGlobalTransform(timeCount);
		//FbxVector4 t0 = m_meshNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		//FbxVector4 r0 = m_meshNode->GetGeometricRotation(FbxNode::eSourcePivot);
		//FbxVector4 s0 = m_meshNode->GetGeometricScaling(FbxNode::eSourcePivot);
		//FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);

		
		
		// <各頂点に掛けるための最終的な行列の配列>

		
		//memset(m_MeshList[mesh_num].m_ClusterDeformation, 0, sizeof(FbxMatrix) * GetControlPointCount);

		//FbxSkin* skinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);//タイプがSkinのものを取得
		//int clusterCount = skinDeformer->GetClusterCount();//スキンのクラスタ数を取得

		


		//int* indices = pMesh->GetPolygonVertices();

		/*for (int vertex_count = 0; vertex_count < GetControlPointCount; vertex_count++)
		{

			int index = indices[vertex_count];

			FbxMatrix OutMatrix;
			memset(&OutMatrix, 0, sizeof(FbxMatrix));
			D3DXVECTOR3 vertex;
			int cluster_size = m_MeshList[mesh_num].m_ClusterData[index].ClusterNum.size();
			
			for (int i = 0; i < m_MeshList[mesh_num].m_ClusterData[index].ClusterNum.size(); i++)
			{
				int cluster_index = m_MeshList[mesh_num].m_ClusterData[index].ClusterNum[i];
				double weight = m_MeshList[mesh_num].m_ClusterData[index].Weight[i];
				OutMatrix += m_MeshList[mesh_num].m_Cluster[m_CurFrame].at(cluster_index) * weight;
			}

			FbxVector4 outVertex = OutMatrix.MultNormalize(pMesh->GetControlPointAt(index));

			vertex.x = -(FLOAT)outVertex[0];
			vertex.y = (FLOAT)outVertex[1];
			vertex.z = (FLOAT)outVertex[2];
			m_MeshList[mesh_num].m_Vertices[vertex_count].Position += vertex;
		}

		for (int  cluster_num = 0; cluster_num < clusterCount; cluster_num++)
		{
			//int vertexIndex 
			FbxMatrix cluster =  m_MeshList[mesh_num].m_Cluster[m_CurFrame].at(cluster_num);


		}*/

		/*for (int cluster_num = 0; cluster_num < clusterCount; cluster_num++)
		{
			for (int vertex_index_num = 0; vertex_index_num < m_MeshList[mesh_num].m_AnimCluster[cluster_num].vertexIndex.size(); vertex_index_num++)
			{
				int index = m_MeshList[mesh_num].m_AnimCluster[cluster_num].vertexIndex[vertex_index_num];
				double weight = m_MeshList[mesh_num].m_AnimCluster[cluster_num].weight[vertex_index_num];

				FbxMatrix OutMatrix;
				FbxVector4 OutVertex;
				OutMatrix = m_MeshList[mesh_num].ClusterAnimMatrix[m_CurFrame].at(cluster_num) * weight;
				OutVertex = OutMatrix.MultNormalize(pMesh->GetControlPointAt(index));

				
			}
		}*/
		CLUSTER Cluster;
		for (int i = 0; i < 127; i++)
		{
			D3DXMatrixIdentity(&Cluster.Cluster[i]);
			//D3DXMatrixTranspose(&Cluster.Cluster[i],&Cluster.Cluster[i]);
		}

		for (int cluster_num = 0; cluster_num < clusterCount; cluster_num++)
		{
			FbxMatrix matrix = m_MeshList[mesh_num].ClusterAnimMatrix[m_CurFrame].at(cluster_num);
			D3DXMATRIX Matrix = D3DXMATRIX(
				(float)matrix[0][0], (float)matrix[0][1], (float)matrix[0][2], (float)matrix[0][3],
				(float)matrix[1][0], (float)matrix[1][1], (float)matrix[1][2], (float)matrix[1][3],
				(float)matrix[2][0], (float)matrix[2][1], (float)matrix[2][2], (float)matrix[2][3],
				(float)matrix[3][0], (float)matrix[3][1], (float)matrix[3][2], (float)matrix[3][3]);
				
			
			Cluster.Cluster[cluster_num] = Matrix;
			//D3DXMatrixTranspose(&Cluster.Cluster[cluster_num], &Cluster.Cluster[cluster_num]);
		}

		
		RENDERER::SetCluster(Cluster);


		//delete[] clusterDeformation;


		//delete[] pCluster;

		/*D3D11_MAPPED_SUBRESOURCE pdata;
		RENDERER::m_pDeviceContext->Map(m_MeshList[mesh_num].m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&m_MeshList[mesh_num].m_AnimVertices[0]), sizeof(ANIMVERTEX_3D) * GetControlPointCount);
		RENDERER::m_pDeviceContext->Unmap(m_MeshList[mesh_num].m_VertexBuffer, 0);*/



		UINT stride = sizeof(ANIMVERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_MeshList[mesh_num].m_VertexBuffer, &stride, &offset);
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_MeshList[mesh_num].m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);



		SetMaterialColor(Sin_Materials()[m_MeshList[mesh_num].m_MaterialName]);


		// テクスチャ設定
		if (Sin_MaterialLinks().count(m_MeshList[mesh_num].m_MaterialName) > 0)
		{
			ID3D11ShaderResourceView* tmp_Textures = Sin_MaterialLinks()[m_MeshList[mesh_num].m_MaterialName];
			// Samplerの設定
			//RENDERER::m_pDeviceContext->PSSetSamplers(0, 1, &RENDERER::m_pSamplerState);
			// PixelShaderで使用するテクスチャの設定
			RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &tmp_Textures);

		}
		else
		{
			// Samplerの設定
			//m_context->PSSetSamplers(0, 1, &m_pSampleLinear);
			// PixelShaderで使用するテクスチャの設定
			//m_context->PSSetShaderResources(0, 1, nullptr);
		}




		// <描画実行>

		//RENDERER::m_pDeviceContext->Draw(GetControlPointCount, 0);
		RENDERER::m_pDeviceContext->DrawIndexed(GetPolygonVertexCount, m_MeshList[mesh_num].m_Indices[0], 0);

	}
	/*for (int mesh_num = 0; mesh_num < Count; mesh_num++)
	{


		// <移動、回転、拡大のための行列を作成>
		//FbxMatrix globalPosition = m_meshNode->EvaluateGlobalTransform(timeCount);
		//FbxVector4 t0 = m_meshNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		//FbxVector4 r0 = m_meshNode->GetGeometricRotation(FbxNode::eSourcePivot);
		//FbxVector4 s0 = m_meshNode->GetGeometricScaling(FbxNode::eSourcePivot);
		//FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);

		FbxMesh* pMesh = m_fbxScene->GetSrcObject<FbxMesh>(mesh_num);

		std::size_t GetControlPointCount = m_MeshList[mesh_num].m_Vertices.size();//頂点数


		//memset(m_MeshList[mesh_num].clusterDeformation, 0, sizeof(FbxMatrix) * GetControlPointCount);
		

		//FbxSkin* skinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);//デフォーマーを取得し、スキンにキャスト
		//int clusterCount = skinDeformer->GetClusterCount();//スキンのクラスター数を取得

		// 各クラスタから各頂点に影響を与えるための行列作成
		//for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
		//	// クラスタ(ボーン)の取り出し
		//	FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);
		//	FbxMatrix vertexTransformMatrix;
		//	FbxAMatrix referenceGlobalInitPosition;
		//	FbxAMatrix clusterGlobalInitPosition;
		//	FbxMatrix clusterGlobalCurrentPosition;
		//	FbxMatrix clusterRelativeInitPosition;
		//	FbxMatrix clusterRelativeCurrentPositionInverse;

		//	cluster->GetTransformMatrix(referenceGlobalInitPosition);
		//	referenceGlobalInitPosition *= geometryOffset;

		//	cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
		//	clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(timeCount);

		//	clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
		//	clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;
		//	vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

		//	//  上で作った行列に各頂点毎の影響度(重み)を掛けてそれぞれに加算
		//	int ControlPointIndicesCount = cluster->GetControlPointIndicesCount();
		//	
		//	for (int cnt = 0; cnt < ControlPointIndicesCount; cnt++) {
		//		int index = cluster->GetControlPointIndices()[cnt];
		//		double weight = cluster->GetControlPointWeights()[cnt];
		//		FbxMatrix influence = vertexTransformMatrix * weight;
		//		m_MeshList[mesh_num].clusterDeformation[index] += influence;
		//	}
		//}

		
		
		// 最終的な頂点座標を計算しVERTEXに変換
		//int count = mesh->GetControlPointsCount();

		//int ver_num = GetControlPointCount;
		//D3D11_MAPPED_SUBRESOURCE msr;
		//RENDERER::m_pDeviceContext->Map(m_MeshList[i].m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		//tmp_vertices = (VERTEX_3D*)msr.pData;

		


		//int* indices = m_pMesh->GetPolygonVertices();
		//
		//for (int cnt = 0; cnt < GetControlPointCount; cnt++) {

		//	int index = indices[cnt];
		//	FbxVector4 outVertex = m_MeshList[mesh_num].clusterDeformation[index].MultNormalize(pMesh->GetControlPointAt(index));

		//	
		//	D3DXVECTOR3 vertex;
		//	vertex.x = -(FLOAT)outVertex[0];
		//	vertex.y = (FLOAT)outVertex[1];
		//	vertex.z = (FLOAT)outVertex[2];

		//	//VERTEX_3D vertex;
		//	tmp_vertices[cnt].Position.x = -vertex.x;
		//	tmp_vertices[cnt].Position.y = vertex.y;
		//	tmp_vertices[cnt].Position.z = vertex.z;

		//	vertex.Position.x = -x;
		//	vertex.Position.y = y;
		//	vertex.Position.z = z;
		//	m_MeshList[mesh_num].m_Vertices[cnt].Position = vertex;
		//	tmp_vertices[cnt].Normal = m_MeshList[mesh_num].m_Vertices[cnt].Normal;
		//	tmp_vertices[cnt].TexturePos = m_MeshList[mesh_num].m_Vertices[cnt].TexturePos;
		//	tmp_vertices[cnt].Tangent = m_MeshList[mesh_num].m_Vertices[cnt].Tangent;
		//}

		VERTEX_3D* tmp_vertices = new VERTEX_3D[GetControlPointCount];
		for (int cnt = 0; cnt < GetControlPointCount; cnt++) {
			tmp_vertices[cnt].Position = m_MeshList[mesh_num].m_AnimationData[m_CurFrame].at(cnt);
		}

		for (int cnt = 0; cnt < GetControlPointCount; cnt++) {
			tmp_vertices[cnt].Normal = m_MeshList[mesh_num].m_Vertices[cnt].Normal;
			tmp_vertices[cnt].TexturePos = m_MeshList[mesh_num].m_Vertices[cnt].TexturePos;
			tmp_vertices[cnt].Tangent = m_MeshList[mesh_num].m_Vertices[cnt].Tangent;
		}

		//　パラメータの受け渡し(頂点)
		//D3D11_MAPPED_SUBRESOURCE pdata;
		//RENDERER::m_pDeviceContext->Map(m_MeshList[mesh_num].m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
		//memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&m_MeshList[mesh_num].m_Vertices[0]), sizeof(VERTEX_3D) * GetControlPointCount);
		//RENDERER::m_pDeviceContext->Unmap(m_MeshList[mesh_num].m_VertexBuffer, 0);

		D3D11_MAPPED_SUBRESOURCE pdata;
		RENDERER::m_pDeviceContext->Map(m_MeshList[mesh_num].m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(tmp_vertices), sizeof(VERTEX_3D) * GetControlPointCount);
		RENDERER::m_pDeviceContext->Unmap(m_MeshList[mesh_num].m_VertexBuffer, 0);

	
		//m_context->UpdateSubresource(VerBuffer, 0, NULL, &m_MeshList[i].m_Vertices[0], 0, 0);



		//delete[] clusterDeformation;
		delete[] tmp_vertices;

		// ---------------------
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_MeshList[mesh_num].m_VertexBuffer, &stride, &offset);
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_MeshList[mesh_num].m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);



		SetMaterialColor(Sin_Materials()[m_MeshList[mesh_num].m_MaterialName]);


		// テクスチャ設定
		if (Sin_MaterialLinks().count(m_MeshList[mesh_num].m_MaterialName) > 0)
		{
			ID3D11ShaderResourceView* tmp_Textures = Sin_MaterialLinks()[m_MeshList[mesh_num].m_MaterialName];
			// Samplerの設定
			//RENDERER::m_pDeviceContext->PSSetSamplers(0, 1, &RENDERER::m_pSamplerState);
			// PixelShaderで使用するテクスチャの設定
			RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &tmp_Textures);

		}
		else
		{
			// Samplerの設定
			//m_context->PSSetSamplers(0, 1, &m_pSampleLinear);
			// PixelShaderで使用するテクスチャの設定
			//m_context->PSSetShaderResources(0, 1, nullptr);
		}


		

		// <描画実行>
		int polygonVertexCount = GetControlPointCount;
		//RENDERER::m_pDeviceContext->DrawIndexed(polygonVertexCount, 0, 0);
		RENDERER::m_pDeviceContext->Draw(polygonVertexCount, 0);
		//m_context->DrawIndexed(m_MeshList[i].m_Indices.size(),0,0);


	}*/
	
}