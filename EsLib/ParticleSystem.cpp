#include "StdAfx.h"
#include "ParticleSystem.h"

IParticleSystem::IParticleSystem(IParticleObject * pParticleObject)
: m_pParticleObject(pParticleObject), m_IsLocal(false), m_fTime(0), m_fPrevEmissionTime(0), m_ForceStop(false), m_ePlayType(ePlay_Stop)
{
}

IParticleSystem::~IParticleSystem(void)
{
	GwReleaseFunctor functor_Release;
	for_each(m_ParticleList.begin(),m_ParticleList.end(),functor_Release);
	for_each(m_DeadParticleList.begin(),m_DeadParticleList.end(),functor_Release);

	m_ParticleList.clear();
	m_DeadParticleList.clear();
}

void				IParticleSystem::UpdateRemove()
{
	vector<IParticle*>::iterator itr_Remove = m_RemoveParticleList.begin();

	for( ; itr_Remove != m_RemoveParticleList.end(); ++itr_Remove )
	{
		vector<IParticle*>::iterator itr = m_ParticleList.begin();

		for( ; itr != m_ParticleList.end(); ++itr )
		{
			if( (*itr) == (*itr_Remove) )
			{
				(*itr)->Release();
				m_ParticleList.erase(itr);
				break;
			}
		}
	}

	m_RemoveParticleList.clear();
}

void				IParticleSystem::Update(float fDeltaTime)
{
	//if( ISingletonObject::IsTool() == true )
	//{
	//	UpdateRemove();
	//}

	if( m_ForceStop == false && m_ePlayType == ePlay_Play)
	{
		m_fTime += fDeltaTime;
		if( m_fLifeTime >= m_fTime || m_pParticleObject->IsLoop() == true )
		{
			if( m_iEmissionRate != 0 )
			{
				if( m_fPrevEmissionTime == 0 || m_fPrevEmissionTime >= m_fEmissionRateTime / m_iEmissionRate )
				{
					if( m_fPrevEmissionTime != 0 )
						m_fPrevEmissionTime = m_fPrevEmissionTime - (m_fEmissionRateTime / m_iEmissionRate);

					int nEmissionCount = m_pParticleObject->GetEmissionCount()->GetValue();

					for( int i = 0; i < nEmissionCount; ++i )
					{
						CreateParticle();
					}
				}
			}

			m_fPrevEmissionTime += fDeltaTime;
		}
		else 
		{
			m_ePlayType = ePlay_Complete;
		}
	}

	if( m_IsLocal == false )
	{
		GwVector	vPos	= m_pParticleObject->GetWorldPos();

		for_each(m_ParticleList.begin(),m_ParticleList.end(),bind2nd(mem_fun(&IParticle::SetParentPos),vPos));
	}

	GwVector vCameraDir = *m_pParticleObject->GetCamera()->GetTargetPos() - *m_pParticleObject->GetCamera()->GetWorldPos();
	for_each(m_ParticleList.begin(),m_ParticleList.end(),bind2nd(mem_fun(&IParticle::SetCameraPos),*m_pParticleObject->GetCamera()->GetWorldPos()));
	for_each(m_ParticleList.begin(),m_ParticleList.end(),bind2nd(mem_fun(&IParticle::Update),fDeltaTime));
}

void				IParticleSystem::Render(float fDeltaTime)
{
	for_each(m_ParticleList.begin(),m_ParticleList.end(),bind2nd(mem_fun(&IParticle::Render),fDeltaTime));
}

void				IParticleSystem::ResetProperty()
{
	m_fLifeTime			= m_pParticleObject->GetLifeTime()->GetValue();
	m_fEmissionRateTime = m_pParticleObject->GetEmissionRateTime()->GetValue();
	m_iEmissionRate		= m_pParticleObject->GetEmissionRate()->GetValue();
	m_fStartDelay		= m_pParticleObject->GetStartDelay()->GetValue();
	m_IsLocal			= m_pParticleObject->IsEmissionLocal();
}

void				IParticleSystem::OnPlay()
{
	switch(m_ePlayType)
	{
	case ePlay_Complete:
	case ePlay_Stop:
		{
			m_fTime = 0;
			m_fPrevEmissionTime = 0;
			m_ePlayType = ePlay_Play;
		}
		break;

	case ePlay_Pause:
		{
			m_ePlayType = ePlay_Play;
		}
		break;
	}

	m_ForceStop = false;

	for_each(m_ParticleList.begin(),m_ParticleList.end(),mem_fun(&IParticle::OnPlay));
}

