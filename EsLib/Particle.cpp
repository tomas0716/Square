#include "stdafx.h"
#include "Particle.h"

class	IParticle::IEventTransformCallback_Rotation : public IGwEventTransformer_Callback
{
public:

	IEventTransformCallback_Rotation(IParticle * pParticle) : m_pParticle(pParticle)	
	{

	}

	virtual void OnOneEventDone() 
	{
		m_pParticle->OnOneEventDone_Rotation();
	}
	
private:
	IParticle	*	m_pParticle;
};

IParticle::IParticle(IParticleSystem * pParticleSystem, IParticleObject * pParticleObject, IGameObject * pGameObject, GwVector vParentPos, GwVector vParentRot, GwVector vParentScale, 
					 GwVector vStartPos, GwVector vStartDirection, float fEmissionLifeTime, float fStartSpeed,
					 GwVector vStartScale, GwVector vStartRotation, GwColor clrStartColor, float fStartAlpha, float fGravity, bool IsLocal)
: m_ePlayType(ePlay_Play), m_fTime(0), m_vCameraPos(GwVector(0,0,1)), m_nCurrTransRotationIndex(0), m_fRotationTime(0), m_pTransRotationInfo(NULL)
{
	m_pParticleSystem = pParticleSystem;

	m_pParticleObject = pParticleObject;

	m_pGameObject = pGameObject;
	m_pGameObject->AddRef();

	m_pScale = Gw::GetEventTransformMgr()->CreateTransformer_Pos(vStartScale);
	m_pColor = Gw::GetEventTransformMgr()->CreateTransformer_Color(clrStartColor);
	m_pAlpha = Gw::GetEventTransformMgr()->CreateTransformer_Scalar(fStartAlpha);
	m_pRotation = Gw::GetEventTransformMgr()->CreateTransformer_Scalar(0);

	Reset(vParentPos, vParentRot, vParentScale, vStartPos, vStartDirection, fEmissionLifeTime,
		  fStartSpeed, vStartScale, vStartRotation, clrStartColor, fStartAlpha, fGravity, IsLocal);
}

IParticle::~IParticle()
{
	m_pGameObject->Release();

	m_pScale->Release();
	m_pColor->Release();
	m_pAlpha->Release();
	m_pRotation->Release();

	if( m_pTransRotationInfo != NULL ) m_pTransRotationInfo->Release();
}

void				IParticle::Update(float fDeltaTime)
{
	if( m_ePlayType == ePlay_Play )
	{
		m_fTime += fDeltaTime;

		if( m_fTime <= m_fEmissionLifeTime )
		{
			//D3DXVec3Normalize(&m_vDirection, &m_vDirection);

			GwVector vNewPos;
			vNewPos.x = m_vDirection.x * fDeltaTime;
			vNewPos.y = m_vDirection.y * fDeltaTime - 0.5f * m_fGravity * fDeltaTime;
			vNewPos.z = m_vDirection.z * fDeltaTime;

			//D3DXVec3Normalize(&m_vDirection, &vNewPos);
			vNewPos = GwVector( vNewPos.x * m_pParticleObject->GetScale().x,
								vNewPos.y * m_pParticleObject->GetScale().y, 
								vNewPos.z * m_pParticleObject->GetScale().z);

			m_vPosition += vNewPos * m_fStartSpeed;
			m_vDirection = vNewPos;

			GwVector vCameraDir = m_vPosition + m_vParentPos - m_vCameraPos;
			m_pGameObject->SetRotationAxis(vCameraDir);
			m_pGameObject->SetPosition(m_vPosition + m_vParentPos);

			// Component
			m_pScale->Update(fDeltaTime);
			GwVector vScale = *m_pScale->GetCurrPos();
			m_pGameObject->SetScale(GwVector(m_vStartScale.x * m_vParentScale.x * vScale.x, m_vStartScale.y * m_vParentScale.y * vScale.y, m_vStartScale.z * m_vParentScale.z * vScale.z));

			m_pColor->Update(fDeltaTime);
			m_pAlpha->Update(fDeltaTime);
			GwColor clrColor = *m_pColor->GetCurrColor();
			clrColor.a = m_pAlpha->GetCurrScalar();
			m_pGameObject->SetColor(clrColor);

			m_pRotation->Update(fDeltaTime);

			GwMatA matResRot;
			GwVector vResRotation;
			
			GwMatA matStartRot = IHelper::GetMatrix_ToRotation(GwVector(D3DXToRadian(m_vStartRotation.x), D3DXToRadian(m_vStartRotation.y), D3DXToRadian(-m_vStartRotation.z)));
			GwMatA matParentRot = IHelper::GetMatrix_ToRotation(GwVector(D3DXToRadian(m_vParentRotation.x), D3DXToRadian(m_vParentRotation.y), D3DXToRadian(-m_vParentRotation.z)));
			
			if( m_pTransRotationInfo != NULL )
			{
				GwVector vComponentRotation = GwVector(0,0,0);

				m_fRotationTime += fDeltaTime;

				GwMatA matRotation;
				D3DXMatrixRotationAxis(&matRotation, &m_pTransRotationInfo->m_vRotationAxis, m_pTransRotationInfo->m_fSpeed * m_fRotationTime);
				
				matResRot = matRotation * matStartRot * matParentRot;
			}
			else 
			{
				matResRot = matStartRot * matParentRot;
			}

			vResRotation.x = D3DXToDegree(atan2( matResRot._23, matResRot._33 ));
			vResRotation.y = D3DXToDegree(asin(matResRot._13));
			vResRotation.z = D3DXToDegree(atan2( matResRot._12, matResRot._11 )); 

			m_pGameObject->SetRotation(vResRotation);

			m_pGameObject->Update(fDeltaTime);
		}
		else
		{
			m_ePlayType = ePlay_Complete;

			m_pParticleSystem->InValidParticle(this);
		}

	}
}

