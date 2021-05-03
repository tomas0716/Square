#pragma once

class ITransformer_Pos : public ITransformer
{
public:
	ITransformer_Pos(GwVector vPos);
	~ITransformer_Pos(void);

	virtual bool		Update(float fElapsedTime);

	/** 현재 시간의 세개의 값( 위치값 ) 을 얻기 */
	virtual GwVector	GetCurrPos()			{ return m_vPos; }

private:

	virtual void		OneEvent();
	virtual void		CompleteEvent();
	virtual void		OnReset();

	GwVector			m_vPos;
	GwVector			m_vPrevPos;
};
