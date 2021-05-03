#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITypeModule

class ITypeModule 
{
public:
	ITypeModule();
	virtual ~ITypeModule();

	void			AddType(IType * pType);

	int				GetCount();
	IType		*	GetType_byIndex(int nIndex);
private:

	vector<IType*>	m_TypeList;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITypeModule

class ITypeManager : public TGwGetInstance<ITypeManager>
{
public:
	ITypeManager(void);
	~ITypeManager(void);

	void			AddModule(ITypeModule * pModule);
	IType		*	FindType(const char * pszClassName);
	IType		*	FindType(ULONG uHashCode);

private:

	vector<IType*>	m_TypeList;
};

