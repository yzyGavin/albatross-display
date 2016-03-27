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
//  FILENAME: Tssr.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CTssrDlg.
//		This file contains all the actions and interactions linked to the CREATE
//		TSSR pop-up window.
//				
// 
//  COMMENTS
//		Refer to the file Tssr.ui to check the connections between the 
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

#include "AsdApp.h"
#include "WfAttrib.h"
#include "DataMngr.h"
#include "WdwMngr.h"
#include "View.h"
#include "Track.h"

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CTssrDlg
/// This class is used to define the content of the pop-up and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the list box, this function fills the member parameter
///  m_Arcid with the selected item label. The pop-up is then automatically closed.
///  The Events returnPressed and clicked are connected to this function.
///  Argument : the selected list box item
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTssrDlg::pSsrListPressed( QListBoxItem * item)
{
	//If item is not valid (==0) nothing is done
	//in the other case the selected value is retrieved, the autoclose timer is 
	//killed and the pop-up is closed.
	if (item)
	{
		m_Arcid=item->text();
		killTimer(m_TimerId);
		m_TimerId=0;
		accept();
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pOther push button the pop-up is then automatically
///  closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTssrDlg::pOtherPressed()
{
	//the autoclose timer is killed and the pop-up is closed. The m_Other parameter 
	//is set to true to indicate the TSSR other pop-up must be invoked.
	m_Other=true;
	killTimer(m_TimerId);
	m_TimerId=0;
	accept();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon invocation of the pop-up, the needed information about the track is 
///  retrieved and used to initialize the content and presentation of the
///  window widgets. The pop-up is modal.
///  Argument : QString, the TSSR of the track for which the pop-up has been invoked.
/// 
///  RETURNS: 
///  int, The value returned by a modal dialog: QDialog::Accepted or QDialog::Rejected.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CTssrDlg::DoModal( QString Tssr )
{
	//The need to move the window according to the display of the title bar is
	//identified
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(CAsdApp::GetApp()->GetTrackMngr()->m_pTssrDialog, CWFAttrib::POPUP);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();

	CArray<QString,QString&> TagTable;
	CDataMngr::GetTagList(TagTable);
	pSsrList->clear();
	//The proposed callsign tags are offline defined
	for (int i=0;i<TagTable.GetSize();i++)
		pSsrList->insertItem(TagTable[i]);
	pSsr->setText(Tssr);
	m_Other=false;
	m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
	setMouseTracking(true);
	this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
	//Positionning of the window
    QRect rect1=pSsrList->geometry();
	QPoint pnt(rect1.center().x(),rect1.top()+10+TitleOffset);
	pnt=2*pos()-geometry().topLeft()-pnt;
	move(pnt);
	
	pSsrList->verticalScrollBar()->setLineStep(19);

	int res=exec();
	return res;
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
void CTssrDlg::mouseMoveEvent( QMouseEvent * )
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
void CTssrDlg::timerEvent( QTimerEvent * e )
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
void CTssrDlg::mousePressEvent( QMouseEvent * e )
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

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is added to complete the quick search facility, a found item 
///  matching the quick search facility criteria must be displayed (the list must
///  be scrolled so that...) at the top of the list box.
///  Argument : the selected list box item.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTssrDlg::SetItemAtTop(QListBoxItem* Item)
{
	//If the item is valid, the list is scrolled so that the item "Item" is
	//displayed at the top of the visible entries.
    if (Item)
    {
        int index = pSsrList->index(Item);
        pSsrList->setTopItem(index);
    }
}

//--------------------------------< THE END >----------------------------------


