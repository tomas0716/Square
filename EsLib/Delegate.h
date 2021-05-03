#pragma once

class IDelegate_Store_Parent
{
public:
	IDelegate_Store_Parent()		{ }
	~IDelegate_Store_Parent()	{ }
};

class IDelegate_Store_00 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_00() { }

	typedef void (*Function) (void*  pObjectPtr);
	Function  m_Function;
};

template<typename Arg01>
class IDelegate_Store_01 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_01() { }

	typedef void (*Function) (void* pObjectPtr, Arg01);
	Function  m_Function;
};

template<typename Arg01, typename Arg02>
class IDelegate_Store_02 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_02() { }

	typedef void (*Function) (void* pObjectPtr, Arg01, Arg02);
	Function  m_Function;
};

template<typename Arg01, typename Arg02, typename Arg03>
class IDelegate_Store_03 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_03() { }

	typedef void (*Function) (void* pObjectPtr, Arg01, Arg02, Arg03);
	Function  m_Function;
};

template<typename Arg01, typename Arg02, typename Arg03, typename Arg04>
class IDelegate_Store_04 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_04() { }

	typedef void (*Function) (void* pObjectPtr, Arg01, Arg02, Arg03, Arg04);
	Function  m_Function;
};

template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05>
class IDelegate_Store_05 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_05() { }

	typedef void (*Function) (void* pObjectPtr, Arg01, Arg02, Arg03, Arg04, Arg05);
	Function  m_Function;
};

template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06>
class IDelegate_Store_06 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_06() { }

	typedef void (*Function) (void* pObjectPtr, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06);
	Function  m_Function;
};

template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07>
class IDelegate_Store_07 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_07() { }

	typedef void (*Function) (void* pObjectPtr, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07);
	Function  m_Function;
};

template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08>
class IDelegate_Store_08 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_08() { }

	typedef void (*Function) (void* pObjectPtr, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08);
	Function  m_Function;
};

template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, typename Arg09>
class IDelegate_Store_09 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_09() { }

	typedef void (*Function) (void* pObjectPtr, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09);
	Function  m_Function;
};

template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, typename Arg09, typename Arg10>
class IDelegate_Store_10 : public IDelegate_Store_Parent
{
public:
	IDelegate_Store_10() { }

	typedef void (*Function) (void* pObjectPtr, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09, Arg10);
	Function  m_Function;
};

