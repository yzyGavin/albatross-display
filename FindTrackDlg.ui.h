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
//  FILENAME: FindTrackDlg.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the interactions of Find Track window
// 
//  COMMENTS
//    x
//
//  BUILD INFORMATION 
//    x
//
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
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
#include "View.h"
#include "Track.h"
#include "AsdApp.h"

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CFindTrackDlg
/// This class is used to highlight a searched track
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Function called upon acknowledgement of the input in the text field. It allows 
///  to check whether or not the searched track exists or not. If it exists it is
///  highlighted, otherwise an error message is displayed in the FindTrack window
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFindTrackDlg::buttonOk_clicked()
{
 
 bool TrackFound=false;
 //Call of the Find Track function
 TrackFound = CAsdApp::GetApp()->GetTrackMngr()->FindTrack(textEdit1->text());
 if (TrackFound)
 {
  //If the track has been found and highlighted the window is closed
  accept();
 }
 else
 {
  //Otherwise the window remains open and the following error message is 
  //displayed in the status bar
  textLabel1->setText("Track not found");
 }
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Function called to clear the text fields
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFindTrackDlg::Init()
{
  qDebug("*************************************************************************************************************************");
	textEdit1->setText("");
    textLabel1->setText("");
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called at every input of the user in the edit 
///  text field, to check if the input is valid and set the inputs in upper case.
/// 
///  Argument : QString, the new content of the edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CFindTrackDlg::searchedCallsignChanged( const QString & callsign )
{
	QString strUpper(callsign.upper());
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
		int iCurPos=textEdit1->cursorPosition()-1;
		textEdit1->setText (strUpper);
	
		if(iCurPos>=0&&iCurPos<(int)textEdit1->text().length())
			textEdit1->setCursorPosition(iCurPos);
	}
	else
	{
		int iCurPos=textEdit1->cursorPosition();

		textEdit1->setText (strUpper);

		if(iCurPos>=0&&iCurPos<(int)textEdit1->text().length())
			textEdit1->setCursorPosition(iCurPos);
	}

}
