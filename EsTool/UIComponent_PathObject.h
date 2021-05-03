#pragma once

class CUIComponent_PathObject : public CUIComponent
{
	I_DECLARE_DYNCREATE(CUIComponent_PathObject);

	enum eDot_EditBox
	{
		eDot_EditBox_Time,
		eDot_EditBox_X,
		eDot_EditBox_Y,
		eDot_EditBox_Z,

		eDot_EditBox_Max,
	};

public:
	CUIComponent_PathObject(void);
	~CUIComponent_PathObject(void);

	virtual void		Initialize();
	virtual void		CreateUI_Dot();

	virtual void		Update(float fDeltaTime);

	void				OnCheckBox_Entire(bool IsEntire);
	void				ChangeText_Entire();
	void				ChangeText_Block(sPathDotInfo * pPathDotInfo, eDot_EditBox eEditBox);
	void				OnButtonClick_Dot_Remove(sPathDotInfo * pPathDotInfo);
	void				OnButtonClick_Dot_Select(sPathDotInfo * pPathDotInfo, int nIndexPlus);
	void				OnButtonClick_Controller_Reset_Tangent(sPathDotInfo * pPathDotInfo);

	void				OnCheckBox_CreationAutoPlay(bool IsAutoPlay);

	void				OnButtonClick_GameObject_Remove();

	void				OnButtonClick_Play();
	void				OnButtonClick_Pause();
	void				OnButtonClick_Stop();
	void				OnButtonClick_Align_Tail();
	void				OnButtonClick_Reset_Tangent();

private:

	void				OnPathObjectInfoChange(NotificationArg * p);
	void				OnDragReceive_GameObject(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender);

	IGwUIEditbox		*	m_pEditbox_EntireTime;
	IGwUICheckbox		*	m_pCheckBox_Entire;

	struct sDotInfo
	{
		sDotInfo()
		{
			pBaseCtrl = NULL;
			pStatic_Length = NULL;
			for( int i = 0; i < eDot_EditBox_Max; ++i )
				pEditBox[i] = NULL;
		}
		~sDotInfo()
		{
			if( pBaseCtrl != NULL ) pBaseCtrl->Release();
			if( pStatic_Length != NULL ) pStatic_Length->Release();
			for( int i = 0; i < eDot_EditBox_Max; ++i )
				if( pEditBox[i] != NULL ) pEditBox[i]->Release();
		}

		IGwUIDummyCtrl	*	pBaseCtrl;
		IGwUIStatic		*	pStatic_Length;
		IGwUIEditbox	*	pEditBox[eDot_EditBox_Max];
	};

	map<sPathDotInfo*,sDotInfo*>	m_DotInfoTable;

	IGwUICheckbox		*	m_pCheckBox_CreationAutoPlay;
	CUIDragItemSprite	*	m_pUIDragItemSprite_GameObject;
	IGwUIStatic			*	m_pStatic_GameObjectName;
	IGwUIButton			*	m_pButton_Remove;

	IGwUIButton			*	m_pButton_Play;
	IGwUIButton			*	m_pButton_Pause;
	IGwUIButton			*	m_pButton_Stop;

	class CCheckboxCallback_Entire;
	class CUIEditbox_Callback_Entire;
	class CUIEditbox_Callback_Block;
	class CUIButton_Callback_Dot_Remove;
	class CUIButton_Callback_Dot_Select;
	class CUIButton_Callback_Controller_Reset_Tangent;

	class CCheckboxCallback_CreationAutoPlay;
	class CUIButton_Callback_GameObject_Remove;

	class CUIButton_Callback_Play;
	class CUIButton_Callback_Pause;
	class CUIButton_Callback_Stop;	
	class CUIButton_Callback_Align_Tail;
	class CUIButton_Callback_Reset_Tangent;
};