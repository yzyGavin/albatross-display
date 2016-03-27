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
//  FILENAME: StatusBar.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CStatusBar which allows to display and update the 
//		information displayed in the Status Bar.
//				
// 
//  COMMENTS
//
//  BUILD INFORMATION 
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
#include "StatusBar.h"
#include "View.h"
#include "DisplayElement.h"
#include "VisuMap.h"
#include "RwdView.h"
#include "ColorMngr.h"
#include "FontMngr.h"

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------
//-----------------------------------------------------------------------------
//
/// \class CStatusBar
/// This class is used to display the Independant/Dependant mode, radar and 
///	weather services used, filters and coordinates of the mouse pointer
///	on the status bar of a radar window
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the contructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CStatusBar::CStatusBar(CRwdView* pView)
    : QWidget( (QWidget*)pView,"")
{
	QRect rect=pView->rect();
	setGeometry(0,rect.bottom()-30,rect.width(),30);
	setBackgroundMode(NoBackground);
	show();
	m_pHBitmap=NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the contructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CStatusBar::CStatusBar(CStatusBar& pStatusBar)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CStatusBar::~CStatusBar()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the Independant/Dependant mode on the status
///		bar of radar windows
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CStatusBar::UpdateInd(bool Ind,bool Update)
{
	m_IsInd=Ind;
	if (Update)
		update();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the radar services used on the status
///		bar of radar windows
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CStatusBar::UpdateRadarService(QString Service,bool Unavail,bool Different, bool Update)
{
	m_RadarService=Service;
	m_RadarIsNotAvail=Unavail;
	m_RadarIsDiff=Different;
	if (Update)
		update();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the height filters on the status
///		bar of radar windows
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctRwsFltGen005, CIAsdFctRwsFltGen010
//-----------------------------------------------------------------------------
void CStatusBar::UpdateHeightFilter(int LowFilter,bool LowFilterOn,int HighFilter,bool HighFilterOn,int DepFilter,bool DepFilterOn,bool Update)
{
	m_LowFilter=LowFilter;
	m_LowFilterIsOn=LowFilterOn;
	m_HighFilter=HighFilter;
	m_HighFilterIsOn=HighFilterOn;
	m_DepFilter=DepFilter;
	m_DepFilterIsOn=DepFilterOn;
	if (Update)
		update();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates, on the status
///		bar of radar windows, the Primary, Vfr, Military, Datablock
///		filters anf the Weather service and his status 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctRwsFltGen005, CIAsdFctRwsFltGen010
//-----------------------------------------------------------------------------
void CStatusBar::UpdateFilters(bool Prim,bool Vfr,bool Mil,bool DataBlock,bool Weather,bool WeatherNotAvail,bool Update)
{
	m_PrimFilter=Prim;
	m_VfrFilter=Vfr;
	m_MilFilter=Mil;
	m_DataFilter=DataBlock;
	m_WeatherIsOn=Weather;
	m_WeatherNotAvail=WeatherNotAvail;
	if (Update)
		update();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the latitude and longitude on the status
///		bar of radar windows
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CStatusBar::UpdateCoordinates(QString Coordinates,bool Update)
{
	if (Update)
	  {
	    m_Coordinates=Coordinates;
	    update();
	  }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method resizes the status of a radar after a resizeEvent
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CStatusBar::resizeEvent ( QResizeEvent * Event)
{
	QSize size=Event->size();
	if (m_pHBitmap)
		delete m_pHBitmap;
	m_pHBitmap = new QPixmap(size);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method resizes the status of a radar after a resizeEvent
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CStatusBar::DrawHeightFilters(QPainter* dc)
{
	QString str1,str2,str3,str4,str5;
	QSize sizesep,sizeservice,sizeheight,size1,size2,size3,size4,size5;

	dc->setPen(CColorMngr::GetColor(DefWindowTextClr));
	sizeheight=dc->boundingRect(0,0,0,0,Qt::AlignLeft|Qt::AlignTop,"H(000/000/999)").size();
	sizeservice=dc->boundingRect(0,0,0,0,Qt::AlignLeft|Qt::AlignTop,"WWWWWWWWWWWWWWWWWWW").size();
	QRect rect=QRect(41+sizeservice.width(),1,sizeheight.width()+23,28);
	int posy=(20+sizeheight.height())/2;
	sizesep=dc->boundingRect(0,0,0,0,Qt::AlignLeft,"/").size();
	str1="H(";
	size1=dc->boundingRect(0,0,0,0,Qt::AlignLeft|Qt::AlignTop,str1).size();
	str2.sprintf("%.3d",m_LowFilter);
	size2=dc->boundingRect(0,0,0,0,Qt::AlignLeft,str2).size();
	str3.sprintf("%.3d",m_HighFilter);
	size3=dc->boundingRect(0,0,0,0,Qt::AlignLeft,str3).size();
	str4.sprintf(" %.3d",m_DepFilter);
	size4=dc->boundingRect(0,0,0,0,Qt::AlignLeft,str4).size();
	str5=")";
	size5=dc->boundingRect(0,0,0,0,Qt::AlignLeft,str5).size();
	dc->drawText(rect.left()+5,posy,str1);

	if (!m_DepFilterIsOn)
	{
		dc->setPen(CColorMngr::GetColor(DefMenuDisTextClr));
		dc->drawText(rect.left()+4+size1.width(),posy-1,str4);
		dc->setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
	}
	else
		dc->setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc->drawText(rect.left()+5+size1.width(),posy,str4);

	if (!m_DepFilterIsOn)
	{
		dc->setPen(CColorMngr::GetColor(DefMenuDisTextClr));
		dc->drawText(rect.left()+4+size1.width()+size4.width(),posy-1,"/");
		dc->setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
	}
	else
		dc->setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc->drawText(rect.left()+5+size1.width()+size4.width(),posy,"/");

	if (!m_LowFilterIsOn)
	{
		dc->setPen(CColorMngr::GetColor(DefMenuDisTextClr));
		dc->drawText(rect.left()+4+size1.width()+size4.width()+sizesep.width(),posy-1,str2);
		dc->setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
	}
	else
		dc->setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc->drawText(rect.left()+5+size1.width()+size4.width()+sizesep.width(),posy,str2);

	if ((!m_LowFilterIsOn) && (!m_HighFilterIsOn))
	{
		dc->setPen(CColorMngr::GetColor(DefMenuDisTextClr));
		dc->drawText(rect.left()+4+size1.width()+size4.width()+sizesep.width()+size2.width(),posy-1,"/");
		dc->setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
	}
	else
		dc->setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc->drawText(rect.left()+5+size1.width()+size4.width()+sizesep.width()+size2.width(),posy,"/");

	if (!m_HighFilterIsOn)
	{
		dc->setPen(CColorMngr::GetColor(DefMenuDisTextClr));
		dc->drawText(rect.left()+4+size1.width()+size4.width()+sizesep.width()+size2.width()+sizesep.width(),posy-1,str3);
		dc->setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
	}
	else
		dc->setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc->drawText(rect.left()+5+size1.width()+size4.width()+sizesep.width()+size2.width()+sizesep.width(),posy,str3);

	dc->setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc->drawText(rect.left()+5+size1.width()+size2.width()+sizesep.width()+size3.width()+sizesep.width()+size4.width(),posy,str5);

	Draw3dRect(dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));
	rect.addCoords(1,1,-1,-1);
	Draw3dRect(dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method draws and positions in the status of a radar the 
///		radar and weather services, the filters, the coordinates of the mouse pointer
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctRwsFltGen005, CIAsdFctRwsFltGen010
//-----------------------------------------------------------------------------
void CStatusBar::paintEvent ( QPaintEvent * )
{
        int WarningClr, posy;
	if (!m_pHBitmap)
		return;
	QPainter dc;
	QRect rect,fullrect;
	rect=QWidget::rect() ;
	
	rect.addCoords ( 0,0,1,1);

	fullrect=rect;
	dc.begin(m_pHBitmap);
	dc.fillRect(rect,QBrush(CColorMngr::GetColor(CColorMngr::GetColorNb("RW_STATUS_BAR_BKGRND"))));
	WarningClr=CColorMngr::GetColorNb("WARNING");
	int UrgencyClr=CColorMngr::GetColorNb("URGENCY");
	QString str1,str2,str3,str4,str5;
	QSize sizesep,sizeservice,sizeheight,sizefilter,sizecoord,size1,size2,size3,size4,size5;
	dc.setFont(CFontMngr::GetFontByName("STATUS_BAR"));

	//draw the rectangle for independent status in the status bar of the radar window
	rect=QRect(1,1,40,28);
	if (m_IsInd)
	  {
	    dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
		dc.drawText(rect,Qt::AlignCenter,"Ind");
	  }
	Draw3dRect(&dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));
	rect.addCoords(1,1,-1,-1);
	Draw3dRect(&dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));

	//draw the rectangle of the radar services in the status bar of a radar window
	sizeservice=dc.boundingRect(0,0,0,0,Qt::AlignLeft|Qt::AlignTop,"WWWWWWWWWWWWWWWWWWW").size();
	rect=QRect(41,1,sizeservice.width(),28);
	if (m_RadarIsNotAvail)
		dc.setPen(CColorMngr::GetColor(UrgencyClr));
	else
		dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect,Qt::AlignLeft|Qt::AlignVCenter," "+m_RadarService);
	Draw3dRect(&dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));
	rect.addCoords(1,1,-1,-1);
	Draw3dRect(&dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));

	//draw height filter in the status bar of a radar window
	DrawHeightFilters(&dc);

	//draw primary, VFR, Military, Datablock, and weather filters
	sizeheight=dc.boundingRect(0,0,0,0,Qt::AlignLeft|Qt::AlignTop,"H(000/000/999)").size();
	posy=(20+sizeheight.height())/2;
	sizefilter=dc.boundingRect(0,0,0,0,Qt::AlignLeft|Qt::AlignTop,"P/V/M/D/W").size();
	sizesep=dc.boundingRect(0,0,0,0,Qt::AlignLeft,"/").size();
	rect=QRect(41+sizeservice.width()+sizeheight.width()+23,1,sizefilter.width()+25,28);
	str1="P";
	size1=dc.boundingRect(0,0,0,0,Qt::AlignLeft,str1).size();
	str2="V";
	size2=dc.boundingRect(0,0,0,0,Qt::AlignLeft,str2).size();
	str3="M";
	size3=dc.boundingRect(0,0,0,0,Qt::AlignLeft,str3).size();
	str4="D";
	size4=dc.boundingRect(0,0,0,0,Qt::AlignLeft,str4).size();
	str5="W";
	size5=dc.boundingRect(0,0,0,0,Qt::AlignLeft,str5).size();

	// set the primary filter text and position in the satus bar of a radar window
	if (!m_PrimFilter)
	{
		dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
		dc.drawText(rect.left()+4,posy-1,str1);
		dc.setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
	}
	else
		dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect.left()+5,posy,str1);
	dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect.left()+5+size1.width(),posy,"/");

	// set the VFR filter text and position in the satus bar 
	if (!m_VfrFilter)
	{
		dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
		dc.drawText(rect.left()+4+size1.width()+sizesep.width(),posy-1,str2);
		dc.setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
	}
	else
		dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect.left()+5+size1.width()+sizesep.width(),posy,str2);
	dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect.left()+5+size1.width()+size2.width()+sizesep.width(),posy,"/");

	// set the military filter text and position in the satus bar of a radar window
	if (!m_MilFilter)
	{
		dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
		dc.drawText(rect.left()+4+size1.width()+size2.width()+2*sizesep.width(),posy-1,str3);
		dc.setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
	}
	else
		dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect.left()+5+size1.width()+size2.width()+2*sizesep.width(),posy,str3);
	dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect.left()+5+size1.width()+size2.width()+size3.width()+2*sizesep.width(),posy,"/");

	// set the datafilter filter text and position in the satus bar of a radar window
	if (!m_DataFilter)
	{
		dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
		dc.drawText(rect.left()+4+size1.width()+size2.width()+3*sizesep.width()+size3.width(),posy-1,str4);
		dc.setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
	}
	else
		dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect.left()+5+size1.width()+size2.width()+3*sizesep.width()+size3.width(),posy,str4);
	dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect.left()+5+size1.width()+size2.width()+size3.width()+size4.width()+3*sizesep.width(),posy,"/");

	// set the availablity or not of weather and position it in the satus bar of a radar window
	if (m_WeatherNotAvail)
		dc.setPen(CColorMngr::GetColor(UrgencyClr));
	else
		if (!m_WeatherIsOn)
		{
			dc.setPen(CColorMngr::GetColor(DefMenuDisTextClr));
			dc.drawText(rect.left()+4+size1.width()+size2.width()+4*sizesep.width()+size3.width()+size4.width(),posy-1,str5);
			dc.setPen(CColorMngr::GetColor(DefMenuLeftTopClr));
		}
		else
			dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	dc.drawText(rect.left()+5+size1.width()+size2.width()+4*sizesep.width()+size3.width()+size4.width(),posy,str5);
	Draw3dRect(&dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));
	rect.addCoords(1,1,-1,-1);
	Draw3dRect(&dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));

	//draw latitude/longitude coordinates in the status bar of a radar window
	dc.setPen(CColorMngr::GetColor(DefWindowTextClr));
	sizecoord=dc.boundingRect(0,0,0,0,Qt::AlignLeft|Qt::AlignTop,"99 99 99 N 999 99 99 E").size();
	rect=QRect(41+sizeservice.width()+sizeheight.width()+sizefilter.width()+48,1,sizecoord.width()+25,28);
	dc.drawText(rect.left()+5,posy,m_Coordinates);
	Draw3dRect(&dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));
	rect.addCoords(1,1,-1,-1);
	Draw3dRect(&dc,rect,CColorMngr::GetColor(DefWindowRightBottomClr),CColorMngr::GetColor(DefWindowLeftTopClr));
	dc.end();
	bitBlt(this,0,0,m_pHBitmap,0,0,fullrect.right(),fullrect.bottom(),CopyROP);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method draws the rectangle in the status bar to set the text in it
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CStatusBar::Draw3dRect(QPainter* dc,QRect rect,QColor color1,QColor color2)
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

//--------------------------------< THE END >----------------------------------

