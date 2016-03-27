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
//  FILENAME: AsdView.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the ASD planview where every object are displayed
//		It also contains the range and center of the planview
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


#if !defined(ASDVIEW_H_INCLUDED_)
#define ASDVIEW_H_INCLUDED_


//-GLOBAL DATA-----------------------------------------------------------------

class CAsdView;
class CTrkMiles;

typedef struct {
	int		Precision;
	int		MinX;
	int		MaxX;
	int		MinY;
	int		MaxY;
	QPoint	Centre;
	int		Range;
	bool	ScrollBar;
} WindowParams;

//typedef enum {underlay,primary,overlay} layers;


//-CLASS DEFINITION---------------------------------------------------------

class CAsdView : public CView
{
friend class CRangeMarkView;
public :
	CAsdView(CAsdView& pAsdView);
protected :
	CAsdView();           // protected constructor used by dynamic creation
private:
	QBrush* m_pBrush;
	bool m_InKeyMode;	
	QPixmap*    m_pTmpBitmap;
	QPixmap*    m_Bitmap;
	QRect m_RectInvalid;
      	WindowParams m_Param;
	void    RecomputeParameters();
	double m_Multiplicator;
	double m_DeviceOrgX;
	double m_DeviceOrgY;
	double m_LogicalOrgX;
	double m_LogicalOrgY;
	QPoint m_ptPos;
public:
	void	SetParam(WindowParams* pParam);
	void    SetRange(int new_range);
	void    SetCentre(QPoint new_centre);
	int		GetRange();
	QPoint  GetCentre();
	void InvalidateRect(QRect* lpRect);
	bool RectVisible( QRect* lpRect);
	virtual void OnDraw(QPainter* pDC);      // overridden to draw this view
	// Operations
	void DPtoLP(QPoint* lpPoints, int nCount = 1) const;
	void DPtoLP(QRect* lpRect) const;
	void DPtoLP(QSize* lpSize) const;
	void LPtoDP(QPoint* lpPoints, int nCount = 1) const;
	void LPtoDP(QRect* lpRect) const;
	void LPtoDP(QSize* lpSize) const;

public:
protected:
	//	virtual void OnDraw(QPainter* pDC);      // overridden to draw this view
	
	// Implementation
protected:
	virtual ~CAsdView();
	
	// Generated message map functions
protected:
	void resizeEvent ( QResizeEvent * Event);
	void paintEvent ( QPaintEvent * Event);
};

#endif // end of file 

//--------------------------------< THE END >----------------------------------


