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
//  FILENAME: CommonDataTypes.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This class is used to represent a track identifier as SIC, SAC, TRACK NUMBER.
//	  It also manages alert acknowledgement and classification.
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
// 
//-----------------------------------------------------------------------------


//-INCLUDE FILES---------------------------------------------------------------
#include <stdlib.h>
#include "CommonDataTypes.h"


//-DATA------------------------------------------------------------------------

LPCSTR  TrkColorNames[]={"SELECTED_FRGRND","SELECTED_BKGRND","RW_TOOLS","ASSUMED","COUPLED","UNCOUPLED","URGENCY","WARNING","HOLD","INFORMATION","TAG","ROCD_FTNM","MODESTAG"};
    
                   
//-PROTOTYPES------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CKey
/// This class is used to represent a track identifier as SIC, SAC, TRACK NUMBER.
/// It also manages acknowledgement and classification.
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Constructor.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CKey::CKey(int key1, int key2, int key3, bool Ack, int order)
: m_Key1(key1), m_Key2(key2), m_Key3(key3), m_Ack(Ack), m_Mark(true), m_Order(order)
{
	UpdateTime();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		set the last update time and update marker of the instance to the current time
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CKey::UpdateTime()
{
	m_LastUpdate = QDateTime ::currentDateTime();
	m_Mark = true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		destructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CKey::~CKey()
{
	m_Key1 = -1;
	m_Key2 = -1;
	m_Key3 = -1;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		returns the elapsed time between the current time and the last update time.
/// 
///  RETURNS: elapsed time in seconds.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CKey::GetElapsedRefreshTime()
{
	return abs(QDateTime::currentDateTime().toTime_t()- m_LastUpdate.toTime_t());
}


//-----------------------------------------------------------------------------
//
/// \class CKeyList
/// This class is used to store a list of CKey object.
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Constructor.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CKeyList::CKeyList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Destructor.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CKeyList::~CKeyList()
{
	RemoveAll();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		returns the number of objects stored
/// 
///  RETURNS: the number of object stored
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CKeyList::GetCount()
{
	return m_Keys.count();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		deletes all the objects stored
/// 
///  RETURNS: delete all the objects stored
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CKeyList::RemoveAll()
{
    QPtrListIterator<CKey> it( m_Keys );
	CKey* pk = it.current();
    
	while ( pk != 0 ) 
	{
        ++it;
		delete pk;
		pk = it.current();
	}
	
	m_Keys.clear();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		finds a CKey object that matches with 
///      - SIC(key1) SAC(key2) 
///      - key3==track number of the searched track.
/// 
///  RETURNS: the CKey object found. NULL if not found.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CKey* CKeyList::FindKey(int key1, int key2, int key3)
{
    QPtrListIterator<CKey> it( m_Keys );
    CKey* pk=NULL;
	pk = it.current(); 
    
	while ( pk != 0 ) 
	{
        
		++it;
		if (pk->m_Key1 == key1 && pk->m_Key2 == key2 && (key3 == -1 || pk->m_Key3 == key3))
			return pk;
		
		pk = it.current(); 
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		returns the elapsed time since last refresh of the track referd by sic,sac,trknum.
/// 
///  RETURNS: elapsed time in sec. -1 if no entry found.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CKeyList::GetElapsedRefreshTime(int key1, int key2, int key3)
{
	CKey* pk = FindKey(key1, key2, key3);
	
	if (pk)
		return pk->GetElapsedRefreshTime();
	else
		return -1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		adds a new entry sic, sac, trknum with an order in the list.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CKeyList::AddKey(int key1, int key2, int key3, int order)
{
	CKey* pk = FindKey(key1, key2, key3);
	
	if (pk)
		pk->UpdateTime();
	else
	{
		if (order == -1)
			m_Keys.append(new CKey(key1, key2, key3));
		else
		{
			for(int i = 0; i < m_Keys.count(); i++)
			{
				CKey* pk=m_Keys.at(i);
				
				if (order < pk->m_Order)
				{
					m_Keys.insert(i, new CKey(key1, key2, key3, false, order));
					return;
				}
			}
			m_Keys.append(new CKey(key1, key2, key3, false, order));
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		removes an entry sic, sac
/// 
///  RETURNS: true if removed. false otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CKeyList::RemoveKey(int key1, int key2, int key3)
{
	CKey* pk = FindKey(key1, key2, key3);
	if (pk)
	{
		m_Keys.remove(pk);
		delete pk;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		returns a tracknumber associated with a SIC SAC.
/// 
///  RETURNS: trknum if sic sac exists. -1 otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CKeyList::GetKey(int key1, int key2)
{
	CKey* pk = FindKey(key1, key2);
	
	if (pk)
		return pk->m_Key3;
	else
		return -1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		tests the missing of element in the list
/// 
///  RETURNS: true if the list is empty. false otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CKeyList::IsEmpty()
{
	return m_Keys.count() == 0;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		copy the list given in parameter
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CKeyList::Copy(const CKeyList& src)
{
	RemoveAll();
    QPtrListIterator<CKey> it( src.m_Keys );
    CKey* pk=NULL;
	pk = it.current(); 
    
	while ( pk != 0 ) 
	{
        ++it;
		m_Keys.append(new CKey(pk->m_Key1, pk->m_Key2, pk->m_Key3, pk->m_Ack, pk->m_Order));
		pk = it.current(); 
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		set acknowledge flag for an entry
/// 
///  RETURNS: true if entry found. false otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CKeyList::Ack(int key1, int key2, int key3, bool Ack)
{
	CKey* pk = FindKey(key1, key2, key3);
	
	if (pk)
	{
		pk->m_Ack = Ack;
		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		reset the update marker for all entries in the list
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CKeyList::UnMarkAll()
{
    QPtrListIterator<CKey> it( m_Keys );
	CKey* pk = it.current();
    
	while ( pk != 0 ) 
	{
        ++it;
		pk->m_Mark=false;
		pk = it.current();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		remove all entries for which the update marker is not set
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CKeyList::RemoveUnMarked()
{
    QPtrListIterator<CKey> it( m_Keys );
	CKey* pk = it.current();
    
	while ( pk != 0 ) 
	{
        ++it;
		if (!pk->m_Mark)
		{
			m_Keys.remove(pk);
			delete pk;
		}
		pk = it.current();
	}
}


//-----------------------------------------------------------------------------
//
/// \class CKeyListIterator
/// ierator class to enumerate the entries of a CKeyList
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		constructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CKeyListIterator::CKeyListIterator(const CKeyList &l)
 : m_It(l.m_Keys) 
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		destructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CKeyListIterator::~CKeyListIterator()
{
}
   

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		returns the current entry in the list
/// 
///  RETURNS: true if current element found. false otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CKeyListIterator::current(int *key1, int *key2, int *key3, bool* Ack, int* order)
{
	if (key1) 
		*key1 = -1;
	if (key2) 
		*key2 = -1;
	if (key3) 
		*key3 = -1;
	if (Ack) 
		*Ack = false;
	if (order) 
		*order = -1;
	
	CKey* pk = m_It.current();
	if (pk)
	{
		if (key1) 
			*key1 = pk->m_Key1;
		
		if (key2) 
			*key2 = pk->m_Key2;
		
		if (key3) 
			*key3 = pk->m_Key3;
		
		if (Ack)  
			*Ack  = pk->m_Ack;
		
		if (order) 
			*order = pk->m_Order;

		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		jump to the next entry in the list 
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CKeyListIterator::operator++()
{
	++m_It;
}

//--------------------------------< THE END >----------------------------------


