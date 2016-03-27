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
//  FILENAME: TrackControl.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class TrackControl.
//		This file contains all the actions and interactions linked to the TRACK
//		CONTROL window.
//				
// 
//  COMMENTS
//		Refer to the file TrackControl.ui to check the connections between the 
//		following functions and the window widgets.
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

#include "StdAfx.h"
#include "View.h"
#include "AsdView.h"
#include "DisplayElement.h"
#include "./CommonFiles/GraphicSymbol.h"
#include "Track.h"
#include "AsdApp.h"
#include "RwdView.h"
#include "FontMngr.h"
#include "InfoMngr.h"

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class TrackControl
/// This class is used to define the content of the Track Control window
/// and all available interactions. This window allows the modification 
/// of the track presentation.
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon invocation of the window the needed information for the window settings
///  (current presentation of the track) are retrieved from the associated TpView
///  class.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void TrackControl::Init()
{
 		m_bSliderPressed = false;
		QString str;
		bool IsApp=CAsdApp::GetApp()->GetInfoMngr()->IsAppPos();

		//Settings for Afterglows slider
		int value=m_pView->GetViewParam(CRwdView::hist_length);
		str.sprintf("%d",value);
		DotsLabel->setText(str);
		HistoryDots->setValue(value);
		QRect rect=HistoryDots->sliderRect();
		QPoint pos=QPoint(rect.center().x(),rect.top());
		pos=HistoryDots->mapTo(this,pos);
		rect=DotsLabel->frameGeometry();
		rect.moveCenter(QPoint(pos.x(),pos.y()-rect.height()/2));
		DotsLabel->setGeometry(rect);

		//Settings for Halo slider (will be activated for build 2)
		value=m_pView->GetViewParam(CRwdView::halo_length);
		str.sprintf("%d",value);
		HaloLabel->setText(str);
		Halo->setValue(value);
		rect=Halo->sliderRect();
		pos=QPoint(rect.center().x(),rect.top());
		pos=Halo->mapTo(this,pos);
		rect=HaloLabel->frameGeometry();
		rect.moveCenter(QPoint(pos.x(),pos.y()-rect.height()/2));
		HaloLabel->setGeometry(rect);

		//Settings for prediction vectors
		value=m_pView->GetViewParam(CRwdView::pvect_length);
		SliderVector->setValue(value/10);
		str.sprintf("%d",value);
		VectorLabel->setText(str);
		rect=SliderVector->sliderRect();
		pos=QPoint(rect.center().x(),rect.top());
		pos=SliderVector->mapTo(Prediction_Vector,pos);
		rect=VectorLabel->frameGeometry();
		rect.moveCenter(QPoint(pos.x(),pos.y()-rect.height()/2));
		VectorLabel->setGeometry(rect);
		UncoupledVector->setChecked(m_pView->GetViewParam(CRwdView::pvect_uncpl));
		CoupledVector->setChecked(m_pView->GetViewParam(CRwdView::pvect_coupl));
		TimeMarksVector->setChecked(m_pView->GetViewParam(CRwdView::pvect_time));

		//Optional data fields visibility
		//Full DB fields
		Field0->setChecked(m_pView->GetViewParam(CRwdView::fld_fdb_atyp));
		Field1->setChecked(m_pView->GetViewParam(CRwdView::fld_fdb_rwy));
		Field3->setChecked(m_pView->GetViewParam(CRwdView::fld_fdb_gsp));
		if (IsApp)
			Field3->hide();
		else
			Field3->show();
		Field4->setChecked(m_pView->GetViewParam(CRwdView::fld_fdb_rocd));
		Field5->setChecked(m_pView->GetViewParam(CRwdView::fld_fdb_rvr));
		Field6->setChecked(m_pView->GetViewParam(CRwdView::fld_fdb_modes));
		Field7->setChecked(m_pView->GetViewParam(CRwdView::fld_fdb_ades));
		if (!IsApp)
			Field7->hide();
		else
			Field7->show();
		Field8->setChecked(m_pView->GetViewParam(CRwdView::fld_fdb_sid));
		if (!IsApp)
			Field8->hide();
		else
			Field8->show();

		//Limited DB Fields
		Field10->setChecked(m_pView->GetViewParam(CRwdView::fld_ldb_atyp));
		Field11->setChecked(m_pView->GetViewParam(CRwdView::fld_ldb_rwy));
		Field13->setChecked(m_pView->GetViewParam(CRwdView::fld_ldb_gsp));
		Field14->setChecked(m_pView->GetViewParam(CRwdView::fld_ldb_rocd));
		Field15->setChecked(m_pView->GetViewParam(CRwdView::fld_ldb_rvr));
		Field16->setChecked(m_pView->GetViewParam(CRwdView::fld_ldb_modes));


		//Uncoupled DB Fields
		Field22->setChecked(m_pView->GetViewParam(CRwdView::fld_unc_gsp));
		Field23->setChecked(m_pView->GetViewParam(CRwdView::fld_unc_rocd));
		Field24->setChecked(m_pView->GetViewParam(CRwdView::fld_unc_modes));

		//Tagged DB Fields
		Field25->setChecked(m_pView->GetViewParam(CRwdView::fld_tag_atyp));
		Field26->setChecked(m_pView->GetViewParam(CRwdView::fld_tag_gsp));
		Field27->setChecked(m_pView->GetViewParam(CRwdView::fld_tag_rocd));
		Field28->setChecked(m_pView->GetViewParam(CRwdView::fld_tag_modes));
		
		//Fields is a QButtonGroup to which all the check boxes are associated.
		//When a check box is ticked or unticked, a signal is sent to Fields
		//object with the value associated to the check box.
		Fields->insert(Field0,CRwdView::fld_fdb_atyp);
		Fields->insert(Field1,CRwdView::fld_fdb_rwy);
		Fields->insert(Field3,CRwdView::fld_fdb_gsp);
		Fields->insert(Field4,CRwdView::fld_fdb_rocd);
		Fields->insert(Field5,CRwdView::fld_fdb_rvr);
		Fields->insert(Field6,CRwdView::fld_fdb_modes);
		Fields->insert(Field7,CRwdView::fld_fdb_ades);
		Fields->insert(Field8,CRwdView::fld_fdb_sid);

		Fields->insert(Field10,CRwdView::fld_ldb_atyp);
		Fields->insert(Field11,CRwdView::fld_ldb_rwy);
		Fields->insert(Field13,CRwdView::fld_ldb_gsp);
		Fields->insert(Field14,CRwdView::fld_ldb_rocd);
		Fields->insert(Field15,CRwdView::fld_ldb_rvr);
		Fields->insert(Field16,CRwdView::fld_ldb_modes);

		Fields->insert(Field22,CRwdView::fld_unc_gsp);
		Fields->insert(Field23,CRwdView::fld_unc_rocd);
		Fields->insert(Field24,CRwdView::fld_unc_modes);

		Fields->insert(Field25,CRwdView::fld_tag_atyp);
		Fields->insert(Field26,CRwdView::fld_tag_gsp);
		Fields->insert(Field27,CRwdView::fld_tag_rocd);
		Fields->insert(Field28,CRwdView::fld_tag_modes);

		//Data block font size setting
		FontSize->setButton(m_pView->GetViewParam(CRwdView::font_size));
		QFont font= CFontMngr::GetFontByName("SMALL_NOT_CS_AFS_DBLOCK");
		FontSize1->setFont(font);
		font= CFontMngr::GetFontByName("MEDIUM_NOT_CS_AFS_DBLOCK");
		FontSize2->setFont(font);
		font= CFontMngr::GetFontByName("BIG_NOT_CS_AFS_DBLOCK");
		FontSize3->setFont(font);
		mouseMoveEvent(NULL);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon modification of the prediction vector slider handle position, this function is called
///  to submit the change to the associated TpView class and to update the slider 
///  handle label.
///  Argument : int, the slider value 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppPvs025
//
//-----------------------------------------------------------------------------
void TrackControl::SliderVectorValueChanged( int value)
{
	m_pView->SetViewParam(CRwdView::pvect_length,value*10);
	QString str;
	str.sprintf("%d",value*10);
	VectorLabel->setText(str);
	QRect rect=SliderVector->sliderRect();
	QPoint pos=QPoint(rect.center().x(),rect.top());
	pos=SliderVector->mapTo(Prediction_Vector,pos);
	rect=VectorLabel->frameGeometry();
	rect.moveCenter(QPoint(pos.x(),pos.y()-rect.height()/2));
	VectorLabel->setGeometry(rect);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon modification of the afterglow slider handle position, this function is called
///  to submit the change to the associated TpView class and to update the slider 
///  handle label.
///  Argument : int, the slider value
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppAfg020
//
//-----------------------------------------------------------------------------
void TrackControl::HistoryDotsValueChanged( int value)
{
	m_pView->SetViewParam(CRwdView::hist_length,value);
	QString str;
	str.sprintf("%d",value);
	DotsLabel->setText(str);
	QRect rect=HistoryDots->sliderRect();
	QPoint pos=QPoint(rect.center().x(),rect.top());
	pos=HistoryDots->mapTo(this,pos);
	rect=DotsLabel->frameGeometry();
	rect.moveCenter(QPoint(pos.x(),pos.y()-rect.height()/2));
	DotsLabel->setGeometry(rect);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called when an TrackControl object is destructed.
///  Argument : QObject, the current object (not used in this function).
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void TrackControl::TrackControlDestroyed( QObject * )
{

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION:
///  For build 2. 
///  Upon modification of the Halo slider handle position, this function is called
///  to submit the change to the associated TpView class and to update the slider 
///  handle label.
///  Argument : int, the slider value
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void TrackControl::HaloValueChanged( int value)
{
	m_pView->SetViewParam(CRwdView::halo_length,value);
	QString str;
	str.sprintf("%d",value);
	HaloLabel->setText(str);
	QRect rect=Halo->sliderRect();
	QPoint pos=QPoint(rect.center().x(),rect.top());
	pos=Halo->mapTo(this,pos);
	rect=HaloLabel->frameGeometry();
	rect.moveCenter(QPoint(pos.x(),pos.y()-rect.height()/2));
	HaloLabel->setGeometry(rect);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon modification of one Optional data field check box status, this function is called
///  to submit the change to the associated TpView class.
///  Argument : int, the check box associated identification in the QButtonGroup
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk060
//
//-----------------------------------------------------------------------------
void TrackControl::FieldsPressed( int value)
{
	m_pView->SetViewParam(value,!m_pView->GetViewParam(value));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon modification of the fonts radio button selection, this function is called
///  to submit the change to the associated TpView class.
///  Argument : int, the radio button value
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppDbk040
//
//-----------------------------------------------------------------------------
void TrackControl::FontSizePressed( int value )
{
	m_pView->SetViewParam(CRwdView::font_size,value);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon modification of the Uncoupled check box status, this function is called
///  to submit the change to the associated TpView class.
///  Argument : bool, whether the check box is ticked or not
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppPvs020
//
//-----------------------------------------------------------------------------
void TrackControl::UncoupledVectorToggled( bool on)
{
	m_pView->SetViewParam(CRwdView::pvect_uncpl,on);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon modification of the Coupled check box status, this function is called
///  to submit the change to the associated TpView class.
///  Argument : bool, whether the check box is ticked or not
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppPvs015
//
//-----------------------------------------------------------------------------
void TrackControl::CoupledVectorToggled( bool on)
{
	m_pView->SetViewParam(CRwdView::pvect_coupl,on);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon modification of the Time Marks check box status, this function is called
///  to submit the change to the associated TpView class.
///  Argument : bool, whether the check box is ticked or not
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppPvs030
//
//-----------------------------------------------------------------------------
void TrackControl::TimeMarksVectorToggled( bool on)
{
	m_pView->SetViewParam(CRwdView::pvect_time,on);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon modification pressing the LCM Reset push button, this function is called
///  to cancel the LCM inhibition for manually oriented tracks.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTppLls066
//
//-----------------------------------------------------------------------------
void TrackControl::pLcmResetPressed()
{
	CViewTrack::ResetLcm();
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
void TrackControl::mouseMoveEvent( QMouseEvent *  )
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
void TrackControl::timerEvent( QTimerEvent * e )
{
	//If the timer event is the m_TimerId, the mouse pointer is not over the
	//window and a slider is not pressed, the timer is killed and the window
	//is closed. All inputs are discarded.
	if (m_TimerId==e->timerId() && (!m_bSliderPressed))
	{
		QPoint pnt=QCursor::pos();
		QRect rect=frameGeometry();
		if (!rect.contains(pnt))
		{
			killTimer(m_TimerId);
			m_TimerId=0;
			CWdwMngr::HideWindow(this);
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is called when a TrackControl object is created
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void TrackControl::init()
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
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void TrackControl::mousePressEvent( QMouseEvent * e )
{
 	m_bSliderPressed = false;
	//If the pressed button is the wheel button, the timer is killed and the 
	//window is closed. All inputs are discarded.
	if (e->button()==MidButton)
	{
		killTimer(m_TimerId);
		m_TimerId=0;
		CWdwMngr::HideWindow(this);
		
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is called when the user presses on a slider handle. This method
///  is added to prevent the closing of the window while the user 
///  is moving a slider handle outside the window.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void TrackControl::SliderPressedSlot()
{
	//When the m_bSliderPressed parameter is set to true the autoclose feature
	//is de-activated
 m_bSliderPressed = true;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is called when the user releases any slider handle. This method
///  is added to prevent the closing of the window while the user 
///  is moving a slider handle outside the window.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: ¨NONE
//
//-----------------------------------------------------------------------------
void TrackControl::SliderReleaseSlot()
{
	//Following the release action, the m_bSliderPressed is set to false
	//and the auto-close feature is re-initialized.
	if (m_TimerId)
		killTimer(m_TimerId);
	m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
	m_bSliderPressed = false;

}

//--------------------------------< THE END >----------------------------------
