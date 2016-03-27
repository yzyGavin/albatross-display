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
//  FILENAME: ColorMngr.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file manages the color of the ASD
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

//-GLOBAL DATA-----------------------------------------------------------------
#if !defined(ColorMngr_H)
#define ColorMngr_H

#define MasterCategory -1
#define AOCSymbCategory 0
#define AOCDbCategory 1
#define NonAOCSymbCategory 2
#define NonAOCDbCategory 3
#define MouseCategory 4
#define WeatherCategory 5
#define OtherMapCategory 6
#define LineCategory 7
#define NavCategory 8
#define RangeMarkersCategory 9
#define AsdToolsCategory 10
#define WindowsCategory 11
#define DFFinderCategory 12

#define DefWindowBackClr 0
#define DefWindowTextClr 1
#define DefWindowLeftTopClr 2
#define DefWindowRightBottomClr 3
#define DefWindowTitleClr 4
#define DefMenuBackClr 5
#define DefMenuTextClr 6
#define DefMenuLeftTopClr 7
#define DefMenuRightBottomClr 8
#define DefMenuHighBackClr 9
#define DefMenuHighTextClr 10
#define DefMenuDisTextClr 11
#define LastFixedClr 11

class CIniFile;

//-CLASS DEFINITION---------------------------------------------------------
namespace CColorMngr
{
	void InitTables();
	void RemoveTables();
	int GetColorNb(QString Name);
	QColor GetColor(int colornb);
	QColor TransformColor(QColor color,int Cat);
	QColor TransformColorNb(int ColorNb,int Cat);
	void SetBrightness(int Category,int val);
	int  GetBrightness(int Category);
	void Setup(CIniFile* pFile,QString SetupName,bool Save);
};
#endif  // end of file

//--------------------------------< THE END >----------------------------------

