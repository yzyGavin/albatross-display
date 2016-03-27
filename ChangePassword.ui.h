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
//  FILENAME: ChangePassword.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CChangePasswd.
//		This file contains functions to manage the Change Password window. 
//				
// 
//  COMMENTS
//		Refer to the file ChangePassword.ui to check the connections between 
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

#include "AsdApp.h"
#include "UserMngr.h"

//-GLOBAL DATA-----------------------------------------------------------------

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CChangePasswd
/// This class is used to define the content of the window and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the OK push button, this function calls the ChangePassword
///  function which determines if the change of password is accepted or not.
///	 If the change is accepted, the window is closed, in the other case an error
///  message returned by the ChangePassword function is displayed in the status bar.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctCpw010
//
//-----------------------------------------------------------------------------
void CChangePasswd::OkClicked()
{
  	QString strAnswer=CAsdApp::GetApp()->GetUserMngr()->ChangePassword(CurrentPassword->text(),NewPassword->text(),ConfirmPassword->text());
	if (strAnswer.isEmpty())
		CWdwMngr::HideWindow(this);
	else
	{
		//The error message returned by the ChangePassword function is displayed
		//in the status bar and the edit text fields are emptied.
		Error->setText(strAnswer);
		CurrentPassword->clear();
		NewPassword->clear();
		ConfirmPassword->clear();
		CurrentPassword->setFocus();
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
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CChangePasswd::CancelClicked()
{
	CWdwMngr::HideWindow(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon an input within the CurrentPassword edit text field, the function
///  ApplyNamingConventions is called to check the validity of the input.
///  Argument : QString, the content of the CurrentPassword edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CChangePasswd::PasswordChanged( const QString & strPassword )
{
    ApplyNamingConventions(strPassword,m_strCurPW,CurrentPassword);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon an input within the NewPassword edit text field, the function
///  ApplyNamingConventions is called to check the validity of the input.
///  Argument : QString, the content of the NewPassword edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CChangePasswd::NewPasswordChanged( const QString & strNewPassword )
{
    ApplyNamingConventions(strNewPassword,m_strNewPW,NewPassword);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon an input within the ConfirmPassword edit text field, the function
///  ApplyNamingConventions is called to check the validity of the input.
///  Argument : QString, the content of the ConfirmPassword edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CChangePasswd::ConfirmPasswordChanged( const QString & strConfirmPassword )
{
    ApplyNamingConventions(strConfirmPassword,m_strNewConfPW,ConfirmPassword);
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
void CChangePasswd::mousePressEvent( QMouseEvent * e )
{
	if (e->button()==MidButton)
		CWdwMngr::HideWindow(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is called when an CChangePasswd is created (constructor)
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CChangePasswd::init()
{
	//By default the OK push button is not available
	buttonOk->setEnabled(false);
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called at every input of the user, to check if the input is
///  valid and computes the availability of the OK push button.
///  Argument 1 : QString, the content of the edit text field.
///  Argument 2 : QString, the text in the edit text field before the user input.
///  Argument 3 : QLineEdit, an edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctCpw015, CIAsdFctCpw020
//
//-----------------------------------------------------------------------------
void CChangePasswd::ApplyNamingConventions( QString strName, QString & strCurName, QLineEdit * qle )
{
	QString strUpper(strName.upper());
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
		int iCurPos=qle->cursorPosition()-1;
		qle->setText (strCurName);

		if(iCurPos>=0&&iCurPos<(int)qle->text().length())
			qle->setCursorPosition(iCurPos);
	}
	else
	{
		int iCurPos=qle->cursorPosition();
		strCurName=strUpper;
		qle->setText (strCurName);

		if(iCurPos>=0&&iCurPos<(int)qle->text().length())
			qle->setCursorPosition(iCurPos);
	}

	//Sets the availability of the OK push button
	QString strCur=CurrentPassword->text();
	QString strNewPass=NewPassword->text();
	QString strNewPass2=ConfirmPassword->text();
	buttonOk->setEnabled(strCur.length()>4 && strNewPass.length()>4 && strNewPass2.length()>4);
}

//--------------------------------< THE END >----------------------------------
