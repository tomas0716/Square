#include "StdAfx.h"
#include "EsToolApplication.h"


class CGameEvent_DokkaebiFire_CameraShaking::CCallback_StartDelay : public IGwEventTransformer_Callback
{
public:
	CCallback_StartDelay(CGameEvent_DokkaebiFire_CameraShaking * pThis) : m_pThis(pThis) { }

	virtual void OnDone() 
	{
		m_pThis->OnDone_StartDelay();
	}

private:
	CGameEvent_DokkaebiFire_CameraShaking * m_pThis;
};

class CGameEvent_DokkaebiFire_CameraShaking::CCallback_Shaking : public IGwEventTransformer_Callback
{
public:
	CCallback_Shaking(CGameEvent_DokkaebiFire_CameraShaking * pThis) : m_pThis(pThis) { }

	virtual void OnDone() 
	{
		m_pThis->OnDone_Shaking();
	}

private:
	CGameEvent_DokkaebiFire_CameraShaking * m_pThis;
};

CGameEvent_DokkaebiFire_CameraShaking::CGameEvent_DokkaebiFire_CameraShaking(IGwCamera * pCamera, float fStartDelay, float fShakingTime, float fRadius, int nShakingStrength)
: m_pCamera(pCamera), m_fShakingTime(fShakingTime), m_fRadius(fRadius), m_nShakingStrength(nShakingStrength)
{
	m_pPos_Eye = Gw::GetEventTransformMgr()->CreateTransformer_Pos(GwVector(0,0,0));
	m_pPos_Look = Gw::GetEventTransformMgr()->CreateTransformer_Pos(GwVector(0,0,0));

	IGwTransformEvent_Scalar * pEventScalar = NULL;

	m_pTimer_StartDelay = Gw::GetEventTransformMgr()->CreateTransformer_Scalar(0);
	pEventScalar = Gw::GetEventTransformMgr()->CreateEvent_Scalar(fStartDelay,0);
	m_pTimer_StartDelay->AddEvent(pEventScalar); pEventScalar->Release();
	m_pTimer_StartDelay->SetCallback(new CCallback_StartDelay(this));
	m_pTimer_StartDelay->Replay();

	m_pTimer_Shaking = Gw::GetEventTransformMgr()->CreateTransformer_Scalar(0);
	pEventScalar = Gw::GetEventTransformMgr()->CreateEvent_Scalar(fShakingTime,0);
	m_pTimer_Shaking->AddEvent(pEventScalar); pEventScalar->Release();
	m_pTimer_Shaking->SetCallback(new CCallback_Shaking(this));
}

CGameEvent_DokkaebiFire_CameraShaking::~CGameEvent_DokkaebiFire_CameraShaking()
{
	m_pTimer_StartDelay->Release();
	m_pTimer_Shaking->Release();

	m_pPos_Eye->Release();
	m_pPos_Look->Release();
}

void	CGameEvent_DokkaebiFire_CameraShaking::Update(float fDeltaTime)
{
	m_pTimer_StartDelay->Update(fDeltaTime);
	m_pTimer_Shaking->Update(fDeltaTime);

	m_pPos_Eye->Update(fDeltaTime);
	m_pPos_Look->Update(fDeltaTime);

	GwVector vEye = *m_pPos_Eye->GetCurrPos();
	GwVector vLook = *m_pPos_Look->GetCurrPos();

	//IGwCamera * pCamera = m_pRndMultiCameraMgr->GetCurrCamera()->GetGwCamera();
	
	if( m_pCamera != NULL )
	{
		m_pCamera->SetWorldPos(vEye);
		//m_pCamera->SetWorldAt(vLook);

		if( m_pCamera->GetTarget() != NULL )
		{
			float fTargetOffsetHeight = m_pCamera->GetTargetOffsetHeight();
			vLook.y -= fTargetOffsetHeight;
			m_pCamera->GetTarget()->SetWorldPos(vLook);
		}

		m_pCamera->Update(fDeltaTime);
	}	
}

void	CGameEvent_DokkaebiFire_CameraShaking::Render3D(float fDeltaTime)
{
}

void	CGameEvent_DokkaebiFire_CameraShaking::PostRender3D(float fDeltaTime)
{
}

void	CGameEvent_DokkaebiFire_CameraShaking::Render2D(float fDeltaTime)
{
}

