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
//  FILENAME: VersionNumberValidator.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the implementation of the CVersionNumberValidator class
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

#include "VersionNumberValidator.h"

//-DATA------------------------------------------------------------------------

                       
//-PROTOTYPES------------------------------------------------------------------

                       

//-----------------------------------------------------------------------------

//

/// \class CVersionNumberValidator 

/// QValidator derived class performig the validation of a string against an

/// ASD system version number format

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor
///  parent is the parent QObject
/// 
///  RETURNS: 
///  Void
//
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CVersionNumberValidator::CVersionNumberValidator(QObject* parent): QValidator(parent)
{

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor
/// 
///  RETURNS: 
///  Void
//
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CVersionNumberValidator::~CVersionNumberValidator()
{

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
QValidator::State CVersionNumberValidator::validate(QString& input, int& pos) const
{
	//split the string into two parts
	QStringList versionParts = QStringList::split(".", input, true);
	QStringList::Iterator it = versionParts.begin();
	/*CDS BREAK ptraccess 14/05/2006 This rule can not be applied to iterators */
	QString firstPart((*it).stripWhiteSpace());
	it++;
	/*CDS BREAK ptraccess 14/05/2006 This rule can not be applied to iterators */
	QString secondPart((*it).stripWhiteSpace());
	bool atLeastOnePartEmpty(false), atLeastOnePartInvalid(false), bothPartsNull(false);
	if (firstPart.isEmpty() || secondPart.isEmpty())
		atLeastOnePartEmpty = true;
	
	bool firstConversionOk, secondConversionOk;
	int firstInt = firstPart.toInt(&firstConversionOk);
	int secondInt = secondPart.toInt(&secondConversionOk);
	
	atLeastOnePartInvalid = (firstInt > 65535);
	atLeastOnePartInvalid = atLeastOnePartInvalid || (secondInt > 65535);
	atLeastOnePartInvalid = atLeastOnePartInvalid || (!firstConversionOk && !firstPart.isEmpty());
	atLeastOnePartInvalid = atLeastOnePartInvalid || (!secondConversionOk && !secondPart.isEmpty());
	
	if ((firstPart == "0") && (secondPart == "0"))
		bothPartsNull = true;
	
	if (!atLeastOnePartInvalid && !atLeastOnePartEmpty && !bothPartsNull)
		return QValidator::Acceptable;
	else
		if (!atLeastOnePartInvalid && atLeastOnePartEmpty)
			return QValidator::Intermediate;
		else
			return QValidator::Invalid;
		
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Fills empty positions of the version number with 0s and removes useless leading zeros
///  inputString is a reference to the string to fix up
/// 
///  RETURNS: 
///  Void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CVersionNumberValidator::fixup(QString& inputString) const
{

	QStringList versionParts = QStringList::split(".", inputString, true);
	QStringList::Iterator it = versionParts.begin();
	while (it != versionParts.end())
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
			if (part.contains("0") == part.length())
				*it = "0";
		}

		it++;
	}

	inputString = "";
	it = versionParts.begin();
	while (it != versionParts.end())
	{
		QString part(*it);
		inputString += part + ".";
		it++;
	}

	inputString = inputString.left(inputString.length() - 1);
	
}

//--------------------------------< THE END >----------------------------------