void				IParticle::Render(float fDeltaTime)
{
	if( m_ePlayType == ePlay_Play )
	{
		m_pGameObject->Render(fDeltaTime);
	}
}

void				IParticle::SetGameObject(IGameObject * pGameObject)
{
	m_pGameObject->Release();
	m_pGameObject = pGameObject;
	m_pGameObject->AddRef();
}

IGameObject		*	IParticle::GetGameObject()
{
	return m_pGameObject;
}

void				IParticle::SetParentPos(GwVector vParentPos)
{
	if( m_IsLocal == false ) m_vParentPos = vParentPos;
}

void				IParticle::SetCameraPos(GwVector vCameraPos)
{
	m_vCameraPos = vCameraPos;
}

void				IParticle::OnPlay()
{
	if( m_ePlayType == ePlay_Stop )
	{
		m_pScale->Replay();
		m_pColor->Replay();
		m_pAlpha->Replay();
		m_pRotation->Replay();
	}

	switch(m_ePlayType)
	{
	case ePlay_Stop:
	case ePlay_Pause:
		{
			m_ePlayType = ePlay_Play;
			m_fTime = 0;
		}
		break;
	}
}

void				IParticle::OnPause()
{
	if( m_ePlayType == ePlay_Play )
	{
		m_ePlayType = ePlay_Pause;
	}
}

void				IParticle::OnStop()
{
	if( m_ePlayType == ePlay_Play || m_ePlayType == ePlay_Pause)
	{
		m_pScale->Stop();
		m_pColor->Stop();
		m_pAlpha->Stop();
		m_pRotation->Stop();

		m_ePlayType = ePlay_Stop;
		m_fTime = 0;
	}
}

void				IParticle::Reset(GwVector vParentPos, GwVector vParentRot, GwVector vParentScale, 
									 GwVector vStartPos, GwVector vStartDirection, float fEmissionLifeTime, float fStartSpeed,
									 GwVector vStartScale, GwVector vStartRotation, GwColor clrStartColor, float fStartAlpha, float fGravity, bool IsLocal)
{
	m_vParentPos				= vParentPos;
	m_vParentRotation			= vParentRot;
	m_vParentScale				= vParentScale;

	m_vPosition					= vStartPos;
	GwMat matParent = IHelper::GetMatrix(GwVector(0,0,0), GwVector(D3DXToRadian(vParentRot.x), D3DXToRadian(vParentRot.y), D3DXToRadian(-vParentRot.z)), GwVector(1,1,1));
	D3DXVec3TransformCoord(&m_vDirection, &vStartDirection, &matParent);
	//D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	m_fEmissionLifeTime			= fEmissionLifeTime;
	m_fStartSpeed				= fStartSpeed;
	m_vStartScale				= vStartScale;
	m_vStartRotation			= vStartRotation;
	m_clrStartColor				= clrStartColor;
	m_fStartAlpha				= fStartAlpha;
	m_fGravity					= fGravity;
	m_IsLocal					= IsLocal;

	m_pGameObject->Reset();
	m_pGameObject->SetScale(GwVector(m_vStartScale.x * m_vParentScale.x, m_vStartScale.y * m_vParentScale.y, m_vStartScale.z * m_vParentScale.z));
	m_pGameObject->SetRotation(m_vStartRotation + m_vParentRotation);
	m_pGameObject->SetColor(m_clrStartColor);

	CreateComponent_Trans_Scale();
	CreateComponent_Trans_Color();
	CreateComponent_Trans_Alpha();
	CreateComponent_Trans_Rotation();

	m_ePlayType					= ePlay_Stop;
}

