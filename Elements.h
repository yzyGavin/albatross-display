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
//  FILENAME: elements.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the tools to display in the planview as Range and Bearing line,
//		Range Marker, Meteo display, Track Miles Line and Direction Finder Bearing.
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

#if !defined(AFX_ELEMENTS_H)
#define AFX_ELEMENTS_H

//-INCLUDE FILES---------------------------------------------------------------

#include "DisplayElement.h"
#include "./CommonFiles/GraphicSymbol.h"
#include "Interface.h"

//-GLOBAL DATA-----------------------------------------------------------------

class CTrack;
class CVectorView;
class CAsterixMsg;

//-CLASS DEFINITION------------------------------------------------------------



//Parent Class which manages the Range And Bearing Lines  
class CVector: public CObject
{
	friend class CVectorView;

public:

	static void Create(CView* pView);
	static void RemoveTables();
	static bool IsInCreation();
	static bool IsMaxNumberReached(CView* pView);
	static void RemoveView(CView* pView);
	CVector(CVector& pVector);
	~CVector();
	void Update();
	void UpdateTrk(CTrack* pTrack,bool cancel = false);

protected:

	enum {firstcreated,modify,created} m_State;
	QPoint m_FPoint;
	QPoint m_LPoint;
	int m_Bearing;
	int m_Distance;
	int m_Time;
	bool m_IsCreated;
	QPoint m_OldLPoint;
	static CArray<CVector*,CVector*> m_sVectorTable;

	CTrack* m_pFTrack;
	CTrack* m_pLTrack;
	CTrack* m_pOldLTrack;
	CVectorView* m_pElem;
	static CVector* m_psCurVector;

	CVector(CView* pView);
	void SetFirstPoint(QPoint point,CView* pView);
	void SetLastPoint(QPoint point,CView* pView,bool created,bool modcnl=false);	
};

//Class which manages the Range and Bearing Line display
class CVectorView: public CDisplayElement
{
	friend class CVector;

private:


	bool m_Select;
	QPoint m_FPoint;
	QPoint m_LPoint;
	QRect m_TextRect;


	CView* m_pView;
	CVector* m_pVector;


	CVectorView();
	CVectorView(CVectorView& pVectorView);
	~CVectorView();
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int prio);
	void OnUpdate();
	int GetPriority(QPoint point,bool Check=false);
	void SelectElement(bool Select);
	void OnMouseMove(UINT nFlags,QPoint point);
	void OnLButtonDown(UINT nFlags,QPoint point);
	void OnMButtonDown(UINT nFlags,QPoint point);
	void OnRButtonDown(UINT nFlags,QPoint point);
	void OnKeyDown();
};


//Parent Class which manages the Creation of an Auxiliary Radar Window  
class CNewRangeView;

class CNewRange: public CObject
{
	friend class CNewRangeView;

public:

	//static methods
	static void Create(CView* pView);
	static void RemoveView(CView* pView);
	static void InitTables();
	static void RemoveTables();
	static bool IsInCreation();

	//dynamic methods
	CNewRange(CView* pView);
	CNewRange(CNewRange& pNewRange);
	~CNewRange();
	
private:

	//variables
	enum {firstcreated,modify,created} m_State;
	QPoint m_InitPoint;
	QPoint m_EndPoint;

	//pointers
	CNewRangeView* m_pElem;
	static CNewRange* m_psCurRange;

	//dynamic methods
	void SetFirstPoint(QPoint point,CView* pView);
	void SetLastPoint(QPoint point,CView* pView,bool terminated,bool cancel=false);
};

//Class which manages the creation of the Radar Window
class CNewRangeView: public CDisplayElement
{
	friend class CNewRange;

private:

	//variables
	QPoint m_InitPoint;
	QPoint m_EndPoint;

	//pointers
	CNewRange* m_pNewRange;
	CView* m_pView;

	//dynamic methods
	CNewRangeView(CNewRangeView& pNewRangeView);
	CNewRangeView();	
	~CNewRangeView();
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int prio);
	void OnUpdate();
	void OnMouseMove(UINT nFlags,QPoint point);
	void OnLButtonDown(UINT nFlags,QPoint point);
	void OnMButtonDown(UINT nFlags,QPoint point);	
	void OnKeyDown();	
};

//Parent Class which manages Meteo Data
class CMeteoView;
class CWeatherMngr;

class CMeteoData: public CObject
{
	friend class CMeteoView;
	friend class CWeatherMngr;

public:

	//methods
	CMeteoData();
	CMeteoData(CMeteoData& pmeteo);
	~CMeteoData();
	void UpdateFromRadar(CAsterixMsg* pMsg);
	void DisplayMeteo(bool display);
	bool IsMeteoDisplayed();
	void Init();

private:

