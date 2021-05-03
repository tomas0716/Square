
template<class CallbackClass,class UserClass>
void					CUIHelper::BindUIScrollCallbackFunction( IGwUIScrollBase* pScroll, void* pUserData )
{
	IGwUISlider_Callback * pScrollButtonCallback;
	pScrollButtonCallback = Gw::GetUIControlMgr()->CreateUIControl_Callback<CallbackClass>();
	static_cast<CallbackClass*>(pScrollButtonCallback)->SetUserData( static_cast<UserClass*>(pUserData) );  
	pScroll->SetCallback( pScrollButtonCallback );
	pScrollButtonCallback->Release();
}

template<class DerivedFromButtonCallback>
DerivedFromButtonCallback*	CUIHelper::CreateButtonCallback()
{
	return Gw::GetUIControlMgr()->CreateUIControl_Callback< DerivedFromButtonCallback >();
}

template<class DerivedFromButtonCallback, typename P1>
DerivedFromButtonCallback*	CUIHelper::CreateButtonCallback(const P1& p1)
{
	return Gw::GetUIControlMgr()->CreateUIControl_Callback< DerivedFromButtonCallback >(p1);
}

template<class DerivedFromButtonCallback, typename P1, typename P2>
DerivedFromButtonCallback*	CUIHelper::CreateButtonCallback(const P1& p1, const P2& p2)
{
	return Gw::GetUIControlMgr()->CreateUIControl_Callback< DerivedFromButtonCallback >(p1, p2);
}

template<class DerivedFromButtonCallback, typename P1, typename P2, typename P3>
DerivedFromButtonCallback*	CUIHelper::CreateButtonCallback(const P1& p1, const P2& p2, const P3& p3)
{
	return Gw::GetUIControlMgr()->CreateUIControl_Callback< DerivedFromButtonCallback >(p1, p2, p3);
}

template<class DerivedFromButtonCallback, typename P1, typename P2, typename P3, typename P4>
DerivedFromButtonCallback*	CUIHelper::CreateButtonCallback(const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
	return Gw::GetUIControlMgr()->CreateUIControl_Callback< DerivedFromButtonCallback >(p1, p2, p3, p4);
}

template<class DerivedFromButtonCallback, typename P1, typename P2, typename P3, typename P4, typename P5>
DerivedFromButtonCallback*	CUIHelper::CreateButtonCallback(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
	return Gw::GetUIControlMgr()->CreateUIControl_Callback< DerivedFromButtonCallback >(p1, p2, p3, p4, p5);
}