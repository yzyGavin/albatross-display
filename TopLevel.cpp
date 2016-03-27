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
//  FILENAME: TopLevel.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CTopLevel which manages the Top Level Menu Bar.
//		This includes, the pull down menus, the TLM Lock feature and the alert 
//		indications for safety nets and ASD System Modes.
//				
// 
//  COMMENTS
//		There is one declared instance of CTopLevel per screen, but only one 
//		instance of window invocable from the TLM for the application.
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
#include "AsdApp.h"
#include "AsdMenu.h"
#include "TopLevel.h"
#include "FontMngr.h"
#include "ColorMngr.h"
#include "WdwMngr.h"
#include "View.h"
#include "AsdView.h"
#include "DisplayElement.h"
#include "VisuMap.h"
#include "RwdView.h"
#include "Elements.h"
#include "Track.h"
#include "SetupMngr.h"
#include "./CommonFiles/IniFile.h"
#include "WinHints.h" 
#include "UserMngr.h"
#include "TimeWnd.h"
#include "InfoMngr.h"
#include "SystemMode.h"
#include "FlightPlan.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-GLOBAL DATA-----------------------------------------------------------------

#define ID_SIGNIN 101
#define ID_SIGNOUT 102
#define ID_CHANGEPASSWORD 103
#define ID_CUSTOMSET 104
#define ID_DEFAULTLAYOUT 105
#define ID_ROCDUNIT 106
#define ID_BRIGHTNESS 107
#define ID_MAINRADAR 111
#define ID_INDEPENDENT 117 
#define ID_WEATHER 127
#define ID_SYSINFO 136
#define ID_STOP 137
#define ID_FINDTRACK 148
#define ID_BL3NM 191
#define ID_TR3NM 192
#define ID_BL5NM 193
#define ID_TR5NM 194
#define ID_FULLCSULIST 180
#define ID_COUPLEDCSULIST 181
#define ID_MODESCSULIST 182
#define ID_TAGCSULIST 183
#define ID_BINCSULIST 184
#define ID_FONTLISTSMALL 140
#define ID_FONTLISTMEDIUM 141
#define ID_FONTLISTBIG 142

CColorDlg* CTopLevel::m_psColor = NULL;
CFindTrackDlg* CTopLevel::m_psFindTrack = NULL;
CustomizationSetDialog* CTopLevel::m_psCustomDlg = NULL;
CSystemInfo* CTopLevel::m_psSystemInfo = NULL;
CSSaveAs* CTopLevel::m_psSaveAs=NULL;
CArray<CTopLevel*,CTopLevel*> CTopLevel::m_sWndTable;

