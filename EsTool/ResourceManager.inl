
template<typename T>
T					*	CResourceManager::CreateResource(const char * pszFilePath, const char * pszFileName)
{
	string strFullName = pszFilePath;
	strFullName += "\\";
	strFullName += pszFileName;

	map<string,IGwRefObject*>::iterator itr = m_ResourceTable.find(strFullName);

	if( itr != m_ResourceTable.end() )
	{
		return (T*)itr->second;
	}

	IGwBillboardTex	* pTex = CreateBillboardTex(pszFilePath, pszFileName);

	if( pTex != NULL )
	{
		m_ResourceTable.insert(std::map<string, IGwRefObject*>::value_type(strFullName, pTex));
		return (T*)pTex;
	}

	IGwAnimSequence	* pAnimSequence = CreateAnimSequence(pszFilePath, pszFileName);

	if( pAnimSequence != NULL ) 
	{
		m_ResourceTable.insert(std::map<string, IGwRefObject*>::value_type(strFullName, pAnimSequence));
		return (T*)pAnimSequence;
	}

	return NULL;
}

template<typename T>
T					*	CResourceManager::CreateResource(const char * pszFilePath, const char * pszTexturePath, const char * pszFileName)
{
	string strFullName = pszFilePath;
	strFullName += "\\";
	strFullName += pszFileName;

	map<string,IGwRefObject*>::iterator itr = m_ResourceTable.find(strFullName);

	if( itr != m_ResourceTable.end() )
	{
		return (T*)itr->second;
	}

	IGwStaticObject	* pStaticObject = CreateStaticObject(pszFilePath, pszFileName, pszTexturePath);

	if( pStaticObject != NULL ) 
	{
		m_ResourceTable.insert(std::map<string, IGwRefObject*>::value_type(strFullName, pStaticObject));
		return (T*)pStaticObject;
	}

	return NULL;
}