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
//  FILENAME: TimeWnd.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CTimeWnd which manages the Time Window.
//		This includes the display of the current time, and the invocation of the
//		QNH/TL information.
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
#include "TimeWnd.h"
#include "AsdApp.h"
#include "ColorMngr.h"
#include "FontMngr.h"
#include "WdwMngr.h"
#include "InfoMngr.h"

//-GLOBAL DATA-----------------------------------------------------------------

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CArray<QString,QString> CTimeWnd::m_StringTable;
CArray<int,int> CTimeWnd::m_OrderTable;
CArray<CTimeWnd*,CTimeWnd*> CTimeWnd::m_WndTable;

const int MinHeigthTLM=30;

//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------
//-----------------------------------------------------------------------------
//
/// \class CTimeWnd
/// This class is used to create the Time Window and the QNH/TL window
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CTimeWnd::CTimeWnd():
	QWidget(0,0,WType_Dialog|WStyle_Customize  |WStyle_NoBorder)
{
	m_Time = QDateTime::currentDateTime();
	m_LastUpd = m_Time;
	m_pHBitmap = NULL;
	m_IsFullSize=false;
	OnCreate();
	m_WndTable.Add(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the copy constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CTimeWnd::CTimeWnd(CTimeWnd& pTimeWnd)
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
CTimeWnd::~CTimeWnd()
{
	CWdwMngr::RemoveWindow(this);
	if (m_pHBitmap)
		delete m_pHBitmap;
	for (int i=0;i<m_WndTable.GetSize();i++)
		if (m_WndTable[i]==this)
			m_WndTable.RemoveAt(i);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method initializes a table with the QNH/TL per Area ID
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CTimeWnd::InitTables()
{
	m_StringTable.Add("1013 - 8   OS");
	m_StringTable.Add("1013 - 12  BR");
	m_StringTable.Add("1013 - 8   BL");
	m_StringTable.Add("1013 - 8   CI");
	m_StringTable.Add("1013 - 8   SP");
	m_StringTable.Add("1013 - 8   LX");
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method draws the timer window and the QNH/TL if displayed
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctQtl005, CIAsdFctQtl006, CIAsdFctQtl007
///					CIAsdFctQtl015
//-----------------------------------------------------------------------------
void CTimeWnd::paintEvent ( QPaintEvent * )
{
	CAsdApp::GetApp()->setStartXPaint(true);
	CAsdApp::GetApp()->setLastXPaint(QDateTime::currentDateTime());
	QPainter dc; // device context for painting
	dc.begin(m_pHBitmap);
	dc.setBackgroundMode(TransparentMode);
	dc.setPen(CColorMngr::GetColor(DefMenuTextClr));
	QRect rect=QWidget::rect();
	dc.fillRect(rect,CColorMngr::GetColor(CColorMngr::GetColorNb("TOPLEVEL_MENU_BKGRND")));
	QString str1,str2,str3,str4;
	m_Time=QDateTime::currentDateTime();
	str1.sprintf("%.2d",m_Time.time().hour());
	str2.sprintf("%.2d",m_Time.time().minute());
	str3.sprintf("%.2d",m_Time.time().second());
	str4 = " : ";
	dc.setFont(m_Font1);
	QSize size1=dc.boundingRect(0,0,0,0,Qt::AlignCenter,str1).size();
	QSize size3=dc.boundingRect(0,0,0,0,Qt::AlignCenter,str3).size();
	QSize size4=dc.boundingRect(0,0,0,0,Qt::AlignCenter,str4).size();
	dc.setFont(m_Font2);
	QSize size2=dc.boundingRect(0,0,0,0,Qt::AlignCenter,str2).size();
	int posx = (m_SmallSize.width()-size1.width()-size2.width()-size3.width()-(size4.width()*2))/2;
	int posy = (m_SmallSize.height()-size1.height())/2+size1.height()-5;
	int posy2 = (m_SmallSize.height()-size2.height())/2+size2.height()-8;
	dc.setFont(m_Font1);
	dc.drawText(posx,posy,str1);
	dc.drawText(posx+size1.width(),posy,str4);
	dc.setFont(m_Font2);
	dc.drawText(posx+size1.width()+size4.width(),posy2,str2);
	dc.setFont(m_Font1);
	dc.drawText(posx+size1.width()+size4.width()+size2.width(),posy,str4);
	dc.drawText(posx+size1.width()+size2.width()+(2*size4.width()),posy,str3);
	if (m_IsFullSize)
	{
		dc.setFont(m_Font3);
		QSize size4(0,0);
		QString text="No QNH/TL";//CAsdApp::GetApp()->GetInfoMngr()->GetQnhTl();
		size4=dc.boundingRect(0,0,0,0,Qt::AlignLeft,text).size();
		if (m_SmallSize.height()<=MinHeigthTLM)
			dc.drawText((m_SmallSize.width()-size4.width())/2,MinHeigthTLM-3+size4.height(),text);
		else
			dc.drawText((m_SmallSize.width()-size4.width())/2,posy+size4.height()+5,text);
	}
	QPen pen(CColorMngr::GetColor(DefMenuTextClr));
	dc.setPen(pen);
	dc.drawLine(0,m_SmallSize.height(),m_FullSize.width()+100,m_SmallSize.height());
	dc.end();
	bitBlt(this,0,0,m_pHBitmap,0,0,rect.right(),rect.bottom(),CopyROP);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method retrieves the number of QNH/TL entries
/// 
///  RETURNS: 
///		the number of QNH/TL entries
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CTimeWnd::AreaNb()
{
	return m_StringTable.GetSize();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method retrieves the name of a QNH/TL entry area 
/// 
///  RETURNS: 
///		the name of QNH/TL entry area
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
QString CTimeWnd::GetAreaName(int i)
{
	if ((i>=0) && (i<m_StringTable.GetSize()))
	{
		return m_StringTable[i];
	}
	return "";
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method selects a QNH/TL according to the area ID
///		and adds it below the time window 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CTimeWnd::SelectArea(int i,bool select)
{
	int j;
	if ((i>=0) && (i<m_StringTable.GetSize()))
	{
		bool fnd=false;
		for (j=0;j<m_OrderTable.GetSize();j++)
		{
			if (m_OrderTable[j]==i)
			{
				if (select)
					return;
				else
				{
					m_OrderTable.RemoveAt(j);
					fnd=true;
					break;
				}				
			}
		}
		if (!fnd)
		{
			if (select)
				m_OrderTable.Add(i);
			else
				return;
		}
		for (j=0;j<m_WndTable.GetSize();j++)
		{
			m_WndTable[j]->OnUpdate();
			if (m_WndTable[j]->m_IsFullSize)
				m_WndTable[j]->resize(m_WndTable[j]->m_FullSize);
			m_WndTable[j]->update();
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks whether the area ID for the QNH/TL 
///		is already selected or not
/// 
///  RETURNS: 
///		true if the area is selected, false otherwise 
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CTimeWnd::GetSelectArea(int i)
{
	for (int j=0;j<m_OrderTable.GetSize();j++)
	{
		if (m_OrderTable[j]==i)
		{
			return true;
		}
	}
	return false;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the display of the time window every seconds
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CTimeWnd::timerEvent ( QTimerEvent * )
{
	QDateTime tmp = QDateTime::currentDateTime();
	m_Time=m_Time.addSecs(m_LastUpd.secsTo(tmp));
	m_LastUpd = tmp;
	update();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates the Time Window
/// 
///  RETURNS: 
///		0 by default
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CTimeWnd::OnCreate() 
{
	startTimer(500);
	QPainter dc(this);
	int fontnb=CFontMngr::GetFontNb("TIME_WINDOW_HS");
	m_Font1=CFontMngr::GetFont(fontnb);
	dc.setFont(m_Font1);
	QSize size1=dc.boundingRect(0,0,0,0,Qt::AlignLeft,"   99 : ").size();
	QSize size2=dc.boundingRect(0,0,0,0,Qt::AlignLeft," : 99").size();
	fontnb=CFontMngr::GetFontNb("TIME_WINDOW_M");
	m_Font2=CFontMngr::GetFont(fontnb);
	dc.setFont(m_Font2);
	QSize size3=dc.boundingRect(0,0,0,0,Qt::AlignLeft,"99").size();
	if (size3.height()<MinHeigthTLM)
		m_SmallSize=QSize(size1.width()+size2.width()+size3.width(),MinHeigthTLM);
	else
		m_SmallSize=QSize(size1.width()+size2.width()+size3.width(),size3.height());
	fontnb=CFontMngr::GetFontNb("QNHTL");
	m_Font3=CFontMngr::GetFont(fontnb);
	OnUpdate();
	resize(m_SmallSize);	
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method update the size of the Time Window adding or not 
///		the QNH/TL value if selected or not
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CTimeWnd::OnUpdate()
{
	QPainter dc(this);
	dc.setFont(m_Font3);
	QSize size4(0,0);
		
		for (int i=0;i<m_StringTable.GetSize();i++)
		{
				QSize size5=dc.boundingRect(0,0,0,0,Qt::AlignLeft,m_StringTable[i]).size();
				if (size5.width()>m_SmallSize.width())
					m_SmallSize.setWidth(size5.width());
				size4.setHeight(size5.height());
		}

	if (m_SmallSize.height()<MinHeigthTLM)
		m_FullSize=QSize(m_SmallSize.width(),MinHeigthTLM+size4.height());
	else
		m_FullSize=QSize(m_SmallSize.width(),m_SmallSize.height()+size4.height());
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the Time Window is displayed
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CTimeWnd::resizeEvent ( QResizeEvent * Event)
{
	QSize size=Event->size();
	if (m_pHBitmap)
		delete m_pHBitmap;
	m_pHBitmap = new QPixmap(size);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method toggles the Time window adding or not
///		the QNH/TL value each time the user click AB on Time Window
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CTimeWnd::mousePressEvent ( QMouseEvent * e )
{
	if (e->button()==LeftButton)
	{
		if (m_IsFullSize)
		{
			resize(m_SmallSize);
			update();
		}
		else
		{
			resize(m_FullSize);
			update();
		}
		m_IsFullSize=!m_IsFullSize;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user save or load a custom set  
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctQtl004 
//-----------------------------------------------------------------------------
void CTimeWnd::Setup(int topnb,QString& Key,bool save)
{
	QString timestr;
	int timewnd=topnb-1;
	int timesize;
	if (m_WndTable[timewnd])
	{
		if (save)
	    {
			// save the Time window with the QNH/TL value if displayed
			timestr.sprintf("%d", m_WndTable[timewnd]->m_IsFullSize);
			Key=timestr;
	    }
	    else
	    {
			// load the Time window with the QNH/TL value displayed or not
			timestr=Key;
			sscanf(timestr,"%d",&timesize);	
			m_WndTable[timewnd]->m_IsFullSize=timesize;
			if (m_WndTable[timewnd]->m_IsFullSize)
			{
				// Time Window and QNH/TL value are displayed
				m_WndTable[timewnd]->resize(m_WndTable[timewnd]->m_FullSize);
				m_WndTable[timewnd]->update();
			}
			else
			{
				// Time Window is displayed but QNH/TL value is not displyed
				m_WndTable[timewnd]->resize(m_WndTable[timewnd]->m_SmallSize);
				m_WndTable[timewnd]->update();
			}		
	     }	    
	  }
}

//--------------------------------< THE END >----------------------------------
