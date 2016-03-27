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
//  FILENAME: FlightPlan.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the management of flight plan data received from CCS
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
// 
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include <stdlib.h>
#include "FlightPlan.h"
#include "Geo.h"
#include "AsdApp.h"
#include "View.h"
#include "DisplayElement.h"
#include "LineView.h"
#include "FontMngr.h"
#include "ColorMngr.h"
#include "FlightPlan.h"
#include "Track.h"
#include "InfoMngr.h"
#include "CouplingMngr.h"




//-GLOBAL DATA-----------------------------------------------------------------
// table of font used in the CSU lists
 CArray<QFont,QFont> CViewFpl::m_FontTable;
 // array that gathers information of type CaFieldDef
 CArray<CViewFpl::FplFieldDef,CViewFpl::FplFieldDef> CViewFpl::m_FieldDef;

//-PROTOTYPES------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CFplMngr
/// This class is used to set flight plan data retrieved both from the CCS
///	and from the radar services
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the contructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CFplMngr::CFplMngr()
{
	m_FplTable.SetSize(10000);
	for (int i=0;i<m_FplTable.GetSize();i++)
		m_FplTable[i]=NULL; 
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CFplMngr::~CFplMngr()
{
	RemoveAll();
	m_FplTable.RemoveAll();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method makes a link to CFPLEventStub class
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CFplMngr::Init(CFPLEventStub* pHEvent)
{
	m_iFontSize=0;

	m_pHEvent = pHEvent;
	//Full List construction
	m_pFplFullList = new CLineView();
	m_pFplFullList->OnInitialUpdate();
	m_pFplFullList->setCaption("Full FPL List");
	CWdwMngr::AddWindow(m_pFplFullList,"CSU_LIST", -1, false);

	m_pFplFullList->SetStyle(LV_VERTGRID);
	m_pFplFullList->SetBkColor(CColorMngr::GetColorNb("WND_MAIN_AREA_BKGRND"));
	m_pFplFullList->SetVerticalMargin(2);
	m_pFplFullList->SetHorizontalMargin(5);
	m_pFplFullList->AddColumn("C/S",90);
	m_pFplFullList->AddColumn("SSR",60);
	m_pFplFullList->AddColumn("ATYP",60);
	m_pFplFullList->AddColumn("CFL",60);
	m_pFplFullList->AddColumn("AFL",60);
	m_pFplFullList->AddColumn("ADES",60);

	m_pFplFullList->SetMaxDisplay(20);
	m_pFplFullList->SetCompareFunc(&CViewFpl::FplCompare);
	AddView(m_pFplFullList);
	m_pFplFullList->SetTitleFont("TITLE_BAR");

	//Coupled FPL List Construction
	m_pFplCoupledList = new CLineView();
	m_pFplCoupledList->OnInitialUpdate();
	m_pFplCoupledList->setCaption("Coupled FPL List");
	CWdwMngr::AddWindow(m_pFplCoupledList,"CSU_LIST", -1, false);

	m_pFplCoupledList->SetStyle(LV_VERTGRID);
	m_pFplCoupledList->SetBkColor(CColorMngr::GetColorNb("WND_MAIN_AREA_BKGRND"));
	m_pFplCoupledList->SetVerticalMargin(2);
	m_pFplCoupledList->SetHorizontalMargin(5);
	m_pFplCoupledList->AddColumn("C/S",90);
	m_pFplCoupledList->AddColumn("SSR",60);
	m_pFplCoupledList->AddColumn("ATYP",60);
	m_pFplCoupledList->AddColumn("CFL",60);
	m_pFplCoupledList->AddColumn("AFL",60);
	m_pFplCoupledList->AddColumn("ADES",60);

	m_pFplCoupledList->SetMaxDisplay(20);
	m_pFplCoupledList->SetCompareFunc(&CViewFpl::FplCompare);
	AddView(m_pFplCoupledList);
	m_pFplCoupledList->SetTitleFont("TITLE_BAR");

	//Mode S Tagged FPL List Construction
	m_pFplModeSList = new CLineView();
	m_pFplModeSList->OnInitialUpdate();
	m_pFplModeSList->setCaption("Mode S Tagged FPL List");
	CWdwMngr::AddWindow(m_pFplModeSList,"CSU_LIST", -1, false);

	m_pFplModeSList->SetStyle(LV_VERTGRID);
	m_pFplModeSList->SetBkColor(CColorMngr::GetColorNb("WND_MAIN_AREA_BKGRND"));
	m_pFplModeSList->SetVerticalMargin(2);
	m_pFplModeSList->SetHorizontalMargin(5);
	m_pFplModeSList->AddColumn("C/S",90);
	m_pFplModeSList->AddColumn("SSR",60);
	m_pFplModeSList->AddColumn("ATYP",60);
	m_pFplModeSList->AddColumn("CFL",60);
	m_pFplModeSList->AddColumn("AFL",60);
	m_pFplModeSList->AddColumn("ADES",60);

	m_pFplModeSList->SetMaxDisplay(20);
	m_pFplModeSList->SetCompareFunc(&CViewFpl::FplCompare);
	AddView(m_pFplModeSList);
	m_pFplModeSList->SetTitleFont("TITLE_BAR");

	//Tagged FPL List Construction
	m_pFplTagList = new CLineView();
	m_pFplTagList->OnInitialUpdate();
	m_pFplTagList->setCaption("Tagged FPL List");
	CWdwMngr::AddWindow(m_pFplTagList,"CSU_LIST", -1, false);

	m_pFplTagList->SetStyle(LV_VERTGRID);
	m_pFplTagList->SetBkColor(CColorMngr::GetColorNb("WND_MAIN_AREA_BKGRND"));
	m_pFplTagList->SetVerticalMargin(2);
	m_pFplTagList->SetHorizontalMargin(5);
	m_pFplTagList->AddColumn("C/S",90);
	m_pFplTagList->AddColumn("SSR",60);
	m_pFplTagList->AddColumn("ATYP",60);
	m_pFplTagList->AddColumn("CFL",60);
	m_pFplTagList->AddColumn("AFL",60);
	m_pFplTagList->AddColumn("ADES",60);

	m_pFplTagList->SetMaxDisplay(20);
	m_pFplTagList->SetCompareFunc(&CViewFpl::FplCompare);
	AddView(m_pFplTagList);
	m_pFplTagList->SetTitleFont("TITLE_BAR");

	//BIN FPL List Construction
	m_pFplBinList = new CLineView();
	m_pFplBinList->OnInitialUpdate();
	m_pFplBinList->setCaption("BIN FPL List");
	CWdwMngr::AddWindow(m_pFplBinList,"CSU_LIST", -1, false);

	m_pFplBinList->SetStyle(LV_VERTGRID);
	m_pFplBinList->SetBkColor(CColorMngr::GetColorNb("WND_MAIN_AREA_BKGRND"));
	m_pFplBinList->SetVerticalMargin(2);
	m_pFplBinList->SetHorizontalMargin(5);
	m_pFplBinList->AddColumn("C/S",90);
	m_pFplBinList->AddColumn("SSR",60);
	m_pFplBinList->AddColumn("ATYP",60);
	m_pFplBinList->AddColumn("CFL",60);
	m_pFplBinList->AddColumn("AFL",60);
	m_pFplBinList->AddColumn("ADES",60);

	m_pFplBinList->SetMaxDisplay(20);
	m_pFplBinList->SetCompareFunc(&CViewFpl::FplCompare);
	AddView(m_pFplBinList);
	m_pFplBinList->SetTitleFont("TITLE_BAR");

	CViewFpl::InitTables();
	m_Frozen=false;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to display the Full FPL List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::DisplayFPLFullListWindow()
{
	if (m_pFplFullList)
	{
		m_pFplFullList->OnUpdate(NULL,NULL,NULL);
		CWdwMngr::ShowWindow(m_pFplFullList,-1);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to access the Full FPL List window pointer
/// 
///  RETURNS:
///  pointer to the Full List CLineView object
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CLineView* CFplMngr::GetFullListDlg()
{
	if (m_pFplFullList)
	{
		return m_pFplFullList;
	}
	else
		return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to remove from display the Full FPL List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::RemoveFPLFullListWindow()
{
	if (m_pFplFullList)
		CWdwMngr::HideWindow(m_pFplFullList);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to display the Coupled FPL List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::DisplayFPLCoupledListWindow()
{
	if (m_pFplCoupledList)
	{
		m_pFplCoupledList->OnUpdate(NULL,NULL,NULL);
		CWdwMngr::ShowWindow(m_pFplCoupledList,-1);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to access the Coupled FPL List window pointer
/// 
///  RETURNS:
///  pointer to the Coupled List CLineView object
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CLineView* CFplMngr::GetCoupledListDlg()
{
	if (m_pFplCoupledList)
	{
		return m_pFplCoupledList;
	}
	else
		return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to remove from display the Coupled FPL List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::RemoveFPLCoupledListWindow()
{
	if (m_pFplCoupledList)
		CWdwMngr::HideWindow(m_pFplCoupledList);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to display the FPL Mode S Tagged List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::DisplayFPLModeSListWindow()
{
	if (m_pFplModeSList)
	{
		m_pFplModeSList->OnUpdate(NULL,NULL,NULL);
		CWdwMngr::ShowWindow(m_pFplModeSList,-1);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to access the Mode S Tagged FPL List window pointer
/// 
///  RETURNS:
///  pointer to the Mode S Tagged List CLineView object
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CLineView* CFplMngr::GetModeSListDlg()
{
	if (m_pFplModeSList)
	{
		return m_pFplModeSList;
	}
	else
		return NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to remove from display the Mode S Tagged FPL List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::RemoveFPLModeSListWindow()
{
	if (m_pFplModeSList)
		CWdwMngr::HideWindow(m_pFplModeSList);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to display the Tagged FPL List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::DisplayFPLTagListWindow()
{
	if (m_pFplTagList)
	{
		m_pFplTagList->OnUpdate(NULL,NULL,NULL);
		CWdwMngr::ShowWindow(m_pFplTagList,-1);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to access the Tagged FPL List window pointer
/// 
///  RETURNS:
///  pointer to the Tagged List CLineView object
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CLineView* CFplMngr::GetTaggedListDlg()
{
	if (m_pFplTagList)
	{
		return m_pFplTagList;
	}
	else
		return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to remove from display the Tagged FPL List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::RemoveFPLTagListWindow()
{
	if (m_pFplTagList)
		CWdwMngr::HideWindow(m_pFplTagList);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to display the FPL Bin List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::DisplayFPLBinListWindow()
{
	if (m_pFplBinList)
	{
		m_pFplBinList->OnUpdate(NULL,NULL,NULL);
		CWdwMngr::ShowWindow(m_pFplBinList,-1);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to access the Bin FPL List window pointer
/// 
///  RETURNS:
///  pointer to the Bin List CLineView object
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CLineView* CFplMngr::GetBinListDlg()
{
	if (m_pFplBinList)
	{
		return m_pFplBinList;
	}
	else
		return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to remove from display the Bin FPL List window
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::RemoveFPLBinListWindow()
{
	if (m_pFplBinList)
		CWdwMngr::HideWindow(m_pFplBinList);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to remove a view to the STCA list (CLineView)
/// 
///  RETURNS:
///  an integer which is equal to 1
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplMngr::AddView(CView* pView)
{
	m_ViewTable.Add(pView);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to remove a view to the STCA list (CLineView)
/// 
///  RETURNS:
///  an integer which is equal to 1
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CFplMngr::RemoveView(CView* pView)
{
	for (int view=0;view<m_ViewTable.GetSize();view++)
	{
		if (pView==m_ViewTable[view])
		{
			m_ViewTable.RemoveAt(view);
			break;
		}
	}
	return 1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method removes the flight plan table locally stored
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CFplMngr::RemoveAll()
{
	for (int fpl=0;fpl<m_FplTable.GetSize();fpl++)
	{
		if (m_FplTable[fpl])
			delete m_FplTable[fpl];
		m_FplTable[fpl]=NULL; 
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks cyclically the validity of a flight plan and fills the
///		FPL Lists with the different valid FPLs
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CFplMngr::FPLAgingCkeck()
{

	QDateTime curtime=QDateTime::currentDateTime();
	for (int i=0;i<m_FplTable.GetSize();i++)
	{
		if ((m_FplTable[i]) && (abs(m_FplTable[i]->m_LastReceivedTime.secsTo(curtime))>300))
		{
			delete m_FplTable[i];
			m_FplTable[i]=NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the flight plan corresponding to the flight plan ID given 
///		as parameter
/// 
///  RETURNS: 
///		the flight plan associated to the flight plan ID
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CFlightPlan* CFplMngr::GetFplById(int FplId)
{
	if (FplId == -1)
		return NULL;
	else
		return m_FplTable[FplId];
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets a flight plan associated to the SSR code given in argument
/// 
///  RETURNS: 
///		the flight plan associated to the ssr code
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CFlightPlan* CFplMngr::GetFpl(int ssr)
{
	if (ssr!=0)
	{
		QString strssr;
		strssr.sprintf("%.4o",ssr);
		for (int fpl=0;fpl<m_FplTable.GetSize();fpl++)
		{
			if (m_FplTable[fpl])
			{
				if ((m_FplTable[fpl]->m_ASsr.compare(strssr)==0) || (m_FplTable[fpl]->m_Ssr.compare(strssr)==0))
					return m_FplTable[fpl];
			}
		}
	}
	return NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates, if not already created, a flight plan associated to
///		the FPL Number given as argument
///		The flight plan is created or updated with the information given as
///		argument: Callsign, ADEP, ADES, ATYP
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CFplMngr::CreateFromTrack(int FplId, QString CsCallSign, int Sic, int Sac, int TrkNum)
{
	if ((FplId!=-1) && (FplId<m_FplTable.GetSize()))
	{
		bool newfpl=false;
		//Retrieves the FPL data corresponding to the FPL Number
		CFlightPlan* pFpl=m_FplTable[FplId];
		//If the FPL data does not exist it is created
		if (!pFpl)
		{
			newfpl=true;
			pFpl=new CFlightPlan(FplId);
			m_FplTable[FplId]=pFpl;
		}
		if (pFpl)
		{
			bool doupdate = pFpl->UpdateDataFromTrack(CsCallSign, Sic, Sac, TrkNum);
			//The flight plan is created
			if (newfpl)
			{
				if (m_pHEvent)
					m_pHEvent->OnCreateFPL(FplId);
			}
			else if (doupdate)
			{
				//The flight plan is updated only if necessary
				if (m_pHEvent)
					m_pHEvent->OnUpdateFPL(FplId);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets a flight plan ID associated to the callsign given as parameter
/// 
///  RETURNS: 
///		the flight plan ID associated to the callsign
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CFplMngr::GetFpl(QString Cls)
{
	for (int fpl=0;fpl<m_FplTable.GetSize();fpl++)
	{
		if (m_FplTable[fpl])
		{
			if (m_FplTable[fpl]->m_Arcid.compare(Cls)==0)
				return fpl;
		}
	}
	return -1;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets a flight plan associated to the SSR code given in argument
/// 
///  RETURNS: 
///		the flight plan associated to the ssr code
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CFplMngr::RemoveFromTrack(int trknum)
{
	if (trknum!=-1)
	{
		CFlightPlan* pFPL = m_FplTable[trknum];
		if (pFPL)
		{
			m_FplTable[trknum]=NULL;
			if (m_pHEvent)
				m_pHEvent->OnEraseFPL(trknum, pFPL);
			delete pFPL;
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called to fill the different FPL Lists with the created FPL
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CFplMngr::FillFPLLists()
{
	//We first check apply the FPL Ageing process
	FPLAgingCkeck();

	//If the FPl List display is frozen, the content of the lists is not modified
	if (!m_Frozen)
	{
		for (int fpl=0;fpl<m_FplTable.GetSize();fpl++)
		{
			if (m_FplTable[fpl])
			{
				CouplingData* pCouplinData = CAsdApp::GetApp()->GetCouplingMngr()->FindCouplingDataByFplID((m_FplTable[fpl]->m_FPLNb));
				if (pCouplinData)
				{
					CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(pCouplinData->m_Sic, pCouplinData->m_Sac, pCouplinData->m_TrkNum);
				
					//If the FPL is linked to no Track, it is added to the BIN FPL List
					//Oterwise it is added in the Full FPL List then to the list corresponding 
					//to its association with the Track
					if (pTrack)
					{
						if (!m_FplTable[fpl]->m_PresenceInListTable[Full])
						{
							//FPL added to the Full FPL List if not already added
							m_FplTable[fpl]->m_PresenceInListTable[Full] = true;
							CViewFpl* pElem=new CViewFpl();
							pElem->Init(m_FplTable[fpl],m_pFplFullList);
							pElem->SetFont(m_iFontSize);
							m_FplTable[fpl]->m_pElemTable.Add(pElem);
							m_pFplFullList->AddElement(pElem);
						}

							//FPL added to the Coupled FPL List
						if (pCouplinData->m_Tag==COUPLED && !m_FplTable[fpl]->m_PresenceInListTable[Coupled])
						{
							m_FplTable[fpl]->m_PresenceInListTable[Coupled] = true;
							//Removed from Mode S Tagged FPL List if was in it
							if (m_FplTable[fpl]->m_PresenceInListTable[ModeS])
							{
								m_FplTable[fpl]->m_PresenceInListTable[ModeS]= false;
								bool found = false;
								for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
								{
									found = m_pFplModeSList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
									if (found)
										m_FplTable[fpl]->m_pElemTable.RemoveAt(i);
								}
							}
							//Removed from Tagged FPL List if was in it
							if (m_FplTable[fpl]->m_PresenceInListTable[Tag])
							{
								m_FplTable[fpl]->m_PresenceInListTable[Tag]= false;
								bool found = false;
								for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
								{
									found = m_pFplTagList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
									if (found)
										m_FplTable[fpl]->m_pElemTable.RemoveAt(i);
								}
							}

							CViewFpl* pElem=new CViewFpl();
							pElem->Init(m_FplTable[fpl],m_pFplCoupledList);
							pElem->SetFont(m_iFontSize);
							m_FplTable[fpl]->m_pElemTable.Add(pElem);
							m_pFplCoupledList->AddElement(pElem);
						}

							//FPL added to the Mode S Tagged FPL List
						if (pCouplinData->m_Tag==MODESTAGGED && !m_FplTable[fpl]->m_PresenceInListTable[ModeS])
						{
							m_FplTable[fpl]->m_PresenceInListTable[ModeS] = true;
							//Removed from Coupled FPL List if was in it
							if (m_FplTable[fpl]->m_PresenceInListTable[Coupled])
							{
								m_FplTable[fpl]->m_PresenceInListTable[Coupled]= false;
								bool found = false;
								for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
								{
									found = m_pFplCoupledList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
									if (found)
										m_FplTable[fpl]->m_pElemTable.RemoveAt(i);
								}
							}
							//Removed from Tagged FPL List if was in it
							if (m_FplTable[fpl]->m_PresenceInListTable[Tag])
							{
								m_FplTable[fpl]->m_PresenceInListTable[Tag]= false;
								bool found = false;
								for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
								{
									found = m_pFplTagList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
									if (found)
										m_FplTable[fpl]->m_pElemTable.RemoveAt(i);
								}
							}

							CViewFpl* pElem=new CViewFpl();
							pElem->Init(m_FplTable[fpl],m_pFplModeSList);
							pElem->SetFont(m_iFontSize);
							m_FplTable[fpl]->m_pElemTable.Add(pElem);
							m_pFplModeSList->AddElement(pElem);
						}

							//FPL added to the Tagged FPL List
						if (pCouplinData->m_Tag==TAGGED  && !m_FplTable[fpl]->m_PresenceInListTable[Tag])
						{
							m_FplTable[fpl]->m_PresenceInListTable[Tag] = true;
							//Removed from Coupled FPL List if was in it
							if (m_FplTable[fpl]->m_PresenceInListTable[Coupled])
							{
								m_FplTable[fpl]->m_PresenceInListTable[Coupled]= false;
								bool found = false;
								for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
								{
									found = m_pFplCoupledList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
									if (found)
										m_FplTable[fpl]->m_pElemTable.RemoveAt(i);
								}
							}
							//Removed from Mode S Tagged FPL List if was in it
							if (m_FplTable[fpl]->m_PresenceInListTable[ModeS])
							{
								m_FplTable[fpl]->m_PresenceInListTable[ModeS]= false;
								bool found = false;
								for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
								{
									found = m_pFplModeSList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
									if (found)
										m_FplTable[fpl]->m_pElemTable.RemoveAt(i);
								}
							}
							
							CViewFpl* pElem=new CViewFpl();
							pElem->Init(m_FplTable[fpl],m_pFplTagList);
							pElem->SetFont(m_iFontSize);
							m_FplTable[fpl]->m_pElemTable.Add(pElem);
							m_pFplTagList->AddElement(pElem);
						}
					}
					else
					{
						//FPL is added to BIN FPL List and removed from any other List
						if (!m_FplTable[fpl]->m_PresenceInListTable[Bin])
						{
							for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
							{
								m_pFplFullList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
								m_pFplCoupledList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
								m_pFplModeSList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
								m_pFplTagList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
							}
							for (int j=0;j<m_FplTable[fpl]->m_pElemTable.GetSize();j++)
							{
								m_FplTable[fpl]->m_pElemTable.RemoveAt(j);
							}

							m_FplTable[fpl]->m_PresenceInListTable[Bin] = true;
							CViewFpl* pElem=new CViewFpl();
							pElem->Init(m_FplTable[fpl],m_pFplBinList);
							pElem->SetFont(m_iFontSize);
							m_FplTable[fpl]->m_pElemTable.Add(pElem);
							m_pFplBinList->AddElement(pElem);
						}
					
					}
				}
				else
				{
					//FPL is added to BIN FPL List and removed from any other List if no Coupling data is available
					if (!m_FplTable[fpl]->m_PresenceInListTable[Bin])
					{
						for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
						{
							m_pFplFullList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
							m_pFplCoupledList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
							m_pFplModeSList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
							m_pFplTagList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
						}
						for (int j=0;j<m_FplTable[fpl]->m_pElemTable.GetSize();j++)
						{
							m_FplTable[fpl]->m_pElemTable.RemoveAt(j);
						}

						m_FplTable[fpl]->m_PresenceInListTable[Bin] = true;
						CViewFpl* pElem=new CViewFpl();
						pElem->Init(m_FplTable[fpl],m_pFplBinList);
						pElem->SetFont(m_iFontSize);
						m_FplTable[fpl]->m_pElemTable.Add(pElem);
						m_pFplBinList->AddElement(pElem);
					}
			
				}

				//Update all lists
				for (int k=0;k<m_FplTable[fpl]->m_pElemTable.GetSize();k++)
				{
					m_FplTable[fpl]->m_pElemTable[k]->OnUpdate();
				}

			}
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called to empty the different FPL Lists
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CFplMngr::EmptyAllLists()
{
	for (int fpl=0;fpl<m_FplTable.GetSize();fpl++)
	{
		if (m_FplTable[fpl])
		{
			for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
			{
				m_pFplFullList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
				m_pFplCoupledList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
				m_pFplModeSList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
				m_pFplTagList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
				m_pFplBinList->RemoveElement(m_FplTable[fpl]->m_pElemTable[i]);
			}
			for (int j=0;j<m_FplTable[fpl]->m_pElemTable.GetSize();j++)
			{
				m_FplTable[fpl]->m_pElemTable.RemoveAt(j);
			}
			for (int k=0;k<m_FplTable[fpl]->m_PresenceInListTable.GetSize();k++)
			{
				m_FplTable[fpl]->m_PresenceInListTable[k] = false;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called to Freeze or unfreeze the different FPL Lists
///		i.e. the text FROZEN is added in the FPL lists caption
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CFplMngr::FreezeAllLists(bool frozen)
{
	if (frozen)
	{
		m_Frozen=true;
		m_pFplFullList->setCaption("Full FPL List   ***** FROZEN *****");
		m_pFplCoupledList->setCaption("Coupled FPL List   ***** FROZEN *****");
		m_pFplModeSList->setCaption("Mode S Tagged FPL List   ***** FROZEN *****");
		m_pFplTagList->setCaption("Tagged FPL List   ***** FROZEN *****");
		m_pFplBinList->setCaption("BIN FPL List   ***** FROZEN *****");
	}
	else
	{
		m_pFplFullList->setCaption("Full FPL List");
		m_pFplCoupledList->setCaption("Coupled FPL List");
		m_pFplModeSList->setCaption("Mode S Tagged FPL List");
		m_pFplTagList->setCaption("Tagged FPL List");
		m_pFplBinList->setCaption("BIN FPL List");
		RemoveAll();
		m_Frozen=false;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the new font size selected by user for all FPL Lists
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CFplMngr::SetFontForAllEntries(int Size)
{
	m_iFontSize=Size;
	for (int fpl=0;fpl<m_FplTable.GetSize();fpl++)
	{
		if (m_FplTable[fpl])
		{
			for (int i=0;i<m_FplTable[fpl]->m_pElemTable.GetSize();i++)
			{
				m_FplTable[fpl]->m_pElemTable[i]->SetFont(Size);
			}
		}
	}
	m_pFplFullList->UpdateScrollBarSize();
	m_pFplCoupledList->UpdateScrollBarSize();
	m_pFplModeSList->UpdateScrollBarSize();
	m_pFplTagList->UpdateScrollBarSize();
	m_pFplBinList->UpdateScrollBarSize();

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns the font size currently applied for all FPL Lists
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CFplMngr::GetListsFontSize()
{
	return m_iFontSize;
}


//-----------------------------------------------------------------------------
//
/// \class CFlightPlan
/// This class is used to store the attributes of the flight plan 
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CFlightPlan::CFlightPlan(int FPLNb)
{
	m_CurFixOccur=1;
	m_FPLNb=FPLNb;

	m_DataTable.SetSize(6);
	m_PresenceInListTable.SetSize(5);
	for (int i=0;i<m_PresenceInListTable.GetSize();i++)
	{
		m_PresenceInListTable[i]=false;
	}


}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the copy constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CFlightPlan::CFlightPlan(CFlightPlan& pFpl)
{
	m_CurFixOccur=pFpl.m_CurFixOccur;
	m_FPLNb=pFpl.m_FPLNb;

	m_DataTable.SetSize(6);
	m_PresenceInListTable.SetSize(5);
	for (int i=0;i<m_PresenceInListTable.GetSize();i++)
	{
		m_PresenceInListTable[i]=false;
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CFlightPlan::~CFlightPlan()
{
	CFplMngr* pFplMngr=CAsdApp::GetApp()->GetFplMngr();
	for (int i=0;i<m_pElemTable.GetSize();i++)
	{
		bool delet=false;
		delet = pFplMngr->m_pFplFullList->RemoveElement(m_pElemTable[i]);
		delet = pFplMngr->m_pFplCoupledList->RemoveElement(m_pElemTable[i]);
		delet = pFplMngr->m_pFplModeSList->RemoveElement(m_pElemTable[i]);
		delet = pFplMngr->m_pFplTagList->RemoveElement(m_pElemTable[i]);
		delet = pFplMngr->m_pFplBinList->RemoveElement(m_pElemTable[i]);
	}
	for (int j=0;j<m_pElemTable.GetSize();j++)
	{
		m_pElemTable.RemoveAt(j);
	}

}








//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the flight plan data with the received ones in
///		argument
/// 
///  RETURNS: 
///		true if the flight plan data has changed
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CFlightPlan::UpdateDataFromTrack(QString CsCallSign, int Sic, int Sac, int TrkNum)
{
	CInfoMngr* pInfoMngr=CAsdApp::GetApp()->GetInfoMngr();
	m_LastReceivedTime=QDateTime::currentDateTime();
	bool ret = false;
	m_FPLNb=TrkNum;

	CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(Sic, Sac, TrkNum);
	// find associated track
	if (pTrack)
	{
		QString trackssr;
		QString afl;
		if (pTrack->m_TD.m_CurAlt==GARBLED_INVALID)
			afl = "****";
		else
		{
			afl = pInfoMngr->GetCorrectedAltitude(pTrack->GetPos(),pTrack->m_TD.m_CurAlt);
			if (afl.left(1) == "-")
				afl = afl.left(4);

		}

		if (pTrack->m_TD.m_SSR != -1)
			if (pTrack->m_TD.m_SSR==GARBLED_INVALID)
				trackssr.sprintf("****");
			else
				trackssr.sprintf("%.4o",pTrack->m_TD.m_SSR);
		else
			trackssr.sprintf("----");

		if (m_Arcid!=CsCallSign)
			ret = true;
		if (m_Ssr!=trackssr)
			ret = true;
		if (m_ArcTyp!=pTrack->m_TD.m_Arctyp)
			ret = true;
		if (m_Cfl!=pTrack->m_TD.m_Cfl)
			ret = true;
		if (m_Afl!=afl)
			ret = true;
		if (m_Ades!=pTrack->m_TD.m_Ades)
			ret = true;

		//Update with received data 
		m_Arcid=CsCallSign;
		m_Ssr=trackssr;
		m_ArcTyp=pTrack->m_TD.m_Arctyp;
		m_Cfl=pTrack->m_TD.m_Cfl;
		m_Afl=afl;
		m_Ades=pTrack->m_TD.m_Ades;
		m_Wtc=pTrack->m_TD.m_Wtc;
	
	}

	m_DataTable[0]=m_Arcid;
	m_DataTable[1]=m_Ssr;
	m_DataTable[2]=m_ArcTyp;
	m_DataTable[3]=m_Cfl;
	m_DataTable[4]=m_Afl;
	m_DataTable[5]=m_Ades;

	for (int i=0;i<m_pElemTable.GetSize();i++)
	{
		m_pElemTable[i]->OnUpdate();
	}

	
	return ret;
}



//-----------------------------------------------------------------------------
//
/// \class CViewFpl
///  This class displays the line view
///  
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the constructor
/// 
///  RETURNS: void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CViewFpl::CViewFpl()
{
 m_Select = false;
 m_Line=-1;
 
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the destructor
/// 
///  RETURNS:
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CViewFpl::~CViewFpl()
{
 for (int fld=0;fld<m_FieldTable.GetSize();fld++)
  delete m_FieldTable[fld];
 if (((CLineView*)m_pView)->GetElementNb()==0)
  ((CLineView*)m_pView)->IconifyView(1);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method update the field of the line view according 
///  to the data line view
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::OnUpdate()
{
 for (int col=0;col<m_FieldDef.GetSize();col++)
 {
  if (col<m_FieldTable.GetSize())
   m_FieldTable[col]->SetData(m_pFpl->m_DataTable[m_FieldDef[col].DataNb]);

 }
 m_pView->OnUpdate(NULL,0,this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method draws the line and manages the drawing of each line fields 
///  Argument1: pDC is a pointer of the type QPainter
///  Argument2: Line is the line number which must be drawn
///  Argument3: Col is the column number which must be drawn
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::OnDraw(QPainter* pDC,int Line,int Col)
{
 
 if(m_Select)
 {
  QRect rect = m_pView->GetDrawRect(Line,-1);
  int color=CColorMngr::GetColorNb("SELECTED_BKGRND");
  pDC->fillRect(rect.left(),rect.top(),rect.width(),rect.height(),CColorMngr::GetColor(color));
 }

 
 if(Col==-1)
 {
  for (int col=0;col<m_FieldTable.GetSize();col++)
  {
    m_FieldTable[col]->OnDraw(pDC,Line,col);
  }
 }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method sets the line to the position in argument
///  Argument: new line number
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::SetLine(int Line)
{
	m_Line = Line;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method sets the line of an alert list in a selected status
///  Argument: select is a boolean
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::SetSelectedElement(bool select)
{
 m_Select = select;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method manages the selected track from the Radar Window and 
///  the selected element from the list line.
///  Argument: bSelect is equal to true, the line is selected,
///      is equal to false, the line is deselected.
/// 
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::SelectElement(bool select)
{

	if(m_Line!=-1)
	{
		m_Select = select;
		m_FirstElemSelected=select;
	}

  m_pView->OnUpdate(NULL,0,NULL);
 
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method initializes the line. It creates the line fields.
///  Argument1: pData is a pointer of type CAlarmInt
///  Argument2: pView is a pointer of type CLineView
///  Argument2: pParent is a pointer of type CDisplayElement. It is not used.
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::Init(LPVOID pData,CView *pView,CDisplayElement* pParent)
{
	m_pView = (CLineView*)pView;
	m_pFpl = (CFlightPlan*)pData;

	CFplField* pField=NULL;
	for (int i=0;i<m_FieldDef.GetSize();i++)
	{
		pField = new CFplField;
		pField->Init(&m_FieldDef[i],pView,this);
		m_FieldTable.Add(pField);
	}

	if (((CLineView*)m_pView)->IsIconified())
		((CLineView*)m_pView)->IconifyView(-1);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method returns the rectangle size of line field
///  Argument: Col is the column number
///
///  RETURNS:
///  returns the rectangle size of line field 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
QSize CViewFpl::GetSize(int Col)
{
 if (Col<m_FieldTable.GetSize())
  return m_FieldTable[Col]->GetSize(Col);
 else
  return QSize(0,0);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method allows to sort the list. It always returns true because the
///  new element is always add to the top of the list.
///
///  RETURNS:
///  true
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CViewFpl::FplCompare(long lParam1,long lParam2,long lParamSort,CView* pView)
{
 int result;
 CViewFpl* pFpl1=(CViewFpl*)lParam1,*pFpl2=(CViewFpl*)lParam2;
 result= (pFpl1->m_FieldTable[0]->m_Data>=pFpl2->m_FieldTable[0]->m_Data);

 return result;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is called when the user clicks mouse AB 
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdInt010, CIAsdFctAlwMsa015, CIAsdFctAlwApw015
//
//-----------------------------------------------------------------------------
void CViewFpl::OnLButtonDown(UINT nFlags,QPoint point)
{


}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is called when the user clicks mouse SB 
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdInt010
//
//-----------------------------------------------------------------------------
void CViewFpl::OnRButtonDown(UINT nFlags,QPoint point)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is called when the user clicks mouse WB 
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdInt010, CIAsdFctAlwStc035
//
//-----------------------------------------------------------------------------
void CViewFpl::OnMButtonDown(UINT nFlags,QPoint point)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method sets the field color on an alert list 
///  Argument1: iIndex is a field index 
///  Argument2: iColor is a color index 
///
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::SetColor(int iIndex, int iColor)
{
 if(iIndex==-1)
 {
  for(int i=0;i<m_FieldDef.GetSize();i++)
  {
   m_FieldDef[i].ColorNb = iColor;
   if(i!=0)
    m_FieldDef[i].SelectColorNb = iColor;
  }
 }
 else
 {
  if(iIndex<m_FieldDef.GetSize() && iIndex>=0)
  {
   m_FieldDef[iIndex].ColorNb = iColor;
   if(iIndex!=0)
    m_FieldDef[iIndex].SelectColorNb = iColor;
  }
 }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method changes the font size in an alert list
///  Argument: iSize is a size index:
///     - iSize==1: for the small font size
///     - iSize==2: for the medium font size
///     - iSize==3: for the big font size
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::SetFont(int iSize)
{
 if(iSize>=0 && iSize<=3)
 {
  for(int i=0;i<m_FieldDef.GetSize();i++)
  {
   if (i==0)
    m_FieldDef[i].FontNb = iSize;
   else
    m_FieldDef[i].FontNb = iSize+3;
  }
 }
 m_pView->OnUpdate(NULL,0,NULL);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method initializes the field elements of the STCA list and
///  loads the fonts.
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::InitTables()
{
 QFont font=CFontMngr::GetFontByName("SMALL_CS_AFS_FPLLIST");
 m_FontTable.Add(font);
 font=CFontMngr::GetFontByName("MEDIUM_CS_AFS_FPLLIST");
 m_FontTable.Add(font);
 font=CFontMngr::GetFontByName("BIG_CS_AFS_FPLLIST");
 m_FontTable.Add(font);
 font=CFontMngr::GetFontByName("SMALL_NOT_CS_AFS_FPLLIST");
 m_FontTable.Add(font);
 font=CFontMngr::GetFontByName("MEDIUM_NOT_CS_AFS_FPLLIST");
 m_FontTable.Add(font);
 font=CFontMngr::GetFontByName("BIG_NOT_CS_AFS_FPLLIST");
 m_FontTable.Add(font);


 FplFieldDef def;
 def.ColorNb=InfoClr;
 def.SelectColorNb = CColorMngr::GetColorNb("SELECTED_FRGRND");
 def.FontNb=0;
 def.Style = Qt::AlignLeft;
 def.DataNb=0;
 m_FieldDef.Add(def);

 def.FontNb=3;
 def.ColorNb=InfoClr;
 def.DataNb=1;
 m_FieldDef.Add(def);

 def.DataNb=2;
 m_FieldDef.Add(def);

 def.DataNb=3;
 m_FieldDef.Add(def);

 def.DataNb=4;
 m_FieldDef.Add(def);

 def.DataNb=5;
 m_FieldDef.Add(def);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method removes the fonts used in the alert list
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CViewFpl::RemoveTables()
{
 m_FontTable.RemoveAll();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method returns the font in the m_FontTable at the index put in Argument.
///
///  RETURNS:
///  QFont a font
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
QFont CViewFpl::GetFontTableAt(int index)
{
 return m_FontTable[index];
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method returns the m_Select attribute.
///
///  RETURNS:
///  bool: whether the alarm is selected or not
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
bool CViewFpl::GetSelect()
{
 return m_Select;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method returns the m_FirstElemSelected attribute.
///
///  RETURNS:
///  bool: whether an element in the alert list is selected
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
bool CViewFpl::GetFirstElemSelected()
{
 return m_FirstElemSelected;
}

//-----------------------------------------------------------------------------
//
/// \class CFplField
///  This class displays the field of the line view
///  
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the constructor
///
///  RETURNS:
///
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CFplField::CFplField()
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is destructor
///
///  RETURNS:
///
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CFplField::~CFplField()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method draws a field of the line
///  Argument1: pDC is a pointer of the type QPainter
///  Argument2: Line is the line number which must be drawn
///  Argument3: Col is the column number which must be drawn
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplField::OnDraw(QPainter* pDC,int Line,int Col)
{
	CouplingData* pcpld = NULL;
	if (m_pParent->m_pFpl)
		pcpld = CAsdApp::GetApp()->GetCouplingMngr()->FindCouplingDataByFplID(m_pParent->m_pFpl->m_FPLNb);


	QRect rect = m_pView->GetDrawRect(Line,Col);
	pDC->setFont(m_pParent->GetFontTableAt(m_pFieldDef->FontNb));
	if (m_pParent->GetSelect())
	{
		if(m_pParent->GetFirstElemSelected())
			pDC->setPen(CColorMngr::GetColor(m_pFieldDef->SelectColorNb));
		else
			pDC->setPen(CColorMngr::GetColor(m_pFieldDef->ColorNb));
	}
	else
	{
		switch (Col)
		{
		case 0 : //Callsign
			pDC->setPen(CColorMngr::GetColor(m_pFieldDef->ColorNb));
			if (pcpld != NULL)
			{
				CTrack* pTrack = CAsdApp::GetApp()->GetTrackMngr()->GetTrack(pcpld->m_Sic, pcpld->m_Sac, pcpld->m_TrkNum);

				if (pcpld->m_Tag==MODESTAGGED)
					pDC->setPen(CColorMngr::GetColor(CColorMngr::GetColorNb("FPL_MODESTAG")));
				if (pcpld->m_Tag==TAGGED)
					pDC->setPen(CColorMngr::GetColor(CColorMngr::GetColorNb("FPL_TAG")));
				if (pcpld->m_Tag==COUPLED)
					pDC->setPen(CColorMngr::GetColor(CColorMngr::GetColorNb("FPL_COUPLED")));
				if (pTrack)
				{
					if (pTrack->IsAoc())
						pDC->setPen(CColorMngr::GetColor(CColorMngr::GetColorNb("ASSUMED")));
				}
			}
			break;
		case 1 ://SSR code
			pDC->setPen(CColorMngr::GetColor(m_pFieldDef->ColorNb));
			if (m_Data == "****")
				pDC->setPen(CColorMngr::GetColor(CColorMngr::GetColorNb("WARNING")));
			break;
		case 4 ://AFL
			pDC->setPen(CColorMngr::GetColor(m_pFieldDef->ColorNb));
			if (m_Data == "****")
				pDC->setPen(CColorMngr::GetColor(CColorMngr::GetColorNb("WARNING")));
			break;
		default :
			pDC->setPen(CColorMngr::GetColor(m_pFieldDef->ColorNb));
			break;
		}
	
	}
	pDC->drawText(rect,m_pFieldDef->Style,m_Data);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method initializes the line field.
///  Argument1: pData is a pointer of type CAlarmInt
///  Argument2: pView is a pointer of type CLineView
///  Argument3: pParent is a pointer of type CViewFpl.
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplField::Init(LPVOID pData,CView *pView,CDisplayElement* pParent)
{
 m_pView = (CLineView*)pView;
 m_pFieldDef = (CViewFpl::FplFieldDef*)pData;
 m_pParent = (CViewFpl*)pParent;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method returns the rectangle size of line field
///  Argument: Col is the column number
///
///  RETURNS:
///  returns the rectangle size of line field
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
QSize CFplField::GetSize(int Col)
{
 QPainter dc(m_pView);
 dc.setFont(m_pParent->GetFontTableAt(m_pFieldDef->FontNb));

 QSize size = dc.boundingRect(0,0,0,0,m_pFieldDef->Style,m_Data).size();
 return size;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method sets m_Data attribute
///
///  RETURNS:
///  void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFplField::SetData(QString data)
{
 m_Data = data;
}

//--------------------------------< THE END >----------------------------------
