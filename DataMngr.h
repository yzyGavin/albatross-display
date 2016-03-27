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
//  FILENAME: DataMngr.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file initiates the offline data in the ASD
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

#if !defined(DataMngr_H)
#define DataMngr_H

//-INCLUDE FILES---------------------------------------------------------------
#include "./CommonFiles/Templ.h"

//-CLASS DEFINITION---------------------------------------------------------
namespace CDataMngr
{
	void InitTables();
	QString GetCompany(QString CompShort);
	bool GetRunway(QString Ades,QString Rwy,QPoint,int& Orientation,int& ColorNb);
	QStringList GetRunwayList(QString Ades);
	bool IsBGAdes(QString Ades);
	void GetTagList(CArray<QString,QString&>& list);
	int GetTitleOffset();
}


#endif  // end of file

//--------------------------------< THE END >----------------------------------