	//variables
	typedef struct
	{
		int Level;
		QPoint pos1;
		QPoint pos2;
		QPoint pos3;
		QPoint pos4;
	} m_VectorElem;
	typedef struct
	{
		CArray<m_VectorElem,m_VectorElem> VectorTable;
	} m_Vectors;
	m_Vectors m_VectorTable[2];
	QPoint m_Radar;
	int m_CurActive;
	int m_Scale;
	bool m_RecptStarted;
	int m_ColorLevelNb[8];
	CArray<CMeteoView*,CMeteoView*> m_ElemTable;
	bool m_IsDisplayed;
};
//Class which manages the display of the Meteo information
class CMeteoView: public CDisplayElement
{

public:

	//static methods
	static void Setup(QString IniName,QString Section,bool save);

	//dynamic methods
	CMeteoView(int Prio);
	CMeteoView(CMeteoView& pMeteoView);
	~CMeteoView();
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int prio);
	void OnUpdate();
	
protected:

	//variables
	CArray<CMeteoData::m_VectorElem,CMeteoData::m_VectorElem> m_VectorTable;
	int m_Prio;

	//pointers
	CView* m_pView;
	CMeteoData* m_pMeteo;	
};

// associates the sector to the weather radar service Id
struct Secto_WthRdServ {
	// Name of the sector
	QString Sector;
	// weather radar service id
	QString Id;
	};

// CDS BREAK JPM 2006/05/11 Multiple inheritance with QObject which is not an abstract class.
//       The alternative solution would be to inherit CObject from QObject
//       But the implementation of QT does not allow this design. 
class CWeatherMngr : public CObject, public QObject
{
	friend class CMeteoData;

public:


	CWeatherMngr();
	CWeatherMngr(CWeatherMngr& pWeatherMngr);
	~CWeatherMngr();
	void timerEvent ( QTimerEvent * );
	bool IsWeatherOn();
	bool IsWeatherFailed();
	void DisplayWeather(bool on);
	void AddView(CView* pView);
	void RemoveView(CView* pView);
	void OnReceiveMsg(BYTE* pMsg,int Len,sockaddr_in* addr=NULL,int RcvIdent=0);
	bool Read(CIniFile& IniFile, CInterfaceList& itfs, QString& Error);
	void Init();
	void OpenCurrentWeatherSocket();

private:


	BYTE m_Sic;
	BYTE m_Sac;
	double m_Period;
	CArray<Secto_WthRdServ,Secto_WthRdServ> m_SectoSocketTable;	
	CMeteoData m_CurMeteo;
	CArray<CView*,CView*> m_ViewTable;
	bool m_IsFailed;
	int m_iCurrentSocket;
	int m_Timer1Id;
	int m_Timer2Id;
	int m_WthrPrio;

	
	CAsterixSource* m_pServers['Z'-'A'+1];
};

//Parent Class which manages the Range Markers  
class CRangeMarkView;
class CRangeMark: public CObject
{
	friend class CRangeMarkView;

public:

	//static methods
	static void Create();
	static void AddView(CView* pView);
	static void RemoveView(CView* pView);
	static CRangeMarkView* GetCRangeMarkView(CView* pView);
	static bool IsRangeMarkInModif();

	//methods
	CRangeMark();
	CRangeMark(CRangeMark *pRangeMark);
	~CRangeMark();
	
private :

	//variables
	CArray<CRangeMarkView*,CRangeMarkView*> m_ElemTable; // list of elements
	static CArray<CView*,CView*> m_sViewTable; // list of views
	static CArray<CRangeMark*,CRangeMark*> m_sRMarkTable; // list of RMark
};

//Class which manages the Range Markers display  
class CRangeMarkView: public CDisplayElement
{
public:


	//methods
	CRangeMarkView();
	CRangeMarkView(CRangeMarkView& pRangeMarkView);
	~CRangeMarkView();
	void SetResizeOffset(QPoint point);
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int prio);
	void OnUpdate();
	int GetPriority(QPoint point,bool);
	void SelectElement(bool Select);
	void OnMouseMove(UINT,QPoint point);
	void OnLButtonDown(UINT,QPoint point);
	void OnMButtonDown(UINT nFlags,QPoint);
	void SetInitialPosition(QRect rect);

	//static methods
	static void SetOrientation(bool Orientation);
	static bool GetOrientation();
	static void SetRange(int range);
	static int  GetRange();
	static void InitTables();
	static void SetNbRangeMarker(int nb);
	static int GetNbRangeMarker();

	bool IsMove();

private :

	//variables
	bool m_OnMove;
	QPoint m_CenterPoint; // center point coordinates
	QPoint m_GripPoint; // grip point coordinates
	bool m_Select; // selection flag 
	static int m_sNbRangeMarker; // number of range marker
	static int m_sInterline; // number of interline
	static int m_sRange; // range
	static int m_sOrientation; // orientation
	
	//pointers
	CView* m_pView; // pointer on the current view
	CRangeMark* m_pRangeMark; // pointer on the range marker

	//methods
	void OnKeyDown();	
};


