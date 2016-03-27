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
//  FILENAME: DataMngr.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file initiates the offline data in the ASD
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


//-INCLUDE FILES---------------------------------------------------------------
#include "StdAfx.h"
#include "DataMngr.h"
#include "ColorMngr.h"
#include "./CommonFiles/IniFile.h"
#include "String.h"
#include "AsdApp.h"
#include <stdlib.h>
#include "WinHints.h"


//-GLOBAL DATA-----------------------------------------------------------------

//company computation
CArray<QString,QString> m_CompTable;
CArray<QString,QString> m_CompShortTable;
int FPos[26],LPos[26];

typedef struct {
	QString Ades;
	QString Rwy;
	QPointArray* pPointTable;
	int Orientation;
	int ColorNb;
} RwyDef;

CArray<RwyDef,RwyDef> RwyTable;

CArray<QString,QString> m_TagTable;

int m_TitleOffset=0;


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CDataMngr
/// This class is used to load some offline files as <version>_Companies.ini,
///	<version>_Contexts.txt and <version>_Users.txt and store the content
///	on a table
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is used to load the long and short names of the company offline 
///	 defined. The values are stored in m_CompTable for long company name and 
///	 m_CompShortTable for short company name.
///  A RwyTable is used to stored the runway leader line offline defined.
///	 The offline defined tags are stored in m_TagTable
/// 
///  RETURNS: 
///	 void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CDataMngr::InitTables()
{
	CString AllKey;
	CString CompKey,CompShort,tmpstr,Key;
	CWin_Hints* pCWinHints=CAsdApp::GetApp()->GetWinHints();

	int pos=0,alllen,curindex=-1,curletter,oldletter=-1,len,i;
	
	for (i=0;i<26;i++)
	{
		FPos[i]=0;
		LPos[i]=-1;
	}

	CIniFile IniFile(CAsdApp::GetApp()->GetDefaultOfflinePath()+"Companies.ini");
	if (!IniFile.IsOpen())
	{
		CAsdApp::GetApp()->WriteLogMsg("File "+CAsdApp::GetApp()->GetDefaultOfflinePath()+"Companies.ini does not exist",LogError);
		CAsdApp::GetApp()->setOfflineOk(false);
	}

	alllen=IniFile.GetIniProfileSection("COMPANIES",CompKey);
	while(pos<alllen)
	{
		// store the name and short name of airline companies in m_CompShortTable and m_CompTable tables
		while (CompKey[pos]!=0)
			CompShort+=CompKey[pos++];
		
		len=IniFile.GetIniProfileString("COMPANIES",CompShort,AllKey);
		if (AllKey.GetNbrOfArgument('|')>=1)
		{
			//if first argument is taken, the 9 char long mnemonic is taken
			//if second argument is taken, the full name is taken
			AllKey.GetArgumentNb(2,'|',tmpstr);
		}

		CompShort.upper();
		m_CompShortTable.Add(CompShort);
		curindex = m_CompTable.Add(tmpstr);
		curletter = CompShort[0].latin1()-'A';
		
		if (curletter!=oldletter) 
			FPos[curletter]=curindex;
		
		LPos[curletter]=curindex;
		oldletter = curletter;
		pos++;
		CompShort=CString("");
	}
	IniFile.Close();

	// check if the <version>_ContentsCheck.txt and <version>_ASDUsers.txt exists in the adaptation data set
	IniFile.Open(CAsdApp::GetApp()->GetDefaultOfflinePath()+"ContentsCheck.txt");
    if (!IniFile.IsOpen())
    {
        CAsdApp::GetApp()->WriteLogMsg("File "+CAsdApp::GetApp()->GetDefaultOfflinePath()+"ContentsCheck.txt does not exist",LogError);
        CAsdApp::GetApp()->setOfflineOk(false);
    }
    IniFile.Close();
                                                                            
    IniFile.Open(CAsdApp::GetApp()->GetDefaultOfflinePath()+"Users.txt");        
	if (!IniFile.IsOpen())
    {
        CAsdApp::GetApp()->WriteLogMsg("File "+CAsdApp::GetApp()->GetDefaultOfflinePath()+"Users.txt does not exist",LogError);
        CAsdApp::GetApp()->setOfflineOk(false);
    }
    IniFile.Close();

	IniFile.Open(CAsdApp::GetApp()->GetApplicationIni());
	// store the runway leader line in the RwyTable table
	RwyDef currwy;
	int x,y;
	for (int rwy=1;;rwy++)
	{
		Key.sprintf("RWY%d",rwy);
		len=IniFile.GetIniProfileString("RWY_LEADER_LINES",Key,AllKey);
		int nbarg=AllKey.GetNbrOfArgument('|');
		if (nbarg>=7)
		{
			AllKey.GetArgumentNb(1,'|',currwy.Ades);
			AllKey.GetArgumentNb(2,'|',currwy.Rwy);
			AllKey.GetArgumentNb(3,'|',tmpstr);
			currwy.Orientation=atoi(tmpstr.ascii());
			AllKey.GetArgumentNb(4,'|',tmpstr);
			currwy.ColorNb=CColorMngr::GetColorNb(tmpstr);
			currwy.pPointTable=new QPointArray(nbarg-4);
			
			for (int tmp=5;tmp<=nbarg;tmp++)
			{
				AllKey.GetArgumentNb(tmp,'|',tmpstr);
				sscanf(tmpstr,"%d,%d",&x,&y);
				currwy.pPointTable->setPoint(tmp-5,x,y);
			}

			RwyTable.Add(currwy);

		}
		else
			break;
	}

	// store the callsign tags in m_TagTable table
	len=IniFile.GetIniProfileString("CALLSIGN_TAGS","TAGS",AllKey);
	int nbarg=AllKey.GetNbrOfArgument('|');
	
	for (i=1;i<=nbarg;i++)
	{
		AllKey.GetArgumentNb(i,'|',tmpstr);
		m_TagTable.Add(tmpstr);
	}

	// retrieve the title offset of the title bar and apply it for all ASD windows
	len=IniFile.GetIniProfileString("GENERAL","TITLE_OFFSET",AllKey);
	if (len)
	{
		m_TitleOffset=atoi(AllKey.ascii());
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method get the full name of an airline company given the short name 
///	 as parameter
/// 
///  RETURNS: 
///	 the name of the airline company
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
QString CDataMngr::GetCompany(QString CompShort)
{
	int curletter = CompShort[0].latin1()-'A';
	
	if ((curletter>=0) && (curletter<=25))
	{
		for (int i = FPos[curletter];i<=LPos[curletter];i++)
		{
			if (m_CompShortTable[i].compare(CompShort)==0)
			{
				return m_CompTable[i];
				break;
			}
		}
	}

	return "";
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method gives the orientation and color of a runway associated to 
///	 destination airport
/// 
///  RETURNS: 
///	 true if the orientation of the leader line has been found otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CDataMngr::GetRunway(QString Ades,QString Rwy,QPoint pnt,int& Orientation,int& ColorNb)
{
	for (int rwy=0;rwy<RwyTable.GetSize();rwy++)
	{
		RwyDef currwy=RwyTable[rwy];
		
		if ((Ades==currwy.Ades) && (Rwy==currwy.Rwy))
		{
			if (currwy.pPointTable->boundingRect().contains(pnt))
			{
				QRegion rgn(*currwy.pPointTable);
				
				if (rgn.contains(pnt))
				{
					Orientation=currwy.Orientation;
					ColorNb=currwy.ColorNb;
					return true;
				}
			}
			break;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method gives the list of runway associated to a destination airport (Ades)
/// 
///  RETURNS: 
///	 the list of runways
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
QStringList CDataMngr::GetRunwayList(QString Ades)
{
	QStringList RwyList;
	int rwy=0;

	for (rwy=0;rwy<RwyTable.GetSize();rwy++)
	{
		RwyDef currwy=RwyTable[rwy];
		if (Ades==currwy.Ades)
		{
			RwyList.append(currwy.Rwy);
		}
	}

	return RwyList;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method check if the Ades is in the FIR (RwyTable table)
/// 
///  RETURNS: 
///	 true if its in the FIR otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CDataMngr::IsBGAdes(QString Ades)
{
	int rwy=0;
	
	for (rwy=0;rwy<RwyTable.GetSize();rwy++)
	{
		RwyDef currwy=RwyTable[rwy];
		if (Ades==currwy.Ades)
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method get the list of tags from the m_TagTable table
/// 
///  RETURNS: 
///	 void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CDataMngr::GetTagList(CArray<QString,QString&>& list)
{
	list.RemoveAll();
	
	for (int i=0;i<m_TagTable.GetSize();i++)
		list.Add(m_TagTable[i]);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method gets the title offset of the title bar
/// 
///  RETURNS: 
///	 the offset of the title bar
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
 int CDataMngr::GetTitleOffset()
 {
	 return m_TitleOffset;
 }

//--------------------------------< THE END >----------------------------------
