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
//  FILENAME: Ades.ui.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------/
// 
//  DESCRIPTION 
//    This file contains the description of all interactions with the ADES popup
//   window
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
#include "FlightPlan.h"
#include "View.h"
#include "Track.h"


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CAdesDlg
/// This class is used to define the content of the pop-up and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the list box, this function fills the member parameter
///  m_Ades with the selected item label. The pop-up is then automatically closed.
///  Argument : the selected list box item
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicDes005
//
//-----------------------------------------------------------------------------
void CAdesDlg::pListAdesClicked( QListBoxItem * item )
{
	//If item is not valid (==0) nothing is done
	//in the other case the selected value is retrieved, the autoclose timer is 
	//killed and the pop-up is closed.
    if (item)
    {
		m_Ades = item->text();
		killTimer(m_TimerId);
		m_TimerId=0;
		accept();
    }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pAlt1 push button, this function fills the member
///  parameter m_Ades with the button label. The pop-up is then automatically closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicDes005
//
//-----------------------------------------------------------------------------
void CAdesDlg::pAlt1Clicked()
{
	//the label of the push button is retrieved, the autoclose timer is 
	//killed and the pop-up is closed.
	m_Ades = m_Alt1;
	killTimer(m_TimerId);
    m_TimerId=0;
	accept();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pAlt2 push button, this function fills the member
///  parameter m_Ades with the button label. The pop-up is then automatically closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicDes005
//
//-----------------------------------------------------------------------------
void CAdesDlg::pAlt2Clicked()
{
	m_Ades = m_Alt2;
	killTimer(m_TimerId);
    m_TimerId=0;
	accept();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon invocation of the pop-up, the needed information about the track are 
///  retrieved and used to initialize the content and presentation of the
///  window widgets.
///  Argument 1 : The Identifier of the track for which the pop-up has been invoked.
///  Argument 2 : The first Alternate Airport ICAO ident of the track for which the
///  pop-up has been invoked, if available in its flight plan.
///  Argument 3 : The second Alternate Airport ICAO ident of the track for which the
///  pop-up has been invoked, if available in its flight plan.
///  Argument 4 : The destination airport of the track for which the pop-up has been
///  invoked.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAdesDlg::Init( QString Arcid, QString Altrnt1, QString Altrnt2, QString ades)
{
    resize(98, 264);
    setMouseTracking(true);
    m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
	int i;

	//The need to move the window according to the display of the title bar is
	//identified
	QWidget* w = CAsdApp::GetApp()->GetTrackMngr()->m_pAdesDialog;
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(w, CWFAttrib::POPUP);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();

	//Initialization of the pop-up widgets with the transmitted arguments   
    pArcid->setText(Arcid);
	this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));

	//The alternate airports push buttons are only available if associated to 
	//an airport
	m_Alt1 = Altrnt1;
	m_Alt2 = Altrnt2;
	if (m_Alt1 != "")
	{
		pAlt1->setEnabled(true);
		pAlt1->setText(m_Alt1);
	}
	else
	{
		pAlt1->setEnabled(false);
		pAlt1->setText("Alt1");
	}

    if (m_Alt2 != "")
	{
		if (m_Alt1 == "")
		{
			pAlt1->setEnabled(true);
			pAlt1->setText(m_Alt1);
			pAlt2->setEnabled(false);
			pAlt2->setText("Alt2");
		}
		else
		{
			pAlt2->setEnabled(true);
			pAlt2->setText(m_Alt2);
		}
	}
	else
	{
		pAlt2->setEnabled(false);
		pAlt2->setText("Alt2");
	}

	//The List box is filled with the offline defined list of airports
  
	QValueList<QString>::iterator it;
	CAdesList AdesList = CAsdApp::GetApp()->GetAdesList();

    
	//The List box is filled with the offline defined list of ADES
	pListAdes->clear();
    for ( it = AdesList.getAdesList()->begin(); it != AdesList.getAdesList()->end(); ++it )
    {
		QString ades;
		// CDS BREAK NGT 2006/05/22 Use of ptr->fld access cannot be used in this 
		//								case the iterator is of type QValueList 
		ades = *it;
		pListAdes->insertItem(ades);
    }

	//If an alternate airport is available, the pop-up opens so that the mouse
	//pointer is over the pAlt1 push button.
	//If the track has no alternate airport:
	//If the track is associated to an ADES and this ADES is within the list
	//the corresponding item is selected, the list is scrolled so that it is
	//visible and the pop-up is displayed so that
	//the mouse pointer is over this item. In the other case, the pop-up opens 
	//so that the mouse pointer is over the first item of the list.
    if (ades!="")
    {
		QListBoxItem* pListSelected = pListAdes->findItem(ades);
		if (pListSelected)
		{
			pListAdes->setSelected(pListSelected, true);
		    pListAdes->setTopItem((pListAdes->index(pListSelected))-4);
		}

    }
    QListBoxItem* pItem =  pListAdes->selectedItem ();

	if (m_Alt1 != "")
	{
		int dx = (pAlt1->width()/2) + pAlt1->x();
		int dy = (pAlt1->height()/2) + pAlt1->y();
		int newx = x() - dx;
		int newy = y() - dy - TitleOffset;
		QPoint pnt(newx, newy);
		move(pnt);
	}
	else
	{
		if (!pItem)
		{
			i = pListAdes->topItem () ;
			pItem =  pListAdes->item(i);
		}
		
		if (pItem)
		{
			QRect rect = pListAdes->itemRect ( pItem );
			int dx = (rect.width()) / 2 + pListAdes->x()+rect.x();
			int dy = (rect.height()) / 2 + pListAdes->y()+rect.y();
			int newx = x() - dx;
			int newy = y() - dy - TitleOffset;
			QPoint pnt(newx, newy);
			move(pnt);
		}
		else
		{
			int dx = pListAdes->x();
			int dy = pListAdes->y();
			int newx = x() - dx;
			int newy = y() - dy - TitleOffset;
			QPoint pnt(newx, newy);
			move(pnt);
		}
	}
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
void CAdesDlg::mouseMoveEvent( QMouseEvent * )
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
void CAdesDlg::timerEvent( QTimerEvent * e )
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
void CAdesDlg::mousePressEvent( QMouseEvent * e )
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

