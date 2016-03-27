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
//  FILENAME: AsdApp.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file retrieves the parameters offline defined in the adaptation dataset.
//		This file instanciates the tracks, Fpl, coupling, local tags, safety nets, weather
// 
//  COMMENTS
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

#if !defined(AFX_ASDAPP__INCLUDED_)
#define AFX_ASDAPP__INCLUDED_


//-INCLUDE FILES---------------------------------------------------------------
#include "WfAttrib.h"
#include "CommonDataTypes.h"
#include "TopLevel.h"
#include "TimeWnd.h"
#include "Interface.h"



//-GLOBAL DATA-----------------------------------------------------------------
class CTrackMngr;
class CFplMngr;
class CInfoMngr;
class CCouplingMngr;
class CWeatherMngr;
class CWin_Hints;
class CUserMngr;
class CSystemMode;
class CEventMngr;

typedef enum { MPNormalSelect,MPWorking,MPPanning,MPZooming,MPCrosshair,MPNoEntry} CursorTypes;
typedef enum {DEBUG_LOW=1, DEBUG_HIGH} DebugLevel;

const QString GrpSbgrpLocRolDepDir="/GroupSubgroupLocationRoleDependent/";


//-CLASS DEFINITION---------------------------------------------------------
class CATypList : public CObject
{
public :
	//This structure defines the offline defined ATYP-WTC associations 
	struct m_WakeTC {
			//ATYP
			QString ATyp;
			//List of associated WTC
			QValueList<QString> Wtc;
		};
public :
	CATypList();
	~CATypList();
	bool Add(QString Atyp, QString Wtc);
	bool ReadDataSet(CIniFile& file);
	QValueList<m_WakeTC>* getATypList() 
		{ return &m_ATypList; };
private :
	QValueList<m_WakeTC> m_ATypList;
};

class CAdesList : public CObject
{
public :
	CAdesList();
	~CAdesList();
	bool Add(QString Ades);
	bool ReadDataSet(CIniFile& file);
	QValueList<QString>* getAdesList() 
		{ return &m_AdesList; };
private :
	QValueList<QString> m_AdesList;
};

class CDiscreteSSRList : public CObject
{
public :
	//This structure defines a range of SSR discrete codes
	struct m_SsrRange {
			//starting SSR code of the range
			int MinSsr;
			//ending SSR code of the range
			int MaxSsr;
		};
public :
	//Constructor
	CDiscreteSSRList();
	//Destructor
	~CDiscreteSSRList();
	//This method adds a new SSR discrete code range to the m_DiscretSsrList list
	bool Add(int Min, int Max);
	//This method retrieves the SSR discrete codes form the version.ini file
	bool ReadDataSet(CIniFile& file);
	//This method returns the m_DiscretSsrList list
	QValueList<m_SsrRange>* getDiscretSsrList() 
	{ 
		return &m_DiscretSsrList; 
	};
private :
	//List of discrete SSR code ranges
	QValueList<m_SsrRange> m_DiscretSsrList;
};


class CFIRList : public CObject
{
public :
	//This structure defines a FIR - COPX association
	struct m_FirCopx {
			//FIR name
			QString Fir;
			//List of associated COPX (coordination points)
			QValueList<QString> Copx;
		};
public :
	//Constructor
	CFIRList();
	//Destructor
	~CFIRList();
	//This method adds a new FIR - COPX association to the m_FirList list
	bool Add(QString Fir, QString Copx);
	//This method retrieves the FIR - COPX associations from the version.ini file
	bool ReadDataSet(CIniFile& file);
	//This method returns the m_FirList list
	QValueList<m_FirCopx>* getFirList() 
	{
		return &m_FirList; 
	};
private :
	//FIR - COPX association list
	QValueList<m_FirCopx> m_FirList;
};

