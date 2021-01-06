/*�uBASE.cpp�v=============================================
�@�E�O���t�@�C����ǂݍ���Ńf�o�b�N������������B
  �E�f�B���N�g���p�X��ʂ��֐�

�@�@����ҁF������N�@	(-"-)
==========================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include "director.h"
static  std::string g_szRootPath;


static INI g_Ini;



//
//
//
INI* GetIni()
{
	return &g_Ini;
}
//
//
//�@�ݒ�p�����[�^�[�ǂݍ���
HRESULT LoadIni()
{
	SetDataDirectory();	//�f�B���N�g���w��
	FILE* fp = fopen("game.ini", "rt");

	char line[MAX_PATH];

	fgets(line, MAX_PATH, fp);
	g_Ini.MeshRender = atoi(line);//1=���b�V����\�� 0=��\��

	fgets(line, MAX_PATH, fp);
	g_Ini.SoundPlay = atoi(line);//1=�T�E���h�Đ� 0=�Đ����Ȃ�

	fgets(line, MAX_PATH, fp);
	g_Ini.CameraZ = (FLOAT)atof(line);//�J������Z�ʒu

	fclose(fp);

	return S_OK;
}

//
//
//�@������
void InitDirectory(const std::string root)
{
	g_szRootPath = root;
}

//
//
//�@���[�g�Z�b�g
void SetRootDirectory()
{
	SetCurrentDirectory(g_szRootPath.c_str());
}
//
//
//�@Sound�p�X
void SetSoundDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\04 Sound File";
	SetCurrentDirectory(tmp.c_str());
}
//
//
//�@Data�p�X
void SetDataDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\01 Data File";
	SetCurrentDirectory(tmp.c_str());
}
//
//
//�@Visual�p�X
void SetVisualDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\02 Visual File";
	SetCurrentDirectory(tmp.c_str());
}
//
//
//�@Visual�p�X2�K�w
void SetVisualDirectory(std::string file_name)
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\02 Visual File";
	tmp += "\\";
	tmp += file_name;
	SetCurrentDirectory(tmp.c_str());
}
//
//
//�@Shader�p�X
void SetShaderDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\03 Shader File";
	SetCurrentDirectory(tmp.c_str());
}
//
//
//
//std::string FindFile(const std::string path_name, const std::string file_name, const std::string file_format)
//{
//	SetRootDirectory();
//
//	HANDLE hFind;
//	WIN32_FIND_DATA fd;
//
//
//	std::string name;
//	name = path_name;
//	name += "\\";
//	name += file_name;
//	name += "\\*";
//	name += file_format;
//
//
//	hFind = FindFirstFile(name.c_str(), &fd);
//
//	// �������s? 
//	if (hFind == INVALID_HANDLE_VALUE) {
//		MSG("Fbx�ǂݍ��ݎ��s�I(�����p�X���~�X���Ă邩��)");
//		return "Error!";
//	}
//
//	// �����I��
//	FindClose(hFind);
//
//	std::string fbx_name = fd.cFileName;
//	int char_num = 0;
//	for (int i = 0; fbx_name[i] != NULL; i++)
//	{
//		char_num++;
//	}
//	std::string return_name;
//
//	//strncpy(return_name, fd.cFileName, char_num);
//	return_name = fbx_name.erase(char_num);
//
//	return return_name;
//}