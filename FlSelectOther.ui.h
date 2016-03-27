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
//  FILENAME: FlSelectOther.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the interactions with the CFL/RFL/REP other popup window
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
#include "DataMngr.h"
#include "View.h"
#include "Track.h"


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CFlSelectOther
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
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton0Pressed()
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
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton1Pressed()
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
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton2Pressed()
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
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton3Pressed()
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
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton4Pressed()
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
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton5Pressed()
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
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton6Pressed()
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
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton7Pressed()
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
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton8Pressed()
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
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::PushButton9Pressed()
{
	ComputeChar('9');
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  The char put in argument is added to the current CFL in the edit text field.
///  The function Level_textChanged is then called with the new CFL value.
///  Argument : the character corresponding to the label of the selected push button.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::ComputeChar( char btn )
{
	QString lev=Level->text();
	lev+=btn;
	LevelTextChanged(lev);
	Level->setText (m_strLevel);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function checks whether the added character thanks to the push buttons
///  or the keyboard keys (linked to the event textChanged) is valid or not.
///  If the input is valid and the FL entered is 3 digits long, the input is
///  submitted and the pop-up is automatically closed.   
///  Argument : the current text in the edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicCfl015
//
//-----------------------------------------------------------------------------
void CFlSelectOther::LevelTextChanged( const QString & textIni)
{
    //If the text is longer than 3 char, only the last one is kept
	bool bBadEnter=false;
	QString text = textIni;
	if (text.length()>3)
	{
		Level->setText(text.left(1));
		text = text.right(1);
	}
	//Each char is checked, only numbers are allowed
	for (int i=0; i<((int)text.length()); i++)
	{
		QChar c=text[i];
		bool bDigit=(c>='0') && (c<='9');
		if (!bDigit)
			bBadEnter=true;
	}	
	//As this check is performed each time an input is performed, the not correct
	//char is the last one and is removed
	if(bBadEnter)
	{
		int iCurPos=Level->cursorPosition()-1;
		Level->setText (m_strLevel);
	
		if(iCurPos>=0&&iCurPos<(int)text.length())
			Level->setCursorPosition(iCurPos);
	}
	//If the input is correct and length == 3 the entered value is validated
	else if(m_strLevel!=text)
	{
		m_strLevel=text;
		if (m_strLevel.length()==3)
		{
			m_Level=m_strLevel.toInt();
			killTimer(m_TimerId);
			m_TimerId=0;
			accept();
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
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::mouseMoveEvent( QMouseEvent * )
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
void CFlSelectOther::timerEvent( QTimerEvent * e )
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
void CFlSelectOther::mousePressEvent( QMouseEvent * e )
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
///  Argument : int, the current flight level value to be displayed in the 
///  edit text field as selected at invocation of the pop-up. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::Init(int cfl)
{
	m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
	setMouseTracking(true);
	this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));

	//The need to move the window according to the display of the title bar is
	//identified
	QWidget* w = CAsdApp::GetApp()->GetTrackMngr()->m_pFlSelectOtherDialog;
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(w, CWFAttrib::POPUP);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();

	//The pop-up must open so that the mouse pointer is over the Assr edit text field
	QRect rect1=Level->geometry();
	QPoint pnt=2*pos()-geometry().topLeft()-rect1.center();
	pnt.setY(pnt.y() - TitleOffset);
	move(pnt);
	//the current flight level value to be displayed in the 
	//edit text field as selected 
	if (cfl!=0)
	{
		m_strLevel.sprintf("%.3d",cfl);
		Level->setText(m_strLevel);
	}
	else
		Level->setText("");

    m_Level=0;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when the "Enter" key of the keyboard is pressed.
///  This action leads to the validation of the input, then the pop-up is
///  automatically closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicCfl015
//
//-----------------------------------------------------------------------------
void CFlSelectOther::LevelReturnPressed()
{
  m_Level=Level->text().toInt();
  killTimer(m_TimerId);
  m_TimerId=0;
  accept();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when an CFlSelectOther object is destroyed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFlSelectOther::destroy()
{
}

//--------------------------------< THE END >----------------------------------
