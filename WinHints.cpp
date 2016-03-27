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
//  FILENAME: WinHints.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CWin_Hints which manages the interface with the 
//		window Manager.
//		Allows to display the ASD windows (colors, border presentation, content of
//		the title bar, decoration buttons ...) as required in Design documents.
//		Allows to have the correct window behavior: collapse, move, automatic
//		repositionning...  
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
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include <qapplication.h>
#include "WinHints.h"
#include "qpainter.h"
#if (defined Q_WS_X11)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdlib.h>
#include "SKMessageBox.h"
#include "ColorMngr.h"
#endif

//-GLOBAL DATA-----------------------------------------------------------------

                       
//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------
//-----------------------------------------------------------------------------
//
/// \class CWin_Hints
/// This class is used to execute the window manager command from the ASD
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWin_Hints::CWin_Hints()
{
  m_bOccupied=false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWin_Hints::~CWin_Hints()
{

}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method calls the WindowSize method of class CWdwMngr
///		if the window is collapsed
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------.
void CWin_Hints::CollapseWindow(QWidget* w, bool bDisplayCollapse)
{
#if (defined Q_OS_LINUX)

	if (bDisplayCollapse)
		CWdwMngr::WindowSize(w);
#endif

}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method changes the border color of a window when
///		an important track is lying underneath it
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------.
void CWin_Hints::ChangeBorderColor(QWidget* w, int iCode)
{
// TO BE DONE IN A NEXT RELEASE

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the rectangle of the window w
/// 
///  RETURNS: 
///		a rectangle QRect
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------.
QRect CWin_Hints::GetXWindowGeometry(QWidget* w)
{
  QRect rGeo;
  rGeo=w->frameGeometry();
#if (defined Q_WS_X11)
  Window root;
  int X,Y;
  unsigned int dummy,W,H;
  Window *pTmpCurTab,tmproot,tmpparent1,tmpparent2;
  unsigned int tmpnbr=0;
  XQueryTree(w->x11Display(),w->winId(),&tmproot,&tmpparent1,&pTmpCurTab,&tmpnbr);
  XFree(pTmpCurTab);
  XQueryTree(w->x11Display(),tmpparent1,&tmproot,&tmpparent2,&pTmpCurTab,&tmpnbr);
  XFree(pTmpCurTab);
  XGetGeometry(w->x11Display(),tmpparent2,&root,&X,&Y,&W,&H,&dummy,&dummy);
  rGeo=QRect(X,Y,W,H);
#endif
  return rGeo;
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method computes the RGB color from offline defined color
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------.
void CWin_Hints::GetRGBColor(QString OfflineColor, QString& sred, QString& sgreen, QString& sblue)
{
#if (defined Q_WS_X11)
	int red, green, blue;

    QColor color1=CColorMngr::GetColor(CColorMngr::GetColorNb(OfflineColor));
    color1.getRgb(&red, &green, &blue);
    sred = sred.setNum(red,16);
    sgreen = sgreen.setNum(green,16);
    sblue = sblue.setNum(blue,16);
#endif
}









//--------------------------------< THE END >----------------------------------

