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
//  FILENAME: SignIn.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CSignIn.
//		This file contains functions to manage the Sign-in window. 
//		This includes the sign-in action, cancel action and the fact that the 
//		window cannot be closed if the sign-in is required and the CWP is not
//		signed in.
//				
// 
//  COMMENTS
//		Refer to the file SignIn.ui to check the connections between 
//		the following functions and the window widgets.
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

#include "UserMngr.h"
#include "AsdApp.h"

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CSignIn
/// This class is used to define the content of the window and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called at the creation of an CSignIn object.
///  The OK button is grayed-out by default.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CSignIn::init()
{
  buttonOk->setEnabled(false);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called at every input of the user in the User Name edit 
///  text field, to check if the input is valid and computes the availability 
///  of the OK push button. 
///  Argument : QString, the new content of the User Name edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CSignIn::UserNameChanged( const QString & strUserName )
{
	//Set the text with the naming conventions.
	QString strUpper(strUserName.upper());
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
		int iCurPos=leUserName->cursorPosition()-1;
		leUserName->setText (m_strUser);
	
		if(iCurPos>=0&&iCurPos<(int)leUserName->text().length())
			leUserName->setCursorPosition(iCurPos);
	}
	else
	{
		int iCurPos=leUserName->cursorPosition();

		m_strUser=strUpper;
		leUserName->setText (m_strUser);

		if(iCurPos>=0&&iCurPos<(int)leUserName->text().length())
			leUserName->setCursorPosition(iCurPos);
	}

    //Sets the availability of the OK push button
	if (lePassword->text().length()>4 && leUserName->text().length()>2)
		buttonOk->setEnabled(true);
	else
		buttonOk->setEnabled(false);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the OK push button, this function calls the SignIn
///  function which determines if the sign-in is accepted or not.
///	 If the sign-in is accepted, the window is closed, in the other case an error
///  message returned by the SignIn function is displayed in the status bar.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctSio030, CIAsdFctSio041, CIAsdFctSio050,
///				     CIAsdFctSio055, CIAsdFctSio060
//
//-----------------------------------------------------------------------------
void CSignIn::OkClicked()
{
	QString strAnswer=CAsdApp::GetApp()->GetUserMngr()->SignIn(leUserName->text(),lePassword->text());
	if (strAnswer.isEmpty())
		hide();
	else 
	{
		Error->setText(strAnswer);
		lePassword->clear();
		lePassword->setFocus();
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the Cancel push button, the window is closed and all the
///  inputs are discarded.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctSio070, CIAsdFctSio075, CIAsdFctSio080
//
//-----------------------------------------------------------------------------
void CSignIn::CancelClicked()
{
	//The Cancel action is only allowed if the CWP is not sign-in required or if 
	//it is sign-in required but it is already signed-in.
	//In the other case the text fields are emptied but the window remains opened.
	leUserName->setFocus();
	lePassword->clear();
	leUserName->clear();
	CWdwMngr::HideWindow(this);
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called at every input of the user in the Password edit 
///  text field, to check if the input is valid and computes the availability 
///  of the OK push button. 
///  Argument : QString, the new content of the Password edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CSignIn::PasswordChanged(const QString & strPassword)
{
	//Set the text with the naming conventions.
	QString strUpper(strPassword.upper());
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
		int iCurPos=lePassword->cursorPosition()-1;
		lePassword->setText (m_strPW);
	
		if(iCurPos>=0&&iCurPos<(int)lePassword->text().length())
			lePassword->setCursorPosition(iCurPos);
	}
	else
	{
		int iCurPos=lePassword->cursorPosition();

		m_strPW=strUpper;
		lePassword->setText (m_strPW);

		if(iCurPos>=0&&iCurPos<(int)lePassword->text().length())
			lePassword->setCursorPosition(iCurPos);
	}

    //Sets the availability of the OK push button
	if (leUserName->text().length()>2 && lePassword->text().length()>4)
		buttonOk->setEnabled(true);
	else
		buttonOk->setEnabled(false);

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
void CSignIn::mousePressEvent( QMouseEvent * e )
{
  	if (e->button()==MidButton)
		CWdwMngr::HideWindow(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when the window is closed with the close button decoration.
///  Argument : Close Event, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CSignIn::closeEvent(QCloseEvent* )
{
	accept();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is called every time a key of the keyboard is pressed.
///  Argument : The key event. Key events occur when a key is pressed or 
///  released when a widget has keyboard input focus.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CSignIn::keyPressEvent( QKeyEvent * e )
{
	QDialog::keyPressEvent ( e );
}

//--------------------------------< THE END >----------------------------------
