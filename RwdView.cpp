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
//  FILENAME: RwdView.cpp
//  TYPE    : c++ code-file
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
#include "DisplayElement.h"
#include "VisuMap.h"
#include "RwdView.h"
#include "Track.h"
#include "AsdMenu.h"
#include "Elements.h"
#include "Geo.h"
#include "FontMngr.h"
#include "ColorMngr.h"
#include "WdwMngr.h"
#include "SetupMngr.h"
#include "./CommonFiles/IniFile.h"
#include "stdlib.h"
#include "AsdApp.h"
#include "String.h"
#include "DataMngr.h"
#include "WinHints.h" 
#include "SKMessageBox.h" 
#include "TopLevel.h" 
#include "InfoMngr.h"
#include <qslider.h> 
#include <qbuttongroup.h>
#include "ImportantTrack.h"
#include "LineView.h"

//-GLOBAL DATA-----------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_RB 101
#define ID_RANGE 102
#define ID_PANNING 103
#define ID_DEFAULT 104
#define ID_QUICKLOOKPLUS 105
#define ID_QUICKLOOKMINUS 106
#define ID_HEIGHTPLUS 108
#define ID_HEIGHTMINUS 109
#define ID_HEIGHTFILTER 110
#define ID_PRIMARY 111
#define ID_DATABLOCK 114
#define ID_TRACKCONTROL 116
#define ID_AUXRADAR 118

