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
//  FILENAME: AsdMenu.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the radar menu, pull down menu, cascading menu, context menu
// 
//  COMMENTS
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
// 
//-----------------------------------------------------------------------------

#if !defined(ASDMENU_H_INCLUDED_)
#define ASDMENU_H_INCLUDED_


//-GLOBAL DATA-----------------------------------------------------------------
class CAsdMenu;


//-CLASS DEFINITION---------------------------------------------------------

class CMenuWnd : public QWidget
{
	friend class CAsdMenu;

public:
	virtual ~CMenuWnd();

private:

	typedef enum {no,cancelsingle,cancelall,cancelpopup,canceldown,cancelup} m_UserArg;
	int m_Timer1;
	int m_Timer2;
	int m_iVerticalTextOffset;
	CArray<QPoint,QPoint> m_PointTable;
	CArray<QRect,QRect> m_RectTable;
	CArray<QRect,QRect> m_RectSelectTable;
	int m_CurSelect;
	int m_TimeOut;
	QRect m_Rect;
	UINT m_nFlags;
	bool m_FirstMove;


	CMenuWnd* m_pMenuParent;
	CObject* m_pCommandWnd;
	CObject* m_pDisplayWnd;
	CAsdMenu* m_pAsdMenu;	
	CMenuWnd* m_pSubMenu;
	CAsdMenu* m_pNewMenu;


	CMenuWnd();
	CMenuWnd(CMenuWnd& pMenu);
	int TrackPopupMenu(CAsdMenu* pMenu,UINT nFlags,int x,int y,CObject* pWnd, CObject* pDisplayWnd=NULL,QRect* lprect = NULL,CMenuWnd* pParent=NULL);	
	bool IsMenuParent(QWidget* pWnd);
	int ComputeSelected(QPoint point);
	void CancelWnd(m_UserArg arg);
	

protected:

	void Draw3dRect(QPainter* dc,QRect rect,QColor color1,QColor color2);
	void paintEvent ( QPaintEvent * Event);
	void mouseMoveEvent ( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	void OnLButtonUp(UINT nFlags, QPoint point);
	void OnRButtonDown(UINT nFlags, QPoint point);
	void OnMButtonDown(UINT nFlags, QPoint point);
	void timerEvent ( QTimerEvent * );
};


class CAsdMenu : public CObject
{
	friend class CMenuWnd;
	friend class CRwdView;
	friend class CTopLevel;
	friend class CTrackMngr;
	friend class CTrack;
	friend class CViewTrack;
	friend class CLabel;
	friend class CVisuMap;
	friend class CCmdUI;

private:


	bool m_IsOpen;
	CMenuWnd* m_pHWnd;
	QRect m_LastSelectionRect;
	typedef struct
	{
		QString Text;
		bool IsSeparator;
		bool IsDisabled;
		bool IsChecked;
		bool IsAutoClose;
		bool m_UserColor;
		QColor Color;
		UINT Id;
		CAsdMenu* pMenu;
		CObject* pCmdObject;
	} m_MenuItem;
	CArray<m_MenuItem,m_MenuItem> m_ItemTable;
	CAsdMenu();
	CAsdMenu(CAsdMenu& pMenu);
	~CAsdMenu();
	void RemoveAll();
	const CAsdMenu& operator =( const CAsdMenu& MenuSrc );
	int TrackPopupMenu(int x,int y,CObject* pWnd,QRect* lprect = NULL,CObject* pDisplayWnd=NULL);
	void StopTracking(bool immediate = true);
	bool IsMenuOpen();
	void InsertItem(int Index,QString Text,UINT id,bool Autoclose=false,bool Checked=false,bool Disabled=false);
	void InsertItem(int Index,QString Text,CAsdMenu* pMenu);
	int AddItem(QString Text,UINT id,bool Autoclose=false,bool Checked=false,bool Disabled=false);
	int AddItem(QString Text,CAsdMenu* pMenu);
	void InsertSeparator(int Index=-1);
	QString GetItemText(int Index);
	bool IsItemSeparator(int Index);
	bool IsItemChecked(int Index);
	bool IsItemDisabled(int Index);
	bool IsItemAutoclose(int Index);
	CAsdMenu* GetPopupMenu(int Index);
	void SetText(int Index,QCString Text);
	void SetChecked(int Index,bool Checked);
	void SetDisabled(int Index,bool Checked);
	void SetItemTextColor(int index, QColor* pColor);
	bool GetItemTextColor(int index, QColor& color);
	void SetCommandObject(int index,CObject* pObject);
	CAsdMenu* SetPopupMenu(int index,CAsdMenu* pMenu);
	UINT GetId(int Index);
	int GetItemCount();
	bool IsAllItemDisable();
	void UpdateAllCmdUI(CObject* pCommandWnd);
	CObject* GetCommandObject(int index);	
	CObject* GetCommandObject();
	bool IsOpen() 
	{ 
		return m_IsOpen; 
	};
	void SetOpen(bool isopen) 
	{ 
		m_IsOpen = isopen; 
	};
	QRect getLastSelectionRect() 
	{ 
		return m_LastSelectionRect; 
	};
	void setLastSelectionRect(QRect LastSelectionRect) 
	{ 
		m_LastSelectionRect = LastSelectionRect; 
	};
	CMenuWnd* getHWnd() 
	{ 
		return m_pHWnd; 
	};
};


#endif // end of file

//--------------------------------< THE END >----------------------------------
