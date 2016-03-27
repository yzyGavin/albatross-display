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
//  FILENAME: Wtc.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CWtcDlg.
//		This file contains all the actions and interactions linked to the WTC
//		pop-up window.
//				
// 
//  COMMENTS
//		Refer to the file Wtc.ui to check the connections between the 
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

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CWtcDlg
/// This class is used to define the content of the pop-up and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the list box, this function fills the member parameter
///  m_Wtc with the selected item label. The pop-up is then automatically closed.
///  Argument : the selected list box item
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CWtcDlg::pListWtcClicked( QListBoxItem * item)
{
	//If item is not valid (==0) nothing is done
	//in the other case the selected value is retrieved, the autoclose timer is 
	//killed and the pop-up is closed.
	if (item)
    {
		m_Wtc = item->text();
		killTimer(m_TimerId);
		m_TimerId=0;
		accept();
    }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon invocation of the pop-up, the needed information about the track is 
///  retrieved and used to initialize the content and presentation of the
///  window widgets.
///  Argument : QString, the identifier of the track for which the pop-up has been invoked.
///  Argument : QString, the WTC of the track for which the pop-up has been invoked.
///  Argument : QString, the ATYP of the track for which the pop-up has been invoked.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicAcw030
//
//-----------------------------------------------------------------------------
void CWtcDlg::Init( QString Arcid, QString wtc, QString Atyp)
{
	//The need to move the window according to the display of the title bar is
	//identified
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(CAsdApp::GetApp()->GetTrackMngr()->m_pWtcDialog, CWFAttrib::POPUP);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();
	
    setMouseTracking(true);
    m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
    this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
    pListWtc->clear();
    pArcid->setText(Arcid);	    

	//The possible WTC offline defined to be associated to the track ATYP are 
	//retrieved
	// CDS BREAK NGT 2006/05/23 Use of ptr->fld access cannot be used in 
	//						this case the iterator is of type QValueList 
	QValueList<CATypList::m_WakeTC>::iterator it;
	CATypList arctyp = CAsdApp::GetApp()->GetATypWtcList();
	int dy=0;
    for ( it = arctyp.getATypList()->begin(); it != arctyp.getATypList()->end(); ++it )
    {
		if ((*it).ATyp == Atyp)
		{
			// CDS BREAK NGT 2006/05/23 Use of ptr->fld access cannot be used in 
			//						this case the iterator is of type QValueList 
			QValueList<QString>::iterator it2;
			for ( it2 = (*it).Wtc.begin(); it2 != (*it).Wtc.end(); ++it2 )
				pListWtc->insertItem((*it2));
			break;
		}
    }

    //If no WTC is found or if the track has no associated ATYP, all the possibilities
	//are added to the list
	if (pListWtc->count() ==0)
	{
		pListWtc->insertItem("L");
		pListWtc->insertItem("M");
		pListWtc->insertItem("H");
	}

	//The current WTC, if any should be selected and the window should 
	//open so that the mouse pointer is over it.
    if (wtc!="")
    {
		QListBoxItem* pListSelected = pListWtc->findItem(wtc);
		if (pListSelected) 
			pListWtc->setSelected(pListSelected, true);
    }

    QListBoxItem* pItem =  pListWtc->selectedItem ();
    if (!pItem)
    {
		int i = pListWtc->currentItem () ;
		pItem =  pListWtc->item(i);
    }
    if (!pItem)
    {
		int i = pListWtc->topItem () ;
		pItem =  pListWtc->item(i);
	}

    if (pItem)
	{
		int index = pListWtc->index(pItem);
		QRect rect = pListWtc->itemRect ( pItem );
		int dx = (rect.right() - rect.left()) / 2 + pListWtc->x();
		if (index !=0)
			dy = (index)*(rect.bottom() - rect.top()) + (rect.bottom() - rect.top())/2 + pListWtc->y();
		else
			dy = (rect.bottom() - rect.top()) / 2 + pListWtc->y();
		int newx = x() - dx;
		int newy = y() - dy - TitleOffset;
		QPoint pnt(newx, newy);
		move(pnt);
	}
	else
	{
		int dx = pListWtc->x();
		int dy = pListWtc->y();
		int newx = x() - dx;
		int newy = y() - dy - TitleOffset;
		QPoint pnt(newx, newy);
		move(pnt);
	}
	pListWtc->verticalScrollBar()->setLineStep(19);
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
void CWtcDlg::mouseMoveEvent( QMouseEvent * )
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
void CWtcDlg::timerEvent( QTimerEvent * e )
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
void CWtcDlg::mousePressEvent( QMouseEvent * e )
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
