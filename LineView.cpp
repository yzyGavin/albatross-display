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
//  FILENAME: LineView.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the declaration of the CLineElement and CLineView class
//    They are used by the Alert and CSU traffic list
//	  It consists in behavior and display behavior of the list
// 
//  COMMENTS
//
//  BUILD INFORMATION 
//
//  COMMUNICATION / SYNCHRONISATION / LOCKS
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

#include "View.h"
#include "DisplayElement.h"
#include "LineView.h"
#include "ColorMngr.h"
#include "FontMngr.h"
#include <stdlib.h>

//-DATA------------------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CAPTIONSIZE 22

//-PROTOTYPES------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CLineElement
///		This class is used to define a list line element
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CLineElement::~CLineElement()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the size of a line element
///		Argument: the column number
///
///  RETURNS:
///		returns the QSize of the element if the size has been successfully found
///		otherwise it returns an empty QSize.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
QSize CLineElement::GetSize(int Col)
{
	return QSize(0,0);
}

//-----------------------------------------------------------------------------
//
/// \class CLineView
///		This class is used to define a list view
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
//
//-----------------------------------------------------------------------------
CLineView::CLineView()
{
	m_BkColor = 0;
	m_pBitmap = NULL;
	m_CurSort = 0;
	m_pfnCompare = NULL;
	m_LineHeight = -1;
	m_FirstPos = 0;
	m_MaxDisplay = -1;
	m_TitleFont.setFamily("Courier");
	m_TitleFont.setPointSize(10);
//	m_TitleFont.CreatePointFont(100,"Courier");
	m_IsOpen = false;	
	m_HMargin = 0;
	m_VMargin = 0;
	m_pfnAction=NULL;
	m_Style = 0;
	m_pCurElem=NULL;
	m_GridWidth=0;
	m_IconifyNb=-1;
	m_CurSize=QSize(0,0);
	//m_WarningColor=-1;
	//m_Warning=false;
	m_TimerId=0;
	setBackgroundMode(NoBackground);
	m_pScrollBar = new QScrollBar(this);
	connect( m_pScrollBar, SIGNAL( valueChanged(int) ), this , SLOT( OnScrollDown(int) ) );
	m_pScrollBar->hide(); 
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the copy constructor
///
///  RETURNS:
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CLineView::CLineView(CLineView &LineView)
{
}    
       
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
///
///  RETURNS:
///
//
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CLineView::~CLineView()
{
	delete m_pBitmap;

	if (m_TimerId)
		killTimer(m_TimerId);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is a slot which manages the visibility of entries in the list
///		when the list is scrolled
///
///  RETURNS: void
///
//
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLineView::OnScrollDown(int pos)
{
	m_FirstPos=pos;
	OnUpdate(NULL,NULL,NULL);

	if (m_pCurElem)
	{
		m_pCurElem->SelectElement(false);
		m_pCurElem=NULL;
	}

}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		draws the list
///		Argument: a pointer on the QPainter object
///
///  RETURNS:
///		void		
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CLineView::OnDraw(QPainter* pDC)
{
	QRect rect,DrawRect,ArrowRect;
	int col,line,curposX = 0;
//	GetUpdateRect(&rect);
	rect=QWidget::rect();//GetClientRect(&rect);
	pDC->fillRect(rect.left(),rect.top(),rect.width(),rect.height(),CColorMngr::GetColor(m_BkColor));
	pDC->setBackgroundMode(TransparentMode);

	//
	// Draw header
	//
	if (!(m_Style & LV_NOHEADER))
	{
		pDC->setFont(m_TitleFont);
		if (m_Style & LV_GRIPPER)
		{
			DrawRect = QRect(QPoint(curposX,1),QPoint(curposX+m_GridWidth-1,m_TitleHeight));
			//		pDC->DrawFrameControl(&DrawRect,DFC_BUTTON,DFCS_BUTTONPUSH);
		}
		if (m_Style & LV_TITLEARROW)
		{
			DrawRect = QRect(curposX,1,curposX+m_GridWidth-1,m_TitleHeight);
			/*QBitmap bitmap;
					if (m_Warning)
			bitmap.LoadBitmap(IDB_TITLEDANGER);
			else
			bitmap.LoadBitmap(IDB_TITLEARROW);
			CDC memdc;
			memdc.CreateCompatibleDC(pDC);
			CBitmap* pOldBitmap = memdc.SelectObject(&bitmap);
			if ((m_Warning) && (m_WarningColor!=-1))*/
			//pDC->SetTextColor(CColorMngr::GetColor(m_WarningColor));
			/*else
			pDC->SetTextColor(CColorMngr::GetColor(m_ColumnTable[0].ColorNb));
			pDC->BitBlt(0,0,m_TitleHeight,m_TitleHeight,&memdc,0,0,SRCCOPY);
			memdc.SelectObject(pOldBitmap);*/
		}
		curposX=0;//m_GridWidth;
		for (col=0;col<m_ColumnTable.GetSize();col++)
		{
			DrawRect = QRect(curposX+m_GridWidth*(col==0),0,m_ColumnTable[col].Size,m_TitleHeight);
			if (m_Style & LV_BUTTONTITLE)
			{
				if (m_ColumnTable[col].ButtonState)
				{
					//				pDC->DrawFrameControl(&DrawRect,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_PUSHED);
					DrawRect.moveBy(1,1);
					if (abs(m_CurSort)==col+1)
					{
						ArrowRect=DrawRect;
						ArrowRect.setLeft(ArrowRect.right()-20);
						DrawRect.moveRight(-20);
						/*					if (m_CurSort>0)
						pDC->DrawFrameControl(&ArrowRect,DFC_SCROLL,DFCS_SCROLLDOWN);
						else
						pDC->DrawFrameControl(&ArrowRect,DFC_SCROLL,DFCS_SCROLLUP);*/
					}
				}
				else
				{
					//				pDC->DrawFrameControl(&DrawRect,DFC_BUTTON,DFCS_BUTTONPUSH);
					if (abs(m_CurSort)==col+1)
					{
						ArrowRect=DrawRect;
						ArrowRect.setLeft(ArrowRect.right()-20);
						DrawRect.moveRight(-20);
						/*					if (m_CurSort>0)
						pDC->DrawFrameControl(&ArrowRect,DFC_SCROLL,DFCS_SCROLLDOWN);
						else
						pDC->DrawFrameControl(&ArrowRect,DFC_SCROLL,DFCS_SCROLLUP);*/
					}
				}
				
			}
			DrawRect.addCoords(-m_HMargin,-m_VMargin,-m_HMargin,-m_VMargin);
			//		pDC->SetTextColor(RGB(255,255,255));
			pDC->setPen(CColorMngr::GetColor(m_ColumnTable[col].ColorNb));
			if (m_ColumnTable[col].Size)
				pDC->drawText(DrawRect,AlignCenter,m_ColumnTable[col].Title);//,DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);
			curposX+=m_ColumnTable[col].Size;
		}
	}

	//
	// Draw selected line
	//
	if(m_pCurElem)
	{
		int color=CColorMngr::GetColorNb("SELECTED_BKGRND");
		pDC->fillRect(m_CurRect.left(),m_CurRect.top(),m_CurRect.width(),m_CurRect.height(),CColorMngr::GetColor(color));
	}
	
	//
	// Draw all lines
	//
	for (line=m_FirstPos;line<=m_LastPos;line++)
	{
		/*for (col=0;col<m_ColumnTable.GetSize();col++)
		{
			if (m_ColumnTable[col].Size!=0)
				m_ElemTable[m_PosTable[line]].pElem->OnDraw(pDC,line,col);
		}*/
		if (m_PosTable[line]<m_ElemTable.GetSize())
			if (m_ElemTable[m_PosTable[line]].pElem)
				m_ElemTable[m_PosTable[line]].pElem->OnDraw(pDC,line,-1);
	}


	//
	// Draw Grid if any
	//
	if (m_Style & LV_VERTGRID)
	{
		QPen pen(QColor(255,255,255));
		curposX=0;//m_GridWidth;
		int posY=0;
		pDC->setPen(pen);
		if (m_Style & LV_BUTTONTITLE)
			posY=m_TitleHeight;
		for (col=0;col<m_ColumnTable.GetSize()-1;col++)
		{
			curposX+=m_ColumnTable[col].Size;
			pDC->drawLine(curposX-1,posY,curposX-1,rect.bottom());
		}
	}
	if (m_Style & LV_TITLEGRID)
	{
		QPen pen(QColor(255,255,255));
		pDC->setPen(pen);
		int posY=m_TitleHeight;
		pDC->drawLine(0,posY,rect.right(),posY);
	}
	if (m_Style & LV_HORZGRID)
	{
		QPen pen(QColor(255,255,255));
		int posY=m_TitleHeight;
		pDC->setPen(pen);
		for (line=m_FirstPos;line<m_LastPos;line++)
		{
			posY+=m_ElemTable[m_PosTable[line]].SizeY;
			pDC->drawLine(0,posY,rect.right(),posY);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the rectangle of a line graphical element
///		Argument1: line number
///		Argument2: column number
///
///  RETURNS:
///		returns the QRect of the element if the rectangle has been successfully found
///		otherwise it returns an empty QRect.
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
QRect CLineView::GetDrawRect(int Line,int Col)
{
	QRect rect;
	if (Line==-1)
		rect=QRect(0,0,0,m_TitleHeight);
	else
		rect=m_ElemTable[m_PosTable[Line]].CurRect;
	if(Col==-1)
		return rect;
	int curposx=0;//m_GridWidth;
	for (int colnb=0;colnb<m_ColumnTable.GetSize();colnb++)
	{
		if (colnb==Col)
		{
			rect.setLeft(curposx);
			rect.setRight(curposx+m_ColumnTable[colnb].Size);
			rect.addCoords(m_HMargin,m_VMargin,-m_HMargin,-m_VMargin);
			break;
		}
		curposx+=m_ColumnTable[colnb].Size;
	}
	return rect;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets column number according to the X cursor position
///		Argument: the X position
///
///  RETURNS:
///		returns the column number if it has been successfully found,
///		otherwise it returns -1
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CLineView::GetColumnNb(int PosX)
{
	int curposx=0;//m_GridWidth;
	for (int colnb=0;colnb<m_ColumnTable.GetSize();colnb++)
	{
		if ((PosX>=curposx) && (PosX<=curposx+m_ColumnTable[colnb].Size))
		{
			return colnb;
			break;
		}
		curposx+=m_ColumnTable[colnb].Size;
	}
	return -1;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the list size
///
///  RETURNS:
///		returns the list size
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CLineView::GetElementNb()
{
	return m_ElemTable.GetSize();
}

/*CLineElement* CLineView::GetLineElement(int pos)
{
	if (pos<m_ElemTable.GetSize())
		return m_ElemTable[pos].pElem;
	return NULL;
}*/

/*void CLineView::SetType(int type)
{
	m_Type=type;
}

int CLineView::GetType()
{
	return m_Type;
}*/

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method is called when the window is resizing
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::resizeEvent ( QResizeEvent * Event)
{
	if (m_pBitmap)
		delete m_pBitmap;

	m_pBitmap = new QPixmap(Event->size());

	update();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method allows to add a new line element to the list
///		Argument: the new line element
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::AddElement(CLineElement* pElem)
{
	ElemDef NewDef;
	NewDef.pElem = pElem;
	NewDef.CurRect=QRect();
	NewDef.SizeY=0;
	m_ElemTable.Add(NewDef);

	SortItem();

	if (m_IsOpen)
	{
		if (m_pCurElem)
		{
			m_pCurElem->SelectElement(false);
			m_pCurElem=NULL;
		}
		OnUpdate(NULL,0,NULL);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method allows to keep the current element in a list at the same position
///		Argument: the new line element
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::MakeElementVisible(CLineElement* pElem)
{
	int oldpos=m_FirstPos;
	int CurPos=0;
	for (int elem=0;elem<m_ElemTable.GetSize();elem++)
	{
		if (pElem==m_ElemTable[elem].pElem)
		{
			for (int i=0;i<m_PosTable.GetSize();i++)
			{
				if (m_PosTable[i]==elem)
				{
					CurPos=i;
					break;
				}
			}
			if (CurPos<m_FirstPos)
				m_FirstPos=CurPos;
			else
			{
				int NbrOfDisplay = m_LastPos-m_FirstPos+1;
				if (CurPos>=m_FirstPos+NbrOfDisplay)
					m_FirstPos=CurPos-NbrOfDisplay+1;
			}
			if ((m_IsOpen) && (oldpos!=m_FirstPos))
			{
				OnUpdate(NULL,NULL,NULL);
				m_pScrollBar->setValue(m_FirstPos);
			}
			break;
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method allows to remove a line element from the list
///		Argument: the line element that must be removed
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CLineView::RemoveElement(CLineElement* pElem)
{
	for (int elem=0;elem<m_ElemTable.GetSize();elem++)
	{
		if (pElem==m_ElemTable[elem].pElem)
		{
			m_ElemTable.RemoveAt(elem);
			
			if (m_pCurElem==pElem)
				m_pCurElem=NULL;

			
			SortItem();
			
			if (m_IsOpen)
			{
				if (m_pCurElem)
				{
					m_pCurElem->SelectElement(false);
					m_pCurElem=NULL;
				}
				OnUpdate(NULL,0,NULL);
			}
			
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method allows to update the list according to the following argument
///		Argument1: this argument is not used and must be equal to NULL
///		Argument2: this argument is not used and must be equal to 0 or NULL
///		Argument3: the line element that must be updated.
///				   If this argument is equal to NULL, every line elements are updated
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctAlwGen005, CIAsdFctAlwGen010
//
//-----------------------------------------------------------------------------
void CLineView::OnUpdate(CView* pSender, long lHint, CObject* pHint) 
{
	CLineElement*  pElem = (CLineElement*)pHint;
	int CurElem=-1;
	int CurPos=0;
	int sizeY = 0,sizeX=0,CurPosY=0,line,col;
	bool RecomputeAll = false;

	//
	// Check if just one element must be updated
	//
	if (pHint)
	{
		// Search for the line element
		for (int pos=0;pos<m_ElemTable.GetSize();pos++)
		{
			if (m_ElemTable[pos].pElem==pElem)
			{
				CurElem = pos;
				break;
			}

		}

		// Check if this line element is included in an invisible column
		for (int col=0;col<m_ColumnTable.GetSize();col++)
		{
			QSize size=pElem->GetSize(col);

			if (size.height()+2*m_VMargin>sizeY)
				sizeY=size.height()+2*m_VMargin;

			if ((m_ColumnTable[col].State) && (m_ColumnTable[col].Elem==CurElem))
			{
				RecomputeAll=true;
				break;
			}
		}
		
		// Check if one element of the line is higher than the selected element
		if (!RecomputeAll)
		{
			if ((m_LineHeight==-1) && (sizeY!=m_ElemTable[CurElem].SizeY))
				RecomputeAll=true;
		}

		// Search the line position according to the selected element
		for (int i=0;i<m_PosTable.GetSize();i++)
		{
			if (m_PosTable[i]==CurElem)
			{
				CurPos=i;
				break;
			}
		}

		// Sort the list
		if (m_ElemTable.GetSize())
		{
			if (m_PosTable.GetSize())
			{
				if ((CurPos!=0))
				{
					if (m_ElemTable.GetSize()>m_PosTable[CurPos+1] && m_ElemTable.GetSize()>m_PosTable[CurPos] && m_ElemTable.GetSize()>m_PosTable[CurPos-1])
					{
						if ((m_pfnCompare) && (((CurPos!=0) && (!m_pfnCompare((long)m_ElemTable[m_PosTable[CurPos]].pElem,(long)m_ElemTable[m_PosTable[CurPos-1]].pElem,m_CurSort,this))) ||
									   ((CurPos!=m_PosTable.GetSize()-1) && (!m_pfnCompare((long)m_ElemTable[m_PosTable[CurPos+1]].pElem,(long)m_ElemTable[m_PosTable[CurPos]].pElem,m_CurSort,this)))))
						{
							SortItem();
							RecomputeAll = true;
						}
					}
				}
			}
		}

		if (!RecomputeAll)
		{
			update(m_ElemTable[CurElem].CurRect);
		}
	}
	else
		RecomputeAll = true;

	if (RecomputeAll)
	{
		int NbrOfDisplay;

		// Set value to the m_FirstPos and m_LastPos
		if (m_MaxDisplay==-1)
		{
			NbrOfDisplay = m_PosTable.GetSize();
			m_FirstPos=0;
			m_LastPos=NbrOfDisplay-1;
		}
		else
		{
			if (m_PosTable.GetSize()>m_MaxDisplay)
			{
				NbrOfDisplay=m_MaxDisplay;
			}
			else
				NbrOfDisplay = m_PosTable.GetSize();

			m_LastPos=m_FirstPos+NbrOfDisplay-1;

			if (m_LastPos>=m_PosTable.GetSize())
			{
				m_LastPos=m_PosTable.GetSize()-1;
				m_FirstPos=m_LastPos-NbrOfDisplay+1;
			}
		}

		// Init rectangle
		for (line=0;line<m_FirstPos;line++)
			m_ElemTable[m_PosTable[line]].CurRect=QRect();

		// Init invisible column size
		for (col=0;col<m_ColumnTable.GetSize();col++)
			if (m_ColumnTable[col].State)
				m_ColumnTable[col].Size=0;

		// Set the line element height according to the font
		for (line=m_FirstPos;line<=m_LastPos;line++)
		{
			sizeY=m_TitleHeight;
			for (int col=0;col<m_ColumnTable.GetSize();col++)
			{ 
				if (m_PosTable[line]<m_ElemTable.GetSize()) 
				{
					pElem = m_ElemTable[m_PosTable[line]].pElem;
					QSize size=pElem->GetSize(col);
				
					if (size.height()+2*m_VMargin>sizeY)
						sizeY=size.height()+2*m_VMargin;
				
					if ((m_ColumnTable[col].State) && (m_ColumnTable[col].Size<size.width()+2*m_HMargin))
					{
						m_ColumnTable[col].Size=size.width()+2*m_HMargin;
						m_ColumnTable[col].Elem=m_PosTable[line];
					}
				}
			}
			if (m_LineHeight!=-1)
				sizeY=m_LineHeight;
			if (m_PosTable[line]<m_ElemTable.GetSize())
				m_ElemTable[m_PosTable[line]].SizeY=sizeY;
		}

		// Set the list width
		for (col=0;col<m_ColumnTable.GetSize();col++)
			sizeX+=m_ColumnTable[col].Size;
		
		// Set the list height
		// Set the line element rectangle
		CurPosY=m_TitleHeight;		
		for (line=m_FirstPos;line<=m_LastPos;line++)
		{
			if (m_PosTable[line]<m_ElemTable.GetSize())
			{
				m_ElemTable[m_PosTable[line]].CurRect = QRect(QPoint(0,CurPosY),QPoint(sizeX,CurPosY+m_ElemTable[m_PosTable[line]].SizeY));
				CurPosY+=m_ElemTable[m_PosTable[line]].SizeY;
			}
		}
		
		for (line=m_LastPos+1;line<m_PosTable.GetSize();line++)
		{
			if (m_PosTable[line]<m_ElemTable.GetSize())
				m_ElemTable[m_PosTable[line]].CurRect=QRect();//.SetRectEmpty();
		}

		if ((m_MaxDisplay<=0) || (m_MaxDisplay>=m_PosTable.GetSize()))
		{
			if (m_pScrollBar->isVisible())
				m_pScrollBar->hide();
		}
		else
		{
			if (!m_pScrollBar->isVisible())
			{
				m_pScrollBar->setMinValue(0);
				m_pScrollBar->setMaxValue(m_PosTable.GetSize()-m_MaxDisplay+1);

				QRect geometry = QRect(m_ElemTable[m_PosTable[0]].CurRect.width(),m_ElemTable[m_PosTable[0]].CurRect.height(),18,20*m_ElemTable[m_PosTable[0]].SizeY);
				if (geometry.x() != 0 && geometry.y() != 0)
				{
					m_pScrollBar->setGeometry(geometry);
					m_pScrollBar->show();
				}
			}
			else
			{
				if (m_pScrollBar->maxValue() != m_PosTable.GetSize())
					m_pScrollBar->setMaxValue(m_PosTable.GetSize()-m_MaxDisplay+1);

				if (m_pScrollBar->x() == 0 && m_pScrollBar->y() == 0)
				{
					QRect geometry = QRect(m_ElemTable[m_PosTable[0]].CurRect.width(),m_ElemTable[m_PosTable[0]].CurRect.height(),18,20*m_ElemTable[m_PosTable[0]].SizeY);
					m_pScrollBar->setGeometry(geometry);
					m_pScrollBar->show();
				}

			}
		}

		update();
		QRect rect=QWidget::rect();
		
		if ((m_MaxDisplay!=-1) && (m_PosTable.GetSize()>m_MaxDisplay))
			sizeX+=18;//GetSystemMetrics(SM_CXVSCROLL)+2;
		
		// Resize the list if needed
		if ((m_IconifyNb==-1) && ((rect.width()!=sizeX) || (rect.height()!=CurPosY+6)))
		{
			resize(sizeX,CurPosY);
//			CWdwMngr::OrderWindows();
		}

		m_CurSize=(QSize(sizeX,CurPosY));
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method updates the size of the scrollbar when displayed if the size
///     of the font applied in the FPL Lists is modified
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::UpdateScrollBarSize()
{
	if (m_pScrollBar->isVisible())
	{
		QRect geometry = QRect(m_ElemTable[m_PosTable[m_FirstPos]].CurRect.width(),m_ElemTable[m_PosTable[m_FirstPos]].CurRect.height(),18,20*m_ElemTable[m_PosTable[m_FirstPos]].SizeY);
		m_pScrollBar->setGeometry(geometry);
		m_pScrollBar->show();
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method sorts the list
///		Argument: a boolean that update the list
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::SortItem(bool update)
{
	m_PosTable.RemoveAll();
	for (int pos=0;pos<m_ElemTable.GetSize();pos++)
	{
		m_PosTable.Add(pos);
	}
	if (m_pfnCompare)
	{
		int i,j,a;
		
		for (i=0; i<m_PosTable.GetSize();i++)
			for (j=i+1; j<m_PosTable.GetSize();j++)
			{

				if (m_pfnCompare((long)m_ElemTable[m_PosTable[i]].pElem,(long)m_ElemTable[m_PosTable[j]].pElem,m_CurSort,this))
					{
						a= m_PosTable[i];
						m_PosTable[i]=m_PosTable[j];
						m_PosTable[j]=a;
					}
				}
	
	}
	if (m_ElemTable.GetSize())
	{
		for (int line=m_FirstPos;line<=m_LastPos;line++)
		{
			if (m_ElemTable.GetSize()> m_PosTable[line])
				m_ElemTable[m_PosTable[line]].pElem->SetLine(m_PosTable[line]);
		}
	}


	if ((m_IsOpen) && (update))
		OnUpdate(NULL,0,NULL);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method allows to iconify or de-iconify the list view
///		Argument: the column number sets the window width
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::IconifyView(int ColumnNb)
{
	int width=0;//m_GridWidth;
	if (ColumnNb!=m_IconifyNb)
	{
		m_IconifyNb=ColumnNb;

		if (ColumnNb==-1)
		{
			resize(m_CurSize);
		}
		else
		{
			
			for (int col=0;col<ColumnNb;col++)
			{
				width+=m_ColumnTable[col].Size;
			}
			resize(width-1,m_TitleHeight);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method gets the iconify state
///
///  RETURNS: 
///		returns true if the window is iconify, otherwise it returns false.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CLineView::IsIconified()
{
	return (m_IconifyNb!=-1);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method allows to add column to the list
///		Argument1: column title
///		Argument2: column width
///
///  RETURNS: 
///		returns true if the column is add, otherwise it returns false.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CLineView::AddColumn(QString title,int Width)
{
	ColumnDesc curdesc;
	curdesc.Title=title;
	curdesc.State=(Width==-1);
	curdesc.ButtonState=false;
	curdesc.ColorNb=DefWindowTextClr;
	curdesc.Elem=-1;

	if (Width==-1)
		curdesc.Size=0;
	else
		curdesc.Size=Width;

	int pos=m_ColumnTable.Add(curdesc);
	if (m_IsOpen)
		OnUpdate(NULL,0,NULL);

	return pos;		
}

/*void CLineView::SetLineHeight(int Height)
{
	m_LineHeight = Height;
	if (m_IsOpen)
		OnUpdate(NULL,NULL,NULL);
}

int CLineView::GetLineHeight()
{
	return m_LineHeight;
}

void CLineView::SetColumnSize(int Col,int Width)
{
	m_ColumnTable[Col].State=(Width==-1);
	if (!m_ColumnTable[Col].State)
		m_ColumnTable[Col].Size=Width;
	else
		m_ColumnTable[Col].Size=0;
	if (m_IsOpen)
		OnUpdate(NULL,NULL,NULL);
}

int CLineView::GetColumnSize(int Col)
{
	return m_ColumnTable[Col].Size;

}*/

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method sets the title font
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::SetTitleFont(QString FontName)
{
/*	m_TitleFont.DeleteObject();
	int fontnb=CFontMngr::GetFontNb(FontName);
	LOGFONT lf = CFontMngr::GetLogFont(fontnb);*/
	m_TitleFont=CFontMngr::GetFontByName(FontName);
	if (m_IsOpen)
	{
		QPainter dc(this);
		dc.setFont(m_TitleFont);
		QRect tmp=dc.boundingRect(0,0,0,0,Qt::AlignLeft,"W");
		QSize Size = QSize(tmp.width(),tmp.height());
		m_TitleHeight = Size.height()+m_VMargin*2;
		if (m_Style&LV_TITLEARROW)
			m_GridWidth=m_TitleHeight;
		if (m_Style & LV_NOHEADER)
			m_TitleHeight=0;
		OnUpdate(NULL,0,NULL);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method initialize the content and presentation of the window widgets
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::OnInitialUpdate() 
{
	setMouseTracking(true);
	m_IsOpen = true;
	QPainter dc(this);
	dc.setFont(m_TitleFont);
	QRect tmp=dc.boundingRect(0,0,0,0,Qt::AlignLeft,"W");
	QSize Size = QSize(tmp.width(),tmp.height());
	m_TitleHeight = Size.height()+m_VMargin*2;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method sets the horizontal margin
///		Argument: horizontal margin
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::SetHorizontalMargin(int Margin)
{
	m_HMargin = Margin;
	if (m_IsOpen)
		OnUpdate(NULL,0,NULL);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method sets the vertical margin
///		Argument: vertical margin
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::SetVerticalMargin(int Margin)
{
	m_VMargin = Margin;
	if (m_IsOpen)
	{
		QPainter dc(this);
		dc.setFont(m_TitleFont);
		QRect tmp=dc.boundingRect(0,0,0,0,Qt::AlignLeft,"W");
		QSize Size = QSize(tmp.width(),tmp.height());
		m_TitleHeight = Size.height()+m_VMargin*2;
		OnUpdate(NULL,0,NULL);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method sets the background color of the list
///		Argument: color ID that must be defined in the ColorMngr
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::SetBkColor(int ColorNb)
{
	m_BkColor = ColorNb;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method sets the style of the window widgets
///		Argument: style window widgets
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::SetStyle(UINT Style)
{
	m_Style = Style;

	if (m_Style & LV_TITLEARROW)
		m_GridWidth=m_TitleHeight;
	
	update();
}

void CLineView::SetMaxDisplay(int maxdisplay)
{
	m_MaxDisplay = maxdisplay;
/*	if (m_MaxDisplay!=-1)
		ModifyStyle(0,WS_VSCROLL);
	else
		ModifyStyle(WS_VSCROLL,0);*/
	OnUpdate(NULL,NULL,NULL);
}

int CLineView::GetMaxDisplay()
{
	return m_MaxDisplay;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///     This method allows to call a sort function
///		Argument: function compare
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::SetCompareFunc(PFNCOMPARE pfnCompare)
{
	m_pfnCompare = pfnCompare;
	SortItem();
	if (m_IsOpen)
		OnUpdate(NULL,0,NULL);

}

/*int CLineView::AddRemoveProc(PFNREMOVE pfnRemove)
{
	m_RemoveTable.Add(pfnRemove);
	return 0;
}*/

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///    This method is called to draw the list
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::paintEvent ( QPaintEvent * Event)
{
	QRect UpdRect=Event->rect();
	QPainter dc; // device context for painting

	dc.begin(m_pBitmap);
	OnDraw(&dc);
	dc.end();

	// Double buffering
	bitBlt(this,UpdRect.left(),UpdRect.top(),m_pBitmap,UpdRect.left(),UpdRect.top(),UpdRect.width(),UpdRect.height(),CopyROP);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///    This method is called according to mouse motion
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::mouseMoveEvent ( QMouseEvent * e )
{
	QPoint point = e->pos();
	QRect rect=QWidget::rect();

	// Check if the cursor position is inside the window
	if ((!hasMouse()) || (!rect.contains(point)))
	{
		if (m_pCurElem)
			m_pCurElem->SelectElement(false);
		m_pCurElem=NULL;

		//releaseMouse();
		if (m_pfnAction)
			m_pfnAction(this,-1,NCHITTEST);
		return;
	}
	/*else
		if (mouseGrabber()!=this)
			grabMouse();*/

	CLineElement* pNewElem = NULL;

	// Find the line element which is selected
	int posy = m_TitleHeight;
	if (point.y()>m_TitleHeight)
	{
		for (int line=m_FirstPos;line<=m_LastPos;line++)
		{
			if ((point.y()>posy) && (point.y()<posy+m_ElemTable[m_PosTable[line]].SizeY))
			{
				pNewElem =m_ElemTable[m_PosTable[line]].pElem;
				m_CurRect=m_ElemTable[m_PosTable[line]].CurRect;
				break;
			}
			posy+=m_ElemTable[m_PosTable[line]].SizeY;
		}
	}

	if ((m_pCurElem) && (m_pCurElem!=pNewElem))
		m_pCurElem->SelectElement(false);

	if ((pNewElem) /*&& (m_pCurElem!=pNewElem)*/)
	{
		//pNewElem->SelectElement(true);
//		SetTimer(1009,100,NULL);
		if(m_TimerId)
			killTimer(m_TimerId);

		m_TimerId=startTimer(100);
	}
	m_pCurElem=pNewElem;


}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user clicks mouse LB	
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::OnLButtonDown(UINT nFlags, QPoint point) 
{
	QPoint pnt = point;
	int posx=0,ColNb;
	QRect rect;

	if (pnt.y()<m_TitleHeight)
	{
		if (m_Style & LV_BUTTONTITLE)
		{
			for (int col=0;col<m_ColumnTable.GetSize();col++)
			{
				if ((pnt.x()>posx) && (pnt.x()<posx+m_ColumnTable[col].Size))
				{
					m_ColumnTable[col].ButtonState = true;
					rect = QRect(QPoint(posx,0),QPoint(posx+m_ColumnTable[col].Size,m_TitleHeight));
					update(rect);
					break;
				}
				posx+=m_ColumnTable[col].Size;
			}
		}
		else
		{
			ColNb=GetColumnNb(pnt.x());
			if (m_pfnAction)
			{
				m_pfnAction(this,ColNb,LBUTTONDOWN);
			}
		}
	}

	if (m_pCurElem)
		m_pCurElem->OnLButtonDown(nFlags,point);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user release mouse LB 
///		in order to sort the list according to the selected column 
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::OnLButtonUp(UINT nFlags, QPoint point) 
{
	QRect rect;
	int posx=0,ColNb;

	if (m_Style & LV_BUTTONTITLE)
	{
		for (int col=0;col<m_ColumnTable.GetSize();col++)
		{
			if (m_ColumnTable[col].ButtonState)
			{
				m_ColumnTable[col].ButtonState=false;
				rect = QRect(QPoint(posx,0),QPoint(posx+m_ColumnTable[col].Size,m_TitleHeight));
				update(rect);

				if (abs(m_CurSort)==col+1)
					m_CurSort = -m_CurSort;
				else
					m_CurSort = col+1;

				SortItem();

				if (m_IsOpen)
					OnUpdate(NULL,0,NULL);
			}
			posx+=m_ColumnTable[col].Size;
		}
	}
	else
	{
		ColNb=GetColumnNb(point.x());
		if (m_pfnAction)
		{
			m_pfnAction(this,ColNb,LBUTTONUP);
		}
	}

	if (m_pCurElem)
		m_pCurElem->OnLButtonUp(nFlags,point);
	
}

/*void CLineView::OnDestroy() 
{
	for (int rm=0;rm<m_RemoveTable.GetSize();rm++)
	{
		m_RemoveTable[rm](this);
	}
	CView::OnDestroy();
	
	
}*/

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is used to selected a line element
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::timerEvent ( QTimerEvent * event)
{
	QRect rect;
	QPoint point;
	if (event->timerId()==m_TimerId)
	{
		if (m_pCurElem)
		{
			rect=QWidget::frameGeometry();
			point = QCursor::pos();
			if (!rect.contains(point))
			{
				m_pCurElem->SelectElement(false);
				m_pCurElem=NULL;
				killTimer(m_TimerId);
				m_TimerId=0;
			}
			else
			{
				m_pCurElem->SelectElement(true);
				killTimer(m_TimerId);
				m_TimerId=0;
			}
		}
		else
		{
			killTimer(m_TimerId);
			m_TimerId=0;
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user release mouse RB
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::OnRButtonUp(UINT nFlags, QPoint point) 
{
	if (m_pCurElem)
		m_pCurElem->OnRButtonUp(nFlags,point);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user click mouse RB
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::OnRButtonDown(UINT nFlags, QPoint point) 
{
	if (point.y()<m_TitleHeight)
	{
		int ColNb=GetColumnNb(point.x());
		if (m_pfnAction)
			m_pfnAction(this,ColNb,RBUTTONDOWN);
	}
	if (m_pCurElem)
		m_pCurElem->OnRButtonDown(nFlags,point);
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user release mouse MB
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::OnMButtonUp(UINT nFlags, QPoint point) 
{
	if (m_pCurElem)
		m_pCurElem->OnMButtonUp(nFlags,point);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user click mouse MB
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::OnMButtonDown(UINT nFlags, QPoint point) 
{
	if (point.y()<m_TitleHeight)
	{
		int ColNb=GetColumnNb(point.x());
		if (m_pfnAction)
			m_pfnAction(this,ColNb,MBUTTONDOWN);
	}
		if (m_pCurElem)
			m_pCurElem->OnMButtonDown(nFlags,point);
}

/*void CLineView::OnNcMButtonDown(UINT nFlags, CPoint point) 
{
	CView::OnNcMButtonDown(nFlags, point);
	CRect rect;
	GetClientRect(&rect);
	int sizex = rect.Width(),ColNb;
	if (m_Style & LV_GRIPPER)
		sizex=m_GridWidth;
	CRect DrawRect(0,0,sizex,m_TitleHeight);
	CPoint pnt = point;
	ScreenToClient(&pnt);
	if (DrawRect.PtInRect(pnt))
	{
		if (m_Style & LV_GRIPPER)
			GetParent()->ShowWindow(SW_HIDE);
		else
			if (m_pfnAction)
			{
				ColNb=GetColumnNb(pnt.x);
				m_pfnAction(this,ColNb,WM_NCMBUTTONDOWN);
			}
	}
}
void CLineView::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	CView::OnNcRButtonDown(nHitTest, point);
	CRect rect;
	GetClientRect(&rect);
	CRect DrawRect(0,0,rect.Width(),m_TitleHeight);
	CPoint pnt = point;
	ScreenToClient(&pnt);
	if (DrawRect.PtInRect(pnt))
	{
		if (m_pfnAction)
		{
			int ColNb=GetColumnNb(pnt.x);
			m_pfnAction(this,ColNb,WM_NCRBUTTONDOWN);
		}
	}
}

void CLineView::OnSysKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{

}

UINT CLineView::OnNcHitTest( CPoint point )
{
	CRect rect;
	GetClientRect(&rect);
	int sizex = rect.Width();
	if ((m_Style & LV_GRIPPER) || (m_Style & LV_TITLEARROW))
		sizex=m_GridWidth;
	if (m_Style & LV_BUTTONTITLE)
		sizex=0;
	CRect DrawRect(0,0,sizex,m_TitleHeight);
	CPoint pnt(point);
	ScreenToClient(&pnt);
	int col;
	if (DrawRect.PtInRect(pnt))
	{
		if (m_pfnAction)
		{
			col=GetColumnNb(pnt.x);
			if (m_pfnAction(this,col,WM_NCHITTEST))
				return HTCLIENT;
			else
				return HTTRANSPARENT;
		}
		else
		{
			return HTTRANSPARENT;
		}
	}
	return CView::OnNcHitTest(point);
}*/




/*void CLineView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{	
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
	if (!pScrollBar)
	{
		int lastpos = m_FirstPos;
		int NbrOfDisplay = m_LastPos-m_FirstPos+1;
		switch(nSBCode)
		{
		case SB_TOP:
			m_FirstPos = 0;
			break;
		case SB_BOTTOM:
			m_FirstPos=m_PosTable.GetSize()-NbrOfDisplay;
			break;
		case SB_PAGEUP:
			m_FirstPos-=NbrOfDisplay;
			break;
		case SB_PAGEDOWN:
			m_FirstPos+=NbrOfDisplay;
			break;
		case SB_LINEUP:
			m_FirstPos-=1;
			break;
		case SB_LINEDOWN:
			m_FirstPos+=1;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			m_FirstPos = nPos;
			break;
		}
		if (m_FirstPos<0)
			m_FirstPos=0;
		if (m_FirstPos+NbrOfDisplay>m_PosTable.GetSize())
			m_FirstPos=m_PosTable.GetSize()-NbrOfDisplay;
		if (lastpos!=m_FirstPos)
			OnUpdate(NULL,NULL,NULL);
	}
}*/

/*void CLineView::SetColumnTitle(int Col,QString title)
{
	if (Col<m_ColumnTable.GetSize())
	{
		m_ColumnTable[Col].Title=title;
		update();
	}
}*/

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the color of the column
///		Argument1: column number
///		Argument2: color ID
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
/*void CLineView::SetColumnColor(int Col,int ColorNb)
{
	if (Col<m_ColumnTable.GetSize())
	{
		m_ColumnTable[Col].ColorNb=ColorNb;
		update();
	}
}*/

/*void CLineView::SetWarningColor(int ColorNb)
{
	m_WarningColor=ColorNb;
}

void CLineView::SetWarning(bool Warning)
{
	if (m_Warning!=Warning)
	{
		m_Warning = Warning;
		update();
	}
}*/

/*void CLineView::SetTitleProc(PFNTITLEMOUSEACTION pfnAction)
{
	m_pfnAction=pfnAction;
}*/


/*UINT CLineView::OnExitSizeMove(WPARAM wParam,LPARAM lParam)
{
	//check iconfy
	CRect rect;
	GetParent()->GetWindowRect(&rect);
	int width=0;
	if (m_IconifyNb==-1)
	{
		if ((rect.Width()!=m_CurSize.cx) || (rect.Height()!=m_CurSize.cy))
			GetParent()->SetWindowPos(NULL,0,0,m_CurSize.cx,m_CurSize.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE|SWP_DRAWFRAME|SWP_FRAMECHANGED);
	}
	else
	{
		
		for (int col=0;col<m_IconifyNb;col++)
		{
			width+=m_ColumnTable[col].Size;
		}
		if (rect.Width()!=width-1)
			GetParent()->SetWindowPos(NULL,0,0,width-1,m_TitleHeight,SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE|SWP_DRAWFRAME|SWP_FRAMECHANGED);
	}
	return Default();
}*/

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
void CLineView::mousePressEvent ( QMouseEvent * e )
{
	ButtonState btn=e->button();
	switch (btn)
	{
	case LeftButton:
		OnLButtonDown(0,e->pos());
		break;
	case MidButton:
		OnMButtonDown(0,e->pos());
		break;
	case RightButton:
		OnRButtonDown(0,e->pos());
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the mouse Release event from the user in order
///		to called the associated method
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::mouseReleaseEvent ( QMouseEvent * e )
{
	ButtonState btn=e->button();
	switch (btn)
	{
	case LeftButton:
		OnLButtonUp(0,e->pos());
		break;
	case MidButton:
		OnMButtonUp(0,e->pos());
		break;
	case RightButton:
		OnRButtonUp(0,e->pos());
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method hide the window on reception of a closeEvent
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CLineView::closeEvent( QCloseEvent * )
{
	CWdwMngr::HideWindow(this);
}

//--------------------------------< THE END >----------------------------------
