#include "postProcess.h"
#include "fog.h"
#include "directionalLight.h"


class CGameScenePostProcess : public CPostProcess
{
private:

public:
	void Init()
	{
		AddPostProcess<CDirectionalLight>();
		AddPostProcess<CFogEffect>();
	}
	/*void UnInit();
	void Update();
	void Imgui();*/
};
