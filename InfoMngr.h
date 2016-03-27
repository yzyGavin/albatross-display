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
//  FILENAME: InfoMngr.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains information about sectorization, qnh/tl, 
//		external lines, radar statuses
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
// 
//-----------------------------------------------------------------------------

#if !defined(InfoMngr_H)
#define InfoMngr_H

//-INCLUDE FILES---------------------------------------------------------------

#include "CommonDataTypes.h"
#include "InfoMngr.h"

//-CLASS DEFINITION------------------------------------------------------------

class CInfoEventStub : public CLogStub
{
public :
	CInfoEventStub() {};
	~CInfoEventStub() {};
};

class CInfoMngr : public CObject
{
	friend class CAsdApp;
	friend class CWeatherMngr;
	friend class CExitValDlg;
	friend class CFlSelectDlg;
	friend class CEventMngr;
	friend class CSystemInfo;
	friend class CTrack;
	friend class CUserMngr;
	friend class CTimeWnd;
	friend class TrackControl;
	friend class CFlightPlan;

private:

	//variables
	// contains the default cleared flight level
	QString m_DefaultCfl;
	// structure for Departure levels
	typedef struct
	{
		// name of the sector
		QString Sector;
		// destination airport
		QString Adep;
		// runway
		QString Rwy;
		// departure level
		QString Level;
	} m_DepLevels;
	// table of type m_DepLevels
	CArray<m_DepLevels,m_DepLevels> m_DepLevelTable;
	// structure for Arrival levels
	typedef struct
	{
		// name of the sector
		QString Sector;
		// arrival level
		QString Level;
	} m_ArrLevels;
	// table of type m_ArrLevels
	CArray<m_ArrLevels,m_ArrLevels> m_ArrLevelTable;

	//pointers

	// pointer of type CInfoEventStub
	CInfoEventStub* m_pHEvent;

	//virtual methods


	//methods

	// constructors of CInfoMngr class
	CInfoMngr();
	CInfoMngr(CInfoMngr& infoMngr);
	// destructor of CInfoMngr class
	~CInfoMngr();
	// links the CInfoEventStub class to CInfoMngr class. As so, when
	// an event is received by CInfoEventStub class this event will be taken into
	// account by CInfoMngr class.
	// It also initializes the radar list according to sic sac put in argument
	void	Init(CInfoEventStub* pHEvent);
	// retrieves the location, the device position, the default
	// sectorization that is offline defined.
	// Next it retrieves the sector names, the area ID and the QNH/TL associated
	// Then it retrieves the departure and arrival level for the sector and the 
	// default level
	bool	Read(CIniFile& file, QString& Error);
	// gets the location in the hardware configuration file
	QString ReadLocation();
	// returns the line status of an external line according to the line status number
	QString GetExtLineStatus(int cnt);
	// gets the line name of an external line according to the line status number
	QString GetExtLineName(int cnt);
	// gets the main logical CSU (MLCSU parameter) according to the CSU
	bool	CSU2MLCSU(char CSU, char& MLCSU);
	// gets the sector corresponding to the SectorName
	char	SectorName2Sector(QString SectorName);
	// gets the CCS status for Artas and Bypass radar servers
	bool	GetCCSRadarState(bool& ArtasState, bool& BypassState);
	// checks if the CWP is sectorized
	bool	IsSectorised();
	// the corrected altitude according to the Area ID
	QString GetCorrectedAltitude(QPoint pos,int CurAlt);
	// retrieves the default cleared flight level
	QString GetDefaultCfl();
	// checks if the position is an approch position according to the group, subgroup, location and role
	bool	IsAppPos();
	// checks if the sector (sectorid) is controlled by a position
	int		GetCWPID();
};
#endif  // end of file

//--------------------------------< THE END >----------------------------------

