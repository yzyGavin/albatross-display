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
//  FILENAME: SafetyEventHandler.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    Implementation of the class CEventMngr. This class manage the dispatching 
//    of all events coming from flight plans, tracks to the HMI.
// 
//  COMMENTS
//    x
//
//  BUILD INFORMATION 
//    x
//
//  COMMUNICATION / SYNCHRONISATION / LOCKS
//    x
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

#include "StdAfx.h"
#include "View.h"
#include "AsdView.h"
#include "DisplayElement.h"
#include "./CommonFiles/GraphicSymbol.h"
#include "Track.h"
#include "AsdApp.h"
#include "VisuMap.h"
#include "SafetyEventHandler.h"
#include "CouplingMngr.h"
#include "FlightPlan.h"
#include "TopLevel.h"
#include "DataMngr.h"
#include "Elements.h"
#include "UserMngr.h"
#include "WinHints.h"

#include "LineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW2
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//-----------------------------------------------------------------------------
//
/// \class CEventMngr
/// This class is used to manage the coordination communication between the 
/// the different class that manage the handling of message comming from CCS
/// and radar and the HMI. It is aimed at reducing the class coupling in this
/// scope of code.
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
CEventMngr::CEventMngr()
{
	m_SM = FAILED;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method writes a new string in the log file
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::WriteLogMsg(const QString & strMessage, const LogTypes &Type)
{
	CAsdApp::GetApp()->WriteLogMsg(strMessage, Type);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method dump in the trace file under an hexadecimal format the
///      conents of a binary buffer.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::WriteTraceBuffer(BYTE* buffer, int len, bool bOnOneLine, bool bReceive, sockaddr_in* addr)
{
	CAsdApp::GetApp()->WriteTraceBuffer(buffer, len, bOnOneLine, bReceive, addr);
}





//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the handling of the alive message from safety nets
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnUpdateAliveMsg()
{
	CTopLevel::UpdateSystemInfo();
}









//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this returns the type of position
/// 
///  RETURNS: true : approche, false : en-route
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CEventMngr::IsAppPos()
{
	return CAsdApp::GetApp()->GetInfoMngr()->IsAppPos();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the handling of the start of an aging for a track
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnStartAging(CTrack* pTrack)
{

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the handling of a track suppression
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnRemoveTrack(CTrack* pTrack, bool Cancelation)
{
	int sic;
	int sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		CCouplingMngr* pCplMngr = CAsdApp::GetApp()->GetCouplingMngr();
		pCplMngr->RemoveCouplingFromTrack(sic, sac, trknum);
	}
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the acknowledgement of the emergency state on a track
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnTrackEmergAck(CTrack* pTrack, bool ea)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	int sic, sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic == sic && pTrkMngr->m_pTagServer->m_Sac == sac))
		{
			pTrack->m_TD.m_EmergAck = ea;
			pTrack->UpdateFromSafety(true);

		}
	}

}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the highlight on a track
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnTrackHighlight(CTrack* pTrack, bool hl)
{
	int sic, sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic == sic && pTrkMngr->m_pTagServer->m_Sac == sac))
		{
			pTrack->m_TD.m_IsHighlight = hl;
			pTrack->UpdateFromSafety(true);
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the highlight on a track
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnAssr(CTrack* pTrack, QString assr)
{
	int sic, sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic == sic && pTrkMngr->m_pTagServer->m_Sac == sac))
		{
			pTrack->m_TD.m_ASsr = assr;
			pTrack->UpdateFromSafety(true);
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the sending of the ASQI message to CCS
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnRwy(CTrack* pTrack, QString OldRwy, QString Rwy)
{
	int sic, sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic == sic && pTrkMngr->m_pTagServer->m_Sac == sac))
		{
			if (Rwy.length()==2)
			{
				pTrack->m_TD.m_ArrRwy = Rwy;
				pTrack->m_TD.m_ParallelRwy = "";
			}
			else
			{
				pTrack->m_TD.m_ArrRwy = "";
				pTrack->m_TD.m_ParallelRwy = Rwy;
			}
			pTrack->UpdateFromSafety(true);
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the sending of the type and WTC to CCS
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: CIAsdFctDfdCicDes010
//-----------------------------------------------------------------------------
void CEventMngr::OnATypWtc(CTrack* pTrack, QString adep, QString ades, QString arctyp, QString Wtc)
{
	int sic, sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic == sic && pTrkMngr->m_pTagServer->m_Sac == sac))
		{
			pTrack->m_TD.m_Wtc = Wtc;
			pTrack->m_TD.m_Arctyp = arctyp;
			pTrack->m_TD.m_Adep = adep;
			pTrack->m_TD.m_Ades = ades;
			pTrack->UpdateFromSafety(true);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the sending of the AFL to CCS
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnAfl(CTrack* pTrack, int level)
{
	int sic, sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic == sic && pTrkMngr->m_pTagServer->m_Sac == sac))
		{
				pTrack->m_TD.m_Pfl.sprintf("F%.3d", level);
				pTrack->UpdateFromSafety(true);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the sending of the RFL to CCS
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: CIAsdFctDfdCicRfl010
//-----------------------------------------------------------------------------
void CEventMngr::OnRfl(CTrack* pTrack, int level)
{
	int sic, sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic == sic && pTrkMngr->m_pTagServer->m_Sac == sac))
		{
				pTrack->m_TD.m_Rfl.sprintf("F%.3d", level);
				pTrack->UpdateFromSafety(true);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the sending of the CFL to CCS
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnCfl(CTrack* pTrack, int level, QString KdOfAppClearance)
{
	int sic, sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic == sic && pTrkMngr->m_pTagServer->m_Sac == sac))
		{
			if (KdOfAppClearance.length())
			{
				pTrack->m_TD.m_KdOfAppClear.sprintf("%s", KdOfAppClearance.ascii());
				pTrack->m_TD.m_Cfl.sprintf("F000");
				pTrack->UpdateFromSafety(true);
			}
			else
			{
				pTrack->m_TD.m_KdOfAppClear.sprintf("");
				pTrack->m_TD.m_Cfl.sprintf("F%.3d", level);
				pTrack->UpdateFromSafety(true);
			}
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the sending of assume/deassume to CCS
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnAssumeDeassume(CTrack* pTrack)
{
	int sic, sac;
	int trknum;
	if (pTrack->GetMainKey(sic, sac, trknum))
	{
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic == sic && pTrkMngr->m_pTagServer->m_Sac == sac))
		{
			pTrack->m_TD.m_IsAoc = !pTrack->m_TD.m_IsAoc;
			pTrack->UpdateFromSafety(true);
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method implements the search of the callsign or calsign tag
///     given a sic sac trknum. This method works even if the track is not
///     yet received
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CEventMngr::GetArcId(int key1, int key2, int key3, QString& Arcid)
{
	CouplingData* pCplData = GetCouplingData(key1, key2, key3);
	if (pCplData)
	{
		if (pCplData->m_Tag != UNCOUPLED)
		{
			CFplMngr* pFplMngr=CAsdApp::GetApp()->GetFplMngr();
			CFlightPlan* pFpl=pFplMngr->GetFplById(pCplData->m_FplId);
			if (pFpl)
			{
				Arcid = pFpl->m_Arcid;
				return true;
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the complete refresh of the flight plan and safety net
///     data of a track
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnTrackNeedUpdate(CTrack* pTrack, bool newtrack)
{
	int sic, sac, TrkNum;
	if (pTrack->GetMainKey(sic, sac, TrkNum))
	{
		UpdateFromFpl(&pTrack->m_TD, sic, sac, TrkNum);
		pTrack->UpdateFromSafety(true);
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		sends to the CCS a tag track message
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: CIAsdFctDfdCicCst015, CIAsdFctDfdCicCst020, CIAsdFctDfdCicCst030,
///					CIAsdFctDfdCicCst035, CIAsdFctDfdCicCst085, CIAsdFctDfdCicLtg010,
///					CIAsdFctDfdCicLtg015, CIAsdFctDfdCicLtg020
//-----------------------------------------------------------------------------
void CEventMngr::OnTrackTag(CTrack* ptrack, QString Arcid, QPoint* pnt)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();

	bool found = false;
	CTripleEntryListIterator<CouplingData> it( CAsdApp::GetApp()->GetCouplingMngr()->m_CouplingList );
	CouplingData* pltd = it.current();
	while ( pltd != 0 ) 
	{
		++it;
		if (pltd->m_Tag == TAGGED) 
		{
			if (pltd->m_Arcid == Arcid)
			{
				found = true;
				break;
			}
		}
		pltd = it.current();
	}
	if (found)
	{
		CSKMessageBox::ShowWarning( 0, "Warning message", "Tag already exists.");
	}
	else
	{
		int sic, sac;
		int trknum;
		if (ptrack->GetMainKey(sic, sac, trknum))
		{
			bool doit = true;
			if (!pTrkMngr->m_pTagServer)
				pTrkMngr->m_pTagServer = pTrkMngr->m_pTrkSrc;
			if (pTrkMngr->m_pTagServer && (pTrkMngr->m_pTagServer->m_Sic != sic || pTrkMngr->m_pTagServer->m_Sac != sac))
			{
				QString Cp_Asd955 = "All previous local tags will be lost. Ok ?";

				QPoint point=QPoint(0, 20);
				if (pnt)
					point = QPoint(pnt->x(), pnt->y()+20);
				int value = CSKMessageBox::question(0, "Confirmation Message", Cp_Asd955, point,
								QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);	
				if (value == 3)
				{
					CFplMngr* pFPLMngr=CAsdApp::GetApp()->GetFplMngr();
					CCouplingMngr* pCouplingMngr=CAsdApp::GetApp()->GetCouplingMngr();

					pFPLMngr->EmptyAllLists();
					pFPLMngr->RemoveAll();
					pCouplingMngr->RemoveAll();

					pTrkMngr->m_pTagServer = pTrkMngr->GetCurrentServer();
				}
				else
					doit = false;
			}
			if (doit)
			{
				ptrack->m_TD.m_IsCoupled=true;
				ptrack->m_TD.m_IsTagged=true;
				ptrack->m_TD.m_CsTag=Arcid;
				OnFPLUpdateFromTrack(Arcid, sic, sac, trknum, TAGGED);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		sends to the CCS a untag track message
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: CIAsdFctDfdCicCst075, CIAsdFctDfdCicCst080
//-----------------------------------------------------------------------------
void CEventMngr::OnTrackUntag(CTrack* ptrack, QString Arcid, bool MismatchSolve)
{

	CTrackMngr* pTrkMngr = CAsdApp::GetApp()->GetTrackMngr();
	if (pTrkMngr->m_pTrkSrc)
	{
		int sic, sac;
		int trknum;
		if (ptrack->GetMainKey(sic, sac, trknum))
		{
			if (!MismatchSolve)
			{
				CouplingData* pCplData = GetCouplingData(sic, sac, trknum);
				//If the track was tagged because of its Mode S, it should not be automatically
				//Mode S tagged at the next Track update
				if (pCplData->m_Tag == MODESTAGGED)
					ptrack->m_TD.m_IsManUntag = true;	
				ptrack->m_TD.m_IsCoupled = false;
				ptrack->m_TD.m_IsTagged = false;

				//the associated FPL is deleted
				CFplMngr* pFplMngr=CAsdApp::GetApp()->GetFplMngr();
				pFplMngr->RemoveFromTrack(trknum);
				//the associated coupling data is deleted
				CCouplingMngr* pCplMngr = CAsdApp::GetApp()->GetCouplingMngr();
				pCplMngr->RemoveCouplingFromTrack(sic, sac, trknum);				
			}
			else
			{
				//Solve a Callsign Mismatch
				CouplingData* pCplData = GetCouplingData(sic, sac, trknum);

				ptrack->m_TD.m_Mismatch=false;
				ptrack->m_TD.m_ConflictHasBeenSolved=true;


				//The selected Callsign is the conflicting Callsign
				if (Arcid == ptrack->m_TD.m_ConflictingArcid)
				{
					OnFPLUpdateFromTrack(Arcid, sic, sac, trknum, ptrack->m_TD.m_ConflictingAssociation);
				}
				
				for (int view=0;view<ptrack->m_ElemTable.GetSize();view++)
					ptrack->m_ElemTable[view]->OnUpdate();

			}
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		retrieve a free tag and sends to the CCS a tag track message
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: CIAsdFctDfdCicCst055
//-----------------------------------------------------------------------------
void CEventMngr::OnTrackTSSR(CTrack* ptrack, QString Arcid, bool untag, QPoint* pnt)
{
	CCouplingMngr* pCplMngr = CAsdApp::GetApp()->GetCouplingMngr();
	if (untag)
	{
		OnTrackUntag(ptrack, Arcid, false);
	}
	else
	{
		CArray<QString,QString&> TagTable;
		CDataMngr::GetTagList(TagTable);
		for (int i=0;i<TagTable.GetSize();i++)
		{
			if (TagTable[i].compare(Arcid)==0)
			{
				CTagArray FplNbrs;
				pCplMngr->FillTag(&FplNbrs, Arcid);
				Arcid=FplNbrs.FindTag(Arcid);
				break;
			}
		}
		if (Arcid.length() > 7)
			CSKMessageBox::ShowWarning( 0, "Warning message", "Maximum number of local tag reached for this predefined tag.");
		else
			OnTrackTag(ptrack, Arcid, pnt);
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this message handles the selection change of a track server
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnSelectRadarService(CTrackServer* pts)
{
	CFplMngr* pFPLMngr=CAsdApp::GetApp()->GetFplMngr();

	if (!pts)
	{
		// radar service is lost. Going to frozen.
		pFPLMngr->FreezeAllLists(true);
	}
	CTopLevel::UpdateSystemInfo();
	CRwdView::UpdateAllStatusBar();
	CRwdView::UpdateAllViews();
	CRwdView::SetMapsAvailable(false);
}








//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this message handles the value change of the system mode
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnSystemModeChange(SystemMode oldsm, SystemMode newsm)
{
	m_SM = newsm;
	CTopLevel::UpdateSystemInfo();
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the change of status of the TTT interface
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnT2TChange(bool state)
{
	CTopLevel::UpdateSystemInfo();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the reception of a new coupling from CCS.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnCreateCoupling(CouplingData* cpldata)
{
	// find associated track
	CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(cpldata->m_Sic, cpldata->m_Sac, cpldata->m_TrkNum);
	if (pTrack)
	{
		// update track flight plan data
		UpdateFromFpl(&pTrack->m_TD, cpldata->m_Sic, cpldata->m_Sac, cpldata->m_TrkNum);
		// update label fields
		pTrack->UpdateDataTable();
		// update HMI
		for (int view=0;view<pTrack->m_ElemTable.GetSize();view++)
			pTrack->m_ElemTable[view]->OnUpdate();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the reception of a change in coupling from CCS.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: CIAsdSymSmbErs030, CIAsdSymSmbErs110, CIAsdSymSmbFld035
//-----------------------------------------------------------------------------
void CEventMngr::OnUpdateCoupling(CouplingData* cpldata)
{
	CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(cpldata->m_Sic, cpldata->m_Sac, cpldata->m_TrkNum);
	// find associated track
	if (pTrack)
	{
		// update track flight plan data
		UpdateFromFpl(&pTrack->m_TD, cpldata->m_Sic, cpldata->m_Sac, cpldata->m_TrkNum);
		// update label fields
		pTrack->UpdateDataTable();
		// update HMI
		for (int view=0;view<pTrack->m_ElemTable.GetSize();view++)
			pTrack->m_ElemTable[view]->OnUpdate();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the reception of a deletion of coupling from CCS.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnEraseCoupling(CouplingData* cpldata)
{
	CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(cpldata->m_Sic, cpldata->m_Sac, cpldata->m_TrkNum);
	// find associated track
	if (pTrack)
	{
		// update track flight plan data with lack of coupling
		UpdateFromFpl(&pTrack->m_TD, -1, -1, -1);
		// update label
		pTrack->UpdateDataTable();
		// update HMI
		for (int view=0;view<pTrack->m_ElemTable.GetSize();view++)
			pTrack->m_ElemTable[view]->OnUpdate();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the reception of a deletion of all the coupling
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnEraseAllCoupling()
{
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the reception of new flight plan
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnCreateFPL(int fplid)
{
	CouplingData* pcpld = CAsdApp::GetApp()->GetCouplingMngr()->FindCouplingDataByFplID(fplid);
	// retrieve the associated coupling
	if (pcpld)
	{
		int key1, key2, key3;
		if (pcpld->GetMainKey(key1, key2, key3))
		{
			// retrieve the associted track
			CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(key1, key2, key3);
			if (pTrack)
			{
				// update fpl track data 
				UpdateFromFpl(&pTrack->m_TD, key1, key2, key3);
				// update label data fields
				pTrack->UpdateDataTable();
				// update HMI
				for (int view=0;view<pTrack->m_ElemTable.GetSize();view++)
					pTrack->m_ElemTable[view]->OnUpdate();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the reception of an update of a flight plan
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: CIAsdSymSmbErs115, CIAsdSymSmbFld040
//-----------------------------------------------------------------------------
void CEventMngr::OnUpdateFPL(int fplid)
{
	// retrieve the associated coupling
	CouplingData* pcpld = CAsdApp::GetApp()->GetCouplingMngr()->FindCouplingDataByFplID(fplid);
	if (pcpld)
	{
		int key1, key2, key3;
		if (pcpld->GetMainKey(key1, key2, key3))
		{
			// retrieve the associted track
			CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(key1, key2, key3);
			if (pTrack)
			{
				// update fpl track data 
				UpdateFromFpl(&pTrack->m_TD, key1, key2, key3);
				// update label data fields
				pTrack->UpdateDataTable();
				// update HMI
				for (int view=0;view<pTrack->m_ElemTable.GetSize();view++)
					pTrack->m_ElemTable[view]->OnUpdate();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the deletion of a flight plan
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CEventMngr::OnEraseFPL(int fplid, CFlightPlan* pFPL)
{
	CAsdApp* papp = CAsdApp::GetApp();
	CouplingData* pcpld = CAsdApp::GetApp()->GetCouplingMngr()->FindCouplingDataByFplID(fplid);

	// retrieve the associated coupling
	if (pcpld)
	{
		int key1, key2, key3;
		if (pcpld->GetMainKey(key1, key2, key3))
		{
			CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(key1, key2, key3);
			// retrieve the associated track
			if (pTrack)
			{
				// update fpl track data 
				UpdateFromFpl(&pTrack->m_TD, -1, -1, -1);
				// update label data fields
				pTrack->UpdateDataTable();
				// update HMI
				for (int view=0;view<pTrack->m_ElemTable.GetSize();view++)
					pTrack->m_ElemTable[view]->OnUpdate();
			}
		}
	}
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		this method handles the update of the track according to coupling and FPL data
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: CIAsdFctDfdCicLtg025, CIAsdSymSmbNrm105, CIAsdSymSmbNrm110,
//
//-----------------------------------------------------------------------------
void CEventMngr::UpdateFromFpl(TrackData *pTD, int sic, int sac, int TrkNum)
{
	// CDS BREAK NGT 08/06/2006 Bad method length rule violated. The code has been 
	//							factorized as much as possible. 
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	// if the display is not frozen
	if (pMngr->m_pTrkSrc)
	{
		//In case of a ageing uncoupled DB, if the track is not tagged, the identifier of the track is 
		//the TSSR
		//trk/fpl association
		pTD->m_IsCoupled=false;
		pTD->m_IsTagged=false;
		pTD->m_AssociationType=UNCOUPLED;
		pTD->m_CsTag="";
		pTD->m_Arcid="";
		pTD->m_Wtc="";
		pTD->m_IsDeparture=false;
		pTD->m_IsArrival=false;
		pTD->m_Mnemo=0;
		pTD->m_Cfl="";
		pTD->m_ASpeed="";
		pTD->m_Arctyp="";
		pTD->m_CurFix="";
		pTD->m_FltRul="";
		pTD->m_Xpt="";
		pTD->m_Xfl="";
		pTD->m_Etx="";
		pTD->m_Rvr="";
		pTD->m_Rfl="";
		pTD->m_Sid="";
		pTD->m_Route="";
		pTD->m_833="";
		pTD->m_Rvsm="";
		pTD->m_FplCat="";
		pTD->m_Eobt="";
		pTD->m_DepRwy="";
		pTD->m_NextSectorNameOverFlown="";
		pTD->m_Altrnt1="";
		pTD->m_Altrnt2="";

		CouplingData* pCplData = GetCouplingData(sic, sac, TrkNum);

		if (pCplData)
		{ // there is a coupling data associated to this track
			if (pCplData->m_Tag == COUPLED)
			{ // there is a tag on this track
				pTD->m_IsCoupled=true;
				pTD->m_IsTagged=false;
			}
			else
			{
				if (pCplData->m_Tag != UNCOUPLED)
				{
					// this track is coupled
					pTD->m_IsCoupled=true;
					pTD->m_IsTagged=true;
				}
			}
			pTD->m_AssociationType=pCplData->m_Tag;

				
				
			CFplMngr* pFplMngr=CAsdApp::GetApp()->GetFplMngr();
			CFlightPlan* pFpl=pFplMngr->GetFplById(pCplData->m_FplId);
			if (pFpl)
			{  // we found the flight plan associated
				
				if (pTD->m_IsTagged)
					pTD->m_CsTag = pFpl->m_Arcid;
				else
					pTD->m_Arcid = pFpl->m_Arcid;
//				pTD->m_Adep=pFpl->m_Adep;
//				pTD->m_Ades=pFpl->m_Ades;
				QString Rwy = "";//pSeqListMngr->GetArrRwyByCallsign(pTD->m_Arcid);
				if (Rwy != "")
					pTD->m_ArrRwy=Rwy;
				else
				{
					if (pFpl->m_ArrRwy.length())
						pTD->m_ArrRwy=pFpl->m_ArrRwy;
				}
				pTD->m_CurFix=pFpl->m_CurFix;
				pTD->m_Wtc=pFpl->m_Wtc;
				pTD->m_Arctyp=pFpl->m_ArcTyp;
				pTD->m_ASpeed=pFpl->m_ASpeed;
				pTD->m_Cfl = pFpl->m_Cfl;
				pTD->m_FltRul=pFpl->m_FltRul;
				pTD->m_Xpt=pFpl->m_Xpt;
				pTD->m_Xfl=pFpl->m_Xfl;
				pTD->m_Etx=pFpl->m_Etx;
				pTD->m_Rvr=pFpl->m_Rvr;
				pTD->m_Rfl=pFpl->m_Rfl;
				pTD->m_Sid=pFpl->m_Sid;
				pTD->m_ASsr=pFpl->m_ASsr;
				pTD->m_Route=pFpl->m_Route;
				pTD->m_Eobt=pFpl->m_Eobt;
				pTD->m_DepRwy=pFpl->m_DepRwy;
				pTD->m_Altrnt1=pFpl->m_Altrnt1;
				pTD->m_Altrnt2=pFpl->m_Altrnt2;
				pTD->m_NextSectorNameOverFlown=pFpl->m_NextSectorNameOverFlown;
				pTD->m_833=pFpl->m_833;
				pTD->m_Rvsm=pFpl->m_Rvsm;
				pTD->m_FplCat=pFpl->m_FplCat;
				//Definition of departure flight is chaned to match with SRS ASD definition 
				if (pFpl->m_FplCat=="S" || pFpl->m_FplCat=="I")
					pTD->m_IsDeparture=true;
				if (pFpl->m_Ades==LOCAL_AIRPORT)
					pTD->m_IsArrival=true;
			}
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		retrieves the coupling data associtad with a track acrros all track server
///     using the TTT associations.
///     this function works even if there is not track received, only TTT is needed.
/// 
///  RETURNS: the coupling data object found. NULL is not found.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CouplingData* CEventMngr::GetCouplingData(BYTE sic, BYTE sac, int TrkNum)
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	CCouplingMngr* pCplMngr = CAsdApp::GetApp()->GetCouplingMngr();
	CKeyList keys;
	// retrieves all TTT association for this track
	if (pMngr->m_TrackList.GetAllLink(sic, sac, TrkNum, keys))
	{
		CKeyListIterator it( keys );
		int key1=-1, key2=-1, key3=-1;
		// scan all TTT associations
		while ( it.current(&key1, &key2, &key3) ) 
		{
			++it;
			{
				CouplingData* pCplData = pCplMngr->GetCouplingData(key1, key2, key3);
				// if coupling data found, return it.
				if (pCplData)
					return pCplData;
			}
		}
	}
	return NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function called at the reception of a Track Update for which a 
///		Callsign is defined, creates the associated FPL and Coupling data, or
///		Update it if already created
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: 
//-----------------------------------------------------------------------------
void CEventMngr::OnFPLUpdateFromTrack(CString CsCallSign, int sic, int sac, int TrkNum, AssociationType tag)
{
	CFplMngr* pFplMngr=CAsdApp::GetApp()->GetFplMngr();
	CCouplingMngr* pCplMngr = CAsdApp::GetApp()->GetCouplingMngr();
	//Create or update the associated FPL Data, the track number is used as
	//FPL Number
	pFplMngr->CreateFromTrack(TrkNum, CsCallSign, sic, sac, TrkNum);
	//Create or update the associated Coupling Data
	pCplMngr->CreateCouplingFromTrack(sic, sac, TrkNum, TrkNum, CsCallSign, tag);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function returns the type of association from a track to a potential
///		flight plan
/// 
///  RETURNS: type of association
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
AssociationType CEventMngr::GetCouplingDataAssociationType(int sic, int sac, int TrkNum)
{
	AssociationType type = UNCOUPLED;
	CCouplingMngr* pCplMngr = CAsdApp::GetApp()->GetCouplingMngr();
	CouplingData* pCplData = pCplMngr->GetCouplingData(sic, sac, TrkNum);

	// if coupling data found
	if (pCplData)
		type = pCplData->m_Tag;

	return type;
}



//--------------------------------< THE END >----------------------------------
