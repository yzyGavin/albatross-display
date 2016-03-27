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
//  FILENAME: HeightFilterDlg.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the interactions of the Height Filter Window
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
#include "StdAfx.h"
#include "View.h"
#include "AsdView.h"
#include "DisplayElement.h"
#include "./CommonFiles/GraphicSymbol.h"
#include "Track.h"
#include "RwdView.h"
#include "WfAttrib.h"

//-GLOBAL DATA-----------------------------------------------------------------
#define MaxFl 400

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CHeightFilterDlg
/// This class is used to define the content of the pop-up and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon invokation of the window, the edit text fields are filled with the
///  current filter values. The focus is set to the left edit text field. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CHeightFilterDlg::Init()
{
	QString val;
	//Upper filter value
	val.sprintf("%03d",m_pView->GetViewParam(CRwdView::filt_upper_val));
	m_UpperValue=val;
	pUpperEdit->setText(m_UpperValue);

	//Lower filter value
	val.sprintf("%03d",m_pView->GetViewParam(CRwdView::filt_lower_val));
	m_LowerValue=val;
	pLowerEdit->setText(m_LowerValue);

	//Departure filter value
	val.sprintf("%03d",m_pView->GetViewParam(CRwdView::filt_dep_val));
	m_DepartureValue=val;
	pDepartureEdit->setText(m_DepartureValue);

	//Focus setting
	pDepartureEdit->setFocus();
	pLowerEdit->deselect();
	pUpperEdit->deselect();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method checks if the filters consistency rule is respected:
///  Departure value < Lower value < Upper value.
///  If the input is not consistent an error message is displayed in the
///  window status bar.y
/// 
///  RETURNS: 
///  bool: whether the inputs are correct or not
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
bool CHeightFilterDlg::CheckConsistency()
{
	int lower, upper, departure;
	bool ok1=false;
	bool ok2=false;
	bool ok3=false;
	QString msg= "Inconsistent values";
	bool IsConsistent = false;

	int DepLen = pDepartureEdit->text().length();
	int LowLen = pLowerEdit->text().length();
	int UppLen = pUpperEdit->text().length();
	//The length of the input must be 3 digits long, if not an error message is
	//displayed in the status bar
	if (DepLen == 3 && LowLen == 3	&& UppLen == 3)
	{
		lower = pLowerEdit->text().toInt(&ok1);
		upper = pUpperEdit->text().toInt(&ok2);
		departure = pDepartureEdit->text().toInt(&ok3);
		//The value 000 is forbiden, if it is not the case an error message is
		//displayed in the status bar
		if (ok1 && ok2 && ok3)
		{
			// If the consistency rule Departure value < Lower value < Upper value
			//is respected IsConsistent is set to true
			if ((lower<upper) && (departure <= lower))
			{
				IsConsistent = true;
				Error->clear();
			}
			else
				Error->setText(msg);
		}
		else
			Error->setText(msg);
	}
	else
		Error->setText(msg);

	return IsConsistent;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  When the OK push button is clicked, if the current inputs are correct the
///  filter values and active status are
///  updated for the associated radar window (TpView) and the window is closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CHeightFilterDlg::OkClicked()
{
	//If the inputs are not correct an error message is displayed in the
	// window status bar and the window remains open.
	if (CheckConsistency())
	{
		//The values are also saved in the member parameters
		m_LowerValue=pLowerEdit->text();
		m_UpperValue=pUpperEdit->text();
		m_DepartureValue=pDepartureEdit->text();

		m_pView->SetViewParam(CRwdView::filt_lower_on,true);
		m_pView->SetViewParam(CRwdView::filt_lower_val,pLowerEdit->text().toInt());

		m_pView->SetViewParam(CRwdView::filt_upper_on,true);
		m_pView->SetViewParam(CRwdView::filt_upper_val,pUpperEdit->text().toInt());

		m_pView->SetViewParam(CRwdView::filt_dep_on,true);
		m_pView->SetViewParam(CRwdView::filt_dep_val,pDepartureEdit->text().toInt());		

		//the inputs are cleared before hidding the window
		Error->clear();
		pLowerEdit->clear();
		pUpperEdit->clear();
		pDepartureEdit->clear();

		CWdwMngr::HideWindow(this);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  When the Cancel push button is clicked, the current inputs are cleared
///  and the window is closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CHeightFilterDlg::CancelClicked()
{
	Error->clear();
	pLowerEdit->clear();
	pUpperEdit->clear();
	pDepartureEdit->clear();
	CWdwMngr::HideWindow(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions filters the inputs: only digits between 0 and 9 are allowed.
///  This function is called for one QLineEdit and for its content.
///  Argument 1 : A string
///  Argument 2 : One QLineEdit
/// 
///  RETURNS: 
///  bool: whether all the characters of the string are numbers or not
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
bool CHeightFilterDlg::TextChanged(const QString & text, QLineEdit* Edit)
{
    bool bChar=false;

	for (int i=0; i<((int)text.length()); i++)
	{
		bool bDigit=(text[i]>='0') && (text[i]<='9');
		if (!bDigit)
			bChar=true;
	}	

	return bChar;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions is called when the text in the pLowerEdit edit text field is
///  modified. If one added character is not correct, it is removed in the other
///  case, the input is treated. 
///  Argument : The string displayed in the pLowerEdit edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsFltHgt010
//
//-----------------------------------------------------------------------------
void CHeightFilterDlg::pLowerEditTextChanged( const QString & strval)
{
	//TextChanged returns true if one char is not correct
	//As this check is performed each time an input is performed, the not correct
	//char is the last one and is removed
	if(TextChanged(strval, pLowerEdit))
	{
		int iCurPos=pLowerEdit->cursorPosition()-1;
		pLowerEdit->setText (m_LowerValue);
	
		if(iCurPos>=0&&iCurPos<pLowerEdit->text().length())
			pLowerEdit->setCursorPosition(iCurPos);
	}
	else
	{
		//If the text is 3 char long, the focus is switch to the next edit text field.
		if(m_LowerValue!=pLowerEdit->text())
		{
			m_LowerValue=pLowerEdit->text();
		
			if (pLowerEdit->text().length()==3)
			{
				pUpperEdit->setFocus();
				pUpperEdit->selectAll();
			}		
		}

		//If all the filter values are 3 char long, the OK button is enabled
		int DepLen = pDepartureEdit->text().length();
		int LowLen = pLowerEdit->text().length();
		int UppLen = pUpperEdit->text().length();
		if (DepLen == 3 && LowLen == 3	&& UppLen == 3)
			OkButton->setEnabled(true);
		else
			OkButton->setEnabled(false);
		CheckConsistency();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions is called when the text in the pUpperEdit edit text field is
///  modified. If one added character is not correct, it is removed in the other
///  case, the input is treated. 
///  Argument : The string displayed in the pUpperEdit edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsFltHgt010
//
//-----------------------------------------------------------------------------
void CHeightFilterDlg::pUpperEditTextChanged( const QString & strval)
{
	//TextChanged returns true if one char is not correct
	//As this check is performed each time an input is performed, the not correct
	//char is the last one and is removed
	if(TextChanged(strval, pUpperEdit))
	{
		int iCurPos=pUpperEdit->cursorPosition()-1;
		pUpperEdit->setText (m_UpperValue);
	
		if(iCurPos>=0&&iCurPos<pUpperEdit->text().length())
			pUpperEdit->setCursorPosition(iCurPos);
	}
	else 
	{
		//If the text is 3 char long, the focus is switch to the next edit text field.
		if(m_UpperValue!=pUpperEdit->text())
		{
			m_UpperValue=pUpperEdit->text();
		}

		//If all the filter values are 3 char long, the OK button is enabled
		int DepLen = pDepartureEdit->text().length();
		int LowLen = pLowerEdit->text().length();
		int UppLen = pUpperEdit->text().length();
		if (DepLen == 3 && LowLen == 3	&& UppLen == 3)
		{
			OkButton->setEnabled(true);
			OkButton->setFocus();
		}
		else
			OkButton->setEnabled(false);

		CheckConsistency();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions is called when the text in the pDepartureEdit edit text field is
///  modified. If one added character is not correct, it is removed in the other
///  case, the input is treated. 
///  Argument : The string displayed in the pDepartureEdit edit text field.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsFltHgt010
//
//-----------------------------------------------------------------------------
void CHeightFilterDlg::pDepartureEditTextChanged( const QString & strval)
{
	//TextChanged returns true if one char is not correct
	//As this check is performed each time an input is performed, the not correct
	//char is the last one and is removed
	if(TextChanged(strval, pDepartureEdit))
	{
		int iCurPos=pDepartureEdit->cursorPosition()-1;
		pDepartureEdit->setText (m_DepartureValue);
	
		if(iCurPos>=0&&iCurPos<pDepartureEdit->text().length())
			pDepartureEdit->setCursorPosition(iCurPos);
	}
	else 
	{
		//If the text is 3 char long, the focus is switch to the next edit text field.
		if(	m_DepartureValue!=pDepartureEdit->text())
		{
			m_DepartureValue=pDepartureEdit->text();

			if (pDepartureEdit->text().length()==3)
			{
				pLowerEdit->setFocus();
				pLowerEdit->selectAll();
			}
		}

		//If all the filter values are 3 char long, the OK button is enabled
		int DepLen = pDepartureEdit->text().length();
		int LowLen = pLowerEdit->text().length();
		int UppLen = pUpperEdit->text().length();
		if (DepLen == 3 && LowLen == 3	&& UppLen == 3)
			OkButton->setEnabled(true);
		else
			OkButton->setEnabled(false);
		CheckConsistency();
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Function called at the creation of the object (constructor).
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CHeightFilterDlg::init()
{
	setMouseTracking(true);
	m_TimerId=0;
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
void CHeightFilterDlg::mousePressEvent( QMouseEvent * e )
{
	//If the pressed button is the wheel button, the timer is killed and the 
	//window is closed. All inputs are discarded.
	if (e->button()==MidButton)
	{
		killTimer(m_TimerId);
		m_TimerId=0;
		Error->clear();
		pLowerEdit->clear();
		pUpperEdit->clear();
		pDepartureEdit->clear();

		CWdwMngr::HideWindow(this);
	}
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
void CHeightFilterDlg::keyPressEvent( QKeyEvent * e )
{
	//If the pressed key is the escape key, the event is discarded as the escape
	//key should not close the window (has no effect)
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

