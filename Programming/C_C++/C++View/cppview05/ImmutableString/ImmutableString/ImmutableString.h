// ImmutableString.h: interface for the ImmutableString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMMUTABLESTRING_H__7996CE09_2B64_4BBF_A688_F5D69DC1A11A__INCLUDED_)
#define AFX_IMMUTABLESTRING_H__7996CE09_2B64_4BBF_A688_F5D69DC1A11A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <exception>
#include <string.h>
#include <iostream>

using namespace std;

class ImmutableString  
{
	class StringIndexOutOfBoundsException
		:public exception
	{
		
	public:
		StringIndexOutOfBoundsException(int index);
	private:
		int m_iIndex;
	};

	class StringProxy
	{
	private:
		char * value;	// 用来指向真正的字符串
		int offset;		// 在value基础上的偏移量，字符串的逻辑起点是value+offset
		int count;		// 字符串的可用字符数

	friend class ImmutableString;
	public:

		StringProxy(char* str)
			: m_iRef(0)
		{
			value = strdup(str);

			count = strlen(str);
			offset = 0;
		}

		~StringProxy()
		{
			free(value);
		}
	private:
		int m_iRef;

		friend ostream & operator<<(ostream & os, const StringProxy & str)
		{
		char * pc = str.value+str.offset;

		for(int i=0; i<str.count; i++)
			os<<*(pc+i);

		return os;	
		};
	};

public:
	char charAt(int index);
	inline int length();
	ImmutableString(char * str);
	ImmutableString(ImmutableString & str);

private:
	ImmutableString();
public:
	virtual ~ImmutableString();

public:
	bool startsWith(const ImmutableString & prefix, int offset=0);
	bool endsWith(const ImmutableString & suffix);
	int indexOf(char ch, int fromIndex=0);
	int lastIndexOf(char ch, int fromIndex=0);
	ImmutableString subString(int beginIndex, int endIndex);
	ImmutableString subString(int beginIndex);
	ImmutableString & operator=(const ImmutableString & str);
private:
	bool isNewString;
	StringProxy * m_pProxy;
private:
	int AddRef(void);
private:
	int Release(void);
public:
	bool operator==(const ImmutableString & str);
	bool operator!=(const ImmutableString & str);

friend ostream & operator<<(ostream & os, const ImmutableString & str);
};

#endif // !defined(AFX_IMMUTABLESTRING_H__7996CE09_2B64_4BBF_A688_F5D69DC1A11A__INCLUDED_)
