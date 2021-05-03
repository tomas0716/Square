#include "stdafx.h"
#include "Node.h"

I_IMPLEMENT_DYNCREATE(INode, ISingletonObject::GetTypeModule())

UINT	INode::ms_nLastID = 0;

INode::INode() 
: m_strName(""), m_nID(0), m_pParent(NULL)
{
}

INode::INode(const char * pName) 
: m_strName(pName), m_nID(0), m_pParent(NULL)
{
}

INode::INode(const char * pName, UINT nID)
: m_strName(pName), m_nID(nID), m_pParent(NULL)
{
}

INode::~INode()
{
	GwReleaseFunctor functor_Release;
	for_each(m_ChildList.begin(),m_ChildList.end(),functor_Release);
	m_ChildList.clear();
}

void		INode::Update(float fDeltaTime)
{
}

void		INode::Render(float fDeltaTime)
{
}

void		INode::Render(float fDeltaTime, IGwViewFrustumOct * pFrustum)
{
}

void		INode::SetName(const char * pName)
{
	m_strName = pName;
}

string		INode::GetName()
{
	return m_strName;
}

void		INode::SetID(UINT ID)
{
	m_nID = ID;
}

UINT		INode::GetID()
{
	return m_nID;
}

void		INode::SetParent(INode * pParent)
{
	m_pParent = pParent;	
}

INode	*	INode::GetParent()
{
	return m_pParent;
}

void		INode::AddChild(INode * pNode)
{
	INode * pParent = pNode->GetParent();
	if( pParent != NULL ) 
	{
		pNode->AddRef();
		pParent->RemoveChild(pNode);
		m_ChildList.push_back(pNode);
		pNode->SetParent(this);
	}
	else
	{
		m_ChildList.push_back(pNode);	pNode->AddRef();
		pNode->SetParent(this);	
	}
}

void		INode::RemoveChild(INode * pNode)
{
	vector<INode*>::iterator itr = m_ChildList.begin();

	for( ; itr != m_ChildList.end(); ++itr )
	{
		if( *itr == pNode )
		{
			pNode->SetParent(NULL);
			pNode->Release();
			m_ChildList.erase(itr);

			return;
		}
	}
}

int			INode::GetNumChild()
{
	return (int)m_ChildList.size();
}

INode	*	INode::GetChild_byIndex(int Index)
{
	if( Index < 0 && Index >= (int)m_ChildList.size() )
		return NULL;

	return m_ChildList[Index];
}

bool		INode::IsChildAllNode(INode * pNode)
{
	vector<INode*>::iterator itr = m_ChildList.begin();

	for( ; itr != m_ChildList.end(); ++itr )
	{
		if( (*itr) == pNode )
		{
			return true;
		}
		else 
		{
			bool IsFind = (*itr)->IsChildAllNode(pNode);

			if( IsFind == true )
				return true;
		}
	}

	return false;
}

BYTE	*	INode::LoadFile(BYTE * pData)
{
	char szName[32] = { 0, };
	memcpy(szName, pData, sizeof(char) * 32);	pData += sizeof(char) * 32;
	m_strName = szName;
	memcpy(&m_nID, pData, sizeof(UINT));		pData += sizeof(UINT);
	
	if( m_nID == 0 )	m_nID = ++ms_nLastID;
	else				ms_nLastID = max(ms_nLastID, m_nID) + 1;

	int nNumChild = 0;
	memcpy(&nNumChild, pData, sizeof(int));		pData += sizeof(int);

	for( int i = 0; i < nNumChild; ++i )
	{
		char szClassName[64] = { 0, };
		memcpy(szClassName, pData, sizeof(char) * 64); pData += sizeof(char) * 64;

		IType * pType = ITypeManager::GetInstance()->FindType(szClassName);
		INode * pNode = dynamic_cast<INode*>(pType->CreateInstance());

		pData = pNode->LoadFile(pData);

		AddChild(pNode);	pNode->Release();
	}

	return pData;
}

void		INode::SaveFile(IFile & kFile)
{
	kFile.WriteFile((void*)m_strName.c_str(), sizeof(char) * 32);
	kFile.WriteFile(&m_nID, sizeof(UINT));

	int nNumChild = GetNumChild();
	kFile.WriteFile(&nNumChild, sizeof(int));

	for( int i = 0; i < nNumChild; ++i )
	{
		INode * pNode = GetChild_byIndex(i);

		if( pNode )
		{
			const char * pszClassName = IHashObject::GetInstance()->GetHashString(pNode->GetClassHashCode());
			kFile.WriteFile((void*)pszClassName, sizeof(char) * 64);
			
			pNode->SaveFile(kFile);
		}
	}
}