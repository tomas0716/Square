#include "StdAfx.h"
#include "Transformer.h"

void CALLBACK Callback_Null(void * pVoid, ITransformer * pTransformer, const eTransformerEvent & eEvent)	{ }

ITransformer::ITransformer(void)
: m_eTransformerType(eTransformerType_None), m_fTime(0.f),
  m_nEventIdx(0), m_pCurrEvent(NULL), m_pPrevEvent(NULL), m_bComplete(false), m_bPlaying(false)

{
	m_pCallback = new ITransformer_Callback();
}

ITransformer::~ITransformer(void)
{
	vector<ITransformerEvent*>::iterator itr = m_EventList.begin();

	for( ; itr != m_EventList.end(); ++itr )
	{
		ITransformerEvent * pEvent = (*itr);
		pEvent->Release();
	}

	m_EventList.clear();

	m_pCallback->Release();
}

bool					ITransformer::UpdateEvent(float fElapsedTime)
{
	m_fTime += fElapsedTime;

	if( m_fTime >= m_pCurrEvent->GetEventTime() && m_bComplete == false )
	{
		m_pPrevEvent = m_pCurrEvent;

		if( m_nEventIdx + 1 < (int)m_EventList.size() )
		{	
			m_pCurrEvent = m_EventList[++m_nEventIdx];
			OneEvent();
			m_pCallback->OnOneEventDone(m_nEventIdx-1);

			UpdateEvent(0);
		}
		else
		{
			m_bComplete = true;
			CompleteEvent();
			m_pCallback->OnCompleteEventDone();
		}
	}

	return m_bComplete;
}

float					ITransformer::Update_Linear(float fPrevEventValue, float fPrevEventTime, float fCurrEventValue, float fCurrEventTime)
{
	if( fCurrEventTime == fPrevEventTime )
		return fCurrEventValue;

	float fRatio = ((fCurrEventValue - fPrevEventValue) / (fCurrEventTime - fPrevEventTime));

	float fTime = m_fTime;
	if( m_fTime > fCurrEventTime )
		fTime = fCurrEventTime;

	return (((fTime - fPrevEventTime) * fRatio) + fPrevEventValue);

	return 0;
}

GwVector				ITransformer::Update_Linear(GwVector vPrevEventValue, float fPrevEventTime, GwVector vCurrEventValue, float fCurrEventTime)
{
	if( fCurrEventTime == fPrevEventTime )
		return vCurrEventValue;

	float fTime = m_fTime;
	if( m_fTime > fCurrEventTime )
		fTime = fCurrEventTime;

	float fTemp =  (fCurrEventTime - fPrevEventTime);
	GwVector vRatio = GwVector((vCurrEventValue.x - vPrevEventValue.x) / fTemp, (vCurrEventValue.y - vPrevEventValue.y) / fTemp, (vCurrEventValue.z - vPrevEventValue.z) / fTemp );

	return (((fTime - fPrevEventTime) * vRatio) + vPrevEventValue);
}

void					ITransformer::OnPlay()
{
	PrevPlay();
	m_bPlaying = true;
	m_bComplete = false;
}

void					ITransformer::OnPause()
{
	m_bPlaying = false;
}

void					ITransformer::OnStop()
{
	m_bPlaying = false;

	m_fTime = 0.f;

	m_nEventIdx = 0;

	m_pCurrEvent = m_pPrevEvent = m_EventList[m_nEventIdx];

	OnReset();
}

void					ITransformer::AddEvent(ITransformerEvent * pEvent)
{
	pEvent->AddRef();
	m_EventList.push_back(pEvent);
}

void					ITransformer::ClearEvent()
{
	vector<ITransformerEvent*>::iterator itr = m_EventList.begin();

	for( ++itr; itr != m_EventList.end(); )
	{
		ITransformerEvent * pEvent = (*itr);
		pEvent->Release();

		itr = m_EventList.erase(itr);
	}
}

float					ITransformer::GetCurrTime()
{
	return m_fTime; 
}

int						ITransformer::GetNumEvent()
{
	return m_EventList.size();
}

ITransformerEvent	*	ITransformer::GetEvent(int nIndex)
{
	return m_EventList[nIndex];
}

void					ITransformer::SetCallback(ITransformer_Callback * pCallback)
{
	m_pCallback->Release();
	m_pCallback = pCallback;
	m_pCallback->AddRef();
}

void					ITransformer::ClearCallback()
{
	m_pCallback->Release();
	m_pCallback = new ITransformer_Callback();
}