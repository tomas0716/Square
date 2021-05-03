#include "stdafx.h"
#include "Component_Trans.h"

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans

I_IMPLEMENT_DYNCREATE(IComponent_Trans, ISingletonObject::GetTypeModule())

IComponent_Trans::IComponent_Trans()
: IComponent(NULL)
{
}

IComponent_Trans::IComponent_Trans(IGameObject * pParent)
: IComponent(pParent)
{
}

IComponent_Trans::~IComponent_Trans()
{
	vector<ITransInfo*>::iterator itr = m_EventInfoList.begin();
	for( ; itr != m_EventInfoList.end(); ++itr )
	{
		(*itr)->Release();
	}
	m_EventInfoList.clear();
}

void			IComponent_Trans::AddEvent(ITransInfo * pEvent)
{
	m_EventInfoList.push_back(pEvent);
	pEvent->AddRef();
}

void			IComponent_Trans::RemoveEvent(ITransInfo * pEvent)
{
	vector<ITransInfo*>::iterator itr = m_EventInfoList.begin();
	for( ; itr != m_EventInfoList.end(); ++itr )
	{
		if( (*itr) == pEvent )
		{
			(*itr)->Release();
			m_EventInfoList.erase(itr);
			return;
		}
	}
}

bool			IComponent_Trans::RemoveEvent_byIndex(int nIndex)
{
	if( nIndex < 0 && nIndex > (int)(m_EventInfoList.size() - 1) )
		return false;

	RemoveEvent(m_EventInfoList[nIndex]);

	return true;
}

int				IComponent_Trans::GetNumEvent()
{
	return (int)m_EventInfoList.size();
}

ITransInfo	*	IComponent_Trans::GetEvent_byIndex(int nIndex)
{
	if( nIndex < 0 || nIndex >= m_EventInfoList.size() )
		return NULL;

	return m_EventInfoList[nIndex];
}

bool			Compare(ITransInfo * pTransInfo_A, ITransInfo * pTransInfo_B)
{
	return pTransInfo_A->m_fTime < pTransInfo_B->m_fTime;
}

void			IComponent_Trans::Sort()
{
	sort(m_EventInfoList.begin(), m_EventInfoList.end(), Compare);
}

BYTE		*	IComponent_Trans::LoadFile(BYTE * pData)
{
	pData = IComponent::LoadFile(pData);

	char szClassName[64] = { 0, };
	IType * pType = NULL;

	int nNumEvent = 0;
	memcpy(&nNumEvent, pData, sizeof(int));	pData += sizeof(int);

	for( int i = 0; i < nNumEvent; ++i )
	{
		memcpy(szClassName, pData, sizeof(char) * 64); pData += sizeof(char) * 64;
		pType = ITypeManager::GetInstance()->FindType(szClassName);
		ITransInfo * pEvent = dynamic_cast<ITransInfo*>(pType->CreateInstance());
		pData = pEvent->LoadFile(pData);
		m_EventInfoList.push_back(pEvent);
	}

	Sort();

	return pData;
}

void			IComponent_Trans::SaveFile(IFile & kFile)
{
	IComponent::SaveFile(kFile);

	int nNumEvent = (int)m_EventInfoList.size();
	kFile.WriteFile(&nNumEvent, sizeof(int));

	vector<ITransInfo*>::iterator itr = m_EventInfoList.begin();
	for( ; itr != m_EventInfoList.end(); ++itr )
	{
		const char * pszClassName = IHashObject::GetInstance()->GetHashString((*itr)->GetClassHashCode());
		kFile.WriteFile((void*)pszClassName, sizeof(char) * 64);
		(*itr)->SaveFile(kFile);
	}
}

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_Scale

I_IMPLEMENT_DYNCREATE(IComponent_Trans_Scale, ISingletonObject::GetTypeModule())

IComponent_Trans_Scale::IComponent_Trans_Scale()
{
}

IComponent_Trans_Scale::IComponent_Trans_Scale(IGameObject * pParent)
: IComponent_Trans(pParent)
{
}

void			IComponent_Trans_Scale::AddEvent(ITransInfo * pEvent)
{
	ITransScaleInfo * pTransInfo = dynamic_cast<ITransScaleInfo*>(pEvent);
	if( pTransInfo != NULL ) IComponent_Trans::AddEvent(pEvent);
}


//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_Color

I_IMPLEMENT_DYNCREATE(IComponent_Trans_Color, ISingletonObject::GetTypeModule())

IComponent_Trans_Color::IComponent_Trans_Color()
{
}

IComponent_Trans_Color::IComponent_Trans_Color(IGameObject * pParent)
: IComponent_Trans(pParent)
{
}

void			IComponent_Trans_Color::AddEvent(ITransInfo * pEvent)
{
	ITransColorInfo * pTransInfo = dynamic_cast<ITransColorInfo*>(pEvent);
	if( pTransInfo != NULL ) IComponent_Trans::AddEvent(pEvent);
}

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_Alpha

I_IMPLEMENT_DYNCREATE(IComponent_Trans_Alpha, ISingletonObject::GetTypeModule())

IComponent_Trans_Alpha::IComponent_Trans_Alpha()
{
}

IComponent_Trans_Alpha::IComponent_Trans_Alpha(IGameObject * pParent)
: IComponent_Trans(pParent)
{
}

void			IComponent_Trans_Alpha::AddEvent(ITransInfo * pEvent)
{
	ITransAlphaInfo * pTransInfo = dynamic_cast<ITransAlphaInfo*>(pEvent);
	if( pTransInfo != NULL ) IComponent_Trans::AddEvent(pEvent);
}

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_Rotation

I_IMPLEMENT_DYNCREATE(IComponent_Trans_Rotation, ISingletonObject::GetTypeModule())

IComponent_Trans_Rotation::IComponent_Trans_Rotation()
{
}

IComponent_Trans_Rotation::IComponent_Trans_Rotation(IGameObject * pParent)
: IComponent_Trans(pParent)
{
}

void			IComponent_Trans_Rotation::AddEvent(ITransInfo * pEvent)
{
	ITransRotationInfo * pTransInfo = dynamic_cast<ITransRotationInfo*>(pEvent);
	if( pTransInfo != NULL ) IComponent_Trans::AddEvent(pEvent);
}


//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_AnimSequence

I_IMPLEMENT_DYNCREATE(IComponent_Trans_AnimSequence, ISingletonObject::GetTypeModule())

IComponent_Trans_AnimSequence::IComponent_Trans_AnimSequence()
{
}

IComponent_Trans_AnimSequence::IComponent_Trans_AnimSequence(IGameObject * pParent)
: IComponent_Trans(pParent)
{
}

void			IComponent_Trans_AnimSequence::AddEvent(ITransInfo * pEvent)
{
	ITransSequenceAnimInfo * pTransInfo = dynamic_cast<ITransSequenceAnimInfo*>(pEvent);
	if( pTransInfo != NULL ) IComponent_Trans::AddEvent(pEvent);
}
