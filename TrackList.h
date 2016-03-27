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
//  FILENAME: TrackList.h
//  TYPE    : header file
// 

//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file manages all the tracks according to sic-sac information.
//				
// 
//  COMMENTS
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

#if !defined(AFX_TRACKLIST_H)
#define AFX_TRACKLIST_H

//-INCLUDE FILES---------------------------------------------------------------
#include <stdlib.h>
#include "./CommonFiles/Templ.h"
#include "qdatetime.h"
#include "CommonDataTypes.h"

//-CLASSES DEFINITIONS---------------------------------------------------------



template<class TYPE> 
class CTripleEntryElement;
template<class TYPE> 
class CTripleEntryList;
template<class TYPE> 
class CBaseTripleEntry;
template<class TYPE> 
class CTripleEntryListIterator;

//-----------------------------------------------------------------------------
//
/// \class CBaseTripleEntry
/// This class is a template base class to manage a CTripleEntryElement
// 
//
//-----------------------------------------------------------------------------



template<class TYPE>
// CDS BREAK NGT 2006/06/08 Multiple inheritance with QObject which is not an abstract class.
//       The alternative solution would be to inherit CObject from QObject
//       But the implementation of QT does not allow this design. 
class CBaseTripleEntry : public CObject, public QObject
{
	friend class CTripleEntryList<TYPE>;
	friend class CTripleEntryElement<TYPE>;

public :
	//Constructor
	CBaseTripleEntry();
	//copy constructor
	CBaseTripleEntry(CBaseTripleEntry& TripleEntry);
	//Destructor
	~CBaseTripleEntry();
	//This method returns the track number (key3) associated to the sic and sac 
	//given in attribute
	int GetKey(int key1, int key2);
	//This method sets the key value for the applied element
	bool GetMainKey(int& key1, int& key2, int& key3);
	//This method checks whether the entry is empty or not
	bool IsEmpty();
	//This method returns the elapsed time since the last update of the entry
	int GetMainElapsedRefreshTime(QDateTime time);
	//This virtual method manages the ageing of the elements in the list
	virtual bool OnTimeOut(CTripleEntryList<TYPE>& List) = 0;
	//This virtual method is called to cancel a link between to elements of the list
	virtual bool OnRemoveLink(CTripleEntryList<TYPE>& List) = 0;
	//This virtual method is called to add a new link between two elements of the list
	virtual bool OnCreateLink(CTripleEntryList<TYPE>& List) = 0;
	//This virtual method solves the links conflicts
	virtual void OnResolveLink(CTripleEntryList<TYPE>& List) = 0;

private :
	CTripleEntryElement<TYPE>* m_pTEE;
};



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CBaseTripleEntry<TYPE>::CBaseTripleEntry()
: m_pTEE(NULL)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the copy constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CBaseTripleEntry<TYPE>::CBaseTripleEntry(CBaseTripleEntry& TripleEntry)
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CBaseTripleEntry<TYPE>::~CBaseTripleEntry()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method returns the third key of an entry
/// 
///  RETURNS: the key value
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
int CBaseTripleEntry<TYPE>::GetKey(int key1, int key2)
{
	if (m_pTEE)
		return m_pTEE->GetKey(key1, key2);
	else
		return -1;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template sets the key value for the element
/// 
///  RETURNS: true if the keys are retrieved, else false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CBaseTripleEntry<TYPE>::GetMainKey(int& key1, int& key2, int& key3)
{
	if (m_pTEE)
		return m_pTEE->GetMainKey(key1, key2, key3);
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template returns the elapsed time since the last update of the entry.
/// 
///  RETURNS: elapsed time since the last update of the entry
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
int CBaseTripleEntry<TYPE>::GetMainElapsedRefreshTime(QDateTime time)
{
	if (m_pTEE)
		return m_pTEE->GetMainElapsedRefreshTime(time);
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template checks whether the entry is empty or not.
/// 
///  RETURNS: true if the entry is emtpy, else false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CBaseTripleEntry<TYPE>::IsEmpty()
{
	if (m_pTEE)
		return m_pTEE->IsEmpty();
	else
		return false;
}




//-----------------------------------------------------------------------------
//
/// \class CTripleEntryElement
/// This class is a template base class which manages the elements that are added 
///	to the CTripleEntryList list.
// 
//
//-----------------------------------------------------------------------------


template<class TYPE>
class CTripleEntryElement
{
	friend class CTripleEntryList<TYPE>;
	friend class CBaseTripleEntry<TYPE>;
	friend class QPtrList< CTripleEntryElement<TYPE> >;
	friend class CTripleEntryListIterator<TYPE>;

private :
	// variables
	//Object associated to the list entry
	TYPE* m_pBTE;
	//Keys of the entry
	CKeyList m_Keys;
	//Sic value of the entry element
	int m_MainKey1;
	//Sac value of the entry element
	int m_MainKey2;
	//Whether the sic or sac values have been updated once or not
	bool m_IsMainUpdateOnce;
	//Last update time of main keys
	QDateTime m_LastMainUpdate;

	//methods
	//Constructor
	CTripleEntryElement();
	//Copy constructor
	CTripleEntryElement(CTripleEntryElement& TripleEntry);
	//Destructor
	virtual ~CTripleEntryElement();
	//This method cancels the association of the element with the keys 
	//put in argument
	bool RemoveKey(int key1, int key2, int key3);
	//This template method sets the keys of the element
	bool AddKey(int key1, int key2, int key3);
	//This template method returns true if no key is associated to the element
	bool IsEmpty();
	//This template method returns the third key of the current element
	int GetKey(int key1, int key2);
	//This template method is used to retrieve the keys of the element
	bool GetMainKey(int& key1, int& key2, int& key3);
	//This template method returns the elapsed time since the last update of
	//the element
	int GetElapsedRefreshTime(int key1, int key2, int key3);
	//This template method returns the elapsed time since the last update of
	//the element main keys
	int GetMainElapsedRefreshTime(QDateTime time);
	//This template method returns true if the keys put in argument are the main
	//keys of the element.
	bool IsMain(int key1, int key2);
	//This template method sets the object of the element and its associated
	//main keys.
	bool SetObject(TYPE* pelem, int key1, int key2);
	//This template method frees the element (object and keys values are reseted)
	bool ReleaseObject();	
};



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CTripleEntryElement<TYPE>::CTripleEntryElement()
{
	m_IsMainUpdateOnce = false;
	m_MainKey1 = -1;
	m_MainKey2 = -1;
	m_LastMainUpdate=QDateTime::currentDateTime();
	m_pBTE = NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the copy constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CTripleEntryElement<TYPE>::CTripleEntryElement(CTripleEntryElement& TripleEntry)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CTripleEntryElement<TYPE>::~CTripleEntryElement()
{
	m_MainKey1 = -2;
	m_MainKey2 = -2;
	m_pBTE = NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method returns the third key of the current element
/// 
///  RETURNS: the key value
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
int CTripleEntryElement<TYPE>::GetKey(int key1, int key2)
{
	return m_Keys.GetKey(key1, key2);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method returns true if no key is associated to the element
/// 
///  RETURNS: true if the m_Keys object is associated to no key, else false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryElement<TYPE>::IsEmpty()
{
	return m_Keys.IsEmpty();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method returns the elapsed time since the last update of
///		the element.
/// 
///  RETURNS: elapsed time since the last update of the entry
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
int CTripleEntryElement<TYPE>::GetElapsedRefreshTime(int key1, int key2, int key3)
{
	return m_Keys.GetElapsedRefreshTime(key1, key2, key3);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method cancels association of the element with the keys 
///		put in argument.
/// 
///  RETURNS: true if the removal is successful, else false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryElement<TYPE>::RemoveKey(int key1, int key2, int key3)
{
	return m_Keys.RemoveKey(key1, key2, key3);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method sets the keys of the element.
/// 
///  RETURNS: elapsed time since the last update of the entry
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryElement<TYPE>::AddKey(int key1, int key2, int key3)
{
	int doublon3 = m_Keys.GetKey(key1, key2);
	if (doublon3 == -1 || doublon3 == key3)
	{
		m_Keys.AddKey(key1, key2, key3);
		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is used to retrieve the keys of the element.
/// 
///  RETURNS: true if the keys are found, else false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryElement<TYPE>::GetMainKey(int& key1, int& key2, int& key3)
{
	if (m_MainKey1 != -1 && m_MainKey2 != -1)
	{
		key1 = m_MainKey1;
		key2 = m_MainKey2;
		key3 = GetKey(key1, key2);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method returns true if the keys put in argument are the main
///		keys of the element.
/// 
///  RETURNS: true if the keys are the same, else false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryElement<TYPE>::IsMain(int key1, int key2)
{
	if (m_MainKey1 != -1 && m_MainKey2 != -1 && key1 == m_MainKey1 && key2 == m_MainKey2)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method returns the elapsed time since the last update of
///		the element main keys.
/// 
///  RETURNS: elapsed time since the last update of the element main keys
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
int CTripleEntryElement<TYPE>::GetMainElapsedRefreshTime(QDateTime time)
{
	if (m_IsMainUpdateOnce)
	{
		return abs(QDateTime::currentDateTime().toTime_t()- m_LastMainUpdate.toTime_t());
	}
	return -1;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method sets the object of the element and its associated
///		main keys.
/// 
///  RETURNS: false if the object in argument is not valid, else true
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryElement<TYPE>::SetObject(TYPE* pelem, int key1, int key2)
{
	if (!pelem)
		return false;
	m_pBTE = pelem;
	pelem->m_pTEE = this;
	m_IsMainUpdateOnce = true;
	m_LastMainUpdate=QDateTime::currentDateTime();
	m_MainKey1 = key1;
	m_MainKey2 = key2;
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method frees the element (object and keys values are reseted). 
/// 
///  RETURNS: false if the element is already free, else true
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryElement<TYPE>::ReleaseObject()
{
	if (!m_pBTE)
		return false;
	m_pBTE->m_pTEE = NULL;
	m_pBTE = NULL;
	m_MainKey1 = -1;
	m_MainKey2 = -1;
	m_IsMainUpdateOnce = false;
	return true;
}





//-----------------------------------------------------------------------------
//
/// \class CTripleEntryList
/// This class is a template base class which manages the list of CBaseTripleEntry
///	entries.
// 
//
//-----------------------------------------------------------------------------


#define CARRAY CArray<void*,void*>

template<class TYPE>
class CTripleEntryList : public QObject
{
	friend class CTripleEntryListIterator<TYPE>;

public :
	CTripleEntryList();
	virtual ~CTripleEntryList();
	void SetSize(int size1, int size2, int size3, int TimeOutLink, int FreqLink, int TimeOutMain, int FreqMain);
	void Allow(int key1, int key2);
	bool IsAllowed(int key1, int key2);
	bool SetAt(int key1, int key2, int key3, TYPE* pelem);
	bool RemoveAt(int key1, int key2, int key3);
	bool SetLink(int key11, int key12, int key13, int key21, int key22, int key23);
	bool RemoveLink(int key11, int key12, int key13, int key21, int key22, int key23);
	bool CompleteLink(int key11, int key12, int key13, int key21, int key22, int* key23);
	bool GetAllLink(int key1, int key2, int key3, CKeyList& keys);
	void RemoveAll();
	TYPE* Get(int key1, int key2, int key3);
	int count();

private :
	// variables
	int m_Count;	
	QPtrList< CTripleEntryElement<TYPE> > m_AllElemList;
	int m_TimeOutLink;
	int m_FreqLink;
	int m_TimeOutMain;
	int m_FreqMain;
	CARRAY m_List;
	int m_Size1;
	int m_Size2;
	int m_Size3;
	int m_TimerMain;
	int m_TimerLink;

	//methods
	void timerEvent(QTimerEvent* e);
	void AgeMain();
	void AgeLink();
	
};

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CTripleEntryList<TYPE>::CTripleEntryList()
: m_Size1(0), m_Size2(0), m_Size3(0), m_TimeOutLink(-1), m_TimeOutMain(-1), m_FreqLink(-1), m_FreqMain(-1), m_Count(0), m_TimerMain(-1), m_TimerLink(-1)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CTripleEntryList<TYPE>::~CTripleEntryList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method initializes the list. 
/// 
///  RETURNS: void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
void CTripleEntryList<TYPE>::SetSize(int size1, int size2, int size3, int TimeOutLink, int FreqLink, int TimeOutMain, int FreqMain)
{
	m_TimeOutMain = TimeOutMain;
	m_FreqMain = FreqMain;
	m_TimeOutLink = TimeOutLink;
	m_FreqLink = FreqLink;
	m_List.SetSize(size1);
	for(int t = 0; t < size1; t++)
		m_List[t] = NULL;
	m_Size1 = size1;
	m_Size2 = size2;
	m_Size3 = size3;
	if (m_FreqMain != -1)
		m_TimerMain = startTimer(m_FreqMain);
	if (m_FreqLink != -1 && TimeOutLink != -1)
		m_TimerLink = startTimer(m_FreqLink);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is used to define a new authorized key1/key2 
///		combination. 
/// 
///  RETURNS: void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
void CTripleEntryList<TYPE>::Allow(int key1, int key2)
{
	if (!m_List[key1])
	{
		m_List[key1] = new CARRAY;
		((CARRAY *)m_List[key1])->SetSize(m_Size2);
		for(int t = 0; t < m_Size2; t++)
			(*(CARRAY*)m_List[key1])[t] = NULL;
	}
	if (!(*(CARRAY*)m_List[key1])[key2])
	{
		(*(CARRAY*)m_List[key1])[key2] = new CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>;
		( (CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*) (*(CARRAY*)m_List[key1])[key2] )->SetSize(m_Size3);
		for(int t = 0; t < m_Size3; t++)
			(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key1])[key2])[t] = NULL;
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method checks whether a key1/key2 combination is already 
///		defined or not 
/// 
///  RETURNS: true if already allowed, else false 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryList<TYPE>::IsAllowed(int key1, int key2)
{
	if (m_List[key1] && (*(CARRAY*)m_List[key1])[key2])
		return true;
	else
		return false;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method adds an entry to the list with the caracteristics 
///		put in argument
/// 
///  RETURNS: true if the entry is added, else false 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryList<TYPE>::SetAt(int key1, int key2, int key3, TYPE* pelem)
{
	if (!pelem)
		return false;
	CTripleEntryElement<TYPE>* pTEE = (*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key1])[key2])[key3];
	if (pelem->m_pTEE && pelem->m_pTEE != pTEE)
		return false;
	if (pTEE && pTEE->m_pBTE && pTEE->m_pBTE!=pelem)
		return false;
	if (!pTEE)
	{
		pTEE = new CTripleEntryElement<TYPE>;
		if (pTEE->AddKey(key1, key2, key3))
		{
			if (m_AllElemList.find(pTEE) == -1)
				m_AllElemList.append(pTEE);
			(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key1])[key2])[key3] = pTEE;
		}
		else
		{
			// CDS BREAK NGT 2006/06/08 The rule : "empty brackets must be used for delete 
			//					when de-allocating arrays" cannot be used in this case. 
			delete pTEE;
			pTEE = NULL;
			return false;
		}
	}
	pTEE->SetObject(pelem, key1, key2);
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method removes the entry identified by the keys put in 
///		argument.
/// 
///  RETURNS: true if the entry is removed, else false 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryList<TYPE>::RemoveAt(int key1, int key2, int key3)
{
	CTripleEntryElement<TYPE>* pTEE = (*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key1])[key2])[key3];
	if (!pTEE)
		return false;
	TYPE* pelem=pTEE->m_pBTE;
	if (!pelem)
		return false;
	if (pelem->m_pTEE != pTEE)
		return false;
	pTEE->ReleaseObject();
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method creates a link between two entries of the list.
/// 
///  RETURNS: true if the entries are linked, else false 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryList<TYPE>::SetLink(int key11, int key12, int key13, int key21, int key22, int key23)
{
	if (key11 == key21 && key12 == key22)
		return false;
	CTripleEntryElement<TYPE>* pTEE1 = (*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key11])[key12])[key13];
	CTripleEntryElement<TYPE>* pTEE2 = (*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key21])[key22])[key23];
	if (!pTEE1 && !pTEE2)
	{
		CTripleEntryElement<TYPE>* pTEE = new CTripleEntryElement<TYPE>;
		if (pTEE->AddKey(key11, key12, key13))
			if (pTEE->AddKey(key21, key22, key23))
			{
				if (m_AllElemList.find(pTEE) == -1)
					m_AllElemList.append(pTEE);
				(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key11])[key12])[key13] = pTEE;
				(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key21])[key22])[key23] = pTEE;
				if (pTEE->m_pBTE)
					pTEE->m_pBTE->OnCreateLink(*this);
				return true;
			}
		// CDS BREAK NGT 2006/06/08 The rule : "empty brackets must be used for delete 
		//					when de-allocating arrays" cannot be used in this case. 
		delete pTEE;
		return false;
	}
	else if (pTEE1 && !pTEE2)
	{
		if (pTEE1->AddKey(key21, key22, key23))
		{
			pTEE1->AddKey(key11, key12, key13);
			(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key21])[key22])[key23] = pTEE1;
			if (pTEE1->m_pBTE)
				pTEE1->m_pBTE->OnCreateLink(*this);
			return true;
		}
		else
			return false;
	}
	else if (!pTEE1 && pTEE2)
	{
		if (pTEE2->AddKey(key11, key12, key13))
		{
			pTEE2->AddKey(key21, key22, key23);
			(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key11])[key12])[key13] = pTEE2;
			if (pTEE2->m_pBTE)
				pTEE2->m_pBTE->OnCreateLink(*this);
			return true;
		}
		else
			return false;
	}
	else if (pTEE1 == pTEE2)
	{
		pTEE1->AddKey(key11, key12, key13);
		pTEE2->AddKey(key21, key22, key23);
		return true;
	}
	else
	{
		int elps1 = pTEE1->GetMainElapsedRefreshTime(QDateTime::currentDateTime());
		int elps2 = pTEE2->GetMainElapsedRefreshTime(QDateTime::currentDateTime());
		if (elps1 > elps2)
		{

			if (pTEE1->RemoveKey(key11, key12, key13))
			{
				pTEE1->m_MainKey1 = -1;
				pTEE1->m_MainKey2 = -1;
				(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key11])[key12])[key13] = NULL;
			}
			if (pTEE2->AddKey(key11, key12, key13))
			{
				(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key11])[key12])[key13] = pTEE2;
				if (pTEE2->m_pBTE)
					pTEE2->m_pBTE->OnResolveLink(*this);
				return true;
			}
		}
		else
		{
			if (pTEE2->RemoveKey(key21, key22, key23))
			{
				pTEE2->m_MainKey1 = -1;
				pTEE2->m_MainKey2 = -1;
				(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key21])[key22])[key23] = NULL;
			}
			if (pTEE1->AddKey(key21, key22, key23))
			{
				(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key21])[key22])[key23] = pTEE1;
				if (pTEE1->m_pBTE)
					pTEE1->m_pBTE->OnResolveLink(*this);
				return true;
			}
		}
		return false;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method cancels a link between two entries of the list.
