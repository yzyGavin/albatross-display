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
//  FILENAME: TssrOther.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CTssrOtherDlg.
//		This file contains all the actions and interactions linked to the Modify
//		TSSR pop-up window.
//				
// 
//  COMMENTS
//		Refer to the file TssrOther.ui to check the connections between the 
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

#include "WfAttrib.h"
#include "DataMngr.h"
#include "WdwMngr.h"
#include "AsdApp.h"
#include "View.h"
#include "Track.h"



//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CTssrOtherDlg
/// This class is used to define the content of the pop-up and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the OK push button, this function fills the member parameter
///  m_Arcid with the text entered in the edit text field. The pop-up is then automatically closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTssrOtherDlg::pOKPressed()
{
	//the autoclose timer is killed and the pop-up is closed.
    m_Arcid=pArcid->text();
	killTimer(m_TimerId);
	m_TimerId=0;
	accept();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the UNTAG push button, this function fills the member parameter
///  m_Untag with the value true. The pop-up is then automatically closed, any input
///  is discarded.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTssrOtherDlg::pUntagPressed()
{
	m_Untag=true;
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
///  Argument 1 : QString, the TSSR of the track for which the pop-up has been invoked.
///  Argument 2 : QString, the identifier of the track for which the pop-up has been invoked.
/// 
///  RETURNS: 
///  int, The value returned by a modal dialog: QDialog::Accepted or QDialog::Rejected.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CTssrOtherDlg::DoModal( QString Tssr,QString Arcid)
{
	//The need to move the window according to the display of the title bar is
	//identified
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(CAsdApp::GetApp()->GetTrackMngr()->m_pTssrOtherDialog, CWFAttrib::POPUP);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();

	m_Untag=false;
	m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
	setMouseTracking(true);
	this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
	//If the track is already tagged, the edit text field is initialized
	//with the identifier of the track in reverse video
	if (!Arcid.length())
	{
		pArcid->clear();
		pUntag->setDisabled(true);
		pTssr->setText(Tssr);
		pOK->setDisabled(true);
	}
	else
	{
		pArcid->clear();
		Arcid=Arcid.stripWhiteSpace();
		qDebug("***************** *%s*",Arcid.ascii());
		qDebug("************** *%s*",pArcid->text().ascii());
		pUntag->setDisabled(false);
		pArcid->setText(Arcid);
		qDebug("************** *%s*",pArcid->text().ascii());
		pArcid->selectAll();
		pTssr->setText(Arcid);
		pOK->setDisabled(false);
	}
	//Positionning of the window
    QRect rect1=pArcid->geometry();
	QPoint pnt(rect1.center().x(),rect1.top()+10+TitleOffset);
	pnt=2*pos()-geometry().topLeft()-pnt;
	move(pnt);
	int res=exec();
	return res;
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
void CTssrOtherDlg::closeEvent( QCloseEvent * )
{
  if (m_TimerId)
    killTimer(m_TimerId);
  reject();
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
void CTssrOtherDlg::mouseMoveEvent( QMouseEvent * )
{
	if (m_TimerId)
		killTimer(m_TimerId);
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
void CTssrOtherDlg::timerEvent( QTimerEvent * e )
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
void CTssrOtherDlg::mousePressEvent( QMouseEvent * e )
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
///  This function is called at every input of the user in the edit 
///  text field, to check if the input is valid and computes the availability 
///  of the OK push button. 
///  Argument : QString, the new content of the edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CTssrOtherDlg::ArcIdChanged( const QString & strInput )
{
	//Set the text with the naming conventions.
	QString strUpper(strInput.upper());
	bool bBadChar=false;

	//Only letters and numbers are allowed
    for (int i=0; i<((int)strUpper.length()); i++)
    {
		QChar c=strUpper[i];
		bool bMaj=(c>='A') && (c<='Z');
		bool bDigit=(c>='0') && (c<='9');

		if (!(bMaj||bDigit))
			bBadChar=true;
    }	
    
	//if the last input is not valid it is canceled
	if(bBadChar)
	{
		int iCurPos=pArcid->cursorPosition()-1;
		pArcid->setText (m_Arcid);
	
		if(iCurPos>=0&&iCurPos<(int)pArcid->text().length())
			pArcid->setCursorPosition(iCurPos);
	}
	else
	{
		int iCurPos=pArcid->cursorPosition();

		m_Arcid=strUpper;
		pArcid->setText (m_Arcid);

		if(iCurPos>=0&&iCurPos<(int)pArcid->text().length())
			pArcid->setCursorPosition(iCurPos);
	}
	
    //Sets the availability of the OK push button
    if (pArcid->text().length())
      pOK->setDisabled(false);
    else
      pOK->setDisabled(true);	
    
}

//--------------------------------< THE END >----------------------------------

