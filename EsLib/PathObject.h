#pragma once

struct sPathDotInfo
{
	int					nIndex;
	float				fTime;
	GwVector			vLocalPos;
	GwVector			vWorldPos;

	bool				bController;			// 컨트롤러 유무
	float				fLength;

	float				fRealTime;

	sPathDotInfo()
	{
		nIndex			= 0;
		fTime			= 0;
		vLocalPos		= GwVector(0,0,0);
		vWorldPos		= GwVector(0,0,0);

		bController		= false;
		fLength			= 0;

		fRealTime		= 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class IPathObject : public IGameObject
{
	I_DECLARE_DYNCREATE(IPathObject);
public:
	IPathObject();
	IPathObject(const char * pName);
	virtual ~IPathObject(void);

	virtual void				Update(float fDeltaTime);
	virtual void				Render(float fDeltaTime);
	virtual void				Render(float fDeltaTime, IGwViewFrustumOct * pFrustum);

	virtual void				OnPlay();
	virtual void				OnStop();
	virtual void				OnPause();

	virtual ePlayType			GetCurrPlayType();

	virtual void				AddEvent(sPathDotInfo * pEvent);
	virtual void				AddEvent(float fTime, GwVector vPosition, bool bController = false);

	virtual void				InsertEvent(int nIndex, sPathDotInfo * pEvent);
	virtual void				InsertEvent(int nIndex, float fTime, GwVector vPosition, bool bController = false);

	virtual void				EventTimeAdd(int nIndex);

	virtual void				DeleteEvent(int nIndex);
	virtual void				DeleteEvent(sPathDotInfo * pEvent);

	virtual void				DeleteEventAll();

	virtual int					GetNumEvent();
	virtual sPathDotInfo	*	GetEvent(int nIndex);

	virtual void				ReLoad();

	void						OnOneEventDone(int nIndex);
	void						OnEventDone();

	virtual void				SetBlockTime(bool bBlockTime);
	virtual bool				GetBlockTime();

	virtual void				SetEntireTime(float fTime);
	virtual float				GetEntireTime();

	virtual void 				SetFrameRate(float FrameRate);
	virtual float 				GetFrameRate();

	virtual void				SetCallback(IDelegate * pDelegate_OneDot, IDelegate * pDelegate_Complete);
	virtual void				ClearCallback();

	virtual GwVector			GetCurrPos();

	void						SetCreationAutoPlay(bool IsAutoPlay);
	bool						IsCreationAutoPlay();

	void						SetAttachGameObject(IGameObject * pGameObject);
	bool						IsAttachGameObject();
	IGameObject				*	GetAttachGameObject();

	virtual BYTE			*	LoadFile(BYTE * pData);
	virtual void				SaveFile(IFile & kFile);

protected:

	virtual void				ReLoad_NoneBlockTime();
	virtual void				ReLoad_BlockTime();
	virtual int					LoadBlockTime(float fPrevTime, vector<sPathDotInfo*> & EventInfoList);

	void						ResetEventIndex();

	vector<sPathDotInfo*>		m_PathDotInfoList;

	ITransformer_Pos		*	m_pTransformer;

	bool						m_bBlockTime;
	float						m_fEntireTime;

	float						m_fFrameRate;

	vector<int>					m_BlockEndIndexList;

	IDelegate				*	m_pDelegate_OneDot;
	IDelegate				*	m_pDelegate_Complete;

	ePlayType					m_ePlayType;

	bool						m_IsCreationAutoPlay;

	bool						m_IsAttachGameObject;
	IGameObject				*	m_pAttachGameObject;

	GwVector					m_vTransformerPos;

	class IEventTransformerCallback_Path;
};
