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
//  FILENAME: String.cpp
//  TYPE    : c++ code-file
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
#include "String.h"

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-START OF CODE---------------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CString
///		This class inherits from QString with new functions to help
//		QString comparison and treatment.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CString::CString()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CString::~CString()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the copy constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CString::CString(const QString &str):QString(str)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method compares this CString object with another string.
///		It performs a case-insensitive comparison of the strings.
///		Argument: str is the another string. 
/// 
///  RETURNS: 
///		returns 0 if the strings are identical (ignoring case), 
///		<0 if this CString object is less than lpsz (ignoring case), 
///		or >0 if this CString object is greater than lpsz (ignoring case).
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CString::compareNoCase(QString str)
{
	QString str1=upper();
	QString str2=str.upper();

	return compare (str1,str2);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the characters of the string are numerical 
/// 
///  RETURNS: 
///		returns true if all characters of the string are numerical, 
///		otherwise false.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
bool CString::isNumeric()
{
	if (!length())
		return false;
	for  (int i=0;i<(int)length();i++)
	{
		if (!at(i).isDigit())
			return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns the number of element, included in the string, 
///		which are separated by the separator argument.
///		Argument: Separator is the separator character
/// 
///  RETURNS: 
///		returns the number of argument included in the string
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CString::GetNbrOfArgument(char Separator)
{
	int NbArg=0,Fnd=-1;
	QString ArgStr=stripWhiteSpace();
	
	if (ArgStr.at(ArgStr.length()-1)!=Separator)
		ArgStr+=Separator;
	do
	{
		Fnd=ArgStr.find(Separator,Fnd+1);
		if (Fnd==-1)
			break;
		NbArg++;
	}
	while(1);

	return NbArg;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the string is made of more than n strings 
///		which are separated by a separator.
///		Argument1: ArgNb represents the n strings
///		Argument2: Separator represents the separator
///		Argument3: Arg is a reference to a QString. It represents the last 
///				   string which is found in the string
/// 
///  RETURNS: 
///		returns true if the function is succesful, otherwise false.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
bool CString::GetArgumentNb(int ArgNb,char Separator,QString& Arg)
{
	int Fnd=-1,Nxt;
	QString ArgStr=stripWhiteSpace();
	
	if (ArgStr.at(ArgStr.length()-1)!=Separator)
		ArgStr+=Separator;
	
	for (int i=1;i<ArgNb;i++)
	{
		Fnd=ArgStr.find(Separator,Fnd+1);
		if (Fnd==-1)
			return false;
	}
	Nxt=ArgStr.find(Separator,Fnd+1);
	
	if (Nxt==-1)
		return false;
	
	Arg=ArgStr.mid(Fnd+1,Nxt-1-Fnd);
	
	return true;
}

//--------------------------------< THE END >----------------------------------
