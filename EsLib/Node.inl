
template<class T>
void		INode::FindChild_byName(const char * pName, vector<T*> & pChildList)
{
	vector<INode*>::iterator itr = m_ChildList.begin();

	for( ; itr != m_ChildList.end(); ++itr )
	{
		if( strcmp((*itr)->GetName().c_str(), pName) == 0 )
		{
			T * p = dynamic_cast<T*>(*itr);

			if( p != NULL )
				pChildList.push_back(p);
		}
	}
}

template<class T>
void		INode::FindChild_byID(UINT ID, vector<T*> & pChildList)
{
	vector<INode*>::iterator itr = m_ChildList.begin();

	for( ; itr != m_ChildList.end(); ++itr )
	{
		if( (*itr)->GetID() == ID )
		{
			T * p = dynamic_cast<T*>(*itr);

			if( p != NULL )
				pChildList.push_back(p);
		}
	}
}

template<class T>
void		INode::FindChild_AllTreeNode_byName(const char * pName, vector<T*> & pChildList)
{
	FindChild_byName(pName, pChildList);

	vector<INode*>::iterator itr = m_ChildList.begin();

	for( ; itr != m_ChildList.end(); ++itr )
	{
		INode * pChild = (*itr);
		pChild->FindChild_AllTreeNode_byName(pName, pChildList);
	}
}

template<class T>
void		INode::FindChild_AllTreeNode_byID(UINT ID, vector<T*> & pChildList)
{
	FindChild_byID(ID, pChildList);

	vector<INode*>::iterator itr = m_ChildList.begin();

	for( ; itr != m_ChildList.end(); ++itr )
	{
		INode * pChild = (*itr);
		pChild->FindChild_AllTreeNode_byID(ID, pChildList);
	}
}