// EsTool.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "EsTool.h"
#include "FrameWorksConstant.h"

//#include "../vld/vld.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
#ifdef _DEBUG
	std::string strDrive;
	std::string strPath;
	std::string strFilename;
	std::string strExt;

	char szModulePath[MAX_PATH] = "";
	GetModuleFileNameA( NULL , szModulePath , MAX_PATH );
	GwStringUtil::SplitPath ( szModulePath , strDrive , strPath , strFilename , strExt );
	std::string strCurrDir = strDrive + strPath;

	SetCurrentDirectoryA( strCurrDir.c_str());
#endif

	ISingletonObject::Initialize();
	CEsToolApplication * pEsToolApplication = new CEsToolApplication();
	CFrameWorks::CreateInstance();
	CFrameWorks::GetInstance()->OnCreateMainFrameWorks(pEsToolApplication, hInstance, g_nWindowModeWidth, g_nWindowModeHeight, L"EsTool");
	pEsToolApplication->Release();
	CFrameWorks::DestroyInstance();

	return 0;
}