/// 
///  RETURNS: true if the link is canceled, else false 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryList<TYPE>::RemoveLink(int key11, int key12, int key13, int key21, int key22, int key23)
{
	CTripleEntryElement<TYPE>* pTEE1 = (*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key11])[key12])[key13];
	CTripleEntryElement<TYPE>* pTEE2 = (*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key21])[key22])[key23];
	if (pTEE1 && pTEE2)
	{
		if (pTEE1 == pTEE2)
		{
			if (!pTEE1->IsMain(key11, key12))
			{
				if (pTEE1->RemoveKey(key11, key12, key13))
					(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key11])[key12])[key13] = NULL;
				else
					return false;
			}
			if (!pTEE2->IsMain(key21, key22))
			{
				if (pTEE2->RemoveKey(key21, key22, key23))
					(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key21])[key22])[key23] = NULL;
				else
					return false;
			}
			if (pTEE1->IsEmpty())
			{
				if (m_AllElemList.remove(pTEE1))
					delete pTEE1;
			}
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method retrieves the third key (key23) of an entry associated  
///		to another known entry.
/// 
///  RETURNS: true if the key is retrieved, else false 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryList<TYPE>::CompleteLink(int key11, int key12, int key13, int key21, int key22, int* key23)
{
	if (key23)
	{
		CTripleEntryElement<TYPE>* pTEE = (*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key11])[key12])[key13];
		if (pTEE)
		{
			int tmpkey3 = pTEE->GetKey(key21, key22);
			if (tmpkey3 != -1)
			{
				*key23 = tmpkey3;
				return true;
			}
		}
	}
	return false;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method returns the object associated to the list entry 
