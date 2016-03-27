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
//  FILENAME: SystemMode.h
//  TYPE    : header file
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
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 1  2009/08/31 dalleins
//
//
// 
//-----------------------------------------------------------------------------
#if !defined(AFX_SYSTEMMODEAUTOMAT_H)
#define AFX_SYSTEMMODEAUTOMAT_H

//-INCLUDE FILES---------------------------------------------------------------

//-GLOBAL DATA-----------------------------------------------------------------

//-CLASSES DEFINITIONS---------------------------------------------------------


#include "CommonDataTypes.h"

class CSystemModeEventStub : public CLogStub
{

public :

	//virtual methods
	virtual void OnSystemModeChange(SystemMode oldsm, SystemMode newsm) = 0;

	//methods
	CSystemModeEventStub() {};
	~CSystemModeEventStub() {};	
};


class CSystemMode : public QObject
{
	friend class CAsdApp;
	friend class CEventMngr;
	friend class CTopLevel;
	friend class CSystemInfo;
	
private:

	//variables
	static QString m_SystemModeLabel[NBSYSTEMMODE];
	SystemMode m_Sm;

	//pointers
	CSystemModeEventStub* m_pHEvent;

	//methods
	CSystemMode();
	CSystemMode(const CSystemMode & sysMode);
	~CSystemMode();	
	void ComputeSystemMode();	
	QString GetSystemModeLabel(SystemMode sm);
	bool Read(CIniFile& file, QString& Error);
	void Init(CSystemModeEventStub* pHEvent);
	SystemMode GetSystemMode();
	QString GetSystemModeLabel();
	
};

#endif

//--------------------------------< THE END >----------------------------------