void	CGameEvent_DokkaebiFire_CameraShaking::OnDone_StartDelay()
{
	if( m_pCamera == NULL ) return;

	//m_pRndMultiCameraMgr->SetActive(false);
	m_pTimer_Shaking->Replay();

	//CRndGameCamera * pRndGameCamera = m_pRndMultiCameraMgr->GetCurrCamera();
	//IGwCamera * pCamera = pRndGameCamera->GetGwCamera();

	GwVector vEye = *m_pCamera->GetWorldPos();
	GwVector vLook = *m_pCamera->GetWorldAt();

	m_pPos_Eye->Release();
	m_pPos_Look->Release();
	m_pPos_Eye = Gw::GetEventTransformMgr()->CreateTransformer_Pos(vEye);
	m_pPos_Look = Gw::GetEventTransformMgr()->CreateTransformer_Pos(vLook);

	int nNumShaking = (int)(m_fShakingTime * m_nShakingStrength) + 1;
	float fShakingIntervalTime = 1.f / ((float)m_nShakingStrength);
	float fTime = fShakingIntervalTime;

	for( int i = 0; i < nNumShaking; ++i )
	{
		GwVector vInterval = GwVector(GwRandomRange(-m_fRadius * 0.5f, m_fRadius * 0.5f), GwRandomRange(-m_fRadius * 0.5f, m_fRadius * 0.5f), GwRandomRange(-m_fRadius * 0.5f, m_fRadius * 0.5f));

		IGwTransformEvent_Pos * pEventValue = NULL;
		pEventValue = Gw::GetEventTransformMgr()->CreateEvent_Pos(fTime, vEye + vInterval);
		m_pPos_Eye->AddEvent(pEventValue); pEventValue->Release();

		vInterval = GwVector(GwRandomRange(-m_fRadius * 0.5f, m_fRadius * 0.5f), GwRandomRange(-m_fRadius * 0.5f, m_fRadius * 0.5f), GwRandomRange(-m_fRadius * 0.5f, m_fRadius * 0.5f));
		pEventValue = Gw::GetEventTransformMgr()->CreateEvent_Pos(fTime, vLook + vInterval);
		m_pPos_Look->AddEvent(pEventValue); pEventValue->Release();
	}

	m_pPos_Eye->Replay();
	m_pPos_Look->Replay();
}

void	CGameEvent_DokkaebiFire_CameraShaking::OnDone_Shaking()
{
	//m_pRndMultiCameraMgr->SetActive(true);
	//SetValid(false);
}


CEsToolApplication::CEsToolApplication(void) : m_fDelta(1)
{
}

CEsToolApplication::~CEsToolApplication(void)
{
	ISingletonObject::DestroyTypeModul();

	Gw::End();
}

bool		CEsToolApplication::OnCreateDevice()
{
	Gw::Start(CFrameWorks::CreateInstance()->GetHwnd());
	Gw::OnCreateDevice(CFrameWorks::CreateInstance()->GetHwnd(),CFrameWorks::CreateInstance()->GetDevice());

	IGwUICandidate::Init(Gw::GethWnd());

	IGwGameStage*	pStage = Gw::GetGameStageMgr()->CreateGameStage<CGameStage_Main>("CGameStage_Main");
	Gw::GetGameStageMgr()->SetGameStage(1,pStage);
	pStage->Release();

	m_pShaking = new CGameEvent_DokkaebiFire_CameraShaking(NULL, 2, 1, 30, 30);

	return true;
}

bool		CEsToolApplication::OnResetDevice()
{
	Gw::OnResetDevice();

	return true;
}

bool		CEsToolApplication::OnLostDevice()
{
	Gw::OnLostDevice();

	return true;
}

bool		CEsToolApplication::OnDeleteDevice()
{
	IGwUICandidate::Uninit();

	Gw::OnDestroyDevice();

	m_pShaking->Release();

	return true;
}

bool		CEsToolApplication::Update(float fElapsedTime)
{
	if(Gw::GetKeyMapper()->WasKeyDown(VK_ADD) == TRUE)
	{
		m_fDelta = 20;
	}

	if(Gw::GetKeyMapper()->WasKeyDown(VK_SUBTRACT) == TRUE)
	{
		m_fDelta = 1;
	}

	if(Gw::GetKeyMapper()->WasKeyDown(VK_MULTIPLY) == TRUE)
	{
		m_fDelta = 0.04f;
	}

	if(Gw::GetKeyMapper()->WasKeyDown(VK_DIVIDE) == TRUE)
	{
		m_fDelta = 0;
	}

	Gw::Update(fElapsedTime * m_fDelta);

	EventArg_ElapsedTime	ElapsedTime;
	ElapsedTime.fElapsedTime = fElapsedTime * m_fDelta;
	CAppEvent::OnPostUpdate()->OnEvent(ElapsedTime);

	m_pShaking->Update(fElapsedTime);

	if( Gw::GetKeyMapper()->WasKeyDown(VK_F3) == TRUE )
	{
		m_pShaking->Release();
		m_pShaking = new CGameEvent_DokkaebiFire_CameraShaking(CPerspectiveCamera::GetInstance()->GetCamera(), 2, 1, 30, 30);
	}

	return true;
}

bool		CEsToolApplication::Render(float fElapsedTime)
{
	Gw::Render(fElapsedTime * m_fDelta);

	EventArg_ElapsedTime	ElapsedTime;
	ElapsedTime.fElapsedTime = fElapsedTime * m_fDelta;
	CAppEvent::OnPostRender()->OnEvent(ElapsedTime);

	return true;
}