///		identified by the keys put in argument.
/// 
///  RETURNS: true if the object is retrieved, else false 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
TYPE* CTripleEntryList<TYPE>::Get(int key1, int key2, int key3)
{
	if (key3 == -1)
		return NULL;
	CTripleEntryElement<TYPE>* pTEE = (*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key1])[key2])[key3];
	if (pTEE)
		return pTEE->m_pBTE;
	else
		return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method empties the whole list.
/// 
///  RETURNS: void 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
void CTripleEntryList<TYPE>::RemoveAll()
{
	for(int k1 = 0; k1 < m_Size1; k1++)
	{
		if (m_List[k1])
		{
			for(int k2 = 0; k2 < m_Size2; k2++)
			{
				if ((*(CARRAY*)m_List[k1])[k2])
				{
					for(int k3 = 0; k3 < m_Size3; k3++)
					{
						(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[k1])[k2])[k3] = NULL;
					}
				}
			}
		}
	}

	QPtrListIterator< CTripleEntryElement<TYPE> > it( m_AllElemList );
	CTripleEntryElement<TYPE>* pTEE = it.current();
	while ( pTEE != 0 ) 
	{
		++it;
		if (pTEE->m_pBTE && pTEE->m_pBTE->m_pTEE == pTEE)
			delete pTEE->m_pBTE;
		// CDS BREAK NGT 2006/06/08 The rule : "empty brackets must be used for delete 
		//					when de-allocating arrays" cannot be used in this case. 
		delete pTEE;
		pTEE = it.current();
	}
	m_AllElemList.clear();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method removes from the list the entries for which the 
