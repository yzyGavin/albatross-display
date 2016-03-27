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
//  FILENAME: FontMngr.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the management of fonts in the ASD
// 
//  COMMENTS
//
//  BUILD INFORMATION 
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
#include "FontMngr.h"
#include "./CommonFiles/IniFile.h"
#include "AsdApp.h"

//-GLOBAL DATA-----------------------------------------------------------------

typedef struct
{
	QString Name;
	QFont  Font;
} FontData;
CArray<FontData,FontData> m_FontTable;

//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CFontMngr
///		This class manages the fonts of the ASD.
///		
//
//-----------------------------------------------------------------------------

 
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the font table. The fonts are defined 
///		offline in the version.ini file
/// 
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFontMngr::InitTables()
{
	QString AllKey,Key,FontKey,fontname;
	FontData curdata;
	int len,ln,fnd,fontsize;
	CIniFile inifile(CAsdApp::GetApp()->GetApplicationIni());
	len = inifile.GetIniProfileSection("FONTS",AllKey);
	QFont font;
	bool Italic,Underline,Bold;
	
	for (int i=0;i<len;i++)
	{
		if ((AllKey[i]==0) || (i==len-1))
		{
			Italic=Underline=Bold=false;
			ln = inifile.GetIniProfileString("FONTS",Key,FontKey);
			fnd=FontKey.find("|",0);
			fontname=FontKey.left(fnd);
			FontKey=FontKey.right(FontKey.length()-fnd-1);
			fnd=FontKey.find('|');
			if (fnd!=-1)
			{
				sscanf(FontKey.left(fnd),"%d",&fontsize);
				FontKey=FontKey.right(FontKey.length()-fnd-1);
				Italic = (FontKey.find('I')!=-1);
				Bold = (FontKey.find('B')!=-1);
				Underline = (FontKey.find('U')!=-1);
			}
			else
				sscanf(FontKey,"%d",&fontsize);
			font.setFamily(fontname);
			font.setPointSize(fontsize);
			font.setBold(Bold);
			font.setItalic(Italic);
			font.setUnderline(Underline);
			curdata.Name=Key;
			curdata.Font=font;
			m_FontTable.Add(curdata);
			Key="";
		}
		else
			Key+=AllKey[i];
	}

	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method empties the font table
/// 
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CFontMngr::RemoveTables()
{
	m_FontTable.RemoveAll();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns the font position according to the font name
///		from the font table.
///		Argument: Name is the font name
/// 
///  RETURNS:
///		returns the font position if the function is successful, otherwise -1.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CFontMngr::GetFontNb(QString Name)
{
	for (int i=0;i<m_FontTable.GetSize();i++)
	{
		if (m_FontTable[i].Name.compare(Name)==0)
			return i;
	}
	return -1;
}

	
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns the font according to the font position put in argument
///		Argument: fontnb is the font position
/// 
///  RETURNS:
///		returns a QFont object according to the font position 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
QFont CFontMngr::GetFont(int fontnb)
{
	ASSERT(fontnb<m_FontTable.GetSize());
	return m_FontTable[fontnb].Font;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns the font according to the font name
///		Argument: Name is the font name
/// 
///  RETURNS:
///		returns a QFont object according to the font name 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
QFont CFontMngr::GetFontByName(QString Name)
{
	for (int i=0;i<m_FontTable.GetSize();i++)
	{
		if (m_FontTable[i].Name.compare(Name)==0)
			return m_FontTable[i].Font;
	}
	return m_FontTable[0].Font;
}

//--------------------------------< THE END >----------------------------------
