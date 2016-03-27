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
//  FILENAME: CDirectoryFileOperations.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    Implementation of class CDirectoryFileOperations
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
#if !defined(CDIRECTORYFILEOPERATIONS)
#define CDIRECTORYFILEOPERATIONS

//-INCLUDE FILES---------------------------------------------------------------
#include <qstring.h>

//-CLASS DEFINITION---------------------------------------------------------

//Operations on files and directories (copy,delete).
class CDirectoryFileOperations
{
public:

	//variables
	QString m_LastError;

	//methods
	//Default Constructor
	CDirectoryFileOperations();
	//Default Destructor
	virtual ~CDirectoryFileOperations();
	//This function return false if source or target directory are not 
	//valid directory or if the copy failed. The directory is copied to 
	//the target directory, if recursive is true all subdirectories are
	//also copied.
	bool copyDirectory(QString sourceDirectory, QString targetDirectory, int subDirectoryLevel);
	//This function delete all the contents of the target directory, 
	//files and subdirectories.
	bool deleteDirectoryContents(QString targetDirectory);
	//This function returns true if the "targetDirectory" is an existing 
	//directory and false in other	cases (the directory does not exists, 
	//the "targetDirectory" is not a directory but a standard file).
	bool isValidDirectory(const QString & targetDirectory);
	//This function copy the file to the target directory.
	bool copyFileToDirectory(QString filePath, QString targetDirectory);
	//This function returns a description of the last error raised. The 
	//last error is not reseted between two operations.
	QString getLastError();
	//Move oldFilePath to newFilePath.
	bool moveFile(QString oldFilePath, QString newFilePath);

private:
	//This function copy the source file to the target directory.
	bool copyFile(QString source, QString &destination, bool overwrite, bool move);
	
};

#endif  // end of file

//--------------------------------< THE END >----------------------------------

