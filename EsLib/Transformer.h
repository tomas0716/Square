#pragma once

enum eTransformerEvent
{
	eTransformerEvent_None = 0,
	eTransformerEvent_OneEvent,
	eTransformerEvent_CompleteEvent,

	eTransformerEvent_Max,
};

enum eTransformerType
{
	eTransformerType_None = 0,
	eTransformerType_Scalar,
	eTransformerType_Pos,

	eTransformerType_Max,
};

enum eTransformerActionType
{
	eTransformerActionType_None = 0,
	eTransformerActionType_Linear,

	eTransformerActionType_Max,
};

class ITransformer_Callback : public IGwRefObject
{
public:
	ITransformer_Callback(void)	{ }
	~ITransformer_Callback(void)	{ }

	virtual void	OnOneEventDone(int nEventIndex)		{ }		///< 하나의 이벤트가 끝날때 호출되는 함수
	virtual void	OnCompleteEventDone()				{ }		///< 모든 이벤트가 끝날때 호출되는 함수
};

class ITransformer : public IGwRefObject
{
public:
	ITransformer(void);
	virtual ~ITransformer(void);

	virtual void			OnPlay();
	virtual void			OnPause();
	virtual void			OnStop();

	virtual void			AddEvent(ITransformerEvent * pEvent);
	virtual void			ClearEvent();

	virtual float			GetCurrTime();
	virtual int				GetNumEvent();
	virtual ITransformerEvent	*	GetEvent(int nIndex);

	virtual void			SetCallback(ITransformer_Callback * pCallback);
	virtual void			ClearCallback();

protected:

	bool					UpdateEvent(float fElapsedTime);

	float					Update_Linear(float fPrevEventValue, float fPrevEventTime, float fCurrEventValue, float fCurrEventTime);	
	GwVector				Update_Linear(GwVector vPrevEventValue, float fPrevEventTime, GwVector vCurrEventValue, float fCurrEventTime);

	virtual void			OneEvent()	= 0;
	virtual void			CompleteEvent()	= 0;
	virtual void			OnReset() = 0;
	virtual bool			PrevPlay()	{ return true; }
	
	eTransformerType		m_eTransformerType;
	float					m_fTime;

	ITransformerEvent	*	m_pCurrEvent;
	ITransformerEvent	*	m_pPrevEvent;
	int						m_nEventIdx;		// 다음 이벤트의 인덱스

	bool					m_bPlaying;
	bool					m_bComplete;

	vector<ITransformerEvent*>		m_EventList;

	ITransformer_Callback	*	m_pCallback;
};
