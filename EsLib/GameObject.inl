
template<class T>
void			IGameObject::GetComponent_byClassName(string strClassName, vector<T*> & kComponentList)
{
	vector<IComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr !=  m_ComponentList.end(); ++itr )
	{
		const char * pszClassName = IHashObject::GetInstance()->GetHashString((*itr)->GetClassHashCode());

		if( strClassName == pszClassName )
		{
			kComponentList.push_back((T*)(*itr));
		}
	}
}


template<class T>
inline T				*	IGameObject::GetComponent()
{
	vector<IComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr != m_ComponentList.end(); ++itr )
	{
		T * pComponent = dynamic_cast<T*>((*itr));
		if( pComponent != NULL )
		{
			return pComponent;
		}
	}

	return NULL;
}