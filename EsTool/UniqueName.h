#pragma once

// 유일한 이름을 얻기위한 인터페이스
class IUniqueNameable
{
public:
	virtual	const char*	GetUniqueName() = 0;
};


// 이 객체는 각각 CUniqueName 클래스들에 이름이 유일함
// 중요 :
// 단, 소멸 후에는 다른 객체가 같은 이름을 사용할 수 있기 때문에
// 소멸 전에 이름을 등록해 사용했던 것을 모두 해지 해야함.
class CUniqueName : public IUniqueNameable
{
public:
	virtual	const char*	GetUniqueName()	
	{
		assert(state != STATE_DISPOSED);
		state = STATE_USING;		
		return strUniqueName.c_str(); 
	}

	// 이 함수의 목적은 사용자가 실수로 소멸전 등록해지하는 것을 
	// 잊는 걸 방지하는데 있다.
	// 등록 해지는 완전히 사용자에 몫으로 코드상에 사용했던 곳을
	// 검색하여 놓친 곳이 없는지 확인하자.
	void	DeclareDisposeThisUniqueName()
	{
		state = STATE_DISPOSED;
	}

	CUniqueName(const char* strBaseName = GwNullString)
	{
		++ms_iUniqueCount;

		state = STATE_INIT;
		char strTemp[STRING_MAX] = "";
		// 유니크 카운트와 주소와 베이스네임 조합이면 충분해 보임.
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