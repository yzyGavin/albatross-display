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
//  FILENAME: IPAddressValidator.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the implementation of the CIPAddressValidator class
// 
//  COMMENTS
//    Creation:	03-08-2005
//    Author:	Freddy Becker
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

#include <qmessagebox.h>
#include "IPAddressValidator.h"

//-DATA------------------------------------------------------------------------

                       
//-PROTOTYPES------------------------------------------------------------------

//-----------------------------------------------------------------------------

//

/// \class CIPAddressValidator 

/// QValidator derived class performing the validation and fixup of a string supposed to 

/// hold an IP address definition

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor
///   parent is the parent QObject
/// 
///  RETURNS: 
///  Void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CIPAddressValidator::CIPAddressValidator(QObject* parent): QValidator(parent)
{

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor
/// 
///  RETURNS: 
///  Void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CIPAddressValidator::~CIPAddressValidator()
{

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Fills empty positions of an IP address with 0s and removes useless leading zeros
///  inputString is a reference to the string to fix up
/// 
///  RETURNS: 
///  Void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CIPAddressValidator::fixup(QString& inputString) const
{

	QStringList ipParts = QStringList::split(".", inputString, true);
	QStringList::Iterator it = ipParts.begin();
	while (it != ipParts.end())
	{
		QString part(*it);
		if (part.isEmpty())
		{
			*it = "0";
		}

		if (part.toInt() != 0)
			/*CDS BREAK ptraccess 14/05/2006 This rule can not be applied to iterators */
			(*it).setNum(part.toInt());
		///conversion to int fails if more than one zero
		else
		{
			if (part.contains("0") == (int)(part.length()))
				*it = "0";
		}

		it++;
	}

	inputString = "";
	it = ipParts.begin();
	while (it != ipParts.end())
	{
		QString part(*it);
		inputString += part + ".";
		it++;
	}

	inputString = inputString.left(inputString.length() - 1);
	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Performs the QValidator validation
///  
///  RETURNS: 
///  Valid state of the input string
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
QValidator::State CIPAddressValidator::validate(QString& input, int&) const
{
	//QMessageBox::warning(0, "DEBUG", "String to enter validator : " + input, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	//split the string into four parts
	QStringList ipParts = QStringList::split(".", input, true);
	QStringList::Iterator it = ipParts.begin();
	bool atLeastOneByteEmpty(false), atLeastOneByteInvalid(false);
	while (it != ipParts.end())
	{
		/*CDS BREAK ptraccess 14/05/2006 This rule can not be applied to iterators */
		QString part((*it).stripWhiteSpace());
		if (!part.isEmpty())
		{
			bool conversionOk;
			int converted = part.toInt(&conversionOk);
			if ((converted > 255) || !conversionOk)
				atLeastOneByteInvalid = true;
		}
		else
			atLeastOneByteEmpty = true;

		it++;
	}
	
	if (!atLeastOneByteInvalid && !atLeastOneByteEmpty)
		return QValidator::Acceptable;
	else
		if (!atLeastOneByteInvalid && atLeastOneByteEmpty)
			return QValidator::Intermediate;
		else
			return QValidator::Invalid;
		
}

//--------------------------------< THE END >----------------------------------