class CCsuCopxList : public CObject
{
public :
	//This structure symbolises the offline defined CSU - COPX association
	struct m_CsuCopx {
			//Sector Id (one letter)
			char Csu;
			//Associated Coordination points
			QValueList<QString> Copx;
		};
public :
	//Constructor
	CCsuCopxList();
	//Destructor
	~CCsuCopxList();
	//This method adds a new CSU - COPX association
	bool Add(char Csu, QString Copx);
	//This method retrives all the associations from the version.ini file 
	bool ReadDataSet(CIniFile& file);
	//This method returns the complete CSU - COPX association list
	QValueList<m_CsuCopx>* getCsuList()
	{ 
		return &m_CsuList;
	};

private :
	//CSU - COPX association list
	QValueList<m_CsuCopx> m_CsuList;
};

class CDefTlmPosList : public CObject
{
public :
	//This structure defines the position of each CWP screen
	struct m_DefPos {
			//Screen name
			QString Window;
			//Horizontal coordinate
			int X;
			//Vertical coordinate
			int Y;
			//Screen number
			int Screen;
		};
public :
	//Constructor
	CDefTlmPosList();
	//Destructor
	~CDefTlmPosList();
	//This method adds a new screen in the m_DefTlmPosList list
	bool Add(QString window, int x, int y, int Screen);
	//This method retrieves the screen information from the version.ini file
	bool ReadDataSet(CIniFile& file);
	//This method retrieves the position of a screen identified by its number
	//or name
	void GetDefaultTlmPos(QString window,int &X, int &Y, int Screen);
	//This method sets the psoition of a screen
	void SetDefaultTlmPos(QString window, int X, int Y, int Screen);
private :
	//List of screen definitions
	QValueList<m_DefPos> m_DefTlmPosList;
};

class CSystemModeEventMngr;

class CAsdApp: public QApplication
{

public:

	//methods

	// constructor and destructor
	/* CDS BREAK CAsdApp is the application object, it's unique and then there's no need 
	to implement the copy constructor for this class */
	CAsdApp ( int & argc, char ** argv );
	~CAsdApp();	
	//sets the debug level to the value given in parameter
	void SetDebugLevel(int nDebugLevel);

	//static methods

	//retrieves the current application instance
	static CAsdApp* GetApp();
	//Log information in log file depending on the Debug Level
	void WriteLogMsg(const QString & strMessage, const LogTypes &Type, int nDebugLevel=0);
	void WriteLogAsterix(const QString & strMessage, const LogTypes &Type, int nDebugLevel=0);
	void WriteLogMsgInternal(QString header, const QString & fileName, const QString & strMessage, const LogTypes &Type, int nDebugLevel);
	
