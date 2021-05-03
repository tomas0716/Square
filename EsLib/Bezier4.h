#pragma once

class IBezier4 : public IGwRefObject
{
public:
	IBezier4();
	virtual ~IBezier4();

	bool		Update(float fDeltaTime);		// Block 하나가 끝날때 리턴값이 True 가 된다. ( .. 블럭을 알아와야 하는데 알 방법이 이거 밖에 없네.. )
	GwVector	GetCurrPos();

	void		AddPos(GwVector vPos);
	
	void		OnPlay();
	void		OnPause();
	void		OnStop();

	void		ResetBlockTime();
	void		SetBlockTime(int Index, float fTime);

	void		SetCallback(IDelegate * pDelegate);

private:

	GwVector	Update_Bezier4(GwVector vFirst, GwVector vSecond, GwVector vThird, GwVector vFourth, float fBlockTime);

	ePlayType			m_ePlayType;
	
	float				m_fTime;
	float				m_fCurrBezierTime;
	vector<GwVector>	m_PositionList;

	int					m_nBlockStep;
	vector<float>		m_BlockTimeList;
	vector<float>		m_RealBlockTimeList;

	bool				m_IsAutoBlockTime;
	GwVector			m_vCurrPosition;

	IDelegate		*	m_pDelegate_Complete;
};