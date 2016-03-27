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
//  FILENAME: CouplingMngr.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file manages the coupling betweeen a flight plan and a track
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

#if !defined(CouplingMngr_H)
#define CouplingMngr_H

//-INCLUDE FILES---------------------------------------------------------------

#include "CommonDataTypes.h"
#include "TrackList.h"

//-GLOBAL DATA-----------------------------------------------------------------

class CouplingData;


//-CLASS DEFINITION------------------------------------------------------------
class CCouplingEventStub : public CLogStub
{
public :

	// constructor of class CCouplingEventStub
	CCouplingEventStub() {};
	// destructor of class CCouplingEventStub
	~CCouplingEventStub() {};
	// creates the coupling when a CCS message is received
	virtual void OnCreateCoupling(CouplingData* cpldata) = 0;
	// updates the coupling when a CCS message is received
	virtual void OnUpdateCoupling(CouplingData* cpldata) = 0;
	// erases the coupling when a CCS message is received
	virtual void OnEraseCoupling(CouplingData* cpldata) = 0;
	// erases all the coupling when a CCS message is received
	virtual void OnEraseAllCoupling() = 0;
};

class CouplingData : public CBaseTripleEntry<CouplingData>
{
	friend class CCouplingMngr;
	friend class CEventMngr;


public:

	//methods
	~CouplingData();

	//virtual methods
	virtual bool OnTimeOut(CTripleEntryList<CouplingData>& List);
	virtual bool OnRemoveLink(CTripleEntryList<CouplingData>& List);

	AssociationType m_Tag;
	BYTE m_Sic;
	BYTE m_Sac;
	int m_TrkNum;

	
private:

	
	//variables
	int m_FplId;
	QString m_Arcid;
//	bool m_Mismatch;
//	QString m_ConflictingArcid;
//	AssociationType m_ConflictingAssociation;
	
	//virtual methods
	virtual bool OnCreateLink(CTripleEntryList<CouplingData>& List);
	virtual void OnResolveLink(CTripleEntryList<CouplingData>& List);

	//methods
	CouplingData(CCouplingEventStub* pHEvent);
	CouplingData(CouplingData& pCplData);
	CCouplingEventStub* m_pHEvent;
	
};



class CCouplingMngr : public CObject
{

private :
	//Event Stub object
	CCouplingEventStub* m_pHEvent;
public:
	//Constructor
	CCouplingMngr();
	//Copy constructor
	CCouplingMngr(CCouplingMngr& pCplMngr);
	//Destructor
	~CCouplingMngr();
	//Init method
	void Init(CCouplingEventStub* pHEvent, int CTTTCycle, int CouplingCycle);
	//This method returns a coupling data associated to a sic/sac and a track number
	CouplingData* GetCouplingData(BYTE sic, BYTE sac, int Pistid);
	//This method returns a coupling data associated to a FPL id 
	CouplingData* FindCouplingDataByFplID(int fplid);
	//This method fills the offline defined tag numbers  
	void FillTag(CArray<bool,bool> *pFplNbrs, QString Arcid);
	//This method removes all the coupling info stored in the m_CouplingList list
	void RemoveAll();
	//Coupling data list
	CTripleEntryList<CouplingData> m_CouplingList;
	void CreateCouplingFromTrack(int sic, int sac, int trknum, int fplid, QString Arcid, AssociationType tag);
	void RemoveCouplingFromTrack(int sic, int sac, int trknum);
};
#endif  // end of file

//--------------------------------< THE END >----------------------------------

