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
//  FILENAME: DisplayElement.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file is inhirited by other classes and is used 
//		to interact with the elements of the planview
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

#if !defined(AFX_DISPLAY_ELEMENT_H)
#define AFX_DISPLAY_ELEMENT_H

//-INCLUDE FILES---------------------------------------------------------------

#include "AsdView.h"

//-GLOBAL DATA-----------------------------------------------------------------

typedef enum {NoViewType,MapView,TrackView,VectorView,MeteoView,NewRangeView};

typedef struct {
	QColor Color;
	bool   IsFlashing;
} FullColor;


#define PRIO_MAP_MAX			 32
#define PRIO_VECTOR				 50
#define PRIO_TRKMILES			 50
#define PRIO_RANGEMARK			 50

#define PRIO_TRACK_HALO			100
#define PRIO_TRACK_ROUTE		101
#define PRIO_TRACK_SYMBOL		102
#define PRIO_TRACK_LABEL_GLOBAL 103
#define PRIO_TRACK_LABEL		103
#define PRIO_TRACK_LABEL_ELEM_2	104
#define PRIO_TRACK_LABEL_ELEM_1	105

#define PRIO_RANGEMARK_MOVE		110
#define PRIO_TRACK_LABEL_MOVING 110


//-CLASS DEFINITION---------------------------------------------------------

/* CDS BREAK JPM 2006/05/19 Multiple inheritance with QObject which is not an abstract class.
       The alternative solution would be to inherit CObject from QObject
       But the implementation of QT does not allow this design. */
class CDisplayElement: public CObject, public QObject
{

public:

	//variables
	int m_Type;
	QRect m_BoundingRect;

public:
	// destructor
	virtual ~CDisplayElement() {};
	// called to initialize an element on a planview
	virtual void Init(LPVOID pData,CView* pView,CDisplayElement*  pParent= NULL){}
	// draws an element on a planview
	virtual void OnDraw(QPainter* pDC,int Prio) {}
	// draws an element on a planview
	virtual void OnDraw(QPainter* pDC,int Line,int Col) {}
	// updates an element on a planview
	virtual void OnUpdate() {}
	// called when the mouse move on an element of the planview
	virtual void OnMouseMove(UINT id,QPoint pnt){}
	// called when the user clicks AB on an element of the planview
	virtual void OnLButtonDown(UINT id,QPoint pnt){}
	// called when the user released the AB button from an element of the planview
	virtual void OnLButtonUp(UINT id,QPoint pnt){}
	// called when the user clicks WB on an element of the planview
	virtual void OnMButtonDown(UINT id,QPoint pnt){}
	// called when the user released the WB button from an element of the planview
	virtual void OnMButtonUp(UINT id,QPoint pnt){}
	// called when the user clicks SB on an element of the planview
	virtual void OnRButtonDown(UINT id,QPoint pnt){}
	// called when the user released the SB button from an element of the planview
	virtual void OnRButtonUp(UINT id,QPoint pnt){}
	// called when the user interact with a key of the keyboard
	virtual void OnKeyDown(){}
	// called when the user releases the key from the keyboard
	virtual void OnKeyUp(int ){}
	// select an element of the planview
	virtual void SelectElement(bool Select){}
	// calculates the priority of an element on the planview
	virtual int  GetPriority(QPoint pnt,bool Check=false)
		{
			return 0;
		}
	// check if an important track is lying under a window
	virtual int IsImportant()
		{
			return 0;
		}
	// gets the symbol for an element of the planview
	virtual void GetSymbRect(QRect&){}
};


#endif  // end of file

//--------------------------------< THE END >----------------------------------
