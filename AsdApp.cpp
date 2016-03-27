/*
 * Copyright (C) 2013 SkySoft-ATM 
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
//  FILENAME: AsdApp.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file retrieves the parameters offline defined in the adaptation dataset.
//		This file instanciates the tracks, Fpl, coupling, local tags, safety nets, weather 
// 
//  COMMENTS
//
//
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 2  2013/12/29 dalleins
//
//
//
// 
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------
#include "StdAfx.h"
#include <stdlib.h>
#include "Asterix.h"
#include "AsdApp.h"
#include "WdwMngr.h"
#include "FontMngr.h"
#include "DataMngr.h"
#include "ColorMngr.h"
#include "View.h"
#include "DisplayElement.h"
#include "AsdView.h"
#include "AsdMenu.h"
#include "TrackControl.h"
#include "RwdView.h"
#include "Track.h"
#include "FlightPlan.h"
#include "InfoMngr.h"
#include "CouplingMngr.h"
#include "SetupMngr.h"
#include "Elements.h"
#include "Geo.h"
#include "VisuMap.h"
#include "Zooming.xpm"
#include "NormalSelect.xpm"
#include "Crosshair.xpm"
#include "qsplashscreen.h"
#include "UserMngr.h"
#include "WinHints.h"
#include "SystemMode.h"
#include "Interface.h"
#include "UserMngr.h"
#include "WfAttrib.h"
#include "SafetyEventHandler.h"
#include "LineView.h"

#if (defined Q_WS_X11)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <unistd.h>
#endif
#undef KeyPress


//-GLOBAL DATA-----------------------------------------------------------------
CAsdApp* CurApp=NULL;


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CAsdApp
/// This class is used to create the object the ASD needs to and to retrieve
///	information from the adaptation data set files
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
/* CDS BREAK CAsdApp is the application object, it's unique and then there's no need 
	to implement the copy constructor for this class */
