/*
 * Copyright (C) 2009 SkySoft-ATM 
 *		ROUTE DE PRE-BOIS 15-17
 *		CH-1215 GENEVA
 *		SWITZERLAND
 * 		tel int 00 41 22 417 4770
 *  
 * 		for any Albatross related question : <info@albatross.aero>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * The Court of competent jurisdiction is Geneva Switzerland 
*/
/*
 * Copyright (C) 2009 SkySoft-ATM 
 *		ROUTE DE PRE-BOIS 15-17
 *		CH-1215 GENEVA
 *		SWITZERLAND
 * 		tel int 00 41 22 417 4770
 *  
 * 		for any Albatross related question : <info@albatross.aero>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * The Court of competent jurisdiction is Geneva Switzerland 
*/
//-----------------------------------------------------------------------------
// 
//  SYSTEM  : Albatross
//  PROCESS : Display
//  FILENAME: Templ.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CArray with all functions necessary to work with
//		arrays.
//				
// 
//  COMMENTS
//		All the functions are templates in order to be used with every type of 
//		data, structure or object.  
// 
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 1  2009/08/31 dalleins
//
//
// 
//-----------------------------------------------------------------------------

#if !defined(AFX_TEMPLATE__INCLUDED_)
#define AFX_TEMPLATE__INCLUDED_

//-INCLUDE FILES---------------------------------------------------------------

//-GLOBAL DATA-----------------------------------------------------------------

typedef unsigned char       BYTE;

#include <string.h>

#ifdef DEBUG
#else   // _DEBUG
#endif // !_DEBUG

#ifndef _INC_NEW
	#include <new>
#endif

//-CLASSES DEFINITIONS---------------------------------------------------------

