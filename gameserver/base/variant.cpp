#include "variant.h"
#include <string>
#include <assert.h>

KVariant::KVariant()
{
	m_uType = eVT_STRING;
	m_uIndexType = eVIT_STRING;

	m_Content.Init();
	m_IndexContent.Init();
}

KVariant::~KVariant()
{
	// 清理 tree 子节点
	std::map<const char*,KTreeNode*>::iterator it = m_pChildren.begin();
	for (; it != m_pChildren.end(); ++it)
	{
		if (it->second) {
			delete it->second;
			it->second = NULL;
		}
	}
	m_pChildren.clear();

	// 清理 list 子节点
	KListNode* pNode = m_pContainer;
	while (pNode)
	{
		KListNode* pNext = pNode->GetNext();
		delete pNode;
		pNode = NULL;

		pNode = pNext;
	}

	// 清理自己中的 content
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}
	// 清理 index content
	if (m_uIndexType == eVIT_STRING)
	{
		m_IndexContent.Uninit();
	}
}

KVariant& KVariant::operator[](int nIndex)
{
	KVariant* pNode = (KVariant*)m_pContainer;

	int nPos = 0;
	while (pNode)
	{
		nPos = pNode->m_nIndexNumer;

		if (nPos == nIndex)
		{
			break;
		}
		pNode = (KVariant*)pNode->GetNext();
		nPos++;
	}

	if (pNode == NULL)
	{
		pNode = new KVariant;
		AddTail(pNode);
		pNode->SetIndexNumebr(nIndex);
	}

	return *pNode;
}

KVariant& KVariant::operator[](const char* szIndex)
{
	KVariant* pNode = (KVariant*)m_pChildren[szIndex];
	if (pNode == NULL)
	{
		pNode = new KVariant;
		m_pChildren[szIndex] = pNode;
		
		pNode->m_pParent = this;
		pNode->SetIndexName(szIndex);
	}

	return *pNode;
}

KVariant& KVariant::operator=(int nValue)
{
	SetNumber(nValue);
	return *this;
}

KVariant& KVariant::operator=(char* szValue)
{
	SetString(szValue);
	return *this;
}

KVariant& KVariant::operator=(const char* szValue)
{
	SetString(szValue);
	return *this;
}

KVariant& KVariant::operator=(KVariant& sVariant)
{
	// 先拷贝 content 部分
	switch (sVariant.GetType())
	{
	case eVT_BOOL:
		SetBool(sVariant.GetBool());
		break;
	case eVT_FLOAT:
		SetFloat(sVariant.GetFloat());
		break;
	case eVT_LONG_NUMBER:
		SetLongNumber(sVariant.GetLongNumber());
		break;
	case eVT_NUMBER:
		SetNumber(sVariant.GetNumber());
		break;
	case eVT_STRING:
		SetString(sVariant.GetString());
		break;
	}

	// 不拷贝 index content
	/*if (sVariant.GetIndexType() == eVIT_STRING)
	{
		m_IndexContent.CopyString(sVariant.GetIndexName());
	} else if (sVariant.GetIndexType() == eVIT_NUMBER)
	{
		m_nIndexNumer = sVariant.GetIndexNumber();
	}
	m_uIndexType = sVariant.GetIndexType();*/

	// 然后拷贝 list
	KListNode* pVar = sVariant.GetHeader();
	while (pVar != NULL)
	{
		KVariant* pNewVariant = new KVariant;
		*pNewVariant = *((KVariant*)pVar);
		AddTail(pNewVariant);
		pNewVariant->SetIndexNumebr(((KVariant*)pVar)->GetIndexNumber());

		pVar = pVar->GetNext();
	}

	// 最后是 tree
	//std::map<const char*, KTreeNode*> vChildren = sVariant.GetTree();
	std::map<const char*,KTreeNode*>::iterator it = sVariant.m_pChildren.begin();
	for (; it != sVariant.m_pChildren.end(); ++it)
	{
		if (it->second) {
			KVariant* pVar = (KVariant*) it->second;
			KVariant* pNewVariant = new KVariant;
			*pNewVariant = *pVar;

			pNewVariant->m_pParent = this;
			pNewVariant->SetIndexName(pVar->GetIndexName());

			m_pChildren[pNewVariant->GetIndexName()] = pNewVariant;
		}
	}

	return *this;
}

/************************************************************************/
/* Set & Get                                                            */
/************************************************************************/
void KVariant::SetNumber(int nNumber)
{
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_NUMBER;
	m_nNumber = nNumber;
}

int KVariant::GetNumber()
{
	if (m_uType == eVT_NUMBER)
	{
		return m_nNumber;
	}

	return 0;
}

void KVariant::SetLongNumber(long long number)
{
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_LONG_NUMBER;
	m_llNumber = number;
}


