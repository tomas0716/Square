#include "StdAfx.h"
#include "Table_Menu.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTable_Menu_Item

CTable_Menu_Item::CTable_Menu_Item(const char * pszName, const char * pszFunction)
: INode(pszName), m_strFunction(pszFunction), m_strSyskey(""), m_Char(' '), m_strShortKeyDesc("")
{
}

CTable_Menu_Item::~CTable_Menu_Item()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTable_Menu

CTable_Menu::CTable_Menu(void)
{
	m_pRoot = new INode("");

	Load();
}

CTable_Menu::~CTable_Menu(void)
{
	m_pRoot->Release();
}

void		CTable_Menu::Load()
{
	IGwSLKParser*	pParser;
	IGwParamList*	pParamList;

	Gw::GetParseMgr()->SetMediaPath("EsTool_Resources\\PARAM\\");
	IGwPakData*	pPakData = Gw::GetRscPakMgr()->Load(Gw::GetParseMgr()->GetMediaPath(), "Menu.slk");
	pParser = Gw::GetParseMgr()->CreateSLKParser(pPakData->GetData_Void(), pPakData->GetSize());
	pPakData->Release();

	struct SInfo
	{
		int			nIndex;
		int			nParent;
		char	*	pszName;
		char	*	pszFunction;
		char	*	pszSysKey;
		char		sChar;
		char	*	pszShortKeyDesc;
	};

	pParamList = Gw::GetParseMgr()->CreateParamList(7,
		GWDATATYPE_INT,		GWVARIABLETYPE_DEFAULT,	1,
		GWDATATYPE_INT,		GWVARIABLETYPE_DEFAULT,	1,
		GWDATATYPE_STRING,	GWVARIABLETYPE_DEFAULT, GwNumElement_Ptr,
		GWDATATYPE_STRING,	GWVARIABLETYPE_DEFAULT, GwNumElement_Ptr,
		GWDATATYPE_STRING,	GWVARIABLETYPE_DEFAULT, GwNumElement_Ptr,
		GWDATATYPE_STRING,	GWVARIABLETYPE_DEFAULT, 1,
		GWDATATYPE_STRING,	GWVARIABLETYPE_DEFAULT, GwNumElement_Ptr
		);

	TGwSLKData<SInfo*>*	pImageInfoTable = Gw::GetParseMgr()->CreateSLKData<SInfo*>(pParser, pParamList, 1);

	int	numData = pImageInfoTable->GetNumData();

	for(int i = 0; i < numData; ++i)
	{
		SInfo * pInfo = pImageInfoTable->GetData(i);
		CTable_Menu_Item * pItem = new CTable_Menu_Item(pInfo->pszName, pInfo->pszFunction);
		pItem->SetID(pInfo->nIndex);
		if(pInfo->pszSysKey != NULL)
			pItem->SetSyskey(pInfo->pszSysKey);
		pItem->SetChar(pInfo->sChar);
		if( pInfo->pszShortKeyDesc != NULL )
			pItem->SetShortKeyDesc(pInfo->pszShortKeyDesc);

		INode * pParent = pInfo->nParent == 1 ? NULL : FindParent(m_pRoot, pInfo->nParent);
		if( pParent != NULL ) 
		{
			pParent->AddChild(pItem);	pItem->Release();
		}
		else 
		{
			m_pRoot->AddChild(pItem);	pItem->Release();
		}
	}

	pImageInfoTable->Release();
	pParamList->Release();
	pParser->Release();
}

INode	*	CTable_Menu::FindParent(INode * pNode, int nIndex)
{
	int nNumChild = pNode->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		INode * pChild = pNode->GetChild_byIndex(i);

		if( pChild->GetID() == nIndex )
		{
			return pChild;
		}
		else 
		{
			INode * pFind = FindParent(pChild, nIndex);

			if( pFind != NULL ) return pFind;
		}
	}

	return NULL;
}