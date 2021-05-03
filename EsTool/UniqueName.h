#pragma once

// ������ �̸��� ������� �������̽�
class IUniqueNameable
{
public:
	virtual	const char*	GetUniqueName() = 0;
};


// �� ��ü�� ���� CUniqueName Ŭ�����鿡 �̸��� ������
// �߿� :
// ��, �Ҹ� �Ŀ��� �ٸ� ��ü�� ���� �̸��� ����� �� �ֱ� ������
// �Ҹ� ���� �̸��� ����� ����ߴ� ���� ��� ���� �ؾ���.
class CUniqueName : public IUniqueNameable
{
public:
	virtual	const char*	GetUniqueName()	
	{
		assert(state != STATE_DISPOSED);
		state = STATE_USING;		
		return strUniqueName.c_str(); 
	}

	// �� �Լ��� ������ ����ڰ� �Ǽ��� �Ҹ��� ��������ϴ� ���� 
	// �ش� �� �����ϴµ� �ִ�.
	// ��� ������ ������ ����ڿ� ������ �ڵ�� ����ߴ� ����
	// �˻��Ͽ� ��ģ ���� ������ Ȯ������.
	void	DeclareDisposeThisUniqueName()
	{
		state = STATE_DISPOSED;
	}

	CUniqueName(const char* strBaseName = GwNullString)
	{
		++ms_iUniqueCount;

		state = STATE_INIT;
		char strTemp[STRING_MAX] = "";
		// ����ũ ī��Ʈ�� �ּҿ� ���̽����� �����̸� ����� ����.
		StringCchPrintfA(strTemp, STRING_MAX, "[%d][%0xd][%s]", ms_iUniqueCount, this, strBaseName);
		strUniqueName = strTemp;
	}

	~CUniqueName()
	{
		assert(state != STATE_USING);
	}

private:
	string	strUniqueName;

	enum State
	{
		STATE_INIT,
		STATE_USING,
		STATE_DISPOSED,
	};
	State	state;

	static	UINT	ms_iUniqueCount;
};