CAsdApp::CAsdApp ( int & argc, char ** argv )
: QApplication(argc,argv), m_pMode(NULL), m_InitTimerId(-1)
{
	m_asterixdbg = 0;
	m_Connectionless = false;
	m_pTrackMngr = NULL;
	m_pFplMngr = NULL;
	m_pInfoMngr = NULL;
	m_pCouplingMngr = NULL;
	m_pWeatherMngr = NULL;
	m_pUserMngr = NULL;
	m_pAttribute = NULL;
	m_pWinHints = NULL;
	m_pWRecordResize = NULL;
	m_pEventMngr = NULL;

	m_StartTime = QDateTime::currentDateTime();
	m_OfflineOk=true;
	m_pUserMngr=NULL;
	CAsyncSocket::InitSocket();
	char str[132];
	gethostname(str,132);
	m_HostName=QString(str);
	m_LogDir=".";
	m_TimerId=0;
	CurApp=this;
	m_ColorSet=false;
	m_IsErrorDialogDisplayed = false;
	m_StartXPaint = false;
	m_sViewerVersion="";

	// Initialize the Top Level Menu and the Time window for each screen
	QDesktopWidget* pDesktop=QApplication::desktop();
	m_TopLevelTable.SetSize(pDesktop->numScreens());
	m_TimeWndTable.SetSize(pDesktop->numScreens());
	
	for (int i=0;i<pDesktop->numScreens();i++)
    {
		m_TopLevelTable[i] = NULL;
		m_TimeWndTable[i] = NULL;
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
CAsdApp::~CAsdApp()
{
	if (m_pMode)
		delete []m_pMode;

	if (m_pTrackMngr)
		delete []m_pTrackMngr;

	if (m_pFplMngr)
		delete []m_pFplMngr;

	if (m_pInfoMngr)
		delete []m_pInfoMngr;

	if (m_pCouplingMngr)
		delete []m_pCouplingMngr;

	if (m_pUserMngr)
		delete []m_pUserMngr;

	if (m_pWinHints)
		delete m_pWinHints;

	if (m_pWRecordResize)
	  delete m_pWRecordResize;
	
	killTimer(m_TimerFillList);
		
	CVisuMap::RemoveTables();
	CFontMngr::RemoveTables();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method check if the adaptation data set exists
///		and retrieves the <version>.ini file
/// 
///  RETURNS: 
///		boolean in order to know if the extraction has been performed correctly
///                    
///  REQUIREMENT ID: NONE                               
//-----------------------------------------------------------------------------
bool CAsdApp::ComputeDataSetPath()
{
	m_DefaultOfflinePath=QDir::currentDirPath();
	QDir dirFilename=QDir(m_DefaultOfflinePath);
	QString dataset=dirFilename.entryList("dataset-*").last();
	
	if (dataset.length())
	{
		m_DefaultOfflinePath+="/"+dataset;
		
		if (m_DefaultOfflinePath.endsWith("/"))
			m_DefaultOfflinePath.truncate(m_ApplicationIniName.length()-1);
		
		int fnd=m_DefaultOfflinePath.findRev("-");
		QString ver=m_DefaultOfflinePath.right(m_DefaultOfflinePath.length()-fnd-1);
		m_DataSetPath=m_DefaultOfflinePath;	
		m_DefaultOfflinePath+="/"+ver;
		
		// m_ApplicationIniName is the <version>.ini and file the current path 
		// from where the application is launched
		m_ApplicationIniName =m_DefaultOfflinePath+".ini";
		m_DefaultOfflinePath+="_";
		
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method extract the following parameters from the 
///		<version>.ini file of the adaptation data set:
///				- the radar and weather service parameters,
///				- the time after which the message is discarded if no reception 
///				  radar or weather is received,
///				- the Range marker parameters,
///				- time of framing for windows,
///				- time after which a window is automatically closed 
///				  for windows having the auto-close behavior
/// 
///  RETURNS: 
///		void                 
///                  
///  REQUIREMENT ID: CIAsdItfHwi005, CIAsdItfIrsIoa010 (one port per CSCI or CCS Services) 
///					 CIAsdItfComIda005, CIAsdItfComIpa085, 
///					 CIAsdItfComIpa090 (one port for each CCS services),
///					 CIAsdItfComIra015, CIAsdItfComIba010, CIAsdItfComIta020,
///					 CIAsdItfComGen105, CIAsdFctIniGen020, CIAsdFctIniGen030,
///					 CIAsdFctIniGen150, CIAsdFctIniGen151, CIAsdFctIniAds006,
///					 CIAsdFctIniAds007
//-----------------------------------------------------------------------------
// CDS BREAK JPM 19/05/2006 Bad method lenght rule violated. The code has been 
//							factorized as much as possible and can't be splitted
//							because if an error occurs while reading the dataset
//							the ASD has to stop.
//				
void CAsdApp::ReadVersionIni()
{
	QString IniName=GetApplicationIni();
	CIniFile IniFile(IniName);

	QString LogDir;
	QString Error;
	int len=IniFile.GetIniProfileString("DIRECTORIES","LOG",LogDir);
	
	// Checks if the offline define path log directories exist
	if (!len)
	{
		Error="No LOG found in version.ini [DIRECTORIES]. Stopping ASD.";
		WriteLogMsg(Error, LogError); 
		m_OfflineOk=false;
		return ;
	}

	// Checks the offline define path to retrieve and save the custom sets exists
	len=IniFile.GetIniProfileString("DIRECTORIES","CUSTOM_SETS",m_CustomDir);
	if (!len)
	{
		Error="No CUSTOM_SETS found in version.ini [DIRECTORIES]. Stopping ASD.";
		WriteLogMsg(Error, LogError); 
		m_OfflineOk=false;
		return ;
	}
	
	m_asterixdbg = 0;
	QString asterdbg;
	len=IniFile.GetIniProfileString("GENERAL","ASTERIX_DBG",asterdbg);
	if (len)
	{
		m_asterixdbg=atoi(asterdbg.ascii());
	}


	if (LogDir.length())
	{
		if (LogDir[0].latin1()=='.')
		{
			QDir dir=QDir(QDir::current());
			m_LogDir=dir.path()+LogDir.right(LogDir.length()-1);
		}
		else
		{
			m_LogDir=LogDir;
		}
	}

	// Checks if the general interfaces exists in <version>.ini file
	m_OfflineOk = m_Interfaces.Read(IniFile, Error);
	if (!m_OfflineOk) 
	{ 
		WriteLogMsg(Error, LogError); 
		return; 
	}


	// Checks if the Radar interfaces exists in <version>.ini file
	m_OfflineOk = m_pTrackMngr->Read(IniFile, m_Interfaces, Error);
	if (!m_OfflineOk) 
	{ 
		WriteLogMsg(Error, LogError); 
		return; 
	}

	// Checks if the Weather interfaces exists in <version>.ini file
	m_OfflineOk = m_pWeatherMngr->Read(IniFile, m_Interfaces, Error);
	if (!m_OfflineOk) 
	{ 
		WriteLogMsg(Error, LogError); 
		return; 
	}


	m_OfflineOk = m_pInfoMngr->Read(IniFile, Error);
	if (!m_OfflineOk) 
	{ 
		WriteLogMsg(Error, LogError); 
		return; 
	}

	// Checks if the System mode names exists in <version>.ini file
	m_OfflineOk = m_pMode->Read(IniFile, Error);
	if (!m_OfflineOk) 
	{ 
		WriteLogMsg(Error, LogError); 
		return; 
	}

	QStringList PortNumberList;
	CString KeyRead,CurArg,KeyName;



	// retrieves the time to discard a radar message from the <version>.ini file
	// if it exists otherwise the ASD logs an error and stops
	QString strTemp;
	len=IniFile.GetIniProfileString("TIMES","RADAR_DISCARD",strTemp);
	if(!len)
	{
		Error="No RADAR_DISCARD found in version.ini [TIMES]. Stopping ASD.";
		WriteLogMsg(Error, LogError); 
		m_OfflineOk=false;
		return ;
	}
	m_RadarDiscard=strTemp.toInt();
	strTemp=QString::null;

	// retrieves the time for automatic closure of windows having the auto-close behavior
	// from the <version>.ini file if it exists otherwise the ASD logs an error and stops
	len=IniFile.GetIniProfileString("TIMES","AUTO_CLOSE",strTemp);
	if(!len)
	{
		Error="No AUTO_CLOSE found in version.ini [TIMES]. Stopping ASD.";
		WriteLogMsg(Error, LogError); 
		m_OfflineOk=false;
		return ;
	}
	m_AutoCloseTime=strTemp.toInt()*1000;
	strTemp=QString::null;

	// retrieves the time to discard a weather radar message from the <version>.ini file
	// if it exists otherwise the ASD logs an error and stops
	len=IniFile.GetIniProfileString("TIMES","WEATHER_DISCARD",strTemp);
	if(!len)
	{
		Error="No WEATHER_DISCARD found in version.ini [TIMES]. Stopping ASD.";
		WriteLogMsg(Error, LogError);
		m_OfflineOk=false;
		return ;
	}
	m_WeatherDiscard=strTemp.toInt()*1000;
	strTemp=QString::null;

	// retrieves the range marker parameters from the <version>.ini file
	// if it exists otherwise the ASD logs an error and stops
	len=IniFile.GetIniProfileString("RANGE_MARKERS","H_OFF_BL",strTemp);
	if(!len)
	{
		Error="No H_OFF_BL found in version.ini [RANGE_MARKERS]. Stopping ASD.";
		WriteLogMsg(Error, LogError); 
		m_OfflineOk=false;
		return ;
	}
	m_iOffsetHBL=strTemp.toInt();
	strTemp=QString::null;
	len=IniFile.GetIniProfileString("RANGE_MARKERS","V_OFF_BL",strTemp);
	if(!len)
	{
		Error="No V_OFF_BL found in version.ini [RANGE_MARKERS]. Stopping ASD.";
		WriteLogMsg(Error, LogError); 
		m_OfflineOk=false;
		return ;
	}
	m_iOffsetVBL=strTemp.toInt();
	strTemp=QString::null;
	len=IniFile.GetIniProfileString("RANGE_MARKERS","H_OFF_TR",strTemp);
	if(!len)
	{
		Error="No H_OFF_TR found in version.ini [RANGE_MARKERS]. Stopping ASD.";
		WriteLogMsg(Error, LogError); 
		m_OfflineOk=false;
		return ;
	}
	m_iOffsetHTR=strTemp.toInt();
	strTemp=QString::null;
	len=IniFile.GetIniProfileString("RANGE_MARKERS","V_OFF_TR",strTemp);
	if(!len)
	{
		Error="No V_OFF_TR found in version.ini [RANGE_MARKERS]. Stopping ASD.";
		WriteLogMsg(Error, LogError); 
		m_OfflineOk=false;
		return ;
	}
	m_iOffsetVTR=strTemp.toInt();

	// retrieves the time of framing of a window from the <version>.ini file
	// if it exists otherwise the ASD logs an error and stops
	len=IniFile.GetIniProfileString("TIMES", "FRAMING", strTemp);
	if(!len)
	{
		Error="No FRAMING found in version.ini [RANGE_MARKERS]. Stopping ASD.";
		WriteLogMsg(Error, LogError); 
		m_OfflineOk=false;
		return ;
	}
	m_FramingTimer=strTemp.toInt();
	strTemp=QString::null;

	// Read the data set for Aircraft type/Wake turbulence category, 
	// Discrete SSR code, default windows opens from the top level menu (Tlm)
	m_ATypWtc.ReadDataSet(IniFile);	
	m_AdesList.ReadDataSet(IniFile);
	m_DiscreteSsr.ReadDataSet(IniFile);
	m_DefTlmPosList.ReadDataSet(IniFile);
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the classes TrackMngr, Weather, Fpl, Coupling, Local Tag, 
///		Safetynets, infoMngr and the sequence list
///
///  RETURNS: 
///		void
///   
///  REQUIREMENT ID: NONE                            
//-----------------------------------------------------------------------------
void CAsdApp::CreateInstances()
{
	m_pMode = new CSystemMode[1];
	m_pTrackMngr=new CTrackMngr[1];
	m_pFplMngr=new CFplMngr[1];
	m_pInfoMngr= new CInfoMngr[1];
	m_pCouplingMngr = new CCouplingMngr[1];
	m_pWeatherMngr = new CWeatherMngr[1];
	m_pUserMngr=new CUserMngr[1];
	m_pEventMngr = new CEventMngr[1];
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the classes TrackMngr, Weather, Fpl, Coupling, Local Tag, 
///		Safetynets, infoMngr and the sequence list
///
///  RETURNS: 
///		void
///   
///  REQUIREMENT ID: NONE                            
//-----------------------------------------------------------------------------
void CAsdApp::SetConnections()
{
	if (!m_Connectionless)
	{
		m_pTrackMngr->Init(m_pEventMngr, 2.5);
		m_pWeatherMngr->Init();
		m_pFplMngr->Init(m_pEventMngr);
		m_pCouplingMngr->Init(m_pEventMngr, 2.5, 2.5);
		int i = 0;
		
		for (i='A'; i <= 'Z'; i++)
		{
			CTrackServer* pts = GetTrackMngr()->GetServer((char)i);
			if (pts && !m_pCouplingMngr->m_CouplingList.IsAllowed(pts->m_Sic, pts->m_Sac))
				m_pCouplingMngr->m_CouplingList.Allow(pts->m_Sic, pts->m_Sac);
		}
		
		m_pInfoMngr->Init(m_pEventMngr);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the mouse pointer of radar windows
///
///  RETURNS: 
///		void
///   
///  REQUIREMENT ID: NONE                            
//-----------------------------------------------------------------------------
void CAsdApp::SetRadarCursors()
{
	QPixmap pix1((const char**) NormalSelect);
	m_CursorTable[MPNormalSelect]=QCursor(pix1,2,4);// if those values
	// are changed the values in CMenuWnd::TrackPopupMenu should also
	//be changed to display the menu under the hot spot of the mouse 
	//pointer.
	QPixmap pix2((const char**) Zooming);
	m_CursorTable[MPZooming]=QCursor(pix2,16,16);
	
	QPixmap pix3((const char**) Crosshair);
	m_CursorTable[MPCrosshair]=QCursor(pix3,16,15);
	m_CursorTable[MPPanning]=QCursor(BlankCursor);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method looks for the adaptation data set if exists
///		Then it creates the track, flight plan object but also the Coupling, 
///		and Weather
///		This method creates the Main Radar window as well as the Top Level
///		Menu and the Time window and then apply the default setup according
///		to the sectorization
///
///  RETURNS: 
///		1 if the Init has been successfully performed
///		otherwise it returns 0.
///    
///  REQUIREMENT ID: CIAsdFctCzs025, CIAsdFctIniGen140, CIAsdFctIniAds025,
///					 CIAsdFctIniAds030, CIAsdFctIniEll003, CIAsdFctIniEll005,
///					 CIAsdFctIniEll010, CIAsdFctRcgStp005, CIAsdFctRcgRol010,
///					 CIAsdFctCzs005
//                            
//-----------------------------------------------------------------------------
int CAsdApp::Init(void) 
{
	// CDS BREAK NGT 08/06/2006 Bad method length rule is not violated considering
	//							only the real length of the function.
	CSKMessageBox::Init();

	CreateInstances();
	if (ComputeDataSetPath())
	{
		QFile file(m_ApplicationIniName);
		if (!file.open(IO_ReadOnly|IO_Translate))
		{           
			WriteLogMsg("File "+m_ApplicationIniName+" does not exist",LogError);
			m_OfflineOk=false;
		}
		else
		{	
			file.close();
#if (defined Q_WS_X11)
			// only one asd can be launched at a time on a CWP position
			QString sFileName="/tmp/tmp.tmp";
			QString sCmd="ps -ef|grep albatross-display|grep -vc grep > "+sFileName;
			system(sCmd);
			QFile sTmpFile(sFileName);
			if (!sTmpFile.open(IO_ReadOnly|IO_Translate))
			{
				WriteLogMsg("Impossible to create a temporary file, application will stop",LogError);
				return 0;
			}
			else
			{
				QString sElement;
				sTmpFile.readLine(sElement,1024);
				if ((sElement!="0\n") && (sElement!="1\n"))
				{
					WriteLogMsg("An Albatross Display application is already running on this machine. Application will stop",LogError);
					qDebug ("An Albatross Display application is already running on this machine. Application will stop");
					return 0;
				}
			}
#endif
	
			ReadVersionIni();
			if (!m_OfflineOk)
				return 0;

			if (!m_Connectionless)
			{
				QString olddataset=m_DefaultOfflinePath;
				if (!ComputeDataSetPath())
				{
					CSKMessageBox::critical( 0, "Error Message","ASD startup: No dataset directory has been found\nApplication will be stopped");
					WriteLogMsg("No dataset directory has been found",LogError);
					m_OfflineOk=false;
					return 0;
				}
				if  (olddataset!=m_DefaultOfflinePath)
				{
					m_OfflineOk=true;
					QFile file(m_ApplicationIniName);
					if (!file.open(IO_ReadOnly|IO_Translate))
					{
						CSKMessageBox::critical( 0, "Error Message","ASD startup: No ini file has been found\nApplication will be stopped");
						WriteLogMsg("File "+m_ApplicationIniName+" does not exist",LogError);
						m_OfflineOk=false;
					}
					else
					{
						file.close();
						ReadVersionIni();
					}
				}
			}
		}
	}
	else
	{
		CSKMessageBox::critical( 0, "Error Message","ASD startup: No dataset directory has been found\nApplication will be stopped");
		WriteLogMsg("No dataset directory has been found",LogError);
		m_OfflineOk=false;
	}

	if (!m_OfflineOk)
		return 0;

	// Performs the initialization of object in CAsdApp class
	CColorMngr::InitTables();
	CFontMngr::InitTables();
	CWdwMngr::InitTables();
	CGeo::InitCenterInIni(m_ApplicationIniName);
	CGeo::LoadPoints(m_DefaultOfflinePath);
	SetViewerVersion();

	m_pAttribute=new CWFAttrib(GetDataSetDir(),true);

	m_pWinHints= new CWin_Hints();
	m_pWinHints->CleanAll();
	CDataMngr::InitTables();
	SetColors();
	SetConnections();	
	m_pUserMngr->Init();
	m_pMode->Init(m_pEventMngr);

	QString reason;
	if (!CGraphicSymbol::InitTables(GetDefaultOfflinePath(), reason))
	{
		WriteLogMsg(reason,LogError);
		m_OfflineOk=false;
	}

	CVisuMap::InitTable();
	CViewTrack::InitTables();
	SetRadarCursors();
	
	// Set the button decoration for each ASD window via FVWM commands
	CWdwMngr::SetWindowAttributes();
	
	// Create the main radar window
	CRwdView* view=new CRwdView[1];
	view->OnInitialUpdate();
	view->setCaption("Main Radar");
	CWdwMngr::AddWindow(view,"MAIN_RDR", CWFAttrib::MAIN_RDR, false);
	setMainWidget(view);
	QDesktopWidget* pDesktop=QApplication::desktop();
	CTimeWnd::InitTables();

	if (!m_OfflineOk)
		return 0;

	m_pTrackMngr->SetSourcePriority();

	int i = 0;
	// Create the Top Level Menu and Time window for each screen
	for (i=0;i<pDesktop->numScreens();i++)
	{
		QRect rect=pDesktop->screenGeometry(i);
		int leftpos=rect.left();
		rect.setBottom(rect.top()+30);
		CTopLevel* pTopLevel=new CTopLevel(m_pEventMngr, rect);
		m_TopLevelTable[i] = pTopLevel;
		pTopLevel->SetTrackSourceMenu(m_pTrackMngr->GetTrackMenu());
		CWdwMngr::AddWindow(pTopLevel,"TOPLEVEL",-1, false);
		CTimeWnd* pTimeWnd=new CTimeWnd[1];
		m_TimeWndTable[i] = pTimeWnd;
		pTimeWnd->move(rect.left()+25,rect.top());
		rect=pTimeWnd->geometry();
		CWdwMngr::AddWindow(pTimeWnd,"TIME",-1, false);
		pTopLevel->SetOffsetX(rect.right()-leftpos+10);
		pTopLevel->SetHeight(rect.height());
		pTopLevel->move(leftpos,rect.top());
	}

	ShowTopLevel();
	setGlobalMouseTracking(true);
   	connect( this, SIGNAL(lastWindowClosed()), this, SLOT(quit()) );
	
	
	// Load the default setup according to the group, subgroup, location role
	CSetupMngr::SetDefaultSetup();
	m_InitTimerId=startTimer(1);
	m_TimerFillList=startTimer(4000);
	QString strMsg="Asd Application has Started";
	WriteLogMsg(strMsg,LogInformation);
	
	return 1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called to make the Top Level Menu and Time 
///		window visible on each screen
///
///  RETURNS: 
///		void
/// 
///  REQUIREMENT ID: NONE                              
//-----------------------------------------------------------------------------
void CAsdApp::ShowTopLevel()
{
	QDesktopWidget* pDesktop=QApplication::desktop();
	
	for (int i=0;i<pDesktop->numScreens();i++)
	{
		CWdwMngr::ShowWindow(m_TopLevelTable[i], -1);
		CWdwMngr::ShowWindow(m_TimeWndTable[i], -1);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method catch the event (e) associated to the QT object 
///		(receiver). The Event can be keybord or mouse events appplied to a window
///		or an element of a window.
///		
///  RETURNS: 
///		true if the event associated to the QT object has to be taken
///		into account in the application
/// 
///  REQUIREMENT ID: NONE                               
//-----------------------------------------------------------------------------
bool CAsdApp::notify ( QObject * receiver, QEvent * e )
{
 	if ((e->type()==QEvent::ContextMenu) && (receiver->isWidgetType()))
	{
		return true;
	}
	
	// this is to modify the behavior of the List box in order not to use arrow keys
	if (QEvent::KeyPress==e->type() && (receiver->isWidgetType()))
    {
        int Key=((QKeyEvent*) e)->key();

        if ((Key==Qt::Key_Up) || (Key==Qt::Key_Down) ||
            (Key==Qt::Key_Left) || (Key==Qt::Key_Right))
        {
            QWidget* w=(QWidget *) receiver;
            if (w->isA("QListBox"))
                return true;
        }
    }

	//This is to modify the behavior of the text edit field to
    //avoid pasting the buffer and instead close the window
	//Also to close windows with CW on widgets
	if ((e->type()==QEvent::MouseButtonPress)&&(receiver->isWidgetType()))
	{
		if (((QMouseEvent *)e)->button()==MidButton)
        {
			QWidget* w=(QWidget *) receiver;
			if ((w->isA("QLineEdit")) || (w->isA("QViewportWidget")) || (w->isA("QScrollBar")) ||
				(w->isA("QPushButton")) || (w->isA("QCheckBox")))
                	CWdwMngr::HideWindow(w->topLevelWidget());
        }

		if (((QMouseEvent *)e)->button()==RightButton)
        {
			QWidget* w=(QWidget *) receiver;
			if ((w->isA("QViewportWidget") || w->isA("QListBox")))
	            return true;
		}

		if (((QMouseEvent *)e)->button()==LeftButton)
        {
			QWidget* w=(QWidget *) receiver;
			if (w->isA("QListBox"))
                        return true;
		}
	}

	bool res=QApplication::notify(receiver,e);

	if (e->type() == QEvent::WindowStateChange)
	{
		CWdwMngr::CollapseUncollapseWindow(receiver);
		
#ifdef ENABLE_QDEBUG
		qDebug("WindowStateChange");
#endif
	}

	if (receiver!=CRwdView::m_psMainWnd)
	{
	    if (QEvent::KeyPress==e->type())
		{
#ifdef ENABLE_QDEBUG
			qDebug("key press send to main");
#endif

			int iKey=((QKeyEvent*) e)->key();		

			if ((iKey==Qt::Key_F1)||(iKey==Qt::Key_F2)||
				(iKey==Qt::Key_F3)||(iKey==Qt::Key_F4)||
			    (iKey==Qt::Key_F5)||(iKey==Qt::Key_F6)||
			    (iKey==Qt::Key_F7)||(iKey==Qt::Key_F8)||
			    (iKey==Qt::Key_F9)||(iKey==Qt::Key_F10)||
				(iKey==Qt::Key_Escape))
				CRwdView::m_psMainWnd->PressEvent((QKeyEvent*) e);
			
			res=false;
	    }
	}

 	if ((e->type()==QEvent::MouseMove) && (receiver->isWidgetType()))
	{
	}

	if ((e->type()==QEvent::Move) || (e->type()==QEvent::WindowActivate))
	{
		killTimer(m_TimerId);
		m_TimerId=startTimer(100);

#if (defined Q_WS_X11)
		if (receiver->isWidgetType())
			CWdwMngr::MoveWindowInside((QWidget*) receiver);
#endif
	}

	if ((e->type()==QEvent::Resize)&&(receiver->isWidgetType()))
	{
		killTimer(m_TimerId);
		m_TimerId=startTimer(100);
		killTimer(m_iResizeTimer);
		m_iResizeTimer=startTimer(1000);
		m_pWRecordResize=(QWidget*) receiver;
	}

	return res;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the default set up according to
///		the sectorization. It can be the default customization set from the 
///		DefaultCustomSet.ini file or the last loaded custom set.
///		
///  RETURNS: 
///		void
///                  
///  REQUIREMENT ID: NONE             
//-----------------------------------------------------------------------------
void CAsdApp::ApplyDefaultSetup( )
{
	ShowTopLevel();
	CSetupMngr::SetDefaultSetup();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method catches all the timer event for the CAsdApp class.
///		This method catches:
///			- the end of initialization phase, 
///			- the end of reconfiguration phase, 
///			- the resize of a window,
///			- the move of a window to order the windows by priority
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE                              
//-----------------------------------------------------------------------------
void CAsdApp::timerEvent( QTimerEvent *e )
{
	if (m_InitTimerId==e->timerId())
    {
        ApplyDefaultSetup();

        killTimer(m_InitTimerId);
        m_InitTimerId=0;
    }

	if (m_ReconfigTimerId==e->timerId())
	{
		killTimer(m_ReconfigTimerId);
        m_ReconfigTimerId=0;
		CWdwMngr::SetWindowAttributes();
	}

	if (m_TimerId==e->timerId())
	{
		killTimer(m_TimerId);
		m_TimerId=0;
		CWdwMngr::OrderWindows();
	}

	if ((m_iResizeTimer==e->timerId())&& (m_pWRecordResize))
	{
#ifdef ENABLE_QDEBUG
		qDebug("ResizeTimerEvent");
#endif

		killTimer(m_iResizeTimer);
		m_iResizeTimer=0;
		CWdwMngr::MoveWindowInside(m_pWRecordResize);
		CWdwMngr::MoveWindow();
		m_pWRecordResize=NULL;
	}

	if (m_TimerFillList==e->timerId())
	{
		CTopLevel::UpdateSystemInfo();

		if (GetSystemMode()->GetSystemMode() == NORMAL)
			m_pFplMngr->FillFPLLists();

	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the object of class CSystemMode
///
///  RETURNS: 
///		the object of class CSystemMode
///  
///  REQUIREMENT ID: NONE                              
//-----------------------------------------------------------------------------
CSystemMode* CAsdApp::GetSystemMode()
{
	return m_pMode;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the object of class CTrackMngr
///
///  RETURNS: 
///		the object of class CTrackMngr
///     
///  REQUIREMENT ID: NONE                           
//-----------------------------------------------------------------------------
CTrackMngr* CAsdApp::GetTrackMngr()
{
	return m_pTrackMngr;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the object of class CFplMngr
///
///  RETURNS: 
///		the object of class CFplMngr
///   
///  REQUIREMENT ID: NONE                            
//-----------------------------------------------------------------------------
CFplMngr* CAsdApp::GetFplMngr()
{
	return m_pFplMngr;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the object of class CInfoMngr
///
///  RETURNS: 
///		the object of class CInfoMngr
///  
///  REQUIREMENT ID: NONE                             
//-----------------------------------------------------------------------------
CInfoMngr* CAsdApp::GetInfoMngr()
{
	return m_pInfoMngr;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the object of class CCouplingMngr
///
///  RETURNS: 
///		the object of class CCouplingMngr
///   
///  REQUIREMENT ID: NONE                             
//-----------------------------------------------------------------------------
CCouplingMngr* CAsdApp::GetCouplingMngr()
{
	return m_pCouplingMngr;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the object of class CWeatherMngr
///
///  RETURNS: 
///		the object of class CWeatherMngr
///  
///  REQUIREMENT ID: NONE                              
//-----------------------------------------------------------------------------
CWeatherMngr* CAsdApp::GetWeatherMngr()
{
	return m_pWeatherMngr;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the object of class CUserMngr
///
///  RETURNS: 
///		the object of class CUserMngr
///   
///  REQUIREMENT ID: NONE                             
//-----------------------------------------------------------------------------
CUserMngr* CAsdApp::GetUserMngr()
{
	return m_pUserMngr;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the object of class CWin_Hints
///
///  RETURNS: 
///		the object of class CWin_Hints
///    
///  REQUIREMENT ID: NONE                           
//-----------------------------------------------------------------------------
CWin_Hints* CAsdApp::GetWinHints()
{
	return m_pWinHints;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the name of the application
///
///  RETURNS: 
///		the name of the application
///   
///  REQUIREMENT ID: NONE                             
//-----------------------------------------------------------------------------
QString CAsdApp::GetApplicationIni()
{
	return m_ApplicationIniName;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the default path where the application is located
///
///  RETURNS: 
///		the default path of the application
///  
///  REQUIREMENT ID: NONE                              
//-----------------------------------------------------------------------------
QString CAsdApp::GetDefaultOfflinePath()
{
	return m_DefaultOfflinePath;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the directory where the adaptation data set is located
///
///  RETURNS: 
///		the directory of the adaptation data set
///   
///  REQUIREMENT ID: NONE                              
//-----------------------------------------------------------------------------
QString CAsdApp::GetDataSetDir()
{
	return m_DataSetPath;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the object of class CAsdApp
///
///  RETURNS: 
///		the object of class CAsdApp
///    
///  REQUIREMENT ID: NONE                             
//-----------------------------------------------------------------------------
CAsdApp* CAsdApp::GetApp()
{
	return CurApp;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the colors of windows that are either 
///		pre-defined or offline defined
///
///  RETURNS: 
///		void
///  
///  REQUIREMENT ID: NONE                             
//-----------------------------------------------------------------------------
void CAsdApp::SetColors()
{
	CWin_Hints* pCWinHints=GetWinHints();
	
	if ((pCWinHints) && (!m_ColorSet))
	{
		pCWinHints->SetTitleColor();
		m_ColorSet=true;
	}

	QPalette pal=palette();
	pal.setColor(QPalette::Inactive,QColorGroup::Background,CColorMngr::GetColor(DefWindowBackClr));
	pal.setColor(QPalette::Active,QColorGroup::Background,CColorMngr::GetColor(DefWindowBackClr));
	pal.setColor(QPalette::Disabled,QColorGroup::Background,CColorMngr::GetColor(DefWindowBackClr));

	pal.setColor(QPalette::Inactive,QColorGroup::Base,CColorMngr::GetColor(DefWindowBackClr));
	pal.setColor(QPalette::Active,QColorGroup::Base,CColorMngr::GetColor(DefWindowBackClr));

	pal.setColor(QPalette::Inactive,QColorGroup::Text,CColorMngr::GetColor(DefWindowTextClr));
	pal.setColor(QPalette::Active,QColorGroup::Text,CColorMngr::GetColor(DefWindowTextClr));
	pal.setColor(QPalette::Disabled,QColorGroup::Text,CColorMngr::GetColor(DefMenuDisTextClr));

	pal.setColor(QPalette::Inactive,QColorGroup::Foreground,CColorMngr::GetColor(DefWindowTextClr));
	pal.setColor(QPalette::Active,QColorGroup::Foreground,CColorMngr::GetColor(DefWindowTextClr));

	pal.setColor(QPalette::Inactive,QColorGroup::Button,CColorMngr::GetColor(DefWindowBackClr));
	pal.setColor(QPalette::Active,QColorGroup::Button,CColorMngr::GetColor(DefWindowBackClr));
	pal.setColor(QPalette::Disabled,QColorGroup::Button,CColorMngr::GetColor(DefWindowBackClr));

	pal.setColor(QPalette::Inactive,QColorGroup::ButtonText,CColorMngr::GetColor(DefWindowTextClr));
	pal.setColor(QPalette::Active,QColorGroup::ButtonText,CColorMngr::GetColor(DefWindowTextClr));
	pal.setColor(QPalette::Disabled,QColorGroup::ButtonText,CColorMngr::GetColor(DefWindowTextClr));

	//applies to the outbound bottom rigth border for check box
	pal.setColor(QPalette::Inactive,QColorGroup::Light,CColorMngr::GetColor(DefWindowLeftTopClr));
	pal.setColor(QPalette::Active,QColorGroup::Light,CColorMngr::GetColor(DefWindowLeftTopClr));
	pal.setColor(QPalette::Disabled,QColorGroup::Light,CColorMngr::GetColor(DefWindowLeftTopClr));

	//applies to the inbound bottom rigth border for check box
	pal.setColor(QPalette::Inactive,QColorGroup::Midlight,CColorMngr::GetColor(DefWindowLeftTopClr));
	pal.setColor(QPalette::Active,QColorGroup::Midlight,CColorMngr::GetColor(DefWindowLeftTopClr));
	pal.setColor(QPalette::Disabled,QColorGroup::Midlight,CColorMngr::GetColor(DefWindowRightBottomClr));//CColorMngr::GetColorNb("WND_MAIN_AREA_BKGRND")));
	
	//applies to the inbound top left border for check box
	pal.setColor(QPalette::Disabled,QColorGroup::Shadow,CColorMngr::GetColor(DefWindowRightBottomClr));


	//applies to the outbound top left border for check box
	pal.setColor(QPalette::Inactive,QColorGroup::Dark,CColorMngr::GetColor(DefWindowRightBottomClr));
	pal.setColor(QPalette::Active,QColorGroup::Dark,CColorMngr::GetColor(DefWindowRightBottomClr));
	pal.setColor(QPalette::Disabled,QColorGroup::Dark,CColorMngr::GetColor(DefWindowLeftTopClr));
	
	pal.setColor(QPalette::Inactive,QColorGroup::Mid,CColorMngr::GetColor(DefWindowRightBottomClr));
	pal.setColor(QPalette::Active,QColorGroup::Mid,CColorMngr::GetColor(DefWindowRightBottomClr));

	pal.setColor(QPalette::Inactive,QColorGroup::Highlight,CColorMngr::GetColor(CColorMngr::GetColorNb("REVERSE_VIDEO_BKGRND")));
	pal.setColor(QPalette::Active,QColorGroup::Highlight,CColorMngr::GetColor(CColorMngr::GetColorNb("REVERSE_VIDEO_BKGRND")));

	pal.setColor(QPalette::Inactive,QColorGroup::HighlightedText,CColorMngr::GetColor(CColorMngr::GetColorNb("REVERSE_VIDEO_FRGRND")));
	pal.setColor(QPalette::Active,QColorGroup::HighlightedText,CColorMngr::GetColor(CColorMngr::GetColorNb("REVERSE_VIDEO_FRGRND")));
	setPalette(pal,true);

	QPalette palCheckBox=pal;
	palCheckBox.setColor(QColorGroup::Base,CColorMngr::GetColor(CColorMngr::GetColorNb("TOPLEVEL_MENU_BKGRND")));
	CTopLevel::SetCheckBoxPalette(palCheckBox);


	CTopLevel::UpdateSystemInfo();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the offline defined colors of the cursors
///		in the application
///
///  RETURNS: 
///		void
/// 
///  REQUIREMENT ID: NONE                                
//-----------------------------------------------------------------------------
void CAsdApp::SetCursorColors()
{
	if (!mainWidget())
		return;

#if (defined Q_WS_X11)
	QColor colorinfill=CColorMngr::GetColor(CColorMngr::GetColorNb("MOUSE_INFILL"));
	QColor coloroutline=CColorMngr::GetColor(CColorMngr::GetColorNb("MOUSE_OUTLINE"));
	XColor bg,fg;
	bg.red=coloroutline.red()<<8;
	bg.blue=coloroutline.blue()<<8;
	bg.green=coloroutline.green()<<8;
	XAllocColor(mainWidget()->x11Display(),DefaultColormap(mainWidget()->x11Display(),0),&bg);
	fg.red=colorinfill.red()<<8;
	fg.blue=colorinfill.blue()<<8;;
	fg.green=colorinfill.green()<<8;;
	XAllocColor(mainWidget()->x11Display(),DefaultColormap(mainWidget()->x11Display(),0),&fg);
	XRecolorCursor(mainWidget()->x11Display(),m_CursorTable[MPNormalSelect].handle(),&bg,&fg);
	XRecolorCursor(mainWidget()->x11Display(),m_CursorTable[MPZooming].handle(),&bg,&fg);
	XRecolorCursor(mainWidget()->x11Display(),m_CursorTable[MPCrosshair].handle(),&bg,&fg);
	XRecolorCursor(mainWidget()->x11Display(),m_CursorTable[MPPanning].handle(),&bg,&fg);
#endif
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the cursors according to its CursorTypes: cursor
///
///  RETURNS: 
///		a cursor
///    
///  REQUIREMENT ID: NONE                             
//-----------------------------------------------------------------------------
QCursor* CAsdApp::GetCursor(CursorTypes cursor)
{
	return &m_CursorTable[cursor];
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This retrieves the screen number according to the 
///		location of the window
///
///  RETURNS: 
///		the screen number where is located the window
///    
///  REQUIREMENT ID: NONE                            
//-----------------------------------------------------------------------------
int CAsdApp::MapToScreen(QWidget* pWnd,QPoint& pos)
{
	int screennb=desktop()->screenNumber(pWnd);
	
	if (screennb!=-1)
	{
		QRect rect=desktop()->screenGeometry(screennb);
		pos=pos-rect.topLeft();
	}

	return screennb;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the position of a window 
///		according to the screen number
///
///  RETURNS: 
///		void
///  
///  REQUIREMENT ID: NONE                               
//-----------------------------------------------------------------------------
void CAsdApp::MapFromScreen(int ScreenNb,QPoint& pos)
{
	if ((ScreenNb!=-1) && (ScreenNb<desktop()->numScreens()))
	{
		QRect rect=desktop()->screenGeometry(ScreenNb);
		pos=pos+rect.topLeft();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the function elligibility is 
///		presents for the Group, SubGroup, Location, Role
///
///  RETURNS: 
///		a boolean that indicates if the function (eFName) 
///		has been succesfully retrieved offline defined
///   
///  REQUIREMENT ID: NONE                              
//-----------------------------------------------------------------------------
bool CAsdApp::FunctionEligibility(int eFName)
{
	return m_pAttribute->FunctionEligibility(eFName,"Common","Common","Common", "Common");
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the window elligibility is 
///		presents for the Group, SubGroup, Location, Role
///
///  RETURNS: 
///		a boolean that indicates if the window (ewName) 
///		has been succesfully retrieved offline defined
///   
///  REQUIREMENT ID: NONE                              
//-----------------------------------------------------------------------------
bool CAsdApp::WindowEligibility(int ewName)
{
	return m_pAttribute->WindowEligibility(ewName,"Common","Common","Common", "Common");
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the maximum and minimum size of 
///		the window for the Group, SubGroup, Location, Role
///
///  RETURNS: 
///		a boolean that indicates if the minimum and maximum size
///		has been succesfully retrieved offline defined
///  
///  REQUIREMENT ID: NONE                              
//-----------------------------------------------------------------------------
bool CAsdApp::SetMinMaxAttributes(QWidget* w,int ewName, bool FromCustomSet)
{
	return m_pAttribute->SetMinMaxAttributes(w,ewName,"Common","Common","Common", "Common", FromCustomSet);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the button decorations of 
///		a window for the Group, SubGroup, Location, Role
///
///  RETURNS: 
///		a boolean that indicates if the button decorations
///		has been succesfully retrieved offline defined
///  
///  REQUIREMENT ID: NONE                               
//-----------------------------------------------------------------------------
bool CAsdApp::WindowAttributes(QString Window,int ewName, bool Forced)
{
	return m_pAttribute->WindowAttributes(Window,ewName,"Common","Common","Common", "Common", Forced);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the window is moveable 
///		for the Group, SubGroup, Location, Role
///
///  RETURNS: 
///		a boolean that indicates if the moveability
///		of the window has been succesfully retrieved offline defined
///   
///  REQUIREMENT ID: NONE                             
//-----------------------------------------------------------------------------
bool CAsdApp::HasWindowTitle(QWidget* w,int ewName)
{
	return m_pAttribute->HasWindowTitle(w,ewName,"Common","Common","Common", "Common");
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method moves a window to its default position 
///		according to the sreen number
///
///  RETURNS: 
///		a boolean that indicates if the default position
///		has been succesfully retrieved
///    
///  REQUIREMENT ID: NONE                             
//-----------------------------------------------------------------------------
bool CAsdApp::WindowMoveDefault(QWidget* w,QString ewName, int Scr)
{	
	int x, y;
	CDefTlmPosList pTlmPosList = GetDefTlmPosList();

	pTlmPosList.GetDefaultTlmPos(ewName,x,y, Scr);
	int dx= x;
	int dy= y;
	QDesktopWidget* pDesktop=QApplication::desktop();
	QRect rect=pDesktop->screenGeometry(Scr);
	dx+=rect.left();
	dy+=rect.top();
	w->move(dx, dy);

	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the range and bearing is present for the 
///		group, subgroup, location, role
///
///  RETURNS: 
///		a boolean that indicates if the range and bearing line
///		time to go has been succesfully retrieved offline defined
///  
///  REQUIREMENT ID: NONE                               
//-----------------------------------------------------------------------------
bool CAsdApp::RBToGo()
{
	return m_pAttribute->RBToGo("Common","Common","Common","Common");

}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///    This method Log information in log file depending on the Debug Level
/// 
///  RETURNS
///    void
///   
///  REQUIREMENT ID: CIAsdSymOtmDbg010, CIAsdSymOtmDbg010, CIAsdItfComGen110, 
///					CIAsdFctIniGen200
//
//-----------------------------------------------------------------------------

void CAsdApp::WriteLogAsterix(const QString & strMessage, const LogTypes &Type, int nDebugLevel)
{
  if (m_asterixdbg || Type == LogError)
    WriteLogMsgInternal("", "asterix", strMessage, Type, nDebugLevel);
}

void CAsdApp::WriteLogMsg(const QString & strMessage, const LogTypes &Type, int nDebugLevel)
{
  WriteLogMsgInternal("", "log", strMessage, Type, nDebugLevel);
}

void CAsdApp::WriteLogMsgInternal(QString header, const QString & fileName, const QString & strMessage, const LogTypes &Type, int nDebugLevel)
{
	if (m_DebugLevel >= nDebugLevel)
	{
		//Open the file to log messages.
		QString m_strOfl_LOG_DIRECTORY=".";
		m_strOfl_LOG_DIRECTORY=m_LogDir;
		
		LPCSTR TypesStr[] = {"Information","Warning","Error"};
		QString strType=TypesStr[Type];
		QDir qdLogDir(m_strOfl_LOG_DIRECTORY);

		if (!qdLogDir.exists())
		{
			bool qDirCreate =qdLogDir.mkdir(m_strOfl_LOG_DIRECTORY);

			if (!qDirCreate)
			{
				if (!m_IsErrorDialogDisplayed)
				{
					m_IsErrorDialogDisplayed=true;
					QString message = "Daily log file cannot be created in " + m_strOfl_LOG_DIRECTORY;
					CSKMessageBox::ShowCritical(0,"Error Message",message,CSKMessageBox::Ok,CSKMessageBox::NoButton,CSKMessageBox::NoButton);
				}
			}		
		}
		else
		{
			QString strLogDate=QDateTime::currentDateTime().toString("yyyyMMdd");
			QString strLogFile=m_strOfl_LOG_DIRECTORY+"/"+fileName+"_"+strLogDate;
			QFile fd(strLogFile.ascii());
			bool isopen = fd.open(IO_WriteOnly|IO_Append);
			
			if (!isopen) 
			{
				if (!m_IsErrorDialogDisplayed)
				{
					m_IsErrorDialogDisplayed=true;
					QString message = "Daily log file cannot be created in " + m_strOfl_LOG_DIRECTORY;
					CSKMessageBox::ShowCritical(0,"Error Message",message,CSKMessageBox::Ok,CSKMessageBox::NoButton,CSKMessageBox::NoButton);
				}
			}
			else
			{
				if (!fd.isOpen())
				{
					if (!m_IsErrorDialogDisplayed)
					{
						///If the file cannot be opened, the standard output is used.
						m_IsErrorDialogDisplayed=true;
						QString message = "Daily log file cannot be opened in " + m_strOfl_LOG_DIRECTORY;
						CSKMessageBox::ShowCritical(0,"Error Message",message,CSKMessageBox::Ok,CSKMessageBox::NoButton,CSKMessageBox::NoButton);
					}
				}
				else
				{
					//Write the message.
					m_IsErrorDialogDisplayed=false;
					QString strHeader(strType);
					strHeader+=QDateTime::currentDateTime().toString("\tyyyy MM dd hh:mm:ss:zzz");
					strHeader+="\tApplication\tASD \t";
					strHeader+=m_HostName; ///should be hostname.
					strHeader+="\t";
					strHeader+=header;
					fd.writeBlock(strHeader.ascii(),strHeader.length());
					fd.writeBlock(strMessage.ascii(),strMessage.length());
					fd.writeBlock("\n",1);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///    This method Log information in Trace file
/// 
///  RETURNS
///    void
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
void CAsdApp::WriteTraceBufferAsterix(QString header, BYTE* buffer, int len, bool bOnOneLine, bool bReceive, sockaddr_in* addr, const LogTypes &Type, int nDebugLevel)
{
  if (m_asterixdbg || Type == LogError)
    WriteTraceBufferInternal(header, "asterix", buffer, len, bOnOneLine, bReceive, addr, Type, nDebugLevel);
}


void CAsdApp::WriteTraceBuffer(BYTE* buffer, int len, bool bOnOneLine, bool bReceive, sockaddr_in* addr)
{
  WriteTraceBufferInternal("", "log", buffer, len, bOnOneLine, bReceive, addr, LogInformation);
}

void CAsdApp::WriteTraceBufferInternal(QString header, const QString & fileName, BYTE* buffer, int len, bool bOnOneLine, bool bReceive, sockaddr_in* addr, const LogTypes &Type, int nDebugLevel)
{
	QString trace;
	QString temp;
	QString ascii;
	QString sFinalTrace;
	QString sSeparator = " -;- ";
	
	for(int t = 0; t < len; t++)
	{
		if (t % 16 == 0)
		{
			if (!trace.isEmpty())
			{
				trace = trace + "\"" + ascii + "\"";
				
				if (!bOnOneLine)
					WriteTraceMsgInternal(header, fileName, trace, bReceive, addr, Type, nDebugLevel);
				else
				{
					if(sFinalTrace.isEmpty())
						sFinalTrace = trace;
					else
						sFinalTrace += sSeparator + trace;
				}
			}
			trace.sprintf("%.8x   ", (unsigned int)t);
			ascii="";
		}
		temp.sprintf("%.2x ", (unsigned int)buffer[t]);
		
		if ((t+8) % 16 == 0)
			trace += " ";
		trace += temp;
		
		if (buffer[t] >=32 && buffer[t]<=126)
		{
			temp.sprintf("%c", buffer[t]);
			ascii += temp;
		}
		else
			ascii += ".";
	}

	if (!trace.isEmpty())
	{
		if (!bOnOneLine)
			WriteTraceMsgInternal(header, fileName, trace, bReceive, addr, Type, nDebugLevel);
		else
		{
			if(sFinalTrace.isEmpty())
				sFinalTrace = trace;
			else
				sFinalTrace += sSeparator + trace;
		}
	}

	if (bOnOneLine)
		WriteTraceMsgInternal(header, fileName, sFinalTrace, bReceive, addr, Type, nDebugLevel);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///    This method log information in log file depending on the Debug Level
/// 
///  RETURNS
///    void
///   
///  REQUIREMENT ID: CIAsdItfComGen100  
//-----------------------------------------------------------------------------
void CAsdApp::WriteTraceMsg(const QString & strMessage, bool bReceive, sockaddr_in* addr)
{
      WriteTraceMsgInternal("", "log", strMessage, bReceive, addr, LogInformation);
}

void CAsdApp::WriteTraceMsgInternal(QString header, const QString & fileName, const QString & strMessage, bool bReceive, sockaddr_in* addr, const LogTypes &Type, int nDebugLevel)
{
	QString sDirection;
	QString sAdress;
	QString sText="";
	
	if (addr != NULL)
	{
		if(bReceive) 
			sDirection = "Receive msg from adress :";
		else 
			sDirection = "Send msg to adress : ";
		
		sAdress.sprintf("%x", addr->sin_addr.s_addr);
		sText = sDirection + sAdress + " . Message : ";
	}
	
	WriteLogMsgInternal(header, fileName, sText + strMessage, Type, 1);
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///    This method records the position of a window
/// 
///  RETURNS
///    void
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
void CAsdApp::RecordMove(QWidget* w, QPoint p)
{
	CWdwMngr::RecordMove(w,p);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///    This method cancels the position of a window
/// 
///  RETURNS
///    void
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
void CAsdApp::CancelMove(QWidget* w)
{	
	CWdwMngr::CancelMove(w);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///    This method retrieves the collapsability of the main 
///	   radar for the group, subgroup, location, role
/// 
///  RETURNS
///    a boolean which indicates if the collapsibility of the main 
///	   radar has been successfuly retrieved
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
bool CAsdApp::IsMainCollapsible()
{
	return m_pAttribute->IsMainCollapsible("Common","Common","Common","Common");
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method checks if the CWP is an approach position
///		for the group, subgroup, location, role
/// 
///  RETURNS
///     a boolean which indicates if the CWP is an Approach position
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
bool CAsdApp::IsAppPos()
{
	return m_pAttribute->IsAPPPos("Common","Common","Common","Common");
}


#if (defined Q_WS_X11)
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method catch the XEvent from X Server and 
///		particurlarly the leaveNotify event in order to move the window if 
///		necessary when the mouse pointer leaves a window
/// 
///  RETURNS
///     TRUE if you want to stop the event from being 
///		processed. Return FALSE for normal event dispatching. 
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
bool CAsdApp::x11EventFilter(XEvent * ev)
{
	switch (ev->type) 
	{
	case LeaveNotify:
		if (CWdwMngr::IsRecordMoved())
			CWdwMngr::MoveWindow();
		break;
	default:
		break;
	}
	
	return false;
}
#endif


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method get the Aircraft type list
/// 
///  RETURNS
///     the object of class CATypList
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CATypList CAsdApp::GetATypWtcList()
{
	return m_ATypWtc;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method retrieves the ADES list
/// 
///  RETURNS
///     the object of class CAdesList
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CAdesList CAsdApp::GetAdesList()
{
	return m_AdesList;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method gets the object of CDiscreteSSRList class
/// 
///  RETURNS
///     an object of type CDiscreteSSRList
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
CDiscreteSSRList CAsdApp::GetDiscreteSsrList()
{
	return m_DiscreteSsr;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method gets the object of type CDefTlmPosList
/// 
///  RETURNS
///     an object of type CDefTlmPosList
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
CDefTlmPosList CAsdApp::GetDefTlmPosList()
{
	return m_DefTlmPosList;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///		This method sets the debug level to the value give in parameter 
/// 
///  RETURNS
///		void
///   
///  REQUIREMENT ID: NONE  
//   
//-----------------------------------------------------------------------------
void CAsdApp::SetDebugLevel(int nDebugLevel)
{
	m_DebugLevel = nDebugLevel;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///		The RemoveAllWindow method removes the called windows after a 
///		reconfiguration
/// 
///  RETURNS 
///		void
///   
///  REQUIREMENT ID: NONE  
//   
//-----------------------------------------------------------------------------
void CAsdApp::RemoveAllWindow()
{
	if (CRwdView::m_psMainWnd)
	{
		CWdwMngr::HideWindow(&CRwdView::m_psMainWnd->m_HeightFilter);
		CWdwMngr::HideWindow(&CRwdView::m_psMainWnd->m_TrackControl);
	}

	if (CRwdView::m_psSecWnd1)
	{
		CWdwMngr::HideWindow(&CRwdView::m_psSecWnd1->m_HeightFilter);
		CWdwMngr::HideWindow(&CRwdView::m_psSecWnd1->m_TrackControl);
	}

	if (CRwdView::m_psSecWnd2)
	{
		CWdwMngr::HideWindow(&CRwdView::m_psSecWnd2->m_HeightFilter);
		CWdwMngr::HideWindow(&CRwdView::m_psSecWnd2->m_TrackControl);
	}

	if (CTopLevel::m_psColor)
		CWdwMngr::HideWindow(CTopLevel::m_psColor);

	if (CTopLevel::m_psFindTrack)
		CWdwMngr::HideWindow(CTopLevel::m_psFindTrack);

	if (&GetUserMngr()->m_WChangePasswd)
		CWdwMngr::HideWindow(&GetUserMngr()->m_WChangePasswd);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///		The StartReconfigTimer method starts the m_ReconfigTimerId timer
/// 
///  RETURNS 
///		void
///   
///  REQUIREMENT ID: NONE 
//   
//-----------------------------------------------------------------------------
void CAsdApp::StartReconfigTimer()
{
	m_ReconfigTimerId = startTimer(1000);
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///		This function returns the version of the launched viewer
/// 
///  RETURNS 
///		QString with the launched viewer
///   
///  REQUIREMENT ID: NONE 
//   
//-----------------------------------------------------------------------------
void CAsdApp::SetViewerVersion()
{
	QString strVersion = QDir::currentDirPath();
	QString strItem = "viewer-";
	QDir dirFilename=QDir(QDir::current());
	QString strFileName=strVersion+"/";
	strFileName+=(dirFilename.entryList(strItem+"*").first());
	if (strFileName.endsWith("/"))
		strFileName.truncate(strFileName.length()-1);
	QString tmp=strFileName;
	int len=tmp.findRev("-",-1);//-1 starts to search from the end, -2 at the next to last and so on, returns -1 if not found
	if (len!=-1)
	{
		len++;//not to keep the "-"
		int j=tmp.length();
		tmp=tmp.right(j-len);//to keep only "x.x.x.x.exe"
		m_sViewerVersion = tmp;
	}
	else
		m_sViewerVersion = "not found";
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///		This function returns the version of the launched viewer
/// 
///  RETURNS 
///		QString with the launched viewer
///   
///  REQUIREMENT ID: NONE 
//   
//-----------------------------------------------------------------------------
QString CAsdApp::GetViewerVersion()
{
	return m_sViewerVersion;
}




//-----------------------------------------------------------------------------
//
/// \class CATypList
/// This class is used to contruct the list of aircraft type associated 
///	to the wake turbulence category 
// 
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method is the constructor of the CATypList class
/// 
///  RETURNS
///     void
///  
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CATypList::CATypList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method is the destructor
/// 
///  RETURNS
///     void
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CATypList::~CATypList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method retrieves the information offline defined 
///		concerning the Aircraft type and wake turbulence category
///		file is the <version>.ini file
/// 
///  RETURNS
///     a boolean which indicates if the information has been successfully 
///		retrieved
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CATypList::ReadDataSet(CIniFile& file)
{
	CArray<QString,QString> SectionTable;
	file.GetIniProfileSection("ATYP_WTC",SectionTable);
	
	if (SectionTable.GetSize() == 0)
	{
		CAsdApp::GetApp()->WriteLogMsg("Aircraft Type section in Dataset is Empty.",LogError);
		return false;
	}

	for (int j=0;j<SectionTable.GetSize();j++)
	{
		QString current;
		file.GetIniProfileString("ATYP_WTC",SectionTable[j],current);
		QString atyp = SectionTable[j];
		
		// Fill The ASD Table
		int fnd;
		fnd=current.find("|");
		
		while (fnd!=-1)
		{
			QString wtc;
			wtc = current.left(fnd);
			current=current.right(current.length()-fnd-1);
			fnd=current.find("|");
			Add(atyp, wtc);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method adds an aircraft type (atyp) and its associated wake 
///		turbulence category (wtc) in the Atyp list
/// 
///  RETURNS
///     a boolean true by default
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
bool CATypList::Add(QString atyp, QString wtc)
{
    /* CDS BREAK JPM 2006/05/10 Use of ptr->fld access cannot be used in this case the iterator is of type QValueList */
    QValueList<m_WakeTC>::iterator it;
    for ( it = m_ATypList.begin(); it != m_ATypList.end(); ++it )
	{
		if ( (*it).ATyp == atyp)
		{
			QValueList<QString>::iterator it2;
			
			for ( it2 = (*it).Wtc.begin(); it2 != (*it).Wtc.end(); ++it2 )
			{
				if ( (*it2) == wtc)
					return false;
			}
			(*it).Wtc.append(wtc);
			return false;
		}
	}

	m_WakeTC waketc;
	waketc.ATyp = atyp;
	
	if (wtc != "")
		waketc.Wtc.append( wtc );

	m_ATypList.append( waketc );

	return true;
}


//-----------------------------------------------------------------------------
//
/// \class CAdesList
/// This class is used to contruct the list of ADES 
// 
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method is the constructor of the CAdesList class
/// 
///  RETURNS
///     void
///  
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CAdesList::CAdesList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method is the destructor
/// 
///  RETURNS
///     void
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CAdesList::~CAdesList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method retrieves the information offline defined 
///		concerning the ADES in the <version>.ini file
/// 
///  RETURNS
///     a boolean which indicates if the information has been successfully 
///		retrieved
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CAdesList::ReadDataSet(CIniFile& file)
{
	CArray<QString,QString> SectionTable;
	file.GetIniProfileSection("ADES",SectionTable);
	
	if (SectionTable.GetSize() == 0)
	{
		CAsdApp::GetApp()->WriteLogMsg("ADES section in Dataset is Empty.",LogError);
		return false;
	}

	for (int j=0;j<SectionTable.GetSize();j++)
	{
		QString current;
		file.GetIniProfileString("ADES",SectionTable[j],current);
		
		int fnd;
		fnd=current.find("|");
		
		while (fnd!=-1)
		{
			QString ades;
			ades = current.left(fnd);
			current=current.right(current.length()-fnd-1);
			fnd=current.find("|");
			Add(ades);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method adds an ADES in the ADES list
/// 
///  RETURNS
///     a boolean true by default
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
bool CAdesList::Add(QString ades)
{
	m_AdesList.append( ades );
	return true;
}



//-----------------------------------------------------------------------------
//
/// \class CDiscreteSSRList
/// This class is used to construct the Discrete SSR Code
// 
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method is the construtor
/// 
///  RETURNS
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CDiscreteSSRList::CDiscreteSSRList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method is the destructor
/// 
///  RETURNS
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CDiscreteSSRList::~CDiscreteSSRList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method retrieves the information offline defined 
///		concerning the Discretes SSR code
///		file is <version>.ini file
/// 
///  RETURNS
///     a boolean which indicates if the information has been successfully 
///		retrieved
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
bool CDiscreteSSRList::ReadDataSet(CIniFile& file)
{
	CArray<QString,QString> SectionTable;
	file.GetIniProfileSection("DISCRETE_SSR_CODES",SectionTable);
	
	if (SectionTable.GetSize() == 0)
	{
		CAsdApp::GetApp()->WriteLogMsg("DISCRETE_SSR_CODES section in Dataset is Empty.",LogError);
		return false;
	}

	for (int j=0;j<SectionTable.GetSize();j++)
	{
		QString current;
		file.GetIniProfileString("DISCRETE_SSR_CODES",SectionTable[j],current);
		
		// Fill The ASD Table
		int fnd, fnd2;
		fnd=current.find("|");
		
		while (fnd!=-1)
		{
			QString range;
			range = current.left(fnd);
			current=current.right(current.length()-fnd-1);
			fnd=current.find("|");
			fnd2=range.find("-");
			int min=atoi((range.left(fnd2)).ascii());
			int max=atoi((range.right(range.length()-fnd2-1)).ascii());
			Add(min, max);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method adds a range (mini, max) of dicrete SSR code and its 
///		associated Copx to the FIR list
/// 
///  RETURNS
///     a boolean true by default
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
bool CDiscreteSSRList::Add(int min, int max)
{
	m_SsrRange DiscSsr;
	DiscSsr.MinSsr = min;
	DiscSsr.MaxSsr = max;

	m_DiscretSsrList.append( DiscSsr );

	return true;
}


//-----------------------------------------------------------------------------
//
/// \class CDefTlmPosList
/// This class is used to contruct the list of windows with their default position
// 
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This CDefTlmPosList method is the construtor
///
///  RETURNS
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
CDefTlmPosList::CDefTlmPosList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method is the destructor
///
///  RETURNS
///   
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CDefTlmPosList::~CDefTlmPosList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method retrieves the information offline defined 
///		concerning the default position of the Top Level Menu windows
///		file is <version>.ini file
/// 
///  RETURNS
///     a boolean which indicates if the information has been successfully 
///		retrieved
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CDefTlmPosList::ReadDataSet(CIniFile& file)
{
	QString xPos, yPos;
	int x=0, y=0;
	CArray<QString,QString> SectionTable;
	file.GetIniProfileSection("DEFAULT_TLM_POS",SectionTable);
	
	if (SectionTable.GetSize() == 0)
	{
		CAsdApp::GetApp()->WriteLogMsg("Default TLM Position section in Dataset is Empty.",LogError);
		return false;
	}
	
	for (int j=0;j<SectionTable.GetSize();j++)
	{
		QString current;
		file.GetIniProfileString("DEFAULT_TLM_POS",SectionTable[j],current);
		QString window = SectionTable[j];
		
		// Fill The Table
		int fnd;
		fnd=current.find("|");
		
		if (fnd!=-1)
		{
			xPos = current.left(fnd);
			x = xPos.toInt();
		}

		current=current.right(current.length()-fnd-1);
		fnd=current.find("|");
		
		if (fnd!=-1)
		{
			yPos = current.left(fnd);
			y = yPos.toInt();
		}

		for (int screen=0; screen<CAsdApp::desktop()->numScreens();screen++)
			Add(window, x, y, screen);
	}

	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method adds a window and its position and screen number
/// 
///  RETURNS
///     a boolean true by default
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
bool CDefTlmPosList::Add(QString window, int x, int y, int screenNb)
{
	m_DefPos DefTlmPos;
	
	DefTlmPos.Window = window;
	DefTlmPos.X = x;
	DefTlmPos.Y = y;
	DefTlmPos.Screen = screenNb;
	m_DefTlmPosList.append( DefTlmPos );
	
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method gets the default position of a window 
///		according to the screen number
/// 
///  RETURNS
///     void
///   
///  REQUIREMENT ID: NONE  
//-----------------------------------------------------------------------------
void CDefTlmPosList::GetDefaultTlmPos(QString window,int &X, int &Y, int Scr)
{
	/* CDS BREAK JPM 2006/05/10 Use of ptr->fld access cannot be used in this case the iterator is of type QValueList */
	QValueList<m_DefPos>::iterator it;
    
	for ( it = m_DefTlmPosList.begin(); it != m_DefTlmPosList.end(); ++it )
	{
		if ( (*it).Window == window && (*it).Screen == Scr)
		{
			X= (*it).X;
			Y= (*it).Y;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION
///     This method sets the default position of a window 
///		according to the screen number
/// 
///  RETURNS
///     void
///   
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CDefTlmPosList::SetDefaultTlmPos(QString window, int X, int Y, int Screen)
{
	/* CDS BREAK JPM 2006/05/10 Use of ptr->fld access cannot be used in this case the iterator is of type QValueList */
	QValueList<m_DefPos>::iterator it;
    
	for ( it = m_DefTlmPosList.begin(); it != m_DefTlmPosList.end(); ++it )
	{
		if ( (*it).Window == window && (*it).Screen == Screen)
		{
			QDesktopWidget* pDesktop=QApplication::desktop();
			QRect rect=pDesktop->screenGeometry(Screen);
			(*it).X=X-rect.left();
			(*it).Y=Y-rect.top();
		}
	}
}


//--------------------------------< THE END >----------------------------------


