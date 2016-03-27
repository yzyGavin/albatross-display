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
//  FILENAME: SetupMngr.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//	  Implementation of class CSetupMngr which managemes the Customization Sets. 
// 
//  COMMENTS
//	  x
//
//  BUILD INFORMATION 
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
// 
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include "AsdApp.h"
#include "SetupMngr.h"
#include "ColorMngr.h"
#include "./CommonFiles/IniFile.h"

//-GLOBAL DATA-----------------------------------------------------------------

CArray<CObject*,CObject*> m_ObjectTable;
QString m_DefaultFileName;
QString m_DefaultSetup;

//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CSetupMngr
/// This class is used to load or save the default or personal customization
///	sets.
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method adds an object which will be managed 
///		by the Setup manager
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CSetupMngr::AddObject(CObject* pObject)
{
	m_ObjectTable.Add(pObject);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method removes the object from the Setup manager
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CSetupMngr::RemoveObject(CObject* pObject)
{
	for (int i=0;i<m_ObjectTable.GetSize();i++)
	{
		if (m_ObjectTable[i]==pObject)
			m_ObjectTable[i]=NULL;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method saves or loads the default or personal custom set according 
///		File name and section name given in attribute.
/// 
///  RETURNS: 
///		true by default
///
///  REQUIREMENT ID: CIAsdFctCzs070, CIAsdFctCzs095, CIAsdFctCzs100,
///					 CIAsdFctCzs105, CIAsdFctIniCzs020 
//-----------------------------------------------------------------------------
bool CSetupMngr::Setup(QString FileName,QString SetupName,bool Save)
{
	CIniFile file;
	QString AllKey;
	int i;
	
	QFile fd(FileName);

	if ((!file.Open(FileName)))
	{
		// Catch if an error occurs while saving the customization set
		if (Save)
		{			
			QFile newfile(FileName);
			if (!newfile.open(IO_WriteOnly|IO_Translate|IO_Truncate))
			{
				CSKMessageBox::critical( 0, "Error Message","An error occured. This customization set could not be created.");
				return false;
			}
			newfile.close();
			if (!file.Open(FileName))
			{
				CSKMessageBox::critical( 0, "Error Message","An error occured. This customization set could not be opened.");
				return false;
			}
		}
		else
		{
			CSKMessageBox::critical( 0, "Error Message","An error occured. This customization set could not be opened.");
			return false;
		}
	}
	// reads the customization set or log an error otherwise
	if (!Save)
	{
		file.GetIniProfileString(SetupName,"LAST_SAVE_TIME",AllKey);
		if (AllKey.length()==0)
		{
			CSKMessageBox::critical( 0, "Error Message","An error occured. This customization set could not be read.");
			return false;
		}
	}
	else
	{
		// Save the customization set in the corresponding username file
		bool isopen= fd.open(IO_WriteOnly|IO_Append);
		if (!isopen)
		{
			CSKMessageBox::critical( 0, "Error Message","An error occured. The default customization set could not be saved.");
                        return false;
		}
		QDateTime curtime=QDateTime::currentDateTime();
		file.WriteIniProfileString(SetupName,"LAST_SAVE_TIME",curtime.toString());
		file.WriteIniProfileString("GENERAL1234","LAST_MODIFICATION",curtime.toString());
	}
	// Load or save the setup for windows contained in m_ObjectTable according to username 
	// for the group, subgroup, location, role
	CColorMngr::Setup(&file,SetupName,Save);
	for (i=0;i<m_ObjectTable.GetSize();i++)
	{
	  if (m_ObjectTable[i])
	      m_ObjectTable[i]->Setup(&file,SetupName,Save);
	}
	for (i=m_ObjectTable.GetSize()-1;i>=0;i--)
	{
		if (m_ObjectTable[i]==NULL)
			m_ObjectTable.RemoveAt(i);
	}
	m_DefaultFileName=FileName;
        m_DefaultSetup=SetupName;
	file.Close();

	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method loads the default custom set according to the Group, SubGroup,
///		Location and Role, as given in parameter (SetupName).
///		If one value is not found, it is replace by "Common".
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctIniCzs005 
//-----------------------------------------------------------------------------
void CSetupMngr::SetDefaultSetup()
{
	QString setup="DEFAULT";
	int fnd,j;
	int NbArg;
	//the "/" are removed
	setup.replace("/","");
	QString fullSetupName=setup;
	int NbCommon=fullSetupName.contains("Common");
	for (int i=0; i<NbCommon;i++)
	{
		fnd=fullSetupName.findRev("_");
		if (fnd==-1)
			break;
		fullSetupName=fullSetupName.left(fnd);
	}
	setup.replace("_","");
	QString FileName = CAsdApp::GetApp()->GetDefaultOfflinePath() + "DefaultCustomSet.ini";
	CIniFile file;
	QString AllKey;
	//Check if the Setup name exists as a default customization set, if not the 
	//Group, SubGroup, Location and role are one by one changed with "Common"
	for (;;)
	{
		if (file.Open(FileName))
		{
			file.GetIniProfileString(setup,"LAST_SAVE_TIME",AllKey);
			file.Close();
			if (AllKey.length()!=0)
			{
				m_DefaultFileName=FileName;
				m_DefaultSetup=setup;
				Setup(FileName,setup,false);
				break;
			}		
			fnd=fullSetupName.findRev("_");
			if (fnd==-1)
			{
				if (fullSetupName.length())
				{
					setup="CommonCommonCommonCommonCommon";
					fullSetupName="";
				}
				else
					break;
			}
			else
			{
				fullSetupName=fullSetupName.left(fnd);
				NbArg=fullSetupName.contains("_");
				setup=fullSetupName;
				for (j=0;j<3-NbArg;j++)
					setup+="_Common";
				setup.replace("_","");
			}
		}
		else
			break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method loads the default custom set
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CSetupMngr::ReloadDefaultSetup()
{
	Setup(m_DefaultFileName,m_DefaultSetup,false);
}

//--------------------------------< THE END >----------------------------------

