#include "StdAfx.h"
#include "Transformer_Pos.h"

ITransformer_Pos::ITransformer_Pos(GwVector vPos)
{
	m_vPos = m_vPrevPos = vPos;
	m_eTransformerType = eTransformerType_Pos;

	ITransformerEvent_Pos * pEvent = new ITransformerEvent_Pos(0.f, vPos);
	AddEvent(pEvent); pEvent->Release();

	m_pCurrEvent = m_pPrevEvent = m_EventList[0];
}

ITransformer_Pos::~ITransformer_Pos(void)
{
}


bool		ITransformer_Pos::Update(float fElapsedTime)
{
	if( m_bPlaying == true )
	{
		if( UpdateEvent(fElapsedTime) == false )
		{
			m_vPos = Update_Linear(m_pPrevEvent->GetPos(), m_pPrevEvent->GetEventTime(), m_pCurrEvent->GetPos(), m_pCurrEvent->GetEventTime());
		}
	}

	return true;
}

void		ITransformer_Pos::OneEvent()
{
	m_vPrevPos = m_vPos;
}

void		ITransformer_Pos::CompleteEvent()
{
	m_vPos = m_pCurrEvent->GetPos();
}

void		ITransformer_Pos::OnReset()
{
	m_vPos = m_pCurrEvent->GetPos();
}