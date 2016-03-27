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
//  FILENAME: AsdView.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the ASD planview where every object are displayed
//		It also contains the range and center of the planview
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
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include "View.h"
#include "AsdView.h"
#include "Elements.h"
#include <math.h>
#if (defined Q_WS_X11)
#include <X11/X.h>
#include <X11/Xlib.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "qimage.h"
#include "ColorMngr.h"

//-DATA------------------------------------------------------------------------


//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CAsdView
///		This class describes the interface of all radar views.
///		It inherits fron the View class
///		
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor. It initializes:
///			- the background color of the radar window
///			- the rectangle and the bitmap of the 3 layers
///			- the range of the radar view
///			- center position of radar view
///			- etc...
/// 
///  RETURNS:
///
///  REQUIREMENT ID: CIAsdFctRwsGen025, CIAsdFctRwsAre004
//
//-----------------------------------------------------------------------------
CAsdView::CAsdView()
{
	int colornb=	CColorMngr::GetColorNb("WND_MAIN_AREA_BKGRND");
	
	QColor clr;
	if (colornb!=-1)
		clr=CColorMngr::GetColor(colornb);
	else
		clr=QColor(14,22,22);
	
	m_pBrush = 	new QBrush(clr);
	m_InKeyMode = false;
	m_pTmpBitmap = NULL;
       	m_Param.MinX = -24000;
	m_Param.MaxX = 24000;
	m_Param.MinY = -24000;
	m_Param.MaxY = 24000;
	m_Param.Range=256;
	m_Param.Centre=QPoint(0,0);
	m_Param.Precision = 32;
	m_Param.ScrollBar = true;

	
	m_RectInvalid=QRect();
	m_Bitmap=NULL;
       

	m_ptPos.setX(0);
	m_ptPos.setY(0);

	setBackgroundMode(NoBackground);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the copy constructor.
/// 
///  RETURNS:
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CAsdView::CAsdView(CAsdView& pAsdView)
{

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is destructor.
///
///  RETURNS:
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CAsdView::~CAsdView()
{
	delete m_pBrush;

	delete m_pTmpBitmap;

	delete m_Bitmap;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the content of the window must be drawn.
///		It must be overridden in the radar view 
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::OnDraw(QPainter* )
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the window parameters
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::SetParam(WindowParams* pParam)
{
	m_Param = *pParam;
	RecomputeParameters();
	InvalidateRect(NULL);	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the range
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::SetRange(int new_range)
{	
	if (m_Param.Range!=new_range)
	{
		m_Param.Range=new_range;
		RecomputeParameters();
		InvalidateRect(NULL);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the center position of radar view
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::SetCentre(QPoint new_centre)
{
	m_Param.Centre=new_centre;
	RecomputeParameters();
	InvalidateRect(NULL);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the current range
///
///  RETURNS:
///		the range
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CAsdView::GetRange()
{
	return m_Param.Range;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the center position of radar view
///
///  RETURNS:
///		the center position
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
QPoint CAsdView::GetCentre()
{
	return m_Param.Centre;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method re-computes the window parameters
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::RecomputeParameters()
{
	QRect rect=	QWidget::rect();
	
	m_Multiplicator = (double(2*m_Param.Precision)*double(m_Param.Range))/double(rect.width());
	m_DeviceOrgX=double(rect.width())/2.0;
	m_DeviceOrgY=double(rect.height())/2.0;
    m_LogicalOrgX=m_Param.Centre.x();
    m_LogicalOrgY=m_Param.Centre.y();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts device units into logical units
///		Argument1: lpPoints is a table of QPoint which must be converted
///		Argument2: nCount is the size of QPoint table
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::DPtoLP(QPoint* lpPoints, int nCount) const
{
	for (int i=0;i<nCount;i++)
	{
		lpPoints[i].setX((int)floor(((double(lpPoints[i].x())-m_DeviceOrgX)*m_Multiplicator)+m_LogicalOrgX));
		lpPoints[i].setY((int)floor(((double(lpPoints[i].y())-m_DeviceOrgY)*-m_Multiplicator)+m_LogicalOrgY));
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts device units into logical units
///		Argument: lpRect is pointer to the QRect object which must be converted
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::DPtoLP(QRect* lpRect) const
{
	DPtoLP((QPoint*)lpRect,2);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts device units into logical units
///		Argument: lpSize is pointer to the QSize object which must be converted
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::DPtoLP(QSize* lpSize) const
{
	lpSize->setWidth((int)floor(double(lpSize->width())*m_Multiplicator));
	lpSize->setHeight((int)floor(double(lpSize->height())*m_Multiplicator));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts logical units into device units
///		Argument1: lpPoints is a table of QPoint which must be converted
///		Argument2: nCount is the size of QPoint table
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::LPtoDP(QPoint* lpPoints, int nCount) const
{
	for (int i=0;i<nCount;i++)
	{
		lpPoints[i].setX((int)floor(((double(lpPoints[i].x())-m_LogicalOrgX)/m_Multiplicator)+m_DeviceOrgX));
		lpPoints[i].setY((int)floor(((double(lpPoints[i].y())-m_LogicalOrgY)/-m_Multiplicator)+m_DeviceOrgY));
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts logical units into device units
///		Argument: lpRect is pointer to the QRect object which must be converted
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::LPtoDP(QRect* lpRect) const
{
	LPtoDP((QPoint*)lpRect,2);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts logical units into device units
///		Argument: lpSize is pointer to the QSize object which must be converted
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::LPtoDP(QSize* lpSize) const
{
	lpSize->setWidth((int)floor(double(lpSize->width())/m_Multiplicator));
	lpSize->setHeight((int)floor(double(lpSize->height())/m_Multiplicator));
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the window is resized.
///		The following parameters is re-computed:
///			- the rectangle and the bitmap of the 3 layers
///			- the range of the radar view
///			- center position of radar view
///		Argument: Event is a pointer to the QResizeEvent. 
///				  It defines the new size.
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk021, CIAsdFctRwsAre010, CIAsdFctRwsAre025
//
//-----------------------------------------------------------------------------
void CAsdView::resizeEvent(QResizeEvent* Event)
{
	bool XChange=false;
	bool YChange=false;

#if (defined Q_WS_X11)
	Window root;
	int X,Y;
	unsigned int dummy;
	Window *pTmpCurTab,tmproot,tmpparent1,tmpparent2;
	UINT tmpnbr=0;
	XQueryTree(x11Display(),winId(),&tmproot,&tmpparent1,&pTmpCurTab,&tmpnbr);
	XFree(pTmpCurTab);
	XQueryTree(x11Display(),tmpparent1,&tmproot,&tmpparent2,&pTmpCurTab,&tmpnbr);
	XFree(pTmpCurTab);
	XGetGeometry(x11Display(),tmpparent2,&root,&X,&Y,&dummy,&dummy,&dummy,&dummy);

#ifdef ENABLE_QDEBUG
	qDebug("Size Event X=%d,Y=%d,QtPos X=%d,Y=%d",X,Y,m_ptPos.x(),m_ptPos.y());
#endif

	XChange=(X!=m_ptPos.x());
	YChange=(Y!=m_ptPos.y());
#endif
	
	int cx=Event->size().width();
	int oldcx=Event->oldSize().width();
	int cy=Event->size().height();
	int oldcy=Event->oldSize().height();
	
	if (m_Bitmap==NULL)
		oldcx=0;
	
	if (cx!=0)
	{
		// Recompute bitmap
			if (m_Bitmap==NULL)
			{
				m_Bitmap=new QPixmap(Event->size());
			}
			else
			{
				m_Bitmap->resize(Event->size());
			}
		
		if (m_pTmpBitmap)
			m_pTmpBitmap->resize(Event->size());
		else
			m_pTmpBitmap = new QPixmap(Event->size());
		
		if (oldcx && 
		    (Event->oldSize().width() >= this->minimumSize().width()) &&
            	    (Event->oldSize().height() >= this->minimumSize().height()) &&
	    	    (Event->oldSize().height() > 1) && (Event->size().height() > 1) &&
            	    (Event->size().width() >= this->minimumSize().width()) &&
            	    (Event->size().height() >= this->minimumSize().height()))
		{
			QPoint oldPos(0,0);
			DPtoLP(&oldPos);
			
			//compute new center
			if (XChange)
			{
				#if (defined Q_WS_X11)
				m_ptPos.setX(X);
				#endif
				m_LogicalOrgX-=double(cx-oldcx)*m_Multiplicator/2.0;
			}
			else
				m_LogicalOrgX+=double(cx-oldcx)*m_Multiplicator/2.0;
			
			if (YChange)
			{
				#if (defined Q_WS_X11)
				m_ptPos.setY(Y);
				#endif
				m_LogicalOrgY+=double(cy-oldcy)*m_Multiplicator/2.0;
			}
			else
				m_LogicalOrgY-=double(cy-oldcy)*m_Multiplicator/2.0;

			QRect rect=QWidget::rect();
			m_DeviceOrgX=double(rect.width())/2.0;
			m_DeviceOrgY=double(rect.height())/2.0;
			m_Param.Centre.setX((int)floor(m_LogicalOrgX));
			m_Param.Centre.setY((int)floor(m_LogicalOrgY));
			
			// range has changed
			if (cx!=oldcx)
				m_Param.Range=(int)(floor(m_Multiplicator*double(rect.width()))/double(2*m_Param.Precision));

			QPoint Pos(cx-oldcx,0);
			DPtoLP(&Pos);
		}
		else
			RecomputeParameters();
	}

	OnUpdate(NULL,0,NULL);
	InvalidateRect(NULL);
	
	QPoint offset(0,0);
	if (CRangeMark::GetCRangeMarkView(this)!=NULL && CNewRange::IsInCreation()==false)
	{
		// Recompute range marker
		//Bottom Left Range Marker
		if (!CRangeMarkView::GetOrientation())
		{
			//the bottom border is moved
			if (!YChange && (cy-oldcy)!=0)
			{
				offset=QPoint(0,cy-oldcy);
				CRangeMark::GetCRangeMarkView(this)->SetResizeOffset(offset);
			}
			//the top border is moved
			if (YChange)
			{
			   offset=QPoint(0,cy-oldcy);
			   CRangeMark::GetCRangeMarkView(this)->SetResizeOffset(offset);
			}
		}
		//Top Right Range Marker
		else	
		{
			//the bottom border is moved
			if (!XChange && (cx-oldcx)!=0)
			{
				offset=QPoint(cx-oldcx,0);
				CRangeMark::GetCRangeMarkView(this)->SetResizeOffset(offset);
			}
			//the left border is moved
			if (XChange)
			{
				offset=QPoint(cx-oldcx,0);
				CRangeMark::GetCRangeMarkView(this)->SetResizeOffset(offset);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the content of the window must be drawn
///		It includes the double buffering process:
///			1 - It draws the graphical elements (track, map,...) by layer
///			1 - It merges the layers to the last layer
///			2 - It copies the last layer to the radar view
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::paintEvent ( QPaintEvent * Event)
{
	QRect	rcBounds=Event->rect();

	QPainter tmp;


		if (!m_RectInvalid.isEmpty())
		{
			
			tmp.begin(m_Bitmap);
			
			tmp.fillRect(m_RectInvalid, *m_pBrush);

			OnDraw(&tmp);
			tmp.end();
			m_RectInvalid.setRect(0,0,-1,-1);
		}
	bitBlt(this, rcBounds.left(), rcBounds.top(),m_Bitmap,rcBounds.left(), rcBounds.top(),rcBounds.width(), rcBounds.height(), CopyROP);	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates a region of the radar window
///		Argument1: lpRect is a pointer to the QRect object. It defines 
///				   the region which must be updated
///		Argument2: layer defines the layer which is concerned by the update
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsdView::InvalidateRect(QRect* lpRect)
{	
	QRect rect;

	if (lpRect)
		rect = *lpRect;
	else
		rect=QWidget::rect();

	QRect rectView;
	rectView=QWidget::rect();
	rect = rect.intersect(rectView);

	m_RectInvalid|=rect;

	QWidget::update(rect);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if a region must be updated
///		Argument1: lpRect is a pointer to the QRect object. It defines 
///				   the region which must be checked
///		Argument2: layer defines the layer which is concerned by the check
///
///  RETURNS:
///		returns true if the function is successful, otherwise false 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
bool CAsdView::RectVisible(QRect* lpRect)
{	
	QRect rect=*lpRect;
	return (rect.intersects(m_RectInvalid)!=false);
}

//--------------------------------< THE END >----------------------------------

