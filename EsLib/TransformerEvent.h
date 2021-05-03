#pragma once

class ITransformerEvent : public IGwRefObject
{
public:
	ITransformerEvent(void) {}
	~ITransformerEvent(void) {}

	virtual float GetEventTime()		{ return m_fEventTime; }
	virtual float GetScalar()			{ return m_fScalar; }
	virtual GwVector GetPos()			{ return m_vPos; }

protected:
	float								m_fEventTime;
	float								m_fScalar;
	GwVector							m_vPos;
};

class ITransformerEvent_Scalar : public ITransformerEvent
{
public:
	ITransformerEvent_Scalar(float fTime, float fScalar);
	~ITransformerEvent_Scalar(void)	{ }
};

class ITransformerEvent_Pos : public ITransformerEvent
{
public:
	ITransformerEvent_Pos(float fTime, GwVector vPos);
	~ITransformerEvent_Pos(void)	{ }
};