// CDS BREAK JPM 16.05.2006 only templates are defined in this file and 
//			    methods are implemented in this header file because
//			    of C++ compiler limitation which does not allow
//			    implementation of templates methods in cpp files
//			    
//-----------------------------------------------------------------------------
//
/// \class template
/// This class is used by all ASD classes
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the template of constructor
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
void  ConstructElements(TYPE* pElements, int nCount)
{
	// first do bit-wise zero initialization
	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	// then call the constructor(s)
	while (nCount)
	{
		nCount--;
		::new((void*)pElements) TYPE;
		pElements++;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the template of destructor
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
void  DestructElements(TYPE* pElements, int nCount)
{
	// call the destructor(s)
	while (nCount)
	{
		nCount--;
		pElements->~TYPE();
		pElements++;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is a template that copy sources of type TYPE (pSrc) into pDest
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
void  CopyElements(TYPE* pDest, const TYPE* pSrc, int nCount)
{
	while (nCount)
	{
		nCount--;
		*pDest++ = *pSrc++;
	}
}

//-----------------------------------------------------------------------------
//
/// \class CArray
/// This class is a template used by all classes using array
///	It contains all the operations performed on a table
// 
//
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
class CArray
{
public:
	
	//methods
	CArray();
	CArray(CArray& Array);  
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);
	void FreeExtra();
	void RemoveAll();
	TYPE GetAt(int nIndex) const;
	void SetAt(int nIndex, ARG_TYPE newElement);
	TYPE& ElementAt(int nIndex);
	const TYPE* GetData() const;
	TYPE* GetData();
	void SetAtGrow(int nIndex, ARG_TYPE newElement);
	int Add(ARG_TYPE newElement);
	int Append(const CArray& src);
	void Copy(const CArray& src);
	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);
	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CArray* pNewArray);

	//virtual methods
	virtual ~CArray();

protected:

	//variables	
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

	//pointers
	TYPE* m_pData;   // the actual array of data

};

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::CArray(CArray& Array)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the size of the table belonging to the class TYPE
/// 
///  RETURNS: 
///		the size of a table
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 int CArray<TYPE, ARG_TYPE>::GetSize() const
	{ 
		return m_nSize;
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the upper bound of the size of the table belonging 
///		to the class TYPE
/// 
///  RETURNS: 
///		the upper size of a table
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 int CArray<TYPE, ARG_TYPE>::GetUpperBound() const
	{ 
		return m_nSize-1; 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the size of the table belonging to the class TYPE
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 void CArray<TYPE, ARG_TYPE>::RemoveAll()
	{ 
		SetSize(0, -1); 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the data from a table at a given index
/// 
///  RETURNS: 
///		the type of the table data
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 TYPE CArray<TYPE, ARG_TYPE>::GetAt(int nIndex) const
	{ 
//		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the data in a table at a given index
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 void CArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
	{ 
//		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the data from a table at a given index
/// 
///  RETURNS: 
///		the type of the table data
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 TYPE& CArray<TYPE, ARG_TYPE>::ElementAt(int nIndex)
	{ 
//		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves a constant data from a table
/// 
///  RETURNS: 
///		the type of the table data
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 const TYPE* CArray<TYPE, ARG_TYPE>::GetData() const
	{ 
		return (const TYPE*)m_pData; 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves a data from a table
/// 
///  RETURNS: 
///		the type of the table data
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 TYPE* CArray<TYPE, ARG_TYPE>::GetData()
	{ 
		return (TYPE*)m_pData; 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method adds an element at the end of a table
/// 
///  RETURNS: 
///		the new size of a table
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 int CArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
	{ 
		int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the data from a table at a given index
/// 
///  RETURNS: 
///		the new size of a table
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 TYPE CArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
	{ 
		return GetAt(nIndex); 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the data from a table at a given index
/// 
///  RETURNS: 
///		the new size of a table
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
 TYPE& CArray<TYPE, ARG_TYPE>::operator[](int nIndex)
	{ 
		return ElementAt(nIndex); 
	}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is a constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::CArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is a destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::~CArray()
{
	if (m_pData != NULL)
	{
		DestructElements<TYPE>(m_pData, m_nSize);
		delete[] (BYTE*)m_pData;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the size of a table
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy)
{
//	ASSERT_VALID(this);
//	ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData != NULL)
		{
			DestructElements<TYPE>(m_pData, m_nSize);
			delete[] (BYTE*)m_pData;
			m_pData = NULL;
		}
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
//		ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
		m_pData = (TYPE*) new BYTE[nNewSize * sizeof(TYPE)];
		ConstructElements<TYPE>(m_pData, nNewSize);
		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			ConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			DestructElements<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);
		}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize / 8;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

//		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
//		ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = (TYPE*) new BYTE[nNewMax * sizeof(TYPE)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

		// construct remaining elements
//		ASSERT(nNewSize > m_nSize);
		ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method merges 2 tables in one
/// 
///  RETURNS: 
///		the old size of a table before calling the Append method
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::Append(const CArray& src)
{
//	ASSERT(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);
	CopyElements<TYPE>(m_pData + nOldSize, src.m_pData, src.m_nSize);
	return nOldSize;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method copies the data contained in src array into the 
///		CArray<TYPE, ARG_TYPE>
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::Copy(const CArray& src)
{
//	ASSERT(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);
	CopyElements<TYPE>(m_pData, src.m_pData, src.m_nSize);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method shrinks to the desired size and deletes the extra elements 
///		on an array
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
//		ASSERT(m_nSize <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (TYPE*) new BYTE[m_nSize * sizeof(TYPE)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
		}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method adds a new element on an array at the nIndex position
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement)
{
//	ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1, -1);
	m_pData[nIndex] = newElement;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method inserts a new element on an array at the nIndex position
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount /*=1*/)
{
//	ASSERT(nIndex >= 0);    // will expand to meet need
//	ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount, -1);  // grow it to new size
		// destroy intial data before copying over it
		DestructElements<TYPE>(&m_pData[nOldSize], nCount);
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(TYPE));

		// re-init slots we copied from
		ConstructElements<TYPE>(&m_pData[nIndex], nCount);
	}

	// insert new value in the gap
//	ASSERT(nIndex + nCount <= m_nSize);
	while (nCount)
	{
		nCount--;
		m_pData[nIndex++] = newElement;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method removes an element on an array at the nIndex position
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
//	ASSERT(nIndex >= 0);
//	ASSERT(nCount >= 0);
//	ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
	DestructElements<TYPE>(&m_pData[nIndex], nCount);
	if (nMoveCount)
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(TYPE));
	m_nSize -= nCount;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method inserts an element on an array at the nStartIndex position
///		and calls InsertAt method
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, CArray* pNewArray)
{

//	ASSERT(pNewArray != NULL);
//	ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

#endif

//--------------------------------< THE END >----------------------------------





