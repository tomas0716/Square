#include "StdAfx.h"
#include "UIComponent.h"

CUIComponent::CUIComponent()
: m_pGameObject(NULL), m_fHeight(0), m_pComponent(NULL)
{
	m_pBaseControl = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
}

CUIComponent::~CUIComponent(void)
{
	if( m_pGameObject != NULL ) 
		m_pGameObject->Release();

	if( m_pComponent != NULL )
		m_pComponent->Release();

	m_pBaseControl->Release();
}

void		CUIComponent::SetGameObject(IGameObject * pGameObject)
{
	m_pGameObject = pGameObject;
	m_pGameObject->AddRef();

	Initialize();
}

void		CUIComponent::SetComponent(IComponent * pComponent)
{
	if( m_pComponent != NULL )
		m_pComponent->Release();

	m_pComponent = pComponent;

	if( m_pComponent != NULL ) 
		m_pComponent->AddRef();
}