	//methods
	//due to namespace CDataMngr, CColorMngr, CWdwMngr these methods must be public
	//retrieves a pointer of type CWin_Hints
	CWin_Hints* GetWinHints();
	//retrieves the default offline path
	QString GetDefaultOfflinePath();
	//gets the name of the application
	QString GetApplicationIni();
	//sets the colors of windows that are either pre-defined or offline defined
	void SetColors();
	//sets the offline defined colors of the cursors in the application
	void SetCursorColors();	
	//check if the offline data are retrieved correctly or not
	void setOfflineOk(bool OfflineOk) 
	{ 
		m_OfflineOk = OfflineOk; 
	}
	// retrieves the position of a window according to the screen number
	static void MapFromScreen(int ScreenNb,QPoint& pos);
	//retrieves the screen number according to the location of the window
	static int MapToScreen(QWidget* pWnd,QPoint& pos);
	//sets the connections to the CCS
	void setConnectionless(bool Connectionless) 
	{ 
		m_Connectionless = Connectionless; 
	};
	//looks for the adaptation data set if exists
	//Then it creates the track, flight plan object but also the Coupling, 
	//Alerts, Weather, CCS, Sequence list, Local tag, objects
	//Also creates the Main Radar window as well as the Top Level
	//Menu and the Time window and then apply the default setup according to the sectorization
	int Init();	
	//retrieves the cursors according to its CursorTypes: cursor
	QCursor* GetCursor(CursorTypes cursor);
	//records the position of a window
	void RecordMove(QWidget* w, QPoint p);
	//cancels the position of a window
	void CancelMove(QWidget* w);
	//sets the button decorations of a window for the Group, SubGroup, Location, Role
	bool WindowAttributes(QString Window,int ewName, bool Forced=false);
	//sets the maximum and minimum size of the window for the Group, SubGroup, Location, Role
	bool SetMinMaxAttributes(QWidget* w,int ewName, bool FromCustomSet);
	//logs information in log file depending on the Debug Level
	void WriteTraceMsg(const QString & strMessage, bool bReceive = true, sockaddr_in* addr=NULL);
	void WriteTraceMsgInternal(QString header, const QString & fileName, const QString & strMessage, bool bReceive, sockaddr_in* addr, const LogTypes &Type, int nDebugLevel=0);
	//checks if a window has a title
	bool HasWindowTitle(QWidget* w,int ewName);	
	//moves a window to its default position according to the sreen number
	bool WindowMoveDefault(QWidget* w,QString ewName, int Scr);	
	//checks if the range and bearing is present for the group, subgroup, location, role
	bool RBToGo();
	//check if the adaptation data set exists and retrieves the <version>.ini file
	bool ComputeDataSetPath();
	// checks if the CWP is an approach position for the group, subgroup, location, role
	bool IsAppPos();
	//Logs information in Trace file
	void WriteTraceBuffer(BYTE* buffer, int len, bool bOnOneLine, bool bReceive = true, sockaddr_in* addr=NULL);
	void WriteTraceBufferAsterix(QString header, BYTE* buffer, int len, bool bOnOneLine, bool bReceive, sockaddr_in* addr, const LogTypes &Type, int nDebugLevel=0);
	void WriteTraceBufferInternal(QString header, const QString & fileName, BYTE* buffer, int len, bool bOnOneLine, bool bReceive, sockaddr_in* addr, const LogTypes &Type, int nDebugLevel=0);
	//checks if the main radar window is collapsible
	bool IsMainCollapsible();	
	// removes the called windows after a reconfiguration
	void RemoveAllWindow();
	// called to make the Top Level Menu and Time window visible on each screen
	void ShowTopLevel();
	// checks if the window elligibility is presents for the Group, SubGroup, Location, Role
    bool WindowEligibility(int ewName);
	// checks if the function elligibility is presents for the Group, SubGroup, Location, Role
	bool FunctionEligibility(int efName);
	// starts the m_ReconfigTimerId timer
	void StartReconfigTimer();
	// gets the object of CDiscreteSSRList class
	CDiscreteSSRList GetDiscreteSsrList();	
	//gets the directory where the adaptation data set is located
	QString GetDataSetDir();
	//retrieves a pointer of type CUserMngr
	CUserMngr* GetUserMngr();
	//gets the Aircraft type list
	CATypList GetATypWtcList();
	//gets the Ades list
	CAdesList GetAdesList();
	//retrieves a pointer of type CTrackMngr
	CTrackMngr* GetTrackMngr();	
	//retrieves a pointer of type CInfoMngr
	CInfoMngr* GetInfoMngr();
	// pointer of type CTrackMngr for the management of tracks
	CTrackMngr* m_pTrackMngr;
	// pointer of type CFplMngr for the management of flight plan
	CFplMngr* m_pFplMngr;
	// pointer of type CInfoMngr for the management of information retrieved offline
	CInfoMngr* m_pInfoMngr;
	// pointer of type CCouplingMngr for the management of track to flight plan coupling
	CCouplingMngr* m_pCouplingMngr;
	// pointer of type CWeatherMngr for the management of weather
	CWeatherMngr* m_pWeatherMngr;
	// pointer of type CUserMngr for the management of sign in and password
	CUserMngr* m_pUserMngr;	
	// pointer of type CEventMngr to manage event for external system
	CEventMngr* m_pEventMngr;
	// pointer of type CWFAttrib to get the offline defined attributes of a window or function
	CWFAttrib* m_pAttribute;
	// pointer of type CWin_Hints
	CWin_Hints* m_pWinHints;
	// pointer of type widget to know the window that has been resized
	QWidget* m_pWRecordResize;
	// pointer of type CSystemMode to get the system mode and submode
	CSystemMode* m_pMode;
	// retrieves a pointer of type CFplMngr to manage the fligh plan
	CFplMngr* GetFplMngr();	
	// retrieves a pointer of type CCouplingMngr to manage the coupling
	CCouplingMngr* GetCouplingMngr();
	//retrieves a pointer of type CWeatherMngr
	CWeatherMngr* GetWeatherMngr();
	// retrieves the system mode
	CSystemMode* GetSystemMode();	
	//Returns the version of the launched Viewer
	QString GetViewerVersion();
	//Sets the version of the launched Viewer
	void SetViewerVersion();

