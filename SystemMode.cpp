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
//  FILENAME: SystemMode.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CSystemMode with the management of the ASD System
//		Modes. The application also manages the technical System modes according
//		In order to determine the Modes transitions conditions.
//				
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
#include "SystemMode.h"
#include "View.h"
#include "DisplayElement.h"
#include "RwdView.h"
#include "AsdApp.h"
#include "Track.h"
#include "InfoMngr.h"
#include "CouplingMngr.h"
#include "FlightPlan.h"
#include "TopLevel.h"
#include "VisuMap.h"

//-GLOBAL DATA-----------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Table of System Modes
QString CSystemMode::m_SystemModeLabel[NBSYSTEMMODE];// = {"NORMAL", "BYPASS", "EMERGENCY", "EMERGENCY REDUCED", "FAILED"};



//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CSystemMode
/// This class is used to create the state diagram of system mode, submode and 
///	availability of the interfaces
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is a state diagram which sumerrize the system mode 
///	 according to the interfaces
/// 
///  RETURNS: 
///		an object of type ITF2SM
///
///  REQUIREMENT ID: CIAsdSymSmdNrm005, CIAsdSymSmdNrm010, CIAsdSymSmdNrm015,
///					CIAsdSymSmdNrm020, CIAsdSymSmdEsm005, CIAsdSymSmdEsm010,
///					CIAsdSymSmdBsm005, CIAsdSymSmdBsm010, CIAsdSymSmdErs005,
///					CIAsdSymSmdErs010, CIAsdSymSmdFld005
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CSystemMode::CSystemMode()
 : m_pHEvent(NULL), m_Sm(FAILED)
{
	ComputeSystemMode();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CSystemMode::~CSystemMode()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method links the system mode to the other ASD object via
///		CSystemModeEventStub class
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CSystemMode::Init(CSystemModeEventStub* pHEvent)
{
	m_pHEvent = pHEvent;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves offline defined the name of the different 
///		system mode
/// 
///  RETURNS: 
///		true by default
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CSystemMode::Read(CIniFile& file, QString& Error)
{
	CString AllKey;
	CArray<QString,QString> SysModeNames;
	file.GetIniProfileSection("SYSTEM_MODES",SysModeNames);
	if (SysModeNames.GetSize()!=2)
	{
		Error="At least one System Mode Label is missing";
		return false;
	}
	for (int sec=0;sec<SysModeNames.GetSize();sec++)
	{
		file.GetIniProfileString("SYSTEM_MODES",SysModeNames[sec],AllKey);
		m_SystemModeLabel[sec]=AllKey;
	}
	
	
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the current system mode name
/// 
///  RETURNS: 
///		the name of the current system mode
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
QString CSystemMode::GetSystemModeLabel()
{
	return m_SystemModeLabel[m_Sm];
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the system mode name according to the system mode
/// 
///  RETURNS: 
///		the name of the system mode
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
QString CSystemMode::GetSystemModeLabel(SystemMode sm)
{
	return m_SystemModeLabel[sm];
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the current system mode
/// 
///  RETURNS: 
///		the current system mode
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
SystemMode CSystemMode::GetSystemMode()
{
	return m_Sm;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method calculates the system mode according to
///		the interfaces available
/// 
///  RETURNS: 
///		the current sub-system mode
///
///  REQUIREMENT ID: CIAsdSymSmdNrm005, CIAsdSymSmdNrm010, CIAsdSymSmdNrm015,
///					CIAsdSymSmdNrm020, CIAsdSymSmdEsm005, CIAsdSymSmdEsm010,
///					CIAsdSymSmdBsm005, CIAsdSymSmdBsm010, CIAsdSymSmdErs005,
///					CIAsdSymSmdErs010, CIAsdSymSmdFld005, CIAsdSymSmbGen005, 
///					CIAsdSymSmbNrm150, CIAsdSymSmbNrm180, CIAsdSymSmbNrm210,
///					CIAsdSymSmbNrm220, CIAsdSymSmbNrm275, CIAsdSymSmbNrm335,
///					CIAsdSymSmbBsm105, CIAsdSymSmbBsm160, CIAsdSymSmbBsm165,
///					CIAsdSymSmbBsm230, CIAsdSymSmbBsm235, CIAsdSymSmbBsm280,
///					CIAsdSymSmbEsm115, CIAsdSymSmbEsm135, CIAsdSymSmbEsm195,
///					CIAsdSymSmbEsm230, CIAsdSymSmbErs125, CIAsdSymSmbErs160,
///					CIAsdSymSmbFld105, CIAsdSymSmdNrm005, CIAsdSymSmdNrm010, 
///					CIAsdSymSmdNrm020, CIAsdSymSmdEsm005, CIAsdSymSmdEsm010,
///					CIAsdSymSmdBsm005, CIAsdSymSmdBsm010, CIAsdSymSmdErs005,
///					CIAsdSymSmdErs010, CIAsdSymSmdFld005, CIAsdSymSmdNrm015
//					
//-----------------------------------------------------------------------------
void CSystemMode::ComputeSystemMode()
{
	SystemMode OldSm = m_Sm;
	CTrackMngr* pTrkMngr= CAsdApp::GetApp()->GetTrackMngr();
	if (pTrkMngr != NULL)
	{
		if (pTrkMngr->IsCurrentSourceAv())
			m_Sm= NORMAL;
		else
			m_Sm = FAILED;
	}
	
	if (m_Sm != OldSm)
	{
		if (m_pHEvent)
		{
			m_pHEvent->WriteLogMsg("System mode changed to " + m_SystemModeLabel[m_Sm], LogInformation);
			m_pHEvent->OnSystemModeChange(OldSm, m_Sm);
		}
		return;
	}
	ASSERT(false);
}



//--------------------------------< THE END >----------------------------------
