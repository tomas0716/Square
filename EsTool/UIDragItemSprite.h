#pragma once

class CUIDragItemSprite : public CUIDragItemBase
{
public:
	CUIDragItemSprite(eDragObjectType eType, eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, string strFilePath, string strFileName, IGwRefObject * pParameta);
	~CUIDragItemSprite(void);

	IGwUISprite		*	GetUISprite();
	IGwRefObject	*	GetParameta();

protected:

	void				OnUpdate(const EventArg_ElapsedTime& Arg);

	IGwUISprite		*	m_pSprite;
	CUniqueName			m_strUniqueName;

	IGwRefObject	*	m_pParameta;
};
