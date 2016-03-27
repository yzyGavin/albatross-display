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
//  FILENAME: WdwMngr.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CWdwMngr which manages all actions or interactions
//		on windows.  
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

#if !defined(WdwMngr_H)
#define WdwMngr_H

//-INCLUDE FILES---------------------------------------------------------------

//-GLOBAL DATA-----------------------------------------------------------------


//-CLASSES DEFINITIONS---------------------------------------------------------

namespace CWdwMngr
{
	//methods
	void InitTables();
	void AddWindow(QWidget* pWnd,QString PrioName, int attrib, bool IsCollapse);
	void RemoveWindow(QWidget* pWnd);
	bool CheckWindowInside(QRect qrWindow, int* monitor); 
	void MoveWindowInside(QWidget* w); 
	void OrderWindows(QWidget* w=NULL);
	void SwitchOrderWindowTable(int i, int priohead);
	void OnMouseMove();
	bool IsWindowCollapsed(QWidget* w);
	void CollapseUncollapseWindow(QObject* pQObj);
	int  GetZOrder(QWidget* pWnd);
	void ResetImportantCode(QWidget* pWnd);
	void ShowWindow(QWidget* pWnd, int attrib);
	void ShowCustomSetWindow(QWidget* pWnd, int attrib, bool IsCollapsed);
	void HideWindow(QWidget* pWnd);
	void DecollapseSize(QWidget* pW, int iWidgetPos);
	void CollapseSize(QWidget* pW, int iWidgetPos);
	void WindowSize(QWidget* pW);
	int  WindowId(QWidget* w);
	void SetCollapse(QWidget* w, bool isCollapsed);
	QSize GetDecollapseSize(QWidget* w);
	void SetWindowSize(QWidget* w);
	void SetWindowSize(QWidget* w, int sizeWidth, int sizeHeight);
	void SetWindowAttributes();
	void MoveWindow();
	void RecordMove(QWidget* w, QPoint p);
	void CancelMove(QWidget* w);
    void SetRecordMove(QWidget* w, QPoint p);
    void CancelRecordMove(QWidget* w);
    bool IsRecordMoved();
	void SetAttributes(QString Window, long functions);
	bool IsAttributesChanged(QString Window, long functions);
}


#endif

//--------------------------------< THE END >----------------------------------