void				IParticleSystem::OnPause()
{
	if( m_ePlayType == ePlay_Play )
	{
		m_ePlayType = ePlay_Pause;
	}

	for_each(m_ParticleList.begin(),m_ParticleList.end(),mem_fun(&IParticle::OnPause));
}

void				IParticleSystem::OnStop()
{
	if( m_ePlayType == ePlay_Play || m_ePlayType == ePlay_Pause )
	{
		m_fTime = 0;
		m_fPrevEmissionTime = 0;
		m_ePlayType = ePlay_Stop;
	}

	for_each(m_ParticleList.begin(),m_ParticleList.end(),mem_fun(&IParticle::OnStop));
}

void				IParticleSystem::OnForceStop()
{
	m_ForceStop = true;
}

void				IParticleSystem::CreateParticle()
{
	float		fEmissionLifeTime	= m_pParticleObject->GetEmissionLifeTime()->GetValue();
	float		fStartSpeed			= m_pParticleObject->GetStartSpeed()->GetValue();
	GwVector	vStartScale			= m_pParticleObject->GetStartScale()->GetValue();
	GwVector	vStartRotation		= m_pParticleObject->GetStartRotation()->GetValue();
	GwColor		clrStartColor		= m_pParticleObject->GetStartColor()->GetValue();
	float		fStartAlpha			= m_pParticleObject->GetStartAlpha()->GetValue();
	float		fGravity			= m_pParticleObject->GetGravity()->GetValue();

	GwVector	vPos				= m_pParticleObject->GetWorldPos();
	GwVector	vRot				= m_pParticleObject->GetWorldRot();
	GwVector	vScale				= m_pParticleObject->GetWorldScale();

	GwVector vStartPosition, vStartDirection;
	GetStartInfo(vStartPosition, vStartDirection);

	IParticle * pParticle = NULL; 

	if( ISingletonObject::IsTool() == false )
	{
		pParticle = GetDeadParticle();
	}

	if( pParticle != NULL )
	{
		pParticle->Reset(vPos, vRot, vScale, vStartPosition, vStartDirection, fEmissionLifeTime, fStartSpeed, 
						 vStartScale, vStartRotation, clrStartColor, fStartAlpha, fGravity, m_IsLocal);
		pParticle->OnStop();
		pParticle->OnPlay();
	}
	else 
	{
		IParticleObjectInfo * pParticleObjectInfo = m_pParticleObject->GetParticleObjectInfo();
		IType * pType = ITypeManager::GetInstance()->FindType(pParticleObjectInfo->m_strGameObjectName.c_str());
		IGameObject * pGameObject = dynamic_cast<IGameObject*>(pType->CreateInstance());
		pParticleObjectInfo->SetObject(pGameObject);

		pParticle = new IParticle(this, m_pParticleObject, pGameObject, vPos, vRot, vScale, vStartPosition, vStartDirection, fEmissionLifeTime, fStartSpeed, 
								  vStartScale, vStartRotation, clrStartColor, fStartAlpha, fGravity, m_IsLocal);	pGameObject->Release();

		m_ParticleList.push_back(pParticle);
		pParticle->OnPlay();
	}
}

IParticle		*	IParticleSystem::GetDeadParticle()
{
	if( m_DeadParticleList.empty() == true ) return NULL;

	IParticle* pParticle = m_DeadParticleList.back();
	m_DeadParticleList.pop_back();	pParticle->Release();

	return pParticle;
}

void				IParticleSystem::InValidParticle(IParticle * pParticle)
{
	//m_RemoveParticleList.push_back(pParticle);
	m_DeadParticleList.push_back(pParticle); pParticle->AddRef();
	return ;

	//if( ISingletonObject::IsTool() == true )
	//{
	//	m_RemoveParticleList.push_back(pParticle);
	//}
	//else 
	//{
	//	m_DeadParticleList.push_back(pParticle);	pParticle->AddRef();
	//}
}

void				IParticleSystem::GetStartInfo(GwVector &vPosition, GwVector &vDirection)
{
	vPosition = GwVector(0,0,0);
	vPosition = GwVector(GwRandomRange(-60.f,60.f),GwRandomRange(-5,5),GwRandomRange(-5,5));
	vDirection = GwVector(GwRandomRange(-0.5f,0.5f),GwRandomRange(-0.5f,0.5f),GwRandomRange(-0.5f,0.5f));
	vDirection = GwVector(0,100,0);

	//D3DXVec3Normalize(&vDirection, &vDirection);
}