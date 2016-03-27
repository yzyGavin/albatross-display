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
//  FILENAME: UserMngr.h
//  TYPE    : header file
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
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 1  2009/08/31 dalleins
//
//
// 
//-----------------------------------------------------------------------------
#ifndef USERMNGR_H
#define USERMNGR_H

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include "SignIn.h"
#include "ChangePassword.h"
#include "./CommonFiles/IniFile.h"

//-GLOBAL DATA-----------------------------------------------------------------

//-CLASSES DEFINITIONS---------------------------------------------------------

class CUserMngr  
{

 public:

  //variables
  CChangePasswd m_WChangePasswd;

  //methods
  CUserMngr();
  bool IsSignedIn();
  QString GetUserName();  
  void ChangePassword(QWidget* w);
  void SignIn(QWidget* w);
  void SignOut(QWidget* w);
  QString SignIn(const QString & strUserName, const QString & strPassword);
  QString ChangePassword(const QString & strOldPassword, const QString & strNewPassword1, const QString & strNewPassword2);
  bool Init();

  //virtual methods
  virtual ~CUserMngr();

 private:

  //variables
  bool m_bIsSignedIn;
  bool m_bCustomWarningDisplay;
  QString m_strUserName;
  QString m_strCustomSetFile;
  CSignIn m_WSignIn;
  QStringList m_qslUserList;
  QString m_strCpAsd510;

};

#endif // !defined USERMNGR_H

//--------------------------------< THE END >----------------------------------
