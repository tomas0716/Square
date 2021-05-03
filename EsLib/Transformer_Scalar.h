#pragma once

class ITransformer_Scalar : public ITransformer
{
public:
	ITransformer_Scalar(float fScalar);
	~ITransformer_Scalar(void);

	virtual bool		Update(float fElapsedTime);

	/** 현재 시간의 스칼라(하나의값) 을 얻는다. */
	float				GetCurrScalar()			{ return m_fScalar; }

private:

	virtual void		OneEvent();
	virtual void		CompleteEvent();
	virtual void		OnReset();

	float				m_fScalar;
	float				m_fPrevScalar;
};
