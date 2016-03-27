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
//  FILENAME: Track.h
//  TYPE    : header file
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
#if !defined(AFX_TRACK_H)
#define AFX_TRACK_H

//-INCLUDE FILES---------------------------------------------------------------

#include "DisplayElement.h"
#include "./CommonFiles/GraphicSymbol.h"

#include "AsdApp.h"
#include "TrackList.h"
#include "CommonDataTypes.h"
#include "RwdView.h"
#include "Wtc.h"
#include "Tssr.h"
#include "FlSelectOther.h"
#include "FlSelect.h"
#include "TssrOther.h"
#include "ArcTyp.h"
#include "Ades.h"
#include "Assr.h"
#include "Runway.h"
#include "Elements.h"


//-GLOBAL DATA-----------------------------------------------------------------

class CViewTrack;
class CField;
class CSymbol;
class CVector;
class CTrkMiles;
class CouplingData;
class CTrackServer;
class CThreadMngr;

class CTrack;
class CAsterixMsg;


//-CLASSES DEFINITIONS---------------------------------------------------------

class CTrackServer
{
	friend class CAsdApp;
	friend class CInfoMngr;
	friend class CEventMngr;
	friend class CTopLevel;
	friend class CSystemInfo;
	friend class CTrackMngr;
	friend class CTrack;


private:

	//variables
	BYTE m_Sic;
	BYTE m_Sac;
	char m_Serid;
	QString m_Name;
	int m_Period;
	QString m_Centre;
	int m_Dx;
	int m_Dy;
	int m_Rayon;
	CAsterixSource m_Src;
	QDateTime m_LastReceived252;
	bool m_Start252;	

	//methods
	virtual ~CTrackServer();
	CTrackServer();
	CTrackServer(CTrackServer& trk);	
	bool GetState();		
};


class CTagArray : public CArray<bool,bool>
{
public :

	//methods
	~CTagArray();
	QString FindTag(QString Arcid);
	CTagArray();
	CTagArray(CTagArray& tagArray);
};

class CTrackEventStub : public CLogStub
{
	friend class CTrack;
	friend class CEventMngr;
	friend class CTrackMngr;
	friend class CViewTrack;
	friend class CLabel;
	friend class CViewRoute;

private :

	//methods
	CTrackEventStub() {};
	~CTrackEventStub() {};

	//virtual methods
	virtual void OnTrackNeedUpdate(CTrack* pTrack, bool newtrack) =0;
	virtual void OnRemoveTrack(CTrack* pTrack, bool Cancelation) =0;
	virtual void OnStartAging(CTrack* pTrack) =0;
	virtual void OnTrackTSSR(CTrack* ptrack, QString Arcid, bool untag, QPoint* pnt) =0;
	virtual void OnAssumeDeassume(CTrack* pTrack) =0;
	virtual void OnCfl(CTrack* pTrack, int level, QString KdOfAppClearance) =0;
	virtual void OnATypWtc(CTrack* pTrack, QString adep, QString ades, QString arctyp, QString Wtc) =0;
	virtual void OnTrackTag(CTrack* ptrack, QString Arcid, QPoint* pnt) =0;
	virtual void OnTrackUntag(CTrack* ptrack, QString Arcid, bool MismatchSolve) =0;
	virtual void OnAfl(CTrack* pTrack, int level) =0;
	virtual void OnAssr(CTrack* pTrack, QString assr) =0;
	virtual void OnRfl(CTrack* pTrack, int level) =0;
	virtual void OnRwy(CTrack* pTrack, QString OldRwy, QString Rwy) =0;
	virtual void OnTrackHighlight(CTrack* pTrack, bool hl) =0;
	virtual void OnTrackEmergAck(CTrack* pTrack, bool ea) =0;
	virtual bool GetArcId(int key1, int key2, int key3, QString& Arcid) =0;
	virtual bool IsAppPos() =0;
	virtual void OnSelectRadarService(CTrackServer* pts) =0;
	virtual void OnFPLUpdateFromTrack(CString CsCallSign, int sic, int sac, int TrkNum, AssociationType tag) = 0;
	virtual AssociationType GetCouplingDataAssociationType(int sic, int sac, int TrkNum) = 0;
};

