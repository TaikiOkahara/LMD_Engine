#pragma once

#include "director.h"
#include "gameobject.h"
#include "Imgui11.h"




class Scene
{
	enum DRAW_FASE
	{
		DRAW_FASE_MAX = 3,
		DRAW_FASE_NO_LIGHT = 2,
	};

protected:
	std::list<CGameObject*> m_GameObject[DRAW_FASE::DRAW_FASE_MAX];

public:	
	Scene(){}
	virtual ~Scene(){}


	virtual void Init() = 0;

	virtual void UnInit() {
		for (int i = 0; i < DRAW_FASE::DRAW_FASE_MAX; i++)
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
		for (int i = 0; i < DRAW_FASE::DRAW_FASE_NO_LIGHT; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
	}

	virtual void DrawLighting() {
		//LightingÉpÉXÇæÇØï`âÊ
		for (CGameObject* object : m_GameObject[DRAW_FASE::DRAW_FASE_MAX - 1])
		{
			object->Draw();
		}
	}

	virtual void Update() {
		for (int i = 0; i < DRAW_FASE::DRAW_FASE_MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Update();
			}
		}
	}

	virtual void Imgui() {

		// IMGUIÅ@Frame start
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		for (int i = 0; i < DRAW_FASE::DRAW_FASE_MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Imgui();
			}
		}

		ImGui::EndFrame();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		//ImGui::set
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
			if (typeid(*object) == typeid(T))//Å@å^Çí≤Ç◊ÇÈ
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
			if (typeid(*object) == typeid(T))//Å@å^Çí≤Ç◊ÇÈ
			{
				objects.push_back((T*)object);
			}
		}

		return objects;
	}
};
