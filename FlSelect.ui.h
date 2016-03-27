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
//  FILENAME: FlSelect.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the interactions of the CFL popup window
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
#include "InfoMngr.h"
#include "DataMngr.h"
#include "AsdApp.h"
#include "FlightPlan.h"
#include "WfAttrib.h"
#include "View.h"
#include "Track.h"
#if (defined Q_WS_X11)
#include "WinHints.h"
#endif

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class FlSelectDlg
/// This class is used to define the content of the pop-up and all available 
/// interactions. 
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pDepDefault push button, this function fills the member
///  parameter m_Cfl with the button label. The pop-up is then automatically closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicCfl015, CIAsdFctDfdCicMis010
//
//-----------------------------------------------------------------------------
void CFlSelectDlg::pDepDefaultPressed()
{
	//the label of the push button is retrieved, the autoclose timer is 
	//killed and the pop-up is closed.
	m_Cfl=m_Default;
	killTimer(m_TimerId);
	m_TimerId=0;
	accept();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pArrDefault push button, this function fills the member
///  parameter m_Cfl with the button label. The pop-up is then automatically closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicCfl015, CIAsdFctDfdCicMis010
//
//-----------------------------------------------------------------------------
void CFlSelectDlg::pArrDefaultPressed()
{
	//the label of the push button is retrieved, the autoclose timer is 
	//killed and the pop-up is closed.
	m_Cfl=m_Default;
	killTimer(m_TimerId);
    m_TimerId=0;
	accept();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the pOther push button the pop-up is then automatically
///  closed.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CFlSelectDlg::pOtherPressed()
{
	//the autoclose timer is killed and the pop-up is closed. The m_Cfl parameter 
	//is set to -1 to indicate the CFL other pop-up must be invoked.
	m_Cfl=-1;
	killTimer(m_TimerId);
    m_TimerId=0;
	accept();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon a click AB on the list box, this function fills the member parameter
///  m_Cfl with the selected item label. The pop-up is then automatically closed.
///  The Events returnPressed and clicked are connected to this function.
///  Argument : the selected list box item
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicCfl015, CIAsdFctDfdCicMis010
//
//-----------------------------------------------------------------------------
void CFlSelectDlg::pListLevelClicked( QListBoxItem * item)
{
	//If item is not valid (==0) nothing is done
	//in the other case the selected value is retrieved, the autoclose timer is 
	//killed and the pop-up is closed.
	if (item)
	{
		//If a kind of approach clearance is selected the m_Cfl is set to 0.
		if (pListLevel->count()-1-pListLevel->index(item)<m_iKdAppClearNb)
		{
			m_KdOfAppClear=item->text();
			m_Cfl=0;
		}
		else
		{
			QString str=item->text();
			m_Cfl=str.toInt();
		}
		killTimer(m_TimerId);
		m_TimerId=0;
		accept();
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
void CFlSelectDlg::init()
{
	//It serves to identify the first invocation of the window (See Init method).
	m_FirstInit=true;	
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Upon invocation of the pop-up, the needed information about the track are 
///  retrieved and used to initialize the content and presentation of the
///  window widgets.
///  Argument 1 : The Identifier of the track for which the pop-up has been invoked.
///  Argument 2 : bool set to true if the track is associated to a flight plan.
///  Argument 3 : Flight Rule Category data field.
///  Argument 4 : The departure airport of the track for which the pop-up has been
///  invoked.
///  Argument 5 : The departure runway of the track for which the pop-up has been
///  invoked.
///  Argument 6 : bool set to true if the flight is a departure flight
///  Argument 7 : bool set to true if the flight is a arrival flight
///  Argument 8 : The curent CFL value of the track.
///  Argument 9 : The current kind of approach clearance of the track, if any.
///  Argument 10 : bool set to true if the offline defined kind of approach clearance 
///  must be added to the list box (when pop-up used for another value than CFL: 
///  e.g. RFL, REP).
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctDfdCicCfl005, CIAsdFctDfdCicMis005, CIAsdFctDfdCicRfl005
//
//-----------------------------------------------------------------------------
void CFlSelectDlg::Init( QString Arcid, bool IsFpl, QString FltRul, QString Adep, QString DepRwy, bool Dep, bool Arr , int OldValue, QString AppKind, bool DisplayAppKind)
{
	// CDS BREAK NGT 22/05/2006 Bad method length rule violated. The code has been 
	//							factorized as much as possible. The resulting length
	//							is kept as is to maintain a good readibility of the
	//							and to keep the consistency of the function.

	//The need to move the window according to the display of the title bar is
	//identified
	QWidget* w = CAsdApp::GetApp()->GetTrackMngr()->m_pFlSelectDialog;
	bool HasTitle=CAsdApp::GetApp()->HasWindowTitle(w, CWFAttrib::POPUP);
	int TitleOffset=0;
	if (HasTitle)
		TitleOffset=CDataMngr::GetTitleOffset();
	m_KdOfAppClear="";
	int selecteditem=-1;
	setMouseTracking(true);
	m_TimerId=startTimer(CAsdApp::GetApp()->getAutoCloseTime());
	CInfoMngr* pInfoMngr= CAsdApp::GetApp()->GetInfoMngr();
	QRect rect1,rect2,rect3,rect4,rect5;
	QPoint pnt;
	int dx, dy;
	QString str, strminus, strplus;
	bool closestvalue=false;
	int tmpOldValue, boucle=1;
	int defitem=-1;
	int DefValue=atoi(pInfoMngr->GetDefaultCfl());
	int curitem=-1;
	int Mode=2;
	this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));

	//If it is the first Init of the window, the initial position of each widget 
	//is saved, in the other case those widgets are moved back to their initial 
	//positions.
	pListLevel->clear();
	if (m_FirstInit)
	{
		m_FirstInit = false;
		PopupSize = QSize(120,294);
		m_DepPos=pDepDefault->frameGeometry().topLeft();
		m_ListPos = pListLevel->frameGeometry().topLeft();
		m_ArrPos = pArrDefault->frameGeometry().topLeft();
		m_OtherPos = pOther->frameGeometry().topLeft();
	}
	else
	{
		resize(PopupSize);
		pDepDefault->move(m_DepPos);
		pListLevel->move(m_ListPos);
		pArrDefault->move(m_ArrPos);
		pOther->move(m_OtherPos);
		pDepDefault->show();
		pArrDefault->show();
	}
	//Mode setting and filling of the list box with the flight level numeric
	//values according to the track parameters.
	if (Arr)
		Mode=1;
	if (Dep)
		Mode=0;
	int step=10, top=400, min=5;
	if (CAsdApp::GetApp()->GetInfoMngr()->IsAppPos() && (IsFpl && (FltRul=="I")))
		min=0;
	pArcid->setText(Arcid);
	for (int fl=top;fl>=min;fl-=step)
	{
		if (!(IsFpl && (FltRul=="I")) && (fl==120))
			step=5;
		str.sprintf("%.3d",fl);
		//the item number corresponding to the current value is saved, if found
		if ((OldValue) && (fl==OldValue))
			curitem=pListLevel->count();
		//the item number corresponding to the default CFL value is saved, if found
		if (DefValue && (fl==DefValue))
			defitem=pListLevel->count();
		pListLevel->insertItem(str);
	}

	//Adding kind of approach clearance in the list
	if (DisplayAppKind)
	{
		pListLevel->insertItem("ILS");
		pListLevel->insertItem("VIS");
		pListLevel->insertItem("GOA");
		pListLevel->insertItem("T&G");
		pListLevel->insertItem("CT2");
		pListLevel->insertItem("CT3");
		pListLevel->insertItem("VOR");
		m_iKdAppClearNb=7;
		if (!OldValue)
		{
			for (int i=(pListLevel->count()-1);i>=0;i--)
			{
				if (pListLevel->text(i)==AppKind)
				{
					curitem=i;
					break;
				}
			}
		}
	}
	else
		m_iKdAppClearNb=0;

	//If the default value is not available in the list box, at invocation,
	//the pop-up window opens so that the mouse pointer is placed over the
	//closest available flight level value. If two available flight levels
	//are closest to the current CFL value, the pop-up window opens so that
	//the mouse pointer is placed over the smaller closest available value.
	//This alternative value is not displayed in reverse video.
	if (OldValue && curitem==-1)
	{
		while (curitem==-1)
		{
			tmpOldValue=OldValue-boucle;
			strminus.sprintf("%.3d",tmpOldValue);
			tmpOldValue=OldValue+boucle;
			strplus.sprintf("%.3d",tmpOldValue);
			for (int i=(pListLevel->count()-1);i>=0;i--)
			{
				if (pListLevel->text(i)==strminus)
				{
					curitem=i;
					closestvalue=true;
					break;
				}
				else if (pListLevel->text(i)==strplus)
				{
					curitem=i;
					closestvalue=true;
					break;
				}
			}
			boucle++;
		}
	}
	else if (DefValue && defitem==-1)
	{
		while (defitem==-1)
		{
			tmpOldValue=DefValue-boucle;
			strminus.sprintf("%.3d",tmpOldValue);
			tmpOldValue=DefValue+boucle;
			strplus.sprintf("%.3d",tmpOldValue);
			for (int i=(pListLevel->count()-1);i>=0;i--)
			{
				if (pListLevel->text(i)==strminus)
				{
					defitem=i;
					closestvalue=true;
					break;
				}
				else if (pListLevel->text(i)==strplus)
				{
					defitem=i;
					closestvalue=true;
					break;
				}
			}
			boucle++;
		}
	}
	QListBoxItem* plist = pListLevel->item(defitem);
	if (OldValue==0)
	{
		if (plist)
		{
			pListLevel->setCurrentItem(plist);
			pListLevel->clearSelection();
		}
	}

	//According to the Mode, the Dep, Arr or both push buttons are removed from
	//display. Remaining widgets are moved accordingly. Also the positionning of the 
	//mouse pointer over the pop-up is defined. The selected item if any is set 
	//and the list box is scrolled according to the mode.
	switch (Mode)
	{
		//Departure flight
		//The current or default item must be at the bottom of the list
		//pArrDefault push button is hidden
	case 0:
		//setting the current item
		if (curitem!=-1)
		{
		    pListLevel->setTopItem(curitem-8);
			if (!closestvalue)
				pListLevel->setCurrentItem(curitem);
			selecteditem=curitem;
		}
		else
		{
			if (defitem!=-1)
				pListLevel->setTopItem(defitem-4);
			selecteditem=defitem;
		}
		m_Default=atoi(str);
		pDepDefault->setText(str);
		//positionning of the window
		if (m_Default)
		{
			pDepDefault->setEnabled(true);
			rect1=pDepDefault->frameGeometry();
			pnt=2*pos()-geometry().topLeft()-rect1.center();
		}
		else
		{
			pDepDefault->setEnabled(false);
			rect1=pListLevel->itemRect(pListLevel->item(selecteditem));
			if (selecteditem==-1 || (selecteditem>3 && (pListLevel->count()-selecteditem>4)))
			{
				pnt=2*pos()-geometry().topLeft()-rect1.center();
				pnt.setY(pnt.y()-TitleOffset);
			}
			else
			{
				pnt=2*pos()-geometry().topLeft()-rect1.center();
				pnt.setY(pnt.y()-TitleOffset);
			}
		}
		
		pnt.setY(pnt.y()-TitleOffset);
		move(pnt);
		pArrDefault->hide();
		rect1=pArrDefault->geometry();
		rect2=pOther->geometry();
		pOther->move(rect1.topLeft());
		rect5=rect();
		dx = rect1.right()+rect1.left();
		dy = rect5.bottom()+rect1.top()-rect2.top();
		resize(dx,dy);
		break;

		//Arrival flight
		//The current or default item must be at the top of the list
		//pDepDefault push button is hidden
	case 1:
		//setting the current item
		if (curitem!=-1)
		  {
		    pListLevel->setTopItem(curitem);
			if (!closestvalue)
			    pListLevel->setCurrentItem(curitem);
			selecteditem=curitem;
		  }	
		else
		{
			if (defitem!=-1)
				pListLevel->setTopItem(defitem-4);
			selecteditem=defitem;
		}
		//setting the usual CFL value
		m_Default=atoi(str);
		pArrDefault->setText(str);
		pDepDefault->hide();
		//positionning of the window
		rect1=pDepDefault->geometry();
		rect2=pListLevel->geometry();
		rect3=pArrDefault->geometry();
		rect4=pOther->geometry();
		pnt=QPoint(rect2.left(),rect1.top());
		pListLevel->move(pnt);
		dx = rect3.left();
		dy = rect3.top()+rect1.top()-rect2.top();
		pnt=QPoint(dx,dy);
		pArrDefault->move(pnt);
		dx = rect4.left();
		dy = rect4.top()+rect1.top()-rect2.top();
		pnt=QPoint(dx,dy);
		pOther->move(pnt);
		rect5=rect();
		dx = rect1.right()+rect1.left();
		dy = rect5.bottom()+rect1.top()-rect2.top();
		resize(dx,dy);
		if (m_Default)
		{
			pArrDefault->setEnabled(true);
			rect1=pArrDefault->frameGeometry();
			pnt=2*pos()-geometry().topLeft()-rect1.center();
		}
		else
		{
			pArrDefault->setEnabled(false);
			rect1=pListLevel->itemRect(pListLevel->item(selecteditem));
			if (selecteditem==-1 || (selecteditem>3 && (pListLevel->count()-selecteditem>4)))
			{
				pnt=2*pos()-geometry().topLeft()-rect1.center();
				pnt.setY(pnt.y()-TitleOffset);
			}
			else
			{
				pnt=2*pos()-geometry().topLeft()-rect1.center();
				pnt.setY(pnt.y()-TitleOffset);
			}
		}
		pnt.setY(pnt.y()-TitleOffset);
		move(pnt);
		break;

		//Other kind of flight
		//The current or default item must be in the middle of the list
		//pDepDefault and pArrDefault push buttons are hidden
	case 2:
		//setting the current item
	    if (curitem!=-1)
		{
		    pListLevel->setTopItem(curitem-4);
			if (!closestvalue)
			    pListLevel->setCurrentItem(curitem);
			selecteditem=curitem;
		}
		else
		{
			if (defitem!=-1)
				pListLevel->setTopItem(defitem-4);
			selecteditem=defitem;
		}
		pDepDefault->hide();
		pArrDefault->hide();
		//positionning of the window
		rect1=pDepDefault->geometry();
		rect2=pListLevel->geometry();
		rect3=pArrDefault->geometry();
		rect4=pOther->geometry();
		pnt=QPoint(rect2.left(),rect1.top());
		pListLevel->move(pnt);
		dx = rect3.left();
		dy = rect3.top()+rect1.top()-rect2.top();
		pnt=QPoint(dx,dy);
		pOther->move(pnt);
		rect5=rect();
		dx = rect1.right()+rect1.left();
		dy = rect5.bottom()-rect4.top()+rect3.top()+rect1.top()-rect2.top(); 
		resize(dx,dy);
		rect2=pListLevel->itemRect(pListLevel->item(selecteditem));
	    if (selecteditem!=-1)// || (selecteditem>3 && (pListLevel->count()-selecteditem>4)))
		{
			pnt=2*pos()-geometry().topLeft()-rect2.center();
			dy = pnt.y()-TitleOffset-rect1.height()-rect2.height()/2;
			pnt.setY(dy);
		}
		move(pnt);
		break;
	default:
		break;
	}
	
	pListLevel->verticalScrollBar()->setLineStep(19);
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
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CFlSelectDlg::mouseMoveEvent( QMouseEvent * )
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
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CFlSelectDlg::timerEvent( QTimerEvent * e )
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
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CFlSelectDlg::mousePressEvent( QMouseEvent * e )
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


/*
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is added to complete the quick search facility, a found item 
///  matching the quick search facility criteria must be displayed (the list must
///  be scrolled so that...) at the top fo the list box.
///  Argument : the selected list box item.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CFlSelectDlg::SetItemAtTop(QListBoxItem* Item)
{
	//If the item is valid, the list is scrolled so that the item "item" is
	//display at the top of the visible entries.
	if (Item)
	{
		int index = pListLevel->index(Item);
		pListLevel->setTopItem(index);
	}
}*/

//--------------------------------< THE END >----------------------------------