class CEventMngr;


// CDS BREAK NGT 2006/05/23 Multiple inheritance with QObject which is not an abstract class.
//							The alternative solution would be to inherit CObject from QObject
//							But the implementation of QT does not allow this design.  
class CTrackMngr : public CObject, public QObject
{
	friend class CEventMngr;
	friend class CTrack;
	friend class CViewTrack;
	friend class CLabel;
	friend class CViewRoute;
	friend class CLeader;
	friend class CVelocity;
	friend class CAdesDlg;
	friend class CATypDlg;
	friend class CAsdApp;
	friend class CAssr;
	friend class CVector;
	friend class CFlSelectOther;
	friend class CWtcDlg;
	friend class CFlSelectDlg;
	friend class CInfoMngr;
	friend class CRwdView;
	friend class CRwyDlg;
	friend class CTopLevel;
	friend class CSystemInfo;
	friend class CSystemMode;
	friend class CTssrDlg;
	friend class CTssrOtherDlg;

public:
	//This method returns the track object corresponding to the sic/sac and track number
	CTrack* GetTrack(BYTE sic, BYTE sac, int TrackNb);	
	//Allows to highlight a track presentation
	bool FindTrack(QString SearchArcid);


private:

	//variables
	//List of all the tracks 
	CTripleEntryList<CTrack> m_TrackList;

	//Menu associated to the track
	CAsdMenu m_TrackSourceMenu;
	//Time at which the last switch of radar service occured
	QDateTime m_SwitchTime;
	//Table of managed radar window views
	CArray<CView*,CView*> m_ViewTable;
	//Timer for the track ageing detection
	int m_TrkAgeId;
	//Timer for the blinking effect of data fields
	int m_FlashingId;
	//Timer for SPI blinking effect
	int m_FlashingSpiId;
	//Timer for DU alert detection
	int m_DuplicatSsrId;
	//Time of last synchronization
	QDateTime m_LastSyncTime;
	//Whether the unit for ROCD is ft/min or not
	bool m_RocdUnit;
	//Whether the setting of radar service is performed during CWP Init phase 
	bool m_InitTrackServer;
	//The name of the selected radar service name before failure
	QString m_OldSourceName;
	//Radar service priority order
	QString m_ServPrio;
	//Radar service order within the Radar service cascading menu
	QString m_ServMenu;	
	
	//Whether a pop-up window is displayed or not
	bool m_bDialogIsOn;

	//pointers	
	//Track Server object
	CTrackServer* m_pTagServer;
	//ADES pop-up window
	CAdesDlg* m_pAdesDialog;
	//ATYP pop-up window
	CATypDlg* m_pATypDialog;
	//ASSR pop-up window
	CAssr* m_pAssrDialog;
	//FL pop-up window
	CFlSelectDlg* m_pFlSelectDialog;
	//FL Other pop-up window
	CFlSelectOther* m_pFlSelectOtherDialog;
	//WTC pop-up window
	CWtcDlg* m_pWtcDialog;
	//RWY pop-up window
	CRwyDlg* m_pRwyDialog;
	//Track server object 
	CTrackServer* m_pTrkSrc;
	//ADES pop-up window
	CTssrOtherDlg* m_pTssrOtherDialog;
	//ADES pop-up window
	CTssrDlg* m_pTssrDialog;

	//Track menu context menu
	CAsdMenu* m_pTrackMenu;
	//Track for which the menu is invoked
	CTrack* m_pMenuCallingTrack;
	//The radar window from which the menu is invoked
	CView* m_pMenuCallingWnd;
	//STCA context menu
	CKeyList* m_pKeyListMenu;
	//Event Stub object
	CTrackEventStub* m_pHEvent;
	//List of radar services offline defined
	CTrackServer* m_pServers['Z'-'A'+1];

