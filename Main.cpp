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
//  FILENAME: Main.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the main function which retrieves the argument of the
//		ASD (used if the ASD is launched from the ODP CSCI) and calls the
//		initialization of the application.
//				
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
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include <qapplication.h>
#include "StdAfx.h"
#include "AsdApp.h"
#include "WdwMngr.h"

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method launches the ASD application
/// 
///  RETURNS:
///		0 if the application cannot be launched 
///
///  REQUIREMENT ID: CIAsdSymOtmCzs005, CIAsdSymOtmDbg005, CIAsdFctIniGen004
//
//   ::main( int argc, char ** argv )
//-----------------------------------------------------------------------------
int main( int argc, char ** argv ) 
{
    CAsdApp a( argc, argv );
	// The debug level, if any is set 
	a.SetDebugLevel(DEBUG_LOW);
	if ((argc==2) || (argc == 3))
	{
		QString sTemp = (argv[1]);
		uint nDebugLevel = sTemp.toUInt();
		qDebug("nDebugLevel : %d", nDebugLevel);
		if (nDebugLevel > DEBUG_HIGH) 
			nDebugLevel = DEBUG_LOW;
		a.SetDebugLevel(nDebugLevel);
	}
	a.setConnectionless(argc==4);
	if (!a.Init())
		return 0;
	QString sEssai;
	sEssai.sprintf("argc : %d", argc);
	a.WriteLogMsg(sEssai, LogInformation);


   return a.exec();
}

//--------------------------------< THE END >----------------------------------
