#include "StdAfx.h"
#include "Transformer_Scalar.h"

ITransformer_Scalar::ITransformer_Scalar(float fScalar)
{
	m_fScalar = m_fPrevScalar = fScalar;
	m_eTransformerType = eTransformerType_Scalar; 

	ITransformerEvent_Scalar * pEvent = new ITransformerEvent_Scalar(0.f, fScalar);
	AddEvent(pEvent); pEvent->Release();

	m_pCurrEvent = m_pPrevEvent = m_EventList[0];
}

ITransformer_Scalar::~ITransformer_Scalar(void)
{

}

bool		ITransformer_Scalar::Update(float fElapsedTime)
{
	if( m_bPlaying == true )
	{
		if( UpdateEvent(fElapsedTime) == false )
		{
			m_fScalar = Update_Linear(m_pPrevEvent->GetScalar(),m_pPrevEvent->GetEventTime(),m_pCurrEvent->GetScalar(),m_pCurrEvent->GetEventTime());
		}
	}

	return true;
}

void		ITransformer_Scalar::OneEvent()
{
	m_fPrevScalar = m_pPrevEvent->GetScalar();
}

void		ITransformer_Scalar::CompleteEvent()
{
	m_fScalar = m_pCurrEvent->GetScalar();
}

void		ITransformer_Scalar::OnReset()
{
	m_fScalar = m_pCurrEvent->GetScalar();
}