	//methods
	//Constructor
	CTrackMngr();
	//Copy constructor
	CTrackMngr(CTrackMngr& TrackMngr);
	//Destructor
	~CTrackMngr();
	//This method closes all the menus
	void RemoveMenu();	
	//This method switches the radar service to the next available
	void SwitchNextSource(bool MustBeFPLCoupled);
	//This method sets the radar service priority order
	void SetSourcePriority();
	//This method returns whether the radar service is in the radar service 
	//cascading menu
	bool IsRadarInMenu(char serid);
	//This method returns whether the radar window is within the radar service
	//priority scale
	bool IsRadarInPrioScale(char serid);
	//This method returns the highest available radar service
	CTrackServer* SearchHiAvRdSv();	
	//This method treats the menu item selections
	void OnCommand(UINT id,CObject *pDisplayWnd=NULL);
	//This method sets the status and presentation of the menu items
	void OnUpdateCmdUI(CCmdUI* pCmdUI);
	//This method adds a new radar window to be managed
	void AddView(CView* pView);	
	//This method empties the managed tracks list 
	void RemoveAllTracks();
	//This method treats the received ASTERIX messages
	void OnReceiveMsg(BYTE* pMsg,int Len,sockaddr_in* addr=NULL,int RcvIdent=0);
	//This method returns the currently selected radar service
	CTrackServer* GetTrackSource();	
	//This method sets the currently selected radar service
	void SetTrackSource(CTrackServer* pts);		
	//This method returns the time at which the display has been frozen
	QDateTime GetFrozenTime();
	//This method returns whether the ROCD unit must be Ft/min or not
	bool GetRocdUnit();
	//This method sets the unit of the ROCD data field
	void SetRocdUnit(bool rocdunit);
	//This method sets the optional data fields toggle status for the auxiliary radar windows
	//as for the main radar window
	void SetToggledOptionalFieldsAsMain();
	//This method sets the optional data fields toggle status for a radar window
	//as for the parent radar window 
	void SetToggledOptionalFieldsAsParent(CRwdView* parentView, CRwdView* daugtherView);
	//This method updates all the managed views
	void UpdateAllViews();	
	//This method retrieves the offline defined information in the version.ini file
	bool Read(CIniFile& IniFile, CInterfaceList& itfs, QString& Error);
	//Init function
	bool Init(CTrackEventStub* pHEvent, int TTTCycle);
	//This method returns the radar service corresponding to the name given 
	CTrackServer* GetServer(QString name);	
	//This method returns the radar service corresponding to the sic/sac given 
	CTrackServer* GetServer(int sic, int sac);
	//This method returns the radar service corresponding to the port number given 
	CTrackServer* GetServer(int port);
	//This method returns the currently selected radar service
	CTrackServer* GetCurrentServer();
	//Timer Event slot
	void timerEvent ( QTimerEvent * );
	//This method returns the callsign menu for the tarck
	CAsdMenu* GetTrackMenu();
	//This method empties the FPL list
	void RemoveAllFpl();
	//This method returns a track according to its position
	CTrack* FindTrackByPos(QPoint pos,CView* pView);
	//This method returns the radar service corresponding to a Id
	CTrackServer* GetServer(char serid);
	//This method removes a radar window from the managed radar window list
	void RemoveView(CView* pView);
	//This method returns whether the currently selected radar service is available
	bool IsCurrentSourceAv();
	//This method returns whether coupling is available for the currently selected
	//radar service
	bool IsCurrentSourceAvCpl();
	//This method returns the currently selected radar service name
	QString GetSourceName();

};


class CTrack: public CBaseTripleEntry<CTrack>
{
	friend class CEventMngr;
	friend class CViewTrack;
	friend class CLabel;
	friend class CSymbol;
	friend class CPastPos;
	friend class CVelocity;
	friend class CHalo;
	friend class CViewRoute;
	friend class CLeader;
	friend class CTrackMngr;
	friend class CTrkMiles;
	friend class CTrkMilesView;
	friend class CVector;

public:

