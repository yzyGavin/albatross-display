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
//  FILENAME: TopLevel.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CTopLevel which manages the Top Level Menu Bar.
//		This includes, the pull down menus, the TLM Lock feature and the alert 
//		indications for safety nets and ASD System Modes.
//				
// 
//  COMMENTS
//		There is one declared instance of CTopLevel per screen, but only one 
//		instance of window invocable from the TLM for the application. 
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
#if !defined(AFX_TOPLEVEL_H__5AA47197_AEC2_41F6_B2C0_79BED1ABC94A__INCLUDED_)
#define AFX_TOPLEVEL_H__5AA47197_AEC2_41F6_B2C0_79BED1ABC94A__INCLUDED_

//-INCLUDE FILES---------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonDataTypes.h"
#include "Brightness.h"
#include "StdAfx.h"
#include "AsdMenu.h"
#include "SystemInfo.h"
#include "SaveAs.h"
#include "CustomSetDialog.h"
#include "FindTrackDlg.h" 


//-GLOBAL DATA-----------------------------------------------------------------

//-CLASSES DEFINITIONS---------------------------------------------------------


class CTopLevelEventStub : public CLogStub
{
public :
	CTopLevelEventStub() {};
	~CTopLevelEventStub() {};
};


// CDS BREAK NGT 2006/05/23 Multiple inheritance with QObject which is not an abstract class.
//							The alternative solution would be to inherit CObject from QWidget
//							But the implementation of QT does not allow this design.  
class CTopLevel : virtual public QWidget, virtual public CObject
{
    Q_OBJECT

public:
	//static parameters
	static CSSaveAs* m_psSaveAs;
	static CColorDlg* m_psColor;
	static CFindTrackDlg* m_psFindTrack;
	static CustomizationSetDialog* m_psCustomDlg;
	static CSystemInfo* m_psSystemInfo;
	static CArray<CTopLevel*,CTopLevel*> m_sWndTable;
	//methods
	CTopLevel(CTopLevelEventStub* pHEvent , QRect qrScreenSize=QRect());
	CTopLevel(CTopLevel* pTopLev);
	virtual ~CTopLevel();
	void SetTrackSourceMenu(CAsdMenu* pMenu);
	void SetAutoHide(bool Auto);
	void SetOffsetX(int offset);
	void SetHeight(int height);
	void OnCommand(UINT id, CObject *pDisplayWnd=NULL);
	void OnUpdateCmdUI(CCmdUI* pCmdUI);
	void OnCloseMenu();
	void Setup(CIniFile* pFile,QString Section,bool Save);
	void ShowWidget(QWidget* w, QString ewName, int attrib);
	//static methods
	static void HideAllCustomSetWindow();
	static void DisplayCustomSetWindow(int screen);
	static void SetCheckBoxPalette(QPalette pal);
	static void RemoveAllMenu();
	static void UpdateSystemInfo();
	static CColorDlg* GetColorDlg();
	static CFindTrackDlg* GetFindTrackDlg();
	static CustomizationSetDialog* GetCustomizationSetDialog();
	static CSystemInfo* GetSystemInfo();
private:
	//structure definition
	typedef struct
	{
		QRect Rect;
		QString Text;
		bool IsPushed;
		bool IsDisabled;
	} m_TopItem;
	//parameters
	QString m_SysModelabel;
	CAsdMenu m_Menu;
	CAsdMenu m_AsdMenu;
	CArray<m_TopItem,m_TopItem> m_TopTable;
	int m_FirstPos;
	int m_CurSel;
	QCheckBox m_Btn;
	bool m_AutoHide;
	bool m_PrevHideStatus;
	int m_PosY;
	int m_TimerId;
	int m_iVerticalTextOffset;
	QRect m_MenuRect;
	QPoint m_qpColorDefPos;
	//methods
	void ComputeAlarm();
	void TrackMenu(UINT pos,BOOL bPressEvent);
	void Draw3dRect(QPainter* dc,QRect rect,QColor color1,QColor color2);
protected:
	//events
	void paintEvent ( QPaintEvent * Event);
	void mousePressEvent ( QMouseEvent * e );
	void timerEvent ( QTimerEvent * );
	void mouseMoveEvent ( QMouseEvent * e );
private slots:
	//slots
	void OnAutoHide(bool on);
	CTopLevelEventStub* m_pHEvent;
};

//{{AFX_INSERT_LOCATION}}

#endif // end of file

//--------------------------------< THE END >----------------------------------







