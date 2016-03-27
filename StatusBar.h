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
//  FILENAME: StatusBar.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CStatusBar which allows to display and update the 
//		information displayed in the Status Bar.
//				
// 
//  COMMENTS
//	  x  
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
#if !defined(STATUSBAR_H)
#define STATUSBAR_H

//-INCLUDE FILES---------------------------------------------------------------

//-GLOBAL DATA-----------------------------------------------------------------

//-CLASSES DEFINITIONS---------------------------------------------------------

class CRwdView;

class CStatusBar : public QWidget
{
	friend class CRwdView;

public:

	//methods
	CStatusBar(CRwdView* pView);
	CStatusBar(CStatusBar& pStatusBar);
	~CStatusBar();
	void UpdateInd(bool Ind,bool Update=true);
	void UpdateRadarService(QString Service,bool Unavail,bool Different, bool Update=true);
	void UpdateHeightFilter(int LowFilter,bool LowFilterOn,int HighFilter,bool HighFilterOn,int DepFilter,bool DepFilterOn,bool Update=true);
	void UpdateFilters(bool Prim,bool Vfr,bool Mil,bool DataBlock,bool Weather,bool WeatherNotAvail,bool Update=true);
	void UpdateCoordinates(QString Coordinates,bool Update=true);

private:

	//variables
	bool m_IsInd;
	QString m_RadarService;
	bool m_RadarIsNotAvail;
	bool m_RadarIsDiff;
	int  m_LowFilter;
	bool m_LowFilterIsOn;
	int  m_HighFilter;
	bool m_HighFilterIsOn;
	int  m_DepFilter;
	bool m_DepFilterIsOn;
	bool m_PrimFilter;
	bool m_VfrFilter;
	bool m_MilFilter;
	bool m_DataFilter;
	bool m_WeatherIsOn;
	bool m_WeatherNotAvail;
	QString m_Coordinates;

	//pointer
	QPixmap* m_pHBitmap;

	//methods
	void paintEvent ( QPaintEvent * Event);
	void DrawHeightFilters(QPainter* dc);
	void Draw3dRect(QPainter* dc,QRect rect,QColor color1,QColor color2);
	void resizeEvent ( QResizeEvent * Event);
};
#endif

//--------------------------------< THE END >----------------------------------