bool				CompareTrans(ITransInfo * pTransInfo_A, ITransInfo * pTransInfo_B)
{
	return pTransInfo_A->m_fTime < pTransInfo_B->m_fTime;
}

void				IParticle::CreateComponent_Trans_Scale()
{
	m_pScale->Release();
	m_pScale = Gw::GetEventTransformMgr()->CreateTransformer_Pos(m_vStartScale);

	if( ISingletonObject::IsTool() == true )
	{
		IComponent_Trans_Scale * pComponent_Trans_Scale = m_pParticleObject->GetComponent<IComponent_Trans_Scale>();

		if( pComponent_Trans_Scale != NULL && pComponent_Trans_Scale->IsEnable() == true )
		{
			vector<ITransInfo*>		EventInfoList;

			int nCount = pComponent_Trans_Scale->GetNumEvent();

			for( int i = 0; i < nCount; ++i )
			{
				ITransInfo * pTransInfo = pComponent_Trans_Scale->GetEvent_byIndex(i);

				if( pTransInfo != NULL )
				{
					EventInfoList.push_back(pTransInfo);
				}
			}

			sort(EventInfoList.begin(), EventInfoList.end(), CompareTrans);

			vector<ITransInfo*>::iterator itr = EventInfoList.begin();

			for( ; itr != EventInfoList.end(); ++itr )
			{
				ITransScaleInfo * pTransInfo = dynamic_cast<ITransScaleInfo*>((*itr));

				if( pTransInfo != NULL )
				{
					GwVector vScale = GwVector(pTransInfo->m_vTransScale.x * m_vStartScale.x, pTransInfo->m_vTransScale.y * m_vStartScale.y, pTransInfo->m_vTransScale.z * m_vStartScale.z);
					IGwTransformEvent_Pos * pEvent = NULL;
					pEvent = Gw::GetEventTransformMgr()->CreateEvent_Pos (m_fEmissionLifeTime * pTransInfo->m_fTime, vScale);
					m_pScale->AddEvent( pEvent );	pEvent->Release();
				}
			}
		}
	}
	else 
	{
		IComponent_Trans_Scale * pComponent_Trans_Scale = m_pParticleObject->GetComponent<IComponent_Trans_Scale>();

		if( pComponent_Trans_Scale != NULL )
		{
			int nCount = pComponent_Trans_Scale->GetNumEvent();

			for( int i = 0; i < nCount; ++i )
			{
				ITransScaleInfo * pTransInfo = dynamic_cast<ITransScaleInfo*>(pComponent_Trans_Scale->GetEvent_byIndex(i));

				if( pTransInfo != NULL )
				{
					GwVector vScale = GwVector(pTransInfo->m_vTransScale.x * m_vStartScale.x, pTransInfo->m_vTransScale.y * m_vStartScale.y, pTransInfo->m_vTransScale.z * m_vStartScale.z);

					IGwTransformEvent_Pos * pEvent = NULL;
					pEvent = Gw::GetEventTransformMgr()->CreateEvent_Pos (m_fEmissionLifeTime * pTransInfo->m_fTime, vScale);
					m_pScale->AddEvent( pEvent );	pEvent->Release();
				}
			}
		}
	}
}

