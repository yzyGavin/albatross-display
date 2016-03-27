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
//  FILENAME: FlightPlan.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the management of flight plan data received from CCS
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


#if !defined(AFX_FLIGHT_PLAN_H)
#define AFX_FLIGHT_PLAN_H

#include "CommonDataTypes.h"
#include "Geo.h"
#include "View.h"
#include "DisplayElement.h"
#include "LineView.h"


//-GLOBAL DATA-----------------------------------------------------------------

class CFlightPlan;
class CTrack;
class CViewTrack;
class CViewPln;
class CLineView;
class CLineElement;
class CFplField;

	enum {Full =0, Coupled, ModeS, Tag, Bin};


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-START OF CODE---------------------------------------------------------------


class CFPLEventStub : public CLogStub
{
public :

	//methods
	//Constructor
	CFPLEventStub() {};
	//Destructor
	~CFPLEventStub() {};

	//virtual methods
	//method to create a FPL
	virtual void OnCreateFPL(int fplid) = 0;
	//method to update a FPL
	virtual void OnUpdateFPL(int fplid) = 0;
	//method to erase a FPL
	virtual void OnEraseFPL(int fplid, CFlightPlan* pFPL) = 0;
};


//FPL Manager Class
class CFplMngr : public CObject
{
	friend class CFlightPlan;

public:

	//methods

	//Constructor
	CFplMngr();
	//Copy Constructor
	CFplMngr(const CFplMngr & fplmngr);
	//Destructor
	~CFplMngr();

	//Init function
	void Init(CFPLEventStub* pHEvent);	
	//Returns the FPL object according to the Callsign
	int GetFpl(QString Cls);
	//Returns the FPL object according to the SSR code
	CFlightPlan* GetFpl(int ssr);
	//Returns the FPL object according to the FPL Id
	CFlightPlan* GetFplById(int FplId);
	//Removes all the FPL objects managed by the FPL Manager
	void RemoveAll();
	//Creates a new flight plan following the reception 
	void CreateFromTrack(int FplId, QString CsCallSign, int Sic, int Sac, int TrkNum);
	//Removes a flight plan following the deletion of the associated track
	void RemoveFromTrack(int trknum);
	// allows to add a view to the Fpl list
	void AddView(CView* pView);
	// allows to remove a view to the Fpl list
	int RemoveView(CView *pView);
	//Displays the Full FPL List
	void DisplayFPLFullListWindow();
	//Removes the Full FPL List
	void RemoveFPLFullListWindow();
	//Displays the Cupled FPL List
	void DisplayFPLCoupledListWindow();
	//Removes the Coupled FPL List
	void RemoveFPLCoupledListWindow();
	//Displays the Mode S Tagged FPL List
	void DisplayFPLModeSListWindow();
	//Removes the Mode S Tagged FPL List
	void RemoveFPLModeSListWindow();
	//Displays the Tagged FPL List
	void DisplayFPLTagListWindow();
	//Removes the Tagged FPL List
	void RemoveFPLTagListWindow();
	//Displays the BIN FPL List
	void DisplayFPLBinListWindow();
	//Removes the BIN FPL List
	void RemoveFPLBinListWindow();
	//Empties all the lists
	void EmptyAllLists();
	//Set the frozen captions for the FPL Lists
	void FreezeAllLists(bool frozen);
	//returns the pointer fo the Full FPL List
	CLineView* GetFullListDlg();
	//returns the pointer fo the Coupled FPL List
	CLineView* GetCoupledListDlg();
	//returns the pointer fo the Mode S Tagged FPL List
	CLineView* GetModeSListDlg();
	//returns the pointer fo the Tagged FPL List
	CLineView* GetTaggedListDlg();
	//returns the pointer fo the BIN FPL List
	CLineView* GetBinListDlg();
	//Updates the font to be used for every FPL Lists
	void SetFontForAllEntries(int Size);
	//Returns the current font size
	int GetListsFontSize();
	//method called to fill the different lists with the available FPLs
	void FillFPLLists();



private:
	
