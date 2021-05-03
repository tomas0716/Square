#include "StdAfx.h"
#include "UIComponent_Transform.h"

I_IMPLEMENT_DYNCREATE(CUIComponent_Transform, ISingletonObject::GetTypeModule())

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIEditbox_Callback_Transform

class CUIComponent_Transform::CUIEditbox_Callback_Transform : public IGwUIEditbox_Callback
{
public:
	CUIEditbox_Callback_Transform( CUIComponent_Transform * pComponent, eTransformType eType, eTransformValue eValue)
		:m_pComponent(pComponent), m_eTransformType(eType), m_eTransformValue(eValue)
	{
	}

	virtual void	OnChangeText(IGwUIEditbox* pControl)
	{
		m_pComponent->ChangeText_Transform(m_eTransformType, m_eTransformValue);
	}

private:
	CUIComponent_Transform	*	m_pComponent;
	eTransformType				m_eTransformType;
	eTransformValue				m_eTransformValue;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIComponent_Transform

CUIComponent_Transform::CUIComponent_Transform(void)
{
	CNotificationCenter::GetInstance()->AddObserver(Notification_GameObject_Position_Change, "CUIComponent_Transform", this, &CUIComponent_Transform::OnNotification_Position_Change);
	CNotificationCenter::GetInstance()->AddObserver(Notification_GameObject_Rotation_Change, "CUIComponent_Transform", this, &CUIComponent_Transform::OnNotification_Rotation_Change);
	CNotificationCenter::GetInstance()->AddObserver(Notification_GameObject_Scale_Change, "CUIComponent_Transform", this, &CUIComponent_Transform::OnNotification_Scale_Change);
}

CUIComponent_Transform::~CUIComponent_Transform(void)
{
	for( int nType = 0; nType < eTransformType_Max; ++nType )
	{
		for( int nValue = 0; nValue < eTransformValue_Max; ++nValue )
		{
			if (m_eEditBox_Transform[nType][nValue] != NULL ) 
			{
				m_pBaseControl->RemoveControl(m_eEditBox_Transform[nType][nValue]);
				m_eEditBox_Transform[nType][nValue]->Release();
			}
		}
	}

	if( CNotificationCenter::GetInstance() != NULL ) 
	{
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_GameObject_Position_Change, "CUIComponent_Transform");
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_GameObject_Rotation_Change, "CUIComponent_Transform");
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_GameObject_Scale_Change, "CUIComponent_Transform");
	}
}

