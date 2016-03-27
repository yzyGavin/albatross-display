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
//  FILENAME: UserMngr.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CUserMngr which manages the sign-in/sign-out
//		and change password actions.
//		The following notions are treated: username, password, sign-in required,
//		authorized sign-in, signed-in CWP and custom set files availability.
//				
// 
//  COMMENTS
//
//  BUILD INFORMATION 

//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
//
//  Revision 1  2009/08/31 dalleins
//
//
// 
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include "UserMngr.h"
#include "AsdApp.h"
#include "WdwMngr.h"
#include "TopLevel.h"
#include "WinHints.h"
#include "SKMessageBox.h"
#include "InfoMngr.h"
#include <qlineedit.h>
#include <qdir.h>
#include <qdatetime.h>

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CUserMngr
///  This class manages the sign-in/sign-out and change password actions.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CUserMngr::CUserMngr()
{

  

  m_bIsSignedIn=false;
  m_bCustomWarningDisplay=false;
  m_strUserName="";
  m_strCpAsd510="Invalid Username - Password";  
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CUserMngr::~CUserMngr()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Initialization of the class. 
/// 
///  RETURNS: 
///    true by default
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CUserMngr::Init()
{
	//Declaration of windows, the window properties are set
	CWdwMngr::AddWindow(&m_WChangePasswd,"CHANGE_PASSWD",CWFAttrib::CHANGE_PASSWD, false);
	CWdwMngr::AddWindow(&m_WSignIn,"SIGN_IN", CWFAttrib::SIGN_IN, false);

	//Opening Users File.
	QString strUserFilename=CAsdApp::GetApp()->GetDefaultOfflinePath()+"Users.txt";
	QFileInfo fi(strUserFilename);
	if (fi.exists())
    {
      //Read all usernames;
      QFile fd(strUserFilename);
      fd.open(IO_ReadOnly | IO_Translate);
      int iLength=0;
      QString strLine;
      iLength=fd.readLine(strLine,256);
	  //The user list is filled
      while (iLength>0)
	  {
		  m_qslUserList+=strLine.left(iLength-1);
		  iLength=fd.readLine(strLine,256);
	  }
      fd.close();
    }
	
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the CWP is currently signed in or not.
/// 
///  RETURNS: 
///  bool, whether the CWP is signed in or not
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CUserMngr::IsSignedIn()
{
  return m_bIsSignedIn;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the current username (if the position is already signed-in).
/// 
///  RETURNS: 
///  QString, the username of the currently signed-in user
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
QString CUserMngr::GetUserName()
{
  return m_strUserName;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function displays the Change Password window.
///  Argument : QWidget*, the TopLevel object from which the window is invoked
///				in order to determine the display screen
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctCpw005 (cf.: ChangePassword.ui),
///					 CIAsdFctCpw015 (cf.: ChangePassword.ui)
//
//-----------------------------------------------------------------------------
void CUserMngr::ChangePassword(QWidget* w)
{
  //If not already signed-in this function should not be reachable.
  if (!m_bIsSignedIn)
    return;
  //The text fields are emptied
  m_WChangePasswd.ConfirmPassword->clear();
  m_WChangePasswd.CurrentPassword->clear();
  m_WChangePasswd.NewPassword->clear();
  m_WChangePasswd.Error->clear();
  m_WChangePasswd.CurrentPassword->setFocus();
  //The change password window always opens at an offline defined position
  CAsdApp::GetApp()->WindowMoveDefault(&m_WChangePasswd,"CHANGE_PASSWD",CAsdApp::GetApp()->desktop()->screenNumber(w));
  CWdwMngr::ShowWindow(&m_WChangePasswd,CWFAttrib::CHANGE_PASSWD);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function displays the Sign-in window.
///  Argument : QWidget*, the TopLevel object from which the window is invoked
///				in order to determine the display screen
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctCzs015, CIAsdFctSio020 (cf.: SignIn.ui)
///					 CIAsdFctSio025 (cf.: SignIn.ui)
//
//-----------------------------------------------------------------------------
void CUserMngr::SignIn(QWidget* w)
{
    //The text fields are emptied
	m_WSignIn.leUserName->clear();
	m_WSignIn.lePassword->clear();
	m_WSignIn.Error->clear();
	m_WSignIn.init();
	m_WSignIn.leUserName->setFocus();
	//If invoked from a top level menu, the sign-in window opens in the corresponding
	//screen, else it is displayed in the main screen
	if (w)
		CAsdApp::GetApp()->WindowMoveDefault(&m_WSignIn,"SIGN_IN",CAsdApp::GetApp()->desktop()->screenNumber(w));
	else
		CAsdApp::GetApp()->WindowMoveDefault(&m_WSignIn,"SIGN_IN",0);
	CWdwMngr::ShowWindow(&m_WSignIn,CWFAttrib::SIGN_IN);
}
  


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to cancel the Sign-in information (automatic sign-out)
///  Argument : QWidget*, the TopLevel object from which the sign-out is invoked
///				in order to determine, if necessary, the display screen for the 
///				sign-in window
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctSio085
//
//-----------------------------------------------------------------------------
void CUserMngr::SignOut(QWidget* w)
{
  //If not signed-in this function should not be reachable.
  if (!m_bIsSignedIn)
    return;
  m_bIsSignedIn=false;
  m_strUserName="";
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to check the Sign-in action: correct username,
///  correct password and username authorized
///  Argument 1 : const QString &, the username set in the sign-in window
///  Argument 2 : const QString &, the password set in the sign-in window
/// 
///  RETURNS: 
///  QString, the error message in case the sign-in is not successful, else ""
///
///  REQUIREMENT ID: CIAsdFctCzs035, CIAsdFctCzs040, CIAsdFctCzs045,
///					 CIAsdFctSio040, CIAsdFctSio045
//
//-----------------------------------------------------------------------------
QString CUserMngr::SignIn(const QString & strUserName, const QString & strPassword)
{
	//The password depends on the existence of the password key in the custom
	//set file.
	QString strKey;
	bool bFound=false;

	bFound=(m_qslUserList.findIndex(strUserName)!=-1);

	//The username is in the offline list of username
	if (bFound)
	{
		//The password check must be done with the custom set file if it exists.
		//Else it should be done with the default password.
		m_strCustomSetFile = CAsdApp::GetApp()->getCustomDir() + "/" + strUserName;
		CIniFile iniCustomSet(m_strCustomSetFile);
		strKey="";
		if (iniCustomSet.IsOpen())
			iniCustomSet.GetIniProfileString("GENERAL1234","USER_PASSWORD",strKey);
		else
			//The default password is to be used
			strKey="ABCDE";
		if (strKey!=strPassword)
			return m_strCpAsd510;

		//If the custom set file has not been opened, the file must be created (first 
		//login of the user)
		if (!iniCustomSet.IsOpen())
		{
			QFile newfile(m_strCustomSetFile);
			if (newfile.open(IO_WriteOnly|IO_Translate|IO_Truncate))
			{
				newfile.close();
				if (iniCustomSet.Open(m_strCustomSetFile))
				iniCustomSet.WriteIniProfileString("GENERAL1234","USER_PASSWORD",strKey);
			}
		}

		CIniFile iniAccess;
		m_strUserName=strUserName;
		m_bIsSignedIn=true;
		CTopLevel::HideAllCustomSetWindow();
		CTopLevel::UpdateSystemInfo();
		//If the ODD is not available or if the custom sets are not available, a warning 
		//message is displayed, in the other case the custom set window is displayed
		m_WSignIn.hide();
		//Should Display the custom set window.
		int widgetScrNb=CAsdApp::GetApp()->desktop()->screenNumber(&m_WSignIn);
		CTopLevel::DisplayCustomSetWindow(widgetScrNb);
		return "";
	}
	else
		//The username is not correct
		return m_strCpAsd510;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to check the Change password action: correct password,
///  correct new password confirmation
///  Argument 1 : const QString &, the current password set in the change password window
///  Argument 2 : const QString &, the new password set in the change password window
///  Argument 3 : const QString &, the confirmed new password set in the change password window
/// 
///  RETURNS: 
///  QString, the error message in case the sign-in is not successful, else ""
///
///  REQUIREMENT ID: CIAsdFctCpw022,CIAsdFctCpw023, CIAsdFctCpw025
///					CIAsdFctCpw010
//
//-----------------------------------------------------------------------------
QString CUserMngr::ChangePassword(const QString & strOldPassword, const QString & strNewPassword1, const QString & strNewPassword2)
{
	QString strPassword;
	bool bFound=false;
	bFound=(m_qslUserList.findIndex(m_strUserName)!=-1);

	//The user must be signed-in and have a custom set file
	if (bFound)
	{
		//The password check must be done with the custom set file if it exists.
		//Else it should be done with the default password.
		CIniFile iniCustomSet(m_strCustomSetFile);
		strPassword="";
		if (iniCustomSet.IsOpen())
		{
			iniCustomSet.GetIniProfileString("GENERAL1234","USER_PASSWORD",strPassword);
			iniCustomSet.Close();
		}
		else
			//The password cannot be retrieved
			return "Password not found";
		if (strOldPassword!=strPassword)
			//the current password is wrong
			return "Wrong Password";
		if (strNewPassword1!=strNewPassword2)
			//the password confirmation is wrong 
			return "New password error";

		//If the change is accepted, the new password must be written in the 
		//user custom set file.
		if (iniCustomSet.Open(m_strCustomSetFile))
		{
			QDateTime curtime=QDateTime::currentDateTime();
			iniCustomSet.WriteIniProfileString("GENERAL1234","USER_PASSWORD",strNewPassword2);
			iniCustomSet.WriteIniProfileString("GENERAL1234","LAST_MODIFICATION",curtime.toString());
		}
		return "";
	}
	else
		return "No user file found";
}



//--------------------------------< THE END >----------------------------------
