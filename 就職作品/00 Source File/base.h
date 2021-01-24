/*---------------------------------------
*　base.h
* メインループを実現し、すべてのシーンを統括するクラス
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
#include "window.h"
#include "scene.h"


class Base
{
private:
	static class CScene* m_Scene;

	static void MainLoop();
public:

	static HRESULT Init(HINSTANCE phInstance);
	static HRESULT Run();
	static HRESULT Uninit();

	static class CScene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene() {
		if (m_Scene != NULL)
		{
			m_Scene->UnInit();
			delete m_Scene;
		}

		T* scene = new T();
		m_Scene = scene;

		scene->Init();
	}
};
