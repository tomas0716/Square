#pragma once

class CResourceManager : public TGwGetInstance<CResourceManager>
{
public:
	CResourceManager(void);
	~CResourceManager(void);

	void					Begin();		// Texture 로드 시에만 필요
	void					End();			// Texture 로드 시에만 필요

	template<typename T>
	T					*	CreateResource(const char * pszFilePath, const char * pszFileName);

	template<typename T>
	T					*	CreateResource(const char * pszFilePath, const char * pszTexturePath, const char * pszFileName);

private:

	IGwBillboardTex		*	CreateBillboardTex(const char * pszFilePath, const char * pszFileName);
	IGwStaticObject		*	CreateStaticObject(const char * pszFilePath, const char * pszTexturePath, const char * pszFileName);
	IGwAnimSequence		*	CreateAnimSequence(const char * pszFilePath, const char * pszFileName);

	map<string,IGwRefObject*>	m_ResourceTable;
};

#include "ResourceManager.inl"