void				IParticle::CreateComponent_Trans_Color()
{
	m_pColor->Release();
	m_pColor = Gw::GetEventTransformMgr()->CreateTransformer_Color(m_clrStartColor);

	if( ISingletonObject::IsTool() == true )
	{
		IComponent_Trans_Color * pComponent_Trans_Color = m_pParticleObject->GetComponent<IComponent_Trans_Color>();

		if( pComponent_Trans_Color != NULL && pComponent_Trans_Color->IsEnable() == true )
		{
			vector<ITransInfo*>		EventInfoList;

			int nCount = pComponent_Trans_Color->GetNumEvent();

			for( int i = 0; i < nCount; ++i )
			{
				ITransInfo * pTransInfo = pComponent_Trans_Color->GetEvent_byIndex(i);

				if( pTransInfo != NULL )
				{
					EventInfoList.push_back(pTransInfo);
				}
			}

			sort(EventInfoList.begin(), EventInfoList.end(), CompareTrans);

			vector<ITransInfo*>::iterator itr = EventInfoList.begin();

			for( ; itr != EventInfoList.end(); ++itr )
			{
				ITransColorInfo * pTransInfo = dynamic_cast<ITransColorInfo*>((*itr));

				if( pTransInfo != NULL )
				{
					GwColor clrColor = pTransInfo->m_clrColor;
					IGwTransformEvent_Color * pEvent = NULL;
					pEvent = Gw::GetEventTransformMgr()->CreateEvent_Color (m_fEmissionLifeTime * pTransInfo->m_fTime, clrColor);
					m_pColor->AddEvent( pEvent );	pEvent->Release();
				}
			}
		}
	}
	else 
	{
		IComponent_Trans_Color * pComponent_Trans_Color = m_pParticleObject->GetComponent<IComponent_Trans_Color>();

		if( pComponent_Trans_Color != NULL )
		{
			int nCount = pComponent_Trans_Color->GetNumEvent();

			for( int i = 0; i < nCount; ++i )
			{
				ITransColorInfo * pTransInfo = dynamic_cast<ITransColorInfo*>(pComponent_Trans_Color->GetEvent_byIndex(i));

				if( pTransInfo != NULL )
				{;
					GwColor clrColor = pTransInfo->m_clrColor;
					IGwTransformEvent_Color * pEvent = NULL;
					pEvent = Gw::GetEventTransformMgr()->CreateEvent_Color (m_fEmissionLifeTime * pTransInfo->m_fTime, clrColor);
					m_pColor->AddEvent( pEvent );	pEvent->Release();
				}
			}
		}
	}
}

void				IParticle::CreateComponent_Trans_Alpha()
{
	m_pAlpha->Release();
	m_pAlpha = Gw::GetEventTransformMgr()->CreateTransformer_Scalar(m_fStartAlpha);

	if( ISingletonObject::IsTool() == true )
	{
		IComponent_Trans_Alpha * pComponent_Trans_Alpha = m_pParticleObject->GetComponent<IComponent_Trans_Alpha>();

		if( pComponent_Trans_Alpha != NULL && pComponent_Trans_Alpha->IsEnable() == true )
		{
			vector<ITransInfo*>		EventInfoList;

			int nCount = pComponent_Trans_Alpha->GetNumEvent();

			for( int i = 0; i < nCount; ++i )
			{
				ITransInfo * pTransInfo = pComponent_Trans_Alpha->GetEvent_byIndex(i);

				if( pTransInfo != NULL )
				{
					EventInfoList.push_back(pTransInfo);
				}
			}

			sort(EventInfoList.begin(), EventInfoList.end(), CompareTrans);

			vector<ITransInfo*>::iterator itr = EventInfoList.begin();

			for( ; itr != EventInfoList.end(); ++itr )
			{
				ITransAlphaInfo * pTransInfo = dynamic_cast<ITransAlphaInfo*>((*itr));

				if( pTransInfo != NULL )
				{
					IGwTransformEvent_Scalar * pEvent = NULL;
					pEvent = Gw::GetEventTransformMgr()->CreateEvent_Scalar (m_fEmissionLifeTime * pTransInfo->m_fTime, pTransInfo->m_fAlpha);
					m_pAlpha->AddEvent( pEvent );	pEvent->Release();
				}
			}
		}
	}
	else 
	{
		IComponent_Trans_Alpha * pComponent_Trans_Alpha = m_pParticleObject->GetComponent<IComponent_Trans_Alpha>();

		if( pComponent_Trans_Alpha != NULL )
		{
			int nCount = pComponent_Trans_Alpha->GetNumEvent();

			for( int i = 0; i < nCount; ++i )
			{
				ITransAlphaInfo * pTransInfo = dynamic_cast<ITransAlphaInfo*>(pComponent_Trans_Alpha->GetEvent_byIndex(i));

				if( pTransInfo != NULL )
				{
					IGwTransformEvent_Scalar * pEvent = NULL;
					pEvent = Gw::GetEventTransformMgr()->CreateEvent_Scalar (m_fEmissionLifeTime * pTransInfo->m_fTime, pTransInfo->m_fAlpha);
					m_pAlpha->AddEvent( pEvent );	pEvent->Release();
				}
			}
		}
	}
}

