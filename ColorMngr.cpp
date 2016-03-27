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

//-INCLUDE FILES---------------------------------------------------------------
#include "StdAfx.h"
#include "ColorMngr.h"
#include "./CommonFiles/IniFile.h"
#include "stdlib.h"
#include "AsdApp.h"
#include "View.h"
#include "DisplayElement.h"
#include "VisuMap.h"
#include "RwdView.h"

//-GLOBAL DATA-----------------------------------------------------------------
int m_CategoryTable[14]={100,100,100,100,100,100,100,100,100,100,100,100,100,100};
int m_MasterCategory=100;
	typedef struct
	{
		QColor color;
		int GroupNb;
		QString Name;
	} ColorData;
	CArray<ColorData,ColorData> m_ColorTable;

int PercentColor=8;


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CColorMngr
///		This class manages the color of the ASD.
///		
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the color table. The colors are defined
///		in the source code and in the version.ini file (offline defined)
/// 
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CColorMngr::InitTables()
{
	LPCSTR ColorName[]={"WND_DEFAULT_BKGRND","WND_DEFAULT_FRGRND","WND_DEFAULT_HIGHLIGHT","WND_DEFAULT_SHADOW","COLOR_TITLE","MENU_DEFAULT_BKGRND","MENU_DEFAULT_FRGRND","MENU_DEFAULT_HIGHLIGHT","MENU_DEFAULT_SHADOW","SEL_MENU_DEFAULT_BKGRND","SEL_MENU_DEFAULT_FRGRND","GRAYED_OUT"};
	QColor ColorId[]={QColor(255,255,255),QColor(0,0,0),QColor(255,255,255),QColor(128,128,128),QColor(10,36,106),QColor(212,208,200),QColor(0,0,0),QColor(255,255,255),QColor(128,128,128),QColor(10,36,106),QColor(255,255,255),QColor(128,128,128)};
	ColorData curdata;
	curdata.GroupNb=WindowsCategory;

	int i,j;
	
	//
	// Load pre-defined colors from the source file
	//
	for (i=0;i<=LastFixedClr;i++)
	{
		curdata.Name=ColorName[i];
		curdata.color=ColorId[i];
		m_ColorTable.SetAtGrow(i,curdata);
	}

	//
	// Load colors from the version.ini file (offline definde)
	//
	QString AllKey,Key,ColorKey;
	int len,ln,red1,green1,blue1;
	CIniFile inifile(CAsdApp::GetApp()->GetApplicationIni());
	len = inifile.GetIniProfileSection("COLORS",AllKey);
	
	for (i=0;i<len;i++)
	{
		if ((AllKey[i]==0) || (i==len-1))
		{
			ln = inifile.GetIniProfileString("COLORS",Key,ColorKey);
			sscanf(ColorKey,"%d|%d,%d,%d",&curdata.GroupNb,&red1,&green1,&blue1);
			curdata.color=QColor(red1,green1,blue1);
			curdata.Name=Key.upper();

			for (j=0;j<=LastFixedClr;j++)
			{
				if (curdata.Name.compare(ColorName[j])==0)
				{
					m_ColorTable.SetAt(j,curdata);
					break;
				}
			}

			if (j==LastFixedClr+1)
				m_ColorTable.Add(curdata);

			Key="";
		}
		else
			Key+=AllKey[i];
	}
	
	ln = inifile.GetIniProfileString("GENERAL","COLORPERCENT",ColorKey);
	if (ln)
	{
		int val=atoi(ColorKey.ascii());
		if (val)
			PercentColor=val;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns the color position according to the color name
///		from the color table.
///		Argument: Name is the color name
/// 
///  RETURNS:
///		returns the color position if the function is successful, otherwise -1.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CColorMngr::GetColorNb(QString Name)
{
	QString nam=Name.upper();
	
	for (int c=0;c<m_ColorTable.GetSize();c++)
	{
		if (m_ColorTable[c].Name.compare(nam)==0)
			return c;
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns the color according to the color position
///		Argument: ColorNb is the color position
/// 
///  RETURNS:
///		returns a QColor object according to the color position 
///		if the function is successful, otherwise default white color 
///		(QColor (255,255,255))
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
QColor CColorMngr::GetColor(int ColorNb)
{
	if ((ColorNb>=0) && (ColorNb<m_ColorTable.GetSize()))
	{
		return TransformColor(m_ColorTable[ColorNb].color,m_ColorTable[ColorNb].GroupNb);
	}

	return QColor(255,255,255);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method computes the color argument according to the 
///		brightness category argument.
///		Argument1: color is the color
///		Argument2: Cat is the category
/// 
///  RETURNS:
///		returns a QColor object
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
QColor CColorMngr::TransformColor(QColor color,int Cat)
{

	int red,green,blue,offset;
	red = color.red();
	green = color.green();
	blue = color.blue();
	
	if (Cat==-1)
		offset=100;
	else
		offset= m_CategoryTable[Cat];

	red = (red*offset)/100;
	green = (green*offset)/100;
	blue = (blue*offset)/100;
	
	if(red>255)
		red=255;
	
	if (blue>255)
		blue=255;
	
	if (green>255)
		green=255;
	
	if(red<0)
		red=0;
	
	if (blue<0)
		blue=0;
	
	if (green<0)
		green=0;
	
	return QColor(red,green,blue);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method computes the color according to the color position
///		and the brightness category arguments.
///		Argument1: color is the color
///		Argument2: Cat is the category
/// 
///  RETURNS:
///		returns a QColor object
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
QColor CColorMngr::TransformColorNb(int ColorNb,int Cat)
{
	if ((ColorNb>=0) && (ColorNb<m_ColorTable.GetSize()))
	{
		return TransformColor(m_ColorTable[ColorNb].color,Cat);
	}

	return QColor(255,255,255);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the brightness according to the category argument
///		Argument1: Category is the category
///		Argument2: Value is the brightness value
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CColorMngr::SetBrightness(int Category,int Value)
{
	if (Category==MasterCategory)
		m_MasterCategory=Value;
	else
	{
		m_CategoryTable[Category]=Value;
		
		if (Category==WindowsCategory)
			CAsdApp::GetApp()->SetColors();

		if (Category==MouseCategory)
			CAsdApp::GetApp()->SetCursorColors();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the brightness according to the category argument
///		Argument: Category
/// 
///  RETURNS:
///		returns the brightness value of the category
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CColorMngr::GetBrightness(int Category)
{
	if (Category==MasterCategory)
		return m_MasterCategory;
	else
		return m_CategoryTable[Category];
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method saves or loads the brightness categories according to the Group, 
///		SubGroup, Location and Role.
///		Argument1: pFile is a pointer to the CIniFile object. 
///				   It points on a custom set file.
///		Argument2: SetupName defines the setup name of the custom set.
///		Argument3: Save is equal to true if the setup must be saved, or is equal
///				   to false if the setup must be loaded.
/// 
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CColorMngr::Setup(CIniFile* pFile,QString SetupName,bool Save)
{
	QString AllKey;
	
	if (Save)
	{// Save 
		AllKey.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",m_MasterCategory,m_CategoryTable[0],m_CategoryTable[1],m_CategoryTable[2],m_CategoryTable[3],m_CategoryTable[4],m_CategoryTable[5],m_CategoryTable[6],m_CategoryTable[7],m_CategoryTable[8],m_CategoryTable[9],m_CategoryTable[10],m_CategoryTable[11],m_CategoryTable[12]);
		pFile->WriteIniProfileString(SetupName,"COLORS",AllKey);
	}
	else
	{//load
		pFile->GetIniProfileString(SetupName,"COLORS",AllKey);
		if (AllKey.length())
		{
			sscanf(AllKey,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&m_MasterCategory,&m_CategoryTable[0],&m_CategoryTable[1],&m_CategoryTable[2],&m_CategoryTable[3],&m_CategoryTable[4],&m_CategoryTable[5],&m_CategoryTable[6],&m_CategoryTable[7],&m_CategoryTable[8],&m_CategoryTable[9],&m_CategoryTable[10],&m_CategoryTable[11],&m_CategoryTable[12]);
			
			if ((m_MasterCategory<20) || (m_MasterCategory>120))
				m_MasterCategory=100;
			
			for (int i=0;i<=12;i++)
			{
				if ((m_CategoryTable[i]<20) || (m_CategoryTable[i]>120))
					m_CategoryTable[i]=100;
			}

			CRwdView::UpdateAllViews();
			CAsdApp::GetApp()->SetColors();
			CAsdApp::GetApp()->SetCursorColors();
		}
	}
}

//--------------------------------< THE END >----------------------------------