CRwdView* CRwdView::m_psMainWnd=NULL;
CRwdView* CRwdView::m_psSecWnd1=NULL;
CRwdView* CRwdView::m_psSecWnd2=NULL;
WndFocus CRwdView::m_sWndFocus=MainFocus;
bool CRwdView::m_sIsIndependent=false;
bool CRwdView::m_sIsOnMapReload=false;
bool CRwdView::m_bsForcedMove = false;


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CRwdView
///  This class aims to manage the radar window	properties (range, center, 
///  size, filters...)
///	 This includes the radar menu, status bar, interactions, 
///      keyboard keys, and important track positionning.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: CIAsdFctRwsGen005
//
//-----------------------------------------------------------------------------
CRwdView::CRwdView()
{
	//Radar menu definition
	m_pCurElem = NULL;
	m_pModifElem = NULL;
	m_Mode=Normal;
	m_RadarMenu.AddItem("R/B",ID_RB,true);
	m_RadarMenu.InsertSeparator();
	m_RadarMenu.AddItem("Range",ID_RANGE,true);
	m_RadarMenu.AddItem("Panning",ID_PANNING,true);
	m_RadarMenu.AddItem("Default",ID_DEFAULT,true);
	m_RadarMenu.InsertSeparator();
	m_RadarMenu.AddItem("Quick Look +",ID_QUICKLOOKPLUS,true);
	m_RadarMenu.AddItem("Quick Look -",ID_QUICKLOOKMINUS,true);
	m_RadarMenu.InsertSeparator();
	m_ReducePanning = 0;

	//More cascading menu definition
	CAsdMenu* pMoreMenu=new CAsdMenu();
	pMoreMenu->AddItem("Height +",ID_HEIGHTPLUS);
	pMoreMenu->AddItem("Height -",ID_HEIGHTMINUS);
	pMoreMenu->AddItem("Height Filter...",ID_HEIGHTFILTER,true);
	CAsdMenu* pTfcMenu=new CAsdMenu();
	pTfcMenu->AddItem("Primary",ID_PRIMARY);
	pTfcMenu->AddItem("Data Block",ID_DATABLOCK);
	pMoreMenu->AddItem("Tfc Filters",pTfcMenu);
	pMoreMenu->InsertSeparator();
	CAsdMenu* pMapMenu =NULL;
	pMapMenu=new CAsdMenu();
	CAsdMenu* menu=CVisuMap::GetMenu();
	if (menu)
		pMapMenu=new CAsdMenu(*CVisuMap::GetMenu());
	pMoreMenu->AddItem("Offline Maps",pMapMenu);
	CAsdMenu* pMapOnlineMenu =NULL;
	pMapOnlineMenu=new CAsdMenu();
	pMoreMenu->AddItem("Online Maps",pMapOnlineMenu);
	pMoreMenu->SetDisabled(5,true);
	pMoreMenu->InsertSeparator();
	pMoreMenu->AddItem("Track Control...",ID_TRACKCONTROL,true);
	pMoreMenu->InsertSeparator();
	pMoreMenu->AddItem("Aux Radar",ID_AUXRADAR,true);


	m_RadarMenu.AddItem("More",pMoreMenu);
	m_TimerId=0;
	m_QlPlusTimer=0;
	m_QlMinusTimer=0;
	m_MainWasCollapsible=false;

	//initialization of the view parameters
	for (int i=0;i<=fld_tag_gsp;i++)
		m_Params[i]=0;
	m_DefaultRange=64;
	m_DefaultCentre=QPoint(0,0);
	m_FirstPanningMove=false;
	m_MouseTimerId=startTimer(100);
	//The settings of the radar windows are saved and loaded from the custom sets
	CSetupMngr::AddObject(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CRwdView&, CRwdView object to be initialized
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CRwdView::CRwdView(CRwdView& pTpView)
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
CRwdView::~CRwdView()
{
	//Cancels any tool modification or creation mode
	CVector::RemoveView(this);
	CRangeMark::RemoveView(this);
	CTrkMiles::RemoveView(this);
	CNewRange::RemoveView(this);
	CTOORArrow::RemoveView(this);
	
	//Removed the attached windows
	CWdwMngr::RemoveWindow(&m_TrackControl);
	CWdwMngr::RemoveWindow(&m_HeightFilter);

	//Cancels the view for all the application
	CWdwMngr::RemoveWindow(this);
	CAsdApp::GetApp()->GetTrackMngr()->RemoveView(this);
	CAsdApp::GetApp()->GetWeatherMngr()->RemoveView(this);
	CVisuMap::RemoveView(this);
	CSetupMngr::RemoveObject(this);

	//frees the static pointer
	if (m_psMainWnd==this)
		m_psMainWnd=NULL;
	if (m_psSecWnd1==this)
		m_psSecWnd1=NULL;
	if (m_psSecWnd2==this)
		m_psSecWnd2=NULL;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to draw the radar window content. This function calls
///  the OnDraw function for every graphical element to be displayed in the view.
///  Those elements are listed in the m_ElemTable table. The elements are displayed 
///  from the lower layer to the upper and within a layer from the lowest priority
///  order to the upper, so that elements with the highest priority are displayed
///  over any other graphical elements. 
///  Argument 1 : QPainter*, painting device
///  Argument 2 : int, layer number to be drawn
/// 
///  RETURNS:  
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen020, CIAsdFctMapGen015, CIAsdFctMapGen020,
///					CIAsdFctRwsGen010, CIAsdFctRwsGen015, CIAsdFctRwsGen020,
///					CIAsdFctRwsAre005, CIAsdFctRwsAre020, CIAsdSymSmbNrm075,
///					CIAsdSymSmbNrm025, CIAsdSymSmbBsm020, CIAsdSymSmbEsm010,
///					CIAsdSymSmbErs020, CIAsdSymSmbFld015, CIAsdSymSmbFld020
//
//-----------------------------------------------------------------------------
void CRwdView::OnDraw(QPainter* pDC)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	pDC->setBackgroundMode(TransparentMode);
	int elem,priority_nb;
		for(priority_nb = PRIO_MAP_MAX; priority_nb >=0; priority_nb--)
		{
			for (elem=0;elem<m_ElemTable.GetSize();elem++)
				m_ElemTable[elem]->OnDraw(pDC,priority_nb);
		}
		//If the currently selected radar service is not available the message
		//Display frozen is displayed in the top of the underlay layer
		if (!pTrkMngr->m_pTrkSrc)
		{
			QDateTime FrozenTime=pTrkMngr->GetFrozenTime();
			QRect rect=QWidget::rect();
			pDC->setFont(CFontMngr::GetFontByName("FROZEN"));
			pDC->setPen(CColorMngr::GetColor(CColorMngr::GetColorNb("FROZEN_MSG")));
			QString txt="FROZEN AT "+FrozenTime.toString("hh:mm:ss");
			QSize size=pDC->boundingRect(0,0,0,0,Qt::AlignLeft,txt).size();
			pDC->drawText(rect.center().x()-size.width()/2,rect.center().y(),txt);
		}
		//and with a higher priority the element with implicit focus 
		if (m_pCurElem)
			m_pCurElem->OnDraw(pDC,1);
		if (m_pModifElem)
			m_pModifElem->OnDraw(pDC,0);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to associate to the view a new graphical element to be
///  displayed. Those elements are listed in the m_ElemTable table.
///  Argument : CDisplayElement*, graphical element
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::AddElement(CDisplayElement* pElem)
{
	m_ElemTable.Add(pElem);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to remove a graphical element displayed in the view
///  from the m_ElemTable table.
///  Argument : CDisplayElement*, graphical element
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::RemoveElement(CDisplayElement* pElem)
{
		for (int elem=0;elem<m_ElemTable.GetSize();elem++)
		{
			if (m_ElemTable[elem]==pElem)
			{
				//if this element had the implicit focus the m_pCurElem is freed
				if (m_pCurElem==pElem)
					m_pCurElem=NULL;
				//if this element was under modification the m_pModifElem is freed
				if (m_pModifElem==pElem)
					m_pModifElem=NULL;
				m_ElemTable.RemoveAt(elem);
				break;
			}
		}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the graphical element put in argument is set
///  to be displayed in the radar window (is in the m_ElemTable table).
///  Argument : CDisplayElement*, graphical element
/// 
///  RETURNS: 
///  bool, whether the graphical element is displayed in the radar window
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CRwdView::IsElementDisplayed(CDisplayElement* pElem)
{
		for (int elem=0;elem<m_ElemTable.GetSize();elem++)
		{
			if (m_ElemTable[elem]==pElem)
				return true;
		}
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the graphical element put in argument as the graphical
///  element that is currently under creation or modification.
///  Argument : CDisplayElement*, graphical element
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::SetModifyElement(CDisplayElement* pElem)
{
	m_pModifElem = pElem;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether a graphical element is currently under creation or
///  modification mode or not.
/// 
///  RETURNS: 
///  bool, whether no element is being modified or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CRwdView::IsModifyNull()
{
	return (m_pModifElem == NULL);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function calls the update function of the element put in argument, if any
///  for all the graphical elements associated to the view in the other case.
///  Argument 1 : CView*, not used in this function
///  Argument 2 : long, not used in this function
///  Argument 3 : CObject*, a graphical element
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::OnUpdate(CView* , long , CObject* pHint) 
{
	//If the pointer is valid only this object presentation is updated
	if (pHint)
	{
		CDisplayElement* pElem = (CDisplayElement*)pHint;
		pElem->OnUpdate();
	}
	else
		//all the elements are updated 
		for (int elem=0;elem<m_ElemTable.GetSize();elem++)
				m_ElemTable[elem]->OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to refresh the whole display of every available radar
///  window.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::InvalidateAllViews()
{
	if (m_psMainWnd)
		m_psMainWnd->InvalidateRect(NULL);
	if (m_psSecWnd1)
		m_psSecWnd1->InvalidateRect(NULL);
	if (m_psSecWnd2)
		m_psSecWnd2->InvalidateRect(NULL);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update the presentation of every graphical elements
///  associated to every available radar window.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::UpdateAllViews()
{
	if (m_psMainWnd)
		m_psMainWnd->OnUpdate(NULL,0,NULL);
	if (m_psSecWnd1)
		m_psSecWnd1->OnUpdate(NULL,0,NULL);
	if (m_psSecWnd2)
		m_psSecWnd2->OnUpdate(NULL,0,NULL);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called following the creation of a radar window to initialize
///  its several settings with default settings before the offline defined ones are
///  loaded or retrieved from a custom set or the parent view.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsGen005
//
//-----------------------------------------------------------------------------
void CRwdView::OnInitialUpdate() 
{
	setMouseTracking(true);
	if (!m_psMainWnd)
		m_psMainWnd=this;
	m_pStatusBar=new CStatusBar(this);
	WindowParams param;
	param.Centre = QPoint(0,0);
	param.Range = 64;
	param.MinX = -12000;
	param.MaxX = 12000;
	param.MinY = - 12000;
	param.MaxY = 12000;
	param.Precision = 32;
	param.ScrollBar=true;
	SetParam(&param);
	//This view is now available to display tracks, weather information or tools
	CAsdApp::GetApp()->GetTrackMngr()->AddView(this);
	CAsdApp::GetApp()->GetWeatherMngr()->AddView(this);
	CVisuMap::AddView(this);
	CRangeMark::AddView(this);
	CTOORArrow::AddView(this);
	//Initialization of the associated windows: Track Control and Height Filter
	m_TrackControl.m_pView=this;
	CWdwMngr::AddWindow(&m_TrackControl,"TRK_CTRL", CWFAttrib::TRK_CTRL, false);
	m_TrackControl.Init();
	m_HeightFilter.m_pView=this;
	CWdwMngr::AddWindow(&m_HeightFilter,"HGHT_FILTERS",CWFAttrib::HGHT_FILTERS,false);
	m_HeightFilter.Init();
	//The status bar content is updated
	UpdateStatusBar();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to reload the radar menu Map cascading menu, following
///  the reception of a new set of map from the ODD CSCI.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::ReloadMapMenu()
{
  CAsdMenu* pMapMenu=new CAsdMenu(*CVisuMap::GetMenu());
  CAsdMenu* pSubMenu=m_RadarMenu.GetPopupMenu(9);
  CAsdMenu* pOldMenu=pSubMenu->SetPopupMenu(5,pMapMenu);
  delete pOldMenu;
  //The menu is now available : the parameter m_sIsOnMapReload is set to false
  m_sIsOnMapReload=false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to reload the radar menu Map cascading menu for 
///  every radar window.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::ReloadAllMapMenu()
{
	if (m_psMainWnd)
		m_psMainWnd->ReloadMapMenu();
	if (m_psSecWnd1)
		m_psSecWnd1->ReloadMapMenu();
	if (m_psSecWnd2)
		m_psSecWnd2->ReloadMapMenu();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the pointer to the radar window status bar object.
/// 
///  RETURNS: 
///  CStatusBar*, radar window status bar
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CStatusBar* CRwdView::GetStatusBar()
{
	return m_pStatusBar;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the radar window mode to the one put in argument.
///  Argument : bool, whether the radar windows must be independent or not 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::SetIndependentMode(bool depmode)
{
	m_sIsIndependent=depmode;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the radar windows are independent or not.
/// 
///  RETURNS: 
///  bool, whether the radar windows must be independent or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CRwdView::GetIsIndependent()
{
	return m_sIsIndependent;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the pointer to the first auxiliary radar window object.
/// 
///  RETURNS: 
///  CRwdView*, first auxiliary radar window 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CRwdView* CRwdView::GetSecWnd1()
{
	return m_psSecWnd1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the pointer to the second auxiliary radar window object.
/// 
///  RETURNS: 
///  CRwdView*, second auxiliary radar window 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CRwdView* CRwdView::GetSecWnd2()
{
	return m_psSecWnd2;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function retrieves the settings of the main radar window to apply them
///  to the view put in argument.
///  Argument : CRwdView*, auxiliary radar window 
/// 
///  RETURNS: 
///  void 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::UpdateAuxWndDep(CRwdView* pView)
{
	//Nothing is to be dons if no main radar window is available
	if (m_psMainWnd)
	{
		//The maps are retrieved
		CVisuMap::CopyView(pView,m_psMainWnd);
		//The view Quick Look + and - are options canceled
		if (pView->m_QlPlusTimer)
		{
			pView->killTimer(pView->m_QlPlusTimer);
			pView->m_QlPlusTimer=0;
		}
		if (pView->m_QlMinusTimer)
		{
			pView->killTimer(pView->m_QlMinusTimer);
			pView->m_QlMinusTimer=0;
		}
		//Main parameters and settings are retrieved
		for (int i=0;i<=fld_tag_gsp;i++)
			pView->m_Params[i]=m_psMainWnd->m_Params[i];
		if (m_psMainWnd->m_QlMinusTimer)
			pView->m_Params[filt_lower_on]=m_psMainWnd->m_PrevDownFilter;
		if (m_psMainWnd->m_QlPlusTimer)
			pView->m_Params[filt_upper_on]=m_psMainWnd->m_PrevUpFilter;
		if (m_psMainWnd->m_QlMinusTimer || m_psMainWnd->m_QlPlusTimer)
		{
			pView->m_Params[filt_prim_on]=m_psMainWnd->m_PrevPrimFilter;
			pView->m_Params[filt_db_on]=m_psMainWnd->m_PrevDataFilter;
		}

		//The Track Control and Height Filters windows are updated 
		pView->m_TrackControl.Init();
		pView->m_HeightFilter.Init();
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update the content of the radar window status bar.
/// 
///  RETURNS: 
///  void 
///
///  REQUIREMENT ID: CIAsdFctRwsFltGen005, CIAsdFctRwsFltGen010, CIAsdFctRwsGen070,
///					CIAsdFctRwsWrs010
//
//-----------------------------------------------------------------------------
void CRwdView::UpdateStatusBar()
{
	//radar window mode indication
	m_pStatusBar->UpdateInd(m_sIsIndependent,false);
	//currently selected radar service
	m_pStatusBar->UpdateRadarService(CAsdApp::GetApp()->GetTrackMngr()->GetSourceName(),!CAsdApp::GetApp()->GetTrackMngr()->IsCurrentSourceAv(), false);
	//mouse pointer lat/long in the radar window
	m_pStatusBar->UpdateCoordinates("",false);
	//The filter settings, the values displayed are according to the Quick Look options
	//If Quick look - is activated, the previous value of lower filter is shown
	//If Quick look + is activated, the previous value of upper filter is shown
	if (!m_QlMinusTimer && !m_QlPlusTimer)
	{
		m_pStatusBar->UpdateHeightFilter(GetViewParam(filt_lower_val),GetViewParam(filt_lower_on),GetViewParam(filt_upper_val),GetViewParam(filt_upper_on),GetViewParam(filt_dep_val),GetViewParam(filt_dep_on),false);
		m_pStatusBar->UpdateFilters(GetViewParam(filt_prim_on),GetViewParam(filt_vfr_on),GetViewParam(filt_mil_on),GetViewParam(filt_db_on),CAsdApp::GetApp()->GetWeatherMngr()->IsWeatherOn(),CAsdApp::GetApp()->GetWeatherMngr()->IsWeatherFailed(),false);
	}
	if (!m_QlMinusTimer && m_QlPlusTimer)
	{
		m_pStatusBar->UpdateHeightFilter(GetViewParam(filt_lower_val),GetViewParam(filt_lower_on),GetViewParam(filt_upper_val),m_PrevUpFilter,GetViewParam(filt_dep_val),GetViewParam(filt_dep_on),false);
		m_pStatusBar->UpdateFilters(m_PrevPrimFilter,GetViewParam(filt_vfr_on),GetViewParam(filt_mil_on),m_PrevDataFilter,CAsdApp::GetApp()->GetWeatherMngr()->IsWeatherOn(),CAsdApp::GetApp()->GetWeatherMngr()->IsWeatherFailed(),false);
	}
	if (m_QlMinusTimer && !m_QlPlusTimer)
	{
		m_pStatusBar->UpdateHeightFilter(GetViewParam(filt_lower_val),m_PrevDownFilter,GetViewParam(filt_upper_val),GetViewParam(filt_upper_on),GetViewParam(filt_dep_val),GetViewParam(filt_dep_on),false);
		m_pStatusBar->UpdateFilters(m_PrevPrimFilter,GetViewParam(filt_vfr_on),GetViewParam(filt_mil_on),m_PrevDataFilter,CAsdApp::GetApp()->GetWeatherMngr()->IsWeatherOn(),CAsdApp::GetApp()->GetWeatherMngr()->IsWeatherFailed(),false);
	}
	if (m_QlMinusTimer && m_QlPlusTimer)
	{
		m_pStatusBar->UpdateHeightFilter(GetViewParam(filt_lower_val),m_PrevDownFilter,GetViewParam(filt_upper_val),m_PrevUpFilter,GetViewParam(filt_dep_val),GetViewParam(filt_dep_on),false);
		m_pStatusBar->UpdateFilters(m_PrevPrimFilter,GetViewParam(filt_vfr_on),GetViewParam(filt_mil_on),m_PrevDataFilter,CAsdApp::GetApp()->GetWeatherMngr()->IsWeatherOn(),CAsdApp::GetApp()->GetWeatherMngr()->IsWeatherFailed(),false);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update the status bar content of all the
///  available radar windows.
/// 
///  RETURNS: 
///  void 
///
///  REQUIREMENT ID: CIAsdFctRwsFltGen005, CIAsdFctRwsFltGen010
//
//-----------------------------------------------------------------------------
void CRwdView::UpdateAllStatusBar()
{
	if (m_psMainWnd)
		m_psMainWnd->UpdateStatusBar();
	if (m_psSecWnd1)
		m_psSecWnd1->UpdateStatusBar();
	if (m_psSecWnd2)
		m_psSecWnd2->UpdateStatusBar();
}





//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the radar window which has the focus has
///  changed or not.
/// 
///  RETURNS: 
///  bool, whether the radar window which has the focus has changed or not 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CRwdView::CheckFocus()
{
	//The function returns false if a pop-up window (modal window) is currenly opened	
	if (CAsdApp::GetApp()->GetTrackMngr()->m_bDialogIsOn)
		//no further check is performed
		return false;

	WndFocus wndFocusTmp = OtherFocus;
	//If a track context menu is opened, the radar window from which it has been
	//invoked is retrieved
	if (CAsdApp::GetApp()->GetTrackMngr()->m_pTrackMenu)
	{
		if(CAsdApp::GetApp()->GetTrackMngr()->m_pTrackMenu->getHWnd())
		{
			QPoint pnt=QCursor::pos();
			QRect rect=CAsdApp::GetApp()->GetTrackMngr()->m_pTrackMenu->getHWnd()->frameGeometry();
			if (rect.contains(pnt))
			{
				if (CAsdApp::GetApp()->GetTrackMngr()->m_pMenuCallingWnd == m_psMainWnd)
					wndFocusTmp=MainFocus; 
				if (CAsdApp::GetApp()->GetTrackMngr()->m_pMenuCallingWnd == m_psSecWnd1)
					wndFocusTmp=Sec1Focus;
				if (CAsdApp::GetApp()->GetTrackMngr()->m_pMenuCallingWnd == m_psSecWnd2)
					wndFocusTmp=Sec2Focus;
			}
		}
	}

	//Check if a radar window has the focus
	if(m_psMainWnd)
	{
		if(m_psMainWnd->hasMouse())
		{
			wndFocusTmp=MainFocus;
		}
	}
	if(m_psSecWnd1)
	{
		if(m_psSecWnd1->hasMouse())
		{
			wndFocusTmp=Sec1Focus;
		}
	}
	if(m_psSecWnd2)
	{
		if(m_psSecWnd2->hasMouse())
		{
			wndFocusTmp=Sec2Focus;
		}
	}

	
	//If the radar window having the focus has changed the graphical element 
	//having the implicit focus on the previous window having the focus is 
	//deselected
	if(m_sWndFocus!=wndFocusTmp)
	{
		if(m_sWndFocus==MainFocus && m_psMainWnd)
		{
			if(m_psMainWnd->m_pCurElem)
				m_psMainWnd->m_pCurElem->SelectElement(false);
			m_psMainWnd->m_pCurElem=NULL;
		}
		else if(m_sWndFocus==Sec1Focus && m_psSecWnd1)
		{
			if(m_psSecWnd1->m_pCurElem)
				m_psSecWnd1->m_pCurElem->SelectElement(false);
			m_psSecWnd1->m_pCurElem=NULL;
		}
		else if(m_sWndFocus==Sec2Focus && m_psSecWnd2)
		{
			if(m_psSecWnd2->m_pCurElem)
				m_psSecWnd2->m_pCurElem->SelectElement(false);
			m_psSecWnd2->m_pCurElem=NULL;
		}

		m_sWndFocus=wndFocusTmp;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to send the mouse move event to every opened
///  radar window so that a radar window not having the mouse pointer focus can
///  process the event.
/// 
///  RETURNS: 
///  void 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::ForceMouseMoveEvent ( )
{
	m_bsForcedMove = true;
	if (m_psMainWnd)
		m_psMainWnd->mouseMoveEvent(NULL);
	if (m_psSecWnd1)
		m_psSecWnd1->mouseMoveEvent(NULL);
	if (m_psSecWnd2)
		m_psSecWnd2->mouseMoveEvent(NULL);
	m_bsForcedMove = false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when the mouse pointer is moved over the radar window.
///  Argument : QMouseEvent*, not used in this function
/// 
///  RETURNS: 
///  void 
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen095, CIAsdFctRwsTlsRmk022, CIAsdFctRwsPan005,
///					CIAsdFctRwsGen055
//
//-----------------------------------------------------------------------------
void CRwdView::mouseMoveEvent ( QMouseEvent * e )
{
	QRect radarwndrect;
	QPoint globalpnt;
	QRect statusbarrect;
	QPoint pnt = QCursor::pos();
	QPoint point=mapFromGlobal(pnt);

	CheckFocus();

	//Check the radar window mode
	switch (m_Mode)
	{
	case Normal:
		{
			//No mode is activated for the radar window having the focus
			//No process is to be performed if the radar window is over another
			//radar window for which the panning mode is activated
			if(m_psMainWnd)
			{
				if(m_psMainWnd->m_Mode==Panning)
					break;
			}
			if(m_psSecWnd1)
			{
				if(m_psSecWnd1->m_Mode==Panning)
					break;
			}
			if(m_psSecWnd2)
			{
				if(m_psSecWnd2->m_Mode==Panning)
					break;
			}

			//The status bar must show the current lat long of the mouse pointer
			//if it presents the focus and the mouse pointer is over the view
			QPoint pnt;
			if (m_bsForcedMove || ((point!=m_LastCursorPos) && (hasMouse())))
			{
				killTimer(m_TimerId);
				m_TimerId=startTimer(100);
				pnt=point;
				DPtoLP(&pnt);
				radarwndrect=this->geometry();
				statusbarrect=GetStatusBar()->rect();
				//The mouse pointer must not be over the status bar
				if (globalpnt.y()<=radarwndrect.y()+radarwndrect.height()-statusbarrect.height())
				{
					Latitude Lat;
					Longitude Long;
					double dLat,dLong;
					dLat=pnt.y();
					dLong=pnt.x();
					CGeo::StereographicToGeographic(dLat,dLong,Lat,Long);
					QString LatLong;
					char North='N';
					char East='E';
					if (!Lat.NORTH)
						North='S';
					if (!Long.EAST)
						East='W';
					LatLong.sprintf("%.2d %.2d %.2d %c %.3d %.2d %.2d %c",Lat.deg,Lat.min,Lat.sec,North,abs(Long.deg),abs(Long.min),abs(Long.sec),East);
					m_pStatusBar->UpdateCoordinates(LatLong);
				}
			}
			m_LastCursorPos=point;
			//The event is then transmitted to the graphical element that are
			//under modification or that has the implicit focus
			if (hasMouse())
			{
				if (m_pModifElem)
					m_pModifElem->OnMouseMove(0,point);
				if (m_pCurElem)
				{
					int curprio=m_pCurElem->GetPriority(point,true);
					if (curprio==0)
					{
						m_pCurElem->SelectElement(false);
						m_pCurElem=NULL;
					}
				}
			}
			else
			{
				//If the element being modified is not within the view the event is 
				//transmitted anyway in order to cancel the tool creation mode
				if (m_pModifElem)
				  m_pModifElem->OnMouseMove(0,point);
	
			}
		}
		break;
	case Panning:
		{
			//Panning mode
			if (m_ReducePanning >= 10)
			{
			m_ReducePanning=0;
			QRect rect=QWidget::rect();
			QSize size;
			QPoint pnt;
			size=QSize(rect.center().x()-point.x(),rect.center().y()-point.y());
			DPtoLP(&size);
			size.setHeight(-size.height());
			//Because of the range limitation, the center must remains within 
			//predefined limits
//			if ((size.width()) || (size.height()))
			{
				//Following the first move of the mouse pointer this check is not performed
				//and over all the presentation of the view is not updated as the first move
				//is the positionning of the mouse pointer at the center of the view
				if (!m_FirstPanningMove)
				{
					pnt=GetCentre();
					pnt=QPoint(pnt.x()+size.width(),pnt.y()+size.height());
	
/*					if(pnt.x()>5000)
						pnt.setX(5000);

					if(pnt.x()<-5000)
						pnt.setX(-5000);

					if(pnt.y()>5000)
						pnt.setY(5000);

					if(pnt.y()<-5000)
						pnt.setY(-5000);*/
					
					SetCentre(pnt);
					
					OnUpdate(NULL,0,NULL);
					
					
				}
				m_FirstPanningMove=false;
				rect=geometry();
				QCursor::setPos(rect.center().x(),rect.center().y());
				CTOORArrow::UpdateFromView(this);
			}
			}
			else
			m_ReducePanning ++;
		}
		break;
	case Zoom:
		//Zoom mode
		radarwndrect=this->geometry();
		globalpnt=QCursor::pos();
		statusbarrect=GetStatusBar()->rect();

		//The mouse pointer must be over the radar window to perform the change
		if (globalpnt.x()>=radarwndrect.x()+radarwndrect.width() || globalpnt.x()<=radarwndrect.x() || globalpnt.y()<=radarwndrect.y() || globalpnt.y()>=radarwndrect.y()+radarwndrect.height()-statusbarrect.height())
		{
			m_Mode=Normal;
			setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
		}
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when a button of the mouse pointer is pressed. The 
///  function corresponding to the pressed button is called.
///  Argument : QMouseEvent*, the event description
/// 
///  RETURNS: 
///  void 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::mousePressEvent ( QMouseEvent * e )
{
	ButtonState btn=e->button();
	switch (btn)
	{
	case LeftButton:
		OnLButtonDown(0,e->pos());
		break;
	case MidButton:
		OnMButtonDown(0,e->pos());
		break;
	case RightButton:
		OnRButtonDown(0,e->pos());
		break;
	default:
		break;
	}
	if (!hasMouse())
		setWState(WState_HasMouse);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when a button of the mouse pointer is released. The 
///  function corresponding to the released button is called.
///  Argument : QMouseEvent*, the event description
/// 
///  RETURNS: 
///  void 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::mouseReleaseEvent ( QMouseEvent * e )
{
	ButtonState btn=e->button();
	switch (btn)
	{
	case LeftButton:
		OnLButtonUp(0,e->pos());
		break;
	case MidButton:
		break;
	case RightButton:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the press AB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::OnLButtonDown(UINT nFlags, QPoint point) 
{
	//If no route is under modification and the mode is normal
	//the event is transmitted to the graphical element that is being modified
	//otherwise to the graphical element that has the implicit focus
	switch (m_Mode)
	{
		case Normal:
		{
			if (m_pModifElem)
				m_pModifElem->OnLButtonDown(nFlags,point);
			else
				if (m_pCurElem)
					m_pCurElem->OnLButtonDown(nFlags,point);
		}
		break;
		default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the release AB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::OnLButtonUp(UINT nFlags, QPoint point) 
{
	//the event is transmitted to the graphical element that is being modified
	//if any else to the graphical element that has the implicit focus
	switch (m_Mode)
	{
	case Normal:
		{
			if (m_pModifElem)
				m_pModifElem->OnLButtonUp(nFlags,point);
			else
				if (m_pCurElem)
					m_pCurElem->OnLButtonUp(nFlags,point);
		}
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the press SB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::OnRButtonDown(UINT nFlags, QPoint point) 
{
	//If no route is under modification and the mode is normal
	//the event is transmitted to the graphical element that is being modified
	//otherwise to the graphical element that has the implicit focus or to the 
	//map having the implicit focus if it has a highest priority order
	switch (m_Mode)
	{
	case Normal:
		{
			if (m_pModifElem)
				m_pModifElem->OnRButtonDown(nFlags,point);
			else if (m_pCurElem)
			{
				if (m_pCurElem->GetPriority(point) <= PRIO_MAP_MAX)
					m_RadarMenu.TrackPopupMenu(mapToGlobal(point).x(),mapToGlobal(point).y(),this);
				else
					m_pCurElem->OnRButtonDown(nFlags,point);
			}
			else
				m_RadarMenu.TrackPopupMenu(mapToGlobal(point).x(),mapToGlobal(point).y(),this);
		}
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the press WB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::OnMButtonDown(UINT nFlags, QPoint point) 
{
	switch (m_Mode)
	{
		case Normal:
		{
			//the event is transmitted to the graphical element that is being modified
			//otherwise to the graphical element that has the implicit focus 				
			if (m_pModifElem)
				m_pModifElem->OnMButtonDown(nFlags,point);
			else if (m_pCurElem)
					m_pCurElem->OnMButtonDown(nFlags,point);
			else
				CAsdApp::GetApp()->GetTrackMngr()->RemoveMenu();

			//Ckick WB action removes all opened menus
			RemoveAllRadarMenu();
			CTopLevel::RemoveAllMenu();
		}
		break;
	case Zoom:
		//The Zooming mode is canceled
		m_Mode=Normal;
		setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
		break;
	case Panning:
		//The Panning mode is canceled
		releaseMouse();
		setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
		m_Mode=Normal;
		break;
	default:
		break;
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon the selection of an available item in the radar context menu, the 
///  corresponding action is applied.
///  Argument 1 : UINT, selected menu item number
///  Argument 2 : CObject, not used in this function
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl005, CIAsdFctRwsTlsRbl030, CIAsdFctRwsFltHgt005,
///					CIAsdFctRwsFltHgt006, CIAsdFctRwsFltHgt021, CIAsdFctRwsFltHgt022,
///					CIAsdFctRwsFltPrm005, CIAsdFctRwsFltDbf005, CIAsdFctRwsFltQck005,
///					CIAsdFctRwsFltQck010, CIAsdFctRwsPan005, CIAsdFctMapGen025,
///					CIAsdFctRwsAre015, CIAsdFctRwsAux005
//
//-----------------------------------------------------------------------------
void CRwdView::OnCommand(UINT id,CObject *pDisplayWnd)
{
	CView* pView=NULL;
	QRect rect;
	QPoint pos=QCursor::pos();
	//Positionning of the Track Contol window
	QPoint framepos=QPoint(m_TrackControl.Prediction_Vector->x(),m_TrackControl.Prediction_Vector->y());
	QPoint sliderpos=QPoint(m_TrackControl.SliderVector->x(),m_TrackControl.SliderVector->y());
	QRect handlerect=m_TrackControl.SliderVector->sliderRect();
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(&m_TrackControl, CWFAttrib::TRK_CTRL);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();
	QPoint posTCW=QPoint(pos.x()-4-framepos.x()-sliderpos.x()-handlerect.x()-(handlerect.width())/2,pos.y()-TitleOffset-framepos.y()-sliderpos.y()-handlerect.y()-(handlerect.height())/2);
	//Positionning of the Height Filter window
	QPoint posHFW=QPoint(pos.x()-4-m_HeightFilter.width()/2,pos.y()-TitleOffset-m_HeightFilter.height()/2); 
	if (m_sIsIndependent)
		pView=this;
	switch (id)
	{
	case ID_TRACKCONTROL:
		//opens the Track Control window
		m_TrackControl.Init();
		m_TrackControl.move(posTCW);
		CWdwMngr::ShowWindow(&m_TrackControl,CWFAttrib::TRK_CTRL);
		break;
	case ID_RB:
		//The range and bearing creation mode is activated
		CVector::Create(this);
		break;
	case ID_RANGE:
		//the zooming mode is activated
		if (m_Mode==Normal)
		{
			m_Mode=Zoom;

			if(m_pCurElem)
				m_pCurElem->SelectElement(false);
			m_pCurElem=NULL;

			setCursor(*CAsdApp::GetApp()->GetCursor(MPZooming));
		}
		break;
	case ID_PANNING:
		//the panning mode is activated
		m_ReducePanning=0;
		if (m_Mode==Normal)
		{
			if(m_psMainWnd)
			{
				killTimer(m_psMainWnd->m_TimerId);
				m_psMainWnd->m_TimerId=0;
			}
			if(m_psSecWnd1)
			{
				killTimer(m_psSecWnd1->m_TimerId);
				m_psSecWnd1->m_TimerId=0;
			}
			if(m_psSecWnd2)
			{
				killTimer(m_psSecWnd2->m_TimerId);
				m_psSecWnd2->m_TimerId=0;
			}
			m_Mode=Panning;
			
			if(m_pCurElem)
				m_pCurElem->SelectElement(false);
			m_pCurElem=NULL;

			m_FirstPanningMove=true;
			rect=geometry();
			QCursor::setPos(rect.center().x(),rect.center().y());
			setCursor(*CAsdApp::GetApp()->GetCursor(MPPanning));
			grabMouse();
		}
		break;
	case ID_DEFAULT:
		//Sets the settings of the view to the custom set settings
	    SetRange(m_DefaultRange);
		SetCentre(m_DefaultCentre);
		OnUpdate(NULL,0,NULL);
		CTOORArrow::UpdateFromView(this);
		break;
	case ID_HEIGHTFILTER:
		//Opens the Height filter window
		CRwdView::CloseAllHFW();
	    m_HeightFilter.Init();
		m_HeightFilter.move(posHFW);
		CWdwMngr::ShowWindow(&m_HeightFilter,CWFAttrib::HGHT_FILTERS);
		break;
	case ID_HEIGHTPLUS:
		//toggle the upper filter status
		SetViewParam(filt_upper_on,!GetViewParam(filt_upper_on));
		break;
	case ID_HEIGHTMINUS:
		//toggle the lower filter status
		SetViewParam(filt_lower_on,!GetViewParam(filt_lower_on));
		SetViewParam(filt_dep_on,!GetViewParam(filt_dep_on));
		break;
	case ID_PRIMARY:
		//toggle the primary filter status
		SetViewParam(filt_prim_on,!GetViewParam(filt_prim_on));
		UpdateStatusBar();
		break;
	case ID_DATABLOCK:
		//toggle the data block filter status
		SetViewParam(filt_db_on,!GetViewParam(filt_db_on));
		UpdateStatusBar();
		break;
	case ID_AUXRADAR:
		//activates the create radar window mode
		CNewRange::Create(this);
		break;
	case ID_QUICKLOOKPLUS:
	    //activates the Quick Look + option to the window for which it is invoked
		//otherwise if the mode is independent to all
		if (m_sIsIndependent)
			this->OnQuickLookPlus();
		else 
		{
			if (m_psMainWnd)
				m_psMainWnd->OnQuickLookPlus();
			if (m_psSecWnd1)
				m_psSecWnd1->OnQuickLookPlus();
			if (m_psSecWnd2)
				m_psSecWnd2->OnQuickLookPlus();
		}
		break;
	case ID_QUICKLOOKMINUS:
	    //activates the Quick Look - option to the window for which it is invoked
		//otherwise if the mode is independent to all
		if (m_sIsIndependent)
			this->OnQuickLookMinus();
		else 
		{		
			if (m_psMainWnd)
				m_psMainWnd->OnQuickLookMinus();
			if (m_psSecWnd1)
				m_psSecWnd1->OnQuickLookMinus();
			if (m_psSecWnd2)
				m_psSecWnd2->OnQuickLookMinus();
		}
		break;
	default:
		break;
	}
	//Selection of a map menu item
	if (id>=ID_MAPS)
		CVisuMap::OnMaps(id,pView);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function computes the availability of the radar menu items. 
///  Argument : CCmdUI, menu item
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl010, CIAsdFctRwsFltQck015, CIAsdFctRwsFltQck020,
///					CIAsdFctRwsAux005, CIAsdFctRwsAux010
//
//-----------------------------------------------------------------------------
void CRwdView::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	CView* pView=NULL;
	if (m_sIsIndependent)
		pView=this;
	switch (pCmdUI->GetID())
	{
	case ID_AUXRADAR:
   		pCmdUI->Enable(CAsdApp::GetApp()->WindowEligibility(CWFAttrib::AUX_RDR) && (!m_psSecWnd1 || !m_psSecWnd2)
				&& !CRangeMark::IsRangeMarkInModif() && !CTrkMiles::IsInCreation() 
				&& !CVector::IsInCreation() && !CNewRange::IsInCreation() && !CRwdView::IsInViewModif());
		break;
	case ID_DEFAULT:
		pCmdUI->Enable(true);
		break;
	case ID_RANGE:
		pCmdUI->Enable(!CRangeMark::IsRangeMarkInModif() && !CTrkMiles::IsInCreation() 
		&& !CVector::IsInCreation() && !CNewRange::IsInCreation() && !CRwdView::IsInViewModif());
		break;
	case ID_PANNING:
		pCmdUI->Enable(!CRangeMark::IsRangeMarkInModif() && !CTrkMiles::IsInCreation() 
		&& !CVector::IsInCreation() && !CNewRange::IsInCreation() && !CRwdView::IsInViewModif());
		break;
	case ID_TRACKCONTROL:
		pCmdUI->Enable(true);
		break;
	case ID_HEIGHTFILTER:
		pCmdUI->Enable(true);
       	break;
	case ID_RB:
		pCmdUI->Enable(!CRangeMark::IsRangeMarkInModif() && !CTrkMiles::IsInCreation() 
		&& !CVector::IsInCreation() && !CNewRange::IsInCreation() && !CRwdView::IsInViewModif() 
		&& !CVector::IsMaxNumberReached(this));
		break;
	case ID_PRIMARY:
		pCmdUI->Enable(true);
		if (!m_QlMinusTimer && !m_QlPlusTimer)
			pCmdUI->SetCheck(GetViewParam(filt_prim_on));
		else
			pCmdUI->SetCheck(m_PrevPrimFilter);
		break;
	case ID_DATABLOCK:
		pCmdUI->Enable(true);
		if (!m_QlMinusTimer && !m_QlPlusTimer)
			pCmdUI->SetCheck(GetViewParam(filt_db_on));
		else
			pCmdUI->SetCheck(m_PrevDataFilter);
		break;
	case ID_HEIGHTPLUS:
		pCmdUI->Enable(true);
		//to keep the status of the filter when the Quick look facility is activated
		if (!m_QlPlusTimer)
		  pCmdUI->SetCheck(GetViewParam(filt_upper_on));
		else
		  pCmdUI->SetCheck(m_PrevUpFilter);
		break;
	case ID_HEIGHTMINUS:
		pCmdUI->Enable(true);
		//to keep the status of the filter when the Quick look facility is activated
		if (!m_QlMinusTimer)
		  pCmdUI->SetCheck(GetViewParam(filt_lower_on));
		else
		  pCmdUI->SetCheck(m_PrevDownFilter);
		break;
	case ID_QUICKLOOKPLUS:
		if  (!m_Params[filt_upper_on] && !m_Params[filt_prim_on] && !m_Params[filt_db_on])
			pCmdUI->Enable(false);
		else
			pCmdUI->Enable(true);
		break;
	case ID_QUICKLOOKMINUS:
		if (!m_Params[filt_lower_on] && !m_Params[filt_prim_on] && !m_Params[filt_db_on])
			pCmdUI->Enable(false);
		else
			pCmdUI->Enable(true);
		break;
	default:
		break;

	}
	//availability of the Offline Map cascading menu
	CAsdMenu* menu=CVisuMap::GetMenu();
	CAsdMenu* pSubMenu=m_RadarMenu.GetPopupMenu(9);
	//The menu item is grayed out if the maps are under reload or if no
	//offline map is available
	if (m_sIsOnMapReload || !menu)
		pSubMenu->SetDisabled(5,true);
	else
	{
		pSubMenu->SetDisabled(5,false);
		if (pCmdUI->GetID()>=ID_MAPS)
		{
			CVisuMap* pMap=CVisuMap::GetMap(pCmdUI->GetID());
			if (!pMap)
				return;
			pCmdUI->Enable(true);
			pCmdUI->SetCheck(pMap->IsSelected(this));
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function resizes the radar window and the status bar upon a resize 
///  action of the user. 
///  Argument : QResizeEvent, description of the event
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsAux025
//
//-----------------------------------------------------------------------------
void CRwdView::resizeEvent ( QResizeEvent * Event)
{
	QRect rect=m_pStatusBar->rect();
	m_pStatusBar->move(0,Event->size().height()-rect.height());
	m_pStatusBar->resize(Event->size().width(),rect.height());
	CAsdView::resizeEvent(Event);
	//If the new size is lower than the minimum size or upper than the maximum size, no
	//resize is performed
	if ((Event->oldSize().width() > this->minimumSize().width()) 
		&& (Event->oldSize().height() > this->minimumSize().height()) 
		&& (Event->size().width() > this->minimumSize().width()) 
		&& (Event->size().height() > this->minimumSize().height()))
	{
		CWdwMngr::SetWindowSize(this);
	}
	CTOORArrow::UpdateFromView(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function closes the radar window. 
///  Argument : QCloseEvent, not used in this function
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::closeEvent ( QCloseEvent *  ) 
{
	if (m_RadarMenu.IsMenuOpen())
		m_RadarMenu.StopTracking();
	//The main radar window cannot be destroyed it is hidden instead 
	if (m_psMainWnd==this)
		CWdwMngr::HideWindow(m_psMainWnd);
	if (m_psSecWnd1==this)
		delete this;
	if (m_psSecWnd2==this)
		delete this;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function processes the mouse pointer wheel button scrolling action. 
///  Argument : QWheelEvent, description of the event
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsRan015
//
//-----------------------------------------------------------------------------
void CRwdView::wheelEvent ( QWheelEvent * e )
{
	ButtonState state;

	int m_RangeStep=2,NewRange;
	int zDelta= e->delta();
    state = e->state();
	switch (m_Mode)
	{
	case Normal:
		break;
	case Zoom:
		//In the zoom mode, the zoom is changed within the range limitations
		NewRange=GetRange()-(zDelta/12)*m_RangeStep;
		if (NewRange<10)
		  NewRange=10;
		if (NewRange>1024)
		  NewRange=1024;
		SetRange(NewRange);
		CTOORArrow::UpdateFromView(this);
		OnUpdate(NULL,0,NULL);
		break;
	default:
		break;
	}		
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function processes the keyboard keys press actions. 
///  Argument : QKeyEvent, description of the event
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl015, CIAsdFctRwsTlsGrd095, CIAsdFctRwsRan015
///					 CIAsdFctRwsPan005, CIAsdFctKfk005, CIAsdFctKfk015, 
///					 CIAsdFctKfk020, CIAsdFctKfk025, CIAsdFctKfk030, CIAsdFctKfk050
///				     CIAsdFctKfk055, CIAsdFctKfk060, CIAsdFctKfk075, CIAsdFctKfk080
///					 CIAsdFctKfk081, CIAsdFctKfk090, CIAsdFctKfk004, CIAsdFctKfk010
//
//-----------------------------------------------------------------------------
void CRwdView::keyPressEvent ( QKeyEvent * e )
{
	// CDS BREAK NGT 08/06/2006 Bad method length rule is not violated considering
	//							only the real length of the function.

	//If the current view is not in panning mode but another radar window
	//is in panning mode, the event is transmitted
	if (m_Mode!=Panning)
	{
		if(m_psMainWnd && m_psMainWnd->m_Mode==Panning)
		{
			m_psMainWnd->keyPressEvent ( e );
			return;
		}
		if(m_psSecWnd1 && m_psSecWnd1->m_Mode==Panning)
		{
			m_psSecWnd1->keyPressEvent ( e );
			return;
		}
		if(m_psSecWnd2 && m_psSecWnd2->m_Mode==Panning)
		{
			m_psSecWnd2->keyPressEvent ( e );
			return;
		}
	}

	int m_RangeStep=10;
	QSize size;
	QPoint pnt;
	int NewRange;
	CWeatherMngr* pWeatherMngr= CAsdApp::GetApp()->GetWeatherMngr();

	switch (e->key())
	{
	case Key_Up:
	case Key_Down:
	case Key_Left:
	case Key_Right:
		{
			//Panning with arrow keys within range limitations
			if (e->key()==Key_Up)
				size=QSize(0,120);
			if (e->key()==Key_Down)
				size=QSize(0,-120);
			if (e->key()==Key_Left)
				size=QSize(-120,0);
			if (e->key()==Key_Right)
				size=QSize(120,0);
			DPtoLP(&size);
			pnt=GetCentre();
			pnt.setX(pnt.x()+size.width());
			pnt.setY(pnt.y()+size.height());
//			if(pnt.x()<5000 && pnt.x()>-5000 && pnt.y()<5000 && pnt.y()>-5000)
			{
				SetCentre(pnt);
				OnUpdate(NULL,0,NULL);
			}
			CTOORArrow::UpdateFromView(this);
		}
		break;
	case Key_Plus:
			//Zoom in with range limitation
			NewRange=GetRange()-m_RangeStep;
			if (NewRange<10)
			  NewRange=10;
			SetRange(NewRange);
			OnUpdate(NULL,0,NULL);
			CTOORArrow::UpdateFromView(this);
		break;
	case Key_Minus:
			//Zoom out with range limitation
			NewRange=GetRange()+m_RangeStep;
			if (NewRange>1024)
			  NewRange=1024;
			SetRange(NewRange);
			OnUpdate(NULL,0,NULL);
			CTOORArrow::UpdateFromView(this);
		break;
	case Key_Escape:
		//The event is transmitted to the modified or selected elements
		//or the current zoom or panning mode if any is canceled
		if (m_pModifElem)
		  m_pModifElem->OnKeyDown();
		else if (m_pCurElem)
		    m_pCurElem->OnKeyDown();
		else if (m_Mode!=Normal)
		{
			m_Mode=Normal;
			releaseMouse();
			setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
		}
		break;
	default:
	  break;
	}
	if (this==m_psMainWnd)
	  switch (e->key())
	    {
		case Key_F1:
			//The current tool modification is canceled and the current custom set is loaded
			if (m_psMainWnd)
			{
				if (m_psMainWnd->m_pModifElem)
					m_psMainWnd->m_pModifElem->OnKeyDown();
			}
			if (m_psSecWnd1)
			{
				if (m_psSecWnd1->m_pModifElem)
					m_psSecWnd1->m_pModifElem->OnKeyDown();
			}
			if (m_psSecWnd2)
			{
				if (m_psSecWnd2->m_pModifElem)
					m_psSecWnd2->m_pModifElem->OnKeyDown();
			}
			CSetupMngr::ReloadDefaultSetup();
			break;
		case Key_F2:
			//Not used
			break;
		case Key_F3:
		    //activates the Quick Look + option to the Main radar window
			//otherwise if the mode is independent to all
			if (m_sIsIndependent)
				m_psMainWnd->OnQuickLookPlus();
			else 
			{
				if (m_psMainWnd)
					m_psMainWnd->OnQuickLookPlus();
				if (m_psSecWnd1)
					m_psSecWnd1->OnQuickLookPlus();
				if (m_psSecWnd2)
					m_psSecWnd2->OnQuickLookPlus();
			}
			break;
		case Key_F4:
		    //activates the Quick Look - option to the Main radar window
			//otherwise if the mode is independent to all
			if (m_sIsIndependent)
				m_psMainWnd->OnQuickLookMinus();
			else 
			{		
				if (m_psMainWnd)
					m_psMainWnd->OnQuickLookMinus();
				if (m_psSecWnd1)
					m_psSecWnd1->OnQuickLookMinus();
				if (m_psSecWnd2)
					m_psSecWnd2->OnQuickLookMinus();
			}
			break;
		case Key_F5:
			//Toggles the upper filter status
			m_psMainWnd->SetViewParam(filt_upper_on,!m_psMainWnd->GetViewParam(filt_upper_on));
			break;
		case Key_F6:
			//Toggles the lower and departure filter status
			m_psMainWnd->SetViewParam(filt_lower_on,!m_psMainWnd->GetViewParam(filt_lower_on));
			m_psMainWnd->SetViewParam(filt_dep_on,!m_psMainWnd->GetViewParam(filt_dep_on));
			break;
		case Key_F7:
			//Toggles the primary filter status
			m_psMainWnd->SetViewParam(filt_prim_on,!m_psMainWnd->GetViewParam(filt_prim_on));
			break;
		case Key_F8:
			//Toggles the VFR filter status
			m_psMainWnd->SetViewParam(CRwdView::filt_vfr_on,!m_psMainWnd->GetViewParam(filt_vfr_on));
			break;
		case Key_F9:
			//Toggles the military filter status
			m_psMainWnd->SetViewParam(CRwdView::filt_mil_on,!m_psMainWnd->GetViewParam(filt_mil_on));
			break;
		case Key_F10:
			//toggles the weather display status
			if (!pWeatherMngr->IsWeatherFailed())
			{
				CAsdApp::GetApp()->GetWeatherMngr()->DisplayWeather(!CAsdApp::GetApp()->GetWeatherMngr()->IsWeatherOn());
				CTopLevel::UpdateSystemInfo();
				UpdateAllStatusBar();
			}
			break;
		case Key_Print:
			//Print screen
			if (e->state()==Qt::ShiftButton)
			{
				QPixmap p;
				p=QPixmap::grabWindow(QApplication::desktop()->winId(),0,0,QApplication::desktop()->width(),QApplication::desktop()->height());
				if (p.save("ScreenShot.bmp","BMP"))
					CSKMessageBox::information(0,"Print Screen","The screens capture\nhave been saved\nsucessfully.");
				else
					CSKMessageBox::critical(0,"Print Screen","An error occurred.\nThe screen display\ncould not be saved.");
			}
			break;
		default:
			break;
	    }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function calls the keyPressEvent function. 
///  Argument : QKeyEvent, description of the event
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::PressEvent(QKeyEvent * e)
{
	keyPressEvent(e);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to create a new radar window. 
///  Argument 1 : QRect, rect of the new radar window
///  Argument 2 : CRwdView*, parent view
///  Argument 3 : bool, whether the view is created manualy or not
///  Argument 4 : bool, if the view is collapsed or not
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::CreateSecondaryWnd(QRect RangeRect,CRwdView* pCurView,bool OnlyCreate, bool IsCollapse)
{
	CRwdView* pView=NULL;
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	QRect RealRect=RangeRect;
	LPtoDP(&RealRect);
	QPoint pos1=RealRect.bottomLeft(),pos2=RealRect.topRight();
	pos1=mapToGlobal(pos1);
	pos2=mapToGlobal(pos2);
	RealRect=QRect(pos1,pos2);
	if ((!m_psSecWnd1) || (!m_psSecWnd2))
	{
		//A new radar window object is created
		pView=new CRwdView();
		//initial update
		pView->OnInitialUpdate();
		pView->setCaption("Auxiliary Window");
		//The offline defined minimum and maximum sizes are retrieved and applied
		CAsdApp::GetApp()->SetMinMaxAttributes(pView,CWFAttrib::AUX_RDR, false);
		QSize size_min=pView->minimumSize();
		if (size_min.width()>RealRect.width())
			RealRect.setRight(RealRect.left()+size_min.width());
		if (size_min.height()>RealRect.height())
			RealRect.setBottom(RealRect.top()+size_min.height());
		QSize size_max=pView->maximumSize();
		if (size_max.width()<RealRect.width())
			RealRect.setRight(RealRect.left()+size_max.width());
		if (size_max.height()<RealRect.height())
			RealRect.setBottom(RealRect.top()+size_max.height());

		//If the first auxiliary radar window is attributed, the second is created
		if (m_psSecWnd1)
		{
			//the corresponding pointer is set
			m_psSecWnd2 = pView;
			//the size and position are set, and the window is set to be
			//managed 
			pView->setGeometry(RealRect.left(),RealRect.top()-CDataMngr::GetTitleOffset(),RealRect.width(),RealRect.height());
			CWdwMngr::AddWindow(m_psSecWnd2,"AUX_RDR", CWFAttrib::AUX_RDR, IsCollapse);
			//the range marker, if any is repositionned
			if (!OnlyCreate)
			{
				if (CRangeMark::GetCRangeMarkView(pView)!=NULL)
					CRangeMark::GetCRangeMarkView(pView)->SetInitialPosition(RealRect);
			}
		}
		else
		{
			//The first auxiliary is created
			//the corresponding pointer is set
			m_psSecWnd1 = pView;
			//the size and position are set, and the window is set to be
			//managed  
			pView->setGeometry(RealRect.left(),RealRect.top()-CDataMngr::GetTitleOffset(),RealRect.width(),RealRect.height());
			CWdwMngr::AddWindow(m_psSecWnd1,"AUX_RDR",CWFAttrib::AUX_RDR, IsCollapse);
			//the range marker, if any is repositionned
			if (!OnlyCreate)
			{
				if (CRangeMark::GetCRangeMarkView(pView)!=NULL)
					CRangeMark::GetCRangeMarkView(pView)->SetInitialPosition(RealRect);
			}
		}
		//The information to be displayed and the settings are retrieved from the parent
		//radar window
		if (!OnlyCreate)
		{
			pos1=RealRect.bottomLeft();
			pos2=RealRect.topRight();
			pos1=mapFromGlobal(pos1);
			pos2=mapFromGlobal(pos2);
			RealRect=QRect(pos1,pos2);
			DPtoLP(&RealRect);
			pView->SetRange((RealRect.width()+32)/64);
			pView->SetCentre(RealRect.center());
			if (pCurView)
			{
				CVisuMap::CopyView(pView,pCurView);
				for (int i=0;i<=fld_tag_gsp;i++)
					pView->m_Params[i]=pCurView->m_Params[i];
				if (pCurView->m_QlMinusTimer)
					pView->m_Params[filt_lower_on]=m_psMainWnd->m_PrevDownFilter;
				if (pCurView->m_QlPlusTimer)
					pView->m_Params[filt_upper_on]=m_psMainWnd->m_PrevUpFilter;
				if (pCurView->m_QlMinusTimer || pCurView->m_QlPlusTimer)
				{
					pView->m_Params[filt_prim_on]=m_psMainWnd->m_PrevPrimFilter;
					pView->m_Params[filt_db_on]=m_psMainWnd->m_PrevDataFilter;
				}
				pView->m_TrackControl.Init();
				pView->m_HeightFilter.Init();
				pView->UpdateStatusBar();
				pTrkMngr->SetToggledOptionalFieldsAsParent(pCurView,//parent view
																pView);//created view
			}
			pView->OnUpdate(NULL,0,NULL);
			CWdwMngr::ShowWindow(pView,CWFAttrib::AUX_RDR);
			CTOORArrow::UpdateFromView(pView);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time a started timer of the class reaches its
///  value. 
///  Argument : QTimerEvent*, The timer event.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppGen060, CIAsdFctRwsTppGen061, CIAsdFctRwsTppGen080, 
///					 CIAsdFctRwsTppGen081, CIAsdFctRwsTppGen095
//
//-----------------------------------------------------------------------------
void CRwdView::timerEvent ( QTimerEvent * e)
{
	//Implicit focus timer
	if ((e->timerId()==m_TimerId) && (hasMouse()))
	{
		//the currently selected graphical element is searched
		int curprio = 0,prio=0;
		QPoint point=QCursor::pos();
		point=mapFromGlobal(point);
		CDisplayElement* pElem=NULL;
		//Check if the current is still selected
		if (m_pCurElem)
		{
			curprio=m_pCurElem->GetPriority(point,true);
			if (curprio>0)
				pElem=m_pCurElem;
		}
		//Get the graphical element for which the returned priority is the highest
		for (int elem=m_ElemTable.GetSize()-1;elem>=0;elem--)
		{
			prio=m_ElemTable[elem]->GetPriority(point);
			if (prio>curprio)
			{
				curprio=prio;
				pElem = m_ElemTable[elem];
			}
		}
		//The highest element becomes selected and the former is deselected 
		if (m_pCurElem!=pElem)
		{
			if (m_pCurElem)
				m_pCurElem->SelectElement(false);
			if (pElem)
				pElem->SelectElement(true);
			m_pCurElem=pElem;
		}
		
		killTimer(m_TimerId);
		m_TimerId=0;
	}
	//Quick look + timer
	if (e->timerId()==m_QlPlusTimer)
	{
		//The former values of the filters are retrieved and applied only if 
		//the Quick look - option is not activated
		killTimer(m_QlPlusTimer);
		m_QlPlusTimer=0;
		m_Params[filt_upper_on]=m_PrevUpFilter;
		if (!m_QlMinusTimer)
		{
			m_Params[filt_prim_on]=m_PrevPrimFilter;
			m_Params[filt_db_on]=m_PrevDataFilter;
		}
		OnUpdate(NULL,0,NULL);
	}
	//Quick look - timer
	if (e->timerId()==m_QlMinusTimer)
	{
		//The former values of the filters are retrieved and applied only if 
		//the Quick look + option is not activated
		killTimer(m_QlMinusTimer);
		m_QlMinusTimer=0;
		m_Params[filt_lower_on]=m_PrevDownFilter;
		if (!m_QlPlusTimer)
		{
			m_Params[filt_prim_on]=m_PrevPrimFilter;
			m_Params[filt_db_on]=m_PrevDataFilter;
		}
		OnUpdate(NULL,0,NULL);
	}
	//Mouse move event timer
	if (e->timerId()==m_MouseTimerId)
	{
		QPoint pos1=QCursor::pos();
		QPoint pos=mapFromGlobal(pos1);
		QMouseEvent e(QEvent::MouseMove,pos,0,0);
		mouseMoveEvent(&e);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the value of the parameter put in argument. 
///  Argument : int, parameter number
/// 
///  RETURNS: 
///  int, value of the parameter
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CRwdView::GetViewParam(int ParamNb)
{
	return m_Params[ParamNb];
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the value of the parameter put in argument. 
///  Argument 1 : int, parameter number
///  Argument 2 : int, parameter value to be applied
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::SetViewParam(int ParamNb,int ParamValue)
{
	if (m_sIsIndependent)
	{
		if (m_Params[ParamNb]!=ParamValue)
		{
			m_Params[ParamNb]=ParamValue;
			OnUpdate(NULL,0,NULL);
			UpdateStatusBar();
		}
	}
	else
	{
		if (m_psMainWnd)
		{
			if (m_psMainWnd->m_Params[ParamNb]!=ParamValue)
			{
				m_psMainWnd->m_Params[ParamNb]=ParamValue;
				m_psMainWnd->OnUpdate(NULL,0,NULL);
				m_psMainWnd->UpdateStatusBar();
			}
		}
		if (m_psSecWnd1)
		{
			if (m_psSecWnd1->m_Params[ParamNb]!=ParamValue)
			{
				m_psSecWnd1->m_Params[ParamNb]=ParamValue;
				m_psSecWnd1->OnUpdate(NULL,0,NULL);
				m_psSecWnd1->UpdateStatusBar();
			}
		}
		if (m_psSecWnd2)
		{
			if (m_psSecWnd2->m_Params[ParamNb]!=ParamValue)
			{
				m_psSecWnd2->m_Params[ParamNb]=ParamValue;
				m_psSecWnd2->OnUpdate(NULL,0,NULL);
				m_psSecWnd2->UpdateStatusBar();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function loads the radar window settings from a custom set, or saves the 
///  current settings in the custom set. 
///  Argument 1 : CIniFile*, custom set file
///  Argument 2 : QString, Section name impacted in the custom set file
///  Argument 3 : bool, whether the custom set is read or saved 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsAre012, CIAsdFctRwsAre013, CIAsdFctRwsMai005,
///					CIAsdFctRwsMai015, CIAsdFctRwsMai020, CIAsdFctRwsAux020
//
//-----------------------------------------------------------------------------
void CRwdView::Setup(CIniFile* pFile,QString Section,bool Save)
{
	// CDS BREAK NGT 08/06/2006 Bad method length rule is not violated considering
	//							only the real length of the function.

	QString ParamNames[]= {"filt_lower_on","filt_lower_val","filt_upper_on","filt_upper_val","filt_dep_on","filt_dep_val",
		"filt_prim_on","filt_vfr_on","filt_mil_on","filt_db_on",
		"pvect_uncpl","pvect_coupl","pvect_time","pvect_length","hist_length","halo_length","font_size",
		"fld_fdb_modes","fld_fdb_atyp","fld_fdb_rwy","fld_fdb_gsp","fld_fdb_rocd","fld_fdb_rvr","fld_fdb_ades","fld_fdb_sid",
		"fld_ldb_modes","fld_ldb_atyp","fld_ldb_rwy","fld_ldb_gsp","fld_ldb_rocd","fld_ldb_rvr",
		"fld_unc_modes","fld_unc_gsp","fld_unc_rocd",
		"fld_tag_modes","fld_tag_atyp","fld_tag_gsp","fld_tag_rocd"};

	QString Prefix,AllKey;
	int Range,X,Y,cx,cy,screennb,visible,isCollapsed;
	QPoint windowpos;
	bool main=false;
	QSize WindowSize;
	//Sets the prefix of the information to be read or written according to the
	//current radar window
	if (m_psMainWnd==this)
	{
		Prefix="MAIN_";
		main=true;
	}
	if (m_psSecWnd1==this)
		Prefix="AUX1_";
	if (m_psSecWnd2==this)
		Prefix="AUX2_";

	//The custom set is edited
	if (Save)
	{
		windowpos=pos();
		screennb=CAsdApp::MapToScreen(this,windowpos);
		if (CWdwMngr::IsWindowCollapsed(this))
			isCollapsed=1;
		else
			isCollapsed=0;
		WindowSize =CWdwMngr::GetDecollapseSize(this);
		//write the visibility, screen number, collapse status, window position
		//and window size of the main radar window
		AllKey.sprintf("%d,%d,%d,%d,%d,%d,%d",isShown(),screennb,isCollapsed,windowpos.x(),windowpos.y(),WindowSize.width(),WindowSize.height());
		pFile->WriteIniProfileString(Section,Prefix+"WND",AllKey);
		//If no first aux window is defined, the following line is added in the file
		if (main && !m_psSecWnd1)
			pFile->WriteIniProfileString(Section,"AUX1_WND","0,0,0,0,0,0,0");
		//If no second aux window is defined, the following line is added in the file
		if (main && !m_psSecWnd2)
			pFile->WriteIniProfileString(Section,"AUX2_WND","0,0,0,0,0,0,0");
		//write the range and the center
		AllKey.sprintf("%d,%d,%d",GetRange(),GetCentre().x(),GetCentre().y());
		pFile->WriteIniProfileString(Section,Prefix+"RANGE_CENTRE",AllKey);
		//write all the attributes
		for (int prm=filt_lower_on;prm<=fld_tag_rocd;prm++)
		{
			AllKey.sprintf("%d",m_Params[prm]);
			pFile->WriteIniProfileString(Section,Prefix+ParamNames[prm].upper(),AllKey);
		}
		//write the displayed maps list
		CVisuMap::Setup(this,AllKey,Save);
		pFile->WriteIniProfileString(Section,Prefix+"MAPS",AllKey);
	}
	else
	{
		//The custom set is read
		//retrieve the info according to the prefix
		pFile->GetIniProfileString(Section,Prefix+"WND",AllKey);
		sscanf(AllKey,"%d,%d,%d,%d,%d,%d,%d",&visible,&screennb,&isCollapsed,&X,&Y,&cx,&cy);
		//Main radar window treatment
		if (main)
		{
			if (visible)
			{
				//retrieved size and position
				QDesktopWidget* pDesk=CAsdApp::GetApp()->desktop();
				QRect rect=pDesk->screenGeometry(pDesk->primaryScreen());
				if (cx > rect.width()-8)
					cx=rect.width()-8;
				if (cy > rect.height()-8)
					cy=rect.height()-8;

				//apply the size and position according to the collapse attribute
				if (CAsdApp::GetApp()->IsMainCollapsible())
				{
					windowpos=QPoint(X,Y);
					CAsdApp::MapFromScreen(screennb,windowpos);
					cy = cy -CDataMngr::GetTitleOffset();
					CWdwMngr::SetWindowSize(m_psMainWnd,cx,cy);

					if (m_MainWasCollapsible)
						m_psMainWnd->setGeometry(windowpos.x(),windowpos.y()+CDataMngr::GetTitleOffset(),cx,cy);
					else
						m_psMainWnd->setGeometry(windowpos.x(),windowpos.y(),cx,cy);
					m_MainWasCollapsible=true;
				}
				else 
				{
					//If the main radar window is not collapsible, it is displayed
					//full screen in the main screen
					CWdwMngr::SetWindowSize(m_psMainWnd,cx,cy);
					rect.setWidth(cx);
					rect.setHeight(cy);
					m_psMainWnd->setGeometry(rect);
					m_MainWasCollapsible=false;
				}
				CWdwMngr::ShowCustomSetWindow(m_psMainWnd,CWFAttrib::MAIN_RDR, isCollapsed);
			}
			else
				//if not visible the main is hidden
				CWdwMngr::HideWindow(m_psMainWnd);
			//If the first auxiliary radar window is to be displayed, it is created
			if (main && !m_psSecWnd1)
			{
				pFile->GetIniProfileString(Section,"AUX1_WND",AllKey);
				sscanf(AllKey,"%d,%d,%d,%d,%d,%d,%d",&visible,&screennb,&isCollapsed,&X,&Y,&cx,&cy);
				if (visible)
				{
					CreateSecondaryWnd(QRect(X,Y,cx,cy),NULL,true,isCollapsed);
					windowpos=QPoint(X,Y);
					CAsdApp::MapFromScreen(screennb,windowpos);
					CWdwMngr::SetWindowSize(m_psSecWnd1,cx,cy);
					m_psSecWnd1->setGeometry(windowpos.x()+4,windowpos.y(),cx,cy);
					CWdwMngr::ShowCustomSetWindow(m_psSecWnd1,CWFAttrib::AUX_RDR, isCollapsed);
				}

			}
			//If the second auxiliary radar window is to be displayed, it is created
			if (main && !m_psSecWnd2)
			{
				pFile->GetIniProfileString(Section,"AUX2_WND",AllKey);
				sscanf(AllKey,"%d,%d,%d,%d,%d,%d,%d",&visible,&screennb,&isCollapsed,&X,&Y,&cx,&cy);
				if (visible)
				{
					CreateSecondaryWnd(QRect(X,Y,cx,cy),NULL,true,isCollapsed);
					windowpos=QPoint(X,Y);
					CAsdApp::MapFromScreen(screennb,windowpos);
					m_psSecWnd2->setGeometry(windowpos.x()+4,windowpos.y(),cx,cy);
					CWdwMngr::ShowCustomSetWindow(m_psSecWnd2,CWFAttrib::AUX_RDR, isCollapsed);
				}

			}
		}
		//Auxiliary radar windows treatment
		else
		{
			//if not visible in the loaded custom set,the radar window is destroyed
			if (!visible)
			{
				delete this;
				return;
			}
			else
			{
				//The window position, size and collapse status are applied
				windowpos=QPoint(X,Y);
				CAsdApp::MapFromScreen(screennb,windowpos);
				if (m_psSecWnd1==this) 
				{
					if (!isCollapsed)
				  		m_psSecWnd1->setGeometry(windowpos.x()+4,windowpos.y()+CDataMngr::GetTitleOffset(),cx,cy);
					else
						m_psSecWnd1->move(windowpos.x()+4,windowpos.y());
					CWdwMngr::SetCollapse(m_psSecWnd1,isCollapsed);
				}
				if (m_psSecWnd2==this)
				{
					if (!isCollapsed)
						m_psSecWnd2->setGeometry(windowpos.x()+4,windowpos.y()+CDataMngr::GetTitleOffset(),cx,cy);
					else
						m_psSecWnd2->move(windowpos.x()+4,windowpos.y());
					CWdwMngr::SetCollapse(m_psSecWnd2,isCollapsed);
				}
				CWdwMngr::MoveWindow();
			}
		}
		//The range, center and all settings are retrieved and applied
		pFile->GetIniProfileString(Section,Prefix+"RANGE_CENTRE",AllKey);
		sscanf(AllKey,"%d,%d,%d",&Range,&X,&Y);
		if ((m_psMainWnd==this) || (m_psSecWnd1==this) || (m_psSecWnd2==this))
        {
			SetRange(Range);
			SetCentre(QPoint(X,Y));
			OnUpdate(NULL,0,NULL);
			m_DefaultRange=Range;
			m_DefaultCentre=QPoint(X,Y);
			for (int prm=filt_lower_on;prm<=fld_tag_rocd;prm++)
			{
				pFile->GetIniProfileString(Section,Prefix+ParamNames[prm].upper(),AllKey);
				if (((CString)AllKey).isNumeric())
					m_Params[prm]=atoi(AllKey);
			}
			m_TrackControl.Init();		
			m_HeightFilter.Init();
			pFile->GetIniProfileString(Section,Prefix+"MAPS",AllKey);
			CVisuMap::Setup(this,AllKey,Save);
			UpdateStatusBar();
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when the "Quick Look +" menu item is selected. 
///  It cancels the filters for the current view. 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::OnQuickLookPlus()
{
	//If no quick look option is activated
	if (!m_QlPlusTimer && !m_QlMinusTimer)
	{
		//the current values are saved and canceled
		m_PrevUpFilter=m_Params[filt_upper_on];
		m_PrevPrimFilter=m_Params[filt_prim_on];
		m_PrevDataFilter=m_Params[filt_db_on];
		m_Params[filt_upper_on]=0;
		m_Params[filt_prim_on]=0;
		m_Params[filt_db_on]=0;
		OnUpdate(NULL,0,NULL);
		m_QlPlusTimer=startTimer(5000);
	}
	else if (!m_QlPlusTimer)
	{
		//If only the Quick Look - option is activated, only the upper filter is to
		//be canceled
		m_PrevUpFilter=m_Params[filt_upper_on];
		m_Params[filt_upper_on]=0;
		OnUpdate(NULL,0,NULL);
		m_QlPlusTimer=startTimer(5000);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when the "Quick Look -" menu item is selected. 
///  It cancels the filters for the current view. 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::OnQuickLookMinus()
{
	//If no quick look option is activated
	if (!m_QlMinusTimer && !m_QlPlusTimer)
	{
		//the current values are saved and canceled
		m_PrevDownFilter=m_Params[filt_lower_on];
		m_PrevPrimFilter=m_Params[filt_prim_on];
		m_PrevDataFilter=m_Params[filt_db_on];
		m_Params[filt_lower_on]=0;
		m_Params[filt_prim_on]=0;
		m_Params[filt_db_on]=0;
		OnUpdate(NULL,0,NULL);
		m_QlMinusTimer=startTimer(5000);
	}
	else if (!m_QlMinusTimer)
	{
		//If only the Quick Look + option is activated, only the lower filter is to
		//be canceled
		m_PrevDownFilter=m_Params[filt_lower_on];
		m_Params[filt_lower_on]=0;
		OnUpdate(NULL,0,NULL);
		m_QlMinusTimer=startTimer(5000);
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the window framing important track number to be
///  applied to a window according to the radar window put in argument. 
///  Argument 1 : QRect, rect of the tested window
///  Argument 2 : int, screen number of the tested window
///  Argument 3 : CRwdView*, reference radar window
///   
///  RETURNS: 
///  int, the window framing code for the window according to the radar window
///			put in argument
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CRwdView::GetImportantTrackTpView(QRect WndBR, int iScrNb, CRwdView* pTpView)
{
	int iCode=0;
	int iCodeTemp=0;
	CWin_Hints* pCWinHints=CAsdApp::GetApp()->GetWinHints();

	//If the windows are not on the same screen no further check is needed.
	if (iScrNb!=CAsdApp::GetApp()->desktop()->screenNumber(pTpView))
		return 0;
	//If the radar window pTpView is collapsed no further check is needed
	QRect rTranslate=pCWinHints->GetXWindowGeometry(pTpView);
	if (rTranslate.height()<40)
		return 0;

	//If both bounding rectangle does not intersect no further check is needed
	if (!rTranslate.intersects(WndBR))
		return 0;

	//We need to look for every track containing any emergency information.
	for (int elem=0;elem<pTpView->m_ElemTable.GetSize();elem++)
	{
		if (pTpView->m_ElemTable[elem]->m_Type==2)
		{
			iCodeTemp=pTpView->m_ElemTable[elem]->IsImportant();
			if (iCodeTemp>iCode)
			{
				//Check the track PPS bounding rect with the Window Bounding Rect.
				QRect TrackBRtmp=QRect();
				pTpView->m_ElemTable[elem]->GetSymbRect(TrackBRtmp);
				if (pTpView!=m_psMainWnd || (CAsdApp::GetApp()->IsMainCollapsible()))
					TrackBRtmp=QRect(TrackBRtmp.x(),TrackBRtmp.y()-2+CDataMngr::GetTitleOffset(),TrackBRtmp.width(),TrackBRtmp.height());
				QRect TrackBR;
				TrackBR.setCoords(TrackBRtmp.left()+rTranslate.left(), TrackBRtmp.top()+rTranslate.top(),
							      TrackBRtmp.right()+rTranslate.left(), TrackBRtmp.bottom()+rTranslate.top());

				if (TrackBR.intersects(WndBR))
					iCode=iCodeTemp;
			}
		}
		//If the code found is the maximum, no further check is needed
		if (iCode==4)
			break;
	}
	return iCode;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the window framing important track number to be
///  applied to a window according to all the radar windows. 
///  Argument 1 : QWidget*, widget to be tested
///  Argument 2 : QRect, rect of the tested window
///  Argument 3 : int, screen number of the tested window
///   
///  RETURNS: 
///  int, the window framing code for the window according to all the radar windows
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CRwdView::GetImportantTrackUnderneath(QWidget* w,QRect WndBR, int iScrNb)
{
	int iCodeMain=0;
	int iCodeSec1=0;
	int iCodeSec2=0;
	//If no main radar window is available, no auxiliary can be defined, the 
	//function returns 0
	if (!m_psMainWnd)
		return 0;

	//Get the framing code for the main radar window
	iCodeMain=GetImportantTrackTpView(WndBR,iScrNb,m_psMainWnd);
	//If the code found is the maximum, no further check is needed
	if (iCodeMain==4) 
		return 4;
	int zw = CWdwMngr::GetZOrder(w);

	//Get the framing code for the first auxiliary radar window
	if ((m_psSecWnd1) && (w!=m_psSecWnd1))
	{
		int zsw = CWdwMngr::GetZOrder(m_psSecWnd1);
		if (zw == -1 || zsw == -1 || zw < zsw)
			iCodeSec1=GetImportantTrackTpView(WndBR,iScrNb,m_psSecWnd1);
	}
	//If the code found is the maximum, no further check is needed
	if (iCodeSec1==4) 
		return 4;

	//Get the framing code for the second auxiliary radar window
	if ((m_psSecWnd2) && (w!=m_psSecWnd2))
	{
		int zsw = CWdwMngr::GetZOrder(m_psSecWnd2);
		if (zw == -1 || zsw == -1 || zw < zsw)
			iCodeSec2=GetImportantTrackTpView(WndBR,iScrNb,m_psSecWnd2);
	}
	return _max(iCodeMain,_max(iCodeSec2,iCodeSec1));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to close any opened radar menu. 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::RemoveAllRadarMenu()
{
	if (m_psMainWnd)
		m_psMainWnd->RemoveRadarMenu();
	if (m_psSecWnd1)
		m_psSecWnd1->RemoveRadarMenu();
	if (m_psSecWnd2)
		m_psSecWnd2->RemoveRadarMenu();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to close radar menu associated to the current radar
///  window, if opened. 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::RemoveRadarMenu()
{
  if (m_RadarMenu.IsMenuOpen())
    m_RadarMenu.StopTracking();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to sets the Offline Maps cascading menu availability
///  to the one put in argument
///  Argument : bool, whether the Offline maps cascading menu is available or not
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::SetMapsAvailable(bool IsAllowed)
{
	if (m_psMainWnd)
		m_sIsOnMapReload=IsAllowed;
        if (m_psSecWnd1)
		m_sIsOnMapReload=IsAllowed;
        if (m_psSecWnd2)
		m_sIsOnMapReload=IsAllowed;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether one of the opened radar window is in mode panning
///  or zooming or not
///   
///  RETURNS: 
///  bool, whether one radar window has a mode zoom or panning activated or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CRwdView::IsInViewModif()
{
	if (m_psMainWnd)
	{
		if (m_psMainWnd->m_Mode!=Normal)
			return true;
	}
	if (m_psSecWnd1)
	{
		if (m_psSecWnd1->m_Mode!=Normal)
			return true;
	}
	if (m_psSecWnd2)
	{
		if (m_psSecWnd2->m_Mode!=Normal)
			return true;
	}
	return false;	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function closes all the opened Height Filter windows
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwdView::CloseAllHFW()
{
	if (m_psMainWnd)
	{
		if ((m_psMainWnd->m_HeightFilter).isVisible())
			CWdwMngr::HideWindow(&m_psMainWnd->m_HeightFilter);
	}
	if (m_psSecWnd1)
	{
		if ((m_psSecWnd1->m_HeightFilter).isVisible())
			CWdwMngr::HideWindow(&m_psSecWnd1->m_HeightFilter);
	}
	if (m_psSecWnd2)
	{
		if ((m_psSecWnd2->m_HeightFilter).isVisible())
			CWdwMngr::HideWindow(&m_psSecWnd2->m_HeightFilter);
	}
}

//--------------------------------< THE END >----------------------------------