	//methods
	//Destructor
	~CTrack();
	//virtual methods
	//This method is called to check if some track updates have not been missed
	virtual bool OnTimeOut(CTripleEntryList<CTrack>& List);
	//This method is called to update any link of flight plan or safety information 
	//following a remove link event.
	virtual bool OnRemoveLink(CTripleEntryList<CTrack>& List);
	//This method is called to update the track following a creation
	virtual bool OnCreateLink(CTripleEntryList<CTrack>& List);
	//This method is called in order to update the track with any last received 
	//information: coupling, safety
	virtual void OnResolveLink(CTripleEntryList<CTrack>& List);
	//This method returns the track position
	QPoint GetPos();
	//This method retrieves its track number
	void GetTrkNum(BYTE& sic, BYTE& sac, int& TrkNum);
	//This method associates the track to a TOOR Arrow
	void AssociateTOORArrow(CTOORArrow* pTOORArrow,bool asso);
	//This method returns the track presentation for a radar window
	CViewTrack* GetViewTrack(CView* pView);
	//Track Data object
	TrackData m_TD;
	//This method returns whether the track is assumed by the position or not
	bool IsAoc();


private:
	
	//variables
	//Enum to define the status that can have an alert
	typedef enum {NoAlert,Alert,AlertAck} AlertState;
	//Status of the track DU alert
	AlertState m_DUStatus;
	//Whether the track has already been treated while updating the DU alerts
	bool m_DUFlag;
	//The track list which are in DU alert with the track 
	QValueList<CTrack*> m_DUTrackList;
	//The CFL to be displayed, added to treat the RCFL rule 
	QString m_CflToBeDisplayed;
	//Whether the track was assumed or not during last update
	bool m_WasAOC;
	//Bool the track object has been deleted while an associated pop-up wqas opened
	//No treatment of this pop-up must be performed
	static bool m_Glob_ObjectDeletedOnModal;
	//Table of Alerts status
	AlertState  m_AlertState[StcaMsg+1];
	//Enum of the ageing process status
	enum {WATCH_AGING, START_AGING} m_agingState;
	//Whether the AL-S field must be inhibited
	bool m_AlertInhibition;
	//Heading of the track
	int  m_Heading;
	//Whether the track has entered in ageing process
	bool m_IsTrackAging;
	//Number of missed track updates
	int m_NbMissUpdate;
	//Current position of the tack
	QPoint m_CurPos;
	//Time of assume action
	QDateTime m_AssumeTime;
	//Time of SPI detection
    QDateTime m_SpiTime;
	//Whether a SPI alert is displayed or not
	bool m_IsSpiDisplayed;
	//Whether the track was displayed at last update 
	bool m_WasDisplayed;
	//Leader line orientation
	int m_LeaderPos;
	//Whether the track has the implicit focus
	bool m_Select;
	//The SPI indication value
	int m_Spi;
	//Whether the track update is the first or not
	bool m_Creation;
	//Whether the GRL is automatically displayed following assume action
	bool m_AutoRoute;
	//Whether the track display is being updated or not
	bool m_IsInUpdate;
	//Whether the update is coasted or not 
	bool m_IsCoasted;
	//Leader orientation associated to the runway
	int  m_RwyOrientation;
	//Color associated to the track according to the runway
	int  m_RwyColor;
	//Afterlows position 
	CArray<QPoint,QPoint> m_PastPos;
	//Whether the past position is coasted or not 
	CArray<bool,bool> m_PastCoasted;
	//Table of data field values
	CArray<QString,QString> m_DataTable;
	//Table or track elements
	CArray<CViewTrack*,CViewTrack*> m_ElemTable;
	//Table of attached R&B lines
	CArray<CVector*,CVector*> m_VectorTable;
	//Whether the STCA for the track is acknowledged or not

