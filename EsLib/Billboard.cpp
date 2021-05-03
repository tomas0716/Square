#include "stdafx.h"
#include "Billboard.h"

I_IMPLEMENT_DYNCREATE(IBillboard, ISingletonObject::GetTypeModule())

IBillboard::IBillboard()
{
	m_pBillboard = Gw::GetBillboardMgr()->CreateBillboard();
}

IBillboard::IBillboard(const char * pName)
: IGameObject(pName), m_fTime(0)
{
	m_pBillboard = Gw::GetBillboardMgr()->CreateBillboard();
}

IBillboard::IBillboard(const char * pName, IGwBillboard * pBillboard)
: IGameObject(pName)
{
	m_pBillboard = pBillboard->Clone();
}

IBillboard::~IBillboard()
{
	m_pBillboard->Release();
}

void		IBillboard::Update(float fDeltaTime)
{
	IGameObject::Update(fDeltaTime);
}

void		IBillboard::Render(float fDeltaTime)
{
	if( IsVisible() == true && IsVisible_ClientControl() == true )
	{
		m_pBillboard->SetColor(m_Color);
		m_pBillboard->SetPosition(GetWorldPos());
		m_pBillboard->SetScaleFactor(GwVector2(GetWorldScale().x, GetWorldScale().y));
		m_pBillboard->SetRotationAxis(GetRotationAxis());
		m_pBillboard->SetRotationAngle(GetWorldRot().z);
		
		m_pBillboard->Render(fDeltaTime);	
	}

	IGameObject::Render(fDeltaTime);
}

void		IBillboard::Render(float fDeltaTime, IGwViewFrustumOct * pFrustum)
{
	if( IsVisible() == true && IsVisible_ClientControl() == true )
	{
		m_pBillboard->SetColor(m_Color);
		m_pBillboard->SetPosition(GetWorldPos());
		m_pBillboard->SetScaleFactor(GwVector2(GetWorldScale().x, GetWorldScale().y));
		m_pBillboard->SetRotationAxis(GetRotationAxis());
		m_pBillboard->SetRotationAngle(GetWorldRot().z);
		
		m_pBillboard->Render(fDeltaTime);	
	}

	IGameObject::Render(fDeltaTime, pFrustum);
}

void		IBillboard::SetBillboardTex(IGwBillboardTex * pBillboardTex)
{
	m_pBillboard->SetBillboardTex(pBillboardTex);
}

void		IBillboard::SetBillboard(IGwBillboard * pBillboard)
{
	m_pBillboard->Release();
	m_pBillboard = pBillboard->Clone();
}

void		IBillboard::SetRotation(GwVector vRot)
{
	// Billboard 는 Z 축 회전값 만 사용 한다.

	IObject::SetRotation(GwVector(0,0,vRot.z));
}

void		IBillboard::Reset()
{
}

BYTE	*	IBillboard::LoadFile(BYTE * pData)
{
	pData = IGameObject::LoadFile(pData);

	return pData;
}

void		IBillboard::SaveFile(IFile & kFile)
{
	IGameObject::SaveFile(kFile);
}