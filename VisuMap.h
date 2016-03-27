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
//  FILENAME: VisuMap.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CMapElement and other classes which allow to 
//		manage and display map elementary elements such as: vectors, surfaces,
//		text and beacons.
//		The following attributes are defined : Color, Font, FontSize, Style,
//		the priority, Text, Info field, Beacon type and a position. 
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
#if !defined(AFX_VISUMAP_H__5D6D8300_ACC1_11D3_AC74_00C04F72F475__INCLUDED_)
#define AFX_VISUMAP_H__5D6D8300_ACC1_11D3_AC74_00C04F72F475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-INCLUDE FILES---------------------------------------------------------------

#include "View.h"
#include "AsdView.h"
#include "AsdMenu.h"
#include "RwdView.h"
#include "DisplayElement.h"
#include "./CommonFiles/GraphicSymbol.h"

//-GLOBAL DATA-----------------------------------------------------------------

#define ID_MAPS 10000
#define ID_MAPS_LAST 11000

//Generic Map element class
class CVisuMap;
class CViewVectors;
class CViewSurface;
class CViewText;
class CViewBeacon;
class CViewMap;
class ApplicationWindow;

typedef enum {Ivectors,Isurface,Itext,Ibeacon} mapelemkinds;

//-CLASSES DEFINITIONS---------------------------------------------------------

class CMapElement: public CObject
{
	friend class CVisuMap;
	friend class CMapVectors;
	friend class CMapBeacon;
	friend class CViewMap;

protected:
	
	//variables
	mapelemkinds m_Kind;
	QString m_Info;
	QColor m_Color;
	int m_Prio;
	// defines a map element
	typedef struct
	{
		// foregroud color of the map element
		QColor Color;
		// background color of the map element
		QColor Color2;
		// font
		QString Font;
		// font size
		int FontSize;
		// pen style
		UINT PenStyle;
		// brush style
		UINT BrushStyle;
		// hatch style
		UINT HatchStyle;
		// pattern of the map
		BYTE Patt[8];
		// priority of the map element
		int Prio;
		// directory of the map element usefull to construct the map menu
		QString Text;
		// information on the map element
		QString Info;
		// name of the map element
		QString Beacon;
		// table of points usefull to draw the map element
		CArray<QPoint,QPoint>  PointTable;
	} m_MapParam;

	//methods
	~CMapElement(){}
	CMapElement() 
	{
		m_pInfoWnd=NULL;
	}

private:
	//pointers
	QWidget* m_pInfoWnd;

	//methods		
    CMapElement(m_MapParam param);
	CMapElement(CMapElement& pMapElem);
		
};

class CMapVectors: public CMapElement
{
	friend class CVisuMap;
	friend class CViewVectors;

protected:

	//variables
	UINT m_PenStyle;
	BYTE m_Patt[8];
	int m_PattLen;

	//methods
	~CMapVectors();
	CArray<QPoint,QPoint> m_PointTable;	
    CMapVectors(m_MapParam& param);
	CMapVectors(){}
};

class CMapBeacon: public CMapElement
{
	friend class CVisuMap;
	friend class CViewBeacon;

protected:

	//variables
	QPoint m_Point;
	QString m_Font;
	int m_Fontsize;
	char m_Char;
	QString m_BeaconName;

	//methods
	~CMapBeacon();	
    CMapBeacon(m_MapParam& param);
	CMapBeacon(){}
};

class CMapText: public CMapElement
{
	friend class CVisuMap;
	friend class CViewText;

protected:

	//variables
	QPoint m_Point;
	QString m_Font;
	int m_FontSize;
	int m_IsCentered;
	QString m_Text;

	//methods
	~CMapText();	
    CMapText(m_MapParam& param);
	CMapText(){}
};

class CMapSurface: public CMapElement
{
	friend class CVisuMap;
	friend class CViewSurface;

protected:

