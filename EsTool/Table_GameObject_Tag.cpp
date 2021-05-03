#include "StdAfx.h"
#include "Table_GameObject_Tag.h"

CTable_GameObject_Tag::CTable_GameObject_Tag(void)
{
	m_pRoot = new INode("");

	Load();
}

CTable_GameObject_Tag::~CTable_GameObject_Tag(void)
{
	m_pRoot->Release();
}

void		CTable_GameObject_Tag::Load()
{
	IGwSLKParser*	pParser;
	IGwParamList*	pParamList;

	Gw::GetParseMgr()->SetMediaPath("EsTool_Resources\\PARAM\\");
	IGwPakData*	pPakData = Gw::GetRscPakMgr()->Load(Gw::GetParseMgr()->GetMediaPath(), "GameObject_Tag.slk");
	pParser = Gw::GetParseMgr()->CreateSLKParser(pPakData->GetData_Void(), pPakData->GetSize());
	pPakData->Release();

	struct SInfo
	{
		int			nIndex;
		int			nParent;
		char	*	pszName;
	};

	pParamList = Gw::GetParseMgr()->CreateParamList(3,
		GWDATATYPE_INT,		GWVARIABLETYPE_DEFAULT,	1,
		GWDATATYPE_INT,		GWVARIABLETYPE_DEFAULT,	1,
		GWDATATYPE_STRING,	GWVARIABLETYPE_DEFAULT, GwNumElement_Ptr
		);

	TGwSLKData<SInfo*>*	pImageInfoTable = Gw::GetParseMgr()->CreateSLKData<SInfo*>(pParser, pParamList, 1);

	int	numData = pImageInfoTable->GetNumData();

	for(int i = 0; i < numData; ++i)
	{
		SInfo * pInfo = pImageInfoTable->GetData(i);
		INode * pNode = new INode(pInfo->pszName, pInfo->nIndex);

		INode * pParent = pInfo->nParent == 1 ? NULL : FindParent(m_pRoot, pInfo->nParent);
		if( pParent != NULL ) 
		{
			pParent->AddChild(pNode);	pNode->Release();
		}
		else 
		{
			m_pRoot->AddChild(pNode);	pNode->Release();
		}
	}

	pImageInfoTable->Release();
	pParamList->Release();
	pParser->Release();
}

INode	*	CTable_GameObject_Tag::FindParent(INode * pNode, int nID)
{
	int nNumChild = pNode->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		INode * pChild = pNode->GetChild_byIndex(i);

		if( pChild->GetID() == nID )
		{
			return pChild;
		}
		else 
		{
			INode * pFind = FindParent(pChild, nID);

			if( pFind != NULL ) return pFind;
		}
	}

	return NULL;
}

INode	*	CTable_GameObject_Tag::FindNode(int nID)
{
	int nNumChild = m_pRoot->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		INode * pChild = m_pRoot->GetChild_byIndex(i);

		if( pChild->GetID() == nID )
		{
			return pChild;
		}
		else 
		{
			INode * pFind = FindNode(pChild, nID);

			if( pFind != NULL ) return pFind;
		}
	}

	return NULL;
}

INode	*	CTable_GameObject_Tag::FindNode(INode * pNode, int nID)
{
	int nNumChild = pNode->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		INode * pChild = pNode->GetChild_byIndex(i);

		if( pChild->GetID() == nID )
		{
			return pChild;
		}
		else 
		{
			INode * pFind = FindNode(pChild, nID);

			if( pFind != NULL ) return pFind;
		}
	}

	return NULL;
}