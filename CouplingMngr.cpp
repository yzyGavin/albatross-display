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
//  FILENAME: CouplingMngr.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file manages the coupling betweeen a flight plan and a track
// 
//  COMMENTS
//
//  BUILD INFORMATION 
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


//-INCLUDE FILES---------------------------------------------------------------
#include "StdAfx.h"
#include "View.h"
#include "AsdView.h"
#include "CouplingMngr.h"
#include "AsdApp.h"
#include "Track.h"



//-DATA------------------------------------------------------------------------

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CouplingData
/// This class is used to store the content of a coupling message
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
CouplingData::CouplingData(CCouplingEventStub* pHEvent)
{
	m_Sic = -1;
	m_Sac	= -1;
	m_TrkNum = -1;
	m_pHEvent = pHEvent;
//	m_Mismatch = false;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Copy constructor.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CouplingData::CouplingData(CouplingData& pCplData)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		destructor.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CouplingData::~CouplingData()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this function is called when no corresponding coupling message 
///		has been received in time
/// 
///  RETURNS: true, the object will be deleted by calling method. False otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CouplingData::OnTimeOut(CTripleEntryList<CouplingData>& List)
{
	// find associated track
	CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(m_Sic, m_Sac, m_TrkNum);
	if (pTrack)
	{
		return false;
	}
	else
	{
		if (m_pHEvent)
			m_pHEvent->OnEraseCoupling(this);
		return true;
	}
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this function is called when a TTT message is received but is in 
///     conflict with another TTT association
/// 
///  RETURNS: true, the object will be deleted by calling method. False otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CouplingData::OnResolveLink(CTripleEntryList<CouplingData>& List)
{

}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this function is called when a TTE message is not received on time or 
///     an aging process occured on the link
/// 
///  RETURNS: true, the object will be deleted by calling method. False otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CouplingData::OnRemoveLink(CTripleEntryList<CouplingData>& List)
{
	int key1;
	int key2;
	int key3;
	
	if (GetMainKey(key1, key2, key3))
	{
		if (m_pHEvent)
			m_pHEvent->OnUpdateCoupling(this);
	}

	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this function is called when a new TTT link is created
/// 
///  RETURNS: true, the object will be created by calling method. False otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CouplingData::OnCreateLink(CTripleEntryList<CouplingData>& List)
{
	int key1;
	int key2;
	int key3;
	
	if (GetMainKey(key1, key2, key3))
	{
		if (m_pHEvent)
			m_pHEvent->OnUpdateCoupling(this);
	}

	return true;
}

//-----------------------------------------------------------------------------
//
/// \class CCouplingMngr
/// This class is used to handle and store coupling message
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Initialise the CouplingMngr object
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CCouplingMngr::Init(CCouplingEventStub* pHEvent, int CTTTCycle, int CouplingCycle)
{
	m_pHEvent = pHEvent;
	m_CouplingList.SetSize(256, 256, 4096, -1, -1, CouplingCycle, 3000);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		constructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CCouplingMngr::CCouplingMngr()
 : m_pHEvent(NULL)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Copy constructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CCouplingMngr::CCouplingMngr(CCouplingMngr& pCplMngr)
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
CCouplingMngr::~CCouplingMngr()
{
	RemoveAll();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		remove all coupling stored 
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CCouplingMngr::RemoveAll()
{
	m_CouplingList.RemoveAll();
	
	if (m_pHEvent)
		m_pHEvent->OnEraseAllCoupling();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		retrieves a couplingdata object from sic,sac, track number.
/// 
///  RETURNS: pointer to coupling data if found. NULL otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CouplingData* CCouplingMngr::GetCouplingData(BYTE sic, BYTE sac, int Pistid)
{
	if (m_CouplingList.IsAllowed(sic, sac))
		return m_CouplingList.Get(sic, sac, Pistid);
	else
		return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		retrieves a couplingdata object from flight plan identifier.
/// 
///  RETURNS: pointer to coupling data if found. NULL otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CouplingData* CCouplingMngr::FindCouplingDataByFplID(int fplid)
{
	CTripleEntryListIterator<CouplingData> it( m_CouplingList );
	CouplingData* pcpl= it.current();
	
	while ( pcpl != 0 ) 
	{
		++it;
		if (pcpl->m_FplId != -1 && pcpl->m_FplId == fplid)
		{
			return pcpl;
		}
		pcpl = it.current();
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		fills a boolean array according to tag prefixe in entry.
///		for each coupling stored, if the arcid tag is under the format prefixe + XXXX, 
///		the entry XXXX in the boolean array is set to true.
/// 
///  RETURNS: NONE.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CCouplingMngr::FillTag(CArray<bool,bool> *pFplNbrs, QString Arcid)
{
	if (!pFplNbrs)
		return;

	CTripleEntryListIterator<CouplingData> it( m_CouplingList );
	CouplingData* pcpl= it.current();
	
	while ( pcpl != 0 ) 
	{
		++it;

		if ((pcpl->m_Tag == TAGGED) && (pcpl->m_Arcid.left(Arcid.length())==Arcid))
		{
			int fndfpl=atoi(pcpl->m_Arcid.right(pcpl->m_Arcid.length()-Arcid.length()).ascii());
			if (fndfpl)
				pFplNbrs->SetAtGrow(fndfpl,true);
		}
		
		pcpl = it.current();
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Cancels the coupling data for the track identified by the sic, sac and 
///		track number given in argument
/// 
///  RETURNS: NONE.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CCouplingMngr::RemoveCouplingFromTrack(int sic, int sac, int trknum)
{
	if (m_CouplingList.IsAllowed(sic, sac))
	{
		CouplingData* pCplData = m_CouplingList.Get(sic, sac, trknum);
		if (pCplData)
		{
			if (m_CouplingList.RemoveAt(sic, sac, trknum))
			{
				if (m_pHEvent)
					m_pHEvent->OnEraseCoupling(pCplData);
				delete pCplData;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Creates or updates the coupling data for the track identified by the sic, sac and 
///		track number given in argument
/// 
///  RETURNS: NONE.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CCouplingMngr::CreateCouplingFromTrack(int sic, int sac, int trknum, int fplid, QString Arcid, AssociationType tag)
{
	//Init Coupling Data
	CouplingData CurCoupling(m_pHEvent);
	CurCoupling.m_Tag=UNCOUPLED;
	CurCoupling.m_FplId=-1;
	CurCoupling.m_TrkNum=-1;
	CurCoupling.m_Sic=0;
	CurCoupling.m_Sac=0;
//	CurCoupling.m_Mismatch=false;
//	CurCoupling.m_ConflictingArcid="";
//	CurCoupling.m_ConflictingAssociation=UNCOUPLED;

	//Set Coupling Data
	CurCoupling.m_TrkNum = trknum;
	CurCoupling.m_FplId = fplid;
	CurCoupling.m_Sic=sic;
	CurCoupling.m_Sac=sac;
	CurCoupling.m_Tag=tag;
	CurCoupling.m_Arcid=Arcid.left(7);
	
	
	if (m_CouplingList.IsAllowed(CurCoupling.m_Sic, CurCoupling.m_Sac))
	{
		bool Update=false;
		bool Create=false;
//		bool ConflictAutoSolved=false; 
		CouplingData* pCplData = m_CouplingList.Get(CurCoupling.m_Sic, CurCoupling.m_Sac, CurCoupling.m_TrkNum);
		
		if (!pCplData)
		{
			pCplData = new CouplingData(m_pHEvent);
			Create=true;
		}
		else
		{
			if (CurCoupling.m_FplId!=pCplData->m_FplId)
				Update=true;
			if (CurCoupling.m_Tag!=pCplData->m_Tag)
				Update=true;
			if (CurCoupling.m_Arcid!=pCplData->m_Arcid)
				Update=true;
		}

		if (m_CouplingList.SetAt(CurCoupling.m_Sic, CurCoupling.m_Sac, CurCoupling.m_TrkNum, pCplData))
		{
			pCplData->m_Sic = CurCoupling.m_Sic;
			pCplData->m_Sac = CurCoupling.m_Sac;
			pCplData->m_TrkNum = CurCoupling.m_TrkNum;
			pCplData->m_FplId = CurCoupling.m_FplId;
//			if (!CurCoupling.m_Mismatch && !pCplData->m_Mismatch)
//			{
				pCplData->m_Arcid = CurCoupling.m_Arcid;
				pCplData->m_Tag = CurCoupling.m_Tag;
//			}
/*			if (CurCoupling.m_Mismatch && !pCplData->m_Mismatch)
			{
				pCplData->m_Mismatch = CurCoupling.m_Mismatch;
				pCplData->m_ConflictingArcid = CurCoupling.m_ConflictingArcid;
				pCplData->m_ConflictingAssociation = CurCoupling.m_ConflictingAssociation;
			}*/
/*			if (ConflictAutoSolved)
			{
				pCplData->m_Mismatch = false;
				pCplData->m_ConflictingArcid = "";
				pCplData->m_ConflictingAssociation = UNCOUPLED;
			}*/
			
			
			if (Update)
			{
				if (m_pHEvent)
					m_pHEvent->OnUpdateCoupling(pCplData);
			}
			else if (Create)
			{
				if (m_pHEvent)
					m_pHEvent->OnCreateCoupling(pCplData);
			}
		}
		else
		{
			if (m_pHEvent)
				m_pHEvent->WriteLogMsg("Error on m_CouplingList.SetAt", LogInformation);
			if (Create)
				delete pCplData;
		}
	}
}


//--------------------------------< THE END >----------------------------------