	//pointers
	//Event Stub object
	CTrackEventStub* m_pHEvent;
	//First associated pop-up window
	QDialog* m_pDialog;
	//Second associated pop-up window
	QDialog* m_pDialog2;
	//Attached Track Mile line tool
	CTrkMiles* m_pTrkMiles;
	//Attached TOOR Arrow tool
	CTOORArrow* m_pTOORArrow;

	//methods
	//Constructor
	CTrack(CTrackEventStub* pHEvent);
	//Copy constructor
	CTrack(CTrack& trk);		
	//This method Updates the track following a CAT 30 asterix message
	void UpdateFromRadar(CAsterixMsg* pMsg);
	//This method updates the track following a CAT 4 asterix message
	void UpdateFromSafety(bool UpdateTrk=true);
	//This method returns the track identifier
	QString GetArcId();
	//This method returns the track speed
	double GetSpeed();
	//This method associates the track to a TML tool
	void AssociateTrkMiles(CTrkMiles* pTrkMiles,bool asso = true);	
	//This method returns the important code associated to its status
	int IsImportant();
	//This method updates all associated track presentations
	void UpdateAllViews();
	//This method returns whether the track has a callsign / callsign tag mismatch 
	bool IsCallsignMismatch();
	//This method returns whether the SSR code is discrete or not 
	static bool IsSsrCodeDiscrete(int ssr);
	//This method returns the DU Acknowledge status of the track
	bool AcknowledgeDU();
	//This method returns whether the Tag eligibility is set to yes or not
	bool IsTaggedAllowed();
	//This method returns whether the CFL eligibility is set to yes or not
	bool IsCflAllowed();
	//This method returns whether the REP eligibility is set to yes or not
	bool IsRepAllowed();
	//This method returns whether the RFL eligibility is set to yes or not
	bool IsRflAllowed();
	//This method returns whether the WTC eligibility is set to yes or not
	bool IsWtcAllowed();
	//This method returns whether the ATYP eligibility is set to yes or not
	bool IsATypAllowed();
	//This method returns whether the ADES eligibility is set to yes or not
	bool IsAdesAdepAllowed();
	//This method returns whether the RWY eligibility is set to yes or not
	bool IsRwyAllowed();
	//This method returns whether the ASSR eligibility is set to yes or not
	bool IsAssrAllowed();
	//This method returns whether the Tag eligibility is set to yes or not
	bool IsAssumeDeAssumeAllowed();
	//This method returns whether the track is associated to a tool
	bool IsAssociatedToTool();
	//This method returns whether the Mismatch resolution eligibility is set to yes or not
	bool IsSolveCSMismatchAllowed();
	//This method treats the Click SB on the track identifier
	void ArcidClickSB(QPoint pnt, CObject *pWnd, CView* pView);
	//This method treats the Click WB on the track identifier
	void ArcidClickWB(QPoint pnt, CView* pView);	
	//This method treats the Click AB on the CFL data field
	void CflClickAB(QPoint pnt);
	//This method treats the Click AB on the AFL data field
	void AflClickAB(QPoint pnt);
	//This method treats the Click AB on the RFL data field
	void RflClickAB(QPoint pnt);
	//This method treats the Click AB on the RWY data field
	void RwyClickAB(QPoint pnt);
	//This method treats the Click AB on the TSSR data field
	void TssrClickAB(QPoint pnt);
	//This method is called to create a callsign tag
	void CreateTag(QPoint pnt);
	//This method is called to modify a callsign tag
	void ModifyTag(QPoint pnt);
	//This method treats the Click AB on the Mi data field
	void MiClickAB(CViewTrack* m_pParent);
	//This method treats the Click AB on the WTC data field
	void WtcClickAB(QPoint pnt);
	//This method treats the Click AB on the ATYP data field
	void ATypClickAB(QPoint pnt);
	//This method treats the Click AB on the ADES data field
	void AdesClickAB(QPoint pnt);
	//This method treats the Click AB on the ADEP data field
	void AdepClickAB(QPoint pnt);
	//This method treats the Click AB on the ASSR data field
	void AssrClickAB(QPoint pnt);
	//This method closes all menus and pop-ups associated to the track
	virtual void OnCloseMenu();
	//This method updates all the data field contents
	void UpdateDataTable();
	//This method resets the ageing status
	void ResetAging();
	//This method returns whether the track must enter ageing process or not
	bool AutoAging();	
	//This method treats the interactions with menus
	void OnCommand(UINT id, CObject* pWnd);
	//This method updates the menu items statuses
	void OnUpdateCmdUI(CCmdUI* pCmdUI);
	//This method treats the CLick AB on the identifier of a track
	void ArcidClickAB(QPoint pnt, CViewTrack* pParent);
	//This method switches the selection status of the track
	void Select(bool select,bool WithFpl=true,bool WithArrow=false);
	//This method associates a R&B line extremity to the track
	void AssociateVector(CVector* pVector,bool asso = true);
	//This method associates a CTOORArrow to the track
//	void AssociateTOORArrow(TOORArrow* pArrow,bool asso = true);

};


