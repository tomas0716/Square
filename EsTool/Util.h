#pragma once

class TBtnCallback_Delegate : public IGwUIButton_Callback
{
public:
	virtual void	OnButton_OK(class IGwUIButton* pControl) 
	{
		m_pEvent_Click->OnEvent(EventArg_Null::Object);
	}

	template< typename	_Handler, typename _Method >
	TBtnCallback_Delegate(_Handler* pHandler, _Method pMethod)
	{
		m_pEvent_Click		= new TEvent_Delegate< _Handler, _Method, EventArg_Null >(pHandler, pMethod);
	}

	~TBtnCallback_Delegate()
	{
		m_pEvent_Click->Release();
	}

private:
	TEvent< EventArg_Null >*	m_pEvent_Click;
};