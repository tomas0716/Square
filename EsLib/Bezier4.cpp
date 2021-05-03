#include "stdafx.h"
#include "Bezier4.h"

IBezier4::IBezier4() 
: m_ePlayType(ePlay_Stop), m_fTime(0), m_fCurrBezierTime(0), m_nBlockStep(0), 
  m_vCurrPosition(GwVector(0,0,0))
{
	m_pDelegate_Complete = new IDelegate();
}

IBezier4::~IBezier4()
{
	m_PositionList.clear();
	m_BlockTimeList.clear();
	m_RealBlockTimeList.clear();

	m_pDelegate_Complete->Release();
}

bool		IBezier4::Update(float fDeltaTime)
{
	if( m_ePlayType == ePlay_Play )
	{
		m_fTime += fDeltaTime;

		m_vCurrPosition = Update_Bezier4( m_PositionList[m_nBlockStep * 3], m_PositionList[m_nBlockStep * 3 + 1], 
										  m_PositionList[m_nBlockStep * 3 + 2], m_PositionList[m_nBlockStep * 3 + 3], 
										  m_BlockTimeList[m_nBlockStep]);

		if( m_BlockTimeList[m_nBlockStep] <= m_fTime )
		{
			if( m_nBlockStep < (int)m_BlockTimeList.size() - 1 )
			{
				m_fTime = m_fTime - m_BlockTimeList[m_nBlockStep];
				m_nBlockStep += 1;

				return true;
			}
			else 
			{
				m_ePlayType = ePlay_Complete;

				(*m_pDelegate_Complete)();

				return true;
			}
		}
	}

	return false;
}

GwVector	IBezier4::GetCurrPos()
{
	return m_vCurrPosition;
}

void		IBezier4::AddPos(GwVector vPos)
{
	m_PositionList.push_back(vPos);
}

void		IBezier4::OnPlay()
{
	if( m_ePlayType == ePlay_Stop || m_ePlayType == ePlay_Complete )
	{
		m_fTime = 0;
		m_nBlockStep = 0;
	}

	m_ePlayType = ePlay_Play;
}

void		IBezier4::OnPause()
{
	if( m_ePlayType == ePlay_Play )
	{
		m_ePlayType = ePlay_Pause;
	}
}

void		IBezier4::OnStop()
{
	if( m_ePlayType == ePlay_Play || m_ePlayType == ePlay_Pause )
	{
		m_ePlayType = ePlay_Stop;
	}
}

void		IBezier4::ResetBlockTime()
{
	m_BlockTimeList.clear();	
}

void		IBezier4::SetBlockTime(int Index, float fTime)
{
	if( Index >= (int)m_BlockTimeList.size() )
	{
		m_BlockTimeList.resize(Index+1);
	}

	m_BlockTimeList[Index] = fTime;
}

void		IBezier4::SetCallback(IDelegate * pDelegate)
{
	m_pDelegate_Complete->Release();
	m_pDelegate_Complete = pDelegate;
	m_pDelegate_Complete->AddRef();
}

GwVector	IBezier4::Update_Bezier4(GwVector vFirst, GwVector vSecond, GwVector vThird, GwVector vFourth, float fBlockTime)
{
	float fRatio = m_fTime / fBlockTime;

	float mum1, mum13, mu3;

	mum1 = 1 - fRatio;
	mum13 = mum1 * mum1 * mum1;
	mu3 = fRatio * fRatio * fRatio;

	GwVector vCurrPos;
	vCurrPos = mum13*vFirst + 3*fRatio*mum1*mum1*vSecond + 3*fRatio*fRatio*mum1*vThird + mu3*vFourth;

	return vCurrPos;
}