typedef enum {noelem=-1,lead,velocity,pastp,halo,symb,label} elemtypes;



typedef struct
{
	elemtypes type;
	int	colornb;
	int prio;
	int	fontnb;
	int	datanb;
	int	length;
	QPoint pos;
	int viewparam;
	QString Mnemo;
} ElemDef;


typedef CArray<ElemDef,ElemDef> LabelDef;

typedef CArray<LabelDef*,LabelDef*> LabelTable;

typedef struct
{
	int Angle;
	int Len;
} OrientationDef;


typedef struct
{
	CArray <OrientationDef,OrientationDef> OrientationTable;
	CArray<ElemDef,ElemDef> ElemDefTable;
	LabelTable LabelDefTable;
} TrackDef;

class CLabel: public CDisplayElement
{
	friend class CViewTrack;
	friend class CTrack;

private:

	//variables
	bool m_Select;
	int m_SelectFld;
	bool m_IsOnField;
	int m_CurDataBlock;
	int m_LastDataBlock;
	QPoint m_PosRect;
	CArray<QRect,QRect> m_RectTable;
	CArray<FullColor,FullColor> m_ClrTable;
	bool m_IsMoving;
	bool m_IsDragged;
	QPoint m_DownPoint;

	//pointers
	LabelDef* m_pDef;
	LabelTable* m_pDefTable;
	CAsdView* m_pView;
	CTrack* m_pTrack;
	CViewTrack* m_pParent;

	//methods
	CLabel();
	CLabel(CLabel& lab);
	~CLabel();
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int Prio);
	int GetPriority(QPoint point,bool Check=false);
	void OnUpdate();
	void SelectElement(bool Select);
	void OnLButtonDown(UINT nFlags,QPoint point);
	void OnMButtonDown(UINT nFlags,QPoint point);
	void OnRButtonDown(UINT nFlags,QPoint point);
	void OnMouseMove(UINT nFlags,QPoint point);
	void OnLButtonUp(UINT nFlags,QPoint point);
	bool IsOptionalForFullDB(int field);
	bool IsOptionalForLimitedDB(int field);
	bool IsOptionalForTaggedDB(int field);
	void RocdClickAB();
	bool IsSelectable(int DataNb);
	QString FillDataField(ElemDef CurElem);		
	bool IsOptionalFieldSelectable(int DataNb);
};

class CLeader: public CDisplayElement
{
	friend class CViewTrack;

public:

	//methods

