// ImmutableString.cpp: implementation of the ImmutableString class.
//
//////////////////////////////////////////////////////////////////////

#include "ImmutableString.h"

#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImmutableString::ImmutableString()
{
	
}

ImmutableString::~ImmutableString()
{
	Release();
}

ImmutableString::ImmutableString(char *str)
{
	if(str==NULL || strlen(str)<1)
		throw StringIndexOutOfBoundsException(-1);

	m_pProxy = new StringProxy(str);
	AddRef();
}

inline int ImmutableString::length()
{
	return m_pProxy->count;
}

ImmutableString::StringIndexOutOfBoundsException::StringIndexOutOfBoundsException(int index)
{
	m_iIndex = index;
}

char ImmutableString::charAt(int index)
{
	if ((index < 0) || (index >= m_pProxy->count)) {
	    throw StringIndexOutOfBoundsException(index);
	}
	return m_pProxy->value[index + m_pProxy->offset];
}

bool ImmutableString::startsWith(const ImmutableString & prefix, int toffset)
{
	char * ta = m_pProxy->value;
	int to = m_pProxy->offset + toffset;
	int tlim = m_pProxy->offset + m_pProxy->count;
	char * pa = prefix.m_pProxy->value;
	int po = prefix.m_pProxy->offset;
	int pc = prefix.m_pProxy->count;
	// Note: toffset might be near -1>>>1.
	if ((toffset < 0) || (toffset > m_pProxy->count - pc)) {
	    return false;
	}
	while (--pc >= 0) {
	    if (ta[to++] != pa[po++]) {
	        return false;
	    }
	}
	return true;
}

bool ImmutableString::endsWith(const ImmutableString & suffix)
{
	return startsWith(suffix, m_pProxy->count - suffix.m_pProxy->count);
}

int ImmutableString::indexOf(char ch, int fromIndex)
{
	int max = m_pProxy->offset + m_pProxy->count;
	char * v = m_pProxy->value;

	if (fromIndex < 0) {
	    fromIndex = 0;
	} else if (fromIndex >= m_pProxy->count) {
	    // Note: fromIndex might be near -1>>>1.
	    return -1;
	}
	for (int i = m_pProxy->offset + fromIndex ; i < max ; i++) {
	    if (v[i] == ch) {
		return i - m_pProxy->offset;
	    }
	}
	return -1;
}

int ImmutableString::lastIndexOf(char ch, int fromIndex)
{
	int min = m_pProxy->offset;
	char *v = m_pProxy->value;
	
	for (int i = m_pProxy->offset + ((fromIndex >= m_pProxy->count) ? m_pProxy->count - 1 : fromIndex) ; i >= min ; i--) {
	    if (v[i] == ch) {
		return i - m_pProxy->offset;
	    }
	}
	return -1;
}

ImmutableString ImmutableString::subString(int beginIndex, int endIndex)
{
	if (beginIndex < 0) {
	    throw new StringIndexOutOfBoundsException(beginIndex);
	} 
	if (endIndex > m_pProxy->count) {
	    throw new StringIndexOutOfBoundsException(endIndex);
	}
	if (beginIndex > endIndex) {
	    throw new StringIndexOutOfBoundsException(endIndex - beginIndex);
	}

	ImmutableString str;
	str.m_pProxy = m_pProxy;
	AddRef();

	str.m_pProxy->offset=beginIndex;
	str.m_pProxy->count=endIndex-beginIndex;

	return str;
}


ImmutableString ImmutableString::subString(int beginIndex)
{
	return subString(beginIndex, m_pProxy->count);
}

ImmutableString & ImmutableString::operator=(const ImmutableString & str)
{
	if(*this==str)
		return *this;

	Release();
	m_pProxy = str.m_pProxy;
	const_cast<ImmutableString &>(str).AddRef();

	return *this;
}

int ImmutableString::AddRef(void)
{
	return ++(m_pProxy->m_iRef);
}

int ImmutableString::Release(void)
{
	if(--(m_pProxy->m_iRef)==0){
		delete m_pProxy;
		return 0;
	}

	return m_pProxy->m_iRef;
}

ImmutableString::ImmutableString(ImmutableString & str)
{
	ImmutableString();
	m_pProxy = str.m_pProxy;
	AddRef();
}

bool ImmutableString::operator==(const ImmutableString & str)
{
	if(str.m_pProxy==m_pProxy)
		return true;

	if(strncmp(str.m_pProxy->value+str.m_pProxy->offset, m_pProxy->value+m_pProxy->offset, m_pProxy->count)==0)
		return true;

	return false;
}

bool ImmutableString::operator!=(const ImmutableString & str)
{
	return !(operator==(str));
}

ostream & operator<<(ostream & os, const ImmutableString & str)
{
	os<<*(str.m_pProxy);

	return os;
}