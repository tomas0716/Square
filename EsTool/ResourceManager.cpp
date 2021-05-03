#include "StdAfx.h"
#include "ResourceManager.h"

CResourceManager::CResourceManager(void)
{
}

CResourceManager::~CResourceManager(void)
{
	map<string,IGwRefObject*>::iterator itr = m_ResourceTable.begin();

	for( ; itr != m_ResourceTable.end(); ++itr )
	{
		itr->second->Release();
	}

	m_ResourceTable.clear();
}

void					CResourceManager::Begin()
{
	Gw::GetBillboardMgr()->BeginTextureMerge();
}

void					CResourceManager::End()
{
	Gw::GetBillboardMgr()->EndTextureMerge();
}

IGwBillboardTex		*	CResourceManager::CreateBillboardTex(const char * pszFilePath, const char * pszFileName)
{
	char	strMediaPathOld[STRING_MAX] = "";
	StringCchCopyA(strMediaPathOld, STRING_MAX, Gw::GetMtrlMgr()->GetMediaPath());
	Gw::GetMtrlMgr()->SetMediaPath(pszFilePath);

	IGwBillboardTex * pTex = Gw::GetBillboardMgr()->CreateBillboardTextureMerge_InPak(GwNullString, pszFileName);

	Gw::GetMtrlMgr()->SetMediaPath(strMediaPathOld);

	return pTex;
}

IGwStaticObject		*	CResourceManager::CreateStaticObject(const char * pszFilePath, const char * pszTexturePath, const char * pszFileName)
{
	IGwStaticObject* pStaticObject = NULL;

	char	strMediaPathOld[STRING_MAX] = "";
	StringCchCopyA(strMediaPathOld, STRING_MAX, Gw::GetBundleMgr()->GetMediaPath());

	char	strTexturePathOld[STRING_MAX] = "";
	StringCchCopyA(strTexturePathOld, STRING_MAX, Gw::GetBundleMgr()->GetTexturePath());

	Gw::GetBundleMgr()->SetMediaPath(pszFilePath);
	Gw::GetBundleMgr()->SetTexturePath(pszTexturePath);

	IGwBundle*	pBundle = Gw::GetBundleMgr()->Load_InPak(pszFileName);
	if(pBundle != NULL)
	{
		pStaticObject = Gw::GetScnMgr()->CreateStaticObject(pBundle, SHADERTECH_VS_TOONSHADEONLY, SHADERTECH_VSSHADOW);
		pBundle->Release();
	}

	Gw::GetBundleMgr()->SetMediaPath(strMediaPathOld);
	Gw::GetBundleMgr()->SetTexturePath(strTexturePathOld);

	return pStaticObject;
}

IGwAnimSequence		*	CResourceManager::CreateAnimSequence(const char * pszFilePath, const char * pszFileName)
{
	IGwAnimSequence* pAnimSequence = NULL;

	char	strMediaPathOld[STRING_MAX] = "";
	StringCchCopyA(strMediaPathOld, STRING_MAX, Gw::GetBundleMgr()->GetMediaPath());

	Gw::GetBundleMgr()->SetMediaPath(pszFilePath);

	IGwBundle*	pBundle = Gw::GetBundleMgr()->Load_InPak(pszFileName);
	if(pBundle != NULL)
	{
		pAnimSequence = Gw::GetAnimMgr()->CreateAnimSequence(pBundle);
		pBundle->Release();
	}

	Gw::GetBundleMgr()->SetMediaPath(strMediaPathOld);

	return pAnimSequence;
}