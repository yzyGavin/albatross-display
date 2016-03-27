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
//  FILENAME: ImportantTrack.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the management of window framing effect due to an important
//	  track
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

#include "ImportantTrack.h"
#include "WinHints.h"

#include "View.h"
#include "DisplayElement.h"
#include "RwdView.h"
#include "AsdApp.h"
#include "./CommonFiles/IniFile.h"


//-PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CImportantTrack
/// This class is used to set the border color on a window which have an 
///	important track underneath it
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
CImportantTrack::CImportantTrack()
{
  m_bInit=false;
  m_iNbWindowManaged=0;
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
CImportantTrack::~CImportantTrack()
{
  killTimer(m_iImportantTrackTimer);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets all the window categories that have to present the window
///		framing feature.
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctAccGen020 
//-----------------------------------------------------------------------------
void CImportantTrack::Init()
{  
  m_iNbWindowName=13;
  m_bInit=true;

  m_bWindowEligibilities.Add(false);
  m_strWindowNames.Add("MAIN_RDR");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("AUX_RDR");
  
  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("HGHT_FILTERS");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("SYS_INFO");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("BRGHT_CTRL");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("TRK_CTRL");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("POPUP");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("CHANGE_PASSWD");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("CUSTOM_SET");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("SAVE_AS");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("SIGN_IN");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("FIND_TRACK");

  m_bWindowEligibilities.Add(true);
  m_strWindowNames.Add("CSU_LIST");

  //The window framing effect must be updated every 500ms
  m_iImportantTrackTimer=startTimer(500);
  m_iImportantTrackTimer=startTimer(CAsdApp::GetApp()->getFramingTimer());
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method adds the window objects that shall have the border 
///		highlight behavior in a table
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctAccGen020 
//-----------------------------------------------------------------------------
void CImportantTrack::AddWindow(QString strWindowName,QWidget* w)
{
	int i=0;

	if (w!=0)
	{
		if (strWindowName=="MESSAGE_BOX")
		{
			m_qwWindows.InsertAt(m_iNbWindowManaged,w);
			m_iWindowCode.InsertAt(m_iNbWindowManaged,-1);
			m_OldWindowPos.InsertAt(m_iNbWindowManaged,QPoint());
			m_iNbWindowManaged++;
		}
		else
			while (i<m_iNbWindowName)
			{
				if (m_strWindowNames[i]==strWindowName)
				{
					if (m_bWindowEligibilities[i])
						m_qwWindows.InsertAt(m_iNbWindowManaged,w);
					else
					{
						QWidget* wNull = NULL;
						m_qwWindows.InsertAt(m_iNbWindowManaged,wNull);
					}
					m_iWindowCode.InsertAt(m_iNbWindowManaged,-1);
					m_OldWindowPos.InsertAt(m_iNbWindowManaged,QPoint());
					m_iNbWindowManaged++;
					break;
				}
				i++;
			}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method removes the window from the important track table.
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CImportantTrack::RemoveWindow(QWidget* w)
{
	int i=0;
	while (i<m_iNbWindowManaged)
	{
		if (m_qwWindows[i]==w)
		{
			m_qwWindows.RemoveAt(i);
			m_iWindowCode.RemoveAt(i);
			m_OldWindowPos.RemoveAt(i);
			m_iNbWindowManaged--;
		}
		i++;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks cyclically if a window has an important
///		track underneath. If it's the case the border of the window is highlighted
///		in a specific color.
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctAccGen020 
//-----------------------------------------------------------------------------
void CImportantTrack::timerEvent(QTimerEvent* )
{
	QWidget* w=0;
	CWin_Hints* pCWinHints=CAsdApp::GetApp()->GetWinHints();
	int iScreen,iCode,iPreviousCode;

	// loop on all windows defined in Init method
	for (int iWnd=0;iWnd<m_iNbWindowManaged;iWnd++)
	{
		w=m_qwWindows[iWnd];
		if (w)
		{
			// retrieve the screen number and the previous important code
			// Each code corresponds to a border highlight color
			iScreen=CAsdApp::desktop()->screenNumber(w);
			iPreviousCode=m_iWindowCode[iWnd];
			if (w->isVisible())
			{
				QPoint qpNew=w->geometry().topLeft();
				if (qpNew!=m_OldWindowPos[iWnd])
				{
					m_OldWindowPos[iWnd]=qpNew;
					return;
				}
				m_OldWindowPos[iWnd]=qpNew;
				QRect rGeo;
				rGeo=pCWinHints->GetXWindowGeometry(w);
				iCode=CRwdView::m_psMainWnd->GetImportantTrackUnderneath(w,rGeo,iScreen);
				// if an important Track lies underneath the window and the window has moved
				//	(previous code is then different than current code)
				// the ASD changes the border color of the window
				if (iCode!=iPreviousCode)
				{
					m_iWindowCode[iWnd]=iCode;
					pCWinHints->ChangeBorderColor(w,iCode);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method resets the important code of a window 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CImportantTrack::ResetImportantCode(QWidget* w)
{
	for (int iWnd=0;iWnd<m_iNbWindowManaged;iWnd++)
	{
		if (w==m_qwWindows[iWnd])
			m_iWindowCode[iWnd]=-1;
	}
}

//--------------------------------< THE END >----------------------------------
