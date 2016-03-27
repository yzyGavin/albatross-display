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
//  FILENAME: IniFile.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CIniFile which helps in working with offline
//		files.
//		This includes: open, close files, read sections and write sections.
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

#if !defined(AFX_INIFILE_H_INCLUDED_)
#define AFX_INIFILE_H_INCLUDED_

//-INCLUDE FILES---------------------------------------------------------------

#include "qfile.h"
#include "Templ.h"

//-GLOBAL DATA-----------------------------------------------------------------

//-CLASSES DEFINITIONS---------------------------------------------------------


typedef unsigned long       DWORD;

class CIniFile : public QFile
{
public:

	//methods
	//Constructor
	CIniFile();
	//Constructor
	CIniFile(QString FileName, bool create = false);
	//Destructor
	~CIniFile();
	//This method opens an ini file and if the file exists, reads the file
	//line per line to store it on a table. If the file does not exist creates the file
	bool  Open(QString FileName, bool create = false);
	//This method closes the file
	void Close();
	//This method returns whether the file is opened or not
	bool IsOpen();
	//This method extracts all the section names from an ini file
	DWORD GetIniProfileSectionNames(QString& ReturnedString);
	//This method checks if a section is present twice in an ini file
	DWORD GetIniProfileSectionNames(QString& ReturnedString, bool& bDouble);
	//This method checks if a section is present twice in an ini file and returns 
	//the list of section name
	QStringList GetIniProfileSectionNames(bool& bDouble);
	//This method retrieves a string corresponding to a section name
	DWORD GetIniProfileSection(QString SecName,QString& ReturnedString);
	//This method retrieves a list of string corresponding to a section name
	DWORD GetIniProfileSection(QString SecName,CArray<QString,QString>& ReturnedStringArray);
	//This method extracts the string from the ini file corresponding to the Section
	//Name (SecName) and the KeyName and then returns the corresponding string length.
	DWORD GetIniProfileString(QString SecName,QString KeyName,QString& ReturnedString);
	//This method extracts the string from the ini file corresponding to the Section
	//Name (SecName) and the KeyName. Then extracts the number of occurence of the key
	//name then returns the corresponding string length
	DWORD GetIniProfileString(QString SecName,QString KeyName,QString& ReturnedString, int& nbOccurence);
	//This method writes a String in a section name (SecName) of an ini file 
	//corresponding to the KeyName
	bool WriteIniProfileString(QString SecName,QString KeyName,QString String);
	//This method deletes the content of a section from an ini file
	bool DeleteIniProfileSection(QString SecName);
	//This method gets the location of odp.ini file
	static QString GetODPPath();
	
private:

	//variables
	//Table of string filled with the content of an ini file when opened 
	CArray<QString,QString> m_StringTable;
	//Whether the ini file has been modified or not
 	bool m_Modified;
	//Whether the ini file is opened or not
	bool m_Open;
};


#endif

//--------------------------------< THE END >----------------------------------
