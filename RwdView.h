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
//  FILENAME: RwdView.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CRwdView which aims to manage the radar window
//		properties (range, center, size, filters...)
//		This includes the radar menu, status bar, interactions, keyboard keys,
//		and important track positionning.
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

#if !defined(AFX_TPVIEW_H__BF475F90_8ADB_11D4_9B11_00104BF17B60__INCLUDED_)
#define AFX_TPVIEW_H__BF475F90_8ADB_11D4_9B11_00104BF17B60__INCLUDED_

//-INCLUDE FILES---------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AsdView.h"
#include "AsdMenu.h"
#include "TrackControl.h"
#include "HeightFilterDlg.h"
#include "StatusBar.h"

//-GLOBAL DATA-----------------------------------------------------------------

typedef enum {MainFocus, Sec1Focus, Sec2Focus, StcaListFocus, OtherFocus} WndFocus;

//-CLASSES DEFINITIONS---------------------------------------------------------


class CRwdView : public CAsdView
{
	friend class CAsdApp;
	friend class CColorDlg;
	friend class CVector;
	friend class CTrkMilesView;
	friend class CTrkMiles;
	friend class CRangeMarkView;
	friend class CRangeMark;
	friend class CTOORArrowView;
	friend class CTOORArrow;
	friend class CWeatherMngr;
	friend class CMeteoData;
	friend class CNewRangeView;
	friend class CNewRange;
	friend class CVectorView;
	friend class CImportantTrack;
	friend class CHeightFilterDlg;
	friend class CEventMngr;
	friend class CTopLevel;
	friend class CTrackMngr;
	friend class CTrack;
	friend class CViewTrack;
	friend class CLabel;
	friend class CPastPos;
	friend class CVelocity;
	friend class CVisuMap;
	friend class TrackControl;

public:

	//variables
	int m_ReducePanning;

	// enumerates all the ASD filters
	typedef enum {
		filt_lower_on,filt_lower_val,filt_upper_on,filt_upper_val,filt_dep_on,filt_dep_val,
		filt_prim_on,filt_vfr_on,filt_mil_on,filt_db_on,
		pvect_uncpl,pvect_coupl,pvect_time,pvect_length,hist_length,halo_length,font_size,
		fld_fdb_modes,fld_fdb_atyp,fld_fdb_rwy,fld_fdb_gsp,fld_fdb_rocd,fld_fdb_rvr,fld_fdb_ades,fld_fdb_sid,
		fld_ldb_modes,fld_ldb_atyp,fld_ldb_rwy,fld_ldb_gsp,fld_ldb_rocd,fld_ldb_rvr,
		fld_unc_modes,fld_unc_gsp,fld_unc_rocd,
		fld_tag_modes,fld_tag_atyp,fld_tag_gsp,fld_tag_rocd
	} m_ViewParams;
	// table to store all filter values
	int m_Params[fld_tag_gsp+1];

	//static methods

	//due to namespace CColorMngr this methods must be public
	// called to update the presentation of every graphical elements
	// associated to every available radar window
	static void UpdateAllViews();
	
private:

	//variables

	// enumerates the different mode for the mouse: normal zooming and panning
	enum ZoomModes {Normal,Zoom,Panning} m_Mode;
	// value before invoking Quick Look for up filter
	int m_PrevUpFilter;	
	// value before invoking Quick Look for down filter
	int m_PrevDownFilter;
	// timer for Quick Look +
	int m_QlPlusTimer;
	// timer for Quick Look -
	int m_QlMinusTimer;
	// boolean value before collapsing the main radar window
	bool m_MainWasCollapsible;
	// default range value
	int m_DefaultRange;
	// default range center
	QPoint m_DefaultCentre;
	// stores the last point in the radar window to know if the latitude and 
	// longitude in the radar windor has to be updated
	QPoint m_LastCursorPos;
	// timer used for implicit focus
	int m_TimerId;
	// boolean to indicate that the ASD is in mode panning
	bool m_FirstPanningMove;
	
	// value before invoking Quick Look for primary filter
	int m_PrevPrimFilter;
	// value before invoking Quick Look for datablock filter
	int m_PrevDataFilter;
	// timer started when mouse move on a radar window
	int m_MouseTimerId;
	// object of type CAsdMenu for context menu
	CAsdMenu m_RadarMenu;
	// table of type CDisplayElement
	CArray<CDisplayElement*,CDisplayElement*> m_ElemTable;	
	// object of type TrackControl
	TrackControl m_TrackControl;
	// object of type CHeightFilterDlg
	CHeightFilterDlg m_HeightFilter;

	//static variables

	// indicates which window has the focus
	static WndFocus m_sWndFocus;
	// set to true to send the mouse move event to every opened radar window 
	// so that a radar window not having the mouse pointer focus can process the event
	static bool m_bsForcedMove;
	// indicates if radar windows are independant or not
	static bool m_sIsIndependent;
	// indicates if the maps have changed
	static bool m_sIsOnMapReload;
	// called to update the status bar content of all the available radar windows
	static void UpdateAllStatusBar();
	// returns whether one of the opened radar window is in mode panning or zooming or not
	static bool IsInViewModif();
	// called in order to close any opened radar menu
	static void RemoveAllRadarMenu();
	// called to reload the radar menu Map cascading menu for every radar window
	static void ReloadAllMapMenu();
	// called to sets the Offline Maps cascading menu availability
	// to the one put in argument
	static void SetMapsAvailable(bool IsAllowed);

	//pointers

	// pointer of type CDisplayElement for the current element used
	CDisplayElement* m_pCurElem;
	// pointer of type CDisplayElement for the modified element
	CDisplayElement* m_pModifElem;
	// pointer of type CStatusBar
	CStatusBar* m_pStatusBar;

