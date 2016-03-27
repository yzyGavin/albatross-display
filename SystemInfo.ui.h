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
//  FILENAME: SystemInfo.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CSystemInfo.
//		This file contains one function to update the information presented in the
//		System Information window (dynamic window) and another one to close it 
//		with the Click WB action.
//				
// 
//  COMMENTS
//		Refer to the file SystemInfo.ui to check the connections between the 
//		following functions and the window widgets.
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
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include "View.h"
#include "AsdView.h"
#include "DisplayElement.h"
#include "./CommonFiles/GraphicSymbol.h"
#include "Track.h"
#include "AsdApp.h"
#include "AsdMenu.h"
#include "ColorMngr.h"
#include "Elements.h"
#include "SystemMode.h"
#include "UserMngr.h"
#include "WinHints.h"

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CSystemInfo
/// This class is used to define the content of the System Information window.
/// This window retrieves all the information needed and presents it dynamically
/// at any change.
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when the object is created. It fills the member
///  list m_ExtLineTable which allows to update all the External lines information
///  with only one loop.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CSystemInfo::init()
{

}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to update the information presented in
///	 System Information window.
///	 As the System Information window is a control window, this function must be
///	 called every time a status changes, an evalution is performed, ATCO actions...
///	 in order to have always the current and correct situation of the CWP. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctSiw005, CIAsdFctSiw015, CIAsdFctSiw016,
///					CIAsdFctSiw020, CIAsdFctSiw025, CIAsdFctSiw030, 
///					CIAsdFctSiw035, CIAsdFctSiw045, CIAsdFctSiw050, 
///					CIAsdFctSiw055, CIAsdFctSiw056, CIAsdFctSiw060,
///					CIAsdFctSiw065, CIAsdFctSiw070, CIAsdFctSiw075, 
///					CIAsdFctSiw080, CIAsdFctSiw090, CIAsdFctSiw095, 
///					CIAsdFctSiw100, CIAsdFctSiw105, CIAsdFctSiw110,
///					CIAsdFctSiw111, CIAsdFctSiw112, CIAsdFctSiw115
//
//-----------------------------------------------------------------------------
void CSystemInfo::Update()
{
	// CDS BREAK NGT 22/05/2006 Bad method length rule violated. The code has been 
	//							factorized as much as possible (init() function 
	//							and the list m_ExtLineTable are added).

	//managers initialization
	CTrackMngr* pTrkMngr= CAsdApp::GetApp()->GetTrackMngr();
	CWeatherMngr* pWeatherMngr= CAsdApp::GetApp()->GetWeatherMngr();
	CUserMngr* pUserMngr= CAsdApp::GetApp()->GetUserMngr();
	CSystemMode* pSystMode=CAsdApp::GetApp()->GetSystemMode();

	//colors initialization
	QColor UrgColor=CColorMngr::GetColor(CColorMngr::GetColorNb("URGENCY"));
	QColor NormalColor=CColorMngr::GetColor(DefWindowTextClr);
	QColor NormalBkColor=CColorMngr::GetColor(DefWindowBackClr);
	QColor RocdColor=CColorMngr::GetColor(CColorMngr::GetColorNb("ROCD_FTNM"));
	QColor WarningClr=CColorMngr::GetColor(CColorMngr::GetColorNb("WARNING"));

	QString str;

	//update currently selected radar service name and status
	pTrackService->setPaletteBackgroundColor(NormalBkColor);
	if (pTrkMngr->IsCurrentSourceAv())
		pTrackService->setPaletteForegroundColor(NormalColor);
	else
		pTrackService->setPaletteForegroundColor(UrgColor);
	pTrackService->setText(pTrkMngr->GetSourceName());

	//update current system mode
	pSystemMode->setPaletteBackgroundColor(NormalBkColor);
	SystemMode curmode=FAILED;
	if (pSystMode)
	{
		pSystMode->ComputeSystemMode();
		curmode=pSystMode->GetSystemMode();
	}
	if (curmode!=NORMAL)
		pSystemMode->setPaletteForegroundColor(UrgColor);
	else
		pSystemMode->setPaletteForegroundColor(NormalColor);
	if (pSystMode)
		pSystemMode->setText(pSystMode->GetSystemModeLabel());
	else
		pSystemMode->setText("*********");
	

	//update the weather radar service status
	pWeather->setPaletteBackgroundColor(NormalBkColor);
	if (pWeatherMngr->IsWeatherFailed())
	{
		pWeather->setPaletteForegroundColor(UrgColor);
		pWeather->setText("Failed");
	}
	else
	{
		pWeather->setPaletteForegroundColor(NormalColor);
		if (pWeatherMngr->IsWeatherOn())
			pWeather->setText("On");
		else
			pWeather->setText("Off");
	}
	
	//update the Adaptation data set version
	AdaDataSetVersion->setPaletteBackgroundColor(NormalBkColor);
	AdaDataSetVersion->setPaletteForegroundColor(NormalColor);
	QString tmp=CAsdApp::GetApp()->GetDataSetDir();
	if (tmp.find("-")!=-1)
		AdaDataSetVersion->setText(tmp.section("-",-1));
	else
		AdaDataSetVersion->setText("x.x");
	
	//ASD application version update
	pASDVersion->setPaletteBackgroundColor(NormalBkColor);
	pASDVersion->setPaletteForegroundColor(NormalColor);
	pASDVersion->setText(CAsdApp::GetApp()->GetViewerVersion());

	
	//update the ROC/D Unit
	pRocd->setPaletteBackgroundColor(NormalBkColor);
	if (pTrkMngr->GetRocdUnit())
		{
		pRocd->setPaletteForegroundColor(NormalColor);
		pRocd->setText("Ft/min");
	}
	else
	{
		pRocd->setPaletteForegroundColor(RocdColor);
		pRocd->setText("Ft/Nm");
	}
	
	//update the Username
	pUsername->setPaletteBackgroundColor(NormalBkColor);
	pUsername->setPaletteForegroundColor(NormalColor);
	pUsername->setText(pUserMngr->GetUserName());

	//DB type
	pAPPACC->setPaletteBackgroundColor(NormalBkColor);
	pAPPACC->setPaletteForegroundColor(NormalColor);
	if (CAsdApp::GetApp()->IsAppPos())
		pAPPACC->setText("APP");
	else
		pAPPACC->setText("ACC");

	
	
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is added to implement the Click WB action which must close the
///  window.
///  Argument : The mouse event. Mouse events occur when a mouse button is
///  pressed or released inside a widget.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CSystemInfo::mousePressEvent( QMouseEvent * e )
{
	if (e->button()==MidButton)
	{
		CWdwMngr::HideWindow(this);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when the window is closed.
///  Argument : Close Event, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CSystemInfo::closeEvent( QCloseEvent * )
{
	//The window position is saved for further invocation
	CAsdApp::GetApp()->getDefTlmPosList()->SetDefaultTlmPos("SYS_INFO", this->x(), this->y(), CAsdApp::GetApp()->desktop()->screenNumber(this));
	CWdwMngr::HideWindow(this);
}

//--------------------------------< THE END >----------------------------------




