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
//  FILENAME: Brightness.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the interaction of Brightness Control Window
// 
//  COMMENTS
// //-----------------------------------------------------------------------------
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
#include "ColorMngr.h"
#include "StdAfx.h"
#include "View.h"
#include "DisplayElement.h"
#include "VisuMap.h"
#include "RwdView.h"
#include "./CommonFiles/IniFile.h"
#include "FlightPlan.h"
#include "stdlib.h"
#include "AsdApp.h"
//-GLOBAL DATA-----------------------------------------------------------------


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CColorDlg
///		This class is a dialog box which allows to modify 
///		the brightness categories.
///		It is just composed by several sliders (One by category). 
///
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes all slider values.
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CColorDlg::Init()
{
	m_OldMasterValue=CColorMngr::GetBrightness(MasterCategory);
	m_IsInit=true;
	MasterSlider->setValue(m_OldMasterValue);
	AocSymbSlider->setValue(CColorMngr::GetBrightness(AOCSymbCategory));
	AocDbSlider->setValue(CColorMngr::GetBrightness(AOCDbCategory));
	OthSymbSlider->setValue(CColorMngr::GetBrightness(NonAOCSymbCategory));
	OthDbSlider->setValue(CColorMngr::GetBrightness(NonAOCDbCategory));
	MouseSlider->setValue(CColorMngr::GetBrightness(MouseCategory));
	WeatherSlider->setValue(CColorMngr::GetBrightness(WeatherCategory));
	MapInSlider->setValue(CColorMngr::GetBrightness(LineCategory));
	MapOutSlider->setValue(CColorMngr::GetBrightness(OtherMapCategory));
	MapSymbSlider->setValue(CColorMngr::GetBrightness(NavCategory));
	WindowSlider->setValue(CColorMngr::GetBrightness(WindowsCategory));
	AsdToolsSlider->setValue(CColorMngr::GetBrightness(AsdToolsCategory));
	RangeMarkSlider->setValue(CColorMngr::GetBrightness(RangeMarkersCategory));
	DFSlider->setValue(CColorMngr::GetBrightness(DFFinderCategory));
	m_IsInit=false;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		The master slider modifies all other slider values .
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::MasterSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	int diff = value-m_OldMasterValue;
	int newvalue=AocSymbSlider->value()+diff;
	if ((newvalue>AocSymbSlider->maxValue()) || (newvalue<AocSymbSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=AocDbSlider->value()+diff;
	if ((newvalue>AocDbSlider->maxValue()) || (newvalue<AocDbSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=OthSymbSlider->value()+diff;
	if ((newvalue>OthSymbSlider->maxValue()) || (newvalue<OthSymbSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=OthDbSlider->value()+diff;
	if ((newvalue>OthDbSlider->maxValue()) || (newvalue<OthDbSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=MouseSlider->value()+diff;
	if ((newvalue>MouseSlider->maxValue()) || (newvalue<MouseSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=WeatherSlider->value()+diff;
	if ((newvalue>WeatherSlider->maxValue()) || (newvalue<WeatherSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=MapInSlider->value()+diff;
	if ((newvalue>MapInSlider->maxValue()) || (newvalue<MapInSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=MapOutSlider->value()+diff;
	if ((newvalue>MapOutSlider->maxValue()) || (newvalue<MapOutSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=MapSymbSlider->value()+diff;
	if ((newvalue>MapSymbSlider->maxValue()) || (newvalue<MapSymbSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=WindowSlider->value()+diff;
	if ((newvalue>WindowSlider->maxValue()) || (newvalue<WindowSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=AsdToolsSlider->value()+diff;
	if ((newvalue>AsdToolsSlider->maxValue()) || (newvalue<AsdToolsSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	newvalue=RangeMarkSlider->value()+diff;
	if ((newvalue>RangeMarkSlider->maxValue()) || (newvalue<RangeMarkSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}

	newvalue=DFSlider->value()+diff;
	if ((newvalue>DFSlider->maxValue()) || (newvalue<DFSlider->minValue()))
	{
		MasterSlider->setValue(m_OldMasterValue);
		return;
	}
	AocSymbSlider->setValue(AocSymbSlider->value()+diff);
	AocDbSlider->setValue(AocDbSlider->value()+diff);
	OthSymbSlider->setValue(OthSymbSlider->value()+diff);
	OthDbSlider->setValue(OthDbSlider->value()+diff);
	MouseSlider->setValue(MouseSlider->value()+diff);
	WeatherSlider->setValue(WeatherSlider->value()+diff);
	MapInSlider->setValue(MapInSlider->value()+diff);
	MapOutSlider->setValue(MapOutSlider->value()+diff);
	MapSymbSlider->setValue(MapSymbSlider->value()+diff);
	WindowSlider->setValue(WindowSlider->value()+diff);
	AsdToolsSlider->setValue(AsdToolsSlider->value()+diff);
	RangeMarkSlider->setValue(RangeMarkSlider->value()+diff);
	DFSlider->setValue(DFSlider->value()+diff);
	m_OldMasterValue=value;
	CColorMngr::SetBrightness(MasterCategory,value);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::AocSymbSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(AOCSymbCategory,value);
	CRwdView::UpdateAllViews();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::AocDbSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(AOCDbCategory,value);
	CRwdView::UpdateAllViews();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::OthSymbSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(NonAOCSymbCategory,value);
	CRwdView::UpdateAllViews();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::OthDbSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(NonAOCDbCategory,value);
	CRwdView::UpdateAllViews();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::MouseSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(MouseCategory,value);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::WeatherSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(WeatherCategory,value);
	CRwdView::UpdateAllViews();

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::MapInSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(OtherMapCategory,value);
	CRwdView::UpdateAllViews();

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::MapOutSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(LineCategory,value);
	CRwdView::UpdateAllViews();

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::MapSymbSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(NavCategory,value);
	CRwdView::UpdateAllViews();

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::RngMarkSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(RangeMarkersCategory,value);
	CRwdView::UpdateAllViews();	
}


/* DEAD_CODE_BUILD2 BEGIN 
void CColorDlg::TfcListSlider_valueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(TrafficListCategory,value);
}
DEAD_CODE_BUILD2 END */ 

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::WindowSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(WindowsCategory,value);	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::AsdToolsSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(AsdToolsCategory,value);
	CRwdView::UpdateAllViews();

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the slider value is changed.
///		Argument: value is the new slider value 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::DFSliderValueChanged( int value)
{
	if (m_IsInit)
		return;
	CColorMngr::SetBrightness(DFFinderCategory,value);
	CRwdView::UpdateAllViews();

}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method allows to reset all slider values (to default value: 100)
///		when the reset button is pressed.	
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctBcw030
//
//-----------------------------------------------------------------------------
void CColorDlg::ResetPressed()
{
	m_IsInit=true;
	MasterSlider->setValue(100);
	m_IsInit=false;
	AocSymbSlider->setValue(100);
	AocDbSlider->setValue(100);
	OthSymbSlider->setValue(100);
	OthDbSlider->setValue(100);
	MouseSlider->setValue(100);
	WeatherSlider->setValue(100);
	MapInSlider->setValue(100);
	MapOutSlider->setValue(100);
	MapSymbSlider->setValue(100);
	WindowSlider->setValue(100);
	AsdToolsSlider->setValue(100);
	RangeMarkSlider->setValue(100);
	DFSlider->setValue(100);
	m_OldMasterValue=100;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method allows to hide this window when the middle button is pressed.
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CColorDlg::mousePressEvent( QMouseEvent * e )
{
	if (e->button()==MidButton)
	{
		CWdwMngr::HideWindow(this);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method allows to hide the window instead of to close and delete it
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CColorDlg::closeEvent( QCloseEvent * )
{
	CAsdApp::GetApp()->getDefTlmPosList()->SetDefaultTlmPos("BRGHT_CTRL", this->x(), this->y(), CAsdApp::GetApp()->desktop()->screenNumber(this));
	CWdwMngr::HideWindow(this);
}

//--------------------------------< THE END >----------------------------------