	//variables
	//Whether the FPL Lists display is forzen or not
	bool m_Frozen;
	//Table of FPL objects
	CArray<CFlightPlan*,CFlightPlan*> m_FplTable;
	//pointers
	CFPLEventStub* m_pHEvent;
	//pointer to FPL Lists
	//Full FPL list
	CLineView *m_pFplFullList;
	//Coupled FPL List
	CLineView *m_pFplCoupledList;
	//Mode S Tagged FPL List
	CLineView *m_pFplModeSList;
	//Tagged FPL List
	CLineView *m_pFplTagList;
	//BIN FPL List
	CLineView *m_pFplBinList;
	//Size of the font to be applied
	int m_iFontSize;
	// table of type CView used to display the Fpl alerts
	CArray<CView*,CView*> m_ViewTable;

	//methods
	//methods to destroy the FPL objects which have not been updated since a predefined time
	void FPLAgingCkeck();


};


class CEventMngr;
class CViewFpl;

// CDS BREAK JPM 2006/05/11 Multiple inheritance with QObject which is not an abstract class.
//       The alternative solution would be to inherit CObject from QObject
//       But the implementation of QT does not allow this design. 
class CFlightPlan: public CObject, public QObject
{
	friend class CEventMngr;
	friend class CFplMngr;
	friend class CTrack;
	friend class CSymbol;
	friend class CViewTrack;
	friend class CLabel;
	friend class FlSelectDlg;
	friend class CViewFpl;


public :
	bool UpdateDataFromTrack(QString CsCallSign, int Sic, int Sac, int TrkNum);
	//FPL Number
	int m_FPLNb;
	CArray<bool,bool> m_PresenceInListTable;

private:

	//variables
	//First Alternative runway 
	QString  m_Altrnt1;
	//Second Alternative runway
	QString  m_Altrnt2;

	//dynamic methods
	//Constructor
	CFlightPlan(int FPLNb);
	//Copy constructor
	CFlightPlan(CFlightPlan& pFpl);
	//Destructor
	~CFlightPlan();
	
protected:

	//variables
	//Flight Plan callsign
	QString  m_Arcid;
	//Flight Plan ADEP
	QString  m_Adep;
	//Flight Plan ADES
	QString  m_Ades;
	//Flight Plan Kind of approach clearance
	QString	 m_AppKind;
	//Flight Plan End of block time
	QString  m_Eobt;
	//Flight Plan ATYP
	QString  m_ArcTyp;
	//Flight Plan flight rule category
	QString  m_FltRul;
	//Flight Plan WTC
	QString  m_Wtc;
	//Flight Plan Actual speed
	QString  m_ASpeed;
	//Flight Plan AFL
	QString  m_Afl;
	//Flight Plan route
	QString  m_Route;
	//Flight Plan TSSR
	QString  m_Ssr;
	//Flight Plan ASSR
	QString  m_ASsr;
	//Flight Plan CFL
	QString  m_Cfl;
	//Flight Plan RFL
	QString  m_Rfl;
	//Flight Plan departure runway
	QString  m_DepRwy;
	//Flight Plan arrival runway
	QString  m_ArrRwy;
	//Flight Plan Current fix of the route
	QString  m_CurFix;
	//Occurence of the flight plan current fix
	int		 m_CurFixOccur;
	//Flight Plan Exit Point
	QString  m_Xpt;
	//Flight Plan Attitude intention indicator
	//Flight Plan exit flight level
	QString  m_Xfl;
	//Flight Plan Exit Point
	QString  m_Etx;
	//Flight Plan RVR
	QString  m_Rvr;
	//Flight Plan 8,33 indication
	QString  m_833;
	//Flight Plan RVSM indication
	QString  m_Rvsm;
	//Flight Plan status
	QString  m_State;
	//Flight Plan SID
	QString  m_Sid;
	//Flight Plan Calculated end of block time
	QString  m_Cobt;
	//Flight Plan Sectorization
	QString  m_Sector;
	//Flight Plan Hold FIx
	QString  m_HoldFix;
	//Flight Plan Hold TIme
	QString  m_HoldTime;
	//Flight Plan category
	QString  m_FplCat;
	//Flight Plan counter
	int		 m_Counter;
	//Flight Plan received time
	QDateTime		m_LastReceivedTime;
	//Flight Plan Next sectors to be overflown
	QString			m_NextSectorNameOverFlown;
	//Table of entries for teh flight plan displayed in the different FPL Lists
	CArray<CViewFpl*,CViewFpl*> m_pElemTable;
	//Table of data available in the different FPL Lists
	CArray<QString,QString> m_DataTable;

};

