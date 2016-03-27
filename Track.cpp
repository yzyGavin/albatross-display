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
//  FILENAME: Track.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains all the classes aiming at managing the display of the
//		tracks.
//		Each graphical element of a track is identified by its own class: Leader
//		Line, Data Block, Graphical Route Legs, Present Position Symbols, Afterglows
//		Halo and Prediction Vector. 
//		It also includes the management of the track server, the type of datablock
//		and data fields to be displayed, the interactions with data fields or 
//		track elements.
//				
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
#include "View.h"
#include "AsdView.h"
#include "DisplayElement.h"
#include "Track.h"
#include "RwdView.h"
#include "math.h"
#include "Elements.h"
#include "ColorMngr.h"
#include "FontMngr.h"
#include "DataMngr.h"
#include "./CommonFiles/IniFile.h"
#include "Geo.h"
#include "FlSelectOther.h"
#include "AsdApp.h"
#include "Asterix.h"
#include "RwdView.h"
#include "InfoMngr.h"
#include "UserMngr.h"
#include "FlSelect.h"
#include "Tssr.h"
#include "TssrOther.h"
#include "TopLevel.h"
#include "WfAttrib.h"
#include "WdwMngr.h"
#include "Interface.h"
#include "Wtc.h"
#include "ArcTyp.h"
#include "SKMessageBox.h"
#include "Ades.h"
#include "Assr.h"
#include "VisuMap.h"
#include "CommonDataTypes.h"
#include <qlineedit.h> 
#include "LineView.h"
#include "FlightPlan.h"
#include "CouplingMngr.h"

//-GLOBAL DATA-----------------------------------------------------------------

#define DECALRADARX 0//-24000 // en 64 de NM
#define DECALRADARY 0


#define IDC_HIGHLIGHT 101
#define IDC_UNHIGHLIGHT 102
#define IDC_FORCE 103
#define IDC_UNFORCE 104
#define IDC_COUPLE 105
#define IDC_ASSUME 111
#define IDC_UNASSUME 112
#define IDC_UNTAG 114
#define IDC_TAG 115
#define IDC_REMOVE_FROM_LIST 116

#define IDC_TRKM 302
#define IDC_HALO 303

#define IDC_ARCID1_MISMATCH 401
#define IDC_ARCID2_MISMATCH 402

bool CTrack::m_Glob_ObjectDeletedOnModal = false;

bool m_GlobalFlashing=false;
bool m_GlobalSpiFlashing=false;


CArray<QFont,QFont> CViewTrack::m_FontTable;
CArray<unsigned short,unsigned short> CViewTrack::m_ColorTable;
TrackDef* CViewTrack::m_pTrackDef;

#define ID_TRACKMENU 1000


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CTrackServer
/// This class is used to manage the track servers and radar services: availability
/// range, period, type...  
// 
//
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackServer::CTrackServer()
: m_Start252(false)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CTrackServer&, CTrackServer object to be initialized
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackServer::CTrackServer(CTrackServer& trk)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackServer::~CTrackServer()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the status of the radar service.
/// 
///  RETURNS: 
///  bool, whether the radar service is available or not
///
///  REQUIREMENT ID: CIAsdItfComGen120
//
//-----------------------------------------------------------------------------
bool CTrackServer::GetState()
{
	//If the asterix message of Cat 252 is not valid the track server is not available
	if (!m_Src.GetState())
	{
		m_Start252 = false;
		return false;
	}
	//If no message of the Cat 252 has been received the track server is not available
	else if (!m_Start252)
		return false;
	//If no message of the Cat 252 has been received for an offline defined time, the 
	//track server is not available.
	else if ( abs(m_LastReceived252.toTime_t()-QDateTime::currentDateTime().toTime_t()) >= CAsdApp::GetApp()->getRadarDiscard() )
	{
		m_Start252 = false;
		return false;
	}
	//In the other case the track server is available
	return true;
}


//-----------------------------------------------------------------------------
//
/// \class CTagArray
/// This class is used to fill with a single number identification a 
/// callsign tag when selected from the Create TSSR pop-up window.
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTagArray::CTagArray()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CTagArray&, CTagArray object to be initialized
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTagArray::CTagArray(CTagArray& tagArray)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTagArray::~CTagArray()
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is used to complete automatically the callsign that are selected 
///  in the Create TSSR pop-up window. Those callsign tags are completed with 2 digits,
///  determined by incrementation of last attributed number or by a free identification.
///  Argument : QString, a callsign without identification
/// 
///  RETURNS: 
///  QString, the completed callsign tag
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
QString CTagArray::FindTag(QString Arcid)
{
	int FplNbr=1;
	QString res;
	//The first available number is defined as the identification of the callsign tag
	for (FplNbr=1;FplNbr<GetSize();FplNbr++)
		if (!(*this)[FplNbr])
			break;
	res.sprintf("%s%.2d",Arcid.ascii(),FplNbr);
	return res;
}



