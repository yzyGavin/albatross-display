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
//  FILENAME: ArcTyp.ui.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the interactions with the Aircraft type popup window
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
#include "DataMngr.h"
#include "AsdApp.h"
#include "View.h"
#include "Track.h"

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CATypDlg
/// This class is used to define the content of the pop-up and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the list box, this function fills the member parameter
///  m_ATyp with the selected item label. The pop-up is then automatically closed.
///  Argument : the selected list box item
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CATypDlg::ListAtypClicked( QListBoxItem * item )
{
	//If item is not valid (==0) nothing is done
	//in the other case the selected value is retrieved, the auto-close timer is 
	//killed and the pop-up is closed.
    if (item)
    {
		m_ATyp = item->text();
		killTimer(m_TimerId);
		m_TimerId=0;
		accept();
    }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon invocation of the pop-up, the needed information about the track are 
///  retrieved and used to initialize the content and presentation of the
///  window widgets.
///  Argument 1 : The Identifier of the track for which the pop-up has been invoked.
///  Argument 2 : The ATYP of the track for which the pop-up has been invoked if any.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CATypDlg::Init( QString Arcid, QString atyp)
{
	QValueList<CATypList::m_WakeTC>::iterator it;
	CATypList arctyp = CAsdApp::GetApp()->GetATypWtcList();
    this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));

	//The need to move the window according to the display of the title bar is
	//identified
	QWidget* w = CAsdApp::GetApp()->GetTrackMngr()->m_pATypDialog;
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(w, CWFAttrib::POPUP);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();

    setMouseTracking(true);
    m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
    
	//Initialization of the pop-up widgets with the transmitted arguments   
    pArcid->setText(Arcid);
    
	//The List box is filled with the offline defined list of ATYP
    pListATyp->clear();
    for ( it = arctyp.getATypList()->begin(); it != arctyp.getATypList()->end(); ++it )
    {
		QString aircrafttype;
		// CDS BREAK NGT 2006/05/22 Use of ptr->fld access cannot be used in this 
		//								case the iterator is of type QValueList 
		aircrafttype = (*it).ATyp;
		pListATyp->insertItem(aircrafttype);
    }

	//If the track is associated to an ATYP and this ATYP is within the list
	//the corresponding item is selected, the list is scrolled so that it is
	//visible and the pop-up is displayed so that
	//the mouse pointer is over this item. In the other case, the pop-up opens 
	//so that the mouse pointer is over the first item of the list.
    if (atyp!="")
    {
		QListBoxItem* pListSelected = pListATyp->findItem(atyp);
		if (pListSelected) 
			pListATyp->setSelected(pListSelected, true);
    }
    QListBoxItem* pItem =  pListATyp->selectedItem ();
    if (!pItem)
    {
		int i = pListATyp->topItem () ;
		pItem =  pListATyp->item(i);
	}
    if (pItem)
	{
		QRect rect = pListATyp->itemRect ( pItem );
		int dx = (rect.width()) / 2 + pListATyp->x()+rect.x();
		int dy = (rect.height()) / 2 + pListATyp->y()+rect.y();
		int newx = x() - dx;
		int newy = y() - dy - TitleOffset;
		QPoint pnt(newx, newy);
		move(pnt);
	}
	else
	{
		int dx = pListATyp->x();
		int dy = pListATyp->y();
		int newx = x() - dx;
		int newy = y() - dy - TitleOffset;
		QPoint pnt(newx, newy);
		move(pnt);
	}
	pListATyp->verticalScrollBar()->setLineStep(19);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is added to implement the auto-close behavior.
///  If the mouse pointer is moved over the pop-up, the auto-close timer is
///  killed and newly initialized.
///  Argument : The mouse move event, not used in this method.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CATypDlg::mouseMoveEvent( QMouseEvent * )
{
    if (m_TimerId)
		killTimer(m_TimerId);
	m_TimerId=0;
	m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is added to implement the auto-close behavior.
///  If the m_TimerId auto-close timer is reached, meaning the mouse pointer
///  has been out of the window for m_AutoCloseTime seconds, the window is
///  automatically closed.
///  Argument : The timer event.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CATypDlg::timerEvent( QTimerEvent * e )
{
	//If the timer event is the m_TimerId, and the mouse pointer is not over the
	//window, the timer is killed and the window is closed. All inputs are discarded.
    if (m_TimerId==e->timerId())
	{
		QPoint pnt=QCursor::pos();
		QRect rect=frameGeometry();
		if (!rect.contains(pnt))
		{
			killTimer(m_TimerId);
			m_TimerId=0;
			reject();
		}
	}
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
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CATypDlg::mousePressEvent( QMouseEvent * e )
{
	//If the pressed button is the wheel button, the timer is killed and the 
	//window is closed. All inputs are discarded.
    if (e->button()==MidButton)
	{
		killTimer(m_TimerId);
		m_TimerId=0;
		reject();
		
	}
}

//--------------------------------< THE END >----------------------------------