	//variables
	UINT m_PenStyle;
	QColor m_Color2;
	UINT m_BrushStyle;
	UINT m_HatchStyle;
	uchar m_Patt[8];

	//methods
	~CMapSurface();
	CArray<QPoint,QPoint> m_PointTable;	
    CMapSurface(m_MapParam& param);
	CMapSurface(){}
};

class CVisuMap : public CObject
{
	friend class CViewMap;
	friend class CAsdApp;
	friend class CRwdView;
	friend class CViewRoute;

private:

	//variables

	// boolean to indicate if a map was selected or not
	bool m_WasSelected;
	// boolean to indicate if a map is selected
	bool m_IsSelectable;
	// contains the beacon usefull in the graphical route leg tool
	static QString m_GrlBeacon;	
	// contains the information on maps
	QString m_Info;
	// Id of the map menu
	UINT m_Id;
	// name of the map menu
	QString m_MapMenuName;
	// table of type CMapElement
	CArray<CMapElement*,CMapElement*> m_ElementTable;
	// name of the map
	QString m_MapName;
	// name of the map file
	QString m_FileName;
	// size of the map file
	long m_Size;
	// time of the last modified map file
	QTime m_Time;
	// table of elements of type CViewMap
	CArray<CViewMap*,CViewMap*> m_ElemTable;
	// tables of map views of type CView
	static CArray<CView*,CView*> m_ViewTable;
	// table of maps of type CVisuMap
	static CArray<CVisuMap*,CVisuMap*> m_MapTable;	
	// version of the ASD
	static QString m_Version;
	// pointer of type CAsdMenu
	static CAsdMenu* m_pMenu;

	//dynamics methods

	// constructor of CVisuMap class
	CVisuMap();
	CVisuMap(QString MapName);
	CVisuMap(CVisuMap& pVisuMap);
	// destructor of CVisuMap class
	~CVisuMap();
	// checks if the map is selected on the top level menu
	BOOL		IsSelected(CView* pView=NULL);
	// turns the map selected or not on the radar window
	void		SetSelection(BOOL selection,CView* pView=NULL);
	// gets the map menu according to the map file
	QString		GetMenuName();
	// stores for each map the name, size and time of creation or update
	void		GetAttributes(QString* &name,long &size,QTime* &mtime);
	
	// static methods
	// loads the graphical route leg maps when the route is in modification
	static void LoadGrlMap(CAsdView* view);	
	// adds the map element in the radar window
	static void AddView(CView* pView);
	// removes the map element from the radar window
	static void RemoveView(CView* pView);
	// copy the elements of the source view in the destination view
	static void CopyView(CView* pDestView,CView* pSrcView);	
	// removes the map element on the table and the associated map menu
	// if the map does not exist anymore
	static void RemoveTables();
	// updates the map menu from the radar window
	static void OnMaps(UINT nID,CView* pView=NULL);	
	// called with the default custom set to display the map on the radar window
	static void Setup(CView* pView,QString& Key,bool save);
	// retrieves information on a map element on the radar window
	static void GetInformation(CView* pView,QPoint point,QString& info);
	// retrieves a specific map according to the map menu ID parameter
	static CVisuMap* GetMap(UINT nId);
	// calls the reload map method at initialization
	static void InitTable();
	// removes the graphical route leg maps when the route is nomore in route modification
	static void RemoveGrlMap();	
	// reloads a map already displayed if this one has changed
	static void ReloadMap();
	// retrieves the map menu
	static CAsdMenu* GetMenu();

};

class CViewMapElement : public CDisplayElement
{
public:

	//virtual methods
	virtual ~CViewMapElement() {};
	virtual CMapElement* GetInformation(QPoint point,int priority,QString &info)
	{
		return NULL;
	}
	virtual void SelectElement(bool Select)
	{
		if (Select!=m_Select)
		{
			m_Select=Select;
			OnUpdate();
		}
	}

protected:

	//variables
	bool m_Select;
};


