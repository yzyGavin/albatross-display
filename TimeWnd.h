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
//  FILENAME: TimeWnd.h
//  TYPE    : header file
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
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 1  2009/08/31 dalleins
//
//
// 
//-----------------------------------------------------------------------------
#if !defined(AFX_TIMEWND_H__F0292DAF_A05E_11D4_A4C3_00C04F72F475__INCLUDED_)
#define AFX_TIMEWND_H__F0292DAF_A05E_11D4_A4C3_00C04F72F475__INCLUDED_

//-INCLUDE FILES---------------------------------------------------------------

//-GLOBAL DATA-----------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-CLASSES DEFINITIONS---------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CTimeWnd window

class CTimeWnd : public QWidget
{
	friend class CAsdApp;
	friend class CTopLevel;

public:

	// static methods
	static void SelectArea(int i,bool select);
	static QString GetAreaName(int i);
	static bool GetSelectArea(int i);
	static int AreaNb();

private:

	//variables
	QDateTime m_Time;
	QDateTime m_LastUpd;
	QFont m_Font1;
	QFont m_Font2;
	QFont m_Font3;
	QSize m_SmallSize;
	QSize m_FullSize;
	bool  m_IsFullSize;
	static CArray<QString,QString> m_StringTable;
	static CArray<int,int> m_OrderTable;
	static CArray<CTimeWnd*,CTimeWnd*> m_WndTable;

	//pointers
	QPixmap* m_pHBitmap;
	
	//dynamic methods	
	CTimeWnd();
	CTimeWnd(CTimeWnd& pTimeWnd);
	virtual ~CTimeWnd();
	void OnUpdate();	
	void paintEvent ( QPaintEvent * Event);
	void mousePressEvent ( QMouseEvent * e );
	void timerEvent ( QTimerEvent * );
	void resizeEvent ( QResizeEvent * Event);
	int OnCreate();	

	//static methods	
	static void InitTables();	
	static void Setup(int topnb,QString& Key,bool save);
	
};

#endif // end of file

//--------------------------------< THE END >----------------------------------