//Parent Class which manages the Track Mile Lines  
class CTrkMilesView;

// CDS BREAK JPM 2006/05/11 Multiple inheritance with QObject which is not an abstract class.
//       The alternative solution would be to inherit CObject from QObject
//       But the implementation of QT does not allow this design. 
class CTrkMiles: public CObject, public QObject
{
	friend class CTrkMilesView;

public:

	//methods
	~CTrkMiles();
	void UpdateTrk(CTrack* pTrack,bool cancel = false);	

	//static methods
	static bool IsInCreation();	
	static void RemoveView(CView* pView);
	static void Create(CView* pView,CTrack* pTrack);

private:

	//variables
	int m_Distance;
	int m_TrkMileTimer; 
	QDateTime m_FinishTime;
	enum {modify,created} m_State;
	CArray<QPoint,QPoint> m_PointTable;
	static CArray<CTrkMiles*,CTrkMiles*> m_sTrkMilesTable;

	//pointers
	CTrack* m_pTrack;
	CTrkMilesView* m_pElem;	
	static CTrkMiles* m_psCurTrkMiles;

	//methods
	CTrkMiles(CView* pView);
	CTrkMiles(CTrkMiles& pTrkMiles);
	void AddPoint(QPoint point,bool next,bool created,CView* pView);
	void timerEvent(QTimerEvent* e);

	//static methods
	static void RemoveTables();
};

//Parent Class which manages the Track Mile Lines display
class CTrkMilesView: public CDisplayElement
{
	friend class CTrkMiles;

protected:
	~CTrkMilesView();

private:

	//pointers

	//methods
	CTrkMilesView();
	
	CTrkMilesView(CTrkMilesView& pTrkMilesView);
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int);
	void OnUpdate();
	int GetPriority(QPoint point,bool Check=false);
	void SelectElement(bool Select);
	void OnMouseMove(UINT,QPoint point);
	void OnLButtonDown(UINT,QPoint point);
	void OnMButtonDown(UINT,QPoint);
	void OnRButtonDown(UINT,QPoint point);	
	CView* GetView();
private:
	
	//variables
	QRect m_TextRect;
	bool m_Select;
	CArray<QPoint,QPoint> m_PointTable;

	//pointers
	CView* m_pView;
	CTrkMiles* m_pTrkMiles;

	//methods
	void OnKeyDown();
};



//Parent Class which manages the TOOR Arrows 
	typedef enum {TOOR_N,TOOR_NE,TOOR_E,TOOR_SE,TOOR_S,TOOR_SW,TOOR_W,TOOR_NW} TOORDirection;

class CTOORArrowView;
class CTOORArrow: public CObject
{
	friend class CTOORArrowView;

public:

	//static methods
	static void Create(CTrack* pTrack);
	static void AddView(CView* pView);
	static void RemoveView(CView* pView);
	static void UpdateFromView(CView* RwdWiew);
	static void RemoveAllArrows();
	void CancelTOORArrow(CTrack* pTrack);
	void UpdateTrk(CTrack* pTrack,bool update);

	//methods
	CTOORArrow(CTrack* pTrack);
	CTOORArrow(CTOORArrow *pTOORArrow);
	~CTOORArrow();
	
private :

	//pointers
	CTrack* m_pTrack;

	//variables
	CArray<CTOORArrowView*,CTOORArrowView*> m_ElemTable; // list of elements
	static CArray<CView*,CView*> m_sViewTable; // list of views
	static CArray<CTOORArrow*,CTOORArrow*> m_sTOORArrowsTable; // list of RMark
};

//Parent Class which manages the TOOR Arrow display  
class CTOORArrowView: public CDisplayElement
{
public:

	QPoint m_TrkOutOfRgePos;

	//methods
	CTOORArrowView(QPoint trkpos);
	CTOORArrowView(CTOORArrowView& pTOORArrowView);
	~CTOORArrowView();
	void SetResizeOffset(QPoint point);
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnDraw(QPainter* pDC,int prio);
	void OnUpdate();
	int GetPriority(QPoint point,bool);
	void SelectElement(bool Select);
	void OnLButtonDown(UINT,QPoint point);
	QPoint SetCenterArrowPos();
	void UpdateFromTrack(QPoint point);
	void UpdateFromRadarView();


private :

	//variables
	QPoint m_CenterPoint; // center point coordinates
	bool m_Select; // selection flag 
	TOORDirection m_sOrientation; // orientation
	TOORDirection GetTOORDirection();

	//pointers
	CView* m_pView; // pointer on the current view
	CTOORArrow* m_pTOORArrow; // pointer on the range marker

};



#endif  // end of file

//--------------------------------< THE END >----------------------------------