void				IParticle::CreateComponent_Trans_Rotation()
{
	m_nCurrTransRotationIndex = 0;
	m_fRotationTime = 0.0f;

	m_pRotation->Release();
	m_pRotation = Gw::GetEventTransformMgr()->CreateTransformer_Scalar(0);

	if( ISingletonObject::IsTool() == true )
	{
		IComponent_Trans_Rotation * pComponent_Trans_Rotation = m_pParticleObject->GetComponent<IComponent_Trans_Rotation>();

		if( pComponent_Trans_Rotation != NULL && pComponent_Trans_Rotation->IsEnable() == true )
		{
			vector<ITransInfo*>		EventInfoList;

			int nCount = pComponent_Trans_Rotation->GetNumEvent();

			for( int i = 0; i < nCount; ++i )
			{
				ITransInfo * pTransInfo = pComponent_Trans_Rotation->GetEvent_byIndex(i);

				if( pTransInfo != NULL )
				{
					EventInfoList.push_back(pTransInfo);
				}
			}

			sort(EventInfoList.begin(), EventInfoList.end(), CompareTrans);

			vector<ITransInfo*>::iterator itr = EventInfoList.begin();

			for( ; itr != EventInfoList.end(); ++itr )
			{
				ITransRotationInfo * pTransInfo = dynamic_cast<ITransRotationInfo*>((*itr));

				if( pTransInfo != NULL )
				{
					IGwTransformEvent_Scalar * pEvent = NULL;
					pEvent = Gw::GetEventTransformMgr()->CreateEvent_Scalar (m_fEmissionLifeTime * pTransInfo->m_fTime , 0);
					m_pRotation->AddEvent( pEvent );	pEvent->Release();
				}

				m_pRotation->SetCallback(new IEventTransformCallback_Rotation(this));
			}

			if( EventInfoList.empty() == false && EventInfoList[0]->m_fTime == 0.0f )
			{
				SetRotationTransInfo(m_nCurrTransRotationIndex + 1);
				++m_nCurrTransRotationIndex;
			}
		}
	}
	else 
	{
		IComponent_Trans_Rotation * pComponent_Trans_Rotation = m_pParticleObject->GetComponent<IComponent_Trans_Rotation>();

		if( pComponent_Trans_Rotation != NULL )
		{
			int nCount = pComponent_Trans_Rotation->GetNumEvent();

			for( int i = 0; i < nCount; ++i )
			{
				ITransRotationInfo * pTransInfo = dynamic_cast<ITransRotationInfo*>(pComponent_Trans_Rotation->GetEvent_byIndex(i));

				if( pTransInfo != NULL )
				{
					IGwTransformEvent_Scalar * pEvent = NULL;
					pEvent = Gw::GetEventTransformMgr()->CreateEvent_Scalar (m_fEmissionLifeTime * pTransInfo->m_fTime , 0);
					m_pRotation->AddEvent( pEvent );	pEvent->Release();
				}

				m_pRotation->SetCallback(new IEventTransformCallback_Rotation(this));
			}

			if( nCount > 0 )
			{
				ITransRotationInfo * pTransInfo = dynamic_cast<ITransRotationInfo*>(pComponent_Trans_Rotation->GetEvent_byIndex(0));

				if( pTransInfo != NULL && pTransInfo->m_fTime == 0 )
				{
					SetRotationTransInfo(m_nCurrTransRotationIndex + 1);
					++m_nCurrTransRotationIndex;
				}
			}
		}
	}
}

void				IParticle::OnOneEventDone_Rotation()
{
	SetRotationTransInfo(m_nCurrTransRotationIndex);
	++m_nCurrTransRotationIndex;
}

void				IParticle::SetRotationTransInfo(int nIndex)
{
	IComponent_Trans_Rotation * pComponent_Trans_Rotation = m_pParticleObject->GetComponent<IComponent_Trans_Rotation>();

	if( pComponent_Trans_Rotation != NULL )
	{
		if( m_pTransRotationInfo != NULL ) m_pTransRotationInfo->Release();
		m_pTransRotationInfo = dynamic_cast<ITransRotationInfo*>(pComponent_Trans_Rotation->GetEvent_byIndex(nIndex - 1));
		if( m_pTransRotationInfo != NULL ) m_pTransRotationInfo->AddRef();

		m_fRotationTime = 0;
	}
}