void		CUIComponent_Transform::Initialize()
{
	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

	IGwUISprite * pSprite_Back = CUIHelper::CreateUISprite("transform.png");
	m_pBaseControl->AddControl(pSprite_Back); pSprite_Back->Release();

	m_fHeight = pSprite_Back->GetSize()->y;

	GwVector2 vStartPos = GwVector2(78, 29);
	float fInterval_X = 63;
	float fInterval_Y = 27;

	for( int nType = 0; nType < eTransformType_Max; ++nType )
	{
		GwVector vValue;

		switch(nType)
		{
		case eTransformType_Position:	vValue = m_pGameObject->GetPosition(); 	break;
		case eTransformType_Rotation:	vValue = m_pGameObject->GetRotation(); 	break;
		case eTransformType_Scale:		vValue = m_pGameObject->GetScale(); 	break;
		}

		for( int nValue = 0; nValue < eTransformValue_Max; ++nValue )
		{
			m_eEditBox_Transform[nType][nValue] = CUIHelper::CreateUIEditBox(12,"±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(1,1,1,1), GwColor(0,0,1,1));
			m_eEditBox_Transform[nType][nValue]->SetLimitEditboxSizeX(55);
			m_eEditBox_Transform[nType][nValue]->SetLocalPos(GwVector2(vStartPos.x + fInterval_X * nValue, vStartPos.y + fInterval_Y * nType));

			wchar_t wszValue[STRING_128] = { 0, };
			char szValue[STRING_128] = { 0, };
			switch(nValue)
			{
			case eTransformValue_X:		sprintf(szValue, "%.2f", vValue.x); 	break;
			case eTransformValue_Y:		sprintf(szValue, "%.2f", vValue.y); 	break;
			case eTransformValue_Z:		sprintf(szValue, "%.2f", vValue.z); 	break;
			}

			ConvertAnsiStringToWideCch(wszValue, szValue, STRING_128);	

			m_eEditBox_Transform[nType][nValue]->SetText(wszValue);

			CUIEditbox_Callback_Transform * pCallback = new CUIEditbox_Callback_Transform(this, (eTransformType)nType, (eTransformValue)nValue);
			m_eEditBox_Transform[nType][nValue]->SetCallback(pCallback); pCallback->Release();

			m_pBaseControl->AddControl(m_eEditBox_Transform[nType][nValue]);
		}
	}

	Gw::GetBillboardMgr()->EndTextureMerge();
}

void		CUIComponent_Transform::ChangeText_Transform(eTransformType eType, eTransformValue eValue)
{
	const wchar_t * pText = m_eEditBox_Transform[eType][eValue]->GetText();

	float fValue = _wtof(pText);

	switch(eType)
	{
	case eTransformType_Position:	
		{
			GwVector vPos = m_pGameObject->GetPosition();
			switch(eValue)
			{
			case eTransformValue_X:		m_pGameObject->SetPosition(GwVector(fValue,vPos.y,vPos.z));  	break;
			case eTransformValue_Y:		m_pGameObject->SetPosition(GwVector(vPos.x,fValue,vPos.z));  	break;
			case eTransformValue_Z:		m_pGameObject->SetPosition(GwVector(vPos.x,vPos.y,fValue));  	break;
			}
		}
		break;
	case eTransformType_Rotation:	
		{
			GwVector vRot = m_pGameObject->GetRotation();
			switch(eValue)
			{
			case eTransformValue_X:		m_pGameObject->SetRotation(GwVector(fValue,vRot.y,vRot.z));  	break;
			case eTransformValue_Y:		m_pGameObject->SetRotation(GwVector(vRot.x,fValue,vRot.z));  	break;
			case eTransformValue_Z:		m_pGameObject->SetRotation(GwVector(vRot.x,vRot.y,fValue));  	break;
			}
		}
		break;
	case eTransformType_Scale:
		{
			GwVector vScale = m_pGameObject->GetScale();
			switch(eValue)
			{
			case eTransformValue_X:		m_pGameObject->SetScale(GwVector(fValue,vScale.y,vScale.z));  	break;
			case eTransformValue_Y:		m_pGameObject->SetScale(GwVector(vScale.x,fValue,vScale.z));  	break;
			case eTransformValue_Z:		m_pGameObject->SetScale(GwVector(vScale.x,vScale.y,fValue));  	break;
			}
		}
		break;
	}

	CPickingObjectWireFrameRender::GetInstance()->OnUpdate();
}

void		CUIComponent_Transform::OnNotification_Position_Change(NotificationArg * p)
{
	NotificationArg_GameObject_Position * pArg = dynamic_cast<NotificationArg_GameObject_Position*>(p);

	if( pArg != NULL )
	{
		char szValue_X[STRING_128] = { 0, };
		char szValue_Y[STRING_128] = { 0, };
		char szValue_z[STRING_128] = { 0, };

		wchar_t wszValue_X[STRING_128] = { 0, };
		wchar_t wszValue_Y[STRING_128] = { 0, };
		wchar_t wszValue_Z[STRING_128] = { 0, };

		sprintf(szValue_X, "%.2f", pArg->m_vPosition.x);
		sprintf(szValue_Y, "%.2f", pArg->m_vPosition.y);
		sprintf(szValue_z, "%.2f", pArg->m_vPosition.z);

		ConvertAnsiStringToWideCch(wszValue_X, szValue_X, STRING_128);	
		ConvertAnsiStringToWideCch(wszValue_Y, szValue_Y, STRING_128);	
		ConvertAnsiStringToWideCch(wszValue_Z, szValue_z, STRING_128);	

		m_eEditBox_Transform[eTransformType_Position][eTransformValue_X]->SetText(wszValue_X);
		m_eEditBox_Transform[eTransformType_Position][eTransformValue_Y]->SetText(wszValue_Y);
		m_eEditBox_Transform[eTransformType_Position][eTransformValue_Z]->SetText(wszValue_Z);
	}
}

void		CUIComponent_Transform::OnNotification_Rotation_Change(NotificationArg * p)
{
	NotificationArg_GameObject_Rotation * pArg = dynamic_cast<NotificationArg_GameObject_Rotation*>(p);

	if( pArg != NULL )
	{
		char szValue_X[STRING_128] = { 0, };
		char szValue_Y[STRING_128] = { 0, };
		char szValue_z[STRING_128] = { 0, };

		wchar_t wszValue_X[STRING_128] = { 0, };
		wchar_t wszValue_Y[STRING_128] = { 0, };
		wchar_t wszValue_Z[STRING_128] = { 0, };

		sprintf(szValue_X, "%.2f", pArg->m_vRotation.x);
		sprintf(szValue_Y, "%.2f", pArg->m_vRotation.y);
		sprintf(szValue_z, "%.2f", pArg->m_vRotation.z);

		ConvertAnsiStringToWideCch(wszValue_X, szValue_X, STRING_128);	
		ConvertAnsiStringToWideCch(wszValue_Y, szValue_Y, STRING_128);	
		ConvertAnsiStringToWideCch(wszValue_Z, szValue_z, STRING_128);	

		m_eEditBox_Transform[eTransformType_Rotation][eTransformValue_X]->SetText(wszValue_X);
		m_eEditBox_Transform[eTransformType_Rotation][eTransformValue_Y]->SetText(wszValue_Y);
		m_eEditBox_Transform[eTransformType_Rotation][eTransformValue_Z]->SetText(wszValue_Z);
	}
}

void		CUIComponent_Transform::OnNotification_Scale_Change(NotificationArg * p)
{
	NotificationArg_GameObject_Scale * pArg = dynamic_cast<NotificationArg_GameObject_Scale*>(p);

	if( pArg != NULL )
	{
		char szValue_X[STRING_128] = { 0, };
		char szValue_Y[STRING_128] = { 0, };
		char szValue_z[STRING_128] = { 0, };

		wchar_t wszValue_X[STRING_128] = { 0, };
		wchar_t wszValue_Y[STRING_128] = { 0, };
		wchar_t wszValue_Z[STRING_128] = { 0, };

		sprintf(szValue_X, "%.2f", pArg->m_vScale.x);
		sprintf(szValue_Y, "%.2f", pArg->m_vScale.y);
		sprintf(szValue_z, "%.2f", pArg->m_vScale.z);

		ConvertAnsiStringToWideCch(wszValue_X, szValue_X, STRING_128);	
		ConvertAnsiStringToWideCch(wszValue_Y, szValue_Y, STRING_128);	
		ConvertAnsiStringToWideCch(wszValue_Z, szValue_z, STRING_128);	

		m_eEditBox_Transform[eTransformType_Scale][eTransformValue_X]->SetText(wszValue_X);
		m_eEditBox_Transform[eTransformType_Scale][eTransformValue_Y]->SetText(wszValue_Y);
		m_eEditBox_Transform[eTransformType_Scale][eTransformValue_Z]->SetText(wszValue_Z);
	}
}