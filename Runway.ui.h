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
//  FILENAME: Runway.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the interactions of the runway popup window
// 
//  COMMENTS
//    
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

#include "AsdApp.h"
#include "DataMngr.h"
#include "View.h"
#include "Track.h"

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CRwyDlg
/// This class is used to define the content of the RWY pop-up and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the list box, this function fills the member parameter
///  m_Rwy with the selected item label. The pop-up is then automatically closed.
///  Argument : the selected list box item
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwyDlg::pRwyListClicked( QListBoxItem * item )
{
	if (item)
	{
		//If item is not valid (==0) nothing is done
		//in the other case the selected value is retrieved, the autoclose timer is 
		//killed and the pop-up is closed.
		m_Rwy=item->text();
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
///  Argument 1 : QString, The Identifier of the track for which the pop-up has been invoked.
///  Argument 2 : QString, The Ades of the track for which the pop-up has been invoked
///  Argument 3 : QString, The Arrival runway of the track for which the pop-up has been invoked
///  Argument 4 : QString, The parallel runway indication as furnished by the flight plan.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicArr010, CIAsdFctDfdCicArr020
//
//-----------------------------------------------------------------------------
void CRwyDlg::Init(QString Arcid, QString Ades, QString Rwy, QString ParallelRwy)
{
	int currentItem = -1;
	int selectedItem = -1;
	m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
	this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
	setMouseTracking(true);

	//The need to move the window according to the display of the title bar is
	//identified
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(CAsdApp::GetApp()->GetTrackMngr()->m_pRwyDialog, CWFAttrib::POPUP);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();

	//The pop-up is resized to fit the number of entries in the List Box
	//The list of runways according to the Ades is offline defined
	pRwyList->clear();
	QStringList strlRwyList=CDataMngr::GetRunwayList(Ades);
	int nbitem=strlRwyList.count();
	if (nbitem>9)
	{
		pRwyList->resize(86,171);
		resize(98,33+171);
	}
	else
	{
		pRwyList->resize(86,nbitem*19+5);
		resize(98,33+nbitem*19+5);
	}


	pArcid->setText(Arcid);
	QStringList::Iterator it;
	//The list box is filled in. The current runway is selected
	//if the current runway is a parallel runway, the window must open
	//so that the mouse pointer is over the other item 
	for (it=strlRwyList.begin() ; it != strlRwyList.end() ; ++it)
	{
		if (ParallelRwy.length())
		{
			// CDS BREAK NGT 2006/05/23 Use of ptr->fld access cannot be used in 
			//						this case the iterator is of type QValueList 
			if ((*it).left(2) == ParallelRwy.left(2))
			{
				if (ParallelRwy != (*it))
					currentItem=pRwyList->count();
				if (ParallelRwy == (*it))
					selectedItem=pRwyList->count();
			}
		}
		else
		{
			if (Rwy == (*it))
			{
				currentItem=pRwyList->count();
				selectedItem=pRwyList->count();
			}

		}
		pRwyList->insertItem(*it);
	}

	//positionning of the window
	QPoint pos=QCursor::pos();
	QRect Rect;
	if (currentItem != -1 && selectedItem != -1)
	{
		pRwyList->setCurrentItem(selectedItem);
		pRwyList->setSelected(selectedItem, true);
//	    pRwyList->setTopItem(currentItem-4);

		Rect = pRwyList->itemRect(pRwyList->item(currentItem));
	}
	else
	{
		if (currentItem != -1)
		{
			pRwyList->setSelected(selectedItem, true);
		    pRwyList->setTopItem(currentItem-4);

			//pRwyList->setCurrentItem(currentItem);
			Rect = pRwyList->itemRect(pRwyList->item(currentItem));
		}
		else
			Rect = pRwyList->itemRect(pRwyList->item(0));
	}

	pos=pos - pRwyList->rect().topLeft() - Rect.center();
	pos.setY(pos.y()-3*TitleOffset);
	move(pos);

	pRwyList->verticalScrollBar()->setLineStep(19);

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
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwyDlg::mouseMoveEvent( QMouseEvent * )
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
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwyDlg::timerEvent( QTimerEvent * e )
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
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CRwyDlg::mousePressEvent( QMouseEvent * e )
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