//Class for one entry within the FPL Lists
class CViewFpl : public CLineElement
{
 friend class CFlightPlan;
 friend class CFplMngr;
public:
 // structure that gives information on a field for an alert list
 typedef struct
 {
  //Data field number
  int      DataNb;
  //Font number
  int      FontNb;
  //Color number
  int   ColorNb;
  //Color for the field with implicit focus
  int   SelectColorNb;
  //Font for the field with implicit focus
  int   SelectFont;
  //Style to draw the data field
  UINT  Style;
 } FplFieldDef;




 //methods
 //Accessor to m_FontTable attribute
 QFont GetFontTableAt(int index);
 //Accessor to m_Select attribute
 bool GetSelect();
 //Accessor to m_FirstElemSelected attribute
 bool GetFirstElemSelected();
 // initializes the field elements of the FPL list and loads the fonts
 static void InitTables();
 // pointer of type CAlarmInt that give information on a conflict
 CFlightPlan* m_pFpl;
 //Sets the new line number for the entry
 void SetLine(int Line);



private:

 //variables
 // table containing the data field for alert lists 
 CArray<QString,QString> m_DataTable;
 // indicates if an alarm is selected or not
 bool m_Select;
 // indicates if an element in the alert list is selected
 bool m_FirstElemSelected;
 // value of the track ID associated to the selected alarm in the alert list
 UINT m_iTrackID;
 // line associated to the selected alarm in the alert list
 int  m_Line;
 // table of type containing the fields for the alert list
 CArray<CFplField*,CFplField*> m_FieldTable;
  // table of fint used in the alert list
 static CArray<QFont,QFont> m_FontTable;
 // array that gathers information of type CaFieldDef
 static CArray<FplFieldDef,FplFieldDef> m_FieldDef;

 //pointers

 // pointer of type CLineView
 CLineView* m_pView;

 //static methods

 // allows to sort the alert list
 static int FplCompare(long lParam1, long lParam2,long lParamSort,CView* pView);

 //methods

 // constructors of CViewCa class
 CViewFpl();
 // destructor of CViewCa class
 ~CViewFpl();
 // creates the line fields for the alarm in the alert list
 void Init(LPVOID pData,CView *pView,CDisplayElement* pParent=NULL);
 //called when the user clicks mouse AB
 void OnLButtonDown(UINT nFlags,QPoint point);
 // called when the user clicks mouse SB 
 void OnRButtonDown(UINT nFlags,QPoint point);
 // called when the user clicks mouse WB
 void OnMButtonDown(UINT nFlags,QPoint point);
 // draws the line and manages the drawing of each line fields for the alert list
 void OnDraw(QPainter* pDC,int Line,int Col); 
 // manages the selected track from the Radar Window and the selected 
 // element from the list line
 void SelectElement(bool select);
 // sets the line of an alert list in a selected status
 void SetSelectedElement(bool select); 
 // returns the rectangle size of line field
 QSize GetSize(int Col); 
// removes the fonts used in the alert list
 void RemoveTables();
 // changes the font size in an alert list
 void SetFont(int iSize);
 // sets the field color on an alert list
 void SetColor(int iIndex, int iColor);
 // update the field of the line view according to the data line view
 void OnUpdate();
 
};


//Class for the data fields objects displayed for the entries within a FPL List
class CFplField : public CLineElement
{
 friend class CViewFpl;
public:

 //constructors of CFplField class
 CFplField();
 // destructor of CFplField class
 ~CFplField();
 // draws a field of the line
 void OnDraw(QPainter* pDC,int Line,int Col);
 // initializes the line field
 void Init(LPVOID pData,CView *pView,CDisplayElement* pParent=NULL);
 // returns the rectangle size of line field
 QSize GetSize(int Col);
 //Accessor to m_Data
 void SetData(QString data);

protected:

 // content of the field
 QString m_Data;
 // pointer of type CLineView
 CLineView* m_pView;
 // give access the the parent class CViewCa
 CViewFpl* m_pParent;
 // field definition of type CaFieldDef
 CViewFpl::FplFieldDef* m_pFieldDef;
};



#endif  // end of file

//--------------------------------< THE END >----------------------------------