///		refresh time has expired.
/// 
///  RETURNS: void 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
void CTripleEntryList<TYPE>::AgeMain()
{
	m_Count=0;
	QDateTime curtime = QDateTime::currentDateTime();
	QPtrListIterator< CTripleEntryElement<TYPE> > it( m_AllElemList );
	CTripleEntryElement<TYPE>* pTEE = it.current();
	while ( pTEE != 0 ) 
	{
		++it;
		TYPE* pBTE = pTEE->m_pBTE;
		if (pBTE)
		{
			m_Count++;
			if (m_TimeOutMain != -1 && pTEE->GetMainElapsedRefreshTime(curtime) >= m_TimeOutMain) 
			{
				if (pBTE->OnTimeOut(*this))
				{
					pTEE->ReleaseObject();
					delete pBTE;
					m_Count--;
				}
			}
		}
		pTEE = it.current();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method cancels the links between two object if it has expired.
/// 
///  RETURNS: void 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
void CTripleEntryList<TYPE>::AgeLink()
{
	QPtrListIterator< CTripleEntryElement<TYPE> > it( m_AllElemList );
	CTripleEntryElement<TYPE>* pTEE = it.current();
	while ( pTEE != 0 ) 
	{
		++it;
		CKeyListIterator it2( pTEE->m_Keys );
		int key1=-1, key2=-1, key3=-1;
		while ( it2.current(&key1, &key2, &key3) ) 
		{
			++it2;
			{
				if (!pTEE->IsMain(key1, key2))
				{
					if (pTEE->GetElapsedRefreshTime(key1, key2, key3) >= m_TimeOutLink) 
					{
						if (pTEE->RemoveKey(key1, key2, key3))
						{
							(*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key1])[key2])[key3] = NULL;
							if (pTEE->m_pBTE)
								pTEE->m_pBTE->OnRemoveLink(*this);
						}
					}
				}
			}
		}
		if (pTEE->IsEmpty() && !pTEE->m_pBTE)
		{
			m_AllElemList.remove(pTEE);
			// CDS BREAK NGT 2006/06/08 The rule : "empty brackets must be used for delete 
			//					when de-allocating arrays" cannot be used in this case. 
			delete pTEE;
		}
		pTEE = it.current();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is called each time a started timer of the class 
///		reaches its value.
/// 
///  RETURNS: void 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
void CTripleEntryList<TYPE>::timerEvent(QTimerEvent* e)
{
	//ageing of the entries
	if (e->timerId() == m_TimerMain)
		AgeMain();

	//ageing of the links between entries
	if (e->timerId() == m_TimerLink)
		AgeLink();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method returns the number of entries within the list.
/// 
///  RETURNS: number of entries within the list 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
int CTripleEntryList<TYPE>::count()
{
	return m_Count;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method retrieves all the entries linked with the entry 
///		identified by the keys put in argument.
/// 
///  RETURNS: true if some entries are found, else false 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
bool CTripleEntryList<TYPE>::GetAllLink(int key1, int key2, int key3, CKeyList& keys)
{
	if (key3 == -1)
		return false;
	CTripleEntryElement<TYPE>* pTEE = (*(CArray<CTripleEntryElement<TYPE>*,CTripleEntryElement<TYPE>*>*)(*(CARRAY*)m_List[key1])[key2])[key3];
	if (pTEE)
	{
		keys.RemoveAll();
		keys.Copy(pTEE->m_Keys);
		return true;
	}
	else
		return false;
}


//-----------------------------------------------------------------------------
//
/// \class CTripleEntryListIterator
/// This class is a template base class to manage an iterator to parse the list 
///	of type CTripleEntryList.
// 
//
//-----------------------------------------------------------------------------


template<class TYPE>
class CTripleEntryListIterator
{
private :
	//Definition of the iterator for the type <TYPE>
	QPtrListIterator< CTripleEntryElement<TYPE> > it;
public:
	//Constructor
    CTripleEntryListIterator(const CTripleEntryList<TYPE> &l);
	//Virtual destructor
	virtual ~CTripleEntryListIterator();
	//Redefinition of the current() method (returns the current item in the list)
    TYPE *current()   const;
	//Redefintion of the ++ operator (switches of item in the list)
    TYPE *operator++();
};

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CTripleEntryListIterator<TYPE>::CTripleEntryListIterator(const CTripleEntryList<TYPE> &l)
 : it(l.m_AllElemList) 
{
	if (current() == NULL)
		++(*this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method is the destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
CTripleEntryListIterator<TYPE>::~CTripleEntryListIterator()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template method returns the next object of the list if any.
/// 
///  RETURNS: next object of the list, if any, else NULL 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
TYPE * CTripleEntryListIterator<TYPE>::current()   const
{
	CTripleEntryElement<TYPE>* pTEE = it.current();
	if (pTEE)
		return pTEE->m_pBTE;
	else
		return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This template operator returns the next object of the list if any.
/// 
///  RETURNS: next object of the list, if any, else NULL 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
template<class TYPE>
TYPE * CTripleEntryListIterator<TYPE>::operator++()
{
	CTripleEntryElement<TYPE>* pTEE = NULL;
	do
	{
		++it;
		pTEE = it.current();
	} while (pTEE && !pTEE->m_pBTE);
	if (pTEE)
		return pTEE->m_pBTE;
	else
		return NULL;
}

#endif

//--------------------------------< THE END >----------------------------------


