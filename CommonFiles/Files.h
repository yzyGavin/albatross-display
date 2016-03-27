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
//  FILENAME: Files.h
//  TYPE    : header file
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

#ifndef FILES_H
#define FILES_H

//-GLOBAL DATA-----------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------
#include <qfile.h>
#include <qstring.h>
#include "StdAfx.h"
#include "IniFile.h"
                       
//-CLASSES DEFINITIONS---------------------------------------------------------

///class CFiles Declaration
class CFiles  
{
public:

	//variables
	//Array of List
	QValueList<QStringList> m_ArrayOfList;

	//methods
	//Constructor
	CFiles();
	//Constructor
	CFiles(QString strFileName);
	//Destructor
	virtual ~CFiles();
	//This method checks an ini file composed with keys
	int CheckTabFile();
	//This method checks that each field of qslVerif is corresponding to
	//a field in the Array of List
	int VerifFieldFromList(QStringList qslVerif, int nPosition);
	//This method checks if the QString qsVerif is present in the array 
	//m_ArrayOfList at the column nPosition
	int VerifFieldPresent( QString qsVerif, int nPosition );
	//This method checks if two fields are the same in a same column: nPosition
	int CheckDoubleAndEmpty(int nPosition);
	//This method checks if the QString String is present in the QStringList qslList.
	int VerifFieldInList( QStringList qslList,QString String );
	//This method opens a file and loads a key table and also an AttribTable 
	int OpenFileAndCheckTableKey();
	//This method checks the Argument list : ArgList, takes each element of this 
	//list and looks if each element of the	attribute list (AttribList) is present
	int CheckArgList(CArray<QString, QString> * ArgList);
	//This method opens a file and loads a key table and also an AttribTable 
	int OpenAndLoadFile();
	//This method opens a file and loads a key table and also an AttribTable 
	int OpenAndLoadFile(QString strFileName);
	//This method writes a key in a ini file
	int WriteKey(QString SecName,QString KeyName,QString String);
	//This function is an accessor
	void SetFileName(QString FileName);
	//This function is an accessor	
	void SetSectionName(QString SectionName);
	//This function is an accessor
	void SetAttribList(QStringList sAttribList);
	//This function is an accessor
	QString GetErrorLabel();
	//This function is an accessor
	QString GetFileName();
	//This function is an accessor
	QString GetSectionName();
	//This function is an accessor
	void SetErrorLabel(QString ErrorLabel);
	//This method empties the m_AttribList list
	void ClearAttribList();
	//This function is an accessor
	QValueList<QStringList> GetArrayOfList();
	//This method checks if the file m_FileName exists
	bool FileExist();	

private:

	//variables
	//List of attributes
	QStringList m_AttribList;
	//All section names
	QString m_Sections;
	//Error label
	QString m_ErrorLabel;
	//Section name
	QString m_SectionName;
	//File name
	QString m_FileName;
	//File Keys list
	QStringList m_KeyList;
};

#endif  // end of file

//--------------------------------< THE END >----------------------------------

