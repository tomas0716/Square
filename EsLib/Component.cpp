#include "stdafx.h"
#include "Component.h"

I_IMPLEMENT_DYNCREATE(IComponent, ISingletonObject::GetTypeModule())

IComponent::IComponent()
: m_IsEnable(true), m_pParentGameObject(NULL)
{

}

IComponent::IComponent(IGameObject * pParent) 
: m_IsEnable(true), m_pParentGameObject(pParent)
{
}

IComponent::~IComponent()
{
}

void			IComponent::SetParent(IGameObject * pParent)
{
	m_pParentGameObject = pParent;
}

IGameObject	*	IComponent::GetParent()
{
	return m_pParentGameObject;
}

void			IComponent::SetEnable(bool IsEnable)
{
	m_IsEnable = IsEnable;
}

bool			IComponent::IsEnable()
{
	return m_IsEnable;
}

BYTE		*	IComponent::LoadFile(BYTE * pData)
{
	memcpy(&m_IsEnable, pData, sizeof(bool));		pData += sizeof(bool);

	return pData;
}

void			IComponent::SaveFile(IFile & kFile)
{
	kFile.WriteFile(&m_IsEnable, sizeof(bool));
}