	//static pointers	

	// pointer of type CRwdView for main radar window
	static CRwdView* m_psMainWnd;
	// pointer of type CRwdView for the first secondary radar window
	static CRwdView* m_psSecWnd1;
	// pointer of type CRwdView for the second secondary radar window
	static CRwdView* m_psSecWnd2;

	

	// destructor of the class
	virtual ~CRwdView();
	// called following the creation of a radar window to initialize
	// its several settings with default settings before the offline defined ones are
	// loaded or retrieved from a custom set or the parent view
	virtual void OnInitialUpdate();

	//static methods
	
	// sets the radar window mode dependant or not
	static void SetIndependentMode(bool depmode);
	// retrieves the settings of the main radar window to apply them
	// to the view put in argument
	static void UpdateAuxWndDep(CRwdView* pView);
	// closes all the opened Height Filter windows
	static void CloseAllHFW();
	// called to refresh the whole display of every available radar window
	static void InvalidateAllViews();	
	// called in order to send the mouse move event to every opened radar window 
	// so that a radar window not having the mouse pointer focus can process the event
	static void ForceMouseMoveEvent( );	
	// returns whether the radar windows are independent or not
	static bool GetIsIndependent();	
	// returns the pointer to the first auxiliary radar window object
	static CRwdView* GetSecWnd1();
	// returns the pointer to the second auxiliary radar window object
	static CRwdView* GetSecWnd2();

	//methods

	// constructors of CRwdView class 
	CRwdView();
	CRwdView(CRwdView& pTpView);
	// called to draw the radar window content
	void OnDraw(QPainter* pDC);
	// Upon the selection of an available item in the radar context menu, the 
	// corresponding action is applied
	void OnCommand(UINT id, CObject *pDisplayWnd=NULL);
	// computes the availability of the radar menu items
	void OnUpdateCmdUI(CCmdUI* pCmdUI);
	// sets the value of the parameter put in argument
	void SetViewParam(int ParamNb,int ParamValue);	
	// updates the content of the radar window status bar
	void UpdateStatusBar();
	// creates a new radar window
	void CreateSecondaryWnd(QRect RangeRect,CRwdView* pView=NULL,bool OnlyCreate=false, bool Iscollapse=false);
	// closes radar menu associated to the current radar window, if opened
	void RemoveRadarMenu();	
	// reloads the radar menu Map cascading menu, following the reception 
	// of a new set of map from the ODD CSCI
	void ReloadMapMenu();
	// calls the keyPressEvent function
	void PressEvent(QKeyEvent * e);
	// loads the radar window settings from a custom set, or saves the 
	// current settings in the custom set
	void Setup(CIniFile* pFile,QString Section,bool Save);
	// returns whether the radar window which has the focus has changed or not
	bool CheckFocus();
	// called when the "Quick Look +" menu item is selected
	void OnQuickLookPlus();
	// called when the "Quick Look -" menu item is selected
	void OnQuickLookMinus();
	// returns the window framing important track number to be
	// applied to a window according to the radar window put in argument
	int GetImportantTrackTpView(QRect WndBR, int iScrNb, CRwdView* pTpView);
	// Generated message map functions
	// called when the mouse pointer is moved over the radar window
	void mouseMoveEvent ( QMouseEvent * e );
	// called when a button of the mouse pointer is pressed
	void mousePressEvent ( QMouseEvent * e );
	// called when a button of the mouse pointer is released
	void mouseReleaseEvent ( QMouseEvent * e );
	// called to manage the press AB action
	void OnLButtonDown(UINT nFlags, QPoint point);
	// called to manage the release AB action
	void OnLButtonUp(UINT nFlags, QPoint point);
	// called to manage the press SB action
	void OnRButtonDown(UINT nFlags, QPoint point);
	// called to manage the press WB action
	void OnMButtonDown(UINT nFlags, QPoint point);
	// processes the mouse pointer wheel button scrolling action
	void wheelEvent ( QWheelEvent * e );
	// resizes the radar window and the status bar upon a resize action of the user
	void resizeEvent ( QResizeEvent * Event);
	// closes the radar window
	void closeEvent ( QCloseEvent * e ); 
	// called each time a started timer of the class reaches its value
	void timerEvent ( QTimerEvent * );
	// processes the keyboard keys press actions
	void keyPressEvent ( QKeyEvent * e );
	// called to associate to the view a new graphical element to be displayed
	void AddElement(CDisplayElement* pElem);
	// called to remove a graphical element displayed in the view from the m_ElemTable table
	void RemoveElement(CDisplayElement* pElem);
	// returns whether a graphical element is currently under creation or modification mode or not
	bool IsModifyNull();
	// returns the pointer to the radar window status bar object
	CStatusBar* GetStatusBar();
	// sets the graphical element put in argument as the graphical
	// element that is currently under creation or modification
	void SetModifyElement(CDisplayElement* pElem);
	// returns whether the graphical element put in argument is set
	// to be displayed in the radar window
	bool IsElementDisplayed(CDisplayElement* pElem);
	// returns the window framing important track number to be
	// applied to a window according to all the radar windows
	int GetImportantTrackUnderneath(QWidget* w,QRect WndBR, int iScrNb);
	// returns the value of the parameter put in argument
	int GetViewParam(int ParamNb);

	//virtual methods

	// calls the update function of the element put in argument, if any
	// for all the graphical elements associated to the view in the other case
	virtual void OnUpdate(CView* pSender, long lHint, CObject* pHint);
};



//{{AFX_INSERT_LOCATION}}

#endif // end of file

//--------------------------------< THE END >----------------------------------