//-----------------------------------------------------------------------------
//
/// \class CTrackMngr
/// This class is used to manage the track environment: track source, available 
/// radar services, automatic or manual selection of radar services, menus associated to
/// tracks, pop-ups, track display options...
/// This class contains a table of all the available tracks.  
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of the member parameters
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackMngr::CTrackMngr()
: m_InitTrackServer(true), m_pHEvent(NULL)
{
	m_bDialogIsOn = false;
	m_pMenuCallingTrack = NULL;
	m_pMenuCallingWnd = NULL;
	m_pTrackMenu=NULL;
	m_pKeyListMenu=NULL;

	m_pTrkSrc=NULL;
	m_RocdUnit=true;
	m_SwitchTime = QDateTime::currentDateTime();
	m_pTagServer = NULL;
	int i;
	for (i='A'; i <= 'Z'; i++)
	{
		m_pServers[i - 'A'] = NULL;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CTrackMngr&, CTrackMngr object to be initialized
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackMngr::CTrackMngr(CTrackMngr& TrackMngr)
{
	m_pTrackMenu=NULL;
	m_pKeyListMenu=NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackMngr::~CTrackMngr()
{
	//All track objects, menus and pop-ups are destroyed.
	RemoveAllTracks();
	RemoveMenu();
	CWdwMngr::RemoveWindow(m_pTssrDialog);
	CWdwMngr::RemoveWindow(m_pFlSelectOtherDialog);
	CWdwMngr::RemoveWindow(m_pFlSelectDialog);
	CWdwMngr::RemoveWindow(m_pTssrOtherDialog);
	CWdwMngr::RemoveWindow(m_pWtcDialog);
	CWdwMngr::RemoveWindow(m_pATypDialog);
	CWdwMngr::RemoveWindow(m_pAdesDialog);
	CWdwMngr::RemoveWindow(m_pAssrDialog);
	CWdwMngr::RemoveWindow(m_pRwyDialog);

	CViewTrack::RemoveTables();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Init function of the class. Definition of timers, menu and pop-ups.
///  Argument 1 : CTrackEventStub, event that allows to link tracks with coupling
///  information, safety information...
///  Argument 2 : int, periodicity of the CTTT message
/// 
///  RETURNS: 
///  bool, returns true to indicate the init phase was successful.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrackMngr::Init(CTrackEventStub* pHEvent, int TTTCycle)
{
	m_TrackList.SetSize(256, 256, 4096, TTTCycle, 3000, 0, 2000);
	//Initialization of the radar services
	for (int i='A'; i <= 'Z'; i++)
	{
		CTrackServer* pts = m_pServers[i - 'A'];
		if (pts)
		{
			if (!m_TrackList.IsAllowed(pts->m_Sic, pts->m_Sac))
				m_TrackList.Allow(pts->m_Sic, pts->m_Sac);
			pts->m_Src.SetMngr(this);
			pts->m_Src.CreateRcvSocket();
		}
	}

	m_LastSyncTime = QDateTime(QDate(2000,1,1));

	//Initialization of timers
	m_TrkAgeId=startTimer(1000);
	m_FlashingId=startTimer(1000);
	m_FlashingSpiId=startTimer(333);
	m_DuplicatSsrId=startTimer(3000);
	m_pHEvent = pHEvent;

	//Initialization of pop-ups
	m_pTssrDialog = new CTssrDlg;
	m_pFlSelectOtherDialog = new CFlSelectOther;
	m_pFlSelectDialog = new CFlSelectDlg;
	m_pTssrOtherDialog = new CTssrOtherDlg;
	m_pATypDialog = new CATypDlg;
	m_pAdesDialog = new CAdesDlg;
	m_pAssrDialog = new CAssr;
	m_pWtcDialog = new CWtcDlg;
	m_pRwyDialog = new CRwyDlg;

	//setting of pop-up attributes
	CWdwMngr::AddWindow(m_pTssrDialog,"POPUP",CWFAttrib::POPUP, false);
	CWdwMngr::AddWindow(m_pFlSelectOtherDialog,"POPUP",CWFAttrib::POPUP, false);
	CWdwMngr::AddWindow(m_pFlSelectDialog,"POPUP",CWFAttrib::POPUP, false);
	CWdwMngr::AddWindow(m_pTssrOtherDialog,"POPUP",CWFAttrib::POPUP, false);
	CWdwMngr::AddWindow(m_pWtcDialog,"POPUP",CWFAttrib::POPUP, false);
	CWdwMngr::AddWindow(m_pATypDialog,"POPUP",CWFAttrib::POPUP, false);
	CWdwMngr::AddWindow(m_pAdesDialog,"POPUP",CWFAttrib::POPUP, false);
	CWdwMngr::AddWindow(m_pAssrDialog,"POPUP",CWFAttrib::POPUP, false);
	CWdwMngr::AddWindow(m_pRwyDialog,"POPUP",CWFAttrib::POPUP, false);
	
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to retrieve the offline defined information needed
///  by the CTrackMngr class (definition of radar services).
///  Argument 1 : CIniFile, file to be read
///  Argument 2 : CInterfaceList, list of available interfaces
///  Argument 3 : QString, message to be logged in case of problem with one 
///  parameter retrieval
/// 
///  RETURNS: 
///  bool, returns true to indicate the offline defined information was correct.
///
///  REQUIREMENT ID: CIAsdItfHwi005
//
//-----------------------------------------------------------------------------
bool CTrackMngr::Read(CIniFile& IniFile, CInterfaceList& itfs, QString& Error)
{
	CString KeyRead;
	int i;
	char serid;
	CString KeyName;
	QString CurArg;
	CArray<QString,QString> ReturnedStringArray;
	QString strSection = "RADAR_MENU";
	// Read the radar services
	IniFile.GetIniProfileSection(strSection.ascii(), ReturnedStringArray);
	if (ReturnedStringArray.GetSize() > 0)
	{
		QString strPrioScale;
		QString strProperty = "PRIORITY_SCALE";
		int iKeyRead=IniFile.GetIniProfileString(strSection.ascii(), strProperty.ascii(), strPrioScale);
		if (iKeyRead)
			m_ServPrio=strPrioScale;
		QString strMenuRadarService;
		strProperty = "RADAR_LIST";
		iKeyRead=IniFile.GetIniProfileString(strSection.ascii(), strProperty.ascii(), strMenuRadarService);
		if (iKeyRead)
			m_ServMenu=strMenuRadarService;
	}
	else
	{
		Error = "No Radar Menu Definition";
		return false;
	}
	

	//Initialization of offline defined radar services
	for (i='A'; i <= 'Z'; i++)
	{
		serid = (char) i;
		if (m_ServMenu.find(serid) != -1)
		{
			m_pServers[i - 'A'] = NULL;
			CurArg="";
			KeyName.sprintf("%c",i);
			int len=IniFile.GetIniProfileString("RADAR_SERVICE",KeyName,KeyRead);
			if (len)
			{
				int nbarg=KeyRead.GetNbrOfArgument('|');
				if (nbarg==8)
				{
					CTrackServer* pts = new CTrackServer[1];
					//Get the ident char
					pts->m_Serid=(char) i;
					pts->m_Src.m_Serid = pts->m_Serid;
					//Get the radar service name
					KeyRead.GetArgumentNb(1,'|',CurArg);
					pts->m_Name=CurArg;
					//Get the sic
					KeyRead.GetArgumentNb(2,'|',CurArg);
					CurArg = CurArg.stripWhiteSpace();
					pts->m_Sic = atoi(CurArg.ascii());
					//Get the sac
					KeyRead.GetArgumentNb(3,'|',CurArg);
					CurArg = CurArg.stripWhiteSpace();
					pts->m_Sac = atoi(CurArg.ascii());
					//Get the update periodicity
					KeyRead.GetArgumentNb(4,'|',CurArg);
					CurArg = CurArg.stripWhiteSpace();
					pts->m_Period = atoi(CurArg.ascii());
					//Get the radar service position and range
					KeyRead.GetArgumentNb(5,'|',CurArg);
					CurArg = CurArg.stripWhiteSpace();
					pts->m_Centre = CurArg;
					pts->m_Dx=0;
					pts->m_Dy=0;
					pts->m_Rayon = 256;
					//Get the IP address
					KeyRead.GetArgumentNb(6,'|',CurArg);
					CurArg = CurArg.stripWhiteSpace();
					int ok=DecodeAddr(pts->m_Src.m_Addr, CurArg, "radar service " + pts->m_Name, Error);
					if (!ok) 
						return false;
					//Get the port number
					KeyRead.GetArgumentNb(7,'|',CurArg);
					CurArg = CurArg.stripWhiteSpace();
					ok=DecodePort(pts->m_Src.m_Port, CurArg, "radar service " + pts->m_Name, Error);
					if (!ok)
						return false;
					//Get the connection interface
					KeyRead.GetArgumentNb(8,'|',CurArg);
					CurArg = CurArg.stripWhiteSpace();
					pts->m_Src.m_pInterface=itfs.Get(CurArg);
					pts->m_Src.m_TimeOut = TIMEOUT_RADAR;
					if (!pts->m_Src.m_pInterface)
					{
						delete []pts;
						Error = "Interface not found for source "+ KeyName;
						return false;
					}
					m_pServers[i - 'A'] = pts;
				}
				else
				{
					Error = "Bad number of argument for radar source "+ KeyName;
					return false;
				}
			}
		}
	}
	int compt = 0;
	for (i='A'; i <= 'Z'; i++)
		if (m_pServers[i - 'A'])
			compt++;

	if (compt == 0)
	{
		Error = "No TRACK SOURCE Interface found";
		return false;
	}
	else
		return true;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to get the CTrackServer object (radar service), if 
///  any, having the name put in argument.
///  Argument : QString, name of the wanted radar service
/// 
///  RETURNS: 
///  CTrackServer, the CTrackServer object having the name put in argument.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackServer* CTrackMngr::GetServer(QString name)
{
	for (int i='A'; i <= 'Z'; i++)
	{
		CTrackServer* pts = m_pServers[i - 'A'];
		if (pts && pts->m_Name == name)
			return pts;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the CTrackServer object (radar service), if 
///  any, having the identification put in argument.
///  Argument : char, identification of the wanted radar service
/// 
///  RETURNS: 
///  CTrackServer, the CTrackServer object having the identification put in argument.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackServer* CTrackMngr::GetServer(char serid)
{
	return m_pServers[serid - 'A'];
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the CTrackServer object (radar service), if 
///  any, having the sic/sac info put in argument.
///  Argument 1 : int, sic value of the wanted radar service
///  Argument 2 : int, sac value of the wanted radar service
/// 
///  RETURNS: 
///  CTrackServer, the CTrackServer object having the sic/sac info put in argument.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackServer* CTrackMngr::GetServer(int sic, int sac)
{
	for (int i='A'; i <= 'Z'; i++)
	{
		CTrackServer* pts = m_pServers[i - 'A'];
		if (pts && pts->m_Sic == sic && pts->m_Sac == sac)
			return pts;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the CTrackServer object (radar service), if 
///  any, having the port number put in argument.
///  Argument : int, port number of the wanted radar service
/// 
///  RETURNS: 
///  CTrackServer, the CTrackServer object having the port number put in argument.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackServer* CTrackMngr::GetServer(int port)
{
	for (int i='A'; i <= 'Z'; i++)
	{
		CTrackServer* pts = m_pServers[i - 'A'];
		if (pts && pts->m_Src.m_Port == htons(port))
			return pts;
	}
	return NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to close any opened pop-up or menu.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrackMngr::RemoveMenu()
{
	CTripleEntryListIterator<CTrack> it( m_TrackList );
	CTrack* ptrack = (CTrack*) it.current();
	//Close any opened pop-ups. Pop-ups are associated to CTrack objects
	//m_pDialog2 is used when a pop-up is invoked from another pop-up.
	while ( ptrack != 0 ) 
	{
		++it;
		if (ptrack->m_pDialog)
		{
			ptrack->m_pDialog->close();
			m_bDialogIsOn = false;
		}
		if (ptrack->m_pDialog2)
		{
			ptrack->m_pDialog2->close();
			m_bDialogIsOn = false;
		}
		ptrack = (CTrack*) it.current();
	}
	//Close Radar menu
	CRwdView::RemoveAllRadarMenu();
	//Close Callsign menu or PPS menu or Mismatch menu
	if (m_pTrackMenu)
	{
		delete m_pTrackMenu;
		m_pTrackMenu = NULL;
	}
	//Close multiple STCA acknowledgement menu 
	if (m_pKeyListMenu)
	{
		delete m_pKeyListMenu;
		m_pKeyListMenu = NULL;
	}
	m_pMenuCallingTrack = NULL;
	m_pMenuCallingWnd = NULL;
	m_TrackSourceMenu.StopTracking(true);
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns true if the currently selected radar service is available.
/// 
///  RETURNS: 
///  bool, whether the currently radar service is available or not.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrackMngr::IsCurrentSourceAv()
{
	if (m_pTrkSrc)
		return m_pTrkSrc->GetState();
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns true if the coupling information is available for the 
///  currently selected radar service.
/// 
///  RETURNS: 
///  bool, whether coupling information is available for the currently selected 
///  radar service or not.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrackMngr::IsCurrentSourceAvCpl()
{
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function allows to know if one radar service is within the radar menu.
///  Argument : char, the ident of the radar service.
/// 
///  RETURNS: 
///  bool, whether the radar service is within the radar menu or not.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrackMngr::IsRadarInMenu(char serid)
{
	if (m_ServMenu.find(serid) != -1)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function allows to know if one radar service is within the offline
///  defined radar service priority scale.
///  Argument : char, the ident of the radar service.
/// 
///  RETURNS: 
///  bool, whether the radar service is within the radar service priority scale
///  or not.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrackMngr::IsRadarInPrioScale(char serid)
{
	if (m_ServPrio.find(serid) != -1)
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the currently selected radar service
/// 
///  RETURNS: 
///  CTrackServer, the currently selected radar service.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackServer* CTrackMngr::GetCurrentServer()
{
	return m_pTrkSrc;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function cancels the current radar service and builds a new one taking 
///  into account the Menu content and the priority scale put in argument.
///  Argument 1 : QString, the offline define radar menu content
///  Argument 2 : QString, the offline define radar priority scale
///  Argument 3 : bool, whether an automatic switch of radar service must be perfomed 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsRsv105, CIAsdFctRwsRsv061, CIAsdFctRwsRsv062
//
//-----------------------------------------------------------------------------
void CTrackMngr::SetSourcePriority()
{
	//During the initialization phase the current radar service (highest in the
	//preferance scale) must remain selected
	m_InitTrackServer = true;
	SetTrackSource(NULL);
	//Following a reconnect to FDP, if the currently selected radar service is 
	//available but does not have coupling info, an automatic switch of radar service
	//must be performed to the highest available radar service for which coupling info 
	//is available
	if (GetCurrentServer() && !IsRadarInMenu(GetCurrentServer()->m_Serid))
	{
		CTrackServer* pNextServ = SearchHiAvRdSv();
		SetTrackSource(pNextServ);
	}
	//Radar menu is emptied and newly built
	m_TrackSourceMenu.RemoveAll();
	for(int i = 0; i < m_ServMenu.length(); i++)
	{
		char serid = m_ServMenu[i].latin1();
		if (serid >= 'A' && serid <= 'Z')
		{
			CTrackServer* pts = GetServer(serid);
			if (pts)
			{
				int curindex = m_TrackSourceMenu.AddItem(pts->m_Name, ID_TRACKMENU+i);
				m_TrackSourceMenu.SetCommandObject(curindex,this);
			}
		}
		else
			m_TrackSourceMenu.InsertSeparator();
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the highest available radar service in the preferance 
///  scale (if any).
///   
///  RETURNS: 
///  CTrackServer, the highest available radar service.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrackServer* CTrackMngr::SearchHiAvRdSv()
{
	for(int j = 0; j < m_ServPrio.length(); j++)
	{
		char serid = m_ServPrio[j].latin1();
		if (serid >= 'A' && serid <= 'Z')
		{
			CTrackServer* pts = GetServer(serid);
			if (pts && pts->GetState())
				return pts;
		}
	}
	return NULL;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the radar menu object.
///   
///  RETURNS: 
///  CAsdMenu, the radar menu.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CAsdMenu* CTrackMngr::GetTrackMenu()
{
	return &m_TrackSourceMenu;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon the selection of an available item in the radar menu, the 
///  corresponding radar service becomes the currently selected radar service.
///  Argument 1 : UINT, selected menu item number
///  Argument 2 : CObject, not used in this function
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrackMngr::OnCommand(UINT id, CObject *pDisplayWnd)
{
	int TrkSource=id-ID_TRACKMENU;
	char serid = m_ServMenu[TrkSource].latin1();
	CTrackServer* pts = GetServer(serid);
	SetTrackSource(pts);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function computes the availability and the coupling info availability 
///  for the radar service menu items displayed in the radar menu. 
///  Argument : CCmdUI, menu item
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsRsv105, CIAsdFctRwsRsv015, CIAsdFctRwsRsv020
//
//-----------------------------------------------------------------------------
void CTrackMngr::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	UINT id=pCmdUI->GetID();
	int TrkSource=id-ID_TRACKMENU;
	char serid = m_ServMenu[TrkSource].latin1();
	CTrackServer* pts = GetServer(serid);;
	pCmdUI->SetCheck(pts==m_pTrkSrc);
	pCmdUI->Enable(pts->GetState());
	pCmdUI->SetTextColor(NULL);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the radar service put in argument as the currently selected 
///  radar service. The graphical impact of such a switch of radar service is
///  calculated. 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsRsv061, CIAsdFctRwsRsv062, CIAsdFctRwsRsv064, 
///					CIAsdFctRwsRsv066, CIAsdFctRwsRsv067, CIAsdSymSmbNrm230,
///					CIAsdSymSmbNrm080, CIAsdSymSmbNrm340, CIAsdSymSmbNrm020,
///					CIAsdSymSmbNrm365, CIAsdSymSmbNrm370, CIAsdSymSmbNrm375,
///					CIAsdSymSmbBsm135, CIAsdSymSmbBsm140, CIAsdSymSmbBsm145,
///					CIAsdSymSmbBsm150, CIAsdSymSmbBsm155, CIAsdSymSmbBsm170,
///					CIAsdSymSmbBsm175, CIAsdSymSmbBsm285, CIAsdSymSmbBsm290,
///					CIAsdSymSmbEsm150, CIAsdSymSmbEsm155, CIAsdSymSmbEsm235,
///					CIAsdSymSmbEsm240, CIAsdSymSmbEsm245, CIAsdSymSmbEsm250,
///					CIAsdSymSmbEsm255, CIAsdSymSmbErs165, CIAsdSymSmbErs170,
///					CIAsdSymSmbErs175, CIAsdSymSmbErs180, CIAsdSymSmbErs185					 
//
//-----------------------------------------------------------------------------
void CTrackMngr::SetTrackSource(CTrackServer* pts)
{
	CFplMngr* pFPLMngr=CAsdApp::GetApp()->GetFplMngr();
	CCouplingMngr* pCouplingMngr=CAsdApp::GetApp()->GetCouplingMngr();
	if (m_pTrkSrc!=pts)
	{
		if (m_pTrkSrc && pts)
		{
			if (pts->m_Sic != m_pTrkSrc->m_Sic || pts->m_Sac != m_pTrkSrc->m_Sac)
			{						
				RemoveAllTracks();
				CVector::RemoveTables();
				CTOORArrow::RemoveAllArrows();
				pFPLMngr->EmptyAllLists();
				pFPLMngr->RemoveAll();
				pCouplingMngr->RemoveAll();

			}
		}
		//When recovering from Frozen state, all tracks are newly displayed
		else if (!m_pTrkSrc)
		{
			RemoveAllTracks();
			CVector::RemoveTables();
			CTOORArrow::RemoveAllArrows();
			pFPLMngr->FreezeAllLists(false);
			pFPLMngr->EmptyAllLists();
			pFPLMngr->RemoveAll();
			pCouplingMngr->RemoveAll();
		}
		//The new radar service tracks are processed
		m_pTrkSrc = pts;
		m_pTagServer = pts;
		if (m_pTrkSrc)
		{
			m_InitTrackServer = false;
			m_OldSourceName = m_pTrkSrc->m_Name;
		}
		if (m_pHEvent)
			m_pHEvent->OnSelectRadarService(m_pTrkSrc);
		m_SwitchTime = QDateTime::currentDateTime();
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the name of the currently selected radar service, if 
///  any, the name of the previously selected radar service if the current display
///  is frozen in the other case. 
///   
///  RETURNS: 
///  QString, the name of the currently selected radar service.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
QString CTrackMngr::GetSourceName()
{
	if (m_pTrkSrc)
		return m_pTrkSrc->m_Name;
	else
		return m_OldSourceName;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to declare a new radar window. This radar window 
///  (CView object) is added to the list of managed radar windows, and a track
///  presentation (CViewTrack object) is added for this radar window for 
///  every track.  
///  Argument : CView, the new radar window
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrackMngr::AddView(CView* pView)
{
	m_ViewTable.Add(pView);
	CTripleEntryListIterator<CTrack> it( m_TrackList );
	CTrack* ptrack=NULL;
	ptrack = (CTrack*) it.current();
	while ( ptrack != 0 ) 
	{
		++it;
		CViewTrack* pvt = new CViewTrack();
		pvt->Init(ptrack, pView);
		ptrack->m_ElemTable.Add(pvt);
		((CRwdView*)pView)->AddElement(pvt);
		pvt->OnUpdate();
		ptrack = (CTrack*) it.current();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to remove a radar window. This radar window 
///  (CView object) is removed from the list of managed radar windows, and all the 
///  track presentations (CViewTrack object) associated to this radar window are
///  deleted.  
///  Argument : CView, the new radar window
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrackMngr::RemoveView(CView* pView)
{
	for (int view=0;view<m_ViewTable.GetSize();view++)
	{
		if (pView==m_ViewTable[view])
		{
			m_ViewTable.RemoveAt(view);

			CTripleEntryListIterator<CTrack> it( m_TrackList );
			CTrack* ptrack=NULL;
			ptrack = (CTrack*) it.current();
			while ( ptrack != 0 ) 
			{
				++it;
				delete ptrack->m_ElemTable[view];
				ptrack->m_ElemTable.RemoveAt(view);
				ptrack = (CTrack*) it.current();
			}
			break;
		}
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to update the presentation of all the tracks
///  across every radar window. 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrackMngr::UpdateAllViews()
{
	CRwdView::UpdateAllViews();
}







//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the track object corresponding to the sic/sac and 
///  track number put in argument.
///   
///  RETURNS: 
///  CTrack, track object if found, NULL in the other case
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrack* CTrackMngr::GetTrack(BYTE sic, BYTE sac, int TrackNb)
{
	if (m_TrackList.IsAllowed(sic, sac))
		return m_TrackList.Get(sic, sac, TrackNb);
	else
		return NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function allows to highlight the track which SSR code or 
///	 Callsign is provided in argument
///   
///  RETURNS: 
///  True, if a correpsonding track object is found, false otherwise
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrackMngr::FindTrack(QString SearchArcid)
{
	QString ssr;
	bool found = false;
	CTripleEntryListIterator<CTrack> it( m_TrackList );
	CTrack* pTrack=NULL;
	pTrack = it.current();
	

	while ( pTrack != 0 ) 
	{
		++it;
		ssr.sprintf("%.4o", pTrack->m_TD.m_SSR);
		//We check first if the input string is the SSR code
		if (SearchArcid.left(4) == ssr)
		{
			found = true;
		}
		else 
		{
			if (SearchArcid.length() < 7)
			{
				SearchArcid = SearchArcid + "       ";
				SearchArcid = SearchArcid.left(7);
			}
			if (pTrack->m_TD.m_Arcid == SearchArcid || pTrack->m_TD.m_CsTag == SearchArcid)
			{
				found = true;
			}
		}
		if (found == true)
		{
			//If a matching track is found it is highlighted
			pTrack->m_pHEvent->OnTrackHighlight(pTrack, true);
			//If the track is not visible in any Radar Window arrows shall indicate the direction in which the track can be found
			bool visible=false;
			for (int view=0;view<m_ViewTable.GetSize();view++)
			{
				if (pTrack->m_ElemTable[view]->m_BoundingRect.intersects(m_ViewTable[view]->rect())==true)
				{
					visible=true;
					break;
				}
			}
			if (!visible)
			{
				CTOORArrow::Create(pTrack);
			}
			
			return true;
		}
		else
			pTrack = it.current();
	}

	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the track object next to the position in the view put 
///  in argument.
///  Argument 1 : QPoint, the position where a track is searched
///  Argument 2 : CView, the radar window on which the track is searched
///   
///  RETURNS: 
///  CTrack, track object if found, NULL in the other case
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrack* CTrackMngr::FindTrackByPos(QPoint pos,CView* pView)
{
	QPoint pnt=pos;
	((CAsdView*)pView)->LPtoDP(&pnt);
	QRect rect;
	int view;
	for (view=0;view<m_ViewTable.GetSize();view++)
	{
		if (m_ViewTable[view]==pView)
			break;
	}
	if (view==m_ViewTable.GetSize())
		return NULL;

	CTripleEntryListIterator<CTrack> it( m_TrackList );
	CTrack* pTrack=NULL;
	pTrack = it.current();
	while ( pTrack != 0 ) 
	{
		++it;
		rect=pTrack->m_ElemTable[view]->m_ElemTable[symb]->m_BoundingRect;
		if (rect.contains(pnt))
			return pTrack;
		pTrack = it.current();
	}
	return NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function removes all the processed tracks. m_TrackList table is emptied
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrackMngr::RemoveAllTracks()
{
	RemoveMenu();
	m_TrackList.RemoveAll();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function computes the received ASTERIX messages of category 30 and 252.
///  Argument 1 : BYTE*, the received message
///  Argument 2 : int, the length of the received message
///  Argument 3 : sockaddr_in, the adress of the receiving socket
///  Argument 4 : int, the emitting radar service ident
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen105, CIAsdFctRwsTppAfg010, CIAsdFctRwsTppAfg011,
///					CIAsdFctRwsTppAge010, CIAsdFctRwsTppAge011, CIAsdItfComIba005,
///					CIAsdItfComIta010, CIAsdItfComIta015, CIAsdFctSyn002
//
//-----------------------------------------------------------------------------

BYTE messfix[3000];
void CTrackMngr::OnReceiveMsg(BYTE *pMsg,int Len,sockaddr_in* addr_in,int RcvIdent)
{
	char serid = RcvIdent;
	if (serid >= 'A' && serid <= 'Z')
	{
		CTrackServer* pts = GetServer(serid);
		if (pts)
		{
			//The selected radar service indication in the radar windo status bar is
			//updated
			for (int view=0;view<m_ViewTable.GetSize();view++)
				((CRwdView*)m_ViewTable[view])->UpdateStatusBar();

			{
				CAsterixMsg* pAstMsg= new CAsterixMsg(pMsg,Len);
				int trknum;
				bool cancel;
				while (pAstMsg->DecodeMessage())
				{
					switch (pAstMsg->GetCategory())
					{
					case 1 :
					case 30 :
					case 48 :
						//If the radar service concerned is the one currently selected, if the 
						//the sic and sac are correct the message is processed
						if (pts==m_pTrkSrc && pAstMsg->IsFieldPresent(TrkNum) && pAstMsg->IsFieldPresent(SicSac))
						{
							trknum=pAstMsg->GetUIntVal(TrkNum);
							int sicsac=pAstMsg->GetUIntVal(SicSac);
							int sic = sicsac&0x00FF;
							int sac = (sicsac&0xFF00) >> 8;
							if (sic == pts->m_Sic && sac == pts->m_Sac && m_TrackList.IsAllowed(sic, sac))
							{
								if (pAstMsg->GetCategory()==30)
									trknum&=0xFFF;
								cancel=false;
								//The SSR code is retrieved
								if (pAstMsg->IsFieldPresent(TrkStatus))
								{
									int Sts=pAstMsg->GetByteVal(TrkStatus,1);
									cancel=(Sts & 0x80);
								}
								//If a track with the same track number is found it is updated
								//in the other case a new track is created and a message is logged
								if (trknum>=0)
								{
									CTrack* ptrack = m_TrackList.Get(sic, sac, trknum);
									if (!cancel)
									{
										bool newtrack = false;
										if (!ptrack)
										{
											ptrack= new CTrack(m_pHEvent);
											newtrack = true;
										}
										if (!m_TrackList.SetAt(sic, sac, trknum, ptrack))
										{
											if (newtrack)
											{
												delete ptrack;
												ptrack = NULL;
											}
											if (m_pHEvent)
												m_pHEvent->WriteLogMsg("Error on m_TrackList.SetAt", LogInformation);
										}
										//The identified track if any is updated, and the ageing 
										//indicator is reinitialized
										if (ptrack)
										{
											ptrack->UpdateFromRadar(pAstMsg);
											ptrack->ResetAging();
											if (newtrack)
											{
												if (m_pHEvent)
												{
													QString strac = "primary";
													if (ptrack->GetArcId().length())
														strac = ptrack->GetArcId();
													QString strmsg;
													strmsg.sprintf("Track %s created.", strac.ascii());
													m_pHEvent->WriteLogMsg(strmsg, LogInformation);
													m_pHEvent->OnTrackNeedUpdate(ptrack, true);
												}
											}
										}
									}
									//If the status of the track is not OK, the track presentation is removed
									//and a message is logged
									else
									{
										if (ptrack)
										{
											QString strac = "primary";
											if (ptrack->GetArcId().length())
												strac = ptrack->GetArcId();
											if (m_pHEvent)
											{
												QString strMessage; 
												strMessage.sprintf("Track %s is canceled.", strac.ascii());
												m_pHEvent->WriteLogMsg(strMessage, LogInformation);
												m_pHEvent->OnRemoveTrack(ptrack, true);
											}
											if (m_TrackList.RemoveAt(sic, sac, trknum))
											{
												delete ptrack;
												ptrack = NULL;
											}
										}
									}
								}
							}
							else
							{
								if (m_pHEvent)
								{
									QString strMessage; 
									strMessage.sprintf("Radar service %s has received category 30 from another track server.", pts->m_Name.ascii());
									m_pHEvent->WriteLogMsg(strMessage, LogError);
								}
							}
						}
						break;
					case 62 :
						//If the radar service concerned is the one currently selected, if the 
						//the sic and sac are correct the message is processed
						if (pts==m_pTrkSrc && pAstMsg->IsFieldPresent(TrkNum))
						{
							trknum=pAstMsg->GetUIntVal(TrkNum);



							int sicsac=pAstMsg->GetUIntVal(SicSac);
							int sic = sicsac&0x00FF;
							int sac = (sicsac&0xFF00) >> 8;
							if (sic == pts->m_Sic && sac == pts->m_Sac && m_TrackList.IsAllowed(sic, sac))
							{

								cancel=false;
								//The SSR code is retrieved
								if (pAstMsg->IsFieldPresent(TrkStatus))
								{
									int Sts=pAstMsg->GetByteVal(TrkStatus,1);
									cancel=(Sts & 0x80);
								}
								//If a track with the same track number is found it is updated
								//in the other case a new track is created and a message is logged
								if (trknum>=0)
								{
									CTrack* ptrack = m_TrackList.Get(sic, sac, trknum);
									if (!cancel)
									{
										bool newtrack = false;
										if (!ptrack)
										{
											ptrack= new CTrack(m_pHEvent);
											newtrack = true;
										}
										if (!m_TrackList.SetAt(sic, sac, trknum, ptrack))
										{
											if (newtrack)
											{
												delete ptrack;
												ptrack = NULL;
											}
											if (m_pHEvent)
												m_pHEvent->WriteLogMsg("Error on m_TrackList.SetAt", LogInformation);
										}
										//The identified track if any is updated, and the ageing 
										//indicator is reinitialized
										if (ptrack)
										{
											ptrack->UpdateFromRadar(pAstMsg);
											ptrack->ResetAging();
											if (newtrack)
											{
												if (m_pHEvent)
												{
													QString strac = "primary";
													if (ptrack->GetArcId().length())
														strac = ptrack->GetArcId();
													QString strmsg;
													strmsg.sprintf("Track %s created.", strac.ascii());
													m_pHEvent->WriteLogMsg(strmsg, LogInformation);
													m_pHEvent->OnTrackNeedUpdate(ptrack, true);
												}
											}
										}
									}
									//If the status of the track is not OK, the track presentation is removed
									//and a message is logged
									else
									{
										if (ptrack)
										{
											QString strac = "primary";
											if (ptrack->GetArcId().length())
												strac = ptrack->GetArcId();
											if (m_pHEvent)
											{
												QString strMessage; 
												strMessage.sprintf("Track %s is canceled.", strac.ascii());
												m_pHEvent->WriteLogMsg(strMessage, LogInformation);
												m_pHEvent->OnRemoveTrack(ptrack, true);
											}
											if (m_TrackList.RemoveAt(sic, sac, trknum))
											{
												delete ptrack;
												ptrack = NULL;
											}
										}
									}
								}

								//the availability of the track server with the received sic and sac
								//is updated
								pts->m_LastReceived252 = QDateTime::currentDateTime();
								pts->m_Start252 = true;

							}
							else
							{
								if (m_pHEvent)
								{
									QString strMessage; 
									strMessage.sprintf("Radar service %s has received category 62 from another track server.", pts->m_Name.ascii());
									m_pHEvent->WriteLogMsg(strMessage, LogError);
								}
							}
						}
						else
						{
							//As no Other Category than 62 is received with RAK replay
							//The Radar Service is considered as available as soon as a
							//track update is received
							//if the sic and sac are present the message is processed
							if (pAstMsg->IsFieldPresent(SicSac))
							{
								int sicsac=pAstMsg->GetUIntVal(SicSac);
								int sic = sicsac&0x00FF;
								int sac = (sicsac&0xFF00) >> 8;
								//the availability of the track server with the received sic and sac
								//is updated
								if (sic == pts->m_Sic && sac == pts->m_Sac)
								{
									pts->m_LastReceived252 = QDateTime::currentDateTime();
									pts->m_Start252 = true;
								}
								else
								{
									if (m_pHEvent)
									{
										QString strMessage; 
										strMessage.sprintf("Radar service %s has received category 62 from another track serveur sic=%d sac=%d.", pts->m_Name.ascii(), (int)sic, (int)sac);
										m_pHEvent->WriteLogMsg(strMessage, LogError);
									}
								}
							}

						}
		
						break;
					case 2 :
					case 34 :
					case 252 :
						//if the sic and sac are present the message is processed
						if (pAstMsg->IsFieldPresent(SicSac))
						{
							int sicsac=pAstMsg->GetUIntVal(SicSac);
							int sic = sicsac&0x00FF;
							int sac = (sicsac&0xFF00) >> 8;
							//the availability of the track server with the received sic and sac
							//is updated
							if (sic == pts->m_Sic && sac == pts->m_Sac)
							{
								pts->m_LastReceived252 = QDateTime::currentDateTime();
								pts->m_Start252 = true;
							}
							else
							{
								if (m_pHEvent)
								{
									QString strMessage; 
									strMessage.sprintf("Radar service %s has received category 252 from another track serveur sic=%d sac=%d.", pts->m_Name.ascii(), (int)sic, (int)sac);
									m_pHEvent->WriteLogMsg(strMessage, LogError);
								}
							}
						}
						break;
					default:
						break;

					}
				}
				/* CDS BREAK NGT 2006/05/16 The rule : "empty brackets must be used for delete 
				when de-allocating arrays" cannot be used in this case */
				delete /*[]*/pAstMsg;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time a started timer of the class reaches its
///  value. The class timers are: radar service ageing, blinking effect and duplicated ssr 
///  codes calculation
///  Argument : The timer event.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsGrd010, CIAsdFctRwsTlsGrd011, CIAsdFctDfdCicM3a015,
///					CIAsdFctDfdCicM3a020, CIAsdFctDfdCicMos030, CIAsdFctDfdCicMos035,
///					CIAsdFctRwsRsv050, CIAsdFctRwsRsv061, CIAsdFctRwsRsv062,
///					CIAsdSymSmbNrm115, CIAsdSymSmbNrm120, CIAsdSymSmbNrm125,
///					CIAsdSymSmbNrm135, CIAsdSymSmbNrm140, CIAsdSymSmbNrm145,
///					CIAsdSymSmbBsm110, CIAsdSymSmbBsm115, CIAsdSymSmbBsm120,
///					CIAsdSymSmbBsm125, CIAsdSymSmbBsm130, CIAsdSymSmbBsm135,
///					CIAsdSymSmbBsm140, CIAsdSymSmbBsm145, CIAsdSymSmbBsm150,
///					CIAsdSymSmbBsm155, CIAsdSymSmbEsm160, CIAsdSymSmbEsm170,
///					CIAsdSymSmbEsm175, CIAsdSymSmbEsm180, CIAsdSymSmbEsm185,
///					CIAsdSymSmbEsm190, CIAsdSymSmbErs145, CIAsdSymSmbErs150,
///					CIAsdSymSmbErs155, CIAsdSymSmbFld020
//
//-----------------------------------------------------------------------------
void CTrackMngr::timerEvent(QTimerEvent* e)
{
	// CDS BREAK NGT 22/05/2006 Bad method length rule violated. The code has been 
	//							factorized as much as possible. The resulting length
	//							is 230 lines and is kept as is to maintain a good
	//							readibility of the function (32 lines of comments).
	int diff;
	QDateTime curtime=QDateTime::currentDateTime();
	CTrackServer* Server=NULL;
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	//If the timer event is the m_TrkAgeId, the availability of the currently selected
	//radar service is recomputed
	if (e->timerId()==m_TrkAgeId)
	{
		if (GetCurrentServer())
		{
			m_InitTrackServer = false;
			//If the currently selected radar service is not available 
			if (!IsCurrentSourceAv())
			{
				CTrackServer* pNextServ = NULL;
			//but is within the preference scale
				if (IsRadarInPrioScale(GetCurrentServer()->m_Serid))
				{
					//an automatic switch is performed to the next available radar service in the preference
					//scale for which coupling is available, if possible,
					pNextServ = SearchHiAvRdSv();
				}
				SetTrackSource(pNextServ);
			}
		}
		//During the init phase,
		else if (m_InitTrackServer)
		{
			CTrackServer* pNextServer=SearchHiAvRdSv();
			//if a radar service in the preference scale is found, the 
			//switch is performed
			if (pNextServer)
			{
				SetTrackSource(pNextServer);
				m_InitTrackServer = false;
			}
		}
	}
	//If the timer event is the m_FlashingId, m_GlobalFlashing parameter is 
	//inverted, to process the blinking effect of data fields.
	//also the auto removal fo GRL after assume is checked
	if (e->timerId()==m_FlashingId)
	{
		m_GlobalFlashing=!m_GlobalFlashing;
		CRwdView::InvalidateAllViews();
	}
	//If the timer event is the m_FlashingSpiId, m_GlobalSpiFlashing parameter is 
	//inverted, to process the blinking effect of data fields.
	if (e->timerId()==m_FlashingSpiId)
	{
		m_GlobalSpiFlashing=!m_GlobalSpiFlashing;
		CTripleEntryListIterator<CTrack> it( m_TrackList );
		CTrack* pTrack=NULL;
		pTrack = it.current();
		while ( pTrack != 0 ) 
		{
			++it;
			if (pTrack->m_Spi>0) 
            {
				for (int elem=0;elem<pTrack->m_ElemTable.GetSize();elem++)
					pTrack->m_ElemTable[elem]->OnUpdate();
                diff = abs(curtime.toTime_t()-pTrack->m_SpiTime.toTime_t());
				if (diff>=20)
				{
					pTrack->m_IsSpiDisplayed=false;
					pTrack->m_WasDisplayed=true;
				}
			}
			pTrack = it.current();
		}
	}
	//If the timer event is the m_DuplicatSsrId, the tracks with duplicated ssr codes are
	//identified
	if (e->timerId()==m_DuplicatSsrId)
	{
        typedef QMap<unsigned int, CTrack*> SsrTrackMap;
        SsrTrackMap map;
		SsrTrackMap::Iterator itMap;
		//The m_DUFlag is reseted to identify the impacted tracks
		CTripleEntryListIterator<CTrack> it2( m_TrackList );
		CTrack* pTrack=NULL;
		pTrack = it2.current();
		while ( pTrack != 0 ) 
		{
			++it2;
			pTrack->m_DUFlag=true;
			pTrack = it2.current();
        }
		CTripleEntryListIterator<CTrack> it( m_TrackList );
		pTrack=NULL;
		pTrack = it.current();
		while ( pTrack != 0 ) 
		{
			++it;
			unsigned int ssr = pTrack->m_TD.m_SSR;
			if (ssr!=-1 && ssr!=GARBLED_INVALID)
			{
				//If the ssr code is an offline defined discrete ssr code, the 
				//duplication is not checked
				if (!CTrack::IsSsrCodeDiscrete(ssr))
				{
					//If the ssr code is not already in the map, it is added
					itMap=map.find(ssr);
					if (itMap == map.end())
						map.insert(ssr, pTrack);
					//In the other case, the track is associated to all the tracks
					//already associated to the same ssr code
					else
					{
						//the ssr code is already associated to several tracks
						if (itMap.data()->m_DUTrackList.count())
						{
							QValueList<CTrack*>::iterator itTrack;
							bool found = false;
							//we check if the current track is not already associated
							//to the tracks with the same ssr code
							for ( itTrack = itMap.data()->m_DUTrackList.begin(); itTrack != itMap.data()->m_DUTrackList.end(); ++itTrack)
							{
								if ((*itTrack) == pTrack)
								{
									found=true;
									break;
								}
							}
							//If not already associated, it is
							if (!found)
							{
								for ( itTrack = itMap.data()->m_DUTrackList.begin(); itTrack != itMap.data()->m_DUTrackList.end(); ++itTrack)
								{
									if (pTrack->m_TD.m_SSR == itMap.data()->m_TD.m_SSR)
									{
										pTrack->m_DUTrackList.append((*itTrack));
										(*itTrack)->m_DUTrackList.append(pTrack);
										(*itTrack)->m_DUFlag=false;
										(*itTrack)->m_DUStatus=CTrack::Alert;
									}
								}
								pTrack->m_DUTrackList.append(itMap.data());
								pTrack->m_DUFlag=false;
								pTrack->m_DUStatus=CTrack::Alert;
								itMap.data()->m_DUTrackList.append(pTrack);
								itMap.data()->m_DUFlag=false;
								itMap.data()->m_DUStatus=CTrack::Alert;
							}
							//in the other case the flag is changed so that the DU alert is maintained
							else
							{
								itMap.data()->m_DUFlag=false;
								pTrack->m_DUFlag=false;
							}
						}
						//The ssr code was associated to only one track
						else
						{
							itMap.data()->m_DUTrackList.append(pTrack);
							itMap.data()->m_DUFlag=false;
							itMap.data()->m_DUStatus=CTrack::Alert;
							pTrack->m_DUTrackList.append(itMap.data());
							pTrack->m_DUFlag=false;
							pTrack->m_DUStatus=CTrack::Alert;
						}
					}
				}
			}
			pTrack = it.current();
        }
		//if m_DUFlag is still true, the m_DUTrackList has to be emptied
        for ( itMap = map.begin(); itMap != map.end(); ++itMap ) 
		{
			if (itMap.data()->m_DUFlag == true && itMap.data()->m_DUTrackList.count())
			{
				QValueList<CTrack*>::iterator itTrack;
				for ( itTrack = itMap.data()->m_DUTrackList.begin(); itTrack != itMap.data()->m_DUTrackList.end(); ++itTrack)
				{
					(*itTrack)->m_DUTrackList.remove(itMap.data());
					if (!(*itTrack)->m_DUTrackList.count())
						(*itTrack)->m_DUStatus=CTrack::NoAlert;
				}
				itMap.data()->m_DUTrackList.clear();
				itMap.data()->m_DUStatus=CTrack::NoAlert;
			}
        }
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the time at which the last update of the current radar
///  service has been received or the time at which the last change of radar service
///  occured.
/// 
///  RETURNS: 
///  QDateTime, the time to be displayed when the display is frozen
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
QDateTime CTrackMngr::GetFrozenTime()
{
	if (m_pTrkSrc && m_pTrkSrc->m_Src.m_pRcvSocket)
		return m_pTrkSrc->m_Src.m_pRcvSocket->m_LastReceived;
	else
		return m_SwitchTime;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function switches the unit of the rate of climb or descent data field.
///  All the tracks within every radar window are immediately updated with the 
///  unit value.
///  Argument : bool, the new unit to be applied
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrackMngr::SetRocdUnit(bool rocdunit)
{
	m_RocdUnit=rocdunit;
	//immediate update of the tracks
	CTripleEntryListIterator<CTrack> it( m_TrackList );
	CTrack* pTrack=NULL;
	pTrack = it.current();
	//for every track the ROCD (VspdData) is calculated with newly selected unit 
	while ( pTrack != 0 ) 
	{
		++it;
		if (rocdunit)
		{
			if (pTrack->m_TD.m_RocdFtMin>99)
				pTrack->m_DataTable[VspdData]="99";
			else
				pTrack->m_DataTable[VspdData].sprintf("%.2d",pTrack->m_TD.m_RocdFtMin);
		}
		else
		{
			if (pTrack->m_TD.m_RocdFtNm>99)
				pTrack->m_DataTable[VspdData]="99";
			else
				pTrack->m_DataTable[VspdData].sprintf("%.2d",pTrack->m_TD.m_RocdFtNm);
		}
		//every track presention is updated
		for (int view=0;view<m_ViewTable.GetSize();view++)
		{
			pTrack->m_ElemTable[view]->m_ToggleRocdUnitFtMin=rocdunit;
			pTrack->m_ElemTable[view]->OnUpdate();
		}


		pTrack = it.current();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  When the radar window mode is changed to dependent, all the track independent
///  settings for auxiliary radar windows must be set as for the main radar window.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrackMngr::SetToggledOptionalFieldsAsMain()
{
	if (m_ViewTable.GetSize()>1)
	{
		int iMainVal=0;
		int view,field;
		CRwdView* pMain=NULL;
		pMain=CRwdView::m_psMainWnd;
		CTripleEntryListIterator<CTrack> it( m_TrackList );
		CTrack* pTrack=NULL;
		pTrack = it.current();
		//The settings are updated for every track
		while ( pTrack != 0 ) 
		{
			++it;
			for (view=0;view<m_ViewTable.GetSize();view++)
			{
				if (((CRwdView*)m_ViewTable[view])==pMain)
					iMainVal=view;
			}
					
			for (view=0;view<m_ViewTable.GetSize();view++)
			{
				//only for auxiliary radar windows
				if (view!=iMainVal)
				{
					//optional data fields
					for (field=0;field<MaxFld;field++)
						pTrack->m_ElemTable[view]->m_ForcedData[field]=pTrack->m_ElemTable[iMainVal]->m_ForcedData[field];
					//the ROCD unit
					pTrack->m_ElemTable[view]->m_ToggleRocdUnitFtMin=pTrack->m_ElemTable[iMainVal]->m_ToggleRocdUnitFtMin;
					//the display of the graphical route leg
					pTrack->m_ElemTable[view]->m_IsToggled=pTrack->m_ElemTable[iMainVal]->m_IsToggled;
					//The Halo length
					pTrack->m_ElemTable[view]->ForceHalo(pTrack->m_ElemTable[iMainVal]->IsForcedHalo());				
					//immediate update of the tracks
					pTrack->m_ElemTable[view]->OnUpdate();
				}
			}
				
			pTrack = it.current();
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  When a radar window is created all the track settings are set as for the 
///  parent radar window.
///  Argument 1 : CRwdView*, parent view
///  Argument 2 : CRwdView*, created view
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrackMngr::SetToggledOptionalFieldsAsParent(CRwdView* parentView, CRwdView* daugtherView)
{
	if (m_ViewTable.GetSize()>1)
	{
		int iParentVal=-1;
		int iDaugVal=-1;
		int view,field;
		CTripleEntryListIterator<CTrack> it( m_TrackList );
		CTrack* pTrack=NULL;
		pTrack = it.current();
		while ( pTrack != 0 ) 
		{
			++it;
			for (view=0;view<m_ViewTable.GetSize();view++)
			{
				if (((CRwdView*)m_ViewTable[view])==parentView)
					iParentVal=view;
				if (((CRwdView*)m_ViewTable[view])==daugtherView)
					iDaugVal=view;
			}
			if (iParentVal!=-1 && iDaugVal!=-1)
			{
				//optional data fields
				for (field=0;field<MaxFld;field++)
					pTrack->m_ElemTable[iDaugVal]->m_ForcedData[field]=pTrack->m_ElemTable[iParentVal]->m_ForcedData[field];
				//the ROCD unit
				pTrack->m_ElemTable[iDaugVal]->m_ToggleRocdUnitFtMin=pTrack->m_ElemTable[iParentVal]->m_ToggleRocdUnitFtMin;
				//The Halo length
				pTrack->m_ElemTable[iDaugVal]->ForceHalo(pTrack->m_ElemTable[iParentVal]->IsForcedHalo());				
				//the display of the graphical route leg
				pTrack->m_ElemTable[iDaugVal]->m_IsToggled=pTrack->m_ElemTable[iParentVal]->m_IsToggled;

			}				
			pTrack = it.current();
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the global ROCD unit to be applied. 
/// 
///  RETURNS: 
///  bool, whether the rate is Ft/min or Ft/Nm
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrackMngr::GetRocdUnit()
{
	return m_RocdUnit;
}


//-----------------------------------------------------------------------------
//
/// \class CTrack
/// This class is used to manage all the attributes of one track.  
// 
//
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of the member parameters
///  Argument : CTrackEventStub*, pointer to the CEventHandler class link
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrack::CTrack(CTrackEventStub* pHEvent)
{
	m_WasAOC=false;
	m_TD.m_Mismatch = false;
	m_TD.m_ConflictingArcid = "";
	m_TD.m_ConflictHasBeenSolved=true;
	m_TD.m_ConflictingAssociation = UNCOUPLED;
	m_TD.m_AssociationType = UNCOUPLED;
	m_pHEvent = pHEvent;
	m_TD.m_IsManUntag = false;
	ResetAging();
	m_TD.m_SSR = -1;
	m_TD.m_IsPrimary=false;
	m_TD.m_IsSecondary=false;
	m_TD.m_IsCoupled=false;
	m_Creation=true;
	m_TD.m_IsAoc=false;
	m_TD.m_RocdFtNm=0;
	m_TD.m_FlModConflict=false;
	m_TD.m_RocdFtMin=0;
	m_TD.m_FlightId="";
	m_TD.m_SStatus=false;
	m_Spi=0;
	m_IsSpiDisplayed=false;
	m_WasDisplayed=false;
	m_LeaderPos = 1;
	m_Select = false;
	m_DataTable.SetSize(MaxFld);
	m_TD.m_Velocity = QSize(0,0);
	m_TD.m_RealVelocity = QSize(0,0);
	m_Heading = 0;
	m_TD.m_Speed=-1;
	m_TD.m_IsDeparture=false;
	m_TD.m_IsArrival=false;
	m_TD.m_IsTagged=false;
	m_AlertInhibition=false;
	m_TD.m_EmergAck=false;
	m_IsInUpdate=false;
	m_TD.m_IsAfl=false;
	m_TD.m_bNegatifAfl=false;
	m_IsCoasted=false;
	m_RwyOrientation=-1;
	m_RwyColor=0;
	m_pDialog=NULL;
	m_pDialog2=NULL;
	m_TD.m_IsHighlight=false;
	for(int t = 0; t <= StcaMsg; t++)
		m_AlertState[t] = NoAlert;

	m_pTrkMiles=NULL;
	m_pTOORArrow=NULL;
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	//Initialization of the track presentation per view (CViewTrack object)
	for (int view=0;view<pTrkMngr->m_ViewTable.GetSize();view++)
	{
		CViewTrack* pElem = new CViewTrack();
		pElem->Init(this,pTrkMngr->m_ViewTable[view]);
		m_ElemTable.Add(pElem);
		((CRwdView*)pTrkMngr->m_ViewTable[view])->AddElement(pElem);
	}
	m_DUStatus=NoAlert;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CTrack&, CTrack object to be initialized
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTrack::CTrack(CTrack& trk)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen105, CIAsdFctRwsTlsRbl080, CIAsdFctRwsTlsRbl081,
///					CIAsdFctRwsTlsGrd040, CIAsdFctRwsTlsTml037
//
//-----------------------------------------------------------------------------
CTrack::~CTrack()
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	//Any associated menu is closed
	if (pTrkMngr->m_pMenuCallingTrack && pTrkMngr->m_pMenuCallingTrack == this)
	{
		if (pTrkMngr->m_pTrackMenu)
		{
			delete pTrkMngr->m_pTrackMenu;
			pTrkMngr->m_pTrackMenu = NULL;
		}
		pTrkMngr->m_pMenuCallingTrack = NULL;
		pTrkMngr->m_pMenuCallingWnd = NULL;
	}
	//All the track presentations are removed  
	for (int view=0;view<pTrkMngr->m_ViewTable.GetSize();view++)
	{
		((CRwdView*)pTrkMngr->m_ViewTable[view])->RemoveElement(m_ElemTable[view]);
		delete m_ElemTable[view];
		m_ElemTable[view] = NULL;
	}

	//The attached Range and bearing lines are removed
	int vect=0;
	for (vect=0;vect<m_VectorTable.GetSize();vect++)
		m_VectorTable[vect]->UpdateTrk(this, true);
	for (vect=0;vect<m_VectorTable.GetSize();vect++)
	{
		delete m_VectorTable[vect];
		m_VectorTable[vect] = NULL;
	}

	//Any track mile line is removed
	if (m_pTrkMiles)
		delete m_pTrkMiles;
	
	//Any TOOR Arrow is removed
	if (m_pTOORArrow)
		delete m_pTOORArrow;

	//Any associated pop-up is removed
	if (m_pDialog)
	{
		m_pDialog->close();
		pTrkMngr->m_bDialogIsOn = false;
		m_Glob_ObjectDeletedOnModal = true;
	}
	if (m_pDialog2)
	{
		m_pDialog2->close();
		pTrkMngr->m_bDialogIsOn = false;
		m_Glob_ObjectDeletedOnModal = true;
	}

	//Any DU Alert is recomputed
	QValueList<CTrack*>::iterator itTrack;
	if (m_DUTrackList.count())
	{
		for ( itTrack = m_DUTrackList.begin(); itTrack != m_DUTrackList.end(); ++itTrack)
		{
			(*itTrack)->m_DUTrackList.remove(this);
			if (!(*itTrack)->m_DUTrackList.count())
				(*itTrack)->m_DUStatus=NoAlert;
		}
		m_DUTrackList.clear();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update all the current track presentation accross
///  all the view declared at the track manager level.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::UpdateAllViews()
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	for (int view=0;view<pTrkMngr->m_ViewTable.GetSize();view++)
		m_ElemTable[view]->OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the position of the track.
///
///  RETURNS: 
///  QPoint, position of the track
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
QPoint CTrack::GetPos()
{
	return m_CurPos;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the identifier of the track.
///
///  RETURNS: 
///  QString, the identifier of the track
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
QString CTrack::GetArcId()
{
	//if the track is coupled it returns the callsign
	if (m_TD.m_IsCoupled && m_TD.m_Arcid.length() > 0)
		return m_TD.m_Arcid;
	//if the track is tagged it returns the callsign tag
	else if (m_TD.m_IsTagged && m_TD.m_CsTag.length() > 0)
		return m_TD.m_CsTag;
	//in the other case it returns the TSSR
	else
	{
		QString arcid;
		arcid.sprintf("%.4o", m_TD.m_SSR);
		return arcid;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function retrieves the number id the track, the sic of the track and 
///  the sac of the track.
///  Argument 1 : BYTE&, adress of the track sic to be set
///  Argument 2 : BYTE&, adress of the track sac to be set
///  Argument 3 : int&, adress of the track number to be set
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::GetTrkNum(BYTE& sic, BYTE& sac, int& TrkNum)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	if (pTrkMngr->m_pTrkSrc)
	{
		TrkNum = GetKey(pTrkMngr->m_pTrkSrc->m_Sic, pTrkMngr->m_pTrkSrc->m_Sac);
		sic = pTrkMngr->m_pTrkSrc->m_Sic;
		sac = pTrkMngr->m_pTrkSrc->m_Sac;
	}
	else
	{
		TrkNum = -1;
		sic = 0;
		sac = 0;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the speed of the track.
///
///  RETURNS: 
///  double, the speed of the track
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
double CTrack::GetSpeed()
{
	if ((m_TD.m_Speed == -1) || (m_TD.m_Speed>2000))
		return sqrt(m_TD.m_Velocity.width()*m_TD.m_Velocity.width()+m_TD.m_Velocity.height()*m_TD.m_Velocity.height());
	else
		return (double)(m_TD.m_Speed) * 32 / 360;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function indicates whether the track is assumed or not
///
///  RETURNS: 
///  bool, whether the track is assumed or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrack::IsAoc()
{
	return m_TD.m_IsAoc;	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function indicates whether the track is associated to a tool (range and 
///  bearing line or track mile line) or not
///
///  RETURNS: 
///  bool, whether the track is associated to a tool or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrack::IsAssociatedToTool()
{
	if (m_pTrkMiles || m_VectorTable.GetSize() || m_pTOORArrow)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the important track code for the window framing tool
///
///  RETURNS: 
///  int, the important track code
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CTrack::IsImportant()
{
	int Prio = 0;
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	
	//if the track has a not acknowledged emergency or is associated to a not acknowledged
	//STCA (the STCA is not temporary hidden) the returned code is 4
	//NO STCA System is available
	
	//if the track is associated to a not acknowledged
	//Duplicated SSR Code or Missed Approach
	//the returned code is 3
	if (m_DUStatus!=NoAlert || m_TD.m_KdOfAppClear=="GOA")
		return 3;

	//if the track is assumed the returned code is 2
	if (m_TD.m_IsAoc)
		Prio = _max(Prio, 2);

	//if the track is highlighted the returned code is 1
	if (m_TD.m_IsHighlight)
	{
		Prio = _max(Prio, 1);
	}

	return Prio;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function associates or cancel the association of a range and bearing 
///  line extremity with the track.
///  Argument 1 : CVector*, the range and bearing object
///  Argument 2 : bool, whether the R&B association has to be created or canceled.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::AssociateVector(CVector* pVector,bool asso)
{
	if (asso)
	{
		m_VectorTable.Add(pVector);
	}
	else
	{
		for (int vect=0;vect<m_VectorTable.GetSize();vect++)
		{
			if (m_VectorTable[vect]==pVector)
			{
				m_VectorTable.RemoveAt(vect);
			}
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update the selection status of the track.
///  Argument 1 : bool, selected status to be set
///  Argument 2 : bool, not use in this function.
///  Argument 3 : bool, not used for build 1.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen065, CIAsdFctRwsTppGen085
//
//-----------------------------------------------------------------------------
void CTrack::Select(bool select,bool WithFpl,bool WithArrow)
{
	if (select!=m_Select)
	{
		//the selected status is switched and the track presentations are updated
		m_Select = select;
		for (int view=0;view<m_ElemTable.GetSize();view++)
			m_ElemTable[view]->OnUpdate();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update the m_DataTable. This table contains all
///  data field values that are displayed in the different data blocks.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsGrd010, CIAsdFctRwsTlsGrd016, CIAsdFctDfdCnt020,
///					CIAsdFctDfdCnt030, CIAsdFctDfdCnt035, CIAsdFctDfdCnt045, 
///					CIAsdFctDfdCnt046, CIAsdFctDfdCnt050, CIAsdFctDfdCnt070, 
///					CIAsdFctDfdCnt075, CIAsdFctDfdCnt080, CIAsdFctDfdCnt095,
///					CIAsdFctDfdCnt105, CIAsdFctDfdCicM3a015, CIAsdFctDfdCicM3a020,
///					CIAsdFctDfdCicM3a025, CIAsdFctDfdCicM3a030, CIAsdFctDfdCicCst070,
///					CIAsdFctDfdCicGrd005, CIAsdFctDfdCicMos025, CIAsdFctDfdCicMos030,
///					CIAsdFctDfdCicMos035
//
//-----------------------------------------------------------------------------
void CTrack::UpdateDataTable()
{
	// CDS BREAK NGT 22/05/2006 Bad method length rule violated. The code has been 
	//							factorized as much as possible. The resulting length
	//							is kept as is to maintain a good readibility of the function.

	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	CInfoMngr* pInfoMngr=CAsdApp::GetApp()->GetInfoMngr();

	//The table is initialized
	for (int data=0;data<m_DataTable.GetSize();data++)
		m_DataTable[data]="";

	//The Identifier of the track is set
	m_DataTable[ArcidData]=GetArcId();
	m_CflToBeDisplayed="";
	
	{
		bool IsApp=false;
		if (m_pHEvent)
			IsApp=m_pHEvent->IsAppPos();

		//Arrival runway is set
		if ((m_TD.m_ArrRwy).length())
			m_DataTable[RwyData]=m_TD.m_ArrRwy;
		else
		{
			if ((m_TD.m_ParallelRwy).length())
				m_DataTable[RwyData]=m_TD.m_ParallelRwy;
		}
		m_DataTable[XptData]=m_TD.m_Xpt;
		m_DataTable[EtxData]=m_TD.m_Etx;
		
		
		//the flight restriction indicator is set if an SFL field is received


		//RVR is set
		if ((m_TD.m_Rvr).length())
			m_DataTable[RvrData]="R"+m_TD.m_Rvr;
		//Flight Rule indication
		if (m_TD.m_FltRul!="I")
			m_DataTable[FltrulData]=m_TD.m_FltRul;
		//RFL
		m_DataTable[RflData]=m_TD.m_Rfl.right(3);
		//SID
		m_DataTable[SidData]=m_TD.m_Sid;
		//ADES
		m_DataTable[AdesData]=m_TD.m_Ades;
		//ADEP
		m_DataTable[AdepData]=m_TD.m_Adep;
		//ASSR
		m_DataTable[AssrData]=m_TD.m_ASsr;
		//If TSSR is different from ASSR Tssr problem data field is forced
		QString sSSR;
		sSSR.sprintf("%.4o",m_TD.m_SSR);
		int cfl=0;
		//Callsign Mismatch Indicator
		uchar ch=0;
		if (m_TD.m_Mismatch)
		{
			ch=0xA9;
			m_DataTable[ArcidMismatch]=QString::fromAscii((char*)&ch,1);
		}
		//CFL
		if (m_TD.m_KdOfAppClear.length() && m_TD.m_Cfl=="F000")
		{
			m_DataTable[CflData]=m_TD.m_KdOfAppClear;
			m_CflToBeDisplayed=m_DataTable[CflData];
		}
		else
		{
			if ((m_TD.m_Cfl.length()) && (m_TD.m_Cfl[0].latin1()=='F'))
			{
				cfl=atoi(m_TD.m_Cfl.right(m_TD.m_Cfl.length()-1));
				m_CflToBeDisplayed=m_TD.m_Cfl.right(m_TD.m_Cfl.length()-1);
				if ((cfl>m_TD.m_CurAlt+3) || (cfl<m_TD.m_CurAlt-3))
				{
					m_DataTable[CflData]=m_TD.m_Cfl.right(m_TD.m_Cfl.length()-1);
					if ((m_TD.m_FltRul=="I") && IsApp && (m_TD.m_Cfl=="F000"))
						m_DataTable[CflData]="ILS";
					m_CflToBeDisplayed=m_DataTable[CflData];
				}
				else
					m_DataTable[CflData]=" ";
			}
		}

		//XFL
		if ((m_TD.m_Xfl.length()) && (m_TD.m_Xfl[0].latin1()=='F'))
		{
			int xfl=atoi(m_TD.m_Xfl.right(m_TD.m_Xfl.length()-1).ascii());
			if ((xfl>m_TD.m_CurAlt+3) || (xfl<m_TD.m_CurAlt-3) || (xfl>cfl+3) || (xfl<cfl-3))
				m_DataTable[XflData]=m_TD.m_Xfl;
			else
				m_DataTable[XflData]=" ";
		}

		//8.33 field is set
		if (m_TD.m_833=="Y")
			m_DataTable[N833Data]="8Y";
		if (m_TD.m_833=="N")
			m_DataTable[N833Data]="8N";
		if (m_TD.m_833=="?")
			m_DataTable[N833Data]="8U";

		//RVSM field is set
		if (m_TD.m_Rvsm=="Y")
			m_DataTable[RvsmData]="RY";
		if (m_TD.m_Rvsm=="N")
			m_DataTable[RvsmData]="RN";
		if (m_TD.m_Rvsm=="?")
			m_DataTable[RvsmData]="RU";
		ch=0;

		//flight category is set
		if (m_TD.m_FplCat=="T")
			ch=0xC7;
		if (m_TD.m_FplCat=="E")
			ch=0xC5;
		if (m_TD.m_FplCat=="S")
		  ch=0xC4;
		if (m_TD.m_FplCat=="I")
		  ch=0xD1;
		m_DataTable[FlightCatData]=QString::fromAscii((char*)&ch,1);
	}
	
	//In case a REP is entered, the following should be in if (m_IsCoupled && !m_IsLocalTag)
	if (m_TD.m_IsAfl)
	{
		if ((m_TD.m_Pfl.length()) && (m_TD.m_Pfl[0].latin1()=='F'))
			m_DataTable[AflData]=m_TD.m_Pfl;
		else
			m_DataTable[AflData]="----";
	}
	//WTC and ATYP
	m_DataTable[WtcData]=m_TD.m_Wtc;
	if (m_TD.m_Arctyp.length())
		m_DataTable[ArctypData]=m_TD.m_Arctyp;

	//Airline company name
	QString CompName;
	if (m_TD.m_IsCoupled && m_TD.m_Arcid.length()>=4)
	{
		char IsNb = m_TD.m_Arcid[3].latin1();
		if ((IsNb<='9')&&(IsNb>='0'))
		{
			CompName=CDataMngr::GetCompany(m_TD.m_Arcid.left(3));
			m_DataTable[CompData] = CompName.left(17);
		}
	}
	else
	{
		if (m_TD.m_IsTagged && m_TD.m_CsTag.length()>=4)
		{
			char IsNb = m_TD.m_CsTag[3].latin1();
			if ((IsNb<='9')&&(IsNb>='0'))
			{
				CompName=CDataMngr::GetCompany(m_TD.m_CsTag.left(3));
				m_DataTable[CompData] = CompName.left(17);
			}
		}
	}

	//Mode S data fields
	if ((m_TD.m_FlightId).length() && m_TD.m_FlightId!="noS" && m_TD.m_FlightId!="N/A")
	{
		m_DataTable[ModeSData]="";
		m_DataTable[FlightIdData]=m_TD.m_FlightId;
	}
	else
	{
		m_DataTable[ModeSData]="S";
		m_DataTable[FlightIdData]=m_TD.m_FlightId;
	}

	//Safety net indications
	m_DataTable[AlertData]="";
	m_DataTable[MiData]="";
 	m_DataTable[AsterData]="";

	if (m_DUStatus==Alert || m_DUStatus==AlertAck && ((m_TD.m_IsAoc || m_TD.m_Mnemo==0) && (m_TD.m_IsCoupled || m_TD.m_IsTagged) || (!m_TD.m_IsCoupled && !m_TD.m_IsTagged)))
		m_DataTable[AlertData]="DU";

	if (m_DataTable[AlertData].length() && m_DataTable[AlertData] != "DU")
	{
		if (m_AlertInhibition)
		{
			m_DataTable[MiData]="!";
			m_DataTable[AlertData]="";
		}
		else
			m_DataTable[MiData]="M";
	}

	//local tag indicator
	int sic, sac;
	int trknum;
	if (GetMainKey(sic, sac, trknum))
	{
		AssociationType type = m_pHEvent->GetCouplingDataAssociationType(sic, sac, trknum);
		if (type == TAGGED)
			m_DataTable[AsterData]="+";
		if (type == MODESTAGGED)
			m_DataTable[AsterData]="s";
	}

	//Flight attitude indication
	if (m_TD.m_TrkFlMod != '*' || !m_TD.m_IsTrkRcd)
		m_DataTable[FAttData]=QString::fromAscii((char*)&(m_TD.m_TrkFlMod),1);
	else
	{
		//In case the field I030/200 of the ARTAS category 030 is not received
		//the flight mode is calculated
		int tmp = int(m_TD.m_TrkRcd);
		float RcValue = m_TD.m_TrkRcd - (float)tmp;
		int TrkVspd;
		if (RcValue >= 0.5)
			TrkVspd = (int)m_TD.m_TrkRcd+1;
		else
			TrkVspd = (int)m_TD.m_TrkRcd;
		uchar ch;
		if (TrkVspd<0)
			ch=0xC5;
		if (TrkVspd>0)
			ch=0xC4;
		if (TrkVspd==0)
			ch=0xC9;
		m_DataTable[FAttData]=QString::fromAscii((char*)&ch,1);
	}


	//ROCD data field
	if (m_TD.m_IsTrkRcd)
	{
		if (CAsdApp::GetApp()->GetTrackMngr()->GetRocdUnit())
		{
			if (m_TD.m_RocdFtMin>99)
				m_DataTable[VspdData]="99";
			else
				m_DataTable[VspdData].sprintf("%.2d",m_TD.m_RocdFtMin);
		}
		else
		{
			if (m_TD.m_RocdFtNm>99)
				m_DataTable[VspdData]="99";
			else
				m_DataTable[VspdData].sprintf("%.2d",m_TD.m_RocdFtNm);
		}
	}
	else
		m_DataTable[VspdData]="**";

	//Ground Speed
	if (m_TD.m_Speed==-1)
	{
		int spd = (int)(sqrt(m_TD.m_Velocity.width()*m_TD.m_Velocity.width()+m_TD.m_Velocity.height()*m_TD.m_Velocity.height())*360/32);
		m_DataTable[GspdData].sprintf("%.3d",spd);
	}
	else
	{
		if (m_TD.m_Speed>999)
			m_DataTable[GspdData].sprintf("999");
		else
			m_DataTable[GspdData].sprintf("%.3d",m_TD.m_Speed);
	}
	//If the track is a secondary track, the following information is available
	if (m_TD.m_IsSecondary)
	{
		//TSSR 
		if (m_TD.m_SSR != -1)
			if (m_TD.m_SSR==GARBLED_INVALID)
				m_DataTable[TssrData].sprintf("****");
			else
				m_DataTable[TssrData].sprintf("%.4o",m_TD.m_SSR);
		else
			m_DataTable[TssrData].sprintf("----");
		//AFL
		if (m_TD.m_IsAfl && !m_TD.m_IsCoupled)
			m_DataTable[AflData]="----";
		else
		{
			if (m_TD.m_CurAlt==GARBLED_INVALID)
				m_DataTable[AflData]="****";
			else
			{
				QString safl = pInfoMngr->GetCorrectedAltitude(m_CurPos,m_TD.m_CurAlt);
				if (safl == "F999" || safl.left(1) == "-")
					m_TD.m_bNegatifAfl=true;
				else
					m_TD.m_bNegatifAfl=false;
				m_DataTable[AflData]= safl;
			}
		}
	}

	//The emergency alerts are computed according to the ssr code
	if (m_TD.m_SSR==07500)
		m_DataTable[EmrgData]="HJ";
	if (m_TD.m_SSR==07600)
		m_DataTable[EmrgData]="RF";
	if (m_TD.m_SSR==07700)
		m_DataTable[EmrgData]="EM";
	if ((m_TD.m_SSR==01601) || (m_TD.m_SSR==01602))
		m_DataTable[EmrgData]="RE";

	m_WasAOC = m_TD.m_IsAoc;

}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function indicates whether the SSR code in argument is discrete or not.
///  The list of discrete ssr codes is offline defined.
///  Argument : int, octal code SSR
///
///  RETURNS: 
///  bool, whether the code in argument is discrete or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrack::IsSsrCodeDiscrete(int ssr)
{
	QString locssr;
	locssr.sprintf("%.4o", ssr);
	ssr=atoi(locssr.ascii());
	CDiscreteSSRList DiscreteSsrList = CAsdApp::GetApp()->GetDiscreteSsrList();

	QValueList<CDiscreteSSRList::m_SsrRange>::iterator it;

	//If the ssr code is part of the discrete ssr code list the function returns true
	for ( it = DiscreteSsrList.getDiscretSsrList()->begin(); it != DiscreteSsrList.getDiscretSsrList()->end(); ++it )
	{
		   /* CDS BREAK NGT 2006/05/16 Use of ptr->fld access cannot be used in this case 
			the iterator is of type QValueList */
			if (ssr > (*it).MinSsr && ssr < (*it).MaxSsr)
				return true;
	}

	return false;
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to update the track safety indications. 
///  Argument : bool, whether the track presentation must be updated or not.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::UpdateFromSafety(bool UpdateTrk)
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	if (pMngr->m_pTrkSrc)
	{
		//update all the data fields
		UpdateDataTable();
		//update the track presentation
		if (UpdateTrk)
		{
			for (int view=0;view<m_ElemTable.GetSize();view++)
				m_ElemTable[view]->OnUpdate();
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to update the track information received from
///  ASTERIX messages. 
///  Argument : CAsterixMsg, the received ASTERIX message.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppPps030, CIAsdFctRwsTppPps035, CIAsdFctRwsTppLls020, 
///					CIAsdFctRwsTppLls025, CIAsdFctRwsTppAfg005, CIAsdFctRwsTppAfg006, 
///					CIAsdFctRwsTppAfg025, CIAsdFctRwsTlsRbl040, CIAsdFctRwsTlsTml036,
///					CIAsdFctDfdCnt045, CIAsdFctDfdCicMos005, CIAsdFctDfdCicMos010
//
//-----------------------------------------------------------------------------
void CTrack::UpdateFromRadar(CAsterixMsg* pMsg)
{
	
	// CDS BREAK NGT 22/05/2006 Bad method length rule violated. The code has been 
	//							factorized as much as possible. The resulting length
	//							is 250 lines and is kept as is to maintain a good
	//							readibility of the function.
	

	QString CallSignTrack="";
	QString CsCallSign="";
	QString CsFlightId="";

	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();

	if (pMngr->m_pTrkSrc)
	{
		int tmp;
		//The former value are saved in order to check if the track presentation has
		//to be updated or not
		int oldSSR = m_TD.m_SSR;
		int oldCurAlt = m_TD.m_CurAlt;
		int oldSpeed = m_TD.m_Speed;
		bool oldIsPrimary = m_TD.m_IsPrimary;
		bool oldIsSecondary = m_TD.m_IsSecondary;
		bool oldEmergAck = m_TD.m_EmergAck;
		bool oldIsTrkRcd = m_TD.m_IsTrkRcd;
		int oldRocdFtMin = m_TD.m_RocdFtMin;
		int oldRocdFtNm=m_TD.m_RocdFtNm;
		bool oldIsAfl = m_TD.m_IsAfl;
		QSize oldVelocity = m_TD.m_Velocity;
		bool oldNegatifAfl = m_TD.m_bNegatifAfl;
		QString oldFlightId = m_TD.m_FlightId;
		bool oldSStatus = m_TD.m_SStatus;

		//the parameters are set to the default values
		m_TD.m_CurAlt = DEFAULT_AFL;
		m_TD.m_Speed = -1;
		QPoint newpos,tmppos;
		m_TD.m_IsPrimary=false;
		m_TD.m_IsSecondary=false;
		bool OldCoasted=m_IsCoasted;
		bool FlModNotReceived=true;
		m_IsCoasted=false;
		switch (pMsg->GetCategory())
		{
			//Category 1
		case 1:
			//Track status: primary, secondary, SPI indication
			if (pMsg->IsFieldPresent(TrkRep))
			{
				tmp=pMsg->GetByteVal(TrkRep,0);
				m_IsCoasted=((tmp & 0x10) == 0x10);
				tmp=(tmp & 0x30)>>4;
				if ((tmp & 0x3) !=0x2)
					m_TD.m_IsPrimary;
				if ((tmp & 0x3) !=0x1)
					m_TD.m_IsSecondary=true;
				m_Spi=(tmp & 0x04)==0x04;
				if (m_Spi>0)
				{
				   if ((!m_IsSpiDisplayed) && (!m_WasDisplayed))
				   {
					  m_IsSpiDisplayed=true;
					  m_SpiTime=QDateTime::currentDateTime();
				   }
				}
				else
				{
				   m_IsSpiDisplayed= false;
				   m_WasDisplayed=false;
				}
			}
			else
				m_TD.m_IsSecondary=false;
			//Polar Track Position
			if (pMsg->IsFieldPresent(TrkPosPol))
			{
				PolarCoord coord=pMsg->GetPolarCoord(TrkPosPol);
				float tmpfloat=(float)(float(coord.Azimuth)*3.14159/32768.0);
				newpos.setX((int)(coord.Range/4.0*sin(tmpfloat)));//+m_Offset.x());
				newpos.setY((int)(coord.Range/4.0*cos(tmpfloat)));//+m_Offset.y());
			}
			//Cardinal Track Position
			if (pMsg->IsFieldPresent(TrkPos))
			{
				newpos=pMsg->GetPos(TrkPos);
				newpos.setX(newpos.x()/2);//+m_Offset.x());
				newpos.setY(newpos.y()/2);//+m_Offset.y());
			}
			//SSR code
			if (pMsg->IsFieldPresent(TrkSsr))
			{
				m_TD.m_SSR=pMsg->GetUIntVal(TrkSsr);
				m_TD.m_SSR=m_TD.m_SSR & 0x0FFF;
				bool IsEmmergency = false;
				if (m_TD.m_SSR==07500 || m_TD.m_SSR==07600 || m_TD.m_SSR==07700 || m_TD.m_SSR==01601 || m_TD.m_SSR==01602)
					IsEmmergency = true;
				if (IsEmmergency && oldSSR != -1 && oldSSR != m_TD.m_SSR)
				{
					m_TD.m_EmergAck=false;
					if (m_pHEvent)
						m_pHEvent->OnTrackEmergAck(this, false);
				}
			}
			//AFL
			if (pMsg->IsFieldPresent(TrkAlt))
			{
				m_TD.m_CurAlt=pMsg->GetUIntVal(TrkAlt);
				if (m_TD.m_CurAlt & 0x8000)
					m_TD.m_CurAlt=GARBLED_INVALID;
				else
				{
					m_TD.m_CurAlt&=0x3FFF;
					if (m_TD.m_CurAlt & 0x2000)
						m_TD.m_CurAlt|=0xFFFFC000;
					m_TD.m_CurAlt=(m_TD.m_CurAlt+2)/4;
				}
			}
			//Track Speed
			if (pMsg->IsFieldPresent(TrkSpd))
			{
				PolarCoord tmppol=pMsg->GetPolarCoord(TrkSpd);
				float Speed=((double)tmppol.Range*.22);
				float tmpSpeed = Speed - (int)Speed;
				if (tmpSpeed >=0.5)
					m_TD.m_Speed = (int)Speed+1;
				else
					m_TD.m_Speed = (int)Speed;
				double tmpfloat=(float)(float(tmppol.Azimuth)*3.14159/32768.0);
				m_TD.m_Velocity.setWidth((int)((double)tmppol.Range*.22*32.0*sin(tmpfloat)/360.0));
				m_TD.m_Velocity.setHeight((int)((double)tmppol.Range*.22*32.0*cos(tmpfloat)/360.0));
				m_TD.m_RealVelocity.setWidth((int)((double)tmppol.Range*sin(tmpfloat)));
				m_TD.m_RealVelocity.setHeight((int)((double)tmppol.Range*cos(tmpfloat)));

			}
			m_TD.m_FlightId="noS";
			m_TD.m_SStatus=false;		
			break;
			//Category 30
		case 30:
			if (pMsg->IsFieldPresent(TrkCls))
			{
				CsCallSign=pMsg->GetStrVal(TrkCls);
			
			}
			//Track status: primary, secondary, SPI indication
			if (pMsg->IsFieldPresent(TrkStatus))
			{
				tmp=pMsg->GetByteVal(TrkStatus,0);
				m_IsCoasted=((tmp & 0x10) == 0x10);
				tmp= ((tmp & 0xE)>>1)& 0x3;
				switch (tmp)
				{
				case 0 : case 4 : 
					m_TD.m_IsPrimary=true;;
					m_TD.m_IsSecondary=true;
					break;
				case 1 : case 5 :
					m_TD.m_IsPrimary=true;;
					break;
				case 2 : case 6 :
					m_TD.m_IsSecondary=true;
					break;
				case 3 : case 7 :
					// unused
					break;
				};
				tmp=pMsg->GetByteVal(TrkStatus,2);
				m_Spi=((tmp & 0x08)== 0x08);
				if (m_Spi>0)
				{
				   if ((!m_IsSpiDisplayed) && (!m_WasDisplayed))
				   {
					  m_IsSpiDisplayed=true;
					  m_SpiTime=QDateTime::currentDateTime();
				   }
				}
				else
				{
				   m_IsSpiDisplayed= false;
				   m_WasDisplayed=false;
				}
			}
			else
				m_TD.m_IsSecondary=false;
			//Track position
			if (pMsg->IsFieldPresent(TrkPos))
			{
				newpos=pMsg->GetPos(TrkPos);
				newpos.setX((newpos.x()+DECALRADARX)/2);
				newpos.setY((newpos.y()+DECALRADARY)/2);
			}
			//Track speed, prediction vector
			if (pMsg->IsFieldPresent(TrkSpd))
			{
				tmppos=pMsg->GetPos(TrkSpd);
				float Speed=sqrt(tmppos.x()*tmppos.x()+tmppos.y()*tmppos.y())*.22;
				float tmpSpeed = Speed - (int)Speed;
				if (tmpSpeed >=0.5)
					m_TD.m_Speed = (int)Speed+1;
				else
					m_TD.m_Speed = (int)Speed;
				m_TD.m_Velocity.setWidth((int)((double)tmppos.x()*.22*32.0/360.0));
				m_TD.m_Velocity.setHeight((int)((double)tmppos.y()*.22*32.0/360.0));
				m_TD.m_RealVelocity.setWidth(tmppos.x());
				m_TD.m_RealVelocity.setHeight(tmppos.y());
			}
			//TSSR code garbled, invalid or valid
			if (pMsg->IsFieldPresent(TrkSsr))
			{
				m_TD.m_SSR=pMsg->GetUIntVal(TrkSsr);
				if (m_TD.m_SSR & 0xC000)
					m_TD.m_SSR=GARBLED_INVALID;
				else
				{
					m_TD.m_SSR=m_TD.m_SSR & 0x0FFF;
					bool IsEmmergency = false;
					if (m_TD.m_SSR==07500 || m_TD.m_SSR==07600 || m_TD.m_SSR==07700 || m_TD.m_SSR==01601 || m_TD.m_SSR==01602)
						IsEmmergency = true;
					if (IsEmmergency && oldSSR != -1 && oldSSR != m_TD.m_SSR)
					{
						m_TD.m_EmergAck=false;
						if (m_pHEvent)
							m_pHEvent->OnTrackEmergAck(this, false);
					}
				}
			}
			//AFL garbled, invalid or valid
			if (pMsg->IsFieldPresent(TrkAlt))
			{
				m_TD.m_CurAlt=pMsg->GetUIntVal(TrkAlt);
				if (m_TD.m_CurAlt & 0xC000) 
					m_TD.m_CurAlt=GARBLED_INVALID;
				else
				{
					m_TD.m_CurAlt&=0x3FFF;
					if (m_TD.m_CurAlt & 0x2000)
					{
						m_TD.m_CurAlt|=0xFFFFC000;
						m_TD.m_bNegatifAfl=true;
					}
					else
						m_TD.m_bNegatifAfl=false;
					m_TD.m_CurAlt/=4;
				}
			}
			//Track flight attitude
			m_TD.m_IsTrkRcd=false;
			m_TD.m_TrkFlMod = '*';
			if (pMsg->IsFieldPresent(TrkFlMod))
			{
				FlModNotReceived=false;
				//The Mode of Flight is indicated by the byte 3 and 4 of the field
				int val=pMsg->GetUIntVal(TrkFlMod);
				bool byte4=val & 0x08;
				bool byte3=val & 0x04;
				// "00" : Level Flight
				if (!byte4 && !byte3)
					m_TD.m_TrkFlMod = 0xC9;
				// "01" : Climb
				if (!byte4 && byte3)
					m_TD.m_TrkFlMod = 0xC4;
				// "10" : Descent
				if (byte4 && !byte3)
					m_TD.m_TrkFlMod = 0xC5;
				// "11" : Undetermined
				if (byte4 && byte3)
					m_TD.m_TrkFlMod = '*';
			}

			//Track vertical speed, ROCD
			if (pMsg->IsFieldPresent(TrkVSpd))
			{
				m_TD.m_IsTrkRcd=true;
				int TrkVspd;
				m_TD.m_TrkRcd=pMsg->GetIntVal(TrkVSpd)*0.0586;
				int tmp = int(m_TD.m_TrkRcd);
				float RcValue = m_TD.m_TrkRcd - (float)tmp;
				if (RcValue >= 0.5)
					TrkVspd = (int)m_TD.m_TrkRcd+1;
				else
					TrkVspd = (int)m_TD.m_TrkRcd;
				m_TD.m_FlModConflict=false;
				if (!FlModNotReceived)
				{
					if (TrkVspd<0 && m_TD.m_TrkFlMod != 0xC5)
						m_TD.m_FlModConflict=true;
					if (TrkVspd>0 && m_TD.m_TrkFlMod != 0xC4)
						m_TD.m_FlModConflict=true;
					if (TrkVspd==0 && m_TD.m_TrkFlMod != 0xC9)
						m_TD.m_FlModConflict=true;
				}
				TrkVspd=abs(TrkVspd);
				m_TD.m_RocdFtMin=TrkVspd;
				m_TD.m_RocdFtNm=0;
				if (m_TD.m_Speed>0)
					m_TD.m_RocdFtNm=(TrkVspd*60)/(m_TD.m_Speed);
			}
			//Mode S, Flight ID
			if (pMsg->IsFieldPresent(TrkAddr))
			{
				m_TD.m_FlightId="";
				if (pMsg->IsFieldPresent(TrkAircraft))
				{
					int curfirstbit=0,curvalue=0;
					for (int p=0;p<8;p++)
					{
							CString str;
							curfirstbit=0;
							curvalue = 0;
							for (int i=(p*6)/8;i<=((p+1)*6-1)/8;i++)
							{
								tmp = pMsg->GetByteVal(TrkAircraft,i);
								curvalue =(curvalue<<8)+tmp;
								curfirstbit+=8;
							}
							tmp = (0xFFFFFFFF<<((p*6-((p*6)/8)*8)+(32-curfirstbit)))>>((p*6-((p*6)/8)*8)+(32-curfirstbit));
							curvalue &= tmp;
							curvalue = curvalue>>(curfirstbit-1-((p+1)*6-1-((p*6)/8)*8));
							if (curvalue)
								if (curvalue>=1 && curvalue<=26) 
									m_TD.m_FlightId+=(char)(curvalue+'@');
								else
									if ((curvalue==32) || ((curvalue>=48) && (curvalue<=57)))
										m_TD.m_FlightId+=(char)curvalue;
									else
										m_TD.m_FlightId+="?";

					}
					CsFlightId=m_TD.m_FlightId;
					m_TD.m_FlightId.lower();
					m_TD.m_SStatus=true;
				}
				else
				{
					m_TD.m_FlightId="N/A";
					m_TD.m_SStatus=true;
				}
			}
			else
			{
				m_TD.m_FlightId="noS";
				m_TD.m_SStatus=false;
			}
			break;

			//Category 048

		case 48:
		  if (pMsg->IsFieldPresent(TrkRep))
		    {
		      tmp=pMsg->GetByteVal(TrkRep,0);
		      m_Spi=(tmp & 0x04)==0x04;
		      tmp=(tmp & 0xE0)>>5;
		      m_IsCoasted=(tmp == 0);// No Detection
		      switch (tmp)
			{
			case 0:
			  //			  if (m_Type==none)
			  // return false;
			  break;
			case 1:
			  m_TD.m_IsPrimary=true;//m_Type=prim;
			  break;
			default:
			  m_TD.m_IsSecondary=true;//m_Type=sec;
			  break;
			}
		    }
		  // m_TrkNb=pMsg->GetUIntVal(TrkNum);
 
		  if (pMsg->IsFieldPresent(TrkPosPol))
		    {
		      PolarCoord coord=pMsg->GetPolarCoord(TrkPosPol);
		      float tmpfloat=(float)(float(coord.Azimuth)*3.14159/32768.0);
		      newpos.setX((int)(coord.Range/8.0*sin(tmpfloat)));//+m_Offset.x;
		      newpos.setY((int)(coord.Range/8.0*cos(tmpfloat)));//+m_Offset.y;
		    }
		  
		  if (pMsg->IsFieldPresent(TrkPos))
		    {
		      newpos=pMsg->GetPos(TrkPos);
		      newpos.setX(newpos.x()/4+DECALRADARX);//m_Offset.x;
		      newpos.setY(newpos.y()/4+DECALRADARY);//m_Offset.y;
		    }
		  
		  if (pMsg->IsFieldPresent(TrkSsr))
		    {
		      m_TD.m_SSR=pMsg->GetUIntVal(TrkSsr);
		      m_TD.m_SSR&=0xFFF;
		      /*		      
		      if (m_SSR==07400) 
			
			m_DataTable[spcdata]="INT";
		      
		      if (m_SSR==07500) 
			
			m_DataTable[spcdata]="HIJ";
		      
		      if (m_SSR==07600) 
			
			m_DataTable[spcdata]="RCF";
		      
		      if (m_SSR==07700)
			
			m_DataTable[spcdata]="EMG";
		      
			m_DataTable[clsdata].Format("%.4o",m_SSR);*/
		      
		    }
		  
		  if (pMsg->IsFieldPresent(TrkAlt))
		    {
		      m_TD.m_CurAlt=pMsg->GetUIntVal(TrkAlt);
		      if (m_TD.m_CurAlt & 0x8000)
			 m_TD.m_CurAlt=-1;
		      else
			{
			   m_TD.m_CurAlt&=0x3FFF;
			  if (m_TD.m_CurAlt & 0x2000)
			    {
			      m_TD.m_CurAlt|=0xFFFFC000;
			      m_TD.m_bNegatifAfl=true;
			    }
			  else
			    m_TD.m_bNegatifAfl=false;
			  m_TD.m_CurAlt=(m_TD.m_CurAlt+2)/4;
			}
		    }
		  
		  if (pMsg->IsFieldPresent(TrkSpd))
		    {
		      PolarCoord tmppol=pMsg->GetPolarCoord(TrkSpd);
		      m_TD.m_Speed=((int)((double)tmppol.Range*.22));
		      double tmpfloat=(float)(float(tmppol.Azimuth)*3.14159/32768.0);
		      m_TD.m_Velocity.setWidth((int)((double)tmppol.Range*.22*32.0*sin(tmpfloat)/360.0));
		      m_TD.m_Velocity.setHeight((int)((double)tmppol.Range*.22*32.0*cos(tmpfloat)/360.0));
		      //m_TD.m_Velocity.cx=((int)((double)tmppol.Range*.22*32.0*sin(tmpfloat)/360.0));
		      //m_TD.m_Velocity.cy=((int)((double)tmppol.Range*.22*32.0*cos(tmpfloat)/360.0));
		      // ?? m_TD.m_RealVelocity.setWidth(tmppos.x());
		      // ?? m_TD.m_RealVelocity.setHeight(tmppos.y());
		    }

		  m_TD.m_RocdFtMin=0;
		  m_TD.m_RocdFtNm=0;
		  m_TD.m_IsTrkRcd=false;
		  m_TD.m_TrkFlMod = '*';

		  if (pMsg->IsFieldPresent(TrkAddr))
		    {
		      m_TD.m_FlightId="";
		      if (pMsg->IsFieldPresent(TrkAircraft))
			{
			  int curfirstbit=0,curvalue=0;
			  for (int p=0;p<8;p++)
			    {
			      CString str;
			      curfirstbit=0;
			      curvalue = 0;
			      for (int i=(p*6)/8;i<=((p+1)*6-1)/8;i++)
				{
				  tmp = pMsg->GetByteVal(TrkAircraft,i);
				  curvalue =(curvalue<<8)+tmp;
				  curfirstbit+=8;
				}

			      tmp = (0xFFFFFFFF<<((p*6-((p*6)/8)*8)+(32-curfirstbit)))>>((p*6-((p*6)/8)*8)+(32-curfirstbit));
			      curvalue &= tmp;
			      curvalue = curvalue>>(curfirstbit-1-((p+1)*6-1-((p*6)/8)*8));
			      if (curvalue)
				if (curvalue>=1 && curvalue<=26) 
				  m_TD.m_FlightId+=(char)(curvalue+'@');
				else
				  if ((curvalue==32) || ((curvalue>=48) && (curvalue<=57)))
				    m_TD.m_FlightId+=(char)curvalue;
				  else
				    m_TD.m_FlightId+="?";
			    }
			  CsFlightId=m_TD.m_FlightId;
			  m_TD.m_FlightId.lower();
			  m_TD.m_SStatus=true;
			  //			  m_ModeSData.MakeLower();
			}
		      else
			{
			  m_TD.m_FlightId="N/A";
			  m_TD.m_SStatus=true;
			  // m_ModeSData="N/A";
			}
		    }
		  else
		    {
		      m_TD.m_FlightId="noS";
		      m_TD.m_SStatus=false;
		    }
		  //m_ModeSData="noS";
		  break;

		//Category 062
		case 62:
			//Track Status
			if (pMsg->IsFieldPresent(TrkStatus))
			{
				tmp=pMsg->GetByteVal(TrkStatus,0);
				m_Spi=(tmp & 0x40)==0x40;
				m_TD.m_IsSecondary=true;
				tmp=pMsg->GetByteVal(TrkStatus,1);
				tmp=pMsg->GetByteVal(TrkStatus,2);
				tmp=pMsg->GetByteVal(TrkStatus,3);
				m_IsCoasted=((tmp & 0x1) == 0x1);

				if (m_Spi>0)
				{
				   if ((!m_IsSpiDisplayed) && (!m_WasDisplayed))
				   {
					  m_IsSpiDisplayed=true;
					  m_SpiTime=QDateTime::currentDateTime();
				   }
				}
				else
				{
				   m_IsSpiDisplayed= false;
				   m_WasDisplayed=false;
				}

			}
			else
				m_TD.m_IsPrimary=true;

			//Track Position
			if (pMsg->IsFieldPresent(TrkPos))
			{
				newpos=pMsg->GetPos(TrkPos);
				newpos.setX((int)(newpos.x()*16.0/1852.0)+DECALRADARX);//+m_Offset.x;
				newpos.setY((int)(newpos.y()*16.0/1852.0)+DECALRADARY);//+m_Offset.y;
			}

			//Track Speed
			if (pMsg->IsFieldPresent(TrkSpd))
			{
				QPoint tmppos=pMsg->GetPos(TrkSpd);
				m_TD.m_Speed=(int)(sqrt(tmppos.x()*tmppos.x()+tmppos.y()*tmppos.y())*3600.0/(4.0*1852.0));
				m_TD.m_Velocity.setWidth((int)((double)tmppos.x()*320.0/(4.0*1852.0)));
				m_TD.m_Velocity.setHeight((int)((double)tmppos.y()*320.0/(4.0*1852.0)));
				m_TD.m_RealVelocity.setWidth(tmppos.x());
				m_TD.m_RealVelocity.setHeight(tmppos.y());
			}
			//Track SSR code
			if (pMsg->IsFieldPresent(TrkSsr))
			{
				m_TD.m_SSR=pMsg->GetUIntVal(TrkSsr);
				m_TD.m_SSR&=0xFFF;
				bool IsEmmergency = false;

				if (m_TD.m_SSR==07500 || m_TD.m_SSR==07600 || m_TD.m_SSR==07700 || m_TD.m_SSR==01601 || m_TD.m_SSR==01602)
					IsEmmergency = true;

				if (IsEmmergency && oldSSR != -1 && oldSSR != m_TD.m_SSR)
				{
					m_TD.m_EmergAck=false;
					if (m_pHEvent)
						m_pHEvent->OnTrackEmergAck(this, false);
				}
			}

			//Track Altitude
			if (pMsg->IsFieldPresent(TrkAlt))
			{
				m_TD.m_CurAlt=pMsg->GetIntVal(TrkAlt);
				int tl=(m_TD.m_CurAlt & 0x8000)==0;
				m_TD.m_CurAlt&=0x7FFF;
				if (m_TD.m_CurAlt & 0x4000)
					m_TD.m_CurAlt|=0xFFFF8000;
				m_TD.m_CurAlt=(m_TD.m_CurAlt+2)/4;			
			}

			//Track Vertical Speed
			if (pMsg->IsFieldPresent(TrkVSpd))
			{
				m_TD.m_IsTrkRcd=true;

				int TrkVspd=pMsg->GetIntVal(TrkVSpd);
				if (TrkVspd >0)
				TrkVspd=(int)((TrkVspd*0.0625)+0.5);
				if (TrkVspd <0)
				TrkVspd=(int)((TrkVspd*0.0625)-0.5);
				TrkVspd=abs(TrkVspd);
				m_TD.m_RocdFtMin=TrkVspd;
				m_TD.m_RocdFtNm=0;
				if (m_TD.m_Speed>0)
					m_TD.m_RocdFtNm=(TrkVspd*60)/(m_TD.m_Speed);
			}

			//Track Callsign
			if (pMsg->IsFieldPresent(TrkCls))
			{
				CsCallSign=pMsg->GetStrVal(TrkCls);
			}

			m_TD.m_TrkFlMod = '*';


			//Track Flight Id
			if (pMsg->IsFieldPresent(TrkAddr))
			{
				m_TD.m_FlightId="";
				if (pMsg->IsFieldPresent(TrkAircraft))
				{
					int curfirstbit=0,curvalue=0;
					for (int p=0;p<8;p++)
					{
							CString str;
							curfirstbit=0;
							curvalue = 0;
							for (int i=(p*6)/8;i<=((p+1)*6-1)/8;i++)
							{
								tmp = pMsg->GetByteVal(TrkAircraft,i);
								curvalue =(curvalue<<8)+tmp;
								curfirstbit+=8;
							}
							tmp = (0xFFFFFFFF<<((p*6-((p*6)/8)*8)+(32-curfirstbit)))>>((p*6-((p*6)/8)*8)+(32-curfirstbit));
							curvalue &= tmp;
							curvalue = curvalue>>(curfirstbit-1-((p+1)*6-1-((p*6)/8)*8));
							if (curvalue>=1 && curvalue<=26) 
								m_TD.m_FlightId+=(char)(curvalue+'@');
							else
								if ((curvalue==32) || ((curvalue>=48) && (curvalue<=57)))
									m_TD.m_FlightId+=(char)curvalue;
								else
									m_TD.m_FlightId+="?";

					}
					CsFlightId=m_TD.m_FlightId;
					m_TD.m_FlightId.lower();
					m_TD.m_SStatus=true;
				}
				else
				{
					m_TD.m_FlightId="N/A";
					m_TD.m_SStatus=true;
				}
			}
			else
			{
				m_TD.m_FlightId="noS";
				m_TD.m_SStatus=false;
			}
			break;

		default:
			break;
		}


		
		//m_IsAfl is set to true, if no AFL is received from the radar. The RFL must be 
		//used
		if (m_TD.m_IsSecondary)
			m_TD.m_IsAfl=(m_TD.m_CurAlt==DEFAULT_AFL);
		//If the track enters a runway area, the color and orientation of the leader line 
		//must be forced according to the offline definition
		m_RwyOrientation=-1;
		if (m_TD.m_IsCoupled)
		{
			int orientation,colornb;
			if (CDataMngr::GetRunway(m_TD.m_Ades, m_TD.m_ArrRwy, m_CurPos, orientation, colornb))
			{
				m_RwyOrientation=orientation;
				m_RwyColor=colornb;
			}
		}
		//Former position is added as a new afterglow
		if (!m_Creation)
		{
			m_PastPos.InsertAt(0,m_CurPos);
			m_PastCoasted.InsertAt(0,OldCoasted);
		}
		if (m_PastPos.GetSize()>255)
		{
			m_PastPos.SetSize(255);
			m_PastCoasted.SetSize(255);
		}
		m_CurPos = newpos;
		m_Heading = ((90-(int)(atan2(m_TD.m_Velocity.height(),m_TD.m_Velocity.width())*180.0/3.14159))+360)%360;
		m_IsInUpdate=true;
		//The PPS, prediction vector, afterglows, graphical route are updated
		for (int view=0;view<m_ElemTable.GetSize();view++)
			m_ElemTable[view]->OnUpdate();
		m_IsInUpdate=false;
		//The tools attached to the track must be updated
		for (int vect=0;vect<m_VectorTable.GetSize();vect++)
			m_VectorTable[vect]->UpdateTrk(this);
		if (m_pTrkMiles)
			m_pTrkMiles->UpdateTrk(this,true);
		
		if (m_pTOORArrow)
		{
			bool visible=false;
			for (int newview=0;newview<pMngr->m_ViewTable.GetSize();newview++)
			{
				if (m_ElemTable[newview]->m_BoundingRect.intersects(pMngr->m_ViewTable[newview]->rect())==true)
				{
					visible=true;
					break;
				}
			}
			if (!visible)
			{
				m_pTOORArrow->UpdateTrk(this,true);
			}
			else
			{
				m_pTOORArrow->UpdateTrk(this,false);
			}
		}

		//If a Callsign is received, the FPL data associated to the track is updated or created at the first 
		//reception
		if (m_pHEvent)
		{
			int key1;
			int key2;
			int key3;
			GetMainKey(key1, key2, key3);
			AssociationType CurAssocType = m_pHEvent->GetCouplingDataAssociationType(key1,key2,key3);
			m_TD.m_Mismatch=false;


			//A Callsign is available in the Asterix Category
			if (CsCallSign.length())
			{
				//If the Track is not coupled or is Mode S Tagged, it is automatically coupled
				if (CurAssocType == UNCOUPLED || CurAssocType == MODESTAGGED)
					m_pHEvent->OnFPLUpdateFromTrack(CsCallSign, key1, key2, key3, COUPLED);
				else
				{
					if (CurAssocType == TAGGED || CurAssocType == COUPLED)
					{
						QString Arcid="";
						if (m_pHEvent->GetArcId(key1,key2,key3,Arcid))
						{
							//If the Track was tagged or Mode S Tagged and the Callsign received in the Asterix Category is the same
							//the track is automatically coupled
							if (Arcid == CsCallSign)
								m_pHEvent->OnFPLUpdateFromTrack(CsCallSign, key1, key2, key3, CurAssocType);
							else
							{
								if (!m_TD.m_ConflictHasBeenSolved || CsCallSign!=m_TD.m_ConflictingArcid)
								{
									m_TD.m_ConflictingArcid=CsCallSign;
									m_TD.m_ConflictingAssociation=COUPLED;
									m_TD.m_Mismatch=true;
									m_TD.m_ConflictHasBeenSolved=false;
								}
							}
						}
					}
				}
			}
			else
			{
				//No Callsign is received but a Mode S Arcid
				if (CsFlightId.length())
				{
					//If the Track is not coupled, it is automatically coupled except if it has been formerly manually uncoupled 
					if (CurAssocType == UNCOUPLED && !m_TD.m_IsManUntag)
						m_pHEvent->OnFPLUpdateFromTrack(CsFlightId.left(7), key1, key2, key3, MODESTAGGED);
					else 
					{
						//If the Track is tagged, the tag is maintained
						if (CurAssocType == TAGGED && m_TD.m_IsTagged)
							m_pHEvent->OnFPLUpdateFromTrack(m_TD.m_CsTag, key1, key2, key3, TAGGED);
						else
						{
							if (CurAssocType == MODESTAGGED)
							{
								QString Arcid="";
								if (m_pHEvent->GetArcId(key1,key2,key3,Arcid))
								{
									//If the Track was Mode S Tagged and the Callsign received in the Asterix Category is the same
									//the track is automatically coupled
									if (Arcid == CsFlightId.left(7))
										m_pHEvent->OnFPLUpdateFromTrack(CsFlightId.left(7), key1, key2, key3, MODESTAGGED);
									else
									{
										if (!m_TD.m_ConflictHasBeenSolved || CsFlightId!=m_TD.m_ConflictingArcid)
										{
											m_TD.m_ConflictingArcid=CsFlightId;
											m_TD.m_ConflictingAssociation=MODESTAGGED;
											m_TD.m_Mismatch=true;
											m_TD.m_ConflictHasBeenSolved=false;
										}
									}
								}
							}
						}
					}
				}
				else
				{
					if (CurAssocType == TAGGED && m_TD.m_IsTagged)
						m_pHEvent->OnFPLUpdateFromTrack(m_TD.m_CsTag, key1, key2, key3, TAGGED);
				}
			}
		}



		//If one information impacting the data fields has been modified
		//m_DataTable and the CEventHandler class must be updated
		if (oldIsSecondary == true && m_TD.m_IsSecondary == false)
			int doit = true;
		if (   oldSSR != m_TD.m_SSR
			|| oldCurAlt != m_TD.m_CurAlt
			|| oldSpeed != m_TD.m_Speed
			|| oldIsPrimary != m_TD.m_IsPrimary
			|| oldIsSecondary != m_TD.m_IsSecondary
			|| oldEmergAck != m_TD.m_EmergAck
			|| oldIsTrkRcd != m_TD.m_IsTrkRcd
			|| oldRocdFtMin != m_TD.m_RocdFtMin
			|| oldRocdFtNm != m_TD.m_RocdFtNm
			|| oldIsAfl != m_TD.m_IsAfl
			|| oldNegatifAfl != m_TD.m_bNegatifAfl
			|| oldVelocity != m_TD.m_Velocity
			|| oldFlightId != m_TD.m_FlightId
			|| oldSStatus != m_TD.m_SStatus)
		{
			UpdateDataTable();
		}
		m_Creation=false;
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to build according to the invocation
///  the callsign/callsign tag mismatch menu, the multiple STCA menu or 
///  the callsign menu.  
///  Argument 1 : UINT, selected menu item number
///  Argument 2 : CObject, not used in this function
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen115, CIAsdFctRwsTppGen120, CIAsdFctDfdCicAss005,
///					CIAsdFctDfdCicAss010, CIAsdFctDfdCicAss015, CIAsdFctDfdCicAss020,
///					CIAsdFctDfdCicHof010, CIAsdFctDfdCicHof015, CIAsdFctDfdCicHld005,
///					CIAsdFctDfdCicHld015, CIAsdFctDfdCicHld016, CIAsdFctDfdCicHld035,
///					CIAsdFctDfdCicCst010, CIAsdFctDfdCicCst025, CIAsdFctDfdCicCst040,
///					CIAsdFctDfdCicCst045, CIAsdFctDfdCicCst065, CIAsdFctDfdCicCst075,
///					CIAsdFctDfdCicCst080, CIAsdFctDfdCicCst085, CIAsdFctDfdCicLtg005,
///					
//
//-----------------------------------------------------------------------------
void CTrack::OnCommand(UINT id, CObject *pDisplayWnd)
{
	// CDS BREAK NGT 08/06/2006 Bad method length rule is not violated considering
	//							only the real length of the function.

	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	QPoint pnt=QCursor::pos();

	switch (id)
	{
	//callsign/callsign tag mismatch menu
	case IDC_ARCID1_MISMATCH:
		//The ATCO selects the callsign of the track to solve the mismatch
		if (m_pHEvent)
		{
			if (m_TD.m_IsTagged)
				m_pHEvent->OnTrackUntag(this, m_TD.m_CsTag, true);
			else 
				m_pHEvent->OnTrackUntag(this, m_TD.m_Arcid, true);
		}
		break;
	case IDC_ARCID2_MISMATCH:
		//The ATCO selects the callsign tag of the track to solve the mismatch
		if (m_pHEvent)
			m_pHEvent->OnTrackUntag(this, m_TD.m_ConflictingArcid, true);
		break;
	//Callsign menu
	case IDC_HIGHLIGHT:	
		if (!m_TD.m_IsHighlight)
		{
			m_TD.m_IsHighlight=true;
			if (m_pHEvent)
				m_pHEvent->OnTrackHighlight(this, m_TD.m_IsHighlight);

			for (int view=0;view<m_ElemTable.GetSize();view++)
				m_ElemTable[view]->OnUpdate();
		}
		break;
	case IDC_UNHIGHLIGHT:
		if (m_TD.m_IsHighlight)
		{
			m_TD.m_IsHighlight=false;
			if (m_pHEvent)
				m_pHEvent->OnTrackHighlight(this, m_TD.m_IsHighlight);

			for (int view=0;view<m_ElemTable.GetSize();view++)
				m_ElemTable[view]->OnUpdate();
		}
		break;
	case IDC_UNTAG:
			ModifyTag(pnt);
		break;
	case IDC_TAG:
			CreateTag(pnt);
		break;
	case IDC_ASSUME:
		if (pTrkMngr->m_pTrkSrc)
		{
			if (m_pHEvent && !m_TD.m_IsAoc)
				m_pHEvent->OnAssumeDeassume(this);
		}
		break;
	case IDC_UNASSUME:
		if (pTrkMngr->m_pTrkSrc)
		{
			if (m_pHEvent && m_TD.m_IsAoc)
				m_pHEvent->OnAssumeDeassume(this);
		}
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the track presentation for the radar window provided in 
///  Argument
///
///  RETURNS: 
///  CViewTrack, the track presentation (object) 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CViewTrack *CTrack::GetViewTrack(CView* pView)
{
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if(m_ElemTable[view]->m_pView == pView)
		{
			return m_ElemTable[view];
		}
	}
	return NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called upon the creation of a track or upon the reception of
///  an update of the track in order to re-initialize the track ageing process.    
///
///  RETURNS: 
///  CViewTrack, the track presentation (object) for the main radar window
///
///  REQUIREMENT ID: CIAsdFctRwsTppAfg010, CIAsdFctRwsTppAge010
//
//-----------------------------------------------------------------------------
void CTrack::ResetAging()
{
	m_NbMissUpdate = 0;
	m_agingState=WATCH_AGING;
	m_IsTrackAging=false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is used to check if the track must enter the ageing process, 
///  or if it has already started the ageing process, if the track must terminate
///  the ageing process (remove the track presentations).    
///
///  RETURNS: 
///  bool, whether the track ageing process has reached its end or not
///
///  REQUIREMENT ID: CIAsdFctRwsTppAfg009, CIAsdFctRwsTppAge006, CIAsdFctRwsTppAge008,
///					CIAsdFctRwsTppAge015, CIAsdFctRwsRsv110, CIAsdFctRwsRsv064
///					
//
//-----------------------------------------------------------------------------
bool CTrack::AutoAging()
{
	QString strac = "primary";
	if (m_DataTable[ArcidData].length())
		strac = m_DataTable[ArcidData];
	else if (m_DataTable[TssrData].length())
		strac = m_DataTable[TssrData];
	bool cr = false;
	switch (m_agingState)
	{
		//if the track was not in ageing, but has missed 2 track updates, the track 
		//enters the ageing process, its presentations must be updated and a message
		//is logged
	case WATCH_AGING :
		if (m_NbMissUpdate<2)
			break;
		m_IsTrackAging=true;
		UpdateAllViews();
		m_agingState = START_AGING;
		if (m_pHEvent)
		{
			QString strmsg;
			strmsg.sprintf("Track Ageing process started for %s.", strac.ascii());
			m_pHEvent->WriteLogMsg(strmsg, LogInformation);
			m_pHEvent->OnStartAging(this);
		}
		break;
		//if the track was already in ageing process, 
	case START_AGING :
		{
			//The presentation of an ageing track in emergency is not modified
			if ((m_DataTable[EmrgData].length()))
				return false;
			//The number of track afterglows is decreased at every missed track update
			else
			{
				if (m_PastPos.GetSize()>0)
				{
					int totalsize = 10;
					if (m_PastPos.GetSize()<=10)
						totalsize = m_PastPos.GetSize() - 1;
					m_PastPos.SetSize( totalsize );
					m_PastCoasted.SetSize( totalsize );
				}
				//If all the afterglows have been supressed, the track ends the ageing process
				//in this only case the function returns true.
				else
					cr = true;
			}
		}
		break;
		//If the ageing status is not identified a message is logged
	default :
		{
			if (m_pHEvent)
			{
				QString strmsg;
				strmsg.sprintf("Track Ageing in bad state %d.", m_agingState);
				m_pHEvent->WriteLogMsg(strmsg, LogError);
			}
		}
		break;
	}
	return cr;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to update the track with any last received information:
///  coupling, safety...
///  Argument : CTripleEntryList<CTrack>&, the list of the tracks, not used in this function    
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::OnResolveLink(CTripleEntryList<CTrack>& List)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();

	if (pTrkMngr->m_pHEvent)
		pTrkMngr->m_pHEvent->OnTrackNeedUpdate(this, false);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to check if some track updates have not been missed 
///  for a track according to the update periodicity of the currently selected 
///  radar service or to update the number of missed updates in case of switch of radar
///  service. 
///  Argument : CTripleEntryList<CTrack>&, the list of the tracks, not used in this function    
///
///  RETURNS: 
///  bool, whether the track has ended the ageing process or not
///
///  REQUIREMENT ID: CIAsdFctRwsTppAge020, CIAsdFctRwsRsv110, CIAsdFctRwsRsv063,
///					CIAsdFctRwsRsv064
//
//-----------------------------------------------------------------------------
bool CTrack::OnTimeOut(CTripleEntryList<CTrack>& List)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	if (pTrkMngr->m_pTrkSrc)
	{
		QDateTime currenttime = QDateTime::currentDateTime();
		int period = pTrkMngr->m_pTrkSrc->m_Period;
		int diff = GetMainElapsedRefreshTime(currenttime);
		int nbmiss = diff / period;
		if (m_NbMissUpdate != nbmiss)
		{
			m_NbMissUpdate = nbmiss;
			bool deltrack = AutoAging();
			if (deltrack)
			{
				QString strmsg;
				QString strac = "primary";
				if (m_DataTable[ArcidData].length())
					strac = m_DataTable[ArcidData];
				else if (m_DataTable[TssrData].length())
					strac = m_DataTable[TssrData];
				strmsg.sprintf("Track Ageing process completed for %s after %d s.", strac.ascii(), diff);
				if (pTrkMngr->m_pHEvent)
				{
					pTrkMngr->m_pHEvent->WriteLogMsg(strmsg, LogInformation);
					pTrkMngr->m_pHEvent->OnRemoveTrack(this, false);
				}
				return true;
			}
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update the track following a creation.
///  The CEventHAndler class checks if the track can be linked to a FPL or to 
///  safety information.
///  Argument : CTripleEntryList<CTrack>&, the list of the tracks, not used in this function    
///
///  RETURNS: 
///  bool, true
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrack::OnCreateLink(CTripleEntryList<CTrack>& List)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();

	if (pTrkMngr->m_pHEvent)
		pTrkMngr->m_pHEvent->OnTrackNeedUpdate(this, false);
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update any link of flight plan or safety information
///  following a remove link event. 
///  Argument : CTripleEntryList<CTrack>&, the list of the tracks, not used in this function    
///
///  RETURNS: 
///  bool, true
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrack::OnRemoveLink(CTripleEntryList<CTrack>& List)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	if (pTrkMngr->m_pHEvent)
		pTrkMngr->m_pHEvent->OnTrackNeedUpdate(this, false);
	return true;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to close any opened context menu associated to a track.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::OnCloseMenu()
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	if (pTrkMngr->m_pTrackMenu)
	{
		pTrkMngr->m_pTrackMenu = NULL;
	}
	pTrkMngr->m_pMenuCallingTrack = NULL;
	pTrkMngr->m_pMenuCallingWnd = NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to perform a tag action
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdSymSmbGen105, CIAsdSymSmbGen110, CIAsdSymSmbNrm040,
///					CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsTaggedAllowed()
{
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc && CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Tag_Track));
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to enter or modify a CFL
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdFctDfdCicLtg030, CIAsdSymSmbGen105, CIAsdSymSmbGen110,
///					CIAsdSymSmbNrm040, CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsCflAllowed()
{
	//returns true if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the currently selected radar service is available 
	//and corresponding eligibility is set to yes for the CWP configuration
	//and the track is assumed by the own sector or uncontrolled
	//and the track is coupled or tagged
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc && (m_TD.m_IsCoupled || m_TD.m_IsTagged));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to solve a CS/CSTAG mismatch
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdSymSmbGen105, CIAsdSymSmbGen110, CIAsdSymSmbNrm040,
///					CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsSolveCSMismatchAllowed()
{
	//returns true if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the currently selected radar service is available 
	//and corresponding eligibility is set to yes for the CWP configuration
	//and the track is assumed by the own sector or uncontrolled
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc && (m_TD.m_IsAoc || m_TD.m_Mnemo==0) 
			&& CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Solve_CsCsTagMismatch));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to enter or modify a REP
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdSymSmbGen105, CIAsdSymSmbGen110, CIAsdSymSmbNrm040,
///					CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsRepAllowed()
{
	//returns true if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the currently selected radar service is available 
	//and corresponding eligibility is set to yes for the CWP configuration
	//and the track is assumed by the own sector or uncontrolled
	//and the track AFL is not available from the currently selected radar service
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc 
		    && (m_TD.m_IsAfl) && (m_TD.m_IsAoc || m_TD.m_Mnemo==0)
			&& CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Rep_Selection));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to enter or modify a RFL
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdFctDfdCicRfl010, CIAsdSymSmbGen105, CIAsdSymSmbGen110, 
///					CIAsdSymSmbNrm040, CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsRflAllowed()
{
	//returns true if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the currently selected radar service is available 
	//and corresponding eligibility is set to yes for the CWP configuration
	//and the track is assumed by the own sector or uncontrolled
	//and the track is coupled and not tagged
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc && m_TD.m_IsCoupled && !m_TD.m_IsTagged);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to enter or modify a WTC
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdFctDfdCicLtg045, CIAsdSymSmbGen105, CIAsdSymSmbGen110, 
///					CIAsdSymSmbNrm040, CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsWtcAllowed()
{
	//returns true if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the currently selected radar service is available 
	//and corresponding eligibility is set to yes for the CWP configuration
	//and the track is assumed by the own sector or uncontrolled
	//and the track is coupled or is tagged
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc
		    && (m_TD.m_IsAoc || m_TD.m_Mnemo==0) && (m_TD.m_IsCoupled || m_TD.m_IsTagged)
			&& CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Wtc_Selection));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to enter or modify a ATYP
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdFctDfdCicLtg050, CIAsdSymSmbGen105, CIAsdSymSmbGen110, 
///					CIAsdSymSmbNrm040, CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsATypAllowed()
{
	//returns true if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the currently selected radar service is available 
	//and corresponding eligibility is set to yes for the CWP configuration
	//and the track is assumed by the own sector or uncontrolled
	//and the track is coupled or tagged
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc
		    && (m_TD.m_IsAoc || m_TD.m_Mnemo==0) && (m_TD.m_IsCoupled || m_TD.m_IsTagged) 
			&& CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::ATyp_Selection));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to enter or modify a ADES
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdSymSmbGen105, CIAsdSymSmbGen110, CIAsdSymSmbNrm040,
///					CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsAdesAdepAllowed()
{
	//returns true if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the currently selected radar service is available 
	//and corresponding eligibility is set to yes for the CWP configuration
	//and the track is assumed by the own sector or uncontrolled
	//and the track is coupled and not tagged
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc
		&& (m_TD.m_IsAoc || m_TD.m_Mnemo==0) && (m_TD.m_IsCoupled && !m_TD.m_IsTagged)
		&& CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Ades_Selection));

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to enter or modify a ASSR
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdSymSmbGen105, CIAsdSymSmbGen110, CIAsdSymSmbNrm040,
///					CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsAssrAllowed()
{
	//returns true if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the currently selected radar service is available 
	//and corresponding eligibility is set to yes for the CWP configuration
	//and the track is assumed by the own sector or uncontrolled
	//and the track is coupled and not tagged
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc 
		&& (m_TD.m_IsAoc || m_TD.m_Mnemo==0) && (m_TD.m_IsCoupled && !m_TD.m_IsTagged)
		&& (CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Assr_Selection)));
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to enter or modify a RWY
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdSymSmbGen105, CIAsdSymSmbGen110, CIAsdSymSmbNrm040,
///					CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsRwyAllowed()
{
	//returns true if:
	//Modifying manually the RWY data field is then allowed if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the track is coupled and not tagged
	//and the track is assumed by the own sector or uncontrolled and the Rwy_Selection eligibility is set to yes for the CWP configuration
	//or the track is assumed by another sector but the Rwy_Sq_NotAssumed_Authorized eligibility is set to yes for the CWP configuration
	return ((m_TD.m_IsCoupled && !m_TD.m_IsTagged) && (CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Rwy_Selection)));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the user is allowed to assume / de-assume a track
///  or not.
///  Returns false if the CWP is not sectorized, if the CCS is not available or 
///  if coupling information is not available for the currently selected radar
///  service
///
///  RETURNS: 
///  bool, whether the user is allowed to perform the action or not
///
///  REQUIREMENT ID: CIAsdFctDfdCicLtg035, CIAsdFctDfdCicLtg040, CIAsdSymSmbGen105,
///					 CIAsdSymSmbGen110, CIAsdSymSmbNrm040, CIAsdSymSmbBsm325
//
//-----------------------------------------------------------------------------
bool CTrack::IsAssumeDeAssumeAllowed()
{
	//returns true if:
	//the CWP is signed-in required and an authorized ATCO is currently signed-in
	//and the currently selected radar service is available 
	//and corresponding eligibility is set to yes for the CWP configuration
	//and the track is coupled or tagged
	return (CAsdApp::GetApp()->GetTrackMngr()->m_pTrkSrc 
		&& ((m_TD.m_IsCoupled) || (m_TD.m_IsTagged)) 
		&& CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Assume_De_assume));
}





//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on the identifier of the track.
///  Argument 1 : QPoint, position of the mouse pointer
///  Argument 2 : CViewTrack*, view on which the action is performed 
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsGrd005, CIAsdFctRwsTlsGrd015, CIAsdFctDfdCicMos020
//
//-----------------------------------------------------------------------------
void CTrack::ArcidClickAB(QPoint pnt, CViewTrack* pParent)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();

	//Toggle of the track id in the data block and on the alert lists 
	if (m_TD.m_IsTagged)
	{
		if(pParent)
		{
			//If the radar window mode is independent the toggle is only performed 
			//on the view put in argument
			if(!CRwdView::GetIsIndependent())
			{
				bool toggle=!pParent->m_IsToggled;

				for (int view=0;view<m_ElemTable.GetSize();view++)
				{
					m_ElemTable[view]->m_IsToggled=toggle;
					m_ElemTable[view]->OnUpdate();
				}

			}
			//The toggle action impacts the lists only if the main radar window is impacted
			else
			{
				pParent->m_IsToggled=!pParent->m_IsToggled;
				pParent->OnUpdate();

				if(((CRwdView*)pParent->m_pView) == CRwdView::m_psMainWnd)
				{
					bool bToggleMain=pParent->m_IsToggled;
							
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a CFL data field.
///  If the interaction is allowed a pop-up is opened in order to set or modify the
///  CFL data field.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CTrack::CflClickAB(QPoint pnt)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	bool ok;

	if (IsCflAllowed())
	{
		//Invocation of the CFL pop-up window
		pTrkMngr->RemoveMenu();
		m_pDialog=pTrkMngr->m_pFlSelectDialog;
		pTrkMngr->m_pFlSelectDialog->move(pnt);
		pTrkMngr->m_pFlSelectDialog->setCaption("CFL");
		int cfl=m_CflToBeDisplayed.toInt(&ok);
		if (!ok)
			cfl = 0;
		pTrkMngr->m_pFlSelectDialog->Init(m_DataTable[ArcidData], (m_TD.m_IsCoupled && !m_TD.m_IsTagged), m_TD.m_FltRul, m_TD.m_Adep, m_TD.m_DepRwy, m_TD.m_IsDeparture, m_TD.m_IsArrival,cfl, m_TD.m_KdOfAppClear,true);
		m_Glob_ObjectDeletedOnModal = false;
		pTrkMngr->m_bDialogIsOn = true;
		int res=pTrkMngr->m_pFlSelectDialog->exec();
		pTrkMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;

		if (res==QDialog::Accepted)
		{
			//Invocation of the CFL other pop-up window
			if (pTrkMngr->m_pFlSelectDialog->m_Cfl==-1)
			{
				//other
				m_pDialog2=pTrkMngr->m_pFlSelectOtherDialog;
				pTrkMngr->m_pFlSelectOtherDialog->setCaption("CFL");
				pTrkMngr->m_pFlSelectOtherDialog->Cls->setText(m_DataTable[ArcidData]);
				pTrkMngr->m_pFlSelectOtherDialog->move(QCursor::pos());
				pTrkMngr->m_pFlSelectOtherDialog->Init(cfl);
				m_Glob_ObjectDeletedOnModal = false;
				pTrkMngr->m_bDialogIsOn = true;
				int res2=pTrkMngr->m_pFlSelectOtherDialog->exec();
				pTrkMngr->m_bDialogIsOn = false;
				CRwdView::ForceMouseMoveEvent();
				if (m_Glob_ObjectDeletedOnModal)
					return;

				//The inputs if any, are transmitted to the CEventHandler class for treatment  
				if ((res2==QDialog::Accepted) && (pTrkMngr->m_pFlSelectOtherDialog->m_Level!=-1))
				{
					if (m_pHEvent)
						m_pHEvent->OnCfl(this, pTrkMngr->m_pFlSelectOtherDialog->m_Level, "");
				}
					
			}
			else
			{
				//The inputs if any, are transmitted to the CEventHandler class for treatment
				if (m_pHEvent)
					m_pHEvent->OnCfl(this, pTrkMngr->m_pFlSelectDialog->m_Cfl, pTrkMngr->m_pFlSelectDialog->m_KdOfAppClear);
			}
		}
		m_pDialog=NULL;
		m_pDialog2=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a AFL data field.
///  If the interaction is allowed a pop-up is opened in order to set or modify the
///  REP data field.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicRep005
//
//-----------------------------------------------------------------------------
void CTrack::AflClickAB(QPoint pnt)
{
	if (IsRepAllowed())
	{
		//Invocation of the REP pop-up window
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		pTrkMngr->RemoveMenu();
		m_pDialog=pTrkMngr->m_pFlSelectDialog;
		pTrkMngr->m_pFlSelectDialog->move(pnt);
		pTrkMngr->m_pFlSelectDialog->setCaption("REP");
		pTrkMngr->m_pFlSelectDialog->Init(m_DataTable[ArcidData], (m_TD.m_IsCoupled && !m_TD.m_IsTagged), m_TD.m_FltRul, m_TD.m_Adep, m_TD.m_DepRwy, false,false,0,"",false);
		m_Glob_ObjectDeletedOnModal = false;
		pTrkMngr->m_bDialogIsOn = true;
		int res=pTrkMngr->m_pFlSelectDialog->exec();
		pTrkMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		if (res==QDialog::Accepted)
		{

			if (pTrkMngr->m_pFlSelectDialog->m_Cfl==-1)
			{
				//Invocation of the REP other pop-up window
				pTrkMngr->m_pFlSelectOtherDialog->Cls->setText(m_DataTable[ArcidData]);
				pTrkMngr->m_pFlSelectOtherDialog->move(QCursor::pos());
				pTrkMngr->m_pFlSelectOtherDialog->setCaption("REP");
				pTrkMngr->m_pFlSelectOtherDialog->Init(atoi(m_DataTable[AflData]));
				m_Glob_ObjectDeletedOnModal = false;
				pTrkMngr->m_bDialogIsOn = true;
				int res2=pTrkMngr->m_pFlSelectOtherDialog->exec();	
				pTrkMngr->m_bDialogIsOn = false;
				CRwdView::ForceMouseMoveEvent();
				if (m_Glob_ObjectDeletedOnModal)
					return;

				//The inputs if any, are transmitted to the CEventHandler class for treatment  
				if ((res2==QDialog::Accepted) && (pTrkMngr->m_pFlSelectOtherDialog->m_Level!=-1))
				{
					if (m_pHEvent)
						m_pHEvent->OnAfl(this, pTrkMngr->m_pFlSelectOtherDialog->m_Level);
				}											
			}
			else
			{
				//The inputs if any, are transmitted to the CEventHandler class for treatment  
				if (res==QDialog::Accepted)
				{
					if (m_pHEvent)
						m_pHEvent->OnAfl(this, pTrkMngr->m_pFlSelectDialog->m_Cfl);
				}
			}
		}
		m_pDialog=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a RFL data field.
///  If the interaction is allowed a pop-up is opened in order to set or modify the
///  RFL data field.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicRfl010
//
//-----------------------------------------------------------------------------
void CTrack::RflClickAB(QPoint pnt)
{
	if (IsRflAllowed())
	{
		//Invocation of the RFL pop-up window
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		pTrkMngr->RemoveMenu();
		m_pDialog=pTrkMngr->m_pFlSelectDialog;
		pTrkMngr->m_pFlSelectDialog->move(pnt);
		pTrkMngr->m_pFlSelectDialog->setCaption("RFL");

		bool ok;
		int rfl=(m_DataTable[RflData]).toInt(&ok);
		if (!ok)
			rfl = 0;
		pTrkMngr->m_pFlSelectDialog->Init(m_DataTable[ArcidData], (m_TD.m_IsCoupled && !m_TD.m_IsTagged), m_TD.m_FltRul, m_TD.m_Adep, m_TD.m_DepRwy, false,false,rfl,"",false);
		
		m_Glob_ObjectDeletedOnModal = false;
		pTrkMngr->m_bDialogIsOn = true;
		int res=pTrkMngr->m_pFlSelectDialog->exec();
		pTrkMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		if (res==QDialog::Accepted)
		{

			if (pTrkMngr->m_pFlSelectDialog->m_Cfl==-1)
			{
				//Invocation of the RFL other pop-up window
				pTrkMngr->m_pFlSelectOtherDialog->Cls->setText(m_DataTable[ArcidData]);
				pTrkMngr->m_pFlSelectOtherDialog->move(QCursor::pos());
				pTrkMngr->m_pFlSelectOtherDialog->setCaption("RFL");
				pTrkMngr->m_pFlSelectOtherDialog->Init(atoi(m_DataTable[RflData]));
				m_Glob_ObjectDeletedOnModal = false;
				pTrkMngr->m_bDialogIsOn = true;
				int res2=pTrkMngr->m_pFlSelectOtherDialog->exec();	
				pTrkMngr->m_bDialogIsOn = false;
				CRwdView::ForceMouseMoveEvent();
				if (m_Glob_ObjectDeletedOnModal)
					return;

				//The inputs if any, are transmitted to the CEventHandler class for treatment  
				if ((res2==QDialog::Accepted) && (pTrkMngr->m_pFlSelectOtherDialog->m_Level!=-1))
				{
					if (m_pHEvent)
						m_pHEvent->OnRfl(this, pTrkMngr->m_pFlSelectOtherDialog->m_Level);
				}											
			}
			else
			{
				//The inputs if any, are transmitted to the CEventHandler class for treatment  
				if (res==QDialog::Accepted)
				{
					if (m_pHEvent)
						m_pHEvent->OnRfl(this, pTrkMngr->m_pFlSelectDialog->m_Cfl);
				}
			}
		}
		m_pDialog=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a TSSR data field.
///  If the interaction is allowed a toggle of the track identifier is performed.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::TssrClickAB(QPoint pnt)
{
	if (!m_TD.m_IsTagged && !m_TD.m_IsCoupled)
	{
//		Toggle TSSR/Flight Id
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a TSSR data field.
///  If the interaction is allowed a pop-up is opened in order to tag the track.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::CreateTag(QPoint pnt)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();

	if (IsTaggedAllowed() && (!m_TD.m_IsCoupled))
	{
		//Invocation of the Create TSSR pop-up window
		pTrkMngr->RemoveMenu();
		m_pDialog=pTrkMngr->m_pTssrDialog;
		pTrkMngr->m_pTssrDialog->move(pnt);
		m_Glob_ObjectDeletedOnModal = false;
		pTrkMngr->m_bDialogIsOn = true;
		int res=pTrkMngr->m_pTssrDialog->DoModal(m_DataTable[TssrData]);
		pTrkMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		
		//Invocation of the Modify TSSR pop-up window
		if (pTrkMngr->m_pTssrDialog->m_Other)
		{
			m_pDialog2=pTrkMngr->m_pTssrOtherDialog;
			pTrkMngr->m_pTssrOtherDialog->move(QCursor::pos());
			m_Glob_ObjectDeletedOnModal = false;
			pTrkMngr->m_bDialogIsOn = true;
			int res2 =pTrkMngr->m_pTssrOtherDialog->DoModal(m_DataTable[TssrData],"");
			pTrkMngr->m_bDialogIsOn = false;
			CRwdView::ForceMouseMoveEvent();
			if (m_Glob_ObjectDeletedOnModal)
				return;
			//The inputs if any, are transmitted to the CEventHandler class for treatment  
			if (res2==QDialog::Accepted)
			{
				if (pTrkMngr->m_pTssrOtherDialog->m_Arcid.length() && m_pHEvent)
					m_pHEvent->OnTrackTSSR(this, pTrkMngr->m_pTssrOtherDialog->m_Arcid, false, &pnt);
			}
		}
		//The inputs if any, are transmitted to the CEventHandler class for treatment  
		else if (res==QDialog::Accepted)
		{
			if (m_pHEvent)
				m_pHEvent->OnTrackTSSR(this, pTrkMngr->m_pTssrDialog->m_Arcid, false, &pnt);
		}
		m_pDialog=NULL;
		m_pDialog2=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a Callsign tag data field.
///  If the interaction is allowed a pop-up is opened in order to modify the
///  callsign tag of the track or to perform an Untag action.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::ModifyTag(QPoint pnt)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();

	if (IsTaggedAllowed() && (m_TD.m_IsTagged))
	{
		//Invocation of the Modify TSSR pop-up window
		pTrkMngr->RemoveMenu();
		m_pDialog=pTrkMngr->m_pTssrOtherDialog;
		pTrkMngr->m_bDialogIsOn = true;
		pTrkMngr->m_pTssrOtherDialog->move(pnt);
		m_Glob_ObjectDeletedOnModal = false;
		int res=pTrkMngr->m_pTssrOtherDialog->DoModal(m_DataTable[TssrData], GetArcId());
		pTrkMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		//The inputs if any, are transmitted to the CEventHandler class for treatment  
		if (res==QDialog::Accepted)
		{
			if (pTrkMngr->m_pTssrOtherDialog->m_Untag)
			{
					if (m_pHEvent)
						m_pHEvent->OnTrackTSSR(this, m_TD.m_CsTag, true, &pnt);
			}
			else 
			{
				if (pTrkMngr->m_pTssrOtherDialog->m_Arcid.length())
					if (m_pHEvent)
						m_pHEvent->OnTrackTSSR(this, pTrkMngr->m_pTssrOtherDialog->m_Arcid, false, &pnt);
			}
		}
		m_pDialog=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a Mi data field.
///  If the interaction is allowed the alert inhibition status is toggled.
///  Argument : CViewTrack, view on which the action is performed
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicAlt005, CIAsdFctDfdCicAlt010
//
//-----------------------------------------------------------------------------
void CTrack::MiClickAB(CViewTrack* m_pParent)
{
	m_AlertInhibition=!m_AlertInhibition;
	UpdateFromSafety();
	m_pParent->OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a WTC data field.
///  If the interaction is allowed a pop-up is opened in order to set or modify the
///  WTC data field.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::WtcClickAB(QPoint pnt)
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();

	if (IsWtcAllowed())
	{
		//Invocation of the WTC pop-up window
		pMngr->RemoveMenu();
		m_pDialog=pMngr->m_pWtcDialog;
		pMngr->m_pWtcDialog->move(pnt);
		QString wtc=m_DataTable[WtcData];
		QString arctyp=m_DataTable[ArctypData];
		pMngr->m_pWtcDialog->Init(m_DataTable[ArcidData],wtc,arctyp);
		m_Glob_ObjectDeletedOnModal = false;
		pMngr->m_bDialogIsOn = true;
		int res=pMngr->m_pWtcDialog->exec();
		pMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		//The inputs if any, are transmitted to the CEventHandler class for treatment  
		if (res==QDialog::Accepted)
		{
			if (m_pHEvent)
				m_pHEvent->OnATypWtc(this, m_TD.m_Adep, m_TD.m_Ades, arctyp, pMngr->m_pWtcDialog->m_Wtc);
		}
		m_pDialog=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a ATYP data field.
///  If the interaction is allowed a pop-up is opened in order to set or modify the
///  ATYP data field and if needed another pop-up opens to update the WTC.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicAcw010
//
//-----------------------------------------------------------------------------
void CTrack::ATypClickAB(QPoint pnt)
{
	if (IsATypAllowed())
	{
		//Invocation of the ATYP pop-up window
		CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
		pMngr->RemoveMenu();
		m_pDialog=pMngr->m_pATypDialog;
		pMngr->m_pATypDialog->move(pnt);
		QString atyp=m_DataTable[ArctypData];
		pMngr->m_pATypDialog->Init(m_DataTable[ArcidData],atyp);
		m_Glob_ObjectDeletedOnModal = false;
		pMngr->m_bDialogIsOn = true;
		int res=pMngr->m_pATypDialog->exec();
		pMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		
		if (res==QDialog::Accepted)
		{
			//Invocation of the WTC pop-up window
			m_pDialog2=pMngr->m_pWtcDialog;
			pMngr->m_pWtcDialog->move(QCursor::pos());
			QString wtc=m_DataTable[WtcData];
			pMngr->m_pWtcDialog->Init(m_DataTable[ArcidData],wtc,pMngr->m_pATypDialog->m_ATyp);
			
			if (pMngr->m_pWtcDialog->pListWtc->count() > 1)
			{					
				m_Glob_ObjectDeletedOnModal = false;
				pMngr->m_bDialogIsOn = true;
				int res2=pMngr->m_pWtcDialog->exec();
				pMngr->m_bDialogIsOn = false;
				CRwdView::ForceMouseMoveEvent();
				if (m_Glob_ObjectDeletedOnModal)
					return;
				//The inputs if any, are transmitted to the CEventHandler class for treatment  
				if ((res2==QDialog::Accepted) && (m_pHEvent))
					m_pHEvent->OnATypWtc(this, m_TD.m_Adep, m_TD.m_Ades, pMngr->m_pATypDialog->m_ATyp, pMngr->m_pWtcDialog->m_Wtc);
			}
			else
			{			
				//The inputs if any, are transmitted to the CEventHandler class for treatment  
				wtc=pMngr->m_pWtcDialog->pListWtc->text(0);
				if (m_pHEvent)
					m_pHEvent->OnATypWtc(this, m_TD.m_Adep, m_TD.m_Ades, pMngr->m_pATypDialog->m_ATyp, wtc);
			}
		}
		m_pDialog=NULL;
		m_pDialog2=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on an ADES data field.
///  If the interaction is allowed a pop-up is opened in order to set or modify the
///  ADES data field.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::AdesClickAB(QPoint pnt)
{
	if (IsAdesAdepAllowed())
	{
		//Invocation of the ADES pop-up window
		CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
		pMngr->RemoveMenu();
		m_pDialog=pMngr->m_pAdesDialog;
		pMngr->m_pAdesDialog->move(pnt);
		pMngr->m_pAdesDialog->setCaption("ADES");
		QString ades=m_DataTable[AdesData];
		pMngr->m_pAdesDialog->Init(m_DataTable[ArcidData],m_TD.m_Altrnt1, m_TD.m_Altrnt2, ades);
		m_Glob_ObjectDeletedOnModal = false;
		pMngr->m_bDialogIsOn = true;
		int res=pMngr->m_pAdesDialog->exec();
		pMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		//The inputs if any, are transmitted to the CEventHandler class for treatment  
		if (res==QDialog::Accepted)
		{ 
			if (m_pHEvent)
				m_pHEvent->OnATypWtc(this, m_TD.m_Adep, pMngr->m_pAdesDialog->m_Ades, m_TD.m_Arctyp, m_TD.m_Wtc);
		}
		m_pDialog=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on an ADEP data field.
///  If the interaction is allowed a pop-up is opened in order to set or modify the
///  ADEP data field.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::AdepClickAB(QPoint pnt)
{
	if (IsAdesAdepAllowed())
	{
		//Invocation of the ADES pop-up window
		CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
		pMngr->RemoveMenu();
		m_pDialog=pMngr->m_pAdesDialog;
		pMngr->m_pAdesDialog->setCaption("ADEP");
		pMngr->m_pAdesDialog->move(pnt);
		QString adep=m_DataTable[AdepData];
		pMngr->m_pAdesDialog->Init(m_DataTable[ArcidData],m_TD.m_Altrnt1, m_TD.m_Altrnt2, adep);
		m_Glob_ObjectDeletedOnModal = false;
		pMngr->m_bDialogIsOn = true;
		int res=pMngr->m_pAdesDialog->exec();
		pMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		//The inputs if any, are transmitted to the CEventHandler class for treatment  
		if (res==QDialog::Accepted)
		{ 
			if (m_pHEvent)
				m_pHEvent->OnATypWtc(this, pMngr->m_pAdesDialog->m_Ades, m_TD.m_Ades, m_TD.m_Arctyp, m_TD.m_Wtc);
		}
		m_pDialog=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on an ASSR data field.
///  If the interaction is allowed a pop-up is opened in order to set or modify the
///  ASSR data field.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::AssrClickAB(QPoint pnt)
{
	if (IsAssrAllowed())
	{
		//Invocation of the ASSR pop-up window
		CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
		pTrkMngr->RemoveMenu();
		m_pDialog=pTrkMngr->m_pAssrDialog;
		pTrkMngr->m_pAssrDialog->setCaption("ASSR");
		pTrkMngr->m_pAssrDialog->Cls->setText(m_DataTable[ArcidData]);
		pTrkMngr->m_pAssrDialog->move(pnt);
		pTrkMngr->m_pAssrDialog->Init();
		pTrkMngr->m_bDialogIsOn = true;
		m_Glob_ObjectDeletedOnModal = false;
		int res=pTrkMngr->m_pAssrDialog->exec();
		pTrkMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		//The inputs if any, are transmitted to the CEventHandler class for treatment  
		if ((res==QDialog::Accepted) && (pTrkMngr->m_pAssrDialog->m_Assr!=-1))
		{
			if (m_pHEvent)
				m_pHEvent->OnAssr(this, pTrkMngr->m_pAssrDialog->m_Assr);
		}
		m_pDialog=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click AB on a RWY data field.
///  If the interaction is allowed a pop-up is opened in order to set or modify the
///  RWY data field.
///  Argument : QPoint, position of the mouse pointer
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicAcw015, CIAsdFctDfdCicAcw020
//
//-----------------------------------------------------------------------------
void CTrack::RwyClickAB(QPoint pnt)
{
	if (IsRwyAllowed())
	{
		//Invocation of the RWY pop-up window
		CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
		pMngr->RemoveMenu();
		m_pDialog=pMngr->m_pRwyDialog;
		pMngr->m_pRwyDialog->move(pnt);
		QString ades=m_DataTable[AdesData];
		QString OldRwy=m_TD.m_ArrRwy;
		QString ParallelRwy=m_TD.m_ParallelRwy;
		pMngr->m_pRwyDialog->setCaption("RWY "+ades);
		pMngr->m_pRwyDialog->Init(m_DataTable[ArcidData],ades,OldRwy,ParallelRwy);
		m_Glob_ObjectDeletedOnModal = false;
		pMngr->m_bDialogIsOn = true;
		int res=pMngr->m_pRwyDialog->exec();
		pMngr->m_bDialogIsOn = false;
		CRwdView::ForceMouseMoveEvent();
		if (m_Glob_ObjectDeletedOnModal)
			return;
		//The inputs if any, are transmitted to the CEventHandler class for treatment  
		if (res==QDialog::Accepted)
		{ 
			if (m_pHEvent)
				m_pHEvent->OnRwy(this, OldRwy, pMngr->m_pRwyDialog->m_Rwy);
		}
		m_pDialog=NULL;
	}
	//The selection status of the track is recomputed following the closure of the pop-up
	QPoint pos=QCursor::pos();
	for (int view=0;view<m_ElemTable.GetSize();view++)
	{
		if (!(m_ElemTable[view]->GetPriority(pos,true)))
			m_ElemTable[view]->SelectElement(false);
		m_ElemTable[view]->OnUpdate();
	}


}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click WB on a track identifier.
///  If the interaction is allowed and the track is under an STCA alert, the alert
///  is acknowledged.
///  Argument 1 : QPoint, position of the mouse pointer
///  Argument 2 : CView*, the radar window on which the action is performed
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctAlwStc035, CIAsdFctAlwStc045
//
//-----------------------------------------------------------------------------
void CTrack::ArcidClickWB(QPoint pnt, CView* pView)
{

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to acknowledge a Duplicated SSR code alert.
///  following the action, the alert remains displayed but steady.
///
///  RETURNS: 
///  bool, whether an alert is acknowledged or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CTrack::AcknowledgeDU()
{
	//If the AL-S data field does not show a DU alert, or if the DU alert is 
	//associated to no other track, no action is performed
	if (m_DataTable[AlertData] != "DU" || m_DUTrackList.count() == 0)
		return false;
	else
	{
		//In the other case, the DU alert is acknowledged for all the tracks associated 
		//to the same TSSR code and returns true
		QValueList<CTrack*>::iterator itTrack;
		if (m_DUTrackList.count())
		{
			for ( itTrack = m_DUTrackList.begin(); itTrack != m_DUTrackList.end(); ++itTrack)
			{
				(*itTrack)->m_DUStatus=AlertAck;
				(*itTrack)->UpdateAllViews();
			}
		}
		m_DUStatus=AlertAck;
		UpdateAllViews();
		return true;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following a click SB on a track identifier.
///  This action opens the callsign context menu with the items
///  corresponding to the track status.
///  Argument 1 : QPoint, position of the mouse pointer
///  Argument 2 : CObject, the CViewTrack object of the track impacted
///  Argument 3 : CView*, the radar window on which the action is performed
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::ArcidClickSB(QPoint pnt, CObject *pWnd, CView* pView)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();

	pTrkMngr->RemoveMenu();
	pTrkMngr->m_pMenuCallingTrack = this;
	pTrkMngr->m_pMenuCallingWnd = pView;
	pTrkMngr->m_pTrackMenu= new CAsdMenu();

	//Highlight/UnHighlight item
	if (m_TD.m_IsHighlight)
		pTrkMngr->m_pTrackMenu->AddItem("Unhighlight",IDC_UNHIGHLIGHT,true);
	else
		pTrkMngr->m_pTrackMenu->AddItem("Highlight",IDC_HIGHLIGHT,true);


	//Uncouple/Tag/Untag Item
	if (m_TD.m_IsTagged)
		pTrkMngr->m_pTrackMenu->AddItem("Untag",IDC_UNTAG,true);
	else if (!(m_TD.m_IsCoupled) &&  !(m_TD.m_IsTagged))
		pTrkMngr->m_pTrackMenu->AddItem("Tag",IDC_TAG,true);


	//Assume/De-assume item
	if (!m_TD.m_IsAoc)
		pTrkMngr->m_pTrackMenu->AddItem("Assume",IDC_ASSUME,true);
	else
		pTrkMngr->m_pTrackMenu->AddItem("De-assume",IDC_UNASSUME,true);

	//Remove from list item grayed-out for built 1
	pTrkMngr->m_pTrackMenu->AddItem("Remove from List",IDC_REMOVE_FROM_LIST,true,false,true);

	pTrkMngr->m_pTrackMenu->TrackPopupMenu(pnt.x(),pnt.y(),this,NULL,pWnd);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function computes the availability of the callsign menu items.
///  Argument : CCmdUI, menu item
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicHof010, CIAsdFctDfdCicHof015, CIAsdFctDfdCicHld005,
///					CIAsdFctDfdCicHld015, CIAsdFctDfdCicHld016, CIAsdFctDfdCicHld035,
///					CIAsdFctDfdCicCst010, CIAsdFctDfdCicCst025, CIAsdFctDfdCicCst040,
///					CIAsdFctDfdCicCst045, CIAsdFctDfdCicCst085
//
//-----------------------------------------------------------------------------
void CTrack::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	switch (pCmdUI->GetID())
	{
	case IDC_HIGHLIGHT:
	case IDC_UNHIGHLIGHT:
		pCmdUI->Enable(true);
		break;
	case IDC_UNTAG:
		pCmdUI->Enable((m_TD.m_IsAoc || m_TD.m_Mnemo==0) && m_TD.m_IsTagged && IsTaggedAllowed());
		break;
	case IDC_TAG:
		pCmdUI->Enable(!m_TD.m_IsTagged && IsTaggedAllowed());
		break;
	case IDC_ASSUME:
		pCmdUI->Enable(IsAssumeDeAssumeAllowed() && (!m_TD.m_IsAoc));
		break;
	case IDC_UNASSUME:
		pCmdUI->Enable(IsAssumeDeAssumeAllowed() && m_TD.m_IsAoc);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function associates a Track Mile line to the track or cancels the
///  association.
///  Argument 1 : CTrkMiles*, track mile line object
///  Argument 2 : bool, whether the association must be created or canceled
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::AssociateTrkMiles(CTrkMiles* pTrkMiles,bool asso)
{
	if (asso)
	{
		m_pTrkMiles=pTrkMiles;
	}
	else
	{
		m_pTrkMiles=NULL;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function associates a TOOR Arrow set to the track or cancels the
///  association.
///  Argument 1 : CTOORArrow*, TOOR Arrow object
///  Argument 2 : bool, whether the association must be created or canceled
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTrack::AssociateTOORArrow(CTOORArrow* pTOORArrow,bool asso)
{
	if (asso)
	{
		m_pTOORArrow=pTOORArrow;
	}
	else
	{
		m_pTOORArrow=NULL;
	}
}


//-----------------------------------------------------------------------------
//
/// \class CViewTrack
/// This class is used to manage the display of one track throught all the radar
/// windows.  
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of the member parameters
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CViewTrack::CViewTrack()
{
	m_Type=TrackView;
	m_LeaderPos = -1;
	m_SelElem = m_LastSelElem = m_ModifElem = -1;
	m_Hooked = false;
	m_HookedSymbol = false;
	m_ManualLeaderPos = false;
	m_BoundingRect=QRect();;
	m_Extended=false;
	m_bStationaryDb=false;
	m_DBIsBeingDragged=false;
	m_ForcedData.SetSize(MaxFld);
	m_ToggleRocdUnitFtMin=true;
	int i;
	m_IsToggled=false;
	for (i=0;i<m_ForcedData.GetSize();i++)
		m_ForcedData[i]=0;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CViewTrack&, CViewTrack object to be initialized
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CViewTrack::CViewTrack(CViewTrack& viewtrk)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CViewTrack::~CViewTrack()
{
	m_SymbolDisplayed = false;
	m_BoundingRect=m_BoundingRect.normalize();
	m_pView->InvalidateRect(&m_BoundingRect);
	for (int elem=0;elem<m_ElemTable.GetSize();elem++)
	{
		delete m_ElemTable[elem];
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Initialization of the track elements and the view on which the track will be
///  displayed. A CviewTrack is associated to a
///  CSymbol object, CPastPos object, CLeader object, CVelocity object, CHalo
///  object and CLabel object.
///  Argument 1 : LPVOID, track object
///  Argument 2 : CView*, view associated to the track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen010, CIAsdFctRwsTppGen012
//
//-----------------------------------------------------------------------------
void CViewTrack::Init(LPVOID pData,CView* pView,CDisplayElement* )
{
	m_pView = (CAsdView*)pView;
	m_pTrack = (CTrack*)pData;
	CLabel *NewLabel=NULL;
	CLeader *NewLeader=NULL;
	CVelocity *NewVelocity=NULL;
	CHalo *NewHalo=NULL;
	CSymbol* NewSymbol=NULL;
	CPastPos* NewPastPos=NULL;
	int elem;
	for (elem=0;elem<m_pTrackDef->ElemDefTable.GetSize();elem++)
	{
		switch (m_pTrackDef->ElemDefTable[elem].type)
		{
			//PPS
		case symb:
			NewSymbol = new CSymbol;
			NewSymbol->Init(m_pTrack,m_pView,this);
			NewSymbol->m_pDef = &m_pTrackDef->ElemDefTable[elem];
			m_ElemTable.Add(NewSymbol);
			break;
			//Afterglows
		case pastp:
			NewPastPos = new CPastPos;
			NewPastPos->Init(m_pTrack,m_pView,this);
			NewPastPos->m_pDef = &m_pTrackDef->ElemDefTable[elem];
			m_ElemTable.Add(NewPastPos);
			break;
			//Leader line
		case lead:
			NewLeader = new CLeader;
			NewLeader->Init(m_pTrack,m_pView,this);
			NewLeader->m_pDef = &m_pTrackDef->ElemDefTable[elem];
			m_ElemTable.Add(NewLeader);
			break;
			//Prediction vector
		case velocity:
			NewVelocity = new CVelocity;
			NewVelocity->Init(m_pTrack,m_pView,this);
			NewVelocity->m_pDef = &m_pTrackDef->ElemDefTable[elem];
			m_ElemTable.Add(NewVelocity);
			break;
			//Halo
		case halo:
			NewHalo = new CHalo;
			NewHalo->Init(m_pTrack,m_pView,this);
			NewHalo->m_pDef = &m_pTrackDef->ElemDefTable[elem];
			m_ElemTable.Add(NewHalo);
			break;
			//Data block
		case label:
			NewLabel = new CLabel();
			NewLabel->Init(m_pTrack,m_pView,this);
			NewLabel->m_pDefTable = &m_pTrackDef->LabelDefTable;
			m_ElemTable.Add(NewLabel);
			break;
		default:
			break;
		
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time an attribute of the class is modified or
///  may be impacted. The update is performed systematically before refreshing 
///  the display (call of OnDraw function).
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen010, CIAsdFctRwsTppGen012, CIAsdFctRwsTppGen020, 
///					CIAsdFctRwsTppGen055, CIAsdFctRwsTppGen100, CIAsdFctRwsTppDbk120, 
///					CIAsdFctRwsTppDbk125, CIAsdFctRwsTppLls005, CIAsdFctRwsTppLls010, 
///					CIAsdFctRwsTppLls020, CIAsdFctRwsTppLls037, CIAsdFctRwsTppLls038, 
///					CIAsdFctRwsTppLls040, CIAsdFctRwsTppLls042, CIAsdFctRwsTppLls043, 
///					CIAsdFctRwsTppLls050, CIAsdFctRwsTppLls055, CIAsdFctRwsTppLls060,
///					CIAsdFctRwsFltGen020, CIAsdFctRwsFltGen021, 
//
//-----------------------------------------------------------------------------
void CViewTrack::OnUpdate()
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	int elem;
	m_CurPos = m_pTrack->m_CurPos;
	m_pView->LPtoDP(&m_CurPos);
	QRect boundingrect,oldboundingrect,tmprect,viewrect;
	QPoint labelpos,leaderpos,leadbegpos;
	oldboundingrect = m_BoundingRect;
	int i,curleaderpos,offsetx=0,curviewnbr=0;
	bool fnd;
	//If the PPS is not within the View the track presentation is not displayed
	viewrect=m_pView->rect();
	if (!viewrect.contains(m_CurPos))
	{
		m_BoundingRect=QRect();
		boundingrect|=oldboundingrect;
		boundingrect=boundingrect.normalize();
		m_pView->InvalidateRect(&boundingrect);
		return;
	}

	//Default positioning of the leader line at creation of the track presentation
	if (m_LeaderPos==-1)
		m_LeaderPos=((m_pTrack->m_Heading+67)%360)/45;

	//In case of stationary data block the current m_LabelRect (rect of the data block)
	//is maintained as m_StationaryDbRect
	if (m_bStationaryDb && !m_DBIsBeingDragged)
	{
		int Leaderlen=((CLeader*)m_ElemTable[lead])->GetLeaderLineLenght();
		if (Leaderlen<350*350)
		{
			if (m_StationaryDbRect==QRect())
			{
				m_StationaryDbRect=m_LabelRect;
			}
		}
		else
		{
			SelectElement(false);
			m_bStationaryDb=false;
			m_StationaryDbRect=QRect();
		}
	}
	else
		m_StationaryDbRect=QRect();
	m_LabelRect = QRect();
	boundingrect=QRect();

	//If the PPS is filtered the associated menus must be closed
	bool SymbolDisplayed = IsSymbolDisplayed();
	if (m_SymbolDisplayed != SymbolDisplayed && !SymbolDisplayed)
	{
		if (pMngr->m_pMenuCallingTrack && pMngr->m_pMenuCallingTrack == m_pTrack)
		{
			if (pMngr->m_pTrackMenu)
			{
				if (   pMngr->m_pTrackMenu->GetCommandObject() == this 
					|| pMngr->m_pTrackMenu->GetCommandObject() == m_pTrack
					|| pMngr->m_pTrackMenu->GetCommandObject() == m_ElemTable[label]
					)
				{
					delete pMngr->m_pTrackMenu;
					pMngr->m_pTrackMenu = NULL;
					pMngr->m_pMenuCallingTrack = NULL;
					pMngr->m_pMenuCallingWnd = NULL;
				}
			}
		}
	}
	m_SymbolDisplayed = SymbolDisplayed;
	m_LabelDisplayed = IsLabelDisplayed();
	m_Color = ComputeColor(TrkDef);

	//compute label rect at (0,0)
	for (elem=0;elem<m_ElemTable.GetSize();elem++)
		m_ElemTable[elem]->m_BoundingRect=QRect();
	for (elem=0;elem<m_ElemTable.GetSize();elem++)
		m_ElemTable[elem]->OnUpdate();

	m_LabelRect=m_ElemTable[label]->m_BoundingRect;
	m_PosRect=((CLabel*)m_ElemTable[label])->m_PosRect;
	
	//LCM process
	if ((m_pTrack->m_IsInUpdate) && (!m_pTrack->m_Select))
	{
		int list[8]={0,0,2,3,4,6,7,1};
		if ((m_LabelDisplayed) && (!m_ManualLeaderPos) && (m_pTrack->m_RwyOrientation!=-1))
			m_LeaderPos=m_pTrack->m_RwyOrientation;
		if ((m_LabelDisplayed) && (!m_ManualLeaderPos) && (m_pTrack->m_RwyOrientation==-1))
		{
			curleaderpos=((m_pTrack->m_Heading+67)%360)/45;
			list[1]=(m_LeaderPos+8-curleaderpos)%8;
			if ((list[1]==1) || (list[1]==5))
				list[1]=2;
			for (i=0;i<pMngr->m_ViewTable.GetSize();i++)
			{
				if (pMngr->m_ViewTable[i]==m_pView)
				{
					curviewnbr=i;
					break;
				}
			}
			for (i=0;i<8;i++)
			{
				curleaderpos=(curleaderpos+list[i])%8;
				ComputeOffset(m_pTrackDef->OrientationTable[curleaderpos].Angle,m_pTrackDef->OrientationTable[curleaderpos].Len,m_LabelRect,m_PosRect,leaderpos,leadbegpos,labelpos);
				offsetx=0;
				tmprect=m_LabelRect;
				tmprect.moveBy(m_CurPos.x()+labelpos.x()+offsetx,m_CurPos.y()+labelpos.y());
				fnd=false;

				CTripleEntryListIterator<CTrack> it( pMngr->m_TrackList );
				CTrack* pTrack=NULL;
				pTrack = it.current();
				while ( pTrack != 0 ) 
				{
					++it;
					if ( (pTrack!=m_pTrack) && (curviewnbr<pTrack->m_ElemTable.GetSize()))
					{
						if (!viewrect.contains(tmprect,TRUE) || tmprect.intersects(pTrack->m_ElemTable[curviewnbr]->m_LabelRect) || tmprect.intersects(pTrack->m_ElemTable[curviewnbr]->m_ElemTable[symb]->m_BoundingRect))
						{
							fnd=true;
							break;
						}
					}
					pTrack = it.current();
				}
				if (!fnd)
				{
					m_LeaderPos=curleaderpos;
					break;
				}
			}
		}
	}
	if (!m_bStationaryDb || m_DBIsBeingDragged)
		ComputeOffset(m_pTrackDef->OrientationTable[m_LeaderPos].Angle,m_pTrackDef->OrientationTable[m_LeaderPos].Len,m_LabelRect,m_PosRect,leaderpos,leadbegpos,labelpos);
	offsetx=0;

	//Update and positionning of all the elements of the track
	for (elem=0;elem<m_ElemTable.GetSize();elem++)
	{
		if ((m_LabelRect.isEmpty()) && (m_ElemTable[elem]->m_Type==lead))
			m_ElemTable[elem]->m_BoundingRect=QRect();
		else
			m_ElemTable[elem]->OnUpdate();
		if ((m_ElemTable[elem]->m_Type==label) && (!m_ElemTable[elem]->m_BoundingRect.isEmpty()))
		{
			if (!m_bStationaryDb || m_DBIsBeingDragged)
				m_ElemTable[elem]->m_BoundingRect.moveBy(m_CurPos.x()+labelpos.x()+offsetx,m_CurPos.y()+labelpos.y());
			else
				m_ElemTable[elem]->m_BoundingRect.moveBy(m_StationaryDbRect.x()+2+offsetx,m_StationaryDbRect.y()+4);
		}
		boundingrect|=m_ElemTable[elem]->m_BoundingRect;
	}

	//Offset of the DB rect to display the Highlight feature
	if 	(m_bStationaryDb && !m_DBIsBeingDragged)
	{
		m_LabelRect.addCoords(-2,-4,10,8);
		m_StationaryDbRect.setWidth(m_LabelRect.width());
		m_StationaryDbRect.setHeight(m_LabelRect.height());
		m_LabelRect=m_StationaryDbRect;
	}
	else
		m_LabelRect.moveBy(m_CurPos.x()+labelpos.x()+offsetx,m_CurPos.y()+labelpos.y());

	m_LabelRect.addCoords(-2,-4,10,8);
	boundingrect.addCoords(-10,-10,10,10);

	//The rect to be refresshed is calculated
	m_BoundingRect=boundingrect;
	boundingrect|=oldboundingrect;
	boundingrect=boundingrect.normalize();
	m_pView->InvalidateRect(&boundingrect);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to draw or redraw the track presentation
///  Argument 1 : QPainter*, drawing device
///  Argument 2 : int, Layer in which the element must be drawn
///  Argument 3 : int, Priority of the track presentation (selected or not) 
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen020
//
//-----------------------------------------------------------------------------
void CViewTrack::OnDraw(QPainter* pDC,int Prio)
{
	int elem;
	//The track presentation is displayed in the primary layer with the TpView Prio
	//set to 1.
	if ((Prio!=0))
		return;
	//The OnDraw functions of associated classes are called.
	if (m_pView->RectVisible(&m_BoundingRect)) 
	{
		for (elem=0;elem<m_ElemTable.GetSize();elem++)
		{
			m_ElemTable[elem]->OnDraw(pDC,Prio);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function transmits the OnkeyDown events to the associated classes. Each 
///  element of the track has its own interactions.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::OnKeyDown()
{
	int elem;

	for (elem=0;elem<m_ElemTable.GetSize();elem++)
	{
		m_ElemTable[elem]->OnKeyDown();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the track data block and leader line must be 
///  displayed or not.
///
///  RETURNS: 
///  bool, whether a data block must be dispayed or not
///
///  REQUIREMENT ID: CIAsdFctRwsFltGen015, CIAsdFctRwsFltHgt015, CIAsdFctRwsFltHgt020,
///					CIAsdFctRwsFltDbf010, CIAsdFctRwsFltDbf015
//
//-----------------------------------------------------------------------------
bool CViewTrack::IsLabelDisplayed()
{
	//Initialization
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();

	//Uncoupled tracks (not coupled and not tagged) data blocks are not displayed if the 
	//data block filter is activated
	if ((!m_pTrack->m_TD.m_IsCoupled) && m_pView->GetViewParam(CRwdView::filt_db_on))
		return false;

	//The following tracks are not impacted by filters:
	//Track with implicit focus, assumed tracks, handed off tracks, tracks in emergency or with alerts,
	//highlighted tracks, associated to a tool
	if (m_pTrack->m_Select 
		|| (m_pTrack->IsAoc()) 
		|| (m_pTrack->m_DataTable[EmrgData].length()) 
		|| m_pTrack->m_TD.m_IsHighlight
		|| m_pTrack->IsAssociatedToTool())
		return true;

	//The tracks for which the AFL is not received or garbled are not filtered
	if (m_pTrack->m_TD.m_CurAlt==DEFAULT_AFL || m_pTrack->m_TD.m_CurAlt==GARBLED_INVALID)
		return true;

	//For the remaining tracks the Height Filter rules apply
	if ((m_pTrack->m_TD.m_IsDeparture) && (m_pView->GetViewParam(CRwdView::filt_lower_on)) 
		&& (m_pView->GetViewParam(CRwdView::filt_dep_on)))
	{
		if (m_pTrack->m_TD.m_CurAlt<m_pView->GetViewParam(CRwdView::filt_dep_val) || 
			((m_pView->GetViewParam(CRwdView::filt_upper_on)) 
			&& (m_pTrack->m_TD.m_CurAlt>m_pView->GetViewParam(CRwdView::filt_upper_val))))
			return false;
		else 
			return true;
	}
	else
	{
		if (((!m_pView->GetViewParam(CRwdView::filt_lower_on)) 
			|| (m_pTrack->m_TD.m_CurAlt>=m_pView->GetViewParam(CRwdView::filt_lower_val))) && 
			((!m_pView->GetViewParam(CRwdView::filt_upper_on)) 
			|| (m_pTrack->m_TD.m_CurAlt<=m_pView->GetViewParam(CRwdView::filt_upper_val))))
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the track PPS, prediction vector and Afterglows
///  must be displayed or not.
///
///  RETURNS: 
///  bool, whether a PPS must be dispayed or not
///
///  REQUIREMENT ID: CIAsdFctRwsFltGen015, CIAsdFctRwsFltHgt015, CIAsdFctRwsFltHgt020,
///					CIAsdFctRwsFltPrm010, CIAsdFctRwsFltPrm015
//
//-----------------------------------------------------------------------------
bool CViewTrack::IsSymbolDisplayed()
{
	//Initialization
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();

	//Primary Tracks
	//If the Primary filter is activated, the PPS of primary tracks are not displayed
	//No corresponding test is available in the IsLabelDisplayed function as the primary track
	//present no data block for batch 1
 	if ((m_pTrack->m_TD.m_IsPrimary) && (!m_pTrack->m_TD.m_IsSecondary) && (m_pView->GetViewParam(CRwdView::filt_prim_on)))
		return false;
	else if ((m_pTrack->m_TD.m_IsPrimary) && (!m_pTrack->m_TD.m_IsSecondary) && (!m_pView->GetViewParam(CRwdView::filt_prim_on)))
		return true;
	
	//The following tracks are not impacted by filters:
	//Track with implicit focus, assumed tracks, handed off tracks, tracks in emergency or with alerts,
	//highlighted tracks, associated to a tool
	if (m_pTrack->m_Select 
		|| (m_pTrack->IsAoc()) 
		|| (m_pTrack->m_DataTable[EmrgData].length()) 
		|| m_pTrack->m_TD.m_IsHighlight
		|| m_pTrack->IsAssociatedToTool())
		return true;
	
	//The tracks for which the AFL is not received or garbled are not filtered
	if (m_pTrack->m_TD.m_CurAlt==DEFAULT_AFL || m_pTrack->m_TD.m_CurAlt==GARBLED_INVALID)
		return true;

	//For the remaining tracks the Height Filter rules apply
	if ((m_pTrack->m_TD.m_IsDeparture) && (m_pView->GetViewParam(CRwdView::filt_lower_on)) 
		&& (m_pView->GetViewParam(CRwdView::filt_dep_on)))
	{
		if (m_pTrack->m_TD.m_CurAlt<m_pView->GetViewParam(CRwdView::filt_dep_val) || 
			((m_pView->GetViewParam(CRwdView::filt_upper_on)) 
			&& (m_pTrack->m_TD.m_CurAlt>m_pView->GetViewParam(CRwdView::filt_upper_val))))
			return false;
		else 
			return true;
	}
	else
	{
		if (((!m_pView->GetViewParam(CRwdView::filt_lower_on)) 
			|| (m_pTrack->m_TD.m_CurAlt>=m_pView->GetViewParam(CRwdView::filt_lower_val))) && 
			((!m_pView->GetViewParam(CRwdView::filt_upper_on)) 
			|| (m_pTrack->m_TD.m_CurAlt<=m_pView->GetViewParam(CRwdView::filt_upper_val))))
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the color corresponding to the associated track element 
///  or data field put in argument. This returned color takes into account the 
///  brigthness settings.
///  Argument : int, graphical element to be colored
///
///  RETURNS: 
///  FullColor, the color to apply at the graphical element
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen025, CIAsdFctRwsTppGen030, CIAsdFctRwsTppGen035,
///					CIAsdFctRwsTppGen040, CIAsdFctRwsTppGen045, CIAsdFctRwsTppGen050, 
///					CIAsdFctRwsTppGen070, CIAsdFctRwsTppPps040, CIAsdFctRwsTppLls020, 
///					CIAsdFctRwsTppLls032, CIAsdFctRwsTppLls033, CIAsdFctRwsTppAfg009,
///					CIAsdFctRwsTlsGrd030, CIAsdFctDfdCnt016, CIAsdFctDfdCnt025,
///					CIAsdFctDfdCnt045, CIAsdFctDfdCnt046, CIAsdFctDfdCnt055, 
///					CIAsdFctDfdCnt060, CIAsdFctDfdCnt085, CIAsdFctDfdCnt100,
///					CIAsdFctDfdCnt115, CIAsdFctDfdCicAlt015, CIAsdFctDfdCicAcw025
//
//-----------------------------------------------------------------------------
FullColor CViewTrack::ComputeColor(int typ)
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	FullColor RetClr;
	RetClr.IsFlashing=false;
	int Category;
	//Determine to which track element the graphical element belongs to, in order 
	//to set the impacting Brightness slider number.
	switch (typ)
	{
	case TrkSymb:
	case TrkPast:
	case TrkVel:
	case TrkSymbSel:
		if (m_pTrack->m_TD.m_IsAoc)
			Category=AOCSymbCategory;
		else
			Category=NonAOCSymbCategory;
		break;
	default:
		if (m_pTrack->m_TD.m_IsAoc)
			Category=AOCDbCategory;
		else
			Category=NonAOCDbCategory;
		break;
	}

	//By default, the color to be used is the color of the track: Assume, coupled
	//or uncoupled
	int ColorNb;
	if (m_pTrack->m_TD.m_IsAoc)
		ColorNb=AocClr;
	else
	{
		if (m_pTrack->m_TD.m_IsCoupled || m_pTrack->m_TD.m_IsTagged)
			ColorNb=CoupledClr;
		else
			ColorNb=UncoupledClr;
	}


	//Then the specific colors are computed
	switch (typ)
	{
	case TrkSymbSel:
	case TrkLabSel:
		ColorNb=SelClr;
		break;
	case TrkDef:
		break;
	case TrkArcid:
		if (m_pTrack->m_TD.m_IsTagged)
			ColorNb=TagClr;
		if (m_pTrack->m_TD.m_AssociationType == MODESTAGGED)
			ColorNb=ModeSTag;
		break;
	case TrkLead:
		if (m_pTrack->m_RwyOrientation!=-1)
		{
			ColorNb=m_pTrack->m_RwyColor;
			RetClr.Color=CColorMngr::TransformColor(CColorMngr::GetColor(ColorNb),Category);
			return RetClr;
		}
		break;
	case TrkSymb:
		if (m_pTrack->m_IsSpiDisplayed) 
			RetClr.IsFlashing=true;
		if (m_pTrack->m_IsTrackAging)
			ColorNb= WarningClr;
		if (m_pTrack->m_IsCoasted)
			ColorNb= InfoClr;
		break;
	case TrkPast:
		if (m_pTrack->m_IsTrackAging)
			ColorNb= WarningClr;
		break;
	case TrkEmg:
		ColorNb=DangerClr;
		RetClr.IsFlashing=!m_pTrack->m_TD.m_EmergAck;
		break;
	case TrkAls:
		if (m_pTrack->m_DataTable[AlertData] == "DU")
			ColorNb=InfoClr;
		if (m_pTrack->m_DUStatus==CTrack::Alert && m_pTrack->m_DataTable[AlertData]=="DU")
			RetClr.IsFlashing=true;
		break;
	case TrkAfl:
		if ((m_pTrack->m_TD.m_IsAfl) && (m_pTrack->m_DataTable[AflData]!="----"))
			ColorNb=InfoClr;
		if (m_pTrack->m_TD.m_bNegatifAfl)
			ColorNb=WarningClr;
		if (m_pTrack->m_AlertState[ClamMsg]==CTrack::Alert)
			RetClr.IsFlashing=true;
		break;
	case TrkCfl:
		if (m_pTrack->m_AlertState[ClamMsg]==CTrack::Alert)
			RetClr.IsFlashing=true;
		if (m_pTrack->m_TD.m_KdOfAppClear=="GOA" && (m_pTrack->m_TD.m_Cfl)=="F000")
			ColorNb= WarningClr;
		break;
	case TrkRvsm:
		if (m_pTrack->m_TD.m_Rvsm=="N" || m_pTrack->m_TD.m_Rvsm=="?")
			ColorNb=InfoClr;
		break;
	case Trk833:
		if (m_pTrack->m_TD.m_833=="N" || m_pTrack->m_TD.m_833=="?")
			ColorNb=InfoClr;
		break;
	case TrkTagged:
		if (m_pTrack->m_TD.m_IsTagged)
			ColorNb=TagClr;
		break;
	case TrkTssrPb:
		ColorNb=WarningClr;
		break;
	case TrkRocd:
		if (!m_ToggleRocdUnitFtMin)
			ColorNb=RocdClr;
		break;
	case TrkAtt:
		if (m_pTrack->m_DataTable[FAttData]=='*' || m_pTrack->m_TD.m_FlModConflict)
			ColorNb=WarningClr;
		break;
	case TrkMism:
		if (m_pTrack->m_TD.m_IsAoc || m_pTrack->m_TD.m_Mnemo==0)
			RetClr.IsFlashing=true;
		break;
	default:
		break;
	}
	RetClr.Color=CColorMngr::TransformColor(CColorMngr::GetColor(m_ColorTable[ColorNb]),Category);
	return RetClr;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selected status of the track to the one put in
///  argument.
///  Argument : bool, selected status of the track to be applied.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen130
//
//-----------------------------------------------------------------------------
void CViewTrack::SelectElement(bool Select)
{
	m_Select = Select;
	//If the track is lost the implicit focus, the event is transmitted
	//to the associated classes and the extended data block is canceled
	if (!m_Select)
	{
		if (m_SelElem!=-1)
			m_ElemTable[m_SelElem]->SelectElement(false);
		m_SelElem = -1;
		HookLabel(false);
	}
	else
	{
		if (m_LastSelElem!=-1)
		{
			m_SelElem= m_LastSelElem;
			m_ElemTable[m_SelElem]->SelectElement(true);
		}
	}
	m_pTrack->Select(m_Select);

	QRect boundingrect = m_BoundingRect;
	boundingrect=boundingrect.normalize();
	m_pView->InvalidateRect(&boundingrect);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns a priority number according to the position of the
///  mouse pointer compared to the track presentation
///  Argument 1 : point, position of the mouse pointer.
///  Argument 2 : bool, whether all the track elements must be checked or not.
///
///  RETURNS: 
///  int, selection priority number of the track
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CViewTrack::GetPriority(QPoint point,bool Check)
{
  if (CTrkMiles::IsInCreation())
    return 0;
  else
    {
	int elem,prio=0;
	m_LastSelElem=-1;
	//Cancels the extended data block
	if ((m_Hooked) && (m_SelElem==-1) && (!m_LabelRect.contains(point)))
		HookLabel(false);
	//Checks whether the mouse pointer is over one track element
	if (Check)
	{
		if (m_Hooked)
		{
			for (elem = 0;elem<m_ElemTable.GetSize();elem++)
			{
				prio = m_ElemTable[elem]->GetPriority(point);
				if (prio)
				{
					if (m_SelElem!=elem)
					{
						if (m_SelElem!=-1)
							m_ElemTable[m_SelElem]->SelectElement(false);
						m_SelElem=elem;
						m_ElemTable[m_SelElem]->SelectElement(true);
						m_pView->InvalidateRect(&m_BoundingRect);
					}
					return prio;
				}
			}
			if (m_SelElem!=-1)
			{
				m_ElemTable[m_SelElem]->SelectElement(false);
				m_pView->InvalidateRect(&m_BoundingRect);
			}
			m_SelElem=-1;
			if (m_LabelRect.contains(point))
				return PRIO_TRACK_LABEL_GLOBAL;
			else
			{
				HookLabel(false);
				return 0;
			}
		}
		if (m_SelElem!=-1)
			return m_ElemTable[m_SelElem]->GetPriority(point);
		else
			return 0;
	}
	if (m_BoundingRect.contains(point))
	{
		for (elem = 0;elem<m_ElemTable.GetSize();elem++)
		{
			prio = m_ElemTable[elem]->GetPriority(point);
			if (prio)
			{
				m_LastSelElem=elem;
				return prio;
			}
		}
	}
	//If the mouse pointer is not over any track element the function returns 0
	return 0;
    }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns important code calculated by the IsImportant function
///
///  RETURNS: 
///  int, the important code of the track
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CViewTrack::IsImportant()
{
	int iCode=m_pTrack->IsImportant();
   	return iCode;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the mouse pointer is moved over a track
///  presentation.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::OnMouseMove(UINT nFlags,QPoint point)
{
	//The event is transmitted to the element that is being modified
	//or to the element currently selected
	if (m_ModifElem!=-1)
		m_ElemTable[m_ModifElem]->OnMouseMove(nFlags,point);
	if (m_SelElem!=-1)
		m_ElemTable[m_SelElem]->OnMouseMove(nFlags,point);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the user performs a click AB over a track
///  presentation. The action is automatically applied or the event is 
///  transmitted to the impacted element object.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppPps045, CIAsdFctRwsTppPps050, CIAsdFctRwsTppPps055, 
///					CIAsdFctRwsTppPps060, CIAsdFctRwsTlsGrd070, CIAsdFctDfdCicCst010 
//
//-----------------------------------------------------------------------------
void CViewTrack::OnLButtonDown(UINT nFlags,QPoint point)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	QPoint pnt,pnt2=point;
	pnt = QPoint(m_LabelRect.left(),m_LabelRect.bottom());
	m_pView->mapToGlobal(pnt);
	m_pView->mapToGlobal(pnt2);
	BYTE sic, sac;
	int trknum;
	m_pTrack->GetTrkNum(sic, sac, trknum);
	switch (m_SelElem)
	{
		//Click AB on the PPS changes the assume status of the track
	case symb:
		if (m_pTrack->IsAssumeDeAssumeAllowed())
		{
			if (pTrkMngr->m_pTrkSrc)
			{
				if (m_pTrack->m_pHEvent)
					m_pTrack->m_pHEvent->OnAssumeDeassume(m_pTrack);
			}
		}
		break;
		//No interaction with afterglows are defined
	case pastp:
		break;
		//for other elements the interactions are specific
	case lead:
	case label:
		m_ElemTable[m_SelElem]->OnLButtonDown(nFlags,point);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the user performs a click AB release over 
///  a track presentation. The action is automatically applied or the event is 
///  transmitted to the impacted element object.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::OnLButtonUp(UINT nFlags,QPoint point)
{
	switch (m_ModifElem)
	{
		//Only the PPS has such a specific interaction
	case label:
		m_ElemTable[m_ModifElem]->OnLButtonUp(nFlags,point);
		break;
	default:
		break;
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the user performs a click WB over a track
///  presentation. The action is automatically applied.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppAge025, CIAsdFctRwsTlsGrd035
//
//-----------------------------------------------------------------------------
void CViewTrack::OnMButtonDown(UINT nFlags,QPoint point)
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	switch (m_SelElem)
	{
		//If the track is an emergency ageing track a click WB removes the track
		//presentation
	case symb:
		if ((m_pTrack->m_IsTrackAging) && (m_pTrack->m_DataTable[EmrgData].length()))
		{
			int key1, key2, key3;
			if (m_pTrack->GetMainKey(key1, key2, key3))
			{
				if (m_pTrack->m_pHEvent)
					m_pTrack->m_pHEvent->OnRemoveTrack(m_pTrack, false);
				if (pMngr->m_TrackList.RemoveAt(key1, key2, key3))
				{
					delete m_pTrack;
				}
			}
		}
		break;
	case lead:
		break;
	case pastp:
		break;
	case halo:
		if (CRwdView::GetIsIndependent())
		{
			ForceHalo(false); 		
		}
		else
		{
			for (int view=0;view<m_pTrack->m_ElemTable.GetSize();view++)
			{
				m_pTrack->m_ElemTable[view]->ForceHalo(false); 
			}
		}
		break;
	case label:
		m_ElemTable[m_SelElem]->OnMButtonDown(nFlags,point);
		break;
	default:
		break;
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the user performs a click SB over a track
///  presentation. The action is automatically applied or the event is 
///  transmitted to the impacted element object.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::OnRButtonDown(UINT nFlags,QPoint point)
{
	switch (m_SelElem)
	{
		//On the PPS opens the track menu
	case symb:
		{
			CTrackMngr* pTrkMngr = CAsdApp::GetApp()->GetTrackMngr();
			pTrkMngr->RemoveMenu();
			pTrkMngr->m_pMenuCallingTrack = m_pTrack;
			pTrkMngr->m_pMenuCallingWnd = m_pView;
			pTrkMngr->m_pTrackMenu = new CAsdMenu();
			pTrkMngr->m_pTrackMenu->AddItem("TML",IDC_TRKM,true);
			pTrkMngr->m_pTrackMenu->InsertSeparator();
			pTrkMngr->m_pTrackMenu->AddItem("HALO",IDC_HALO,true);
 			pTrkMngr->m_pTrackMenu->TrackPopupMenu(m_pView->mapToGlobal(point).x(),m_pView->mapToGlobal(point).y(),this,NULL); 
		}
		break;
		//No interaction for afterglows and leader line
	case pastp:
		break;
	case lead:
		break;
		//Specific interactions
	case label:
		m_ElemTable[m_SelElem]->OnRButtonDown(nFlags,point);
		break;
		//The extended data block is invoked or canceled
	case -1:
		if (m_pTrack->m_TD.m_IsCoupled)
		{
			m_Extended=!m_Extended;
			OnUpdate();
		}
		break;
	default:
		break;
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the extended status of the data block to the one put in
///  argument.
///  Argument : bool, whether the data block is extended or not
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::HookLabel(bool hook)
{
	if (m_Hooked!=hook)
	{
		m_Hooked = hook;
		if (!m_Hooked)
			m_Extended=false;
		OnUpdate();
	}
	if (!m_Hooked && m_Extended)
	{
		m_Extended=false;
		OnUpdate();
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the display status of the track presentation Halo
///  Argument : bool, whether the Halo must be displayed or not
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsGrd015, CIAsdFctRwsTlsGrd045
//
//-----------------------------------------------------------------------------
void CViewTrack::ForceHalo(bool forced)
{
	if (((CHalo*)m_ElemTable[halo])->m_Forced!=forced)
	{
		((CHalo*)m_ElemTable[halo])->m_Forced = forced;
		OnUpdate();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the display status of the track presentation Halo
///  Argument : bool, whether the Halo must be displayed or not
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsGrd015, CIAsdFctRwsTlsGrd045
//
//-----------------------------------------------------------------------------
bool CViewTrack::IsForcedHalo()
{
	return ((CHalo*)m_ElemTable[halo])->m_Forced;
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This static method allows to cancel any manual orientation of the data block.
///  The orientation of the data block is then managed by the LCM feature.
///  Argument : bool, whether the route must be displayed or not
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppLls066
//
//-----------------------------------------------------------------------------
void CViewTrack::ResetLcm()
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	CTripleEntryListIterator<CTrack> it( pMngr->m_TrackList );
	CTrack* pTrack=NULL;
	pTrack = it.current();
	//The manual orientation indicator is initialized for every track
	while ( pTrack != 0 ) 
	{
		++it;
		for (int i=0;i<pTrack->m_ElemTable.GetSize();i++)
			pTrack->m_ElemTable[i]->m_ManualLeaderPos=false;
		pTrack = it.current();
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method returns the number of the data block corresponding to the track
///  status.
///
///  RETURNS: 
///  int, data block number
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk005, CIAsdFctRwsTppDbk010, CIAsdFctRwsTppDbk015, 
///					CIAsdFctRwsTppDbk019, CIAsdFctRwsTppDbk020, CIAsdFctRwsTppDbk025,
///					CIAsdFctRwsTppDbk030, CIAsdFctRwsTppDbk031, CIAsdFctRwsTppDbk032, 
///					CIAsdFctRwsTppDbk033, CIAsdSymSmbEsm125, CIAsdSymSmbEsm145,
///					CIAsdSymSmbEsm165, CIAsdSymSmbErs135
//
//-----------------------------------------------------------------------------
int CViewTrack::ComputeDataBlock()
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();

	bool IsApp=false;
	if (m_pTrack->m_pHEvent)
		IsApp=m_pTrack->m_pHEvent->IsAppPos();
	if ((!m_pTrack->m_TD.m_IsSecondary) && (!m_pTrack->m_TD.m_IsCoupled))
		return -1;

	//If not in Local tag facility, treatment of uncoupled tracks
	if (!m_pTrack->m_TD.m_IsCoupled)
		return LimitedUncoupledDb;
	else
	{
		//If the track is not coupled, treatment of tagged tracks
		if (m_pTrack->m_TD.m_IsTagged)
		{
			if (m_Extended)
				return ExtendedTaggedDb;
			else
				return TaggedLimitedUncoupledDb;
		}
		//If the track is not uncoupled nor tagged, treatment of coupled tracks
		else
		{
			if (IsApp)
			{
				if (m_Extended)
					return ExtendedAppDb;
				else
					if (m_pTrack->m_TD.m_IsAoc)
					{
						return FullAppDb;
					}
					else
					{
						return LimitedDb;
					}
			}
			else
			{
				if (m_Extended)
					return ExtendedAccDb;
				else
					if (m_pTrack->m_TD.m_IsAoc)
					{
						return FullAccDb;
					}
					else
					{
						return LimitedDb;
					}
			}
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method constructs the track presentation. It initializes all the track
///  elements properties : TrackDef object is initialized for the orientation table
///  and the list of elements composing the track.
///  Argument : TrackDef*, Definition of a track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppLls021
//
//-----------------------------------------------------------------------------
void CViewTrack::CreateTrackElements(TrackDef* pTrackDef)
{
	ElemDef newelemdef;
	OrientationDef ordef;

	//Init of Leader Line orientation table : 8 orientations	
	ordef.Angle=0;
	ordef.Len=40;
	pTrackDef->OrientationTable.Add(ordef);

	ordef.Angle=45;
	ordef.Len=35;
	pTrackDef->OrientationTable.Add(ordef);

	ordef.Angle=90;
	ordef.Len=35;
	pTrackDef->OrientationTable.Add(ordef);

	ordef.Angle=135;
	ordef.Len=40;
	pTrackDef->OrientationTable.Add(ordef);

	ordef.Angle=180;
	ordef.Len=35;
	pTrackDef->OrientationTable.Add(ordef);

	ordef.Angle=225;
	ordef.Len=35;
	pTrackDef->OrientationTable.Add(ordef);

	ordef.Angle=270;
	ordef.Len=35;
	pTrackDef->OrientationTable.Add(ordef);

	ordef.Angle=315;
	ordef.Len=35;
	pTrackDef->OrientationTable.Add(ordef);

	pTrackDef->ElemDefTable.SetSize(6);

	//Present Positionning Symbol
	newelemdef.type = symb;
	newelemdef.colornb = TrkSymb;
	newelemdef.fontnb=0;
	newelemdef.prio=PRIO_TRACK_SYMBOL;
	pTrackDef->ElemDefTable[symb]=newelemdef;

	//Afterglows
	newelemdef.type = pastp;
	newelemdef.colornb = TrkPast;
	newelemdef.fontnb=0;
	newelemdef.prio=0;
	newelemdef.length = 3;
	pTrackDef->ElemDefTable[pastp]=newelemdef;

	//Leader line
	newelemdef.type = lead;
	newelemdef.colornb = TrkLead;
	newelemdef.prio = 0;
	pTrackDef->ElemDefTable[lead]=newelemdef;

	//Data block
	newelemdef.type = label;
	newelemdef.colornb = TrkLead;
	newelemdef.prio = PRIO_TRACK_LABEL;
	pTrackDef->ElemDefTable[label]=newelemdef;

	//Prediction Vector
	newelemdef.type = velocity;
	newelemdef.colornb = TrkVel;
	newelemdef.prio = 0;
	newelemdef.length = 90;
	pTrackDef->ElemDefTable[velocity]=newelemdef;

	//Halo
	newelemdef.type = halo;
	newelemdef.colornb = TrkDef;
	newelemdef.prio = PRIO_TRACK_HALO;
	newelemdef.length = 0;
	pTrackDef->ElemDefTable[halo]=newelemdef;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is used to add a data field definition within a data block.
///  Argument 1 : int, color number of the data field
///  Argument 2 : int, priority of the data field
///  Argument 3 : int, font number of the data field
///  Argument 4 : int, data number of the data field
///  Argument 5 : int, length of the data field
///  Argument 6 : QPoint, position of the data field 
///  Argument 7 : int, Corresponding view parameter number if any
///  Argument 8 : QString, Mnemonic if any  
/// 
///  RETURNS: 
///  ElemDef, a new graphical element
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
ElemDef CViewTrack::FormatDef(int colornb,int prio,int fontnb,int datanb,int length,QPoint pos,int viewparam,QString Mnemo)
{
	ElemDef item;
	item.type=label;
	item.prio=prio;
	item.datanb=datanb;
	item.fontnb=fontnb;
	item.colornb=colornb;
	item.length=length;
	item.viewparam=viewparam;
	item.pos=pos;
	item.Mnemo=Mnemo;
	return item;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method describes the content of a Limited Uncoupled Data Block. 
///  This definition is added to the TrackDef object.
///  Argument 1 : LabelDef*, Definition of a track data block
///  Argument 2 : TrackDef*, Definition of a track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk045, CIAsdFctRwsTppDbk050, CIAsdFctRwsTppDbk100,
///					CIAsdFctDfdInt005
//
//-----------------------------------------------------------------------------
void CViewTrack::LimitedUncoupledDB(LabelDef* pLabelDef, TrackDef* pTrackDef)
{
	// Uncoupled data block
	//line 0
	pLabelDef->Add(FormatDef(TrkEmg,PRIO_TRACK_LABEL_ELEM_1,0,EmrgData,2,QPoint(0,0),-1));
	pLabelDef->Add(FormatDef(TrkAls,PRIO_TRACK_LABEL_ELEM_1,0,AlertData,2,QPoint(2,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,ModeSData,1,QPoint(5,0),CRwdView::fld_unc_modes));

	//line 1
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,TssrData,8,QPoint(1,1),-1));

	//line 2
	pLabelDef->Add(FormatDef(TrkAfl,0,1,AflData,4,QPoint(1,2),-1,"----"));
	pLabelDef->Add(FormatDef(TrkAtt,0,0,FAttData,1,QPoint(5,2),-1));
	
	//line 3
	pLabelDef->Add(FormatDef(TrkDef,0,0,GspdData,3,QPoint(1,3),CRwdView::fld_unc_gsp));
	pLabelDef->Add(FormatDef(TrkRocd,PRIO_TRACK_LABEL_ELEM_1,0,VspdData,2,QPoint(5,3),CRwdView::fld_unc_rocd,"CD"));

	pTrackDef->LabelDefTable.Add(pLabelDef);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method describes the content of a Full ACC Data Block. 
///  This definition is added to the TrackDef object.
///  Argument 1 : LabelDef*, Definition of a track data block
///  Argument 2 : TrackDef*, Definition of a track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk045, CIAsdFctRwsTppDbk050, CIAsdFctRwsTppDbk075,
///					CIAsdFctDfdInt005, CIAsdFctDfdCicM3a025
//
//-----------------------------------------------------------------------------
void CViewTrack::FullAccDB(LabelDef* pLabelDef, TrackDef* pTrackDef)
{
	// Full ACC data block
	pLabelDef=new LabelDef;

	//line 0
	pLabelDef->Add(FormatDef(TrkEmg,PRIO_TRACK_LABEL_ELEM_1,0,EmrgData,2,QPoint(1,0),-1));
	pLabelDef->Add(FormatDef(TrkAls,PRIO_TRACK_LABEL_ELEM_1,0,AlertData,2,QPoint(4,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,MiData,1,QPoint(7,0),-1));
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,CoordData,1,QPoint(8,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FltrulData,1,QPoint(9,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,ModeSData,1,QPoint(10,0),CRwdView::fld_fdb_modes));
	
	//line 1
	pLabelDef->Add(FormatDef(TrkMism,PRIO_TRACK_LABEL_ELEM_1,0,ArcidMismatch,1,QPoint(0,1),-1));
	pLabelDef->Add(FormatDef(TrkArcid,PRIO_TRACK_LABEL_ELEM_1,1,ArcidData,8,QPoint(1,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FlightCatData,1,QPoint(9,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,WtcData,1,QPoint(10,1),-1,"W"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,SlashData,1,QPoint(11,1),CRwdView::fld_fdb_atyp,"/"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ArctypData,4,QPoint(12,1),CRwdView::fld_fdb_atyp,"ATYP"));
	pLabelDef->Add(FormatDef(TrkTssrPb,0,0,TssrPbData,4,QPoint(17,1),-1));
	
	//line 2
	pLabelDef->Add(FormatDef(TrkDef,0,0,AsterData,1,QPoint(0,2),-1));
	pLabelDef->Add(FormatDef(TrkAfl,PRIO_TRACK_LABEL_ELEM_1,1,AflData,4,QPoint(1,2),-1,"----"));
	pLabelDef->Add(FormatDef(TrkAtt,0,0,FAttData,1,QPoint(5,2),-1));
	pLabelDef->Add(FormatDef(TrkCfl,PRIO_TRACK_LABEL_ELEM_1,0,CflData,3,QPoint(6,2),-1,"CFL"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,RwyData,3,QPoint(10,2),CRwdView::fld_fdb_rwy,"RWY"));
	
	//line 3
	pLabelDef->Add(FormatDef(TrkDef,0,0,GspdData,3,QPoint(1,3),CRwdView::fld_fdb_gsp));
	pLabelDef->Add(FormatDef(TrkRocd,PRIO_TRACK_LABEL_ELEM_1,0,VspdData,2,QPoint(5,3),CRwdView::fld_fdb_rocd,"CD"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,RvrData,4,QPoint(8,3),CRwdView::fld_fdb_rvr));
	
	//line 4
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,XflData,4,QPoint(6,4),-1,"XFL"));
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,XptData,5,QPoint(11,4),-1,"COPX"));
	
	pTrackDef->LabelDefTable.Add(pLabelDef);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method describes the content of a Full APP Data Block. 
///  This definition is added to the TrackDef object.
///  Argument 1 : LabelDef*, Definition of a track data block
///  Argument 2 : TrackDef*, Definition of a track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk045, CIAsdFctRwsTppDbk050, CIAsdFctRwsTppDbk080,
///					CIAsdFctDfdInt005, CIAsdFctDfdCicM3a025
//
//-----------------------------------------------------------------------------
void CViewTrack::FullAppDB(LabelDef* pLabelDef, TrackDef* pTrackDef)
{
	// Full APP data block
	pLabelDef=new LabelDef;

	//line 0
	pLabelDef->Add(FormatDef(TrkEmg,PRIO_TRACK_LABEL_ELEM_1,0,EmrgData,2,QPoint(1,0),-1));
	pLabelDef->Add(FormatDef(TrkAls,PRIO_TRACK_LABEL_ELEM_1,0,AlertData,2,QPoint(4,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,MiData,1,QPoint(7,0),-1));
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,CoordData,1,QPoint(8,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FltrulData,1,QPoint(9,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,ModeSData,1,QPoint(10,0),CRwdView::fld_fdb_modes));
	
	//line 1
	pLabelDef->Add(FormatDef(TrkMism,PRIO_TRACK_LABEL_ELEM_1,0,ArcidMismatch,1,QPoint(0,1),-1));
	pLabelDef->Add(FormatDef(TrkArcid,PRIO_TRACK_LABEL_ELEM_1,1,ArcidData,8,QPoint(1,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FlightCatData,1,QPoint(9,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,WtcData,1,QPoint(10,1),-1,"W"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,SlashData,1,QPoint(11,1),CRwdView::fld_fdb_atyp,"/"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ArctypData,4,QPoint(12,1),CRwdView::fld_fdb_atyp,"ATYP"));
	pLabelDef->Add(FormatDef(TrkTssrPb,0,0,TssrPbData,4,QPoint(17,1),-1));
	
	//line 2
	pLabelDef->Add(FormatDef(TrkDef,0,0,AsterData,1,QPoint(0,2),-1));
	pLabelDef->Add(FormatDef(TrkAfl,PRIO_TRACK_LABEL_ELEM_1,1,AflData,4,QPoint(1,2),-1,"----"));
	pLabelDef->Add(FormatDef(TrkAtt,0,0,FAttData,1,QPoint(5,2),-1));
	pLabelDef->Add(FormatDef(TrkCfl,PRIO_TRACK_LABEL_ELEM_1,0,CflData,3,QPoint(6,2),-1,"CFL"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,RwyData,3,QPoint(10,2),CRwdView::fld_fdb_rwy,"RWY"));
	
	//line 3
	pLabelDef->Add(FormatDef(TrkDef,0,0,GspdData,3,QPoint(1,3),-1));
	pLabelDef->Add(FormatDef(TrkRocd,PRIO_TRACK_LABEL_ELEM_1,0,VspdData,2,QPoint(5,3),CRwdView::fld_fdb_rocd,"CD"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,RvrData,4,QPoint(8,3),CRwdView::fld_fdb_rvr));
	
	//Line 4
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,AdesData,4,QPoint(1,4),CRwdView::fld_fdb_ades,"ADES"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,SidData,7,QPoint(6,4),CRwdView::fld_fdb_sid));

	pTrackDef->LabelDefTable.Add(pLabelDef);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method describes the content of a Tagged Limited Uncoupled Data Block. 
///  This definition is added to the TrackDef object.
///  Argument 1 : LabelDef*, Definition of a track data block
///  Argument 2 : TrackDef*, Definition of a track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk045, CIAsdFctRwsTppDbk050, CIAsdFctRwsTppDbk105,
///					CIAsdFctDfdInt005
//
//-----------------------------------------------------------------------------
void CViewTrack::TaggedLimitedUncoupledDB(LabelDef* pLabelDef, TrackDef* pTrackDef)
{
	// Tagged data block
	pLabelDef=new LabelDef;

	//Line 0
	pLabelDef->Add(FormatDef(TrkEmg,PRIO_TRACK_LABEL_ELEM_1,0,EmrgData,2,QPoint(1,0),-1));
	pLabelDef->Add(FormatDef(TrkAls,PRIO_TRACK_LABEL_ELEM_1,0,AlertData,2,QPoint(4,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,MiData,1,QPoint(7,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,ModeSData,1,QPoint(10,0),CRwdView::fld_tag_modes));
	
	//Line 1
	pLabelDef->Add(FormatDef(TrkMism,PRIO_TRACK_LABEL_ELEM_1,0,ArcidMismatch,1,QPoint(0,1),-1));
	pLabelDef->Add(FormatDef(TrkArcid,PRIO_TRACK_LABEL_ELEM_1,1,ArcidData,8,QPoint(1,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,WtcData,1,QPoint(10,1),-1,"W"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,SlashData,1,QPoint(11,1),CRwdView::fld_tag_atyp,"/"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ArctypData,4,QPoint(12,1),CRwdView::fld_tag_atyp,"ATYP"));
	
	//Line 2
	pLabelDef->Add(FormatDef(TrkDef,0,0,AsterData,1,QPoint(0,2),-1));
	pLabelDef->Add(FormatDef(TrkAfl,PRIO_TRACK_LABEL_ELEM_1,1,AflData,4,QPoint(1,2),-1,"----"));
	pLabelDef->Add(FormatDef(TrkAtt,0,0,FAttData,1,QPoint(5,2),-1));
	pLabelDef->Add(FormatDef(TrkCfl,PRIO_TRACK_LABEL_ELEM_1,0,CflData,3,QPoint(6,2),-1,"CFL"));
	
	//Line 3
	pLabelDef->Add(FormatDef(TrkDef,0,0,GspdData,3,QPoint(1,3),CRwdView::fld_tag_gsp));
	pLabelDef->Add(FormatDef(TrkRocd,PRIO_TRACK_LABEL_ELEM_1,0,VspdData,2,QPoint(5,3),CRwdView::fld_tag_rocd,"CD"));

	pTrackDef->LabelDefTable.Add(pLabelDef);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method describes the content of a Extended Tagged Data Block. 
///  This definition is added to the TrackDef object.
///  Argument 1 : LabelDef*, Definition of a track data block
///  Argument 2 : TrackDef*, Definition of a track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk045, CIAsdFctRwsTppDbk050, CIAsdFctRwsTppDbk106,
///					CIAsdFctDfdInt005
//
//-----------------------------------------------------------------------------
void CViewTrack::ExtendedTaggedDB(LabelDef* pLabelDef, TrackDef* pTrackDef)
{
	// Extended Tagged data block
	pLabelDef=new LabelDef;

	//Line 0
	pLabelDef->Add(FormatDef(TrkEmg,PRIO_TRACK_LABEL_ELEM_1,0,EmrgData,2,QPoint(1,0),-1));
	pLabelDef->Add(FormatDef(TrkAls,PRIO_TRACK_LABEL_ELEM_1,0,AlertData,2,QPoint(4,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,MiData,1,QPoint(7,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ModeSData,1,QPoint(10,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FlightIdData,8,QPoint(12,0),-1));
	
	//Line 1
	pLabelDef->Add(FormatDef(TrkMism,PRIO_TRACK_LABEL_ELEM_1,0,ArcidMismatch,1,QPoint(0,1),-1));
	pLabelDef->Add(FormatDef(TrkArcid,PRIO_TRACK_LABEL_ELEM_1,1,ArcidData,8,QPoint(1,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,WtcData,1,QPoint(10,1),-1,"W"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,SlashData,1,QPoint(11,1),-1,"/"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ArctypData,4,QPoint(12,1),-1,"ATYP"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,TssrData,4,QPoint(17,1),-1));
	
	//Line 2
	pLabelDef->Add(FormatDef(TrkDef,0,0,AsterData,1,QPoint(0,2),-1));
	pLabelDef->Add(FormatDef(TrkAfl,PRIO_TRACK_LABEL_ELEM_1,1,AflData,4,QPoint(1,2),-1,"----"));
	pLabelDef->Add(FormatDef(TrkAtt,0,0,FAttData,1,QPoint(5,2),-1));
	pLabelDef->Add(FormatDef(TrkCfl,PRIO_TRACK_LABEL_ELEM_1,0,CflData,3,QPoint(6,2),-1,"CFL"));
	
	//Line 3
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,GspdData,3,QPoint(1,3),-1));
	pLabelDef->Add(FormatDef(TrkRocd,PRIO_TRACK_LABEL_ELEM_1,0,VspdData,2,QPoint(5,3),-1,"CD"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,CompData,17,QPoint(8,3),-1));


	pTrackDef->LabelDefTable.Add(pLabelDef);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method describes the content of a Extended ACC Data Block. 
///  This definition is added to the TrackDef object.
///  Argument 1 : LabelDef*, Definition of a track data block
///  Argument 2 : TrackDef*, Definition of a track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk045, CIAsdFctRwsTppDbk050, CIAsdFctRwsTppDbk090,
///					CIAsdFctDfdInt005
//
//-----------------------------------------------------------------------------
void CViewTrack::ExtendedAccDB(LabelDef* pLabelDef, TrackDef* pTrackDef)
{		
	// Extended ACC data block 
	ElemDef elemdef;

	pLabelDef=new LabelDef;
	elemdef.viewparam=-1;
	
	//line 0
	pLabelDef->Add(FormatDef(TrkEmg,PRIO_TRACK_LABEL_ELEM_1,0,EmrgData,2,QPoint(1,0),-1));
	pLabelDef->Add(FormatDef(TrkAls,PRIO_TRACK_LABEL_ELEM_1,0,AlertData,2,QPoint(4,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,MiData,1,QPoint(7,0),-1));
	pLabelDef->Add(FormatDef(TrkCoord,0,0,CoordData,1,QPoint(8,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FltrulData,1,QPoint(9,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ModeSData,1,QPoint(10,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FlightIdData,8,QPoint(12,0),-1));
	pLabelDef->Add(FormatDef(TrkHxy,0,0,SEquippedData,2,QPoint(21,0),-1));

	//line 1
	pLabelDef->Add(FormatDef(TrkMism,PRIO_TRACK_LABEL_ELEM_1,0,ArcidMismatch,1,QPoint(0,1),-1));
	pLabelDef->Add(FormatDef(TrkArcid,PRIO_TRACK_LABEL_ELEM_1,1,ArcidData,8,QPoint(1,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FlightCatData,1,QPoint(9,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,WtcData,1,QPoint(10,1),-1,"W"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,SlashData,1,QPoint(11,1),-1,"/"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ArctypData,4,QPoint(12,1),-1,"ATYP"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,TssrData,4,QPoint(17,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,AssrData,4,QPoint(22,1),-1,"ASSR"));

	//line 2
	pLabelDef->Add(FormatDef(TrkDef,0,0,AsterData,1,QPoint(0,2),-1));
	pLabelDef->Add(FormatDef(TrkAfl,PRIO_TRACK_LABEL_ELEM_1,1,AflData,4,QPoint(1,2),-1,"----"));
	pLabelDef->Add(FormatDef(TrkAtt,0,0,FAttData,1,QPoint(5,2),-1));
	pLabelDef->Add(FormatDef(TrkCfl,PRIO_TRACK_LABEL_ELEM_1,0,CflData,3,QPoint(6,2),-1,"CFL"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,RwyData,3,QPoint(10,2),-1,"RWY"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,SidData,7,QPoint(16,2),-1));
	
	//line 3
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,GspdData,3,QPoint(1,3),-1));
	pLabelDef->Add(FormatDef(TrkRocd,PRIO_TRACK_LABEL_ELEM_1,0,VspdData,2,QPoint(5,3),-1,"CD"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,RvrData,4,QPoint(8,3),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,CompData,17,QPoint(13,3),-1));

	//line 4
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,XflData,4,QPoint(6,4),-1,"XFL"));
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,XptData,5,QPoint(11,4),-1,"COPX"));
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,EtxData,4,QPoint(16,4),-1,"ETX"));
	pLabelDef->Add(FormatDef(TrkRvsm,0,0,RvsmData,2,QPoint(26,4),-1));
	pLabelDef->Add(FormatDef(Trk833,0,0,N833Data,2,QPoint(29,4),-1));
	
	//line 5
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,AdepData,4,QPoint(1,5),-1,"ADEP"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,AdesData,4,QPoint(6,5),-1,"ADES"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,RflData,3,QPoint(11,5),-1,"RFL"));
	
	
	pTrackDef->LabelDefTable.Add(pLabelDef);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method describes the content of an Extended APP Data Block. 
///  This definition is added to the TrackDef object.
///  Argument 1 : LabelDef*, Definition of a track data block
///  Argument 2 : TrackDef*, Definition of a track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk045, CIAsdFctRwsTppDbk050, CIAsdFctRwsTppDbk095,
///					CIAsdFctDfdInt005
//
//-----------------------------------------------------------------------------
void CViewTrack::ExtendedAppDB(LabelDef* pLabelDef, TrackDef* pTrackDef)
{
	// Extended APP data block
	ElemDef elemdef;

	pLabelDef=new LabelDef;
	elemdef.viewparam=-1;
	
	//line 0
	pLabelDef->Add(FormatDef(TrkEmg,PRIO_TRACK_LABEL_ELEM_1,0,EmrgData,2,QPoint(1,0),-1));
	pLabelDef->Add(FormatDef(TrkAls,PRIO_TRACK_LABEL_ELEM_1,0,AlertData,2,QPoint(4,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,MiData,1,QPoint(7,0),-1));
	pLabelDef->Add(FormatDef(TrkCoord,0,0,CoordData,1,QPoint(8,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FltrulData,1,QPoint(9,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ModeSData,1,QPoint(10,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FlightIdData,8,QPoint(12,0),-1));
	pLabelDef->Add(FormatDef(TrkHxy,0,0,SEquippedData,2,QPoint(21,0),-1));

	//line 1
	pLabelDef->Add(FormatDef(TrkMism,PRIO_TRACK_LABEL_ELEM_1,0,ArcidMismatch,1,QPoint(0,1),-1));
	pLabelDef->Add(FormatDef(TrkArcid,PRIO_TRACK_LABEL_ELEM_1,1,ArcidData,8,QPoint(1,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FlightCatData,1,QPoint(9,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,WtcData,1,QPoint(10,1),-1,"W"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,SlashData,1,QPoint(11,1),-1,"/"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ArctypData,4,QPoint(12,1),-1,"ATYP"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,TssrData,4,QPoint(17,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,AssrData,4,QPoint(22,1),-1,"ASSR"));

	//line 2
	pLabelDef->Add(FormatDef(TrkDef,0,0,AsterData,1,QPoint(0,2),-1));
	pLabelDef->Add(FormatDef(TrkAfl,PRIO_TRACK_LABEL_ELEM_1,1,AflData,4,QPoint(1,2),-1,"----"));
	pLabelDef->Add(FormatDef(TrkAtt,0,0,FAttData,1,QPoint(5,2),-1));
	pLabelDef->Add(FormatDef(TrkCfl,PRIO_TRACK_LABEL_ELEM_1,0,CflData,3,QPoint(6,2),-1,"CFL"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,RwyData,3,QPoint(10,2),-1,"RWY"));

	//line 3
	pLabelDef->Add(FormatDef(TrkDef,0,0,GspdData,3,QPoint(1,3),-1));
	pLabelDef->Add(FormatDef(TrkRocd,PRIO_TRACK_LABEL_ELEM_1,0,VspdData,2,QPoint(5,3),-1,"CD"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,RvrData,4,QPoint(8,3),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,CompData,17,QPoint(13,3),-1));

	//line 4
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,AdesData,4,QPoint(1,4),-1,"ADES"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,SidData,7,QPoint(6,4),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,RflData,3,QPoint(14,4),-1,"RFL"));

	//line 5
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,XflData,4,QPoint(6,5),-1,"XFL"));
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,XptData,5,QPoint(11,5),-1,"COPX"));
	pLabelDef->Add(FormatDef(TrkCoord,PRIO_TRACK_LABEL_ELEM_1,0,EtxData,4,QPoint(16,5),-1,"ETX"));
	pLabelDef->Add(FormatDef(TrkRvsm,0,0,RvsmData,2,QPoint(26,5),-1));
	pLabelDef->Add(FormatDef(Trk833,0,0,N833Data,2,QPoint(29,5),-1));

	pTrackDef->LabelDefTable.Add(pLabelDef);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method describes the content of a Limited Data Block. 
///  This definition is added to the TrackDef object.
///  Argument 1 : LabelDef*, Definition of a track data block
///  Argument 2 : TrackDef*, Definition of a track presentation
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk045, CIAsdFctRwsTppDbk050, CIAsdFctRwsTppDbk070,
///					CIAsdFctDfdInt005, CIAsdFctDfdCicM3a025
//
//-----------------------------------------------------------------------------
void CViewTrack::LimitedDB(LabelDef* pLabelDef, TrackDef* pTrackDef)
{
	// Limited data block
	pLabelDef=new LabelDef;
	//line 0 
	pLabelDef->Add(FormatDef(TrkEmg,PRIO_TRACK_LABEL_ELEM_1,0,EmrgData,2,QPoint(1,0),-1));
	pLabelDef->Add(FormatDef(TrkAls,PRIO_TRACK_LABEL_ELEM_1,0,AlertData,2,QPoint(4,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,MiData,1,QPoint(7,0),-1));
	pLabelDef->Add(FormatDef(TrkCoord,0,0,CoordData,1,QPoint(8,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FltrulData,1,QPoint(9,0),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,ModeSData,1,QPoint(10,0),CRwdView::fld_ldb_modes));

	//Line 1
	pLabelDef->Add(FormatDef(TrkMism,PRIO_TRACK_LABEL_ELEM_1,0,ArcidMismatch,1,QPoint(0,1),-1));
	pLabelDef->Add(FormatDef(TrkArcid,PRIO_TRACK_LABEL_ELEM_1,1,ArcidData,8,QPoint(1,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,0,0,FlightCatData,1,QPoint(9,1),-1));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,WtcData,1,QPoint(10,1),-1,"W"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,SlashData,1,QPoint(11,1),CRwdView::fld_ldb_atyp,"/"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,ArctypData,4,QPoint(12,1),CRwdView::fld_ldb_atyp,"ATYP"));
	pLabelDef->Add(FormatDef(TrkTssrPb,0,0,TssrPbData,4,QPoint(17,1),-1));
	
	//Line 2
	pLabelDef->Add(FormatDef(TrkDef,0,0,AsterData,1,QPoint(0,2),-1));
	pLabelDef->Add(FormatDef(TrkAfl,PRIO_TRACK_LABEL_ELEM_1,1,AflData,4,QPoint(1,2),-1,"----"));
	pLabelDef->Add(FormatDef(TrkAtt,0,0,FAttData,1,QPoint(5,2),-1));
	pLabelDef->Add(FormatDef(TrkCfl,PRIO_TRACK_LABEL_ELEM_1,0,CflData,3,QPoint(6,2),-1,"CFL"));
	pLabelDef->Add(FormatDef(TrkDef,PRIO_TRACK_LABEL_ELEM_1,0,RwyData,3,QPoint(10,2),CRwdView::fld_ldb_rwy,"RWY"));
	
	//Line 3
	pLabelDef->Add(FormatDef(TrkDef,0,0,GspdData,3,QPoint(1,3),CRwdView::fld_ldb_gsp));
	pLabelDef->Add(FormatDef(TrkRocd,PRIO_TRACK_LABEL_ELEM_1,0,VspdData,2,QPoint(5,3),CRwdView::fld_ldb_rocd,"CD"));
	pLabelDef->Add(FormatDef(TrkDef,0,0,RvrData,4,QPoint(8,3),CRwdView::fld_ldb_rvr));

	pTrackDef->LabelDefTable.Add(pLabelDef);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method loads all information needed to display a track presentation (m_pTrackDef parameter):
///  colors, fonts, track elements, different data blocks, leader line orientations.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::InitTables()
{
	//load font families
	QString fontNames[6]={"SMALL_NOT_CS_AFS_DBLOCK","SMALL_CS_AFS_DBLOCK","MEDIUM_NOT_CS_AFS_DBLOCK","MEDIUM_CS_AFS_DBLOCK","BIG_NOT_CS_AFS_DBLOCK","BIG_CS_AFS_DBLOCK"};
	for (int fnt=0;fnt<6;fnt++)
		m_FontTable.SetAtGrow(fnt,CFontMngr::GetFontByName(fontNames[fnt]));

	//load of track colors
	for (int cl=0;cl<=ModeSTag;cl++)
		m_ColorTable.Add(CColorMngr::GetColorNb(TrkColorNames[cl]));

	//Init of a new TrackDef object, orientation table and elements list
	TrackDef* pTrackDef=new TrackDef();
	CreateTrackElements(pTrackDef);

	//Init of the data block definitions
	LabelDef* pLabelDef=new LabelDef;
	LimitedUncoupledDB(pLabelDef,pTrackDef);
	FullAccDB(pLabelDef,pTrackDef);
	FullAppDB(pLabelDef,pTrackDef);
	TaggedLimitedUncoupledDB(pLabelDef,pTrackDef);
	ExtendedAccDB(pLabelDef,pTrackDef);
	ExtendedAppDB(pLabelDef,pTrackDef);
	LimitedDB(pLabelDef,pTrackDef);
	ExtendedTaggedDB(pLabelDef,pTrackDef);

	m_pTrackDef=pTrackDef;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method empties the m_pTrackDef parameter.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::RemoveTables()
{
	delete m_pTrackDef;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether or not the line defined by the two first points 
///  in argument intersects the line defined by the two last points in argument.
///  Argument 1 : QPoint, one point of the first line
///  Argument 2 : QPoint, one point of the first line
///  Argument 3 : QPoint, one point of the second line
///  Argument 4 : QPoint, one point of the second line
///
///  RETURNS: 
///  bool, whether the lines put in argument intersect or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
BOOL CViewTrack::Intersect(QPoint p1, QPoint p2, QPoint p3, QPoint p4) const
{ 
   return ((( CCW(p1, p2, p3) * CCW(p1, p2, p4)) <= 0)
        && (( CCW(p3, p4, p1) * CCW(p3, p4, p2)  <= 0) )) ;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function determines, given three points, if when travelling from the first to
///  the second then to the thirdpoint, we travel in a counterclockwise direction.
///  Argument 1 : QPoint, first point
///  Argument 2 : QPoint, second point
///  Argument 3 : QPoint, third point
///
///  RETURNS: 
///  int, 1 if the movement is in a counterclockwise direction, -1 if not.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CViewTrack::CCW(QPoint p0, QPoint p1, QPoint p2) const
{ 
   long dx1, dx2 ;
   long dy1, dy2 ;

	// This is basically a slope comparison: we don't do divisions because
	// of divide by zero possibilities with pure horizontal and pure
	// vertical lines.
	dx1 = p1.x() - p0.x() ;
	dx2 = p2.x() - p0.x() ;
	dy1 = p1.y() - p0.y() ;
	dy2 = p2.y() - p0.y() ;

   return ((dx1 * dy2 > dy1 * dx2) ? 1 : -1) ;
} 

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to calculate the start and end position for the 
///  leader line according to the track position, the gap between the PPS and the
///  leader line, the orientation of the leader line...  
///  Argument 1 : int, orientation of the leader line
///  Argument 2 : int, length of the leader line
///  Argument 3 : QRect, rect of the data block
///  Argument 4 : QPoint, point at the left and middle height of the Identifier Data field
///  Argument 5 : QPoint, leader line ending position point to be set
///  Argument 6 : QPoint, leader line starting position point to be set
///  Argument 7 : QPoint, calculated track position to be set
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppLls005, CIAsdFctRwsTppLls010, CIAsdFctRwsTppLls021
//
//-----------------------------------------------------------------------------
void CViewTrack::ComputeOffset(int angle,int Len,QRect LabelRect,QPoint PosRect,QPoint& pnt1,QPoint& pnt2,QPoint& pnttrack) 
{
	double MaxLen=(float)_max(LabelRect.width(),LabelRect.height())*2;
	//angle in radian
	double ang=(180.0+angle)*2.0*3.14159/360.0;
	QPoint PosEnd,summit[5];
	//A point in the line starting from the PosRect in the direction of the leader
	//line is defined 
	PosEnd=QPoint(PosRect.x()+(int)(MaxLen*cos(ang)),PosRect.y()+(int)(MaxLen*sin(ang)));
	//the data block rect extremities are set in a table with 2 pixels less
	int offset=2;
	summit[0]=summit[4]=QPoint(LabelRect.left()-offset,LabelRect.top()-offset);
	summit[1]=QPoint(LabelRect.right()+offset,LabelRect.top()-offset);
	summit[2]=QPoint(LabelRect.right()+offset,LabelRect.bottom()+offset);
	summit[3]=QPoint(LabelRect.left()-offset,LabelRect.bottom()+offset);
	for (int i=0;i<4;i++)
	{
		//The intersection of the rect and the line PosRect-PosEnd is searched.
		if  (Intersect(PosRect,PosEnd,summit[i],summit[i+1]))
		{
			//This intersection point (summit[i]) x coordinate or y coordonnate
			//allows to calculate the length of the leader line to be removed in the 
			//data block (to PosRect)
			if (i%2==0)
				MaxLen=(double)(summit[i].y()-PosRect.y())/sin(ang);
			else
				MaxLen=(double)(summit[i].x()-PosRect.x())/cos(ang);
			pnt1=QPoint(PosRect.x()+(int)(MaxLen*cos(ang)),PosRect.y()+(int)(MaxLen*sin(ang)));
			pnt2=QPoint(pnt1.x()+(int)(Len*cos(ang)),pnt1.y()+(int)(Len*sin(ang)));
			pnttrack=QPoint(pnt1.x()+(int)((Len+8)*cos(ang)),pnt1.y()+(int)((Len+8)*sin(ang)));

			//leader line ending position point
			pnt1=QPoint(-pnt1.x(),-pnt1.y());
			//leader line starting position point
			pnt2=QPoint(-pnt2.x(),-pnt2.y());
			//track calculated position, taking into account the PosRect point and the
			//leader line orientation
			pnttrack=QPoint(-pnttrack.x(),-pnttrack.y());
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to close any opened context menu associated to a track.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::OnCloseMenu()
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	if (pTrkMngr->m_pTrackMenu)
		pTrkMngr->m_pTrackMenu = NULL;
	pTrkMngr->m_pMenuCallingTrack = NULL;
	pTrkMngr->m_pMenuCallingWnd = NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon the selection of an available item in the PPS context menu, the 
///  corresponding action is applied.
///  Argument 1 : UINT, selected menu item number
///  Argument 2 : CObject, not used in this function
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml005
//
//-----------------------------------------------------------------------------
void CViewTrack::OnCommand(UINT id, CObject *pDisplayWnd)
{
	int view;
	switch (id)
	{
	case IDC_TRKM:
		//The CWP enters the Track Mile line creation mode
		CTrkMiles::Create(this->m_pView,this->m_pTrack);
		break;
	case IDC_HALO:
		if (CRwdView::GetIsIndependent())
		{
			ForceHalo(true); 		
		}
		else
		{
			for (view=0;view<m_pTrack->m_ElemTable.GetSize();view++)
			{
				m_pTrack->m_ElemTable[view]->ForceHalo(true); 
			}
		}
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function computes the availability of the PPS menu items. 
///  Argument : CCmdUI, menu item
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	switch (pCmdUI->GetID())
	{
	case IDC_TRKM:
		{
		pCmdUI->Enable(!m_pTrack->m_pTrkMiles && !CRangeMark::IsRangeMarkInModif() && !CTrkMiles::IsInCreation() 
		&& !CVector::IsInCreation() && !CNewRange::IsInCreation() && !CRwdView::IsInViewModif());//not available if one Track mile line is already associated to the track
		}
		break;
	case IDC_HALO:
		pCmdUI->Enable(true);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns a rect around the current position of the track, in 
///  order to process the window framing 
///  Argument : QRect, rect to be set
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CViewTrack::GetSymbRect(QRect& rect)
{
  	rect=QRect(m_CurPos.x()-4,m_CurPos.y()-4,16,16);
}


//-----------------------------------------------------------------------------
//
/// \class CLabel
/// This class is used to manage the display of the data block of one track
/// presentation  
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of the class member parameters
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CLabel::CLabel()
{
	m_BoundingRect=QRect();
	m_Select = false;
	m_Type = label;
	m_RectTable.SetSize(MaxFld);
	m_ClrTable.SetSize(MaxFld);
	m_SelectFld=-1;
	m_IsOnField=false;
	m_IsMoving=false;
	m_IsDragged=false;
	m_CurDataBlock=-1;
	m_LastDataBlock=-1;
};

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CLabel&, CLabel object to be initialized
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CLabel::CLabel(CLabel& lab)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CLabel::~CLabel()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Initialization of the CLabel Object.
///  Argument 1 : LPVOID, inherited track object
///  Argument 2 : CView*, view associated to the track presentation
///  Argument 3 : CDisplayElement*, inherited CViewTrack object
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLabel::Init(LPVOID pData,CView* pView,CDisplayElement* pParent)
{
	m_pView = (CAsdView*)pView;
	m_pTrack = (CTrack*)pData;
	m_pParent = (CViewTrack*)pParent;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time an attribute of the class is modified or
///  may be impacted. The update is performed systematically before refreshing 
///  the display (call of OnDraw function).
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk066, CIAsdFctDfdCnt005, CIAsdFctDfdCnt010,
///					CIAsdFctDfdCnt015, CIAsdFctDfdCnt020,CIAsdFctRwsTppDbk065
//
//-----------------------------------------------------------------------------
void CLabel::OnUpdate()
{
	QSize cursize;
	QPainter dc(m_pView);
	m_PosRect=QPoint(0,0);
	bool PosRectFnd=false;
	int FntNb=m_pView->GetViewParam(CRwdView::font_size)*2;
	int CurBlock;

	//The current data block is calculated
	CurBlock = m_pParent->ComputeDataBlock();

	//If no data block is to be displayed we leave the function
	if ((CurBlock==-1) || (!m_pParent->m_LabelDisplayed))
	{
		m_BoundingRect=QRect();
		m_CurDataBlock=-1;
		m_LastDataBlock=-1;
		return;
	}

	//In case of change of data block the currently selected data field
	//is retrieved in order not to impact the user activity (closure of pop-ups...)
	if (CurBlock!=m_CurDataBlock)
	{
		int OldSelData=-1;
		if (m_SelectFld!=-1)
			OldSelData=m_pDef->GetAt(m_SelectFld).datanb;
		m_pDef=m_pDefTable->GetAt(CurBlock);
		if (m_SelectFld!=-1)
		{
			m_SelectFld=-1;
			m_IsOnField=false;
			for (int fd=0;fd<m_pDef->GetSize();fd++)
			{
				if (m_pDef->GetAt(fd).datanb==OldSelData)
				{
					m_SelectFld=fd;
					m_IsOnField=true;
					break;
				}
			}
		}
		m_LastDataBlock=m_CurDataBlock;
	}
	m_CurDataBlock=CurBlock;
	int CurLineNb=-1;
	int CurPosX=0,CurHeight=0,CurX=0,MinHeight,PaddingX,CurPosY=0;
	ElemDef CurElem;
	QString CurString,Def;
	QSize CurSize;
	dc.setFont(CViewTrack::m_FontTable[FntNb]);
	CurSize=dc.boundingRect(0,0,0,0,Qt::AlignLeft," ").size();
	PaddingX=CurSize.width();
	MinHeight=CurSize.height();
	bool ElemFoundInLine=true;

	//For each data field to be displayed in the data block, the position
	//and the rect is calculated
	for (int fld=0;fld<m_pDef->GetSize();fld++)
	{
		CurElem=m_pDef->GetAt(fld);
		Def.fill(' ',CurElem.length);
		//Change of line
		if (CurElem.pos.y()!=CurLineNb)
		{
			//If the line 0 or 3 is empty it is hidden and replaced by the following
			if (ElemFoundInLine || (CurLineNb != 0 && CurLineNb != 3))
				CurPosY+=CurHeight;
			ElemFoundInLine=false;
			if (CurHeight)
				CurHeight=MinHeight;
			CurX=CurElem.pos.x();
			CurPosX=CurX*PaddingX;
			CurLineNb=CurElem.pos.y();
		}
		//Data field added to the current line
		else
		{
			CurPosX+=(CurElem.pos.x()-CurX)*PaddingX;
			CurX=CurElem.pos.x();
		}
		dc.setFont(CViewTrack::m_FontTable[FntNb+CurElem.fontnb]);

		//CurString is set to the value of the data set to be displayed in order
		//to calculate the rect of the data field.
		//The value is retrieved, if no value is available and the data field has
		//a mnemonic,the mnemonic is used, else an empty string with the length of
		//the data field is used to prevent from data field flickering
		if (m_pTrack->m_DataTable[CurElem.datanb].length())
			CurString = m_pTrack->m_DataTable[CurElem.datanb];
		else
			if (CurElem.Mnemo.length())
				CurString=CurElem.Mnemo;
			else
				CurString=Def;
		//If the CFL is hidden and the track has the implicit focus, the value is forced to be displayed
		if (CurElem.datanb==CflData && m_pTrack->m_Select)
		{
			CurString=m_pTrack->m_CflToBeDisplayed;
			if (!m_pTrack->m_CflToBeDisplayed.length())
			{
				if (CurString.length()<3)
					CurString=CurElem.Mnemo;
			}
		}		
		//If the Flight Rule is hidden and the track has the implicit focus, the value is forced to be displayed
		if (CurElem.datanb==FltrulData && m_pTrack->m_TD.m_FltRul=="I" && (m_CurDataBlock==ExtendedAccDb || m_CurDataBlock==ExtendedAppDb)) 
			CurString="I";
		
		CurString = CurString.left(CurElem.length);
		cursize=dc.boundingRect(0,0,0,0,Qt::AlignLeft,CurString).size();
		if ((CurString.compare(Def)!=0) && ((CurElem.viewparam==-1) || (m_pView->GetViewParam(CurElem.viewparam)) || m_pParent->m_ForcedData[CurElem.datanb]))
		{
			m_RectTable[fld]=QRect(CurPosX,CurPosY,cursize.width(),cursize.height());
			ElemFoundInLine=true;
		}
		else
			m_RectTable[fld]=QRect();
		m_BoundingRect|=m_RectTable[fld];
		//The attach point of the leader line (m_PosRect) is set
		if (!PosRectFnd && (CurElem.datanb==ArcidData || CurElem.datanb==TssrData))
		{
			PosRectFnd=true;
			m_PosRect=QPoint(m_RectTable[fld].left(),m_RectTable[fld].center().y());
		}
		CurX+=CurElem.length;
		cursize=dc.boundingRect(0,0,0,0,Qt::AlignLeft,Def).size();
		CurPosX+=cursize.width();
		if (m_RectTable[fld].height()>CurHeight)
			CurHeight=cursize.height();
		if (CurElem.colornb==-1)
			m_ClrTable[fld]=m_pParent->m_Color;
		else
			m_ClrTable[fld]=m_pParent->ComputeColor(CurElem.colornb);
	}
	
	if (!m_BoundingRect.isEmpty())
	{
		m_BoundingRect.setLeft(0);
		m_BoundingRect.setTop(0);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to draw or redraw the data block
///  Argument 1 : QPainter*, drawing device
///  Argument 2 : int, not used in this function
///  Argument 3 : int, not used in this function
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen075, CIAsdFctRwsTppGen090, CIAsdFctDfdCnt005,
///					CIAsdFctDfdCnt010, CIAsdFctDfdCnt015, CIAsdFctDfdCnt020, 
///					CIAsdFctDfdCnt025, CIAsdFctDfdCnt075, CIAsdFctDfdCnt095,
///					CIAsdFctDfdInt020
//
//-----------------------------------------------------------------------------
void CLabel::OnDraw(QPainter* pDC,int )
{
	QRect DbRect=QRect();

	//If no data block is displayed we leave the function
	if (m_BoundingRect.isEmpty())
		return;

	//Display the Highlight frame around the data block if the track is selected or not
	if ((m_pTrack->m_Select) && (!m_pParent->m_LabelRect.isEmpty()))
	{
		//If the track is selected and the data block is stationary
		//m_StationaryDbRect rect is used
		if (m_pParent->m_bStationaryDb && !m_IsDragged)
		{
			DbRect=m_pParent->m_StationaryDbRect;
		}
		else
			DbRect=m_pParent->m_LabelRect;
		//In case the track is selected the data block background is filled
		pDC->fillRect(DbRect,CColorMngr::GetColor(m_pParent->m_ColorTable[SelBkClr]));
		if (m_pTrack->m_TD.m_IsHighlight)
		{
		  QPointArray PntTab(5);
		  pDC->setPen(CColorMngr::GetColor(m_pParent->m_ColorTable[FrameClr]));
		  PntTab[0]=PntTab[4]=DbRect.topLeft();
		  PntTab[1]=DbRect.topRight();
		  PntTab[2]=DbRect.bottomRight();
		  PntTab[3]=DbRect.bottomLeft();
		  pDC->drawPolyline(PntTab,0,5);
		}
	}
	else
	{
		if ((m_pTrack->m_TD.m_IsHighlight) && (!m_pParent->m_LabelRect.isEmpty()))
		{
			QPointArray PntTab(5);
			DbRect=m_pParent->m_LabelRect;
			pDC->setPen(CColorMngr::GetColor(m_pParent->m_ColorTable[FrameClr]));
			PntTab[0]=PntTab[4]=DbRect.topLeft();
			PntTab[1]=DbRect.topRight();
			PntTab[2]=DbRect.bottomRight();
			PntTab[3]=DbRect.bottomLeft();
			pDC->drawPolyline(PntTab,0,5);
		}

	}

	if (m_Select)
		pDC->setPen(CColorMngr::GetColor(CViewTrack::m_ColorTable[SelClr]));
	else
		pDC->setPen(QColor(255,255,255));
	QString CurString;
	ElemDef CurElem;
	int FntNb=m_pView->GetViewParam(CRwdView::font_size)*2;
	//All the data fields set to be displayed are drawn
	for (int fld=0;fld<m_pDef->GetSize();fld++)
	{
		//they are drawn if the flashing effect is not set while drawing
		if ((!m_GlobalFlashing) || (!(m_ClrTable[fld].IsFlashing)))
		{
			CurElem=m_pDef->GetAt(fld);
			//set the color
			if ((m_Select) && (m_SelectFld==fld))
				pDC->setPen(m_pParent->ComputeColor(TrkLabSel).Color);
			else
				pDC->setPen(m_ClrTable[fld].Color);

			//A new function is called to fill the data field according to the track presentation conditions
			CurString = FillDataField(CurElem);
			//set the font
			pDC->setFont(CViewTrack::m_FontTable[FntNb+CurElem.fontnb]);
			//Draw
			if (m_RectTable[fld].width())
				pDC->drawText(m_BoundingRect.left()+m_RectTable[fld].left(),m_BoundingRect.top()+m_RectTable[fld].bottom(),CurString);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the text to be displayed for the data field put in 
///  argument.
///  Argument : ElemDef, data field structure
///
///  RETURNS: 
///  QString, the text to be displayed in the data block
///
///  REQUIREMENT ID: CIAsdFctDfdCnt020, CIAsdFctDfdCnt075
//
//-----------------------------------------------------------------------------
QString CLabel::FillDataField(ElemDef CurElem)
{
	QString Def;
	Def.fill(' ',CurElem.length);
	bool Default=true;
	QString CurString;

	switch (CurElem.datanb)
	{
		case ArcidData:
			//The toggle of identifier is set at the CViewTrack level
			if (m_pParent->m_IsToggled && m_pTrack->m_TD.m_IsCoupled && m_pTrack->m_TD.m_IsTagged)
			{
				Default=false;
				CurString = m_pTrack->m_DataTable[TssrData];
			}
			break;
		case VspdData:
			//The unit to display the ROCD is set at the CViewTrack level
			if (m_pParent->m_ToggleRocdUnitFtMin!=CAsdApp::GetApp()->GetTrackMngr()->GetRocdUnit())
			{
				Default=false;
				if (m_pParent->m_ToggleRocdUnitFtMin)
				{
					if (m_pTrack->m_TD.m_RocdFtMin>99)
						CurString="99";
					else
						CurString.sprintf("%.2d",m_pTrack->m_TD.m_RocdFtMin);
				}
				else
				{
					if (m_pTrack->m_TD.m_RocdFtNm>99)
						CurString="99";
					else
						CurString.sprintf("%.2d",m_pTrack->m_TD.m_RocdFtNm);
				}
			}
			break;
		case CflData:
			//RCFL rule
			if (m_pTrack->m_Select)
			{
				Default=false;
				if (m_pTrack->m_DataTable[CurElem.datanb].length())
					CurString = m_pTrack->m_CflToBeDisplayed;
				else
				{
					if (CurElem.Mnemo.length())
						CurString=CurElem.Mnemo+Def;
					else
						CurString=Def;
				}
			}
			break;
			//The Flight Rule indicator (if it equals to "I") must be forced to be displayed if
			//the track has the implicit focus 
		case FltrulData:
			if (m_pTrack->m_TD.m_FltRul=="I" && (m_CurDataBlock==ExtendedAccDb || m_CurDataBlock==ExtendedAppDb)) 
			{
				Default=false;
				CurString="I";
			}
			break;
		default:
			break;
	}

	if (Default)
	{
		//If no specific display rule is applied, the return value is the 
		//data field value, the mnemonic or an empty string
		if (m_pTrack->m_DataTable[CurElem.datanb].length())
			CurString = m_pTrack->m_DataTable[CurElem.datanb]+Def;
		else
		{
			if (CurElem.Mnemo.length())
				CurString=CurElem.Mnemo+Def;
			else
				CurString=Def;
		}
	}
	
	CurString = CurString.left(CurElem.length);
	if (CurString == "----")
		int stop = 1;
	return CurString;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns a priority number according to the positions of the
///  mouse pointer and the data block
///  Argument 1 : point, position of the mouse pointer.
///  Argument 2 : bool, not used in this function.
///
///  RETURNS: 
///  int, selection priority number of the data block
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen060, CIAsdFctRwsTppGen080, CIAsdFctDfdInt020
//
//-----------------------------------------------------------------------------
int CLabel::GetPriority(QPoint point,bool )
{
	//If the user is manually moving the data block
	if (m_IsMoving)
		return PRIO_TRACK_LABEL_MOVING;
	//If no data fields are displayed or if the mouse is outside the data block
	if ((m_BoundingRect.isEmpty()) || (!m_pParent->m_LabelRect.contains(point)))
		return 0;
	int OldSelect=m_SelectFld;
	m_SelectFld=-1;
	m_IsOnField=false;
	QPoint pnt=point;
	pnt.setX(pnt.x()-m_BoundingRect.left());
	pnt.setY(pnt.y()-m_BoundingRect.top());
	//If the mouse is over a data field it is identified as selected
	for (int fld=0;fld<m_pDef->GetSize();fld++)
	{
		for (int fld=0;fld<m_pDef->GetSize();fld++)
		{
			if (m_RectTable[fld].contains(pnt))
			{
				m_SelectFld=fld;
				m_IsOnField=true;
				break;
			}

		}

	}
	//If the selected data field has changed, the display is refreshed 
	if (m_SelectFld!=OldSelect)
		m_pView->InvalidateRect(&m_BoundingRect);
	//If the data field is not selectable, the selection is canceled
	if ((m_SelectFld!=-1) && (!m_pDef->GetAt(m_SelectFld).prio))
		m_SelectFld=-1;
	//If the data field is selectable, its associated priority is returned
	//in the other case the priority of the data block background is returned.
	if (m_SelectFld!=-1)
	{
		if (IsSelectable(m_pDef->GetAt(m_SelectFld).datanb))
			return (m_pDef->GetAt(m_SelectFld).prio);
		else
		{
			m_SelectFld=-1;
			return PRIO_TRACK_LABEL;
		}
	}
	else
		return PRIO_TRACK_LABEL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the mouse pointer is moved over a track
///  data block.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppLls035
//
//-----------------------------------------------------------------------------
void CLabel::OnMouseMove(UINT nFlags,QPoint point)
{
	//Upon a click and drag AB
	if (m_IsMoving)
	{
		//If the manual orientation mode is activated and the user has not released
		//the mouse pointer, the data block orientation is updated 
		if (m_IsDragged)
		{
			if ((point.y()-m_pParent->m_CurPos.y())*(point.y()-m_pParent->m_CurPos.y())+(point.x()-m_pParent->m_CurPos.x())*(point.x()-m_pParent->m_CurPos.x())>16)
			{
				int newpos=((int)((22.5+atan2(point.y()-m_pParent->m_CurPos.y(),point.x()-m_pParent->m_CurPos.x())*180.0/3.14159+360.0)/45.0))%8;
				int oldpos=m_pParent->m_LeaderPos;
				m_pParent->m_LeaderPos=newpos;
				if (oldpos!=m_pParent->m_LeaderPos)
				{
					m_pParent->m_ManualLeaderPos=true;
					m_pParent->OnUpdate();
				}
			}
		}
		//If the user has dragged the mouse pointer sufficiently, the manual 
		//orientation mode is activated
		else
		{
			if ((point.y()-m_DownPoint.y())*(point.y()-m_DownPoint.y())+(point.x()-m_DownPoint.x())*(point.x()-m_DownPoint.x())>25)
			{
				m_IsDragged=true;
				m_pParent->m_DBIsBeingDragged=true;
				m_pParent->OnUpdate();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the user performs a click AB release over 
///  a data block.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLabel::OnLButtonUp(UINT nFlags,QPoint point)
{
	//Upon the release AB action the manual data block orientation mode is canceled.
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	m_IsMoving=false;
	m_IsDragged=false;
	m_pParent->m_DBIsBeingDragged=false;
	m_pParent->m_ModifElem=-1;
	if (!GetPriority(point))
		m_pParent->SelectElement(false);
	m_pParent->OnUpdate();
	for (int view=0;view<pTrkMngr->m_ViewTable.GetSize();view++)
		((CRwdView*)pTrkMngr->m_ViewTable[view])->SetModifyElement(NULL);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selected status of the data block to the one put in
///  argument.
///  Argument : bool, selected status of the data block to be applied.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk120, CIAsdFctRwsTppDbk125
//
//-----------------------------------------------------------------------------
void CLabel::SelectElement(bool Select)
{
	m_Select = Select;
	//If the data block has the implicit focus, the data block becomes stationary
	m_pParent->m_bStationaryDb=Select;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function toggles the ROCD unit to be applied.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLabel::RocdClickAB()
{
	bool newRocdUnit=!(m_pParent->m_ToggleRocdUnitFtMin);
	//The toggle applies only to the current view if the radar window mode
	//is independent
	if (CRwdView::GetIsIndependent())
	{
		m_pParent->m_ToggleRocdUnitFtMin=newRocdUnit;
		m_pParent->OnUpdate();
	}
	//to all the views in the other case
	else
	{
		for (int view=0;view<m_pTrack->m_ElemTable.GetSize();view++)
		{
			m_pTrack->m_ElemTable[view]->m_ToggleRocdUnitFtMin=newRocdUnit;
			m_pTrack->m_ElemTable[view]->OnUpdate();
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the user performs a click AB over a track
///  data block.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppLls035, CIAsdFctDfdCicCfl010, CIAsdFctDfdCicM3a005,
///					CIAsdFctDfdCicArr005, CIAsdFctDfdCicArr015, CIAsdFctDfdCicCst065,
///					CIAsdFctDfdCicAlt005, CIAsdFctDfdCicAlt010, CIAsdFctDfdCicAcw005,
///					CIAsdFctDfdCicAcw015, CIAsdFctDfdCicAcw020, CIAsdFctDfdCicRep005,
///					CIAsdFctDfdCicDes005, CIAsdFctDfdCicMos015
//
//-----------------------------------------------------------------------------
void CLabel::OnLButtonDown(UINT nFlags,QPoint Pos)
{
	QPoint pnt=m_pView->mapToGlobal(Pos);
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	//If the click AB is performed on the background of the data block or over a not
	//selectable data field the manual orientation mode is initialized (not yet validated).
	if (m_SelectFld==-1)
	{
		if (!m_IsOnField)
		{
			bool IsModifiable=true;
			for (int view=0;view<pTrkMngr->m_ViewTable.GetSize();view++)
				IsModifiable &= ((CRwdView*)pTrkMngr->m_ViewTable[view])->IsModifyNull();
			if (IsModifiable)
			{
				for (int view=0;view<pTrkMngr->m_ViewTable.GetSize();view++)
				{
					((CRwdView*)pTrkMngr->m_ViewTable[view])->SetModifyElement(m_pParent->m_pTrack->m_ElemTable[view]);
				}
				m_IsMoving=true;
				m_DownPoint=Pos;
				m_pParent->m_ModifElem=label;
			}
		}
	}
	else
	{
		switch (m_pDef->GetAt(m_SelectFld).datanb)
		{
			//The specific interactions are detailed on the following called functions
			case ArcidData:
				m_pTrack->ArcidClickAB(pnt, m_pParent);	
				break;
			case CflData:
				m_pTrack->CflClickAB(pnt);
				break;
			case AflData:
				m_pTrack->AflClickAB(pnt);
				break;
			case TssrData:
				m_pTrack->TssrClickAB(pnt);
				break;
			case MiData:
				m_pTrack->MiClickAB(m_pParent);
				break;
			case WtcData:
				m_pTrack->WtcClickAB(pnt);
				break;
			case ArctypData:
				m_pTrack->ATypClickAB(pnt);
				break;
			case AdesData:
				m_pTrack->AdesClickAB(pnt);
				break;
			case AdepData:
				m_pTrack->AdepClickAB(pnt);
				break;
			case AssrData:
				m_pTrack->AssrClickAB(pnt);
				break;
			case RwyData:
				m_pTrack->RwyClickAB(pnt);
				break;
			case VspdData:
				RocdClickAB();
				break;
				//In case the track has a CS-CS Tag mismatch the mismatch context menu is invoked
			case ArcidMismatch:
				if (m_pTrack->IsSolveCSMismatchAllowed())
				{
					CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
					pTrkMngr->RemoveMenu();
					pTrkMngr->m_pMenuCallingTrack = m_pTrack;
					pTrkMngr->m_pMenuCallingWnd = m_pView;
					pTrkMngr->m_pTrackMenu = new CAsdMenu();
					if (m_pTrack->m_TD.m_IsTagged)
						pTrkMngr->m_pTrackMenu->AddItem(m_pTrack->m_TD.m_CsTag, IDC_ARCID1_MISMATCH, true);
					else
						pTrkMngr->m_pTrackMenu->AddItem(m_pTrack->m_TD.m_Arcid, IDC_ARCID1_MISMATCH, true);
					pTrkMngr->m_pTrackMenu->AddItem(m_pTrack->m_TD.m_ConflictingArcid, IDC_ARCID2_MISMATCH, true);
					
					pTrkMngr->m_pTrackMenu->TrackPopupMenu(pnt.x(), pnt.y(), m_pTrack);
				}
				break;
			case RflData:
				m_pTrack->RflClickAB(pnt);
				break;
			default:
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the user performs a click WB over a track
///  data block. The action is automatically applied.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLabel::OnMButtonDown(UINT nFlags,QPoint pnt)
{
	QPoint pos;
	if (m_SelectFld!=-1)
	{
		switch (m_pDef->GetAt(m_SelectFld).datanb)
		{
		case ArcidData:
			//The specific function ArcidClickWB is called
			pos=m_pView->mapToGlobal(pnt);
			m_pTrack->ArcidClickWB(pos, m_pView);
			break;
		case AlertData:
			//The DU alert is acknowledged
			m_pTrack->AcknowledgeDU();
			break;
		case EmrgData:
			//The emergency if any is acknowledged
			if ((!m_pTrack->m_TD.m_EmergAck) &&  CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Ack_Emergency_SSR_Code))
			{
				m_pTrack->m_TD.m_EmergAck=true;
				if (m_pTrack->m_pHEvent)
					m_pTrack->m_pHEvent->OnTrackEmergAck(m_pTrack, true);
				m_pParent->OnUpdate();
			}
			break;
		default:
			break;
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the user performs a click SB over a track
///  data block. The action is automatically applied or the event is 
///  transmitted to the impacted element object.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk035, CIAsdFctRwsTppDbk055
//
//-----------------------------------------------------------------------------
void CLabel::OnRButtonDown(UINT nFlags,QPoint pnt)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	int view;
	QPoint pos;
	bool FromExtended=false;
	if (m_CurDataBlock==ExtendedAccDb || m_CurDataBlock==ExtendedAppDb || m_CurDataBlock==ExtendedTaggedDb)
		FromExtended=true;


	if (m_SelectFld!=-1)
	{
		switch (m_pDef->GetAt(m_SelectFld).datanb)
		{
		case ArcidData:
		case TssrData:
			pos=m_pView->mapToGlobal(pnt);
			m_pTrack->ArcidClickSB(pos, m_pParent, m_pView);
			break;
	//If the data block is extended and the data field is optional, the display
	//status of the data field is toggled
		case ArctypData:
			//For the ATYP data field in addition, the slash must be displayed to separate
			//WTC from ATYP data field
			if (FromExtended)
			{
				for (view=0;view<pTrkMngr->m_ViewTable.GetSize();view++)
				{
					if (CRwdView::GetIsIndependent())
					{
						if (m_pView==((CRwdView*)pTrkMngr->m_ViewTable[view]))
						{
							m_pParent->m_ForcedData[m_pDef->GetAt(m_SelectFld).datanb]=!m_pParent->m_ForcedData[m_pDef->GetAt(m_SelectFld).datanb];
							m_pParent->m_ForcedData[SlashData]=!m_pParent->m_ForcedData[SlashData];
							m_pParent->OnUpdate();
						}
					}
					else
					{
						m_pTrack->m_ElemTable[view]->m_ForcedData[m_pDef->GetAt(m_SelectFld).datanb]=!m_pTrack->m_ElemTable[view]->m_ForcedData[m_pDef->GetAt(m_SelectFld).datanb];
						m_pTrack->m_ElemTable[view]->m_ForcedData[SlashData]=!m_pTrack->m_ElemTable[view]->m_ForcedData[SlashData];
						m_pTrack->m_ElemTable[view]->OnUpdate();
					}
				}
			}
			break;
		case RwyData:
		case GspdData:
		case ModeSData:
		case XptData:
		case XflData:
		case EtxData:
		case VspdData:
		case RvrData:
		case AdesData:
		case AdepData:
		case SidData:
			if (FromExtended)
			{
				for (view=0;view<pTrkMngr->m_ViewTable.GetSize();view++)
				{
					if (CRwdView::GetIsIndependent())
					{
						if (m_pView==((CRwdView*)pTrkMngr->m_ViewTable[view]))
						{
							m_pParent->m_ForcedData[m_pDef->GetAt(m_SelectFld).datanb]=!m_pParent->m_ForcedData[m_pDef->GetAt(m_SelectFld).datanb];
							m_pParent->OnUpdate();
						}
					}
					else
					{
						m_pTrack->m_ElemTable[view]->m_ForcedData[m_pDef->GetAt(m_SelectFld).datanb]=!m_pTrack->m_ElemTable[view]->m_ForcedData[m_pDef->GetAt(m_SelectFld).datanb];
						m_pTrack->m_ElemTable[view]->OnUpdate();
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else
	{
		//If the click is perform on a part of the data block, other than
		//an interactable field, the extended label is toggled
		if (!m_IsOnField)
		{
			m_pParent->m_Extended=!m_pParent->m_Extended;
			m_pParent->OnUpdate();
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether an optional data field is selectable or not
///  for a full data block.
///  Argument : int, data field number
///
///  RETURNS: 
///  bool, whether the field is selectable or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CLabel::IsOptionalForFullDB(int field)
{
	bool selectable=false;
	//The field is selectable if
	//the track is assumed
	//and the track is not in hold
	//and the data field is not set to be displayed in the Track Control window
	if (m_pTrack->m_TD.m_IsAoc && !m_pView->GetViewParam(field))
		selectable=true;
	return selectable;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether an optional data field is selectable or not
///  for a limited data block.
///  Argument : int, data field number
///
///  RETURNS: 
///  bool, whether the field is selectable or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CLabel::IsOptionalForLimitedDB(int field)
{
	bool selectable=false;
	//The field is selectable if
	//the track is not assumed
	//and the track is not in hold
	//and the data field is not set to be displayed in the Track Control window
	if (!m_pTrack->m_TD.m_IsAoc && !m_pView->GetViewParam(field))
		selectable=true;
	return selectable;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether an optional data field is selectable or not
///  for a tagged data block.
///  Argument : int, data field number
///
///  RETURNS: 
///  bool, whether the field is selectable or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CLabel::IsOptionalForTaggedDB(int field)
{
	bool selectable=false;
	//The field is selectable if
	//the track is tagged
	//and the data field is not set to be displayed in the Track Control window
	if (m_pTrack->m_TD.m_IsTagged && !m_pView->GetViewParam(field))
		selectable=true;
	return selectable;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the optional data field put in argument
///  is selectable or not.
///  Argument : int, data field number
///
///  RETURNS: 
///  bool, whether the field is selectable or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CLabel::IsOptionalFieldSelectable(int DataNb)
{
	bool doit=false;
	bool FromExtended=false;
	if (m_CurDataBlock==ExtendedAccDb || m_CurDataBlock==ExtendedAppDb || m_CurDataBlock==ExtendedTaggedDb)
		FromExtended=true;

	switch (DataNb)
	{
		case VspdData:
			if (FromExtended && (IsOptionalForFullDB(CRwdView::fld_fdb_rocd) || IsOptionalForLimitedDB(CRwdView::fld_ldb_rocd)
				|| IsOptionalForTaggedDB(CRwdView::fld_tag_rocd)))
				doit=true;
			break;
		//The following fields depend on the same check box status in the Track Control Window
		case XptData:
		case XflData:
			if (m_CurDataBlock==ExtendedAccDb)
				doit=true;
			break;
		case RvrData:
			if (FromExtended && (IsOptionalForFullDB(CRwdView::fld_fdb_rvr) || IsOptionalForLimitedDB(CRwdView::fld_ldb_rvr)))
				doit=true;
			break;
		case SidData:
			if (m_CurDataBlock==ExtendedAppDb && IsOptionalForFullDB(CRwdView::fld_fdb_sid))
				doit=true;
			break;
		case AdesData:
			if (m_CurDataBlock==ExtendedAppDb && IsOptionalForFullDB(CRwdView::fld_fdb_ades))
				doit=true;
			break;
		case AdepData:
			if (m_CurDataBlock==ExtendedAccDb)
				doit=true;
			break;
		case ArctypData:
			if (FromExtended && (IsOptionalForFullDB(CRwdView::fld_fdb_atyp) || IsOptionalForLimitedDB(CRwdView::fld_ldb_atyp)
				|| IsOptionalForTaggedDB(CRwdView::fld_tag_atyp)))
				doit=true;
			break;
		case RwyData:
			if ((m_CurDataBlock==ExtendedAccDb || m_CurDataBlock==ExtendedAppDb)  && ((IsOptionalForFullDB(CRwdView::fld_fdb_rwy) || IsOptionalForLimitedDB(CRwdView::fld_ldb_rwy))))
				doit=true;
			break;
		case GspdData:
			if (m_CurDataBlock==ExtendedAccDb && (IsOptionalForFullDB(CRwdView::fld_fdb_gsp)) 
				|| FromExtended && (IsOptionalForLimitedDB(CRwdView::fld_ldb_gsp) 
				|| IsOptionalForTaggedDB(CRwdView::fld_tag_gsp)))
				doit=true;
			break;
		case ModeSData:
			if (FromExtended && (IsOptionalForFullDB(CRwdView::fld_fdb_modes) || IsOptionalForLimitedDB(CRwdView::fld_ldb_modes)
				|| IsOptionalForTaggedDB(CRwdView::fld_tag_modes)))
				doit=true;
			break;
		default:
			break;
	}	

	return doit;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the data field put in argument is selectable or not.
///  Argument : int, data field number
///
///  RETURNS: 
///  bool, whether the field is selectable or not
///
///  REQUIREMENT ID: CIAsdFctDfdCicCfl010, CIAsdFctDfdCicM3a005, CIAsdFctDfdCicRfl010
//
//-----------------------------------------------------------------------------
bool CLabel::IsSelectable(int DataNb)
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	BYTE sic = 0;
	BYTE sac = 0;
	QString	Ades;
	int trknum = -1;
	m_pTrack->GetTrkNum(sic, sac, trknum);
	
	switch (DataNb)
	{
	case ArcidData:
		return true;
		break;
	case CflData:
		if (m_pTrack->IsCflAllowed())
			return true;
		break;
	case AflData:
		if (m_pTrack->IsRepAllowed())
			return true;
		break;
	case RflData:
		if (m_pTrack->IsRflAllowed())
			return true;
		break;
	case TssrData:
		if (!m_pTrack->m_TD.m_IsCoupled)
			return true;
		break;
	case AlertData:
		if (m_pTrack->m_DUStatus==CTrack::Alert)
			return true;
		break;
	case EmrgData:
		if ((!m_pTrack->m_TD.m_EmergAck) &&  CAsdApp::GetApp()->FunctionEligibility(CWFAttrib::Ack_Emergency_SSR_Code))
			return true;
		break;
	case WtcData:
		if (m_pTrack->IsWtcAllowed())
			return true;
		break;
	case ArctypData:
		if (m_pTrack->IsATypAllowed() || IsOptionalFieldSelectable(DataNb))
			return true;
		break;
	case AdesData:
		if (m_pTrack->IsAdesAdepAllowed() || IsOptionalFieldSelectable(DataNb))
			return true;
		break;
	case AdepData:
		if (m_pTrack->IsAdesAdepAllowed() || IsOptionalFieldSelectable(DataNb))
			return true;
		break;
	case AssrData:
		if (m_pTrack->IsAssrAllowed())
			return true;
		break;
	case ArcidMismatch:
		if (m_pTrack->IsSolveCSMismatchAllowed())
			return true;
		break;
	case VspdData:
		return true;
		break;
	case RwyData:
		Ades = m_pTrack->m_DataTable[AdesData];
		if (m_pTrack->IsRwyAllowed() && CDataMngr::IsBGAdes(Ades))
			return true;
		break;
	case XptData:
	case XflData:
	case EtxData:
		return false;
		break;
	case GspdData:
	case ModeSData:
	case RvrData:
	case SidData:
		return IsOptionalFieldSelectable(DataNb);
		break;
	default:
		break;
	}
	return false;
}


//-----------------------------------------------------------------------------
//
/// \class CSymbol
/// This class is used to manage the display of the Present Position Symbol 
/// of one track presentation.
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of the class member parameters
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CSymbol::CSymbol()
{
	m_Type = symb;
	m_Select = false;
	m_pSymbol = NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class.
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CSymbol::CSymbol(CSymbol& symb)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class.
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CSymbol::~CSymbol()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Initialization of the CSymbol Object.
///  Argument 1 : LPVOID, inherited track object
///  Argument 2 : CView*, view associated to the track presentation
///  Argument 3 : CDisplayElement*, inherited CViewTrack object
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CSymbol::Init(LPVOID pData,CView* pView,CDisplayElement* pParent)
{
	m_pView = (CAsdView*)pView;
	m_pTrack = (CTrack*)pData;
	m_pParent = (CViewTrack*)pParent;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time an attribute of the class is modified or
///  may be impacted. The update is performed systematically before refreshing 
///  the display (call of OnDraw function).
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppPps005, CIAsdFctRwsTppPps010, CIAsdFctRwsTppPps015, 
///					CIAsdFctRwsTppPps020, CIAsdFctRwsTppPps025
//
//-----------------------------------------------------------------------------
void CSymbol::OnUpdate()
{
	m_BoundingRect=QRect();
	QPoint pos=m_pParent->m_CurPos;
	m_pSymbol=NULL;
	//The track symbol is retrieved from the offline definition
	if (m_pParent->m_SymbolDisplayed)
	{
		//Coasted PPS
		if (m_pTrack->m_IsCoasted)
			m_pSymbol=CGraphicSymbol::GetSymbol("TRACK_COASTED");
		//If the track is not coasted and is coupled or tagged
		if ((!m_pSymbol) && ((m_pTrack->m_TD.m_IsCoupled) || (m_pTrack->m_TD.m_IsTagged)))
		{
			//If the track is assumed by another sector
			if (!m_pTrack->IsAoc() && m_pTrack->m_TD.m_Mnemo)
			{
				//The letter of the assuming CSU is set as symbol
				QString SymbolName="TRACK_";
				m_pSymbol=CGraphicSymbol::GetSymbol(SymbolName+m_pTrack->m_TD.m_Mnemo);
			}

		}
		// In any other case, the type of the track, determines the type of symbol
		if (!m_pSymbol)
		{
			if ((m_pTrack->m_TD.m_IsPrimary) && (!m_pTrack->m_TD.m_IsSecondary))

				m_pSymbol=CGraphicSymbol::GetSymbol("TRACK_PRIM");
			if ((!m_pTrack->m_TD.m_IsPrimary) && (m_pTrack->m_TD.m_IsSecondary))
				m_pSymbol=CGraphicSymbol::GetSymbol("TRACK_SSR");
			if ((m_pTrack->m_TD.m_IsPrimary) && (m_pTrack->m_TD.m_IsSecondary))
				m_pSymbol=CGraphicSymbol::GetSymbol("TRACK_COMBINED");
		}
		if (m_pSymbol)
		{
			m_BoundingRect=m_pSymbol->GetBoundingRect(pos);
		}
	}
	if (m_pDef->colornb==-1)
		m_Color = m_pParent->m_Color;
	else
		m_Color = m_pParent->ComputeColor(m_pDef->colornb);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to draw or redraw the PPS.
///  Argument 1 : QPainter*, drawing device
///  Argument 2 : int, not used in this function
///  Argument 3 : int, not used in this function
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppPps030, CIAsdFctRwsTppPps035
//
//-----------------------------------------------------------------------------
void CSymbol::OnDraw(QPainter* pDC,int )
{
	if (m_BoundingRect.isEmpty())
		return;
	//If the flashing is set the PPS is not displayed
	if ((m_GlobalSpiFlashing) && (m_Color.IsFlashing))
		return;
	//Set the color
	if ((m_Select) || (m_pParent->m_Hooked) || m_pTrack->m_Select)
		pDC->setPen(m_pParent->ComputeColor(TrkSymbSel).Color);
	else
		pDC->setPen(m_Color.Color);

	//Draw symbol
	if (m_pSymbol)
		m_pSymbol->Draw(pDC,m_pParent->m_CurPos);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selected status of the PPS to the one put in
///  argument.
///  Argument : bool, selected status of the PPS to be applied.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CSymbol::SelectElement(bool Select)
{
	if (m_Select!=Select)
	{
		//If the PPS has the implicit focus, the data block background is filled-in
		m_pParent->m_HookedSymbol = Select;
		m_pParent->OnUpdate();
	}
	m_Select = Select;
}
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns a priority number according to the position of the
///  mouse pointer and the PPS
///  Argument 1 : point, position of the mouse pointer.
///  Argument 2 : bool, not used in this function.
///
///  RETURNS: 
///  int, selection priority number of the PPS
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen061, CIAsdFctRwsTppGen081
//
//-----------------------------------------------------------------------------
int CSymbol::GetPriority(QPoint point,bool )
{
	if (m_BoundingRect.contains(point))
		return m_pDef->prio;
	else
		return 0;
}


//-----------------------------------------------------------------------------
//
/// \class CPastPos
/// This class is used to manage the display of the afterglows 
/// of one track presentation.
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of the class member parameters
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CPastPos::CPastPos()
{
	m_Type = pastp;
	m_Select = false;
	m_pSymbol = CGraphicSymbol::GetSymbol(' ');
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CPastPos&, CPastPos object to be initialized
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CPastPos::CPastPos(CPastPos& pastpos)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CPastPos::~CPastPos()
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Initialization of the CPastPos Object.
///  Argument 1 : LPVOID, inherited track object
///  Argument 2 : CView*, view associated to the track presentation
///  Argument 3 : CDisplayElement*, inherited CViewTrack object
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CPastPos::Init(LPVOID pData,CView* pView,CDisplayElement* pParent)
{
	m_pView = (CAsdView*)pView;
	m_pTrack = (CTrack*)pData;
	m_pParent = (CViewTrack*)pParent;
	//Two symbols are available for afterglows: coasted and not-coasted
	m_pCoastedSymbol=CGraphicSymbol::GetSymbol("TRACK_PASTCOASTED");
	m_pSymbol=CGraphicSymbol::GetSymbol("TRACK_PASTP");
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time an attribute of the class is modified or
///  may be impacted. The update is performed systematically before refreshing 
///  the display (call of OnDraw function).
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen015
//
//-----------------------------------------------------------------------------
void CPastPos::OnUpdate()
{
	//If the PPS is set to be displayed and an offline defined symbol is available 
	if ((m_pSymbol) && (m_pParent->m_SymbolDisplayed))
	{
		int pastposnb = m_pTrack->m_PastPos.GetSize();
		//Gets the number of afterglows to be displayed
		pastposnb=_min(pastposnb,m_pView->GetViewParam(CRwdView::hist_length));
		QPoint curpos;
		QRect rect;
		
		//Sets the rect to be refreshed
		m_BoundingRect=QRect();
		for (int pos=0;pos<pastposnb;pos++)
		{
			curpos = m_pTrack->m_PastPos[pos];
			m_pView->LPtoDP(&curpos);
			rect = m_pSymbol->GetBoundingRect(curpos);
			rect.addCoords(-1,-1,1,1);
			m_BoundingRect|=rect;
		}
	}
	else
		m_BoundingRect=QRect();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to draw or redraw the afterglows
///  Argument 1 : QPainter*, drawing device
///  Argument 2 : int, not used in this function
///  Argument 3 : int, not used in this function
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CPastPos::OnDraw(QPainter* pDC,int )
{
	if (m_BoundingRect.isEmpty())
		return;
	
	QPoint curpos;
	int pastposnb = m_pTrack->m_PastPos.GetSize();
	//Get the number of afterglows to be displayed (from the track control window)
	pastposnb=_min(pastposnb,m_pView->GetViewParam(CRwdView::hist_length));
	//Sets the color
	pDC->setPen(m_pParent->ComputeColor(m_pDef->colornb).Color);
	//Each afterglow (until the number to be displayed is reached), is associated 
	//to a symbol (coasted or not coasted) and displayed
	for (int pos=0;pos<pastposnb;pos++)
	{
		curpos = m_pTrack->m_PastPos[pos];
		m_pView->LPtoDP(&curpos);
		if (m_pTrack->m_PastCoasted[pos])
		{
			if (m_pCoastedSymbol)
				m_pCoastedSymbol->Draw(pDC,curpos);
		}
		else
		{
			if (m_pSymbol)
				m_pSymbol->Draw(pDC,curpos);
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns a priority number according to the position of the
///  mouse pointer and the afterglows.
///  Argument 1 : point, position of the mouse pointer.
///  Argument 2 : bool, not used in this function.
///
///  RETURNS: 
///  int, selection priority number of the data block
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int  CPastPos::GetPriority(QPoint point,bool )
{
	if (m_BoundingRect.isEmpty())
		return 0;
	QPoint curpos;
	QRect rect;
	int pastposnb = m_pTrack->m_PastPos.GetSize();
	pastposnb=_min(pastposnb,m_pView->GetViewParam(CRwdView::hist_length));

	for (int pos=0;pos<pastposnb;pos++)
	{
		curpos = m_pTrack->m_PastPos[pos];
		m_pView->LPtoDP(&curpos);
		if (m_pSymbol)
			rect = m_pSymbol->GetBoundingRect(curpos);
		rect=rect.normalize();
		rect.addCoords(-4,-4,4,4);
		//If the position of the mouse pointer is within the rect of a displayed
		//afterglow, the function returns the priority set in the track definition
		//object
		if (rect.contains(point))
			return m_pDef->prio;
	}
 return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selected status of the Afterglows to the one put in
///  argument.
///  Argument : bool, selected status of the Afterglow to be applied.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CPastPos::SelectElement(bool Select)
{
	m_Select = Select;
}


//-----------------------------------------------------------------------------
//
/// \class CLeader
/// This class is used to manage the display of the leader line 
/// of one track presentation.
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of the class member parameters
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CLeader::CLeader()
{
	m_Type = lead;
	m_Select = false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CLeader&, CLeader object to be initialized
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CLeader::CLeader(CLeader& lead)
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class.
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CLeader::~CLeader()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time an attribute of the class is modified or
///  may be impacted. The update is performed systematically before refreshing 
///  the display (call of OnDraw function).
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppLls005, CIAsdFctRwsTppLls010
//
//-----------------------------------------------------------------------------
void CLeader::OnUpdate()
{
	//If no data block is needed, no leader line is displayed
	if (m_pParent->m_LabelDisplayed)
	{
		QPoint curpos,leaderpos,leadbegpos,labelpos;
		curpos = m_pParent->m_CurPos;
		//If the data block is stationary, only the starting point is recomputed
		//In the other case both extremities are recomputed
		if (m_pParent->m_bStationaryDb && !m_pParent->m_DBIsBeingDragged)
		{
			//offset is the pixel number from center of PPS
			int offset=8; 
			
			QRect TmpRect=QRect(curpos,m_PosEnd);
			double cos=TmpRect.width()/(sqrt(TmpRect.width()*TmpRect.width()+TmpRect.height()*TmpRect.height()));
       		double sin=TmpRect.height()/(sqrt(TmpRect.width()*TmpRect.width()+TmpRect.height()*TmpRect.height()));
			m_PosInit=QPoint(curpos.x() + (int)(offset*cos+0.5),curpos.y() + (int)(offset*sin+0.5));
		}
		else
		{
			m_pParent->ComputeOffset(m_pParent->m_pTrackDef->OrientationTable[m_pParent->m_LeaderPos].Angle,m_pParent->m_pTrackDef->OrientationTable[m_pParent->m_LeaderPos].Len,m_pParent->m_LabelRect,m_pParent->m_PosRect,leaderpos,leadbegpos,labelpos);
			m_PosInit = curpos-leadbegpos+labelpos;
			m_PosEnd = curpos-leaderpos+labelpos;
		}
		//The refresh rect is updated
		m_BoundingRect = QRect(m_PosInit,m_PosEnd);
		m_BoundingRect=m_BoundingRect.normalize();
		if ((m_BoundingRect.height()==0) || (m_BoundingRect.width()==0))
			m_BoundingRect.addCoords(-1,-1,1,1);
	}
	else
		m_BoundingRect=QRect();
	//Get the color
	if (m_pDef->colornb==-1)
		m_Color = m_pParent->m_Color;
	else
		m_Color = m_pParent->ComputeColor(m_pDef->colornb);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the leader line length in pixels.
///
///  RETURNS: 
///  int, leader line length
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CLeader::GetLeaderLineLenght()
{
	return (((m_PosInit.x()-m_PosEnd.x())*(m_PosInit.x()-m_PosEnd.x()))+((m_PosInit.y()-m_PosEnd.y())*(m_PosInit.y()-m_PosEnd.y())));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Initialization of the CLeader Object.
///  Argument 1 : LPVOID, inherited track object
///  Argument 2 : CView*, view associated to the track presentation
///  Argument 3 : CDisplayElement*, inherited CViewTrack object
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLeader::Init(LPVOID pData,CView* pView,CDisplayElement* pParent)
{
	m_pView = (CAsdView*)pView;
	m_pTrack = (CTrack*)pData;
	m_pParent = (CViewTrack*)pParent;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to draw or redraw the leader line.
///  Argument 1 : QPainter*, drawing device
///  Argument 2 : int, not used in this function
///  Argument 3 : int, not used in this function
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLeader::OnDraw(QPainter* pDC,int Prio)
{
	//If no leader line is displayed or according to the flasshing effect, nothing
	//is displayed
	if (m_BoundingRect.isEmpty())
		return;
	if ((m_GlobalFlashing) && (m_Color.IsFlashing))
		return;
	//Get the color (color of the track)
	QColor curcolor;
	if (m_Select)
		curcolor=m_pParent->ComputeColor(TrkLabSel).Color;
	else
		curcolor=m_Color.Color;
	QPen pen(curcolor,1,Qt::SolidLine);
	pDC->setPen(pen);
	pDC->drawLine(m_PosInit,m_PosEnd);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns a priority number according to the positions of the
///  mouse pointer and the leader line
///  Argument 1 : point, position of the mouse pointer.
///  Argument 2 : bool, not used in this function.
///
///  RETURNS: 
///  int, selection priority number of the data block
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CLeader::GetPriority(QPoint point,bool )
{
	//If the mouse pointer is over the leader line rect, returns the prio defined
	//in the track definition (CViewTrack::m_pTrackDef)
	if (m_BoundingRect.contains(point))
		return m_pDef->prio;
	else
		return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selection status of the leader line to the one put in
///  argument.
///  Argument : bool, selection status of the leader line to be applied.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLeader::SelectElement(bool Select)
{
	m_Select = Select;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the user performs a click AB over a track
///  leader line.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLeader::OnLButtonDown(UINT nFlags,QPoint pnt)
{
	//No interaction is defined for now
}

//-----------------------------------------------------------------------------
//
/// \class CVelocity
/// This class is used to manage the display of the prediction vector 
/// of one track presentation.
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of the class member parameters
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CVelocity::CVelocity()
{
	m_Type = velocity;
	m_Select = false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CVelocity&, CVelocity object to be initialized
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CVelocity::CVelocity(CVelocity& vel)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
///
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CVelocity::~CVelocity()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Initialization of the CVelocity Object.
///  Argument 1 : LPVOID, inherited track object
///  Argument 2 : CView*, view associated to the track presentation
///  Argument 3 : CDisplayElement*, inherited CViewTrack object
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CVelocity::Init(LPVOID pData,CView* pView,CDisplayElement* pParent)
{
	m_pView = (CAsdView*)pView;
	m_pTrack = (CTrack*)pData;
	m_pParent = (CViewTrack*)pParent;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time an attribute of the class is modified or
///  may be impacted. The update is performed systematically before refreshing 
///  the display (call of OnDraw function).
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen015, CIAsdFctRwsTppPvs005, CIAsdFctRwsTppPvs010,
///					CIAsdFctRwsTppPvs011, CIAsdFctRwsTppPvs035
//
//-----------------------------------------------------------------------------
void CVelocity::OnUpdate()
{
  //If the track has a speed different from 0
  if (m_pTrack->m_TD.m_RealVelocity.width() != 0 || m_pTrack->m_TD.m_RealVelocity.height() != 0)
    {
		CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
		QSize vect;
		//offset: number of pixels from center of PPS
		int offset=9; 
		int length=m_pView->GetViewParam(CRwdView::pvect_length);
		//the prediction vector is displayed if the PPS is displayed and according to the 
		//Track Control settings. If the track is under an STCA the display is forced
		if ((m_pParent->m_SymbolDisplayed) 
			&& (((m_pTrack->m_TD.m_IsCoupled)	&& (m_pView->GetViewParam(CRwdView::pvect_coupl))) 
			|| ((!m_pTrack->m_TD.m_IsCoupled) && (m_pView->GetViewParam(CRwdView::pvect_uncpl))))) 
		{
      		vect.setWidth((int)(((double)m_pTrack->m_TD.m_RealVelocity.width()*.22*32.0* length/3600.0)+0.5));
			vect.setHeight((int)(((double)m_pTrack->m_TD.m_RealVelocity.height()*.22*32.0* length/3600.0)+0.5));
			m_PosEnd = QPoint(m_pTrack->m_CurPos.x()+vect.width(),m_pTrack->m_CurPos.y()+vect.height());
			m_pView->LPtoDP(&m_PosEnd);
   			double cos=vect.width()/(sqrt(vect.width()*vect.width()+vect.height()*vect.height()));
   			double sin=vect.height()/(sqrt(vect.width()*vect.width()+vect.height()*vect.height()));
   			m_PosInit = m_pTrack->m_CurPos;
			m_pView->LPtoDP(&m_PosInit);
			m_PosInit = QPoint (m_PosInit.x() + (int)(offset*cos+0.5),m_PosInit.y() - (int)(offset*sin+0.5));
			m_BoundingRect = QRect(m_PosInit,m_PosEnd);
      		m_BoundingRect=m_BoundingRect.normalize();
      		if ((m_BoundingRect.height()==0) || (m_BoundingRect.width()==0))
				m_BoundingRect.addCoords(-2,-2,2,2);
		}
		else
			m_BoundingRect=QRect();
    }
  else
    m_BoundingRect=QRect();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to draw or redraw the prediction vector
///  Argument 1 : QPainter*, drawing device
///  Argument 2 : int, not used in this function
///  Argument 3 : int, not used in this function
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CVelocity::OnDraw(QPainter* pDC,int )
{
  	if (m_BoundingRect.isEmpty())
  		return;
	
	QColor curcolor;
	QSize vect;
	QPoint PosDot;
	//Gets the color
	if (!m_Select)
		if (m_pDef->colornb==-1)
			curcolor = m_pParent->m_Color.Color;
		else
			curcolor = m_pParent->ComputeColor(m_pDef->colornb).Color;
	else
		curcolor = m_pParent->ComputeColor(TrkSymbSel).Color;
	QPen pen(curcolor,0,Qt::SolidLine);
	pDC->setPen(pen);
	pDC->drawLine(m_PosInit,m_PosEnd);
	QBrush brush(curcolor);
	pDC->setBrush(brush);
	int length=m_pView->GetViewParam(CRwdView::pvect_length);
	if (m_pView->GetViewParam(CRwdView::pvect_time))
	{
		for (int i=60;i<=length;i+=60)
		{
			vect.setWidth((int)(((double)m_pTrack->m_TD.m_RealVelocity.width()*.22*32.0* i/3600.0)+0.5));
			vect.setHeight((int)(((double)m_pTrack->m_TD.m_RealVelocity.height()*.22*32.0* i/3600.0)+0.5));
			PosDot = QPoint(m_pTrack->m_CurPos.x()+vect.width(),m_pTrack->m_CurPos.y()+vect.height());
			m_pView->LPtoDP(&PosDot);
			QRect rect(PosDot,PosDot);
			rect.addCoords(-2,-2,2,2);
			pDC->drawEllipse(rect);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns a priority number according to the position of the
///  mouse pointer and the prediction vector
///  Argument 1 : point, position of the mouse pointer.
///  Argument 2 : bool, not used in this function.
///
///  RETURNS: 
///  int, selection priority number of the data block
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int  CVelocity::GetPriority(QPoint point,bool )
{
	//If the mouse pointer is over the prediction vector rect, returns the prio defined
	//in the track definition (CViewTrack::m_pTrackDef)
	if (m_BoundingRect.contains(point))
		return m_pDef->prio;
	else
		return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selected status of the prediction vector to the one put in
///  argument.
///  Argument : bool, selected status of the prediction vector to be applied.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CVelocity::SelectElement(bool Select)
{
	m_Select = Select;
}

//-----------------------------------------------------------------------------
//
/// \class CHalo
/// This class is used to manage the display of the halo of one track presentation.
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is the CHalo class constructor
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CHalo::CHalo()
{
	m_Type = halo;
	m_Select = false;
	m_Forced = false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is the CHalo class copy constructor
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CHalo::CHalo(CHalo& halo)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is the CHalo class destructor
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CHalo::~CHalo()
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is the CHalo class initialization function
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CHalo::Init(LPVOID pData,CView* pView,CDisplayElement* pParent)
{
	m_pView = (CAsdView*)pView;
	m_pTrack = (CTrack*)pData;
	m_pParent = (CViewTrack*)pParent;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update the attributes and position of the halo 
///  track element
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CHalo::OnUpdate()
{
	int length=m_pView->GetViewParam(CRwdView::halo_length);
	if ((m_Forced) && (m_pParent->m_SymbolDisplayed))
	{
		m_BoundingRect = QRect(m_pParent->m_CurPos,m_pParent->m_CurPos);
		QSize size = QSize(32*length,32*length);
		m_pView->LPtoDP(&size);
		m_BoundingRect.addCoords(-size.width(),-size.height(),size.width(),size.height());
		m_BoundingRect=m_BoundingRect.normalize();
	}
	else
		m_BoundingRect=QRect();
	if (m_pDef->colornb==-1)
		m_Color = m_pParent->m_Color;
	else
		m_Color = m_pParent->ComputeColor(m_pDef->colornb);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to display the halo track element
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CHalo::OnDraw(QPainter* pDC,int )
{
	if (m_BoundingRect.isEmpty())
		return;

	QColor curcolor;
	if (m_Select)
		curcolor=m_pParent->ComputeColor(TrkLabSel).Color;
	else
		curcolor=m_Color.Color;
	QPen pen(curcolor,0,Qt::SolidLine);
	pDC->setPen(pen);
	pDC->drawArc(m_BoundingRect,0,360*16);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to set the selection status of the halo element
///
///  RETURNS: 
///  the priority of the halo track element if hooked, 0 otherwise
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int  CHalo::GetPriority(QPoint point,bool )
{
	QPoint vect;
	int dist;
	int length=m_pView->GetViewParam(CRwdView::halo_length);
	if (m_BoundingRect.contains(point))
	{
		vect = point-m_pParent->m_CurPos;
		dist = (int)sqrt(vect.x()*vect.x()+vect.y()*vect.y());
		QSize size(32*length,32*length);
		m_pView->LPtoDP(&size);
		if (abs(dist-size.width())<=5)
			return m_pDef->prio;
		else
			return 0;
	}
	else
		return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selection status of the track to the boolean value 
///  received in argument
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CHalo::SelectElement(bool Select)
{
	m_Select = Select;
}


//--------------------------------< THE END >----------------------------------

