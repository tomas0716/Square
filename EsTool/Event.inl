template<typename T>
void				TEventDispatch<T>::AddEventOUT(const char* strName, EventType* pEventIN)
{
	pEventIN->AddRef();
	m_pEventOUT_List.push_back(pair<string, EventType*>(strName, pEventIN));
}

template<typename T>
void				TEventDispatch<T>::ClearEventOUT(const char* strName)
{
	list<pair<string, EventType*>>::iterator iter;

	for (iter = m_pEventOUT_List.begin(); iter != m_pEventOUT_List.end();)
	{
		if(iter->first == strName)
		{
			iter->second->Release();
			iter = m_pEventOUT_List.erase(iter);
		}
		else 
		{
			++iter;
		}
	}
}

template<typename T>
void				TEventDispatch<T>::ClearAllEventOUT()
{
	list<pair<string, EventType*>>::iterator iter;

	for (iter = m_pEventOUT_List.begin(); iter != m_pEventOUT_List.end(); ++iter)
	{
		iter->second->Release();
	}

	m_pEventOUT_List.clear();
}

template<typename T>
void	TEventDispatch<T>::OnEvent(EventArgType_ConstRef Arg)
{
	if(IsDisposed())	return;

	this->AddRef();

	m_pEventList.clear();
	m_pEventList.reserve(m_pEventOUT_List.size());
	for each(pair<string, EventType*> item in m_pEventOUT_List)
	{
		m_pEventList.push_back(item.second);
		item.second->AddRef();
	}

	for each(EventType* pEvent in m_pEventList)
	{
		pEvent->OnEvent(Arg);
		pEvent->Release();
	}
	m_pEventList.clear();

	this->Release();
}

template<typename T>
bool	TEventDispatch<T>::IsDisposed()
{
	return m_bDisposed;
}

template<typename T>
void	TEventDispatch<T>::Dispose()
{
	if(IsDisposed())	return;		

	for each(pair<string, EventType*> item in m_pEventOUT_List)
	{
		item.second->Release();
	}
	m_pEventOUT_List.clear();

	m_bDisposed = true;
}

template<typename T>
TEventDispatch<T>::TEventDispatch()
{
	m_bDisposed = false;
}
template<typename T>
TEventDispatch<T>::~TEventDispatch()
{
	for each(pair<string, EventType*> item in m_pEventOUT_List)
	{
		item.second->Release();
	}
	m_pEventOUT_List.clear();
}