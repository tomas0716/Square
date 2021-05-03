#include "stdafx.h"
#include "GameObject.h"

I_IMPLEMENT_DYNCREATE(IGameObject, ISingletonObject::GetTypeModule())

IGameObject::IGameObject()
: m_Color(GwColor(1,1,1,1)), m_nTag(2000)
{
}

IGameObject::IGameObject(const char * pName) 
: IObject(pName), m_Color(GwColor(1,1,1,1)), m_nTag(2000)
{

}

IGameObject::~IGameObject()
{
	RemoveComponentAll();
}

void			IGameObject::Update(float fDeltaTime)
{
	IObject::Update(fDeltaTime);

	vector<IComponent*>::iterator itr = m_ComponentList.begin();
	for( ; itr != m_ComponentList.end(); ++itr )
	{
		IComponent * pComponent = (*itr);
		if( pComponent != NULL )
			pComponent->Update(fDeltaTime);
	}

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		IGameObject * pGameObject = dynamic_cast<IGameObject*>(GetChild_byIndex(i));

		if( pGameObject != NULL ) 
		{
			pGameObject->Update(fDeltaTime);
		}
	}
}

void			IGameObject::Render(float fDeltaTime)	
{
	IObject::Render(fDeltaTime);

	vector<IComponent*>::iterator itr = m_ComponentList.begin();
	for( ; itr != m_ComponentList.end(); ++itr )
	{
		IComponent * pComponent = (*itr);
		if( pComponent != NULL )
			pComponent->Render(fDeltaTime);
	}

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		IGameObject * pGameObject = dynamic_cast<IGameObject*>(GetChild_byIndex(i));

		if( pGameObject != NULL ) 
		{
			pGameObject->Render(fDeltaTime);
		}
	}
}

void			IGameObject::Render(float fDeltaTime, IGwViewFrustumOct * pFrustum)
{
	IObject::Render(fDeltaTime, pFrustum);

	vector<IComponent*>::iterator itr = m_ComponentList.begin();
	for( ; itr != m_ComponentList.end(); ++itr )
	{
		IComponent * pComponent = (*itr);
		if( pComponent != NULL )
			pComponent->Render(fDeltaTime, pFrustum);
	}

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		IGameObject * pGameObject = dynamic_cast<IGameObject*>(GetChild_byIndex(i));

		if( pGameObject != NULL ) 
		{
			pGameObject->Render(fDeltaTime, pFrustum);
		}
	}
}

bool			IGameObject::AddComponent(IComponent * pComponent)
{
	vector<IComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr !=  m_ComponentList.end(); ++itr )
	{
		if( (*itr) == pComponent )
			return false;
	}

	m_ComponentList.push_back(pComponent);	pComponent->AddRef();

	return true;
}

bool			IGameObject::RemoveComponent(IComponent * pComponent)
{
	vector<IComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr !=  m_ComponentList.end(); ++itr )
	{
		if( (*itr) == pComponent )
		{
			(*itr)->Release();
			m_ComponentList.erase(itr);
			return true;
		}
	}

	return false;
}

void			IGameObject::RemoveComponentAll()
{
	vector<IComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr !=  m_ComponentList.end(); ++itr )
	{
		(*itr)->Release();
	}

	m_ComponentList.clear();
}

int				IGameObject::GetNumComponent()
{
	return (int)m_ComponentList.size();
}

IComponent	*	IGameObject::GetComponent_byIndex(int nIndex)
{
	if( nIndex < 0 || nIndex >= (int)m_ComponentList.size() )
		return NULL;

	return m_ComponentList[nIndex];
}

IComponent	*	IGameObject::GetComponent_byClassName(string strClassName)
{
	vector<IComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr !=  m_ComponentList.end(); ++itr )
	{
		const char * pszClassName = IHashObject::GetInstance()->GetHashString((*itr)->GetClassHashCode());

		if( strClassName == pszClassName )
		{
			return (*itr);
		}
	}

	return NULL;
}

void			IGameObject::SetColor(GwColor color)		
{
	m_Color = color;
}

void			IGameObject::Reset()
{
}

void			IGameObject::SetTag(int nTag)
{
	m_nTag = nTag;
}

int				IGameObject::GetTag()
{
	return m_nTag;
}

void			IGameObject::CalcAABB()
{
	vector<IComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr !=  m_ComponentList.end(); ++itr )
	{
		(*itr)->CalcAABB();
	}

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		IGameObject * pGameObject = dynamic_cast<IGameObject*>(GetChild_byIndex(i));

		if( pGameObject != NULL ) 
		{
			pGameObject->CalcAABB();
		}
	}
}

BYTE	*		IGameObject::LoadFile(BYTE * pData)
{
	pData = IObject::LoadFile(pData);

	memcpy(&m_nTag, pData, sizeof(int)); pData += sizeof(int);

	RemoveComponentAll();

	char szClassName[64] = { 0, };
	IType * pType = NULL;

	int nNumComponent = 0;
	IComponent * pComponent = NULL;
	memcpy(&nNumComponent, pData, sizeof(int)); pData += sizeof(int);

	for( int i = 0; i < nNumComponent; ++i )
	{
		memcpy(szClassName, pData, sizeof(char) * 64); pData += sizeof(char) * 64;
		pType = ITypeManager::GetInstance()->FindType(szClassName);
		pComponent = dynamic_cast<IComponent*>(pType->CreateInstance());
		pData = pComponent->LoadFile(pData);
		pComponent->SetParent(this);

		m_ComponentList.push_back(pComponent);
	}

	return pData;
}

void					IGameObject::SaveFile(IFile & kFile)
{	
	IObject::SaveFile(kFile);

	kFile.WriteFile(&m_nTag, sizeof(int));

	int nNumComponent = (int)m_ComponentList.size();
	kFile.WriteFile(&nNumComponent, sizeof(int));

	vector<IComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr != m_ComponentList.end(); ++itr )
	{
		const char * pszClassName = IHashObject::GetInstance()->GetHashString((*itr)->GetClassHashCode());
		kFile.WriteFile((void*)pszClassName, sizeof(char) * 64);
		(*itr)->SaveFile(kFile);
	}
}	