class IDelegate : public IGwRefObject
{
public:
	IDelegate() : m_pObjectPtr(NULL), m_pDelegate_Store(NULL), m_pParameta(NULL)
	{
	}
	IDelegate(void * pParameta) : m_pObjectPtr(NULL), m_pDelegate_Store(NULL), m_pParameta(pParameta)
	{
	}
	~IDelegate() 
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;
	}

	void	*	GetParameta()
	{
		return m_pParameta;
	}

	template <class T, void (T::*TMethod)() >
	void Bind(T* pObjectPtr)
	{
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_00();
		((IDelegate_Store_00*)m_pDelegate_Store)->m_Function = &MethodStub_00<T, TMethod>;
	}
	template <class T, typename Arg01, void (T::*TMethod)(Arg01) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr;
		m_pDelegate_Store = new IDelegate_Store_01<Arg01>();
		((IDelegate_Store_01<Arg01>*)m_pDelegate_Store)->m_Function = &MethodStub_01<T, Arg01, TMethod>;
	}
	template <class T, typename Arg01, typename Arg02, void (T::*TMethod)(Arg01, Arg02) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_02<Arg01, Arg02>();
		((IDelegate_Store_02<Arg01,Arg02>*)m_pDelegate_Store)->m_Function = &MethodStub_02<T, Arg01, Arg02, TMethod>;
	}
	template <class T, typename Arg01, typename Arg02, typename Arg03, void (T::*TMethod)(Arg01, Arg02, Arg03) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_03<Arg01, Arg02, Arg03>();
		((IDelegate_Store_03<Arg01,Arg02,Arg03>*)m_pDelegate_Store)->m_Function = &MethodStub_03<T, Arg01, Arg02, Arg03, TMethod>;
	}
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_04<Arg01, Arg02, Arg03, Arg04>();
		((IDelegate_Store_04<Arg01,Arg02,Arg03, Arg04>*)m_pDelegate_Store)->m_Function = &MethodStub_04<T, Arg01, Arg02, Arg03, Arg04, TMethod>;
	}
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_05<Arg01, Arg02, Arg03, Arg04, Arg05>();
		((IDelegate_Store_05<Arg01,Arg02,Arg03, Arg04, Arg05>*)m_pDelegate_Store)->m_Function = &MethodStub_05<T, Arg01, Arg02, Arg03, Arg04, Arg05, TMethod>;
	}
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_06<Arg01, Arg02, Arg03, Arg04, Arg05, Arg06>();
		((IDelegate_Store_06<Arg01,Arg02,Arg03, Arg04, Arg05, Arg06>*)m_pDelegate_Store)->m_Function = &MethodStub_06<T, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, TMethod>;
	}
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_07<Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07>();
		((IDelegate_Store_07<Arg01,Arg02,Arg03, Arg04, Arg05, Arg06, Arg07>*)m_pDelegate_Store)->m_Function = &MethodStub_07<T, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, TMethod>;
	}
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_08<Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08>();
		((IDelegate_Store_08<Arg01,Arg02,Arg03, Arg04, Arg05, Arg06, Arg07, Arg08>*)m_pDelegate_Store)->m_Function = &MethodStub_08<T, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, TMethod>;
	}

	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, typename Arg09, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_09<Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09>();
		((IDelegate_Store_09<Arg01,Arg02,Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09>*)m_pDelegate_Store)->m_Function = &MethodStub_09<T, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09, TMethod>;
	}

	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, typename Arg09, typename Arg10, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09, Arg10) >
	void Bind(T* pObjectPtr)
	{ 
		if( m_pDelegate_Store != NULL ) delete m_pDelegate_Store;

		m_pObjectPtr = pObjectPtr; 
		m_pDelegate_Store = new IDelegate_Store_10<Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09, Arg10>();
		((IDelegate_Store_10<Arg01,Arg02,Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09, Arg10>*)m_pDelegate_Store)->m_Function = &MethodStub_10<T, Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09, Arg10, TMethod>;
	}

	void operator () () const					
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_00*)m_pDelegate_Store)->m_Function)(m_pObjectPtr); }
	template<typename Arg01>
	void operator () (Arg01 a1) const			
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_01<Arg01>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1); }
	template<typename Arg01, typename Arg02>
	void operator () (Arg01 a1, Arg02 a2) const	
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_02<Arg01,Arg02>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1, a2); }
	template<typename Arg01, typename Arg02, typename Arg03>
	void operator () (Arg01 a1, Arg02 a2, Arg03 a3) const	
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_03<Arg01,Arg02,Arg03>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1, a2, a3); }
	template<typename Arg01, typename Arg02, typename Arg03, typename Arg04>
	void operator () (Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4) const
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_04<Arg01,Arg02,Arg03,Arg04>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1, a2, a3, a4); }
	template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05>
	void operator () (Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5) const	
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_05<Arg01,Arg02,Arg03,Arg04,Arg05>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1, a2, a3, a4, a5); }
	template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06>
	void operator () (Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6) const
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_06<Arg01,Arg02,Arg03,Arg04,Arg05,Arg06>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1, a2, a3, a4, a5, a6); }
	template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07>
	void operator () (Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6, Arg07 a7) const
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_07<Arg01,Arg02,Arg03,Arg04,Arg05,Arg06,Arg07>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1, a2, a3, a4, a5, a6, a7); }
	template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08>
	void operator () (Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6, Arg07 a7, Arg08 a8) const	
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_08<Arg01,Arg02,Arg03,Arg04,Arg05,Arg06,Arg07,Arg08>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1, a2, a3, a4, a5, a6, a7, a8); }
	template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, typename Arg09>
	void operator () (Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6, Arg07 a7, Arg08 a8, Arg09 a9) const	
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_09<Arg01,Arg02,Arg03,Arg04,Arg05,Arg06,Arg07,Arg08,Arg09>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
	template<typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, typename Arg09, typename Arg10>
	void operator () (Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6, Arg07 a7, Arg08 a8, Arg09 a9, Arg10 a10) const	
	{ if(m_pDelegate_Store!= NULL) return (*((IDelegate_Store_10<Arg01,Arg02,Arg03,Arg04,Arg05,Arg06,Arg07,Arg08,Arg09,,Arg10>*)m_pDelegate_Store)->m_Function)(m_pObjectPtr, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }

private:

	template <class T, void (T::*TMethod)(void) >
	static void MethodStub_00 (void* pObjectPtr)						
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(); }
	template <class T, typename Arg01, void (T::*TMethod)(Arg01) >
	static void MethodStub_01 (void* pObjectPtr, Arg01 a1)			
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1); }
	template <class T, typename Arg01, typename Arg02, void (T::*TMethod)(Arg01, Arg02) >
	static void MethodStub_02 (void* pObjectPtr, Arg01 a1, Arg02 a2)	
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1, a2); }
	template <class T, typename Arg01, typename Arg02, typename Arg03, void (T::*TMethod)(Arg01, Arg02, Arg03) >
	static void MethodStub_03 (void* pObjectPtr, Arg01 a1, Arg02 a2, Arg03 a3)
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1, a2, a3); }
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04) >
	static void MethodStub_04 (void* pObjectPtr, Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4)
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1, a2, a3, a4); }
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05) >
	static void MethodStub_05 (void* pObjectPtr, Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5)
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1, a2, a3, a4, a5); }
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06) >
	static void MethodStub_06 (void* pObjectPtr, Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6)
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1, a2, a3, a4, a5, a6); }
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07) >
	static void MethodStub_07 (void* pObjectPtr, Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6, Arg07 a7)
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1, a2, a3, a4, a5, a6, a7); }
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08) >
	static void MethodStub_08 (void* pObjectPtr, Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6, Arg07 a7, Arg08 a8)
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1, a2, a3, a4, a5, a6, a7, a8); }
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, typename Arg09, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09) >
	static void MethodStub_09 (void* pObjectPtr, Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6, Arg07 a7, Arg08 a8, Arg09 a9)
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1, a2, a3, a4, a5, a6, a7, a8, a9); }
	template <class T, typename Arg01, typename Arg02, typename Arg03, typename Arg04, typename Arg05, typename Arg06, typename Arg07, typename Arg08, typename Arg09, typename Arg10, void (T::*TMethod)(Arg01, Arg02, Arg03, Arg04, Arg05, Arg06, Arg07, Arg08, Arg09, Arg10) >
	static void MethodStub_10 (void* pObjectPtr, Arg01 a1, Arg02 a2, Arg03 a3, Arg04 a4, Arg05 a5, Arg06 a6, Arg07 a7, Arg08 a8, Arg09 a9, Arg09 a10)
	{ T* p = static_cast<T*>(pObjectPtr);	return (p->*TMethod)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }

	void					*   m_pObjectPtr;
	IDelegate_Store_Parent	*	m_pDelegate_Store;

	void					*	m_pParameta;
};