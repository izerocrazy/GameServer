/********************************************************************
	Copyright (c) 2014-2014 uloveto.com

	http://www.uloveto.com
	
	created:	2014/09/05
	created:	5:9:2014   10:43
	filename: 	variant
	file ext:	h
	author:		zero
	
	purpose:	
*********************************************************************/

#ifndef __variant_h__
#define __variant_h__

#include <stddef.h>
#include <map>
#include "tinyxml2.h"

using namespace tinyxml2;

class KSimpleBuffer 
{
public:
	void Init()
	{
		m_nBufferSize = 0;
		m_pBuffer = NULL;
	};

	unsigned int InitBufferSize(int nBufferSize)
	{
		m_nBufferSize = nBufferSize;
		m_pBuffer = new char[nBufferSize];

		return 1;
	};

	void CopyString(const char* szString, int nStrLen = 0)
	{
		Uninit();

		if (nStrLen <= 0)
		{
			nStrLen = (int)strlen(szString);
		}
		
		int nBufferSize = nStrLen + 1;
		if (nBufferSize > 0 && InitBufferSize(nBufferSize)) // 未做内存判断
		{
			memcpy(m_pBuffer, szString, nBufferSize);
			m_pBuffer[nStrLen] = 0;	
			// 此处曾有一个 bug，nStrlen 写成 nBufferSize，越界。所以在 delete[] 的时候报错，错误为 CRT detected that the application wrote to memory after end of heap buffer
		}
	}

	char* GetBuffer()
	{
		return m_pBuffer;
	};

	void Uninit()
	{
		if (m_pBuffer)
		{
			delete []m_pBuffer;

			m_pBuffer = NULL;
			m_nBufferSize = 0;
		}
	};

private:
	char*			m_pBuffer;
	size_t			m_nBufferSize;
};

class KListNode
{
public:
	KListNode()
	{
		m_nSum = 0;

		m_pNext = NULL;
		m_pPrev = NULL;

		m_pContainer = NULL;
	};

	virtual ~KListNode()
	{};

	KListNode* GetNext()
	{
		return m_pNext;
	}

	int GetSum()
	{
		return m_nSum;
	}

protected:
	void AddTail(KListNode* pTailNode)
	{
		KListNode* pNode = m_pContainer;
		KListNode* pPrev = NULL;

		while (pNode != NULL)
		{
			pPrev = pNode;
			pNode = pNode->m_pNext;
		}

		pNode = pTailNode;
		pNode->m_pPrev = pPrev;
		pNode->m_pNext = NULL;

		if (pPrev)
		{
			pPrev->m_pNext = pNode;
		}

		if (m_pContainer == NULL)
		{
			m_pContainer = pNode;
		}

		m_nSum = m_nSum + 1;
	}


	KListNode* GetHeader()
	{
		return m_pContainer;
	}

	int m_nSum;
	KListNode* m_pNext;
	KListNode* m_pPrev;

	KListNode* m_pContainer;
};

class KTreeNode
{
public:
	struct CompareConstString
	{
	public:
		bool operator()(const char* pA, const char* pB)
		{
			return strcmp(pA, pB) < 0;
		}
	};

	typedef std::map<const char*, KTreeNode*, CompareConstString> VARIANT_MAP;

	KTreeNode()
	{
		m_pChildren.clear();
	}

	virtual ~KTreeNode()
	{};

protected:
	VARIANT_MAP GetTree() { return m_pChildren; }
	KTreeNode* m_pParent;
	VARIANT_MAP m_pChildren;
};

class KVariant : public KListNode, public KTreeNode
{
public:
	enum eVARIANT_TYPE
	{
		eVT_NUMBER,
		eVT_STRING,
		eVT_LONG_NUMBER,
		eVT_FLOAT,
		eVT_BOOL,
	};

	enum eSUB_TYPE
	{
		eST_VALUE = 0x01,
		eST_TREE = 0x02,
		eST_LIST = 0x04,
		eST_ALL = 0x07,
	};

	enum eVARIANT_INDEX_TYPE
	{
		eVIT_NUMBER,
		eVIT_STRING,
	};

	KVariant();
	virtual ~KVariant();
	KVariant& operator[](int nIndex);
	KVariant& operator[](const char* szIndex);
	KVariant& operator=(KVariant& );
	KVariant& operator=(int nValue);
	KVariant& operator=(char* szValue);
	KVariant& operator=(const char* szValue);


	/************************************************************************/
	/* Set & Get                                                            */
	/************************************************************************/
	void SetNumber(int number);
	int GetNumber();

	void SetLongNumber(long long number);
	long long GetLongNumber();

	void SetFloat(float fNumber);
	float GetFloat();

	void SetBool(bool bValue);
	bool GetBool();

	void SetString(const char* szContent, int nSize = 0);
	char* GetString();

	int GetIndexNumber();
	void SetIndexNumebr(int nIndex);
	const char* GetIndexName();
	void SetIndexName(const char* szName);

	/************************************************************************/
	/* add                                                                     */
	/************************************************************************/
	void AddNumber(int number);
	void AddLongNumber(long long number);
	void AddBool(bool bValue);
	void AddFloat(float fValue);
	void AddString(const char* szContent, int nSize = 0);

	/************************************************************************/
	/* to                                                                     */
	/************************************************************************/
	int ToNumber();
	long long ToLongNumber();
	float ToFloat();
	bool ToBool();
	const char* ToString();

	/************************************************************************/
	/* load                                                                     */
	/************************************************************************/
	bool LoadFromXmlString(const char* szXml, int nLen);
	bool LoadFromXml(KVariant& variant, const XMLNode* node);

	unsigned short GetType() { return m_uType; }
	unsigned short GetIndexType() { return m_uIndexType; }
	void ShowVariant(int nState = 0);

protected:
private:
	// Content
	union 
	{
		int				m_nNumber;
		long long		m_llNumber;		// 为什么是 long long，而不是 long
		float			m_fNumber;		
		bool			m_bValue;
		KSimpleBuffer	m_Content;
	};

	// Index Conten
	union
	{
		int				m_nIndexNumer;
		KSimpleBuffer	m_IndexContent;
	};

	unsigned short				m_uType;
	unsigned short				m_uIndexType;
	//unsigned short				m_uStatus;

}; 

#endif // __variant_h__
