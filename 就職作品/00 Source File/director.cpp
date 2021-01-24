/*---------------------------------------
*�@director.cpp
* �C���N���[�h���ꊇ�ōs��
* �f�B���N�g���p�X��ύX����֐��Q
*@author�FOkahara Taiki
----------------------------------------*/
#include <iostream>
#include <fstream>
#include "director.h"

static  std::string g_szRootPath;
static INI g_Ini;



INI* GetIni()
{
	return &g_Ini;
}


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


//�@������
void InitDirectory(const std::string root)
{
	g_szRootPath = root;
}


//�@���[�g�Z�b�g
void SetRootDirectory()
{
	SetCurrentDirectory(g_szRootPath.c_str());
}

void SetSoundDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\04 Sound File";
	SetCurrentDirectory(tmp.c_str());
}


void SetDataDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\01 Data File";
	SetCurrentDirectory(tmp.c_str());
}

void SetVisualDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\02 Visual File";
	SetCurrentDirectory(tmp.c_str());
}


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

void SetShaderDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\03 Shader File";
	SetCurrentDirectory(tmp.c_str());
}
