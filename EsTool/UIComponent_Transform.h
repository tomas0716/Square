#pragma once

enum eTransformType
{
	eTransformType_Position,
	eTransformType_Rotation,
	eTransformType_Scale,

	eTransformType_Max,
};

enum eTransformValue
{
	eTransformValue_X,
	eTransformValue_Y,
	eTransformValue_Z,

	eTransformValue_Max,
};

class CUIComponent_Transform : public CUIComponent
{
	I_DECLARE_DYNCREATE(CUIComponent_Transform);
public:
	CUIComponent_Transform(void);
	~CUIComponent_Transform(void);

	virtual void		Initialize();

	// Callback Recv
	void				ChangeText_Transform(eTransformType eType, eTransformValue eValue);

private:

	void				OnNotification_Position_Change(NotificationArg * p);
	void				OnNotification_Rotation_Change(NotificationArg * p);
	void				OnNotification_Scale_Change(NotificationArg * p);

	IGwUIEditbox	*	m_eEditBox_Transform[eTransformType_Max][eTransformValue_Max];
	class CUIEditbox_Callback_Transform;
};
