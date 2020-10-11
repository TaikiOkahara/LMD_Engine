/*「FBX.h」=============================================
　・Fbxファイル読み込みクラス
　　製作者：岡原大起　	(-"-)
=============================================================*/
#pragma once
#include <fbxsdk.h>
//
//
//
class Fbx
{
public:
	
	//struct VertexWeight
	//{
	//	std::vector<double> Weight;//ウェイト
	//	std::vector<int> ClusterNum;//クラスターのインデックス番号
	//};

	//struct Cluster
	//{
	//	int ClusterIndex;//クラスター番号
	//	
	//	std::vector<int> vertexIndex;//クラスタの影響を受ける頂点インデックス
	//	std::vector<double> weight;//影響度
	//};

	struct MeshData
	{
		ID3D11Buffer* m_VertexBuffer = nullptr;	//!< @brief 頂点バッファ(Shader送信用)
		ID3D11Buffer* m_IndexBuffer = nullptr;	//!< @brief インデックスバッファ(Shader送信用)
		std::vector<VERTEX_3D> m_Vertices;
		ANIMVERTEX_3D* m_AnimVertices = nullptr;

		std::vector<UINT> m_Indices;
		std::string m_MaterialName;

		//std::vector <std::vector<D3DXVECTOR3>> m_AnimationData;//めっちゃ大きいサイズになりそう
		//FbxMatrix* m_ClusterDeformation = nullptr;
		//VertexWeight* m_ClusterData = nullptr;

		//std::vector<std::vector<FbxMatrix>> m_Cluster;
		//Cluster* m_AnimCluster = nullptr;
		
		std::vector< std::vector<FbxMatrix>> ClusterAnimMatrix;
		TEXTUREINI FbxTexture;
	};
	
	struct FbxMaterial
	{
		FbxMaterial()
		{
			for (int i = 0; i < 4; i++)
			{
				Ambient[i] = 1.0f;
				Diffuse[i] = 1.0f;
				Specular[i] = 1.0f;
			}
		}

		void SetAmbient(float r, float g, float b, float factor)
		{
			Ambient[0] = r;
			Ambient[1] = g;
			Ambient[2] = b;
			Ambient[3] = factor;
		}

		void SetDiffuse(float r, float g, float b, float factor)
		{
			Diffuse[0] = r;
			Diffuse[1] = g;
			Diffuse[2] = b;
			Diffuse[3] = factor;
		}

		void SetSpecular(float r, float g, float b, float factor)
		{
			Specular[0] = r;
			Specular[1] = g;
			Specular[2] = b;
			Specular[3] = factor;
		}

		float Ambient[4];
		float Diffuse[4];
		float Specular[4];
		float Alpha;
		
	};

public:

	Fbx(){ ZeroMemory(this, sizeof(Fbx)); }
	~Fbx(){}


	HRESULT Init(const char* file_name, bool isAnimation, int animStackNumber);
	HRESULT Uninit();
	void Draw();

private:
	

	FbxNode*				m_meshNode = nullptr;
	FbxMesh*				m_pMesh = NULL;


	FbxManager*				m_fbxManager = nullptr;
	FbxImporter*			m_fbxImporter = nullptr;
	FbxIOSettings*			m_fbxIos = nullptr;
	FbxScene*				m_fbxScene = nullptr;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	//ID3D11VertexShader* m_AnimVertexShader;
	//ID3D11PixelShader* m_AnimPixelShader;

	ID3D11InputLayout* m_VertexLayout;
	//ID3D11InputLayout* m_AnimVertexLayout;

	//アニメーション用バッファ
	/*ID3D11Buffer *m_AnimVerBuffer = NULL;
	ID3D11Buffer *m_AnimIndBuffer = NULL;
	VERTEX_3D* AnimVertices;*/



	int AnimStackNumber = 0;
	FbxTime FrameTime, timeCount, start, stop;
	bool is_animation = false;//アニメーションのあるFBXかどうか
	int m_MaxAnimFrame;
	int m_CurFrame;
	//FbxMatrix* clusterDeformation = nullptr;


	MATERIAL m_CBufferData_Material;//!< @brief ConstantBufferデータ


	std::vector<std::string> m_fbxMeshNames;
	std::vector<MeshData> m_MeshList;
	

	static std::map<std::string, FbxMaterial>& Sin_Materials(){
		static std::map<std::string, FbxMaterial> m_Materials;
		return m_Materials;
	}

	static std::map<std::string, ID3D11ShaderResourceView*>& Sin_MaterialLinks(){
		static std::map<std::string, ID3D11ShaderResourceView*> m_MaterialLinks;
		return m_MaterialLinks;
	}
	static std::map < std::string, ID3D11ShaderResourceView*>& Sin_Textures(){
		static std::map< std::string, ID3D11ShaderResourceView*> m_Textures;
		return m_Textures;
	}
	
	static std::map<std::string, ID3D11ShaderResourceView*>& Sin_NormalTextureLinks(){
		static std::map<std::string, ID3D11ShaderResourceView*> m_NormalTextureLinks;
		return m_NormalTextureLinks;
	}



	bool LoadTexture(FbxFileTexture* texture, std::string & keyword,std::string& normal_keyword);
	void LoadMaterial(FbxScene* pScene,FbxSurfaceMaterial* material);
	void LoadIndices(MeshData& mesh_data, FbxMesh* mesh,bool animation);
	void LoadNormals(MeshData& mesh_data, FbxMesh* mesh,bool animation);
	void LoadColors(MeshData& mesh_data, FbxMesh* mesh);
	void LoadVertices(MeshData& mesh_data, FbxMesh* mesh,bool animation);
	void LoadUV(MeshData& mesh_data, FbxMesh* mesh,bool animation);
	void LoadAnimation(bool isAnimation,int AnimStackNumber);

	void SetMaterialName(MeshData& mesh_data, FbxMesh* mesh);
	void SetMaterialColor(FbxMaterial& material);

	void CreateMesh(FbxMesh* mesh,bool animation);


	void RenderModel();
	void RenderAnimation();
};