	//retrieves the timer for framing
	int getFramingTimer() 
	{ 
		return m_FramingTimer; 
	};
	//retrieves a pointer of type CDefTlmPosList
	CDefTlmPosList* getDefTlmPosList() 
	{ 
		return &m_DefTlmPosList; 
	}
	//retrieves the custom set directory
	QString getCustomDir() 
	{ 
		return m_CustomDir; 
	}
	//gets the offset of horizontal bottom left range marker
	int getOffsetHBL() 
	{ 
		return m_iOffsetHBL; 
	};
	//gets the offset of vertical bottom left range marker
	int getOffsetVBL() 
	{ 
		return m_iOffsetVBL; 
	};
	//gets the offset of horizontal top left range marker
	int getOffsetHTR() 
	{ 
		return m_iOffsetHTR; 
	};
	//gets the offset of vertical top left range marker
	int getOffsetVTR() 
	{ 
		return m_iOffsetVTR; 
	};
	//retrieves the time to discard weather
	int getWeatherDiscard() 
	{ 
		return m_WeatherDiscard; 
	};
	// checks if the retrieved offline data is correct
	bool getOfflineOk() 
	{ 
		return m_OfflineOk; 
	}
	// gets the boolean which indicates if a painting is on-going
	bool getStartXPaint() 
	{ 
		return 	m_StartXPaint; 
	};	
	// gets the time of last painting
	QDateTime getLastXPaint() 
	{ 
		return m_LastXPaint; 
	};	
	// get the boolean value for connections to the CCS
	bool getConnectionless() 
	{ 
		return m_Connectionless; 
	};
	// gets the time for temporary hide STCA
	int getStcaTempTime() 
	{ 
		return m_StcaTempTime; 
	};
	// gets the time for temporary hide MSAW and APW
	int getMsawApwTempTime() 
	{ 
		return m_MsawApwTempTime; 
	};
	// sets the custom set directory
	void setCustomDir(QString CustomDir) 
	{ 
		m_CustomDir = CustomDir; 
	};
	//retrieves the time when the application starts
	QDateTime getStartTime() 
	{ 
		return m_StartTime; 
	};
	//retrieves the first painting of the application to check server X server
	void setStartXPaint(bool StartXPaint) 
	{ 
		m_StartXPaint = StartXPaint; 
	};
	//retrieves the last painting of the application to check server X server
	void setLastXPaint(QDateTime LastXPaint) 
	{ 
		m_LastXPaint = LastXPaint; 
	};	
	// retrieves a pointer of class CWFAttrib to get window and functions elligibilities and interact on it
	CWFAttrib* getAttribute() 
	{ 
		return m_pAttribute; 
	}
	//retrieves the time to discard radar
	int getRadarDiscard() 
	{ 
		return m_RadarDiscard; 
	};
	//retrieves the time for AutoClose
	int getAutoCloseTime() 
	{ 
		return m_AutoCloseTime; 
	}

private:

	// variables