class CViewVectors : public CViewMapElement
{
public:

	//methods
	CViewVectors(CViewVectors& pViewVectors);
	CViewVectors();
	~CViewVectors();
	void OnDraw(QPainter* pDC,int Prio);
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnUpdate();
	CMapElement* GetInformation(QPoint point,int priority,QString &info);

protected:

	//variables
	CArray<QPoint,QPoint>  m_PointTable;
	QColor m_Color;

	//pointers
	CMapVectors* m_pVector;
	CAsdView* m_pView;	
};

class CViewSurface : public CViewMapElement
{
public:

	//methods
	CViewSurface();
	CViewSurface(CViewSurface& pViewSurface);
	~CViewSurface();
	void OnDraw(QPainter* pDC,int Prio);
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnUpdate();
	CMapElement* GetInformation(QPoint point,int priority,QString &info);

protected:

	//variables
	CArray<QPoint,QPoint>  m_PointTable;
	QColor m_Color;
	QColor m_Color2;

	//pointers
	CMapSurface* m_pSurface;
	CAsdView* m_pView;	
};

class CViewText : public CViewMapElement
{
public:

	//methods
	CViewText();
	CViewText(CViewText& pViewText);
	~CViewText();
	void OnDraw(QPainter* pDC,int Prio);
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnUpdate();
	CMapElement* GetInformation(QPoint point,int priority,QString &info);

protected:

	//variables
	QPoint m_Point;
	QColor m_Color;

	//pointer
	CMapText* m_pText;
	CAsdView* m_pView;
	
};

class CViewBeacon : public CViewMapElement
{
public:

	//methods
	CViewBeacon();
	CViewBeacon(CViewBeacon& pViewBeacon);
	~CViewBeacon();
	void OnDraw(QPainter* pDC,int Prio);
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	void OnUpdate();
	CMapElement* GetInformation(QPoint point,int priority,QString &info);
	void OnLButtonDown(UINT,QPoint);
	void OnRButtonDown(UINT,QPoint);

protected:

	//variables
	QPoint m_Point;
	QColor m_Color;	

	//pointer
	CMapBeacon* m_pBeacon;
	CAsdView* m_pView;
	CGraphicSymbol* m_pSymbol;
};

class CViewMap : public CViewMapElement
{
	friend class CVisuMap;

private:

	// variables
	// set the map to selected if the maps has the implicit focus
	BOOL m_Selected;

	//methods

	// constructors of CViewMap class
	CViewMap();
	CViewMap(CViewMap& pViewMap);
	// destructor of CViewMap class
	~CViewMap();
	// called when a map element has to be drawn
	void OnDraw(QPainter* pDC,int Prio);
	// creates for each map element the corresponding displayed map element
	void Init(LPVOID pData,CView* pView,CDisplayElement* pParent=NULL);
	// called when a map element has to be updated
	void OnUpdate();
	// retrieves information on a map element on the radar window
	CMapElement* GetInformation(QPoint point,int priority,QString& info);
	// gets the priority of a map in order to be selected on the radar window
	int GetPriority(QPoint point,bool Check=false);
	// selects a map element on the radar window
	void SelectElement(bool Select);

protected:

	//variables

	// stores map elements of type CViewMapElement
	CArray<CViewMapElement*,CViewMapElement*> m_ElemTable;

	//pointers
	// pointer of type CVisuMap to have have to all the maps
	CVisuMap* m_pMap;
	// pointer of type CAsdView to have access the radar windows
	CAsdView* m_pView;	
	// pointer that stores the last selected map element
	CViewMapElement* m_pLastSelectedElement;

	//methods

	// starts the modification of a route upon a Click AB on a beacon
	void OnLButtonDown(UINT,QPoint);
	// ends the modification of a route upon a Click SB on a beacon
	void OnRButtonDown(UINT,QPoint);
};

#endif // end of file

//--------------------------------< THE END >----------------------------------
