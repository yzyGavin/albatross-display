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
//  FILENAME: String.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CString which inherits from QString with new 
//		functions to help QString comparison and treatment.
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
#if !defined(String_H)
#define String_H

//-INCLUDE FILES---------------------------------------------------------------

//-GLOBAL DATA-----------------------------------------------------------------

//-CLASSES DEFINITIONS---------------------------------------------------------

class CString : public QString
{
public:
	// constructor of CString class
	CString();
	// copy of constructor
	CString(const QString& str);
	// destructor
	~CString();
	// turns the QString into upper case
	int compareNoCase(QString str);
	// checks if the string is a numeric value
	bool isNumeric();
	// Get the number of arguments in a string according to the Separator parameter
	int GetNbrOfArgument(char Separator);
	// checks if the string is made of more than n strings which are separated by a separator
	bool GetArgumentNb(int ArgNb,char Separator,QString& Arg);
};
#endif

//--------------------------------< THE END >----------------------------------
