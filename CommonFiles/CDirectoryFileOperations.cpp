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
//  FILENAME: CDirectoryFileOperations.cpp
//  TYPE    : c++ code-file
// 

//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CDirectoryFileOperations
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
// 
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------
#include "CDirectoryFileOperations.h"
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qmessagebox.h>

//-GLOBAL DATA-----------------------------------------------------------------
                       
//-LOCAL DATA------------------------------------------------------------------
 
//-PROTOTYPES---------------------------------------------------------
//-----------------------------------------------------------------------------
//
/// \class CDirectoryFileOperations 
/// This class performs operations on directories (copy, move, delete, empty)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Default Constuctor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CDirectoryFileOperations::CDirectoryFileOperations()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Default Destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CDirectoryFileOperations::~CDirectoryFileOperations()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function returns false if source or target directory are not 
///		valid directory or if the copy failed. The directory is copied to 
///		the target directory, if recursive is true all subdirectories are
///		also copied.
/// 
///  RETURNS: 
///		true to copy all subdirectories otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CDirectoryFileOperations::copyDirectory(QString sourceDirectory, QString targetDirectory, int subDirectoryLevel)
{
	//Check:[source,target]Directory are a valid directory.
	QFileInfo sourceInfo=QFileInfo(sourceDirectory);
	QFileInfo targetInfo=QFileInfo(targetDirectory);
	if ((!sourceInfo.isDir()) || (!sourceInfo.exists()))
	{
		//qDebug("Source Is NOT Ok.");
		return false;
	}
	
	//qDebug("Source Is Ok.");
	
	if (!targetInfo.isDir())
	{
		QDir dirTarget;
		dirTarget.mkdir(targetInfo.absFilePath());
	}
	  
	//Creating targetDirectory
	QDir sourceDir=QDir(sourceInfo.absFilePath());
	QDir targetDir=QDir(targetInfo.absFilePath());
	QFileInfo 
	targetDestinationInfo=QFileInfo(targetInfo.absFilePath(),sourceInfo.fileName());
	targetDir.mkdir(sourceInfo.fileName());
	
	int i=2;
	//Copy directory contents
	QFileInfo fileSrcInfo=QFileInfo(sourceInfo.absFilePath(),sourceDir[i]);
	while (sourceDir[i]!=QString::null)
	{
		//if it is a directory then call recursively copyDirectory
		if ((fileSrcInfo.isDir())&&(subDirectoryLevel!=1))
		{
			if (!copyDirectory(fileSrcInfo.filePath(),targetDestinationInfo.filePath(),subDirectoryLevel-1))
				return false;
		}
		  
		//if it is a file then copy the file.
		if (fileSrcInfo.isFile())
		{
			QString strTarget=targetDestinationInfo.filePath();
			if (!copyFile(fileSrcInfo.filePath(),strTarget,true,false))
			return false;
		}
		  
		i++;
		fileSrcInfo=QFileInfo(sourceInfo.absFilePath(),sourceDir[i]);
	}
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function deletes all the contents of the target directory, 
///		files and subdirectories.
/// 
///  RETURNS: 
///		true if succesfull otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CDirectoryFileOperations::deleteDirectoryContents(QString targetDirectory)
{
	//QMessageBox::information(0,"Delete Directory Contents",targetDirectory);
	//If it is a directory
	if (isValidDirectory(targetDirectory))
	{
		QDir dir(targetDirectory);
		int i=2;
		QString file=dir.path()+"/"+dir[i];
		//delete directory contents
		while (dir[i]!=QString::null)
		{
			//QMessageBox::information(0,"Delete file:",file);
			if (isValidDirectory(file))
			{
				//recursive call to deleteDirectoryContents
				if (!deleteDirectoryContents(file))
					return false;
				//deleting directory
				if (!dir.rmdir(file))
					return false;
			}
			else
				//Deleting file
				if (!dir.remove(file))
					return false;
			i++;
			file=dir.path()+"/"+dir[i];
		}
		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function returns true if the "targetDirectory" is an existing 
///		directory and false in other cases (the directory does not exists, 
///		the "targetDirectory" is not a directory but a standard file).
/// 
///  RETURNS: 
///		true if directory exists otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CDirectoryFileOperations::isValidDirectory(const QString & targetDirectory)
{
	QFileInfo fiTarget=QFileInfo(targetDirectory);
	return (QFile::exists( targetDirectory)&&fiTarget.isDir());
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function copies the file to the target directory.
/// 
///  RETURNS: 
///		true if successfull otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CDirectoryFileOperations::copyFileToDirectory(QString filePath, QString targetDirectory)
{
	return (copyFile(filePath,targetDirectory,true,false));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function returns a description of the last error raised. The 
///		last error is not reseted between two operations.
/// 
///  RETURNS: 
///		a string containing the last error raised
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
QString CDirectoryFileOperations::getLastError()
{
	return m_LastError;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		Move oldFilePath to newFilePath.
/// 
///  RETURNS: 
///		true if successfull otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CDirectoryFileOperations::moveFile(QString oldFilePath, QString newFilePath)
{
	return (copyFile(oldFilePath,newFilePath,true,true));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function copies the source file to the target directory.
/// 
///  RETURNS: 
///		true if successfull otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CDirectoryFileOperations::copyFile(QString source, QString &destination, bool overwrite, bool move)
{
	QFileInfo sourceInfo = QFileInfo(source);
	QFileInfo destinationInfo = QFileInfo(destination);

	if(!sourceInfo.exists()) 
    {
    	m_LastError=("File Copy Failed - Source File does not exists.\nSource:\""+source+"\".");
    	return false;
    }
  	else 
  		if(!sourceInfo.isFile())
	    {
      		m_LastError=("File Copy Failed - Source is not a file.\nSource:\""+source+"\".");
      		return false;
		}
		else
			if(!destinationInfo.exists())
		    {
		      m_LastError=("File Copy Failed - Destination cannot be found:\""+destination+"\".");
		      return false;
		    }

	// if destination is directory then use same file name QFileInfo::fileName()
	if(destinationInfo.isDir())
    	destination = (QFileInfo(QDir(destinationInfo.absFilePath()), sourceInfo.fileName())).filePath(); 
	else
		if(!destinationInfo.isFile())
		{
			m_LastError="File Copy failed - Destination is neither a file or directory.";
			return false; // its neither file nor directory
		}

	// if destination file exists and overwrite disabled - return false
	if(!overwrite && QFile::exists(destination))
    {
    	m_LastError="File Copy failed - Destination file exists, overwrite disabled.";
	    return false; // its neither file nor directory 
    }


	QFile sourceFile(source);
	QFile destinationFile(destination);
	QString strMsg;
	strMsg.sprintf("Copy file :%s to :%s\n",source.ascii(),destination.ascii());

	if(sourceFile.open(IO_ReadOnly) && destinationFile.open(IO_WriteOnly))
    {
    	uint dataLength = 4096;
    	char *data = new char[dataLength];
      	//Copying the file
      	while(!sourceFile.atEnd())
		{
	  		destinationFile.writeBlock(data, sourceFile.readBlock(data, dataLength));
	  		destinationFile.flush();
		}
      	delete[] data;
      	if(move)
		{
	  		if(!sourceFile.remove())
	    	{
	      		destinationFile.remove();
	      		sourceFile.close();
	      		destinationFile.close();
	      		m_LastError="File Copy failed - Source file could not be removed.";
	      		return false;
	    	}
		}
      	sourceFile.close();
      	destinationFile.close();
    }
  	else
    {
    	sourceFile.close();
    	destinationFile.close();
      	//qDebug("DBG: Error - Source or destination file could not be opened.");
      	m_LastError="File Copy failed - Source or destination file could not be opened.";
      	return false;
    }
	m_LastError = "";
	return true;
} 
 
//--------------------------------< THE END >----------------------------------

