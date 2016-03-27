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
//  FILENAME: LineView.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the declaration of the CLineElement and CLineView class
//    They are used by the Alert and CSU traffic list
//	  They are described behaviour and display of the list
// 
//  COMMENTS
//    x
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

#if !defined(AFX_LINEVIEW_H)
#define AFX_LINEVIEW_H


//-DATA------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define LV_BUTTONTITLE 0x1
#define LV_VERTGRID 0x2
#define LV_HORZGRID 0x4
#define LV_GRIPPER 0x8
#define LV_TITLEGRID 0x10
#define LV_TITLEARROW 0x20
#define LV_NOHEADER 0x40

#define NCHITTEST 0x100
#define LBUTTONDOWN 0x201
#define LBUTTONUP 0x202
#define RBUTTONDOWN 0x203
#define RBUTTONUP 0x204
#define MBUTTONDOWN 0x205
#define MBUTTONUP 0x206
#define NCRBUTTONDOWN 0x207
#define NCMBUTTONDOWN 0x208

#include "StdAfx.h"
#include "View.h"
#include "DisplayElement.h"

//-PROTOTYPES------------------------------------------------------------------

class CLineElement : public CDisplayElement
{

public:
	virtual ~CLineElement();
	virtual QSize GetSize(int Col);
	virtual void SetLine(int Line){}
};


class CLineView : public CView
{
  Q_OBJECT

	friend class CFplMngr;
	friend class CViewFpl;
	friend class CFplField;
	friend class CFlightPlan;

public slots:
	void OnScrollDown(int pos);


private:
	//New type definition
	typedef int (*PFNCOMPARE)(long,long,long,CView*);
	typedef int (*PFNREMOVE)(CView*);
	typedef bool (*PFNTITLEMOUSEACTION)(CLineView*,int,UINT);
	
	//pointers
	//Line element object
	CLineElement* m_pCurElem;

	//Constructor
	CLineView(); 
	//Copy Constructor
	CLineView(CLineView &LineView);           
	//This method adds a new column to the lineview
	int AddColumn(QString title,int Width);
	//This method sets the background color of the lineview
	void SetBkColor(int colornb);
	//This method adds an element to the lineview
	void AddElement(CLineElement* pElem);
	//This method removes an element to the lineview 
	bool RemoveElement(CLineElement* pElem);
	//This method allows to call a sort function
	void SetCompareFunc(PFNCOMPARE pfnCompare);
	//This method sets the title bar font 
	void SetTitleFont(QString FontName);
	//This method returns the Rect of one element in the lineview
	QRect GetDrawRect(int Line,int Col);
	//This method returns the colomn number corresponding to a position
	int GetColumnNb(int PosX);
	//This method sets the vertical margin of a lineview
	void SetVerticalMargin(int Margin);
	//This method sets the horizontal margin of a lineview
	void SetHorizontalMargin(int Margin);
	//This method sets the style of a linview
	void SetStyle(UINT Style);
	//This method returns the number of elements in the lineview
	int GetElementNb();
	//This method sorts the elements
	void SortItem(bool update=false);
	//This method iconify one column
	void IconifyView(int ColumNb=-1);
	//This method returns whether the lineview is iconified or not
	bool  IsIconified();
	//Allows to update the heigth of the scrollbar upon the change of the Font Size
	void UpdateScrollBarSize();
	//Sets the maximum number of entries to be visible 
	void SetMaxDisplay(int maxdisplay);
	//Returns the maximum number of entries to be visible
	int GetMaxDisplay();
	//Set the entry put in argument to be visible
	void MakeElementVisible(CLineElement* pElem);

	//This method is called for the initial update of the lineview
	virtual void OnInitialUpdate();
	//This method is called to update the lineview content
	virtual void OnUpdate(CView* pSender, long lHint, CObject* pHint);

protected:

	//methods
	//Destructor
	virtual ~CLineView();
	////This method draws the lineview
	virtual void OnDraw(QPainter* pDC);

	//variables
	//Current sorting order
	int m_CurSort;
	//Iconified column number 
	int m_IconifyNb;
	//current size of the lineview
	QSize m_CurSize;
	//current rect of the lineview
	QRect m_CurRect;
	//PFNCOMPARE object
	PFNCOMPARE m_pfnCompare;
	//PFNTITLEMOUSEACTION object
	PFNTITLEMOUSEACTION m_pfnAction;
	//structure used to describe a column
	typedef struct
	{
		//title of the column
		QString Title;
		//size of the color
		int		Size;
		//status of the color
		int		State;
		//element number of the column
		int		Elem;
		//button status
		int		ButtonState;
		//Color number
		int		ColorNb;
	} ColumnDesc;
	//Table of column
	CArray<ColumnDesc,ColumnDesc> m_ColumnTable;
	//Horizontal margin
	int m_HMargin;
	//Vertical margin
	int m_VMargin;
	//line height
	int m_LineHeight;
	//first line position
	int m_FirstPos;
	//last line position
	int m_LastPos;
	//Max lines displayed
	int m_MaxDisplay;
	//grid width
	int m_GridWidth;
	//Structure to define one element displayed in a line
	typedef struct {
		//Line element
		CLineElement* pElem;
		//Rect to display the element
		QRect CurRect;
		//vertical coordinate of the element
		int SizeY;
	} ElemDef;
	//Table of line elements
	CArray<ElemDef,ElemDef> m_ElemTable;
	//Table positions
	CArray<WORD,WORD> m_PosTable;
	//Background color of the lineview
	int m_BkColor;
	//Bitmap to be drawn
	QPixmap* m_pBitmap;
	//Title bar font
	QFont m_TitleFont;
	//title bar height
	int m_TitleHeight;
	//Whether the lineview is opened or not
	bool m_IsOpen;
	//Style of the lineview
	UINT m_Style;
	//timer for the implicit focus within the lineview
	int m_TimerId;
	//Scrollbar
	QScrollBar* m_pScrollBar;


	//Mouse move event slot
	void mouseMoveEvent ( QMouseEvent * e );
	//Mouse press event slot
	void mousePressEvent ( QMouseEvent * e );
	//Mouse release event slot
	void mouseReleaseEvent ( QMouseEvent * e );
	//Mouse left button down event slot
	void OnLButtonDown(UINT nFlags, QPoint point);
	//Mouse right button down event slot
	void OnRButtonDown(UINT nFlags, QPoint point);
	//Mouse middle button down event slot
	void OnMButtonDown(UINT nFlags, QPoint point);
	//Resize event slot
	void resizeEvent ( QResizeEvent * Event);
	//Paint event slot
	void paintEvent ( QPaintEvent * Event);
	//Mouse left button up event slot
	void OnLButtonUp(UINT nFlags, QPoint point);
	//Timer event slot
	void timerEvent ( QTimerEvent * );
	//Mouse right button up event slot
	void OnRButtonUp(UINT nFlags, QPoint point);
	//Mouse middle button up event slot
	void OnMButtonUp(UINT nFlags, QPoint point);
	//Close event slot
    void closeEvent( QCloseEvent * );



};


#endif  // end of file

//--------------------------------< THE END >----------------------------------