//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CTopLevel
///  This class manages the Top Level Menu bars presentation and interactions.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the Brightness Control window pointer.
/// 
///  RETURNS: 
///  CColorDlg*, Brightness Control Window widget pointer
///
///  REQUIREMENT ID: CIAsdFctBcw004 (cf.: Brightness.ui)
///					 CIAsdFctBcw005 (cf.: Brightness.ui)
//
//-----------------------------------------------------------------------------
CColorDlg* CTopLevel::GetColorDlg()
{
	//If the pointer is not valid, the window is initialized
	if (!m_psColor)
	{	
		m_psColor = new CColorDlg;
		CWdwMngr::AddWindow(m_psColor,"BRGHT_CTRL",CWFAttrib::BRGHT_CTRL,false);
		m_psColor->Init();
		CAsdApp::GetApp()->WindowMoveDefault(m_psColor, "BRGHT_CTRL", 0);
	}
	return m_psColor;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the Find Track window pointer.
/// 
///  RETURNS: 
///  CFindTrackDlg*, Find Track Window widget pointer
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
CFindTrackDlg* CTopLevel::GetFindTrackDlg()
{
	//If the pointer is not valid, the window is initialized
	if (!m_psFindTrack)
	{	
		m_psFindTrack = new CFindTrackDlg;
		CWdwMngr::AddWindow(m_psFindTrack,"FIND_TRACK",CWFAttrib::FIND_TRACK,false);
		CAsdApp::GetApp()->WindowMoveDefault(m_psFindTrack, "FIND_TRACK", 0);
	}
	m_psFindTrack->Init();
	return m_psFindTrack;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the Custom Set window pointer.
/// 
///  RETURNS: 
///  CustomizationSetDialog*, Custom Set Window widget pointer
///
///  REQUIREMENT ID: CIAsdFctCzs055 (cf.: CustomSetDialog.ui)
//
//-----------------------------------------------------------------------------
CustomizationSetDialog* CTopLevel::GetCustomizationSetDialog()
{
	//If the pointer is not valid, the window is initialized
	if (!m_psCustomDlg)
	{
		m_psCustomDlg = new CustomizationSetDialog;
		CWdwMngr::AddWindow(m_psCustomDlg,"CUSTOM_SET", CWFAttrib::CUSTOM_SET,false);
		//The Save As window is associated to the custom set window
		m_psSaveAs = new CSSaveAs();
		CWdwMngr::AddWindow(m_psSaveAs,"SAVE_AS",CWFAttrib::SAVE_AS, false);
		CAsdApp::GetApp()->WindowMoveDefault(m_psCustomDlg, "CUSTOM_SET", 0);
	}
	return m_psCustomDlg;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the System Information window pointer.
/// 
///  RETURNS: 
///  CSystemInfo*, System Information Window widget pointer
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CSystemInfo* CTopLevel::GetSystemInfo()
{
	//If the pointer is not valid, the window is initialized
	if (!m_psSystemInfo)
	{
		m_psSystemInfo = new CSystemInfo;
		CWdwMngr::AddWindow(m_psSystemInfo,"SYS_INFO",CWFAttrib::SYS_INFO,false);
		CAsdApp::GetApp()->WindowMoveDefault(m_psSystemInfo, "SYS_INFO", 0);
	}
	return m_psSystemInfo;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters, initialization
///  of the TopLevel menu bar, presentation of the widget.
///  Argument 1 : CTopLevelEventStub*, pointer to the CEventHandler class link
///  Argument 2 : QRect, the rect of the Top Level widget 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTopLevel::CTopLevel(CTopLevelEventStub* pHEvent, QRect qrScreenSize):
	QWidget(0,0,WType_Dialog|WStyle_Customize  |WStyle_NoBorder),
	m_Btn(this,0)
{
	m_pHEvent = pHEvent;
	setGeometry(qrScreenSize);
#ifdef ENABLE_QDEBUG
  // qDebug("screen nber dans TopLevel  %d and size %d %d %d %d", this, qrScreenSize.x(), qrScreenSize.y(), qrScreenSize.width(), qrScreenSize.height());
#endif
    m_iVerticalTextOffset=-4;
	m_FirstPos=50;
	m_CurSel=-1;
	m_AutoHide=true;
	m_PrevHideStatus=true;
	m_PosY=0;
	m_TimerId=0;
	setMouseTracking(true);
	m_MenuRect=QRect();
	int curindex;

	//Settings pull down menu
	CAsdMenu* pMenuSettings= new CAsdMenu();
	pMenuSettings->AddItem("Sign In...",ID_SIGNIN,true);
	pMenuSettings->AddItem("Sign Out",ID_SIGNOUT,true,false,true);
	pMenuSettings->AddItem("Change Password...",ID_CHANGEPASSWORD,true,false,true);
	pMenuSettings->InsertSeparator();
	pMenuSettings->AddItem("Custom Sets...",ID_CUSTOMSET,true,false,true);
	pMenuSettings->AddItem("Default Layout",ID_DEFAULTLAYOUT,true);
	pMenuSettings->InsertSeparator();
	pMenuSettings->AddItem("ROC/D in Ft/Nm",ID_ROCDUNIT,false);
	pMenuSettings->InsertSeparator();
	pMenuSettings->AddItem("Brightness Control...",ID_BRIGHTNESS,true);
	m_Menu.AddItem("Settings",pMenuSettings);

	//AirSitu pull down menu
	CAsdMenu* pMenuAirSitu= new CAsdMenu();
	pMenuAirSitu->AddItem("Main Radar Window...",ID_MAINRADAR,true);
	pMenuAirSitu->AddItem("Independent",ID_INDEPENDENT, false);
	pMenuAirSitu->InsertSeparator();

	CAsdMenu* pMenuTools = new CAsdMenu();
	pMenuTools->AddItem("FindTrack...",ID_FINDTRACK,true);
	CAsdMenu* pMenuMarker= new CAsdMenu();
	pMenuMarker->AddItem("BL 3Nm",ID_BL3NM,true);	
	pMenuMarker->AddItem("TR 3Nm",ID_TR3NM,true);
	pMenuMarker->InsertSeparator();
	pMenuMarker->AddItem("BL 5Nm",ID_BL5NM,true);
	pMenuMarker->AddItem("TR 5Nm",ID_TR5NM,true);
	pMenuTools->AddItem("Range Markers",pMenuMarker);

	curindex=pMenuAirSitu->AddItem("Tools",pMenuTools);
	pMenuAirSitu->InsertSeparator();

	CAsdMenu* pMenuRadar= new CAsdMenu();
	pMenuAirSitu->AddItem("Radar Service",pMenuRadar);
	pMenuAirSitu->AddItem("Weather",ID_WEATHER, false);
	pMenuAirSitu->InsertSeparator();

	CAsdMenu* pCsuList = new CAsdMenu();
	pCsuList->AddItem("FULL LIST",ID_FULLCSULIST,true);
	pCsuList->InsertSeparator();
	pCsuList->AddItem("COUPLED FPL LIST",ID_COUPLEDCSULIST,true);
	pCsuList->AddItem("MODE S TAGGED FPL LIST",ID_MODESCSULIST,true);
	pCsuList->AddItem("TAGGED FPL LIST",ID_TAGCSULIST,true);
	pCsuList->InsertSeparator();
	pCsuList->AddItem("BIN FPL LIST",ID_BINCSULIST,true);
	curindex=pMenuAirSitu->AddItem("CSU Traffic List",pCsuList);

	CAsdMenu* pFontList = new CAsdMenu();
	pFontList->AddItem("Small",ID_FONTLISTSMALL,true);
	pFontList->AddItem("Medium",ID_FONTLISTMEDIUM,true);
	pFontList->AddItem("Big",ID_FONTLISTBIG,true);

	curindex=pMenuAirSitu->AddItem("FPL List Font",pFontList);


	pMenuAirSitu->SetDisabled(curindex,false);
	m_Menu.AddItem("AirSitu",pMenuAirSitu);

	//Info pull down menu
	CAsdMenu* pMenuInfo = new CAsdMenu();
	pMenuInfo->AddItem("System Info...",ID_SYSINFO,true);
	pMenuInfo->AddItem("Stop...",ID_STOP,true);
	m_Menu.AddItem("Info",pMenuInfo);

	//Presentation of the widget
	int offsetX=6,offsetY=6;
	m_TopItem curitem;
	QPainter dc(this);
	int fontnb=CFontMngr::GetFontNb("MENU");
	QFont font= CFontMngr::GetFont(fontnb);
	dc.setFont(font);
	int curpos=m_FirstPos;
	curitem.IsPushed=false;
	//The table of pull down menu is filled
	for (int i=0;i<m_Menu.GetItemCount();i++)
	{
		curitem.Text=m_Menu.GetItemText(i);
		QSize size=dc.boundingRect(0,0,0,0,Qt::AlignLeft,curitem.Text).size();
		curitem.Rect=QRect(QPoint(curpos,offsetY-2),QPoint(curpos+size.width()+offsetX,offsetY+size.height()+5));
		curpos+=size.width()+offsetX;
		curitem.IsDisabled=m_Menu.IsItemDisabled(i);
		m_TopTable.Add(curitem);
	}
	//The Lock check box is positionned and linked to the slot "toggled"
	m_Btn.setGeometry(QRect(10,10,16,16));
	m_Btn.setChecked(m_AutoHide);
    connect( &m_Btn, SIGNAL( toggled(bool) ), this, SLOT( OnAutoHide(bool) ) );
	//The settings of the top level menu are save and loaded in the custom sets
	CSetupMngr::AddObject(this);
	m_sWndTable.Add(this);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CTopLevel::CTopLevel(CTopLevel* pTopLev):
	QWidget(0,0,WType_Dialog|WStyle_Customize  |WStyle_NoBorder),
	m_Btn(this,0)
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
CTopLevel::~CTopLevel()
{
	//The TopLEvel object is removed from the static table
	CSetupMngr::RemoveObject(this);
	for (int i=0;i<m_sWndTable.GetSize();i++)
	{
		if (m_sWndTable[i]==this)
		{
			m_sWndTable.RemoveAt(i);
			break;
		}
	}
	if (m_psCustomDlg)
	{
		CWdwMngr::RemoveWindow(m_psCustomDlg);
		delete m_psCustomDlg;
	}
	if (m_psSaveAs)
	{
		CWdwMngr::RemoveWindow(m_psSaveAs);
		delete m_psSaveAs;
	}	
	if (m_psSystemInfo)
	{
		CWdwMngr::RemoveWindow(m_psSystemInfo);
        delete m_psSystemInfo;
	}
	if (m_psColor)
	{
		CWdwMngr::RemoveWindow(m_psColor);
        delete m_psColor;
	}

	if (m_psFindTrack)
	{
		CWdwMngr::RemoveWindow(m_psFindTrack);
		delete m_psColor;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to display the window put in argument, at the correct 
///  position in the invocation screen and with the offline defined properties.
///  Argument 1 : QWidget*, widget to be displayed
///  Argument 2 : QString, category of the window to be displayed
///  Argument 3 : int, attribute number of the window
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::ShowWidget(QWidget* w, QString ewName, int attrib)
{
	int widgetScrNb=CAsdApp::GetApp()->desktop()->screenNumber(w);
	//If closed or opened in another screen, the window should opened at the last
	//position in the invocation screen, if any, or at the offline define position
	if (CAsdApp::GetApp()->desktop()->screenNumber(this) != widgetScrNb)
	{
		CAsdApp::GetApp()->getDefTlmPosList()->SetDefaultTlmPos(ewName, w->x(), w->y(), widgetScrNb);
		CAsdApp::GetApp()->WindowMoveDefault(w, ewName, CAsdApp::GetApp()->desktop()->screenNumber(this));
	}
	CWdwMngr::ShowWindow(w,attrib);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function associates the radar cascading menu to the Top Level menu.
///  Argument : CAsdMenu*, pointer to the radar cascading menu
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::SetTrackSourceMenu(CAsdMenu* pMenu)
{
	CAsdMenu* pTrackMenu=new CAsdMenu(*pMenu);
	*pTrackMenu=*pMenu;
	CAsdMenu* pSubMenu=m_Menu.GetPopupMenu(1);
	CAsdMenu* pOldMenu=pSubMenu->SetPopupMenu(5,pTrackMenu);
	delete pOldMenu;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function manages the lock status of the Top Level menu bar. The lock 
///  status is set to the status put in argument
///  Argument : bool, lock status to be applied
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::SetAutoHide(bool Auto)
{
	//If the lock status is changed
	if (m_AutoHide!=Auto)
	{
		m_AutoHide=Auto;
		//If no alert or system mode is displayed in the Top Level menu bar
		//the previous lock status is saved in order to be retrieved if the 
		//lock becomes forced
		if (m_SysModelabel.isEmpty())
			m_PrevHideStatus=m_AutoHide;
		//If the bar is not locked the position of the mouse pointer is checked 
		//every 500ms 
		if (!m_AutoHide)
		{
			m_TimerId=startTimer(500);
		}
		else
		{
			killTimer(m_TimerId);
			m_TimerId=0;
			show();
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to move horizontally the menu items in order for them  
///  to not overlap with the Time window
///  Argument : int, offset to be applied
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::SetOffsetX(int offset)
{
	int diff=offset-m_FirstPos;
	for (int i=0;i<m_TopTable.GetSize();i++)
	{
		m_TopTable[i].Rect.addCoords(diff,0,diff,0);
		m_MenuRect|=m_TopTable[i].Rect;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to set the height of the Top Level menu bar to the
///  height of the Time Window. The position of the pull down menu items is then
///  recalculated
///  Argument : int, height to be applied
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::SetHeight(int height)
{
	int sizeY=0,offsetY=0,i;
	for (i=0;i<m_TopTable.GetSize();i++)
	{
		if (m_TopTable[i].Rect.height()>sizeY)
			sizeY=m_TopTable[i].Rect.height();
	}
	//If the new height is higher than the old
	if (height>sizeY)
		offsetY=(height-sizeY)/2;
	//The new position is applied
	for (i=0;i<m_TopTable.GetSize();i++)
	{
		m_TopTable[i].Rect.setTop(offsetY);
		m_TopTable[i].Rect.setBottom(offsetY+sizeY);
		m_MenuRect|=m_TopTable[i].Rect;
	}
	update();
	//The height is applied
	setFixedHeight(height);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to draw the 3D effect around the pull down menu items
///  Argument 1 : QPainter*, drawing device
///  Argument 2 : QRect, rect of the menu item
///  Argument 3 : QColor, top and left border color to be applied
///  Argument 4 : QColor, bottom and right border color to be applied
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::Draw3dRect(QPainter* dc,QRect rect,QColor color1,QColor color2)
{
	QPen prevpen=dc->pen();
	dc->setPen(color1);
	dc->drawLine(rect.bottomLeft(),rect.topLeft());
	dc->drawLine(rect.topLeft(),rect.topRight());
	dc->setPen(color2);
	dc->drawLine(rect.topRight(),rect.bottomRight());
	dc->drawLine(rect.bottomRight(),rect.bottomLeft());
	dc->setPen(prevpen);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the specific QPalette (color coding) put in argument to all the 
///  Top Level check boxes 
///  Argument : QPalette, color coding to be applied to the TLM check boxes
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::SetCheckBoxPalette(QPalette pal)
{
	for (int i=0;i<m_sWndTable.GetSize();i++)
	{
		m_sWndTable[i]->m_Btn.setPalette(pal);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function checks if some System Mode or Safety Nets indications must
///  be displayed or not and if necessary build the text to be displayed
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdSymSmbBsm320, CIAsdSymSmbEsm220
//
//-----------------------------------------------------------------------------
void CTopLevel::ComputeAlarm()
{
	QString oldsmlabel = m_SysModelabel;

	//If the system mode is not Normal,the text to be displayed (offline defined)
	//is retrieved
	if (CAsdApp::GetApp()->GetSystemMode()->GetSystemMode()!=NORMAL)
		m_SysModelabel = CAsdApp::GetApp()->GetSystemMode()->GetSystemModeLabel();
	else
		m_SysModelabel = "";

	//If the indications has changed
	if (oldsmlabel != m_SysModelabel)
	{
		//And no indication is now displayed, the former lock status 
		//of the bar is applied
		if (m_SysModelabel.isEmpty())
		{
			m_Btn.setEnabled(true);
			m_Btn.setChecked(m_PrevHideStatus);
		}
		else
		//And an indication is now displayed, the lock status of the bar is forced
		{
			m_Btn.setEnabled(false);
			m_Btn.setChecked(true);
		}
		update();
		//The order function is called to prevent the Top Level being hiding the Time window
		CWdwMngr::OrderWindows();
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function draws the Top Level Menu Bar.
///  Argument : QPaintEvent, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::paintEvent ( QPaintEvent * )
{
	QPainter dc(this);
	QRect GlobalRect=QWidget::rect(),rect;
	//The rect is filled
	dc.fillRect(GlobalRect,CColorMngr::GetColor(CColorMngr::GetColorNb("TOPLEVEL_MENU_BKGRND")));
	dc.setBackgroundMode(TransparentMode);
	dc.setPen(CColorMngr::GetColor(DefMenuTextClr));
	int fontnb=CFontMngr::GetFontNb("MENU");
	QFont font=CFontMngr::GetFont(fontnb);
	dc.setFont(font);
	//The pull down menu items are drawn according to their selected status
	for (int i=0;i<m_TopTable.GetSize();i++)
	{
		rect=m_TopTable[i].Rect;
		rect.addCoords(3,3,-3,-3);
		//Grayed out menu item
		if (!m_TopTable[i].IsDisabled)
		{
			dc.setPen(CColorMngr::GetColor(DefMenuTextClr));
			dc.drawText(rect.left(),rect.bottom()+m_iVerticalTextOffset,m_TopTable[i].Text);
		}
		else
		{
			dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
			dc.drawText(rect.left()-1,rect.bottom()-1+m_iVerticalTextOffset,m_TopTable[i].Text);
			dc.setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
			dc.drawText(rect.left(),rect.bottom()+m_iVerticalTextOffset,m_TopTable[i].Text);
		}
		rect=m_TopTable[i].Rect;
		//Selected effect
		if ((m_CurSel==i) && (!m_TopTable[i].IsDisabled))
		{
			int lefttopclr=DefWindowLeftTopClr,rightbottomclr=DefWindowRightBottomClr;
			//pushed effect
			if (m_TopTable[i].IsPushed)
			{
				lefttopclr=DefWindowRightBottomClr;
				rightbottomclr=DefWindowLeftTopClr;
			}
			Draw3dRect(&dc,rect,CColorMngr::GetColor(lefttopclr),CColorMngr::GetColor(rightbottomclr));
			rect.addCoords(1,1,-1,-1);
			Draw3dRect(&dc,rect,CColorMngr::GetColor(lefttopclr),CColorMngr::GetColor(rightbottomclr));

		}
		
	}
	int posleft=GlobalRect.right();

	dc.setFont(CFontMngr::GetFontByName("TIME_WINDOW_HS"));
	dc.setPen(CColorMngr::GetColor(CColorMngr::GetColorNb("URGENCY")));

	//System Mode indication
	if (!m_SysModelabel.isEmpty())
	{
		QSize size=dc.boundingRect(0,0,0,0,Qt::AlignLeft,m_SysModelabel).size();
		dc.drawText(posleft-size.width()-5,2+size.height(), m_SysModelabel);
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function closes every opened pull down menu.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::RemoveAllMenu()
{
	for (int i=0;i<m_sWndTable.GetSize();i++)
	{	
		CTopLevel *pTopLevel=m_sWndTable.GetAt(i);

		if(pTopLevel->m_CurSel!=-1)
			pTopLevel->m_TopTable[pTopLevel->m_CurSel].IsPushed=false;

		if (pTopLevel->m_AsdMenu.IsMenuOpen())
			pTopLevel->m_AsdMenu.StopTracking();

		pTopLevel->m_CurSel=-1;
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when the mouse pointer is moved over the Top Level Menu 
///  bar
///  Argument : QMouseEvent, move event description
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::mouseMoveEvent ( QMouseEvent * e )
{
	QPoint point=e->pos();	
	
	int newsel=-1;
	bool waspushed=false;

	//Checks if the mouse pointer is over a menu item rect
	for (int i=0;i<m_TopTable.GetSize();i++)
	{
		QRect rect=m_TopTable[i].Rect;
		if (m_AsdMenu.IsMenuOpen())
			rect.addCoords(0,0,0,10);
		if (rect.contains(point))
		{
			newsel=i;
			break;
		}
	}
	//If the selection has changed, the selection, pushed status of the impacted
	//menu items is recomputed
	if (newsel!=m_CurSel)
	{
		if (m_CurSel!=-1)
		{
			waspushed=m_TopTable[m_CurSel].IsPushed;
			
			if(newsel!=-1)
			{
				m_TopTable[m_CurSel].IsPushed=false;

				if (m_AsdMenu.IsMenuOpen())
				{
					m_AsdMenu.StopTracking();
				}
				m_CurSel=-1;
			}
		}
		if (newsel!=-1)
		{
			if	((waspushed) && (!m_TopTable[newsel].IsDisabled))
				TrackMenu(newsel,false);
			else
			{
				m_CurSel=newsel;
			}
		}
		update(m_MenuRect);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to process a click AB action on a menu item of the bar
///  or the move event over the menu items of the bar. 
///  Argument 1 : UINT, pul down menu item number
///  Argument 2 : BOOL, whether the function is called following a Click AB action or not
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsRsv005
//
//-----------------------------------------------------------------------------
void CTopLevel::TrackMenu (UINT Pos,BOOL bPressEvent)
{
	//If the Pos corresponds to the currently selected menu item, if the menu item 
	//is already pushed and if the user performed a click AB action, the corresponding
	//pull down menu is closed
	if ((m_CurSel==(int)Pos) && (m_TopTable[m_CurSel].IsPushed) && bPressEvent)
	{
		m_TopTable[m_CurSel].IsPushed=false;
		if (m_AsdMenu.IsMenuOpen())
			m_AsdMenu.StopTracking();
		m_CurSel=-1;
	}
	else
	//In the other case the menu item is pushed and the corresponding pull down menu is opened, 
	{
		m_CurSel = Pos;
		m_TopTable[m_CurSel].IsPushed=true;
		QRect rc;
		rc=m_TopTable[m_CurSel].Rect;
		QPoint pos1=rc.topLeft();
		QPoint pos2=rc.bottomRight();
		pos1=mapToGlobal(pos1);
		pos2=mapToGlobal(pos2);
		rc=QRect(pos1,pos2);
		CAsdMenu* tmp = m_Menu.GetPopupMenu(m_CurSel);
		if (tmp)
		{
			m_AsdMenu=*tmp;
			m_AsdMenu.TrackPopupMenu(rc.left(),rc.bottom(),this,&rc);
		}
	}
	update(m_MenuRect);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when the user performs a mouse click. 
///  Argument : QMouseEvent, mouse event description
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::mousePressEvent ( QMouseEvent * e )
{
	QPoint point = e->pos();

	if (e->button()==LeftButton)
	{

		int newsel=-1;
		for (int i=0;i<m_TopTable.GetSize();i++)
		{
			if (m_TopTable[i].Rect.contains(point))
			{
				newsel=i;
				break;
			}
		}
		//If a click AB is performed over a menu item which is not grayed out
		if ((newsel!=-1) && !m_TopTable[newsel].IsDisabled)
			TrackMenu(newsel,true);
	}

}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This slot is called every time the check box is ticked or unticked. 
///  Argument : bool, the tick status of the check box
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::OnAutoHide(bool on) 
{
	SetAutoHide(on);	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time a started timer of the class reaches its
///  value.
///  Argument : QTimerEvent, the timer event.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::timerEvent ( QTimerEvent * e)
{
	QPoint pnt;
	QRect rect;
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	//Top Level visibility check
	if (e->timerId()==m_TimerId)
	{
		pnt=QCursor::pos();
		rect=frameGeometry();
		//When the lock status of the Top Level is false
		//If the mouse pointer is at the top of the screen, the Top Level Menu is displayed
		//otherwise it is hidden
		if ((rect.contains(pnt)) || !m_SysModelabel.isEmpty())
		{
			show();
			CWdwMngr::OrderWindows();
		}
		else
			if ((m_CurSel==-1) || (!m_AsdMenu.IsMenuOpen()))
				hide();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to set the status of the menu item when its associated
///  pull down menu is closed
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::OnCloseMenu()
{
	if (m_CurSel!=-1)
	{
		if (m_TopTable[m_CurSel].IsPushed)
			m_TopTable[m_CurSel].IsPushed=false;
		m_CurSel=-1;
		update(m_MenuRect);
	}		
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon the selection of an available item in the Top Level pull down menus, the 
///  corresponding action is applied.
///  Argument 1 : UINT, selected menu item number
///  Argument 2 : CObject, not used in this function
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk005, CIAsdFctRwsTlsDft075, CIAsdFctRwsWet005,
///					CIAsdFctRwsWet050, CIAsdFctRwsGen065, CIAsdFctRwsGen075,
///					CIAsdSymSmbNrm335, CIAsdSymSmbBsm158, CIAsdSymSmbBsm163,
///					CIAsdSymSmbBsm228, CIAsdSymSmbBsm233, CIAsdSymSmbBsm278,
///					CIAsdSymSmbEsm110, CIAsdSymSmbErs120					
//
//-----------------------------------------------------------------------------
void CTopLevel::OnCommand(UINT id, CObject *pDisplayWnd)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	CWeatherMngr* pWthrMngr=CAsdApp::GetApp()->GetWeatherMngr();
	CFplMngr* pFPLMngr=CAsdApp::GetApp()->GetFplMngr();

	bool rocdunit, depmode;

	switch (id)
	{
	case ID_DEFAULTLAYOUT:
		//Default, reloads the default custom set or the last loaded by the user if any
		CSetupMngr::ReloadDefaultSetup();
		CRwdView::UpdateAllViews();
		break;
	case ID_ROCDUNIT:
		//toggles the ROCD unit for all the tracks
		rocdunit=!pTrkMngr->GetRocdUnit();
		pTrkMngr->SetRocdUnit(rocdunit);
		GetSystemInfo()->Update();
		break;
	case ID_BRIGHTNESS:
		//Opens the Brigthness control window
		ShowWidget(GetColorDlg(), "BRGHT_CTRL", CWFAttrib::BRGHT_CTRL);
		break;
	case ID_FINDTRACK:	
		//Opens the Find Track window
		ShowWidget(GetFindTrackDlg(), "FIND_TRACK", CWFAttrib::FIND_TRACK);
		break;
	case ID_WEATHER:
		//Toggles the display status of the weather presentation  
		pWthrMngr->DisplayWeather(!pWthrMngr->IsWeatherOn());
		GetSystemInfo()->Update();
		CRwdView::UpdateAllStatusBar();
		break;
	case ID_SYSINFO:
		//Opens the System Information window
		ShowWidget(GetSystemInfo(),"SYS_INFO",CWFAttrib::SYS_INFO);		
		GetSystemInfo()->Update();
		break;
	case ID_STOP:
		//Stops the application
		exit(0);//delete CAsdApp::GetApp();
		break;
	case ID_CUSTOMSET:
		//Opens the custom set window
	    if(GetCustomizationSetDialog()->LoadCustomSet())
			ShowWidget(GetCustomizationSetDialog(), "CUSTOM_SET",CWFAttrib::CUSTOM_SET);
		break;
	case ID_SIGNIN:
		//Opens the Sign_in window
	        CAsdApp::GetApp()->GetUserMngr()->SignIn(this);
		break;
	case ID_SIGNOUT:
		//Opens the Sign_out window
	        CAsdApp::GetApp()->GetUserMngr()->SignOut(this);
		HideAllCustomSetWindow();
		GetSystemInfo()->Update();
		break;
	case ID_CHANGEPASSWORD:
		//Opens the changepasswd window
	        CAsdApp::GetApp()->GetUserMngr()->ChangePassword(this);
		break;
	case ID_INDEPENDENT:
		//toggles the radar window mode
	    depmode=!CRwdView::GetIsIndependent();
		CRwdView::SetIndependentMode(depmode);
		if (!CRwdView::GetIsIndependent())
		{	
			pTrkMngr->SetToggledOptionalFieldsAsMain();
			CRwdView* pView=NULL;
			pView=CRwdView::GetSecWnd1();
			if (pView)
			{
				CRwdView::UpdateAuxWndDep(pView);
			}
			pView=CRwdView::GetSecWnd2();
			if (pView)
			{
				CRwdView::UpdateAuxWndDep(pView);
			}
			CRwdView::UpdateAllViews();
		}
		CRwdView::UpdateAllStatusBar();
		break;
	case ID_BL3NM:
		//Displays the Range marker
		if ((CRangeMarkView::GetOrientation()) || (CRangeMarkView::GetRange()!=3))
		{
			CRangeMarkView::SetOrientation(false);	
			CRangeMarkView::SetRange(3);
			CRangeMark::Create();
		}
		else
		{
				CRangeMark::Create();
		}
		break;
	case ID_TR3NM:
		//Displays the Range marker
		if ((!CRangeMarkView::GetOrientation()) || (CRangeMarkView::GetRange()!=3))
		{
			CRangeMarkView::SetOrientation(true);	
			CRangeMarkView::SetRange(3);
			CRangeMark::Create();
		}
		else
		{
			CRangeMark::Create();
		}
		break;
	case ID_BL5NM:
		//Displays the Range marker
		if ((CRangeMarkView::GetOrientation()) || (CRangeMarkView::GetRange()!=5))
		{
			CRangeMarkView::SetOrientation(false);	
			CRangeMarkView::SetRange(5);
			CRangeMark::Create();
		}
		else
		{
			CRangeMark::Create();
		} 
		break;
	case ID_TR5NM:
		//Displays the Range marker
		if ((!CRangeMarkView::GetOrientation()) || (CRangeMarkView::GetRange()!=5))
		{
			CRangeMarkView::SetOrientation(true);	
			CRangeMarkView::SetRange(5);
			CRangeMark::Create();
		}
		else
		{
			CRangeMark::Create();
		}
		break;
	case ID_FULLCSULIST:
		pFPLMngr->DisplayFPLFullListWindow();
		break;
	case ID_COUPLEDCSULIST:
		pFPLMngr->DisplayFPLCoupledListWindow();
		break;
	case ID_MODESCSULIST:
		pFPLMngr->DisplayFPLModeSListWindow();
		break;
	case ID_TAGCSULIST:
		pFPLMngr->DisplayFPLTagListWindow();
		break;
	case ID_BINCSULIST:
		pFPLMngr->DisplayFPLBinListWindow();
		break;
	case ID_FONTLISTSMALL:
		pFPLMngr->SetFontForAllEntries(0);
		break;
	case ID_FONTLISTMEDIUM:
		pFPLMngr->SetFontForAllEntries(1);
		break;
	case ID_FONTLISTBIG:
		pFPLMngr->SetFontForAllEntries(2);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function computes the availability of Top Level pull down menu items. 
///  Argument : CCmdUI, menu item
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctCzs025, CIAsdSymSmbGen107, CIAsdSymSmbGen112,
///					CIAsdSymSmbNrm208, CIAsdSymSmbNrm215, CIAsdSymSmbNrm273,
///					CIAsdSymSmbNrm335, CIAsdSymSmbBsm158, CIAsdSymSmbBsm163
//
//-----------------------------------------------------------------------------
void CTopLevel::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	CWeatherMngr* pWthrMngr=CAsdApp::GetApp()->GetWeatherMngr();
	CFplMngr* pFPLMngr=CAsdApp::GetApp()->GetFplMngr();

	UINT id=pCmdUI->GetID();
	switch (id)
	{
	case ID_DEFAULTLAYOUT:
		pCmdUI->Enable(true);
		break;
	case ID_BRIGHTNESS:
      		pCmdUI->Enable(true);
		break;
	case ID_ROCDUNIT:
		pCmdUI->SetCheck(!CAsdApp::GetApp()->GetTrackMngr()->GetRocdUnit());
		break;
	case ID_INDEPENDENT:
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(CRwdView::GetIsIndependent());
		break;
	case ID_SIGNIN:
		pCmdUI->Enable(true);
		break;
	case ID_SIGNOUT:
		pCmdUI->Enable(CAsdApp::GetApp()->GetUserMngr()->IsSignedIn());
		break;
	case ID_CHANGEPASSWORD:
		pCmdUI->Enable(CAsdApp::GetApp()->GetUserMngr()->IsSignedIn());
		break;
	case ID_CUSTOMSET:
		pCmdUI->Enable(CAsdApp::GetApp()->GetUserMngr()->IsSignedIn());
		break;
	case ID_WEATHER:
		pCmdUI->Enable(!pWthrMngr->IsWeatherFailed());
		pCmdUI->SetCheck(pWthrMngr->IsWeatherOn());
		break;
	case ID_SYSINFO:
	    pCmdUI->Enable(true);
//      	pCmdUI->Enable(CAsdApp::GetApp()->WindowEligibility(CWFAttrib::SYS_INFO));
		break;
	case ID_STOP:
	    pCmdUI->Enable(true);
		break;
	case ID_MAINRADAR:
	    pCmdUI->Enable(CAsdApp::GetApp()->IsMainCollapsible());				
		break;
	case ID_FINDTRACK:	
	    pCmdUI->Enable(true);
		break;
	case ID_FULLCSULIST:
		pCmdUI->Enable(true);
		break;
	case ID_COUPLEDCSULIST:
		pCmdUI->Enable(true);
		break;
	case ID_MODESCSULIST:
		pCmdUI->Enable(true);
		break;
	case ID_TAGCSULIST:
		pCmdUI->Enable(true);
		break;
	case ID_BINCSULIST:
		pCmdUI->Enable(true);
		break;
	case ID_FONTLISTSMALL:
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(pFPLMngr->GetListsFontSize()==0);
		break;
	case ID_FONTLISTMEDIUM:
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(pFPLMngr->GetListsFontSize()==1);
		break;
	case ID_FONTLISTBIG:
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(pFPLMngr->GetListsFontSize()==2);
		break;

	default:
		break;
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function loads the TopLevel bar settings from a custom set, or saves the 
///  current settings in the custom set. 
///  Argument 1 : CIniFile*, custom set file
///  Argument 2 : QString, Section name impacted in the custom set file
///  Argument 3 : bool, whether the custom set is read or saved 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsGen030
//
//-----------------------------------------------------------------------------
void CTopLevel::Setup(CIniFile* pFile,QString Section,bool Save)
{
	CTrackMngr* pTrkMngr=CAsdApp::GetApp()->GetTrackMngr();
	CWeatherMngr* pWthrMngr=CAsdApp::GetApp()->GetWeatherMngr();
	CFplMngr* pFPLMngr=CAsdApp::GetApp()->GetFplMngr();

	int topnb=-1;
	for (int i=0;i<m_sWndTable.GetSize();i++)
	{
		if (m_sWndTable[i]==this)
			topnb=i;
	}
	if (topnb==-1)
	{
#ifdef ENABLE_QDEBUG
		qDebug("problem error topnb");
#endif
	}
	else
	{
		topnb++;
		QString Prefix=QString::number(topnb,10);
		Prefix.prepend("TLM_");
		Prefix.append("_");
		QPoint windowpos;
		int screennb,X,Y,visible;
		QString AllKey;
		int depmode, rocdunit, isweather;
		//The current settings are saved in the custom set
		if (Save)
		{
			//Lock Status for the Top Level Menu bar (check box) 
			AllKey.sprintf("%d", m_AutoHide);
			pFile->WriteIniProfileString(Section,Prefix+"LOCK",AllKey);
			//Time window settings
			CTimeWnd::Setup(topnb, AllKey, Save);
			pFile->WriteIniProfileString(Section,Prefix+"TIME_WINDOW",AllKey);
			//The following attributes are only saved once
			if (topnb==1)
			{
				//Brightness Control window visibility, screen and position
				windowpos=GetColorDlg()->pos();
				screennb=CAsdApp::MapToScreen(this,windowpos);
				AllKey.sprintf("%d,%d,%d,%d",GetColorDlg()->isShown(),screennb,windowpos.x(),windowpos.y());
				pFile->WriteIniProfileString(Section,"BRIGHTNESS_WD",AllKey);
				//System Information window visibility, screen and position
				windowpos=GetSystemInfo()->pos();
				screennb=CAsdApp::MapToScreen(this,windowpos);
				AllKey.sprintf("%d,%d,%d,%d",GetSystemInfo()->isShown(),screennb,windowpos.x(),windowpos.y());
				pFile->WriteIniProfileString(Section,"SYSINFO_WD",AllKey);
				//TLM settings: Independent, ROC/D unit, Weather Display
				AllKey.sprintf("%d,%d,%d", CRwdView::GetIsIndependent(), pTrkMngr->GetRocdUnit(), pWthrMngr->IsWeatherOn());
				pFile->WriteIniProfileString(Section,"TLM_SETTINGS",AllKey);
				//FPL Lists visibility, screen and position
				//Full List
				windowpos=pFPLMngr->GetFullListDlg()->pos();
				screennb=CAsdApp::MapToScreen(this,windowpos);
				AllKey.sprintf("%d,%d,%d,%d",pFPLMngr->GetFullListDlg()->isShown(),screennb,windowpos.x(),windowpos.y());
				pFile->WriteIniProfileString(Section,"FULL_LIST",AllKey);
				//Coupled List
				windowpos=pFPLMngr->GetCoupledListDlg()->pos();
				screennb=CAsdApp::MapToScreen(this,windowpos);
				AllKey.sprintf("%d,%d,%d,%d",pFPLMngr->GetCoupledListDlg()->isShown(),screennb,windowpos.x(),windowpos.y());
				pFile->WriteIniProfileString(Section,"COUPLED_LIST",AllKey);
				//ModeS Tagged List
				windowpos=pFPLMngr->GetModeSListDlg()->pos();
				screennb=CAsdApp::MapToScreen(this,windowpos);
				AllKey.sprintf("%d,%d,%d,%d",pFPLMngr->GetModeSListDlg()->isShown(),screennb,windowpos.x(),windowpos.y());
				pFile->WriteIniProfileString(Section,"MODES_LIST",AllKey);
				//Tagged List
				windowpos=pFPLMngr->GetTaggedListDlg()->pos();
				screennb=CAsdApp::MapToScreen(this,windowpos);
				AllKey.sprintf("%d,%d,%d,%d",pFPLMngr->GetTaggedListDlg()->isShown(),screennb,windowpos.x(),windowpos.y());
				pFile->WriteIniProfileString(Section,"TAG_LIST",AllKey);
				//BIN List
				windowpos=pFPLMngr->GetBinListDlg()->pos();
				screennb=CAsdApp::MapToScreen(this,windowpos);
				AllKey.sprintf("%d,%d,%d,%d",pFPLMngr->GetBinListDlg()->isShown(),screennb,windowpos.x(),windowpos.y());
				pFile->WriteIniProfileString(Section,"BIN_LIST",AllKey);
				//FPL List Font Size
				AllKey.sprintf("%d", pFPLMngr->GetListsFontSize());
				pFile->WriteIniProfileString(Section,"FONT_SIZE_LIST",AllKey);

			}
	  
		}
		else
		{
			//The settings are loaded and applied
			//Time window settings
			if (pFile->GetIniProfileString(Section,Prefix+"TIME_WINDOW",AllKey))
				CTimeWnd::Setup(topnb,AllKey,Save);
			//Lock Status for the Top Level Menu bar (check box)
			if (pFile->GetIniProfileString(Section,Prefix+"LOCK",AllKey))
			{
				sscanf(AllKey,"%d",&visible);
				SetAutoHide(visible);
				m_Btn.setChecked(visible);
				CWdwMngr::OrderWindows();
			}
			//The following attributes are only read once
			if (topnb==1)
			{
				//Brightness Control window visibility, screen and position
				if (pFile->GetIniProfileString(Section,"BRIGHTNESS_WD",AllKey))
				{
					sscanf(AllKey,"%d,%d,%d,%d",&visible,&screennb,&X,&Y);	      
					windowpos=QPoint(X,Y);
					m_qpColorDefPos=windowpos;
					CAsdApp::MapFromScreen(screennb,windowpos);
					GetColorDlg()->Init();
					m_psColor->move(windowpos);
					if (visible)
					{
						if (!m_psColor->isVisible())
							CWdwMngr::ShowCustomSetWindow(m_psColor, CWFAttrib::BRGHT_CTRL, false);
					}
					else
						if (m_psColor->isVisible())
							CWdwMngr::HideWindow(m_psColor);
				}
				else
				{
					CAsdApp::GetApp()->WriteLogMsg("DefaultCustomSet Brightness Position section in Dataset is Empty.",LogError);
				}
				
				//Full FPL List
				if (pFile->GetIniProfileString(Section,"FULL_LIST",AllKey))
				{
					sscanf(AllKey,"%d,%d,%d,%d",&visible,&screennb,&X,&Y);	      
					windowpos=QPoint(X,Y);
					CAsdApp::MapFromScreen(screennb,windowpos);
					pFPLMngr->GetFullListDlg()->move(windowpos);
					if (visible)
					{
						if (!pFPLMngr->GetFullListDlg()->isVisible())
							pFPLMngr->DisplayFPLFullListWindow();
					}
					else
						if (pFPLMngr->GetFullListDlg()->isVisible())
							pFPLMngr->RemoveFPLFullListWindow();
				}
				else
				{
					CAsdApp::GetApp()->WriteLogMsg("DefaultCustomSet Full FPL List Position section in Dataset is Empty.",LogError);
				}

				//Coupled FPL List
				if (pFile->GetIniProfileString(Section,"COUPLED_LIST",AllKey))
				{
					sscanf(AllKey,"%d,%d,%d,%d",&visible,&screennb,&X,&Y);	      
					windowpos=QPoint(X,Y);
					CAsdApp::MapFromScreen(screennb,windowpos);
					pFPLMngr->GetCoupledListDlg()->move(windowpos);
					if (visible)
					{
						if (!pFPLMngr->GetCoupledListDlg()->isVisible())
							pFPLMngr->DisplayFPLCoupledListWindow();
					}
					else
						if (pFPLMngr->GetCoupledListDlg()->isVisible())
							pFPLMngr->RemoveFPLCoupledListWindow();
				}
				else
				{
					CAsdApp::GetApp()->WriteLogMsg("DefaultCustomSet Coupled FPL List Position section in Dataset is Empty.",LogError);
				}

				//ModeS Tagged FPL List
				if (pFile->GetIniProfileString(Section,"MODES_LIST",AllKey))
				{
					sscanf(AllKey,"%d,%d,%d,%d",&visible,&screennb,&X,&Y);	      
					windowpos=QPoint(X,Y);
					CAsdApp::MapFromScreen(screennb,windowpos);
					pFPLMngr->GetModeSListDlg()->move(windowpos);
					if (visible)
					{
						if (!pFPLMngr->GetModeSListDlg()->isVisible())
							pFPLMngr->DisplayFPLModeSListWindow();
					}
					else
						if (pFPLMngr->GetModeSListDlg()->isVisible())
							pFPLMngr->RemoveFPLModeSListWindow();
				}
				else
				{
					CAsdApp::GetApp()->WriteLogMsg("DefaultCustomSet Mode S Tagged FPL List Position section in Dataset is Empty.",LogError);
				}

				//Tagged FPL List
				if (pFile->GetIniProfileString(Section,"TAG_LIST",AllKey))
				{
					sscanf(AllKey,"%d,%d,%d,%d",&visible,&screennb,&X,&Y);	      
					windowpos=QPoint(X,Y);
					CAsdApp::MapFromScreen(screennb,windowpos);
					pFPLMngr->GetTaggedListDlg()->move(windowpos);
					if (visible)
					{
						if (!pFPLMngr->GetTaggedListDlg()->isVisible())
							pFPLMngr->DisplayFPLTagListWindow();
					}
					else
						if (pFPLMngr->GetTaggedListDlg()->isVisible())
							pFPLMngr->RemoveFPLTagListWindow();
				}
				else
				{
					CAsdApp::GetApp()->WriteLogMsg("DefaultCustomSet Tagged FPL List Position section in Dataset is Empty.",LogError);
				}

				//Bin FPL List
				if (pFile->GetIniProfileString(Section,"BIN_LIST",AllKey))
				{
					sscanf(AllKey,"%d,%d,%d,%d",&visible,&screennb,&X,&Y);	      
					windowpos=QPoint(X,Y);
					CAsdApp::MapFromScreen(screennb,windowpos);
					pFPLMngr->GetBinListDlg()->move(windowpos);
					if (visible)
					{
						if (!pFPLMngr->GetBinListDlg()->isVisible())
							pFPLMngr->DisplayFPLBinListWindow();
					}
					else
						if (pFPLMngr->GetBinListDlg()->isVisible())
							pFPLMngr->RemoveFPLBinListWindow();
				}
				else
				{
					CAsdApp::GetApp()->WriteLogMsg("DefaultCustomSet Bin FPL List Position section in Dataset is Empty.",LogError);
				}

				//FPL List Font Size
				if (pFile->GetIniProfileString(Section,"FONT_SIZE_LIST",AllKey))
				{
					sscanf(AllKey,"%d",&screennb);
					pFPLMngr->SetFontForAllEntries(screennb);
				}
				else
				{
					CAsdApp::GetApp()->WriteLogMsg("DefaultCustomSet FONT_SIZE_LIST section in Dataset is Empty.",LogError);
				}


				//System Information window visibility, screen and position
				if (pFile->GetIniProfileString(Section,"SYSINFO_WD",AllKey))
				{
					sscanf(AllKey,"%d,%d,%d,%d",&visible,&screennb,&X,&Y);	      
					if (visible)
					{
						windowpos=QPoint(X,Y);
						CAsdApp::MapFromScreen(screennb,windowpos);
						GetSystemInfo()->move(windowpos);
						if (!m_psSystemInfo->isVisible())
						{
							CWdwMngr::ShowCustomSetWindow(m_psSystemInfo,CWFAttrib::SYS_INFO, false);
							m_psSystemInfo->Update();
						}
					}
					else
						if (GetSystemInfo()->isVisible())
							CWdwMngr::HideWindow(m_psSystemInfo);
				}
				else
				{
					CAsdApp::GetApp()->WriteLogMsg("DefaultCustomSet Sys Info Position section in Dataset is Empty.",LogError);
				}

				//TLM settings: Independent, GRL auto display, ROC/D unit, Weather Display
				if (pFile->GetIniProfileString(Section,"TLM_SETTINGS",AllKey))
				{
					sscanf(AllKey,"%d,%d,%d", &depmode, &rocdunit, &isweather);
					CRwdView::SetIndependentMode(depmode);
					pTrkMngr->SetRocdUnit(rocdunit);
					pWthrMngr->DisplayWeather(isweather);
				}
			}
		}
    }
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to, if necessary, update the System Mode
///  and Safety Nets indication in the Top Level menu bars and the System information
///  window. 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::UpdateSystemInfo()
{
	int i;
	for (i=0;i<m_sWndTable.GetSize();i++)
	{
		m_sWndTable[i]->ComputeAlarm();
		m_sWndTable[i]->update();
		m_sWndTable[i]->GetSystemInfo()->Update();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function displays the Custom set window at the last position in the screen
///  or at an offline defined position, with its content updated for the
///  signed-in user and with the offline defined properties. 
///  Argument : int, screen number where the window must be displayed
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::DisplayCustomSetWindow(int screenNb)
{
	CustomizationSetDialog* CustomSetDlg=m_sWndTable[screenNb]->GetCustomizationSetDialog();  
	if (m_sWndTable.GetSize()>0)
    {
		if(CustomSetDlg->LoadCustomSet())
		{
			int widgetScrNb=CAsdApp::GetApp()->desktop()->screenNumber(CustomSetDlg);
			if (screenNb != widgetScrNb)
			{
				CAsdApp::GetApp()->getDefTlmPosList()->SetDefaultTlmPos("CUSTOM_SET", CustomSetDlg->x(), CustomSetDlg->y(), widgetScrNb);
				CAsdApp::GetApp()->WindowMoveDefault(CustomSetDlg, "CUSTOM_SET", screenNb);
			}
			CWdwMngr::ShowCustomSetWindow(CustomSetDlg,CWFAttrib::CUSTOM_SET, false);
		}
    }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  If opened, this function closes the custom set window and the Save As window. 
///   
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTopLevel::HideAllCustomSetWindow()
{
	if (m_psCustomDlg)
	{
		m_psCustomDlg->CloseSaveAsWnd();
		CWdwMngr::HideWindow(m_psCustomDlg);
	}
}

//--------------------------------< THE END >----------------------------------

