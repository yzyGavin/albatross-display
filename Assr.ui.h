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
//  FILENAME: Assr.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the interactions of the ASSR popup window
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

#include "WfAttrib.h"
#include "WdwMngr.h"
#include "AsdApp.h"
#include "View.h"
#include "Track.h"


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CAssr
/// This class is used to define the content of the pop-up and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton0, this function calls the method ComputeChar
///  with the value 0. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton0Pressed()
{
	ComputeChar('0');
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton1, this function calls the method ComputeChar
///  with the value 1. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton1Pressed()
{
	ComputeChar('1');
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton2, this function calls the method ComputeChar
///  with the value 2. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton2Pressed()
{
	ComputeChar('2');
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton3, this function calls the method ComputeChar
///  with the value 3. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton3Pressed()
{
	ComputeChar('3');
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton4, this function calls the method ComputeChar
///  with the value 4. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton4Pressed()
{
	ComputeChar('4');
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton5, this function calls the method ComputeChar
///  with the value 5. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton5Pressed()
{
	ComputeChar('5');
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton6, this function calls the method ComputeChar
///  with the value 6. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton6Pressed()
{
	ComputeChar('6');
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton7, this function calls the method ComputeChar
///  with the value 7. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton7Pressed()
{
	ComputeChar('7');
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton8, this function calls the method ComputeChar
///  with the value 8. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton8Pressed()
{
	ComputeChar('8');
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pushButton9, this function calls the method ComputeChar
///  with the value 9. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::PushButton9Pressed()
{
	ComputeChar('9');
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  If the current ASSR in the edit text field is less than 4 digits, the method
///  Assr_textChanged is called, in the other case the edit text field is emptied
///  before calling the method Assr_textChanged for the character put in argument.
///  Argument : the character corresponding to the label of the selected push button.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::ComputeChar( char btn )
{
	QString assr=Assr->text();
	if (assr.length()>=4)
		assr="";
	assr+=btn;
	AssrTextChanged(assr);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function checks whether the added character thanks to the push buttons
///  or the keyboard keys (linked to the event textChanged) is valid or not.   
///  Argument : the current text in the edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::AssrTextChanged( const QString & text)
{
      
	QString assr=text;
	QString strNew;
	//An ASSR is an octal field only digits between 0 and 7 are accepted
	for (int i=0; i<((int)assr.length()); i++)
    {
		QChar c=assr[i];
		bool bDigit=(c>='0') && (c<='7');
		if (bDigit)
			strNew+=assr[i];
    }
	//If the length of 4 is reached the input is validated, the autoclose timer
	//is killed and the pop-up is automatically closed. 
	if (strNew.length()==4)
    {
		m_Assr=strNew;
		killTimer(m_TimerId);
		m_TimerId=0;
		accept();
    }
	//The recomputed string is displayed in the edit text field
	Assr->setText(strNew);
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
void CAssr::mouseMoveEvent( QMouseEvent * )
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
void CAssr::timerEvent( QTimerEvent * e )
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
void CAssr::mousePressEvent( QMouseEvent * e )
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
///  Initialization and positionning of the pop-up.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CAssr::Init()
{
	//The need to move the window according to the display of the title bar is
	//identified
	QWidget* w = CAsdApp::GetApp()->GetTrackMngr()->m_pAssrDialog;
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(w, CWFAttrib::POPUP);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();

	//The pop-up must open so that the mouse pointer is over the Assr edit text field
	QRect rect1=Assr->geometry();
	QPoint pnt=2*pos()-geometry().topLeft()-rect1.center();
	m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
	this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
	pnt.setY(pnt.y() - TitleOffset);
	move(pnt);
	m_Assr=-1;
}


//--------------------------------< THE END >----------------------------------
