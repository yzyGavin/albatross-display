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
//  FILENAME: HWFile.cpp
//  TYPE    : c++ code-file
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
//-----------------------------------------------------------------------------
//
/// \class CHWFile
/// This class manages Analyses of Hardware Ini Files
//
//-----------------------------------------------------------------------------
//-INCLUDE FILES---------------------------------------------------------------
typedef unsigned char       BYTE;

#include "qglobal.h"
#include "string.h"
#include "Templ.h"
#include "qstringlist.h"
#include "qvaluevector.h"
#include "qfile.h"

#include "HWFile.h"
#include <qmessagebox.h>
//-GLOBAL DATA-----------------------------------------------------------------

                       
//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------
//-----------------------------------------------------------------------------
//
/// \class CHWFile
/// This class is used to open and close the Hardware configuration file
///	and extract information included in this file
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the default constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CHWFile::CHWFile()
{
	m_Open=false;
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
CHWFile::~CHWFile()
{
	Close();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor which automaticly open the file, 
///		if exists
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CHWFile::CHWFile(QString FileName)
{
	Open(FileName);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method opens the FileName file, if exists
/// 
///  RETURNS: 
///		true if the FileName is successfully opened otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CHWFile::Open(QString FileName)
{
	setName(FileName);
	if (open(IO_ReadOnly|IO_Translate))
	{
		QString tmp;
		while (1)
		{
			if (readLine(tmp,1024)==-1)
				break;
			tmp=tmp.left(tmp.length()-1);
			m_StringTable.Add(tmp);
		}
		m_Open=true;
		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------------------------
//
///  DESCRIPTION:
///  This method indicates if the file is open.
//   THIS METHOD IS USED BY THE LSV. PLEASE DO NOT REMOVE !!!
///
///  RETURNS:
///             void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CHWFile::IsFileOpen()
{
  return m_Open;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method close the file if it is open
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CHWFile::Close()
{
	if (m_Open)
	{
		QString FileName=name();
		close();
		m_StringTable.RemoveAll();
		m_Open=false;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method looks inside the file to found SectionName with [] 
///		without any comment on this line
///		It will then extract all the different lines in the section and put the correct values 
///		in the array m_slArray. This is a QStringList array
/// 
///  RETURNS: 
///		0 if the extraction from the section name has been successfully retrieved
///		otherwise returns a negative value
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CHWFile::FindSection(QString SectionName)
{
	m_slArray.clear();
	QString current;
	QString strTemp="";//.Empty();
	bool bFindSection = false;
	int line=0;
	for (;(line<m_StringTable.GetSize()) && !bFindSection; line++)
	{
		strTemp = "";
		current=m_StringTable[line];
		if(current.length())
		{
			if ((current[0]=='[') && (current.at(current.length()-1)==']'))//section found
			{
				strTemp =current.right(current.length()-1); // remove [
				strTemp =strTemp.left(strTemp.length()-1); // remove ]
				bFindSection = (strTemp == SectionName);
			}
		}
	}
	if (!bFindSection) 
		return -1; //the section has not been found
	if (line == m_StringTable.GetSize()) 
		return -2; // the section is empty and at the end of the file
	
	bFindSection = false;
	for (;(line<m_StringTable.GetSize()) && !bFindSection; line++)
	{
		strTemp = "";
		current=m_StringTable[line];
		if(current.length())
		{
			if ((current[0]=='[') && (current.at(current.length()-1)==']'))//section found
			{
				bFindSection = true;
			}
			else
			{
				ExtractLine(current);
			}
		}
	}
	if ((int)(m_slArray.size() <=0)) 
		return -3; // Array is empty
	return 0;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function take a line of argument separated by a tabulation 
///		It will then put the values in a QStringList and the the QStringList in a array of QSL
///		if 2 slash are presents "//"the part of the line since the 2 slashes will not be considered
///		If the line or the element is only compose of space, it will be ignore
///		If spaces are at the beginning of an element, they will be deleted.
/// 
///  RETURNS: 
///		0 by default
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CHWFile::ExtractLine(QString sLine)
{
	QString sElement;
	QStringList sList;
	QChar CurChar;
	bool bLoopLigne;
	bool bCarPrecSlash = false;
	if (sLine.length() >1)
	{
		sElement ="";
		sList.empty();
		bLoopLigne = true;
		for(uint i=0; (i<=sLine.length()) && bLoopLigne; i++)
		{
			CurChar = sLine[(int)i];
			sElement.append(CurChar);
			if (CurChar =='/')
			{
				bCarPrecSlash = !bCarPrecSlash;
				if (!bCarPrecSlash) //we have 2 slash following
				{
					if (sElement.length() > 2) 
						sList.append(sElement.left(sElement.length()-1));
					sElement = "";
					bLoopLigne = false;
				}				
			}
			else bCarPrecSlash = false;

			if (CurChar == '\t') // this is the separator
			{
				if (sElement.length() > 1) 
					sList.append(sElement.left(sElement.length()-1));
				sElement = "";
			}
			if ((CurChar == ' ') && (sElement.length() <= 1)) //this is a space at a beginning of an element
			{
				sElement = "";
			}
			if ((CurChar == '\n') && (sElement.length() <= 1)) //this is a end of line at a beginning of an element
			{
				sElement = "";
			}
		}
		if (sElement.left(sElement.length()-1) != "") 
			sList.append(sElement.left(sElement.length()-1));
		if (sList.size() > 0) 
			m_slArray.append(sList);
	}
	return 0;
}
//--------------------------------< THE END >----------------------------------

