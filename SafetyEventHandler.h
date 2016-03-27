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
//  FILENAME: SafetyEventHandler.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the declaration of the CEventMngr class
//	  This class is used to manage the coordination communication between the 
//    the different class that manage the handling of message comming from CCS
//    and radar and the HMI. It is aimed at reducing the class coupling in this
//    scope of code.
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

#if !defined(AFX_CSAFETYEVENTHANDLER__INCLUDED_)
#define AFX_CSAFETYEVENTHANDLER__INCLUDED_

//-DATA------------------------------------------------------------------------

#include "SystemMode.h"
#include "CouplingMngr.h"
#include "FlightPlan.h"
#include "InfoMngr.h"
#include "TopLevel.h"
#include "Track.h"

//-PROTOTYPES------------------------------------------------------------------

class CEventMngr : virtual public CTrackEventStub
						 , public CSystemModeEventStub
						 , public CInfoEventStub
						 , public CTopLevelEventStub
						 , public CCouplingEventStub
						 , public CFPLEventStub
{
	friend class CAsdApp;

private :

	//variables
	// copy of the system mode flag
	SystemMode m_SM; 

	//virtual methods
	//This method is used to write a log message
	virtual void WriteLogMsg(const QString & strMessage, const LogTypes &Type);
	//This method is called to write a trace message 
	virtual void WriteTraceBuffer(BYTE* buffer, int len, bool bOnOneLine, bool bReceive, sockaddr_in* addr);
	//This method is called to update the CWP statuses and information
	virtual void OnUpdateAliveMsg();
	//This method handles the complete refresh of the flight plan and safety net
	//data of a track
	virtual void OnTrackNeedUpdate(CTrack* pTrack, bool newtrack);
	//This method implements the handling of a track suppression
	virtual void OnRemoveTrack(CTrack* pTrack, bool Cancelation);
	//This method implements the start of an ageing for a track
	virtual void OnStartAging(CTrack* pTrack);
	//This returns whether the CWP is APP or not
	virtual bool IsAppPos();
	//This method treats the assume/de-assume action of a track
	virtual void OnAssumeDeassume(CTrack* pTrack);
	//This method treats the interaction with a TSSR data field for a track
	virtual void OnTrackTSSR(CTrack* ptrack, QString Arcid, bool untag, QPoint* pnt);
	//This method treats the CFL attribution for a track
	virtual void OnCfl(CTrack* pTrack, int level, QString KdOfAppClearance);
	//This method treats the AFL attribution for a track
	virtual void OnAfl(CTrack* pTrack, int level);
	//This method treats the ASSR attribution for a track
	virtual void OnAssr(CTrack* pTrack, QString assr);
	//This method treats the RFL attribution for a track
	virtual void OnRfl(CTrack* pTrack, int level);
	//This method treats the ARYP/WTC attribution for a track
	virtual void OnATypWtc(CTrack* pTrack, QString adep, QString ades, QString arctyp, QString Wtc);
	//This method treats the tag action for a track
	virtual void OnTrackTag(CTrack* ptrack, QString Arcid, QPoint* pnt);
	//This method treats the untag action for a track
	virtual void OnTrackUntag(CTrack* ptrack, QString Arcid, bool MismatchSolve);
	//This method treats the RWY attribution for a track
	virtual void OnRwy(CTrack* pTrack, QString OldRwy, QString Rwy);
	//This method treats the acknowledgement of an emergency
	virtual void OnTrackEmergAck(CTrack* pTrack, bool ea);
	//This method treats the Highlight action for a track
	virtual void OnTrackHighlight(CTrack* pTrack, bool hl);
	//This method returns the track Identifier
	virtual bool GetArcId(int key1, int key2, int key3, QString& Arcid);
	//This method manages the selection of a radar service
	virtual void OnSelectRadarService(CTrackServer* pts);
	//This method allows to create or update a FPL upon reception of a track update
	virtual void OnFPLUpdateFromTrack(CString CsCallSign, int sic, int sac, int TrkNum, AssociationType tag);
	//This method allows to know if the track is coupled, tagged, modeS tagged or uncoupled
	virtual AssociationType GetCouplingDataAssociationType(int sic, int sac, int TrkNum);


	//This method manages the modification of the system mode
	virtual void OnSystemModeChange(SystemMode oldsm, SystemMode newsm);
	//This method manages the modification of availability of the track to track correlation
	virtual void OnT2TChange(bool state);
	//This method manages the creation of coupling info
	virtual void OnCreateCoupling(CouplingData* cpldata);
	//This method manages update of a coupling info
	virtual void OnUpdateCoupling(CouplingData* cpldata);
	//This method manages the removal of a coupling info
	virtual void OnEraseCoupling(CouplingData* cpldata);
	//This method manages the removal of all the coupling info
	virtual void OnEraseAllCoupling();
	//This method manages the creation of a flight plan
	virtual void OnCreateFPL(int fplid);
	//This method manages the update of a flight plan 
	virtual void OnUpdateFPL(int fplid);
	//This method manages the deletion of a flight plan
	virtual void OnEraseFPL(int fplid, CFlightPlan* pFPL);

	//methods
	//Constructor
	CEventMngr();
	//Destructor
	~CEventMngr() {};
	//This method handles the update of the track according to coupling and FPL data
	void UpdateFromFpl(TrackData *pTD, int sic, int sac, int TrkNum);
	//This method handles the update of the track according to safety net data
	void UpdateFromSafety(TrackData *pTD, int sic, int sac, int TrkNum, bool newtrack);
	//This method returns a coupling data info according to a sic/sact and track number
	CouplingData* GetCouplingData(BYTE sic, BYTE sac, int TrkNum);

};

#endif // end of file


//--------------------------------< THE END >----------------------------------




