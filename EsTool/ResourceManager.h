#pragma once

class CResourceManager : public TGwGetInstance<CResourceManager>
{
public:
	CResourceManager(void);
	~CResourceManager(void);

	void					Begin();		// Texture �ε� �ÿ��� �ʿ�
	void					End();			// Texture �ε� �ÿ��� �ʿ�

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
