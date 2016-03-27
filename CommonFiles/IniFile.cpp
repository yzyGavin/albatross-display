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
//  FILENAME: IniFile.cpp
//  TYPE    : c++ code-file
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

typedef unsigned long       DWORD;
typedef unsigned char       BYTE;

#include "qglobal.h"
#include "string.h"
#include "Templ.h"
#include "qstringlist.h"
#include "qvaluevector.h"
#include "qfile.h"
#include "qdir.h"
#include "qapplication.h"

#include "IniFile.h"

//-GLOBAL DATA-----------------------------------------------------------------


//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CIniFile
/// This class helps in working with offline files.
///	This includes: open, close files, read sections and write sections.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CIniFile::CIniFile()
{
	m_Modified=false;
	m_Open=false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is a constructor taking in attribute a filename to be opened
///		or created
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CIniFile::CIniFile(QString FileName, bool create)
{
	m_Modified=false;
	m_Open=false;	
	Open(FileName, create);
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
CIniFile::~CIniFile()
{
	Close();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method opens an ini file and if the file exists, reads the file
///		line per line to store it on a table. If the file does not exist
///		creates the file
/// 
///  RETURNS: 
///		true if the operation has been successfully performed otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool  CIniFile::Open(QString FileName, bool create)
{
	setName(FileName);
	//File is not opened with the writable right
	if (!create)
	{
		if (open(IO_ReadOnly|IO_Translate))
		{
			QString tmp;
			while (1)
			{
				if (readLine(tmp,4096)==-1)
					break;
				tmp=tmp.left(tmp.length()-1);
				m_StringTable.Add(tmp);
			}
			m_Modified=false;
			m_Open=true;
			return true;
		}
		else
			return false;	
	}
	//File is opened with the writable right
	else
	{
		setName(FileName);
		if (open(IO_ReadWrite|IO_Translate|IO_Append))
		{
			QString tmp;
			while (1)
			{
				if (readLine(tmp,4096)==-1)
					break;
				tmp=tmp.left(tmp.length()-1);
				m_StringTable.Add(tmp);
			}
			m_Modified=false;
			m_Open=true;		
			return true;
		}
		else
			return false;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns whether a file is opened or not
/// 
///  RETURNS: 
///		true if the file is open
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CIniFile::IsOpen()
{
	return m_Open;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method closes a file if the file was opened
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CIniFile::Close()
{
	if (m_Open)
	{
		QString FileName=name();
		close();
		if (m_Modified)
		{
			open(IO_WriteOnly|IO_Translate|IO_Truncate);
			for (int line=0;line<m_StringTable.GetSize();line++)
				writeBlock(m_StringTable[line]+"\n",m_StringTable[line].length()+1);
			close();
		}
		m_StringTable.RemoveAll();
		m_Open=false;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method extracts the string from the ini file corresponding to the 
///		Section Name (SecName) and the KeyName. Then extracts the number of occurence 
//		of the key name then returns the corresponding string length
/// 
///  RETURNS: 
///		the length of the string
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
DWORD CIniFile::GetIniProfileString(QString SecName,QString KeyName,QString& String, int& nbOccurence)
{
	QString current,key;
	String="";
	bool sectionfnd=false;
	int fnd;
	nbOccurence =0;
	int nReturn =0;
	for (int line=0;line<m_StringTable.GetSize();line++)
	{
		current=m_StringTable[line];
		current=current.stripWhiteSpace();
		if (current.length())
		{
			//look if section SecName has been found in the ini file
			if ((current[0]=='[') && (current.at(current.length()-1).latin1()==']'))
			{
				//section found
				if (sectionfnd)
					return 0;
				if (current.compare("["+SecName+"]")==0)
					sectionfnd=true;
			}
			else
			{
				//if section name has been found then looks for the keyName
				//and extracts the string and returns the number of occurence 
				//of the KeyName
				if (sectionfnd)
				{
					fnd=current.find("=");
					if (fnd!=-1)
					{
						key=current.left(fnd);
						if (key.compare(KeyName)==0)
						{
							String=current.right(current.length()-fnd-1);
							nbOccurence++;
							nReturn = String.length();
						}
					}
				}
			}
		}
	}
	return nReturn;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method extracts the string from the ini file corresponding to the 
///		Section Name (SecName) and the KeyName and then returns the corresponding 
///		string length
/// 
///  RETURNS: 
///		the length of the string
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
DWORD CIniFile::GetIniProfileString(QString SecName,QString KeyName,QString& String)
{
	QString current,key;
	String="";
	bool sectionfnd=false;
	int fnd;
	for (int line=0;line<m_StringTable.GetSize();line++)
	{
		current=m_StringTable[line];
		current=current.stripWhiteSpace();
		if (current.length())
		{
			//look if section SecName has been found in the ini file
			if ((current[0]=='[') && (current.at(current.length()-1).latin1()==']'))
			{
				if (sectionfnd)
					return 0;
				if (current.compare("["+SecName+"]")==0)
					sectionfnd=true;
			}
			else
			{
				//if section name has been found then looks for the keyName
				//and extract the string
				if (sectionfnd)
				{
					fnd=current.find("=");
					if (fnd!=-1)
					{
						key=current.left(fnd);
						if (key.compare(KeyName)==0)
						{
							String=current.right(current.length()-fnd-1);
							return String.length();
						}
					}
				}
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method writes a String in a section name (SecName) of an ini file
///		corresponding to the KeyName
/// 
///  RETURNS: 
///		true if the operation has been succesfully performed otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CIniFile::WriteIniProfileString(QString SecName,QString KeyName,QString String)
{
	QString current,key;
	bool sectionfnd=false;
	int fnd;
	m_Modified=true;
	for (int line=0;line<m_StringTable.GetSize();line++)
	{
		current=m_StringTable[line];
		if (current.length())
		{
			// looks if section SecName has been found in the ini file
			if ((current[0]=='[') && (current.at(current.length()-1)==']'))
			{
				if (sectionfnd)
				{
					m_StringTable.InsertAt(line-1,KeyName+"="+String);
					return TRUE;
				}
				if (current.compare("["+SecName+"]")==0)
					sectionfnd=true;
			}
			else
			{
				//if section name and key name have been found
				//add the string on the table
				if (sectionfnd)
				{
					fnd=current.find("=");
					if (fnd!=-1)
					{
						key=current.left(fnd);
						if (key.compare(KeyName)==0)
						{
							m_StringTable[line]=KeyName+"="+String;
							return TRUE;
						}
					}
				}
			}
		}
	}
	//if the section has not been found add the section name and the Keyname
	if (!sectionfnd)
		m_StringTable.Add("["+SecName+"]");
	m_StringTable.Add(KeyName+"="+String);
	return TRUE;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves a string corresponding to a section name
/// 
///  RETURNS: 
///		the length of the string
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
DWORD CIniFile::GetIniProfileSection(QString SecName,QString& String)
{
	QString current,key;
	String="";
	bool sectionfnd=false;
	int fnd;
	int i;
	for (int line=0;line<m_StringTable.GetSize();line++)
	{
		current=m_StringTable[line];
		current=current.stripWhiteSpace();
		if (current.length())
		{
			// looks if section SecName has been found in the ini file
			if ((current[0]=='[') && (current.at(current.length()-1)==']'))
			{
				if (sectionfnd)
					break;
				if (current.compare("["+SecName+"]")==0)
					sectionfnd=true;
			}
			else
			{
				//if section name has been found retrieves the string for the key name
				if (sectionfnd)
				{
					fnd=current.find("=");
					if (fnd!=-1)
					{
						key=current.left(fnd);
						String+=key+"=";
					}
				}
			}
		}
	}

	for (i=0;i<(int)String.length();i++)
	{
		if (String[i]=='=')
			String[i]='\0';
	}
	return i;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves a list of string corresponding to a section name
/// 
///  RETURNS: 
///		0 by default
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
DWORD CIniFile::GetIniProfileSection(QString SecName,CArray<QString,QString>& ReturnedStringArray)
{
	QString current,key;
	bool sectionfnd=false;
	int fnd;
	ReturnedStringArray.RemoveAll();
	for (int line=0;line<m_StringTable.GetSize();line++)
	{
		current=m_StringTable[line];
		current=current.stripWhiteSpace();
		if (current.length())
		{
			// looks if section SecName has been found in the ini file
			if ((current[0]=='[') && (current.at(current.length()-1)==']'))
			{
				if (sectionfnd)
					break;
				if (current.compare("["+SecName+"]")==0)
					sectionfnd=true;
			}
			else
			{
				//if section name has been found, retrieves a list of string
				//coresponding to the section name
				if (sectionfnd)
				{
					fnd=current.find("=");
					if (fnd!=-1)
					{
						key=current.left(fnd);
						ReturnedStringArray.Add(key);
					}
				}
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if a section is present twice in an ini file
///		String: section name to be checked
///		bDouble: set to true if section name is present twice in the ini file
/// 
///  RETURNS: 
///		the number of occurence of the section name in the ini file
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
DWORD CIniFile::GetIniProfileSectionNames(QString& String, bool& bDouble)
{
	CArray<QString,QString> SectionArray;
	QString current;
	QString sDoubleList="";
	String="";//.Empty();
	int i;
	for (int line=0;line<m_StringTable.GetSize();line++)
	{
		current=m_StringTable[line];
		if (current.length())
		{
			// looks if section SecName has been found in the ini file
			if ((current[0]=='[') && (current.at(current.length()-1)==']'))//section found
			{
				QString sSection = current.right(current.length()-1);
				String+=sSection;
				for(int i=0; i< SectionArray.GetSize(); i++)
				{
					//look if the section name was previously found
					bDouble = (SectionArray[i] == sSection);
					if (bDouble) 
					{
						String = sSection;
						return i;
					}
				}
				SectionArray.Add(sSection);
			}
		}
	}
	for (i=0;i<(int)String.length();i++)
	{
		if (String[i]==']')
			String[i]='\0';
	}
	return i;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if a section is present twice in an ini file
///		and returns the list of section name
/// 
///  RETURNS: 
///		the list of section names
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
QStringList CIniFile::GetIniProfileSectionNames(bool& bDouble)
{
	QStringList sectionList;
	QString current;
	QString sDoubleList="";
	for (int line=0;line<m_StringTable.GetSize();line++)
	{
		current=m_StringTable[line];
		if (current.length())
		{
			// looks if section SecName has been found in the ini file
			if ((current[0]=='[') && (current.at(current.length()-1)==']'))
			{
				QString sSection = current.right(current.length()-1);
				
				if (sectionList.find(sSection) != sectionList.end())
					bDouble = true;
				
				sectionList += sSection;
			}
		}
	}
	
	// CDS BREAK JPM 2006/05/17 Use of ptr->fld access cannot be used in this case 
	//							the iterator is of type QStringList 
	QStringList::Iterator iter = sectionList.begin();
	while (iter != sectionList.end())
	{
		(*iter).remove("]");
		iter++;
	}
	
	return sectionList;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method extracts all the section names from an ini file
/// 
///  RETURNS: 
///		the length of the string
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
DWORD CIniFile::GetIniProfileSectionNames(QString& String)
{
	QString current;
	String="";
	int i;
	for (int line=0;line<m_StringTable.GetSize();line++)
	{
		current=m_StringTable[line];
		if (current.length())
		{
			if ((current[0]=='[') && (current.at(current.length()-1)==']'))
				String+=current.right(current.length()-1);
		}
	}
	for (i=0;i<(int)String.length();i++)
	{
		if (String[i]==']')
			String[i]='\0';
	}
	return i;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method deletes the content of a section 
///		from an ini file
/// 
///  RETURNS: 
///		0 if the operation failed
///		different from 0 if the opration is succesfull
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CIniFile::DeleteIniProfileSection(QString SecName)
{
	QString current,key;
	bool sectionfnd=false;
	int fnd,i=0;
	m_Modified=true;
	for (int line=0;line<m_StringTable.GetSize();line++)
	{
		current=m_StringTable[line];
		current=current.stripWhiteSpace();
		if (current.length())
		{
			if ((current[0]=='[') && (current.at(current.length()-1)==']'))
			{
				//section found
				if (sectionfnd)
				break;
				if (current.compare("["+SecName+"]")==0)
				{
					m_StringTable.RemoveAt(line);
					//The next elements have been shifted.
					//line is decreased by one to avoid missing an element.
					line--;
					sectionfnd=true;
					i++;
				}
			}
			else
			{
				if (sectionfnd)
				{
					fnd=current.find("=");
#ifdef ENABLE_QDEBUG
					qDebug("RemoveElement:%s",current.ascii());
#endif
					if (fnd!=-1)
					{
						m_StringTable.RemoveAt(line);
						//The next elements have been shifted.
						//line is decreased by one to avoid missing an element.
						line--;
						i++;
					}
				}
			}
		}
	}
	return i;
  
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the location of odp.ini file
/// 
///  RETURNS: 
///		the odp.ini file location
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
QString CIniFile::GetODPPath()
{	
	return QDir::convertSeparators(qApp->applicationDirPath() + "/odp.ini");
}

//--------------------------------< THE END >----------------------------------
