#pragma once

#include "director.h"
#include "gameobject.h"
#include "Imgui11.h"




class Scene
{
protected:

	enum LAYER
	{
		DRAW_LAYER_HIDE = 0,//��`�惌�C���[
		DRAW_LAYER_DRAW = 1,//��ʓI�ȃI�u�W�F�N�g���C���[
		DRAW_LAYER_LIGHT = 2,//���C�e�B���O���C���[
		DRAW_LAYER_MAX = 3,//���C���[��
	};

	std::list<CGameObject*> m_GameObject[LAYER::DRAW_LAYER_MAX];

public:	
	Scene(){}
	virtual ~Scene(){}


	virtual void Init() = 0;

	virtual void UnInit() {
		for (int i = 0; i < LAYER::DRAW_LAYER_MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}

			m_GameObject[i].clear();
		}
	}

	virtual void Draw() {
		for (int i = 0; i < LAYER::DRAW_LAYER_LIGHT; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
	}

	virtual void DrawLighting() {
		//Lighting�p�X�����`��
		for (CGameObject* object : m_GameObject[LAYER::DRAW_LAYER_MAX - 1])
		{
			object->Draw();
		}
	}

	virtual void Update() {
		for (int i = 0; i < LAYER::DRAW_LAYER_MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Update();
			}
		}
	}

	virtual void Imgui() {	
		for (int i = 0; i < LAYER::DRAW_LAYER_MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Imgui();
			}
		}
	}
	
	template <typename T>
	T* AddGameObject(int Layer)
	{
		T* gameobject = new T();
		m_GameObject[Layer].push_back(gameobject);
		gameobject->Init();

		return gameobject;
	}

	template <typename T>
	T* GetGameObject(int Layer)
	{
		for (CGameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))//�@�^�𒲂ׂ�
			{
				return (T*)object;
			}
		}

		return NULL;
	}

	template <typename T>
	std::vector<T*>GetGameObjects(int Layer)
	{
		std::vector<T*> objects;
		for (CGameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))//�@�^�𒲂ׂ�
			{
				objects.push_back((T*)object);
			}
		}

		return objects;
	}
};
