/*�uDIRECTOR.h�v=============================================
�@�E�S�ẴV�[���𓝊�����N���X
�@�@����ҁF������N�@	(-"-)
=============================================================*/
#pragma once
#include "window.h"
#include "Scene.h"
//#include "Scene.h"
//�@�v���g�^�C�v
//HRESULT GameInit(HINSTANCE phInstance);
//void	GameRun();
//HRESULT GameUninit();

class Scene;

class Base
{
private:
	static class Scene* m_Scene;
	static void MainLoop();
	static void FixFPS60();
public:

	static HRESULT Init(HINSTANCE phInstance);
	static HRESULT Run();
	static HRESULT Uninit();

	static class Scene* GetScene() { return m_Scene; }

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
