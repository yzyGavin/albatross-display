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
//  FILENAME: SaveAs.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the interaction 
//		with the saveas button of the customization set window
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

#include "WdwMngr.h"
#include "AsdApp.h"

//-PROTOTYPES------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CSSaveAs
/// This class is used to create the Save As window and sets the user interactions
///	on it 
// 
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method check if the text is compliant with the naming
///		conventions defined in the specifications
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CSSaveAs::LeCheckText(const QString & strText)
{
    //Naming conventions.
	QString strUpper(strText.upper());
	bool bBadChar=false;

    for (int i=0; i<((int)strUpper.length()); i++)
    {
		QChar c=strUpper[i];
		bool bMaj=(c>='A') && (c<='Z');
		bool bDigit=(c>='0') && (c<='9');
		if (!(bMaj||bDigit))
			bBadChar=true;
    }

	//If Ok button should not be sensible to 0 length string suppress the
	//following line and set the proporty of SaveAs.ui::pbOk.enabled to true.
	pbOk->setEnabled(!strUpper.isEmpty());


	if(bBadChar)
	{
		int iCurPos=leName->cursorPosition()-1;
		leName->setText (m_strName);
	
		if(iCurPos>=0&&iCurPos<(int)leName->text().length())
			leName->setCursorPosition(iCurPos);
	}
	else
	{
		int iCurPos=leName->cursorPosition();

		m_strName=strUpper;
		leName->setText (m_strName);

		if(iCurPos>=0&&iCurPos<(int)leName->text().length())
			leName->setCursorPosition(iCurPos);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method closes the window if the user clicks WB with
///		the use of the mouse
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CSSaveAs::mousePressEvent( QMouseEvent * e )
{
	if (e->button()==MidButton)
	{
		CWdwMngr::HideWindow(this);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called every time a key is pressed
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CSSaveAs::keyPressEvent( QKeyEvent * e )
{
	switch (e->key())
	{
		case Key_Escape:
			  return;
		default:
			break;
	}

	QDialog::keyPressEvent ( e );
}

//--------------------------------< THE END >----------------------------------