	// constructors of CLeader class
	CLeader();
	CLeader(CLeader& lead);
	// destructor of CLeader class
	~CLeader();
	// Initialization of the CLeader Object
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	// draws or redraws the leader line
	void OnDraw(QPainter* pDC,int Prio);
	// returns a priority number according to the positions of the mouse pointer 
	// and the leader line
	int GetPriority(QPoint point,bool Check=false);
	// called each time an attribute of the class is modified or may be impacted
	void OnUpdate();
	// sets the selection status of the leader line to the one put in argument
	void SelectElement(bool Select);
	// called as soon as the user performs a click AB over a track leader line
	void OnLButtonDown(UINT nFlags,QPoint point);
	// returns the leader line length in pixels
	int GetLeaderLineLenght();

protected:

	//variables

	// color used for the leader line
	FullColor m_Color;
	// start position of the leader line
	QPoint m_PosInit;
	// end position of the leader line
	QPoint m_PosEnd;
	// set to true if the leader line is selected otherwise false
	bool m_Select;

	//pointers	
	// pointer of type ElemDef
	ElemDef* m_pDef;	
	// pointer of type CAsdView
	CAsdView* m_pView;
	// pointer of type CTrack
	CTrack* m_pTrack;
	// pointer of type CViewTrack to give access to parent class
	CViewTrack* m_pParent;

};
class CSymbol: public CDisplayElement
{
	friend class CViewTrack;

public:

	//methods
	CSymbol();
	CSymbol(CSymbol& symb);
	~CSymbol();
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int Prio);
	int GetPriority(QPoint point,bool Check=false);
	void OnUpdate();
	void SelectElement(bool Select);

protected:

	//variables
	FullColor m_Color;	
	bool m_Select;

	//pointers
	ElemDef* m_pDef;
	CAsdView* m_pView;
	CTrack* m_pTrack;
	CViewTrack* m_pParent;
	CGraphicSymbol* m_pSymbol;	
};

class CPastPos: public CDisplayElement
{
	friend class CViewTrack;
public:

	//methods
	CPastPos();
	CPastPos(CPastPos& pastpos);
	~CPastPos();
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int Prio);
	int GetPriority(QPoint point,bool Check=false);
	void OnUpdate();
	void SelectElement(bool Select);

protected:

	//variables
	bool m_Select;

	//pointers
	ElemDef* m_pDef;
	CAsdView* m_pView;
	CTrack* m_pTrack;
	CViewTrack* m_pParent;
	CGraphicSymbol* m_pSymbol;
	CGraphicSymbol* m_pCoastedSymbol;	
};

class CVelocity: public CDisplayElement
{
	friend class CViewTrack;

private:

	//methods
	CVelocity();
	CVelocity(CVelocity& vel);
	~CVelocity();
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int Prio);
	int GetPriority(QPoint point,bool Check=false);
	void OnUpdate();
	void SelectElement(bool Select);

protected:

	//variables
	QPoint m_PosInit;
	QPoint m_PosEnd;
	bool m_Select;

	//pointers
	ElemDef* m_pDef;	
	CAsdView* m_pView;
	CTrack* m_pTrack;
	CViewTrack* m_pParent;	
};

class CHalo: public CDisplayElement
{
	friend class CViewTrack;

public:

	//methods
	CHalo();
	CHalo(CHalo& halo);
	~CHalo();
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int Prio);
	int GetPriority(QPoint point,bool Check=false);
	void OnUpdate();
	void SelectElement(bool Select);

protected:

	//variables
	FullColor m_Color;
	bool m_Select;
	bool m_Forced;

	//pointers
	ElemDef* m_pDef;
	CAsdView* m_pView;
	CTrack* m_pTrack;
	CViewTrack* m_pParent;	
};


class CViewTrack: public CDisplayElement
{
	friend class CLabel;
	friend class CSymbol;
	friend class CPastPos;
	friend class CLeader;
	friend class CVelocity;
	friend class CHalo; 
	friend class CViewRoute;
	friend class CTrack;
	friend class CTrackMngr;
	friend class CSourceMngr;
	friend class CAsdApp;
	friend class CEventMngr;
	friend class CViewBeacon;
	friend class TrackControl;



private:

