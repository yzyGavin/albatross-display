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
//  FILENAME: InfoMngr.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains information about sectorization, qnh/tl, external
//		lines, radar statuses
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
// 
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include "String.h"
#include "InfoMngr.h"
#include "./CommonFiles/IniFile.h"
#include "AsdApp.h"
#include "SetupMngr.h"
#include "TopLevel.h"
#include "UserMngr.h"
#include "View.h"
#include "Track.h"
#include "./CommonFiles/HWFile.h"

#if (defined Q_WS_X11)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <unistd.h>
#endif

//-PROTOTYPES------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CInfoMngr
///		This class manages the information about sectorization, qnh/tl, 
///		external lines, radar statuses and checks if a resectorization is needed	
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS:
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CInfoMngr::CInfoMngr()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the copy constructor
/// 
///  RETURNS:
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CInfoMngr::CInfoMngr(CInfoMngr& infoMngr)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS:
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
CInfoMngr::~CInfoMngr()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the location, the device position, the default
///		sectorization that is offline defined.
///		Next it retrieves the sector names, the area ID and the QNH/TL associated
///		Then it retrieves the departure and arrival level for the sector and the 
///		default level
/// 
///  RETURNS:
///		returns true if successfully performed, otherwise false 
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CInfoMngr::Read(CIniFile& file, QString& Error)
{
	
	// the location could not have been retrieved offline defined
/*	if (m_Localisation.isEmpty())
	{
		Error = "No Location found in hardware configuration file. Asd is stopping.";
		return false;
	}

	// the device position (console number) cannot be retrieved from the offline defined file
	if (!m_OflDPosList.ReadDataSet(file))
	{
		Error = "Error reading Console number - CWPID section in configuration file. Asd is stopping.";
		return false;
	}
	
	// the default sectorization cannot be retrieved from the offline defined file
	if (!m_OflSectoList.ReadDataSet(file))
	{
		Error = "Error reading default sectorisation in configuration file. Asd is stopping.";
		return false;
	}
	*/
	CString AllKey;
	CString CsuKey,Key;
	CArray<QString,QString> SectorNames;
	//retrieves the sector name from the offline file and 
	//stores it in the sector table (m_SectorTable)
	/*file.GetIniProfileSection("SECTORS",SectorNames);
	if(!SectorNames.GetSize())
	{
		Error="No Sector found in version.ini [SECTORS]. Stopping ASD.";
		return false;
	}

	for (int sec=0;sec<SectorNames.GetSize();sec++)
	{
		file.GetIniProfileString("SECTORS",SectorNames[sec],AllKey);
		for (int arg=2;arg<=AllKey.GetNbrOfArgument('|');arg++)
		{
			AllKey.GetArgumentNb(arg,'|',CsuKey);
			CsuKey.stripWhiteSpace();
			char CsuId=CsuKey[0].latin1();
			if ((CsuId>='A') && (CsuId<='Z'))
				m_SectorTable[CsuId-'A']=SectorNames[sec];
		}
	}	*/

	//retrieves the area id, qnh and transition level list from the offline defined file
//	m_OflAreaList.ReadDataSet(file);

	m_DepLevels curdep;
	int lev;
	for (lev=1;;lev++)
	{
		Key.sprintf("CFL%d",lev);
		//retrieves all the usual departure flight levels offline defined as well
		//as the associated destination airport, sector and runway
		file.GetIniProfileString("DEP_USUAL_CFLs",Key,AllKey);
		if(!AllKey.length()&&lev==1)
		{
			Error="No Cfl found in version.ini [DEP_USUAL_CFLs]. Stopping ASD.";
			return false;		
		}
		if (!AllKey.length())
			break;
		if (AllKey.GetNbrOfArgument('|')==4)
		{
			AllKey.GetArgumentNb(1,'|',curdep.Sector);
			curdep.Sector.stripWhiteSpace();
			AllKey.GetArgumentNb(2,'|',curdep.Adep);
			curdep.Adep.stripWhiteSpace();
			AllKey.GetArgumentNb(3,'|',curdep.Rwy);
			curdep.Rwy.stripWhiteSpace();
			AllKey.GetArgumentNb(4,'|',curdep.Level);
			curdep.Level.stripWhiteSpace();
			m_DepLevelTable.Add(curdep);
		}
	}

	m_ArrLevels curarr;
	for (lev=1;;lev++)
	{
		Key.sprintf("CFL%d",lev);
		//retrieves all the usual arrival levels offline defined as well
		//as the associated sector
		file.GetIniProfileString("ARR_USUAL_CFLs",Key,AllKey);
		if (!AllKey.length()&&lev==1)
		{
			Error="No Cfl found in version.ini [ARR_USUAL_CFLs]. Stopping ASD.";
			return false;
		}
		if (!AllKey.length())
			break;
		if (AllKey.GetNbrOfArgument('|')==2)
		{
			AllKey.GetArgumentNb(1,'|',curarr.Sector);
			curarr.Sector.stripWhiteSpace();
			AllKey.GetArgumentNb(2,'|',curarr.Level);
			curarr.Level.stripWhiteSpace();
			m_ArrLevelTable.Add(curarr);
		}
	}
	
	m_DefaultCfl="";
	//retrieves the default CFL from the offline defined file
	file.GetIniProfileString("DEFAULT_USUAL_CFL","CFL",m_DefaultCfl);
	if (!m_DefaultCfl.length())
	{
		Error="No Cfl found in version.ini [DEFAULT_USUAL_CFL]. Stopping ASD.";
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method links the CInfoEventStub class to CInfoMngr class. As so, when
///		an event is received by CInfoEventStub class this event will be taken into
///		account by CInfoMngr class.
///		It also initializes the radar list according to sic sac put in argument
/// 
///  RETURNS:
///		void
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
void CInfoMngr::Init(CInfoEventStub* pHEvent)
{
	m_pHEvent = pHEvent;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the location in the hardware configuration file
/// 
///  RETURNS:
///		the location
///
///  REQUIREMENT ID: CIAsdFctIniGen125, CIAsdFctIniGen130
//
//-----------------------------------------------------------------------------
QString CInfoMngr::ReadLocation()
{
	QString location;
	char str[132];
	gethostname(str,132);
	QString sNull;
	QString sHostName = QString(str);
	QString sError;

	CString AllKey;
	CIniFile file(CAsdApp::GetApp()->GetApplicationIni());
	//retrieves the location of hardware configuration file in the ini file
	file.GetIniProfileString("DIRECTORIES", "HW_FILE", AllKey);
	if (AllKey.isEmpty())
	{
		CAsdApp::GetApp()->WriteLogMsg("Hardware configuration file not defined in version.ini [DIRECTORIES] HW_FILE", LogError);
		return location;
	}
	QString LSV_HWFile = AllKey;
	CHWFile fHwFile;
	if (fHwFile.Open(LSV_HWFile))
	{
		// looks for the HWCI_LOCATION section in the hardware configuration file
		int nreturn = fHwFile.FindSection("HWCI_LOCATION");
	   	if (nreturn == 0)
		{
			bool bHostFound = false;
			QString sLocation;
			int nArraySize = (int) fHwFile.m_slArray.size();
			for(int i=0; (i<nArraySize) && !bHostFound; i++)
			{
				QStringList slArrayLine = fHwFile.m_slArray[i];
				int nLineNumber = (int) slArrayLine.size();

				if (nLineNumber != 2)
				{
					QString sLogTemp = "Error at line : " + sNull.setNum(i) + " of the section HWCI_LOCATION : Incorrect argument number :";
					sLogTemp += sNull.setNum(nLineNumber) + " arguments instead of 2\n";
					sError += sLogTemp;
				}
				else if (sHostName.upper() == slArrayLine[0].upper())
				{
					//the location has been found in the hardware configuration file
					bHostFound = true;
					sLocation = slArrayLine[1];
					break;
				}
			}
			if (!bHostFound)
			{
				QString sLogTemp = "Unable to find the current host :" + sHostName + " in the section : HWCI_LOCATION\n";
				sError += sLogTemp;
			}
			else if (sLocation.length() == 0)
			{
				QString sLogTemp = "No correct line has been found in the section HWCI_LOCATION\n";
				sError += sLogTemp;
			}
			else
				location = sLocation;
		}
		else
		{
			QString sLogTemp = "section HWCI_LOCATION not found\n";
			sError += sLogTemp;
		}
	}
	else
	{
		QString sLogTemp = "Hardware Configuration File " + LSV_HWFile + " not found\n";
		sError += sLogTemp;
	}
	if (sError.length())
		CAsdApp::GetApp()->WriteLogMsg(sError,LogError);		
	return location;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the CCS status for Artas and Bypass radar servers
/// 
///  RETURNS:
///		true if both artas and bypass radar servers are available, false otherwise
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CInfoMngr::GetCCSRadarState(bool& ArtasState, bool& BypassState)
{
/*	bool foundArtas = false;
	bool foundBypass = false;
	ArtasState = false;
	BypassState = false;
	
	// scannning all radar services defined in off-line files	
	for (int i='A'; i <= 'Z'; i++)
	{
		CTrackServer* pts = CAsdApp::GetApp()->GetTrackMngr()->GetServer((char)i);
		if (pts)
		{
			// track service found in off-line for artas
			if (pts->m_TypeServer == ARTAS_SERV)
			{
				QString name;
				CRadarList::m_RADARSTATUS radarsts;
				int scanp;
				int id;
				if (m_pRadarList && m_pRadarList->Get(pts->m_Sic, pts->m_Sac, id, name, radarsts, scanp))
				{
					// artas radar service has been found in CCS RSX page
					foundArtas = true;
					if (radarsts == CRadarList::ON)
						ArtasState = true;
				}
			}

			// track service found in off-line for bypass
			if (pts->m_TypeServer == BYPASS_SERV)
			{
				QString name;
				CRadarList::m_RADARSTATUS radarsts;
				int id;
				int scanp;
				if (m_pRadarList && m_pRadarList->Get(pts->m_Sic, pts->m_Sac, id, name, radarsts, scanp))
				{
					// bypass radar service has been found in CCS RSX page
					foundBypass = true;
					if (radarsts == CRadarList::ON)
						BypassState = true;
				}
			}
		}
		if (foundArtas && foundBypass)
			break;
	}*/
	return false;//(foundArtas && foundBypass);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the position is an approch position according to the 
///		group, subgroup, location and role
/// 
///  RETURNS:
///		true if it's an approach position otherwise false
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CInfoMngr::IsAppPos()
{
	bool cr = false;

	if (CAsdApp::GetApp()->IsAppPos()) 
		cr = true;
	return cr;
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the sector corresponding to the SectorName
/// 
///  RETURNS:
///		the sector
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
char CInfoMngr::SectorName2Sector(QString SectorName)
{
	char sector = 0;
/*	if (SectorName != "")
	{
		int SectorTablesize = sizeof(m_SectorTable) / sizeof(QString);
		for(int t = 0; t < SectorTablesize; t++)
			if (m_SectorTable[t] == SectorName)
				sector = t + 'A';
	}*/
	return sector;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the CWP is sectorized
/// 
///  RETURNS:
///		true if sectorized otherwise false
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CInfoMngr::IsSectorised()
{
/*	QString secto = GetSectorList();
	if (secto.isEmpty())
		return false;
	else*/
		return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns the corrected altitude according to the Area ID
/// 
///  RETURNS:
///		the corrected altitude if one exists otherwise "****"
///
///  REQUIREMENT ID: CIAsdFctDfdCnt050
//
//-----------------------------------------------------------------------------
QString CInfoMngr::GetCorrectedAltitude(QPoint pt,int CurAlt)
{
	QString Out;
	//by default, the corrected area ID is EBBR (Area ID = 2)
	int AreaNb=2;
//	int Qnh;
//	int Tl;
	// if an offline or online QNH/TL exists for the area ID, then the 
	// corrected altitude can be calculated
//	if (m_AreaList.Get(AreaNb, Qnh, Tl) || m_OflAreaList.Get(AreaNb, Qnh, Tl))
		//if the current altitude is under the transition level then the correction
		// must be calculated otherwise the current altitude is taken into account
//		if (CurAlt<=Tl)
//		{
			int curalt=CurAlt;//+((1013-Qnh)*28/100);
			if (curalt<-999)
				Out="-999";
			else
			{
				if (curalt<0)
					Out.sprintf("%.4d",curalt);
				else
					Out.sprintf("F%.3d",curalt);
			}
//		}
//		else
//		{
//			if (CurAlt>999)
//				Out="F999";
//			else
//				Out.sprintf("F%.3d",CurAlt);
//		}
//	else 
//		Out = "****";

	return Out;
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the CWP ID for a position
/// 
///  RETURNS:
///		the CWP ID if it exists otherwise -1
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
int CInfoMngr::GetCWPID()
{
/*	int cwpid;
	QString Error;
	CDPosList::m_DSTATUS dsts = CDPosList::OFF;
	//retrieves the CWP ID from the CCS
	if (CAsdApp::GetApp()->FindCWPId(false, cwpid, dsts, Error))
		return cwpid;
	//retrieves the CWP ID offline defined
	else if (CAsdApp::GetApp()->FindCWPId(true, cwpid, dsts, Error))
		return cwpid;
	else*/
		return -1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the main logical CSU (MLCSU parameter) according to the CSU
/// 
///  RETURNS:
///		true if the main logical CSU has been retrieved otherwise false
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
bool CInfoMngr::CSU2MLCSU(char CSU, char& MLCSU)
{
	return false;//m_OflSectoList.CSU2MLCSU(CSU, MLCSU);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the default cleared flight level
/// 
///  RETURNS:
///		the default CFL
///
///  REQUIREMENT ID: NONE
//
//-----------------------------------------------------------------------------
QString CInfoMngr::GetDefaultCfl()
{
	return m_DefaultCfl;
}



//--------------------------------< THE END >----------------------------------