	//Version of the launched viewer
	QString m_sViewerVersion;
	//path of custom set
	QString	m_CustomDir;
	//boolean to check if the offline data have been retrieved correctly
	bool m_OfflineOk;
	//object of type CATypList
	CATypList m_ATypWtc;
	//object of type CAdesList
	CAdesList m_AdesList;
	//object of type m_DiscreteSsr
	CDiscreteSSRList m_DiscreteSsr;	
	//object of type CDefTlmPosList
	CDefTlmPosList m_DefTlmPosList;
	//check if the offline color for ASD application have been set
	bool m_ColorSet;
	//boolean to display a Error message
	bool m_IsErrorDialogDisplayed;	
	//array that contain the Top Level Menu
	CArray<CTopLevel*,CTopLevel*> m_TopLevelTable;
	//array that contain the Time Window
	CArray<CTimeWnd*,CTimeWnd*> m_TimeWndTable;
	// time to discard radar message
	int m_RadarDiscard;
	// time to discard safety messages
	int m_SafetyDiscard;
	//time to discard weather messages
	int m_WeatherDiscard;
	// time to temporary hide MSAW ans APW alerts
	int m_MsawApwTempTime;
	// time to temporary hide STCA alerts
	int m_StcaTempTime;
	// time of auto close popups windows or windows
	int m_AutoCloseTime;
	// offset for horizontal Botton Left range marker
	int m_iOffsetHBL;
	// offset for vertical Botton Left range marker
	int m_iOffsetVBL;
	// offset for horizontal Top Left range marker
	int m_iOffsetHTR;
	// offset for vertical Top Left range marker
	int m_iOffsetVTR;
	// time for framing
	int m_FramingTimer;
	// boolean which indicates if connections have to be set or not
	bool m_Connectionless;
	// timer used to move, activate, resize and order windows
	int m_TimerId;
	// _ApplicationIniName is the <version>.ini and file the current path
	QString m_ApplicationIniName;
	// a list of cursor containing the following cursor type: NormalSelect, Zooming, CrossHair and Panning
	QCursor m_CursorTable[MPNoEntry+1];	
	// default path where the version.ini file is located
	QString m_DefaultOfflinePath;
	// default path where the dataset is located
	QString m_DataSetPath;	
	// directory path for logs
	QString m_LogDir;
	// hostname
	QString m_HostName;	
	// time when the application starts
	QDateTime m_StartTime;
	// timer when a window is resized
	int m_iResizeTimer;
	// timer when a reconfiguration starts
	int m_ReconfigTimerId;
	// object of type CInterfaceList
	CInterfaceList m_Interfaces;
	// boolean which indicates that a painting is on-going
	bool m_StartXPaint;
	// time of the last painting
	QDateTime m_LastXPaint;
	// debug level value
	int m_DebugLevel;
	// timer when the initialization starts
	int m_InitTimerId;
	// timer to update the FPL Lists content
	int m_TimerFillList;
	
	bool m_asterixdbg;
	//dynamic methods	
	
	// gets the object of type CDefTlmPosList
	CDefTlmPosList GetDefTlmPosList();
	// sets the default set up according to the sectorization
	void ApplyDefaultSetup();
	// extract the following parameters from the 
	// <version>.ini file of the adaptation data set:
	//		- the Direction Finder parameters
	//		- the radar and weather service parameters,
	//		- the CCS sources parameters,
	//		- the time after which the message is discarded if no reception 
	//		  safety, radar or weather is received,
	//		- time duration to hide STCA, APW, MSAW alerts,
	//		- the Range marker parameters,
	//		- time of framing for windows,
	//		- time after which a window is automatically closed 
	//		  for windows having the auto-close behavior
	void ReadVersionIni();			
	// catch the event (e) associated to the QT object (receiver)
	bool notify ( QObject * receiver, QEvent * e );
	// catches all the timer event for the CAsdApp class
	void timerEvent( QTimerEvent *e );	
	// initializes the classes TrackMngr, Weather, Fpl, Coupling, Local Tag, 
	// Safetynets, infoMngr and the sequence list
	void CreateInstances();
	// initializes the classes TrackMngr, Weather, Fpl, Coupling, Local Tag, 
	// Safetynets, infoMngr and the sequence list
	void SetConnections();
	// sets the mouse pointer of radar windows
	void SetRadarCursors();
	
#if (defined Q_WS_X11)
	// receives the Event from X11
	bool x11EventFilter(XEvent * ev);
#endif																		
	
};
#endif // end of file

//--------------------------------< THE END >----------------------------------