long long KVariant::GetLongNumber()
{
	if (m_uType == eVT_LONG_NUMBER)
	{
		return m_llNumber;
	}

	return 0;
}

void KVariant::SetFloat(float fNumber)
{
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_FLOAT;
	m_fNumber = fNumber;
}

float KVariant::GetFloat()
{
	if (m_uType == eVT_FLOAT)
	{
		return m_fNumber;
	}

	return 0;
}

void KVariant::SetBool(bool bValue)
{
	if (m_uType == eVT_BOOL)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_BOOL;
	m_bValue = bValue;
}

bool KVariant::GetBool()
{
	if (m_uType == eVT_BOOL)
	{
		return m_bValue;
	}

	return false;
}

void KVariant::SetString(const char* szContent, int nSize)
{
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_STRING;
	if (szContent)
	{
		m_Content.CopyString(szContent, nSize);
	}
}

char* KVariant::GetString()
{
	if (m_uType == eVT_STRING)
	{
		return m_Content.GetBuffer();
	}

	return NULL;
}

int KVariant::GetIndexNumber()
{
	assert(GetIndexType() == eVIT_NUMBER); 
	return m_nIndexNumer; 
}

void KVariant::SetIndexNumebr(int nIndex)
{
	if (GetIndexType() == eVIT_STRING)
	{
		m_IndexContent.Uninit();
	}

	m_nIndexNumer = nIndex;
	m_uIndexType = eVIT_NUMBER;
}

const char *KVariant::GetIndexName()
{ 
	assert(GetIndexType() == eVIT_STRING);
	return m_IndexContent.GetBuffer(); 
}

void KVariant::SetIndexName(const char* szName)
{
	if (GetIndexType() == eVIT_STRING)
	{
		m_IndexContent.Uninit();
	}

	m_IndexContent.CopyString(szName);
	m_uIndexType = eVIT_STRING;
}

/************************************************************************/
/* Add                                                                     */
/************************************************************************/
void KVariant::AddNumber(int nNumber)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetNumber(nNumber);

	AddTail(pNewVariant);
}

void KVariant::AddLongNumber(long long number)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetLongNumber(number);

	AddTail(pNewVariant);
}

void KVariant::AddFloat(float fValue)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetFloat(fValue);

	AddTail(pNewVariant);
}

void KVariant::AddBool(bool bValue)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetBool(bValue);

	AddTail(pNewVariant);	
}

void KVariant::AddString(const char* szContent, int nSize /* = 0 */)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetString(szContent, nSize);

	AddTail(pNewVariant);
}

/************************************************************************/
/* to                                                                     */
/************************************************************************/
int KVariant::ToNumber()
{
	if (m_uType == eVT_NUMBER)
	{
		return m_nNumber;
	}

	assert(0);
	return 1;
}

long long KVariant::ToLongNumber()
{
	assert(0);
	return 1;
}

float KVariant::ToFloat()
{
	assert(0);
	return 0.0;
}

bool KVariant::ToBool()
{
	assert(0);
	return false;
}

const char* KVariant::ToString()
{
	return GetString();
}

void KVariant::ShowVariant(int nState)
{
	// index content
	char szTempInfo[256];
	memset(szTempInfo, 0, 256);
	if (nState)
	{
		memset(szTempInfo, '\t', nState);
	}
	printf("\n%s %s\n", szTempInfo, "begin variant ");
	switch (GetIndexType())
	{
	case eVIT_NUMBER:
		printf("%s index number: %d\n", szTempInfo, GetIndexNumber());
		break;
	case eVIT_STRING:
		printf("%s index name: %s\n", szTempInfo, GetIndexName());
		break;
	}

	// content 部分
	switch (GetType())
	{
	case eVT_BOOL:
		printf("%s bool: %b\n", szTempInfo, GetBool());
		break;
	case eVT_FLOAT:
		printf("%s float: %f\n", szTempInfo, GetFloat());
		break;
	case eVT_LONG_NUMBER:
		printf("%s long: %l\n", szTempInfo, GetLongNumber());
		break;
	case eVT_NUMBER:
		printf("%s number: %d\n", szTempInfo, GetNumber());
		break;
	case eVT_STRING:
		if (m_Content.GetBuffer())
		{
			printf("%s string: %s\n", szTempInfo, m_Content.GetBuffer());
		}
		break;
	}

	// list
	KListNode* pVar = GetHeader();
	while (pVar != NULL)
	{
		((KVariant*)pVar)->ShowVariant(nState + 1);

		pVar = pVar->GetNext();
	}

	// tree
	std::map<const char*,KTreeNode*>::iterator it = m_pChildren.begin();
	for (; it != m_pChildren.end(); ++it)
	{
		if (it->second) {
			KVariant* pVar = (KVariant*) it->second;
			pVar->ShowVariant(nState + 1);
		}
	}

	printf("%s %s\n", szTempInfo, "end variant ");
	return;
}