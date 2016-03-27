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
//  FILENAME: AsdMenu.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the radar menu, pull down menu, cascading menu, context menu
// 
//  COMMENTS
//
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


//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include "AsdMenu.h"
#include "ColorMngr.h"
#include "FontMngr.h"
#include "WdwMngr.h"
#include "AsdApp.h"


//-GLOBAL DATA-----------------------------------------------------------------

CMenuWnd* pCurMenu = NULL;
bool m_bLeftDisplay = false;
CMenuWnd* pLeftDisplayMenu = NULL;


//-PROTOTYPES------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CMenuWnd
/// This class is used to construct the ASD menu or sub menu such as Radar menu,
/// Callsign menu, PPS context menu and STCA context menu  
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CMenuWnd::CMenuWnd(CMenuWnd& pMenu)
:QWidget (0,0)
{
	m_pMenuParent=NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CMenuWnd::CMenuWnd():QWidget (0,0, WType_Dialog|WStyle_Customize  |WStyle_NoBorder ) 
{
	m_pSubMenu = NULL;
	m_Timer1=0;
	m_Timer2=0;
	m_iVerticalTextOffset=-2;
	m_pMenuParent=NULL;
	m_pCommandWnd=NULL;
	m_pDisplayWnd=NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMenuWnd::~CMenuWnd()
{
	CWdwMngr::RemoveWindow(this);

	if (pLeftDisplayMenu==this)
		m_bLeftDisplay=false;

	if (m_pAsdMenu)
		m_pAsdMenu->SetOpen(false);

	if (pCurMenu==this)
		pCurMenu=NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method constructs the ASD context menu
/// 
///  RETURNS: 
///		0 by default
///
///  REQUIREMENT ID: CIAsdFctRwsRsv006, CIAsdFctRwsRsv105
//-----------------------------------------------------------------------------
int CMenuWnd::TrackPopupMenu(CAsdMenu* pMenu,UINT nFlags,int x,int y,CObject* pWnd, CObject *pDisplayWnd, QRect* lprect,CMenuWnd* pParent)
{
	QString menustr;
	QFont font=CFontMngr::GetFontByName("MENU");
		
	m_TimeOut=CAsdApp::GetApp()->getAutoCloseTime();
	m_CurSelect = -1;
	m_pMenuParent = pParent;
	m_pCommandWnd = pWnd;
	m_pDisplayWnd = pDisplayWnd;
	m_pAsdMenu = pMenu;

	if (lprect)
		m_Rect = *lprect;

	m_nFlags = nFlags;
	QSize size,totalsize(0,0),checksize;
	QPoint curpos(0,2);
	QRect currect;
	checksize.setWidth(16);
	checksize.setHeight(16);
	QPainter dc(this);
	dc.setFont(font);
	QRect tmp=dc.boundingRect(0,0,0,0,Qt::AlignLeft,"W");
	curpos.setX(checksize.width());
	m_pAsdMenu->UpdateAllCmdUI(m_pCommandWnd);
	int fld;
	
	for (fld=0;fld<m_pAsdMenu->GetItemCount();fld++)
	{
		if (m_pAsdMenu->IsItemSeparator(fld))
		{
			tmp=dc.boundingRect(0,0,0,0,Qt::AlignLeft,"-");
			size = tmp.size();
		}
		else
		{
			menustr=m_pAsdMenu->GetItemText(fld);
			tmp=dc.boundingRect(0,0,0,0,Qt::AlignLeft,menustr);
			size = tmp.size();
			size.setWidth(size.width()+checksize.width()-2);
			
			if (m_pAsdMenu->GetPopupMenu(fld))
				size.setWidth(size.width()+size.height());
			
			if (checksize.height()>size.height())
				size.setHeight(checksize.height());
			
			size.setHeight(size.height()+4);
		}

		currect.setTop(curpos.y());
		currect.setBottom(curpos.y()+size.height()-1);
		m_RectTable.Add(currect);
		m_RectSelectTable.Add(currect);
		curpos.setY(curpos.y()+size.height()-4);
		m_PointTable.Add(curpos);
		curpos.setY(curpos.y()+4);
		
		if (size.width()>totalsize.width())
			totalsize.setWidth(size.width());

		totalsize.setHeight(totalsize.height()+size.height());
	}

	// set the position of the menu according to the screen number
	totalsize.setWidth(totalsize.width()+checksize.width()+4);
	totalsize.setHeight(totalsize.height()+8);
	QDesktopWidget* pDesktop=QApplication::desktop();
	int mon1,mon2,mon3;
	mon1= pDesktop->screenNumber(QPoint(x,y));
	mon2= pDesktop->screenNumber(QPoint(x+totalsize.width(),y));

	if (m_Rect.isEmpty())
		m_bLeftDisplay=false;

	if ((mon2!=mon1) || (mon2==-1) || (m_bLeftDisplay)) 
	{
		if (!m_Rect.isEmpty())
		{
			x=m_Rect.left()-totalsize.width()-2;
			
			if (!m_bLeftDisplay)
				pLeftDisplayMenu=this;

			m_bLeftDisplay=true;
		}
		else
			x=x-totalsize.width()+2;
	}

	mon1= pDesktop->screenNumber(QPoint(x,y));
	mon3= pDesktop->screenNumber(QPoint(x,y+totalsize.height()));
	
	if ((mon3!=mon1) || (mon3==-1))
	{
		if (!m_Rect.isEmpty())
			y=m_Rect.bottom()-totalsize.height()+4;
		else
			y=y-totalsize.height()+4;
	}

	// Add and set the menu visible
	tmp=QRect(x,y,totalsize.width(),totalsize.height());
	setGeometry(tmp);
	CWdwMngr::AddWindow(this,"MENU", -1,false);
	CWdwMngr::ShowWindow(this,-1);
	currect=QWidget::rect();
	
	for (fld=0;fld<m_pAsdMenu->GetItemCount();fld++)
	{
		m_RectTable[fld].setLeft(m_RectTable[fld].left()+2);
		m_RectTable[fld].setRight(currect.right()-2);
		
		m_RectSelectTable[fld].setRight(currect.right());
	}

	update();
	
	if (!pParent)
	{
		if (pCurMenu)
			pCurMenu->CancelWnd(cancelall);
		
		pCurMenu = this;
		m_pAsdMenu->SetOpen(true);
	}

	// this menu has the auto close behavior so a timer is started
	if(m_Timer1)
		killTimer(m_Timer1);
	m_Timer1=startTimer(m_TimeOut);
	
	setMouseTracking(true);

	return 0;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method draws the ASD menu as a 3D rectangle
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CMenuWnd::Draw3dRect(QPainter* dc,QRect rect,QColor color1,QColor color2)
{
	QPen prevpen=dc->pen();

	dc->setPen(color1);
	dc->drawLine(rect.bottomLeft(),rect.topLeft());
	dc->drawLine(rect.topLeft(),rect.topRight());
	dc->setPen(color2);
	dc->drawLine(rect.topRight(),rect.bottomRight());
	dc->drawLine(rect.bottomRight(),rect.bottomLeft());
	dc->setPen(prevpen);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called to draw the ASD menu
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CMenuWnd::paintEvent ( QPaintEvent * )
{
	QPainter dc(this); // device context for painting
	QFont font=CFontMngr::GetFontByName("MENU");
	dc.setFont(font);
	QRect rect=QWidget::rect();
	dc.fillRect(rect,CColorMngr::GetColor(DefMenuBackClr));
	QString menustr;
	dc.setBackgroundMode(TransparentMode);
	dc.setPen(CColorMngr::GetColor(DefMenuTextClr));
	int lefttopclr=DefMenuLeftTopClr,rightbottomclr=DefMenuRightBottomClr;
	Draw3dRect(&dc,rect,CColorMngr::GetColor(lefttopclr),CColorMngr::GetColor(rightbottomclr));
	rect.addCoords(1,1,-1,-1);
	Draw3dRect(&dc,rect,CColorMngr::GetColor(lefttopclr),CColorMngr::GetColor(rightbottomclr));

	// Draws the ASD menu with an item separator if necessary or a check box
	for (int fld=0;fld<m_pAsdMenu->GetItemCount();fld++)
	{
		if (!m_pAsdMenu->IsItemSeparator(fld))
		{
			QColor Usercolor;
			menustr=m_pAsdMenu->GetItemText(fld);

			if (m_CurSelect==fld)
			{
				rect = m_RectTable[fld];
				dc.fillRect(rect,CColorMngr::GetColor(DefMenuHighBackClr));
				
				if (m_pAsdMenu->IsItemDisabled(fld))
					dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
				else if (m_pAsdMenu->GetItemTextColor(fld, Usercolor))
					dc.setPen(Usercolor);
				else
					dc.setPen(CColorMngr::GetColor(DefMenuHighTextClr));
				
				rect.setLeft(m_PointTable[fld].x());
				rect.setTop(m_PointTable[fld].y());
				dc.drawText(m_PointTable[fld].x(),m_PointTable[fld].y()+m_iVerticalTextOffset,menustr);
			}
			else
			{
				rect = m_RectTable[fld];
				rect.setLeft(m_PointTable[fld].x());
				rect.setTop(m_PointTable[fld].y());
				
				if (m_pAsdMenu->IsItemDisabled(fld))
				{
					dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
					dc.drawText(m_PointTable[fld].x()-1,m_PointTable[fld].y()-1,menustr);
					rect.addCoords(-1,-1,0,0);
					dc.setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
					dc.drawText(m_PointTable[fld].x(),m_PointTable[fld].y()+m_iVerticalTextOffset,menustr);
				}
				else
				{
					if (m_pAsdMenu->GetItemTextColor(fld, Usercolor))
						dc.setPen(Usercolor);
					else
						dc.setPen(CColorMngr::GetColor(DefMenuTextClr));
					
					dc.drawText(m_PointTable[fld].x(),m_PointTable[fld].y()+m_iVerticalTextOffset,menustr);
				}
			}
			if (m_pAsdMenu->IsItemChecked(fld))
			{
				rect = m_RectTable[fld];
				rect.setRight(m_PointTable[fld].x()-1);
				
				if (m_CurSelect==fld)
				{
					if (m_pAsdMenu->IsItemDisabled(fld))
						dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
					else if (m_pAsdMenu->GetItemTextColor(fld, Usercolor))
						dc.setPen(Usercolor);
					else
						dc.setPen(CColorMngr::GetColor(DefMenuHighTextClr));
					
					dc.setBackgroundColor(CColorMngr::GetColor(DefMenuHighBackClr));
				}
				else
				{
					if (m_pAsdMenu->IsItemDisabled(fld))
						dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
					else if (m_pAsdMenu->GetItemTextColor(fld, Usercolor))
						dc.setPen(Usercolor);
					else
						dc.setPen(CColorMngr::GetColor(DefMenuTextClr));
					dc.setBackgroundColor(CColorMngr::GetColor(DefMenuBackClr));
				}

				QPen pen1(dc.pen().color(),2);
				dc.setPen(pen1);
				QPoint pnt1(rect.left()+(rect.right()-rect.left())*2/5,rect.top()+(rect.bottom()-rect.top())*2/3);
				QPoint pnt2(rect.left()+(rect.right()-rect.left())*3/5,rect.bottom()-(rect.bottom()-rect.top())/6);
				QPoint pnt3(rect.left()+(rect.right()-rect.left())*4/5+2,rect.top()+(rect.bottom()-rect.top())*1/3-1);
				
				for (int i=0;i<3;i++)
				{
					dc.drawLine(pnt1,pnt2);
					dc.drawLine(pnt2,pnt3);
					pnt1.setY(pnt1.y()-1);
					pnt2.setY(pnt2.y()-1);
					pnt3.setY(pnt3.y()-1);
				}
			}
			if (m_pAsdMenu->GetPopupMenu(fld))
			{
				rect = m_RectTable[fld];
				rect.setLeft(rect.right()-rect.height());
				
				if (m_CurSelect==fld)
				{
					if (m_pAsdMenu->IsItemDisabled(fld))
						dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
					else
						dc.setPen(CColorMngr::GetColor(DefMenuHighTextClr));

					dc.setBackgroundColor(CColorMngr::GetColor(DefMenuHighBackClr));
				}
				else
				{
					if (m_pAsdMenu->IsItemDisabled(fld))
						dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
					else
						dc.setPen(CColorMngr::GetColor(DefMenuTextClr));

					dc.setBackgroundColor(CColorMngr::GetColor(DefMenuBackClr));
				}
				
				QPen pen = dc.pen();
				QBrush brush1(pen.color());
				QPointArray pnttab(4);
				pnttab.setPoint(0,QPoint(rect.left()+7,rect.bottom()-7));
				pnttab.setPoint(3,QPoint(rect.left()+7,rect.bottom()-7));
				pnttab.setPoint(1,QPoint(rect.left()+7,rect.top()+7));
				pnttab.setPoint(2,QPoint(rect.right()-7,(rect.top()+rect.bottom())/2));
				dc.setBrush(brush1);
				dc.drawPolygon(pnttab,FALSE,0,4);
			}
		}
		else
		{
			QPen pen(CColorMngr::GetColor(DefMenuDisTextClr));
			dc.setPen(pen);
			rect = m_RectTable[fld];
			dc.drawLine(rect.left(),(rect.top()+rect.bottom())/2,rect.right(),(rect.top()+rect.bottom())/2);
			QPen pen2(CColorMngr::GetColor(DefMenuLeftTopClr));
			dc.setPen(pen2);
			dc.drawLine(rect.left(),(rect.top()+rect.bottom())/2+1,rect.right(),(rect.top()+rect.bottom())/2+1);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is construct or delete a menu according to mouse motion
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CMenuWnd::mouseMoveEvent ( QMouseEvent * e )
{
	if(m_Timer1)
		killTimer(m_Timer1);
	m_Timer1=startTimer(m_TimeOut);

	QPoint point = e->pos();

	if (m_FirstMove)
	{
		m_FirstMove = false;
		return;
	}

	int oldselect = m_CurSelect;
	QRect rect=QWidget::rect();

	// Cancel or open a menu according to mouse motion
	if (rect.contains(point))
	{
		m_CurSelect = ComputeSelected(point);
		
		if (m_CurSelect==-1)
		  m_CurSelect=oldselect;

		if (m_CurSelect!=oldselect)
		{
			if (m_CurSelect!=-1)
			{
				update(m_RectTable[m_CurSelect]);

				if (m_pSubMenu)
				{
					m_pSubMenu->CancelWnd(canceldown);
					m_pSubMenu=NULL;
				}

				if(m_Timer2)
					killTimer(m_Timer2);
				m_Timer2=0;

				if ((m_pAsdMenu->GetPopupMenu(m_CurSelect)) && (!m_pAsdMenu->IsItemDisabled(m_CurSelect)))
				{
					m_Timer2=startTimer(200);
				}
			}
			else
			{
				if(m_Timer2)
					killTimer(m_Timer2);
				m_Timer2=0;
			}

			if (oldselect!=-1)
			{
				update(m_RectTable[oldselect]);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method get the selected item
/// 
///  RETURNS: 
///		an integer is returned corresponding to the selected line in the menu
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CMenuWnd::ComputeSelected(QPoint point)
{
	for (int fld=0;fld<m_RectSelectTable.GetSize();fld++)
	{
		if ((m_RectSelectTable[fld].contains(point)) && !(m_pAsdMenu->IsItemSeparator(fld)))
		{
			return fld;
		}
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the menu is associated to a parent
/// 
///  RETURNS: 
///		true if the Menu is associated to a parent otherwise return false
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CMenuWnd::IsMenuParent(QWidget* pWnd)
{
	if (m_pMenuParent==pWnd)
		return true;

	else
	{
		if (m_pMenuParent)
			return m_pMenuParent->IsMenuParent(pWnd);
		else
			return false;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the mouse Press event from the user in order
///		to called the associated method
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CMenuWnd::mousePressEvent ( QMouseEvent * e )
{
	ButtonState btn=e->button();

	switch (btn)
	{
	case LeftButton:
		break;
	case MidButton:
		OnMButtonDown(0,e->pos());
		break;
	case RightButton:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the mouse release event from the user in order
///		to called the associated method
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CMenuWnd::mouseReleaseEvent ( QMouseEvent * e )
{
	ButtonState btn=e->button();
	
	switch (btn)
	{
	case LeftButton:
		OnLButtonUp(0,e->pos());
		break;
	case MidButton:
		break;
	case RightButton:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user clicks WB via the mouse 
///		pointer in order to cancel the menu
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CMenuWnd::OnMButtonDown(UINT nFlags, QPoint point) 
{
	QPoint pnt=mapToGlobal(point);
	QRect rect=geometry();
	
	if (rect.contains(pnt))
	{
		CancelWnd(cancelall);//SendMessage(WM_USER,cancelall,0);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user release the left 
///		button of the mouse pointer in order to activate the functionality associated
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CMenuWnd::OnLButtonUp(UINT nFlags, QPoint point) 
{
	CMenuWnd* pParent=NULL;
	CAsdMenu* pAsdMenu=NULL;
	UINT id;
	
	if ((m_CurSelect!=-1) && !(m_pAsdMenu->IsItemDisabled(m_CurSelect)) && !(m_pAsdMenu->GetPopupMenu(m_CurSelect)) &&  (m_pAsdMenu->GetId(m_CurSelect)!=0))
	{
		QRect rect=m_RectTable[m_CurSelect];
		QPoint pnt1=rect.topLeft(),pnt2=rect.bottomRight();
		mapToGlobal(pnt1);
		mapToGlobal(pnt2);
		rect=QRect(pnt1,pnt2);
		pAsdMenu=m_pAsdMenu;
		pParent=m_pMenuParent;
	
		while (pParent)
		{
			pAsdMenu=pParent->m_pAsdMenu;
			pParent=pParent->m_pMenuParent;
		}

		if (pAsdMenu)
			pAsdMenu->setLastSelectionRect(rect);

		// if the item is auto close it cancel all the menu and activate the functionality
		// else it activates the functionality
		if (m_pAsdMenu->IsItemAutoclose(m_CurSelect))
		{
			id = m_pAsdMenu->GetId(m_CurSelect);
			CObject* tmp=m_pAsdMenu->GetCommandObject(m_CurSelect);
			
			if (!tmp)
				tmp=m_pCommandWnd;

			UINT id=m_pAsdMenu->GetId(m_CurSelect);
			CObject *tmp2=m_pDisplayWnd;
			CancelWnd(cancelall);
			
			if (tmp)
				tmp->OnCommand(id,tmp2);

			return;
		}
		else
		{
			if (m_pAsdMenu->GetCommandObject(m_CurSelect))
				m_pAsdMenu->GetCommandObject(m_CurSelect)->OnCommand(m_pAsdMenu->GetId(m_CurSelect),m_pDisplayWnd);
			else if (m_pCommandWnd)
				m_pCommandWnd->OnCommand(m_pAsdMenu->GetId(m_CurSelect),m_pDisplayWnd);
		}

		for (int fld=0;fld<m_pAsdMenu->GetItemCount();fld++)
		{
			if (m_pAsdMenu->GetId(fld))
			{
				CCmdUI cmdUI;
				cmdUI.m_Id=m_pAsdMenu->GetId(fld);
				cmdUI.m_pMenu=m_pAsdMenu;
				cmdUI.m_MenuIndex=fld;
				
				if (m_pAsdMenu->GetCommandObject(fld))
					m_pAsdMenu->GetCommandObject(fld)->OnUpdateCmdUI(&cmdUI);
				else if (m_pCommandWnd)
					m_pCommandWnd->OnUpdateCmdUI(&cmdUI);
			}
		}
		update();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method create a submenu or cancel the menu
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CMenuWnd::timerEvent ( QTimerEvent * e) 
{
	// TODO: Add your message handler code here and/or call default
	int nIDEvent= e->timerId();
	QRect rect;
	QPoint pos,pos1,pos2;

	// Creates a submenu
	if (nIDEvent==m_Timer2)
	{
		if ((m_CurSelect!=-1)	&& (m_pAsdMenu->GetPopupMenu(m_CurSelect)))
		{
			if(m_Timer2)
				killTimer(m_Timer2);
			m_Timer2=0;

			rect=QWidget::rect();//GetClientRect(&rect);
			pos.setX(rect.right());
			pos.setY(m_RectTable[m_CurSelect].top());
			rect = m_RectTable[m_CurSelect];
			pos1=rect.topLeft();
			pos2=rect.bottomRight();
			pos=mapToGlobal(pos);
			pos1=mapToGlobal(pos1);
			pos2=mapToGlobal(pos2);
			rect=QRect(pos1,pos2);
			m_pSubMenu = new CMenuWnd[1];
			m_pNewMenu = m_pAsdMenu->GetPopupMenu(m_CurSelect);
			m_pSubMenu->TrackPopupMenu(m_pNewMenu,m_nFlags,pos.x()+1,pos.y(),m_pCommandWnd,m_pDisplayWnd,&rect,this);
		}
	}

	// Cancel a menu if it exists
	if (nIDEvent==m_Timer1)
	{
		QPoint pnt=QCursor::pos();
		if (m_pSubMenu==NULL)
		{
			bool hasmouse=false;
			CMenuWnd* pWnd=this;

			if (!m_Rect.isEmpty())
			{
				if (m_Rect.contains(pnt))
				{
					hasmouse=true;
					pWnd=NULL;
				}
			}

			while (pWnd)
			{
				QRect rect=pWnd->frameGeometry();

				if (rect.contains(pnt))
				{
					hasmouse=true;
					break;
				}

				pWnd=pWnd->m_pMenuParent;
			}

			if (!hasmouse)
			{
				if(m_Timer1)
					killTimer(m_Timer1);
				m_Timer1=0;

				CancelWnd(cancelall);
			}
		}
		else
		{
			if(m_Timer1)
				killTimer(m_Timer1);
			m_Timer1=0;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Thîs method cancels submenus or menus according to the arg
///		parameter given
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CMenuWnd::CancelWnd(m_UserArg arg)
{
	switch (arg)
	{
	case cancelsingle:
		if (m_pSubMenu)
		{
			m_pSubMenu->CancelWnd(canceldown);
			m_pSubMenu=NULL;
		}

		if (m_pMenuParent) 
		{
			m_pMenuParent->CancelWnd(cancelpopup);
		}

		destroy();
		delete []this;
		break;
	case cancelall:
		if (m_pMenuParent)
		{
			m_pMenuParent->CancelWnd(cancelup);
			m_pMenuParent=NULL;
		}

		if (m_pSubMenu)
		{
			m_pSubMenu->CancelWnd(canceldown);
			m_pSubMenu=NULL;
		}

		if (m_pCommandWnd)
			m_pCommandWnd->OnCloseMenu();

		destroy();
		delete []this;
		break;
	case cancelpopup:
		m_pSubMenu = NULL;

		killTimer(m_Timer1);
		if(m_Timer1)
			killTimer(m_Timer1);
		m_Timer1=startTimer(m_TimeOut);

		setFocus();
		break;
	case canceldown:
		if (m_pMenuParent)
			m_pMenuParent->CancelWnd(cancelpopup);

		if (m_pSubMenu)
		{
			m_pSubMenu->CancelWnd(canceldown);
			m_pSubMenu=NULL;
		}

		destroy();
		delete []this;
		break;
	case cancelup:
		if (m_pMenuParent)
		{
			m_pMenuParent->CancelWnd(cancelup);
			m_pMenuParent = NULL;
		}

		destroy();
		delete []this;
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//
/// \class CAsdMenu
/// This class is used to associate the items of a popup menu with the 
///	functionality
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor of class CAsdMenu parameter given
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CAsdMenu::CAsdMenu()
: m_pHWnd(NULL)
{
	m_IsOpen=false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor of class CAsdMenu parameter given
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CAsdMenu::CAsdMenu(CAsdMenu& Menu)
{
	m_IsOpen=false;

	for (int i=0;i<Menu.m_ItemTable.GetSize();i++)
	{
		m_MenuItem curitem=Menu.m_ItemTable[i];

		if (curitem.pMenu)
			curitem.pMenu=new CAsdMenu(*curitem.pMenu);

		m_ItemTable.Add(curitem);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor of class CAsdMenu parameter given
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CAsdMenu::~CAsdMenu()
{
	if (m_IsOpen)
		m_pHWnd->CancelWnd(CMenuWnd::cancelall);

	for (int i=0;i<m_ItemTable.GetSize();i++)
	{
		if (m_ItemTable[i].pMenu)
			delete m_ItemTable[i].pMenu;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method deletes and creates the items on the menu parameter given
/// 
///  RETURNS: 
///		an object of class CAsdMenu
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
const CAsdMenu& CAsdMenu::operator =( const CAsdMenu& MenuSrc )
{
	if (&MenuSrc != this)
	{
		int i;
		
		for (i=0;i<m_ItemTable.GetSize();i++)
		{
			if (m_ItemTable[i].pMenu)
				delete m_ItemTable[i].pMenu;
		}
		m_ItemTable.RemoveAll();
		
		for (i=0;i<MenuSrc.m_ItemTable.GetSize();i++)
		{
			m_MenuItem curitem=MenuSrc.m_ItemTable[i];
			
			if (curitem.pMenu)
				curitem.pMenu=new CAsdMenu(*curitem.pMenu);

			m_ItemTable.Add(curitem);
		}
		return *this;
	}
	else
		return *this;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method opens a popup menu associated to a track
/// 
///  RETURNS: 
///		an integer of TrackPopupMenu method for CMenuWnd class
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CAsdMenu::TrackPopupMenu(int x,int y,CObject* pWnd,QRect* lprect,CObject* pDisplayWnd)
{
	m_LastSelectionRect=QRect();
	m_IsOpen=true;
	m_pHWnd = new CMenuWnd[1];

	return m_pHWnd->TrackPopupMenu(this,0,x,y,pWnd,pDisplayWnd,lprect,NULL);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method cancels the popup menu
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CAsdMenu::StopTracking(bool immediate)
{
	if (immediate && m_pHWnd)
		m_pHWnd->CancelWnd(CMenuWnd::cancelall);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if a menu is already opened
/// 
///  RETURNS: 
///		true if the menu is already opened otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CAsdMenu::IsMenuOpen()
{
	return m_IsOpen;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method removes all the ASD popup menus
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CAsdMenu::RemoveAll()
{
	if (m_IsOpen)
		m_pHWnd->CancelWnd(CMenuWnd::cancelall);
	
	for (int i=0;i<m_ItemTable.GetSize();i++)
	{
		if (m_ItemTable[i].pMenu)
			delete m_ItemTable[i].pMenu;
	}

	m_ItemTable.RemoveAll();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method inserts an item at a specified position in the
///		specified popup menu for auto close popups
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CAsdMenu::InsertItem(int Index,QString Text,UINT id,bool AutoClose,bool Checked,bool Disabled)
{
	m_MenuItem item;
	
	item.Text=Text;
	item.Id=id;
	item.pMenu=NULL;
	item.pCmdObject=NULL;
	item.IsSeparator=false;
	item.IsAutoClose=AutoClose;
	item.IsDisabled=Disabled;
	item.IsChecked=Checked;
	item.m_UserColor=false;
	
	if ((Index!=-1) && (Index<m_ItemTable.GetSize()))
		m_ItemTable.InsertAt(Index,item);
	else
		m_ItemTable.Add(item);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method inserts an item at a specified position in the
///		specified popup menu for non auto close popups
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CAsdMenu::InsertItem(int Index,QString Text,CAsdMenu* pMenu)
{
	m_MenuItem item;
	
	item.Text=Text;
	item.Id=0;
	item.pMenu=pMenu;
	item.pCmdObject=NULL;
	item.IsSeparator=false;
	item.IsAutoClose=false;
	item.IsDisabled=false;
	item.IsChecked=false;
	item.m_UserColor=false;
	
	if ((Index!=-1) && (Index<m_ItemTable.GetSize()))
		m_ItemTable.InsertAt(Index,item);
	else
		m_ItemTable.Add(item);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method adds an item in the popup menu 
/// 
///  RETURNS: 
///		the index of the added item
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CAsdMenu::AddItem(QString Text,UINT id,bool AutoClose,bool Checked,bool Disabled)
{
	InsertItem(-1,Text,id,AutoClose,Checked,Disabled);
	return m_ItemTable.GetUpperBound();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method adds an item in the popup menu 
/// 
///  RETURNS: 
///		the index of the added item
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CAsdMenu::AddItem(QString Text,CAsdMenu* pMenu)
{
	InsertItem(-1,Text,pMenu);
	return m_ItemTable.GetUpperBound();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method insert separators in the popup menu 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CAsdMenu::InsertSeparator(int Index)
{
	m_MenuItem item;
	
	item.Id=0;
	item.pMenu=NULL;
	item.IsSeparator=true;
	item.IsAutoClose=false;
	item.IsDisabled=false;
	item.IsChecked=false;
	item.m_UserColor=false;
	
	if ((Index!=-1) && (Index<m_ItemTable.GetSize()))
		m_ItemTable.InsertAt(Index,item);
	else
		m_ItemTable.Add(item);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the text of the popup menu 
///		according to the index specified 
/// 
///  RETURNS: 
///		the text of an item in the popup menu
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
QString CAsdMenu::GetItemText(int index)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		return m_ItemTable[index].Text;
	else
		return "";
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the item is a separator
/// 
///  RETURNS: 
///		true if the item is a separator otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool  CAsdMenu::IsItemSeparator(int index)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		return m_ItemTable[index].IsSeparator;
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the item is a checked
/// 
///  RETURNS: 
///		true if the item is checked otherwise false
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool  CAsdMenu::IsItemChecked(int index)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		return m_ItemTable[index].IsChecked;
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the item is disabled
/// 
///  RETURNS: 
///		true if the item is disabled otherwise false
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool  CAsdMenu::IsItemDisabled(int index)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
	{
		CAsdMenu* pPopupMenu = GetPopupMenu(index);
		
		if (pPopupMenu && pPopupMenu->IsAllItemDisable())
			return true;
		else
			return m_ItemTable[index].IsDisabled;
	}
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the color of the item according 
///		to the index
/// 
///  RETURNS: 
///		true if the color is retrieved otherwise false
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CAsdMenu::GetItemTextColor(int index, QColor& color)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()) && m_ItemTable[index].m_UserColor)
	{
		color = m_ItemTable[index].Color;
		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the color of the item according 
///		to the index
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CAsdMenu::SetItemTextColor(int index, QColor* pColor)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
	{
		if (pColor)
		{
			m_ItemTable[index].Color = *pColor;
			m_ItemTable[index].m_UserColor = true;
		}
		else
			m_ItemTable[index].m_UserColor = false;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the menu has the auto close behavior
/// 
///  RETURNS: 
///		true if the popup menu has the auto close behavior otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool  CAsdMenu::IsItemAutoclose(int index)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		return m_ItemTable[index].IsAutoClose;
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the command object 
///		associated of a popup menu
/// 
///  RETURNS: 
///		an object of class CObject
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CObject* CAsdMenu::GetCommandObject()
{
	if (m_pHWnd)
		return m_pHWnd->m_pCommandWnd;
	else
		return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the command object 
///		associated to the item of a the popup menu given the index as parameter
/// 
///  RETURNS: 
///		an object of class CObject
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CObject*  CAsdMenu::GetCommandObject(int index)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		return m_ItemTable[index].pCmdObject;
	else
		return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the ID of an item given the index as parameter
/// 
///  RETURNS: 
///		an integer corresponding to the ID of the item
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
UINT  CAsdMenu::GetId(int index)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		return m_ItemTable[index].Id;
	else
		return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the popup menu given the index as parameter
/// 
///  RETURNS: 
///		an item of type CAsdMenu
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CAsdMenu*  CAsdMenu::GetPopupMenu(int index)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		return m_ItemTable[index].pMenu;
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the item size of a popup menu
/// 
///  RETURNS: 
///		the number of item in a popup menu
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CAsdMenu::GetItemCount()
{
	return m_ItemTable.GetSize();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets whether the item is checked or not
///		according to the index and the boolean checked
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void  CAsdMenu::SetChecked(int index,bool Checked)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		m_ItemTable[index].IsChecked=Checked;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets whether the item is disabled or not
///		according to the index and the boolean Disabled
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void  CAsdMenu::SetDisabled(int index,bool Disabled)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		m_ItemTable[index].IsDisabled=Disabled;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the command for the associated item
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void  CAsdMenu::SetCommandObject(int index,CObject* pObject)
{
	if ((index>=0) && (index<m_ItemTable.GetSize()))
		m_ItemTable[index].pCmdObject=pObject;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the submenu for the item according to the index
/// 
///  RETURNS: 
///		the menu of type CAsdMenu
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CAsdMenu* CAsdMenu::SetPopupMenu(int index,CAsdMenu* pMenu)
{
	CAsdMenu* pOldMenu=NULL;
	
	if ((index>=0) && (index<m_ItemTable.GetSize()))
	{
		pOldMenu=m_ItemTable[index].pMenu;
		m_ItemTable[index].pMenu=pMenu;
	}

	return pOldMenu;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if all items are disabled in the menu
/// 
///  RETURNS: 
///		true if all items are disables otherwise returns false
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CAsdMenu::IsAllItemDisable()
{
	for (int fld=0;fld<GetItemCount();fld++)
	{
		if (!IsItemSeparator(fld))
		{
			CAsdMenu* pMenu = GetPopupMenu(fld);
			bool disable = false;
			
			if (pMenu)
				disable = pMenu->IsAllItemDisable();
			else
				disable = IsItemDisabled(fld);

			if (!disable)
				return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the functions associated to the item
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CAsdMenu::UpdateAllCmdUI(CObject* pCommandWnd)
{
	for (int fld=0;fld<GetItemCount();fld++)
	{
		if (GetId(fld))
		{
			CCmdUI cmdUI;
			cmdUI.m_Id=GetId(fld);
			cmdUI.m_pMenu=this;
			cmdUI.m_MenuIndex=fld;
			
			if (GetCommandObject(fld))
				GetCommandObject(fld)->OnUpdateCmdUI(&cmdUI);
			else if (pCommandWnd)
				pCommandWnd->OnUpdateCmdUI(&cmdUI);
		} 
		else if (GetPopupMenu(fld))
		{
			GetPopupMenu(fld)->UpdateAllCmdUI(pCommandWnd);
		}
	}
}


//--------------------------------< THE END >----------------------------------