	//variables
	bool m_ToggleRocdUnitFtMin;		
	bool m_DBIsBeingDragged;
	bool m_Hooked;
	bool m_HookedSymbol;
	bool m_LabelDisplayed;
	bool m_SymbolDisplayed;
	CArray<BYTE,BYTE> m_ForcedData;
	int m_LeaderPos;
	bool m_ManualLeaderPos;
	bool m_Select; 
	bool m_bStationaryDb;
	FullColor m_Color;
	QRect m_StationaryDbRect;
	QRect m_LabelRect;
	QPoint m_PosRect;
	CArray<CDisplayElement*,CDisplayElement*> m_ElemTable;
	int m_LastSelElem;
	int m_ModifElem;
	QPoint m_CurPos;
	bool m_Extended;	
	bool m_IsToggled;
	int m_SelElem;

	//static variables
	static CArray<QFont,QFont> m_FontTable;
	static CArray<unsigned short,unsigned short> m_ColorTable;

	//pointers
	CAsdView* m_pView;
	CTrack* m_pTrack;
	static TrackDef* m_pTrackDef;

	//virtual methods
	virtual void OnCloseMenu();

	//static methods	
	static void RemoveTables();
	static void CreateTrackElements(TrackDef* pTrackDef);
	static void LimitedUncoupledDB(LabelDef* pLabelDef, TrackDef* pTrackDef);
	static void FullAccDB(LabelDef* pLabelDef, TrackDef* pTrackDef);
	static void FullAppDB(LabelDef* pLabelDef, TrackDef* pTrackDef);
	static void TaggedLimitedUncoupledDB(LabelDef* pLabelDef, TrackDef* pTrackDef);
	static void ExtendedAccDB(LabelDef* pLabelDef, TrackDef* pTrackDef);
	static void ExtendedAppDB(LabelDef* pLabelDef, TrackDef* pTrackDef);
	static void LimitedDB(LabelDef* pLabelDef, TrackDef* pTrackDef);
	static void ExtendedTaggedDB(LabelDef* pLabelDef, TrackDef* pTrackDef);
	static ElemDef FormatDef(int colornb,int prio,int fontnb,int datanb,int length,QPoint pos,int viewparam,QString Mnemo="");
	static void InitTables();
	static void ResetLcm();	

	//dynamics methods
	CViewTrack();
	CViewTrack(CViewTrack& viewtrk);
	~CViewTrack();	
	int RotateTrack(int pos=-1);
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int Prio);	
	void OnLButtonDown(UINT nFlags,QPoint point);
	void OnMButtonDown(UINT nFlags,QPoint point);
	void OnRButtonDown(UINT nFlags,QPoint point);
	void OnLButtonUp(UINT nFlags,QPoint point);
	void OnMouseMove(UINT nFlags,QPoint point);
	void SelectElement(bool Select);
	int GetPriority(QPoint point,bool Check=false);
	int IsImportant();
	void GetSymbRect(QRect& rect);	
	void HookLabel(bool hook);
	void ForceHalo(bool forced);
	bool IsForcedHalo();
	BOOL Intersect(QPoint p1, QPoint p2, QPoint p3, QPoint p4) const;
	int CCW(QPoint p0, QPoint p1, QPoint p2) const;
	void ComputeOffset(int angle,int Len,QRect LabelRect,QPoint PosRect,QPoint& pnt1,QPoint& pnt2,QPoint& pnttrack);
	void OnKeyDown();	
	void OnCommand(UINT id,CObject *pDisplayWnd=NULL);
	void OnUpdateCmdUI(CCmdUI* pCmdUI);
	bool IsLabelDisplayed();
	bool IsSymbolDisplayed();	
	int  ComputeDataBlock();
	bool IsLocalHighlight();
	FullColor ComputeColor(int typ);
	void OnUpdate();
	
};

#endif

//--------------------------------< THE END >----------------------------------

