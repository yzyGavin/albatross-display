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
//  FILENAME: Files.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 1  2009/08/31 dalleins
// 
//-----------------------------------------------------------------------------
//
/// \class CFiles
/// This Class manage Analyses of Ini Files
//
//-----------------------------------------------------------------------------
//-INCLUDE FILES---------------------------------------------------------------
#include "StdAfx.h"
#include "Files.h"
#include <qstringlist.h> 
#include <qvaluelist.h> 
#include <qfont.h>
#include <qlayout.h>
#include <string.h>
//-GLOBAL DATA-----------------------------------------------------------------

                       
//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CFiles::CFiles(QString strFileName)
{
	m_ErrorLabel="";
	m_AttribList.clear();
	m_FileName = strFileName;
	m_SectionName="";
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CFiles::CFiles()
{
	m_ErrorLabel="";
	m_AttribList.clear();
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
CFiles::~CFiles()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the file m_FileName exists
/// 
///  RETURNS: 
///		true if the file exists otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CFiles::FileExist()
{
  QFile m_fTemp;
  m_fTemp.setName(m_FileName);
  return  m_fTemp.exists();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function opens a file and loads a key table 
///		and also a AttribTable (2Dimension table)	
/// 
///  RETURNS: 
///		-1 if an error occurred during the analyze of the file content.
///		0 if everything append correctly.	
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CFiles::OpenFileAndCheckTableKey()
{
	bool bError = false;
	QStringList m_ListTemp;
	CIniFile iniFile(m_FileName);
	m_ErrorLabel="";
	QString sTmp, sLeft;
	iniFile.GetIniProfileSectionNames(m_Sections);
	// all sections names are now in the String m_Sections, separated by a '\0'
	m_KeyList.clear();
	for (uint i=0;(i<m_Sections.length() && m_Sections.length()!= 0) ;i++)
	{
		if (m_Sections.at(i)=='\0')
		{
			sLeft = m_Sections.left(i); // This is a key
			m_KeyList.append(sLeft); 
			m_Sections = m_Sections.right(m_Sections.length()-i-1); // this is the rest of the list
			i=0;
			if (sLeft.upper().left(m_SectionName.length()) == m_SectionName.upper()) //Here we check the section Name
			{
				m_ListTemp.append(sLeft);

				for (uint k=0; k<=m_AttribList.size(); k++) // Each key is checked
				{
					int nbOccurence =0;
					iniFile.GetIniProfileString(sLeft, m_AttribList[k],sTmp, nbOccurence);
					m_ListTemp.append(sTmp);  
					if (nbOccurence > 1)
					{
						QString s;
						s.sprintf("%d", nbOccurence);
						m_ErrorLabel+= "Key : " + m_AttribList[k] +" is present " + s + " time for section : " + sLeft + " in the file " + m_FileName;
						bError = true;
					}
				}	
				// Each argument of the key is stocked in a 2 day Array. 
				//In fact the Array is a StringList of StringList
				m_ArrayOfList.append(m_ListTemp);
				m_ListTemp.clear();
			}
		}
	}
	if (bError) 
		return -1; 
	if (m_ArrayOfList.size() ==0)
	{
		if (iniFile.IsOpen() == true)
		{
			iniFile.Close();
		}
		else
		{
			m_ErrorLabel+=  m_FileName + " Configuration file is missing\n The CSCI Initialization is aborted";
		}
		return -1;
	}
	iniFile.Close();
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function checks an ini file composed with keys, containing distinct 
///		arguments like a list of user or a list of computers
///		This function checks each argument of the table of list m_ArrayOfList.
///		it return an error message and -1 if and argument is missing
///		return 0 if ok	
/// 
///  RETURNS: 
///		0 if successfull
///		-1 if an error occurs			
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CFiles::CheckTabFile()
{
	m_ArrayOfList.clear();
	QStringList m_ListTemp;
	m_ErrorLabel="";
	uint j=0;
	if (-1 == OpenFileAndCheckTableKey())
		return -1;
	for (uint i=0; i<m_ArrayOfList.size(); i++)
	{
		//Each Argument is compared to the Argument List
		m_ListTemp = m_ArrayOfList[i];
		bool bEmpty = false;
		for (j=0; j<=m_AttribList.size(); j++)
		{
			bEmpty = (m_ListTemp[j] == "");
			if (bEmpty)
			{
				m_ErrorLabel+="The : " + m_KeyList[i] + " / " + m_AttribList[j-1] + " key is missing\n in the ";
				m_ErrorLabel+= m_FileName + " configuration file. \n The CSCI initialization is aborted\n";
				return -1;
			} 
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function checks the Argument list : ArgList
///		takes each element of this list and looks if each element of the
///		attribute list (AttribList) is present
/// 
///  RETURNS: 
///		0 if successfull
///		-1 if an error occurs			
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CFiles::CheckArgList(CArray<QString, QString> * ArgList)
{
	QString sTmp;
	uint j=0;
	bool bKeyFound = false;
	for (uint i=0; i<=m_KeyList.size(); i++)
	{  
		if (m_KeyList[i] == m_SectionName)
		{ 
			bKeyFound = true;
			bool bEmpty = false;
			CIniFile iniFile(m_FileName);

			for (j=0; j<m_AttribList.size(); j++)
			{ 
				int nbOccur =0;
				iniFile.GetIniProfileString(m_SectionName, m_AttribList[j],sTmp, nbOccur);
				ArgList->Add(sTmp);
				bEmpty = bEmpty || (sTmp == "");
				if (bEmpty)
				{
					bEmpty = false;
					m_ErrorLabel+="The : " + m_KeyList[i] + " / " + m_AttribList[j] + " key is missing\n in the ";
					m_ErrorLabel+= m_FileName + " configuration file. \n The CSCI initialization is aborted\n";
					return -1;
				}
				if (nbOccur >1)
                                {
                                        QString S;
                                        S.sprintf("%d", nbOccur);
                                        m_ErrorLabel+="The : " + m_KeyList[i] + " / " + m_AttribList[j] + " key is present " + S + " times \n in the ";
                                        m_ErrorLabel+= m_FileName + " configuration file. \n The CSCI initialization is aborted\n";
                                        return -1;
                                }

			}
			iniFile.Close();
		}
	}
	if (!bKeyFound)
	{
		m_ErrorLabel+="The " + m_SectionName + " KEY is missing\n in the ";
		m_ErrorLabel+= m_FileName + " configuration file. \n The CSCI initialization is aborted\n";
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function checks that each field of qslVerif is corresponding to
///		a field in the Array of List
/// 
///  RETURNS: 
///		0 if successfull
///		-1 if an error occurs			
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CFiles::VerifFieldFromList(QStringList qslVerif, int nPosition)
{
	QStringList m_ListTemp;
	for (uint j=0; j< m_ArrayOfList.size() ; j++)
	{
		m_ListTemp.clear();
		m_ListTemp=m_ArrayOfList[j];
		 QString sTemp = m_ListTemp[nPosition];
		bool bFind=false;
		int z = qslVerif.size();
		QString s;
		s.sprintf("%d", z);
		for (uint i=0; i<qslVerif.size() ; i++)
		{
			bFind = bFind || (qslVerif[i].upper() == sTemp.upper());
		}
		if (!bFind)
		{m_ErrorLabel+="Unknow or absent value/label : " + m_ListTemp[nPosition] + "\n";}
	}
	if (m_ErrorLabel != "")
	{
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function checks if the QString String is present in the QStringList qslList.
///		If so, it then checks if the system is configured for "Case Sensitive"	
///		It return Position -1 if the String was found. -1 if it wasn't found 
/// 
///  RETURNS: 
///		-1 if an error occurs otherwise returns the position in the qslList
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CFiles::VerifFieldInList( QStringList qslList,QString String )
{
	bool bFound=false;
	uint j=0;
	int nRet = -1;
	for (; j< qslList.size() ; ++j)
	{
		bFound = bFound || (String == qslList[j]);
		if (bFound)
		{
			bFound = false;
			 nRet = j;
		}
	}
	return nRet;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function checks if the QString qsVerif is present in the array 
///		m_ArrayOfList at the column nPosition
///		It return 0 if the String was found. -1 if it wasn't found
/// 
///  RETURNS: 
///		0 if the String is found otherwise -1
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CFiles::VerifFieldPresent( QString qsVerif, int nPosition )
{
	QStringList m_ListTemp;
	bool bFound=false;
	uint j=0;
	for (; (j< m_ArrayOfList.size()) && (!bFound); j++)
	{
		m_ListTemp.clear();
		m_ListTemp=m_ArrayOfList[j];

		bFound = bFound || (qsVerif.upper() == m_ListTemp[nPosition].upper());
	}
	if (bFound) 
		return j-1;
	else 
		return -1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function checks if two fields are the same in a same column: nPosition
///		For example it allows to check if a login is present twice with a different password.
///		if a double is found, the system adds string in m_ErrorLabel and return -1. 
///		The Error Label is then checked by the upper functions.
///		This function can be used without using the view of the CSignIn Class
/// 
///  RETURNS: 
///		-1 if an error occurs otherwise 0
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CFiles::CheckDoubleAndEmpty( int nPosition )
{
	QStringList m_ListTemp;
	QStringList m_ListValue, m_ListCompare;
	bool bDouble = false;
	bool bError = false;
	uint j=0;
	for (; j< m_ArrayOfList.size(); j++)
	{
		m_ListTemp=m_ArrayOfList[j];
		m_ListValue.append(m_ListTemp[nPosition]); // We create a list of all parameters to be compared
		m_ListTemp.clear();
	}
	m_ListCompare=m_ListValue;
	QString t="";
	QString u="";
	QString sDoubleName ="";
	for (j=0; j<m_ListValue.size(); j++)
	{
		if (m_ListCompare[j].upper() == "")
		{
			m_ErrorLabel+= "Field " + m_AttribList[nPosition -1] + " is empty in the file : "+ m_FileName + "\nThe CSCI initialization is aborted";
			bError = true;
			return -1;
		}
		for (uint i=1; i<m_ListCompare.size()-j; i++)
		{
			t = m_ListValue[j].upper() ;
			u = m_ListCompare[i+j].upper();
			bDouble = bDouble || (t == u);
			if (bDouble)
			{
				sDoubleName = t;
				break;
			}
 		 }
		if (bDouble)
		{
			m_ErrorLabel+="Two USERS are the sames : " + sDoubleName + " in the file : "+ m_FileName + "\nThe CSCI initialization is aborted";
			bDouble = false;
			bError = true;
			return -1;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This Function is an overloaded of OpenAndLoadFile(QString), using the member 
///		variable m_FileName.
/// 
///  RETURNS: 
///		1 if succesfull otherwise 0
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CFiles::OpenAndLoadFile()
{
	return OpenAndLoadFile(m_FileName);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function opens a file and loads a key table 
///		and also an AttribTable (2Dimension table)
/// 
///  RETURNS: 
///		-1 if an error occurs during the analyze of the file content.
///		0 if everything append correctly.
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CFiles::OpenAndLoadFile(QString strFileName)
{
	m_ArrayOfList.clear();
	QStringList m_ListTemp;
	CIniFile iniFile(strFileName);
	m_ErrorLabel="";
	QString sTmp, sLeft;
	bool bDouble;
	iniFile.GetIniProfileSectionNames(m_Sections, bDouble);
	if (bDouble)
	{ 
	       m_ErrorLabel = " The Section : " + m_Sections.left(m_Sections.length()-1) + " is present twice in the file : " + strFileName + ". The CSCI initialization is aborted";
		  return -2;
	}
	// all sections names are now in the String m_Sections, separated by a '\0'
	m_KeyList.clear();
	uint i=0;
	for (;(i<m_Sections.length() && m_Sections.length()!= 0) ;i++)

	{
		if (m_Sections.at(i)=='\0')
		{
			sLeft = m_Sections.left(i); // This is a key
			m_KeyList.append(sLeft); 
			m_Sections = m_Sections.right(m_Sections.length()-i-1); // this is the rest of the list
			i=0;
			if( sLeft.upper().left(m_SectionName.length()) == m_SectionName.upper()) //Here we check the section Name
			{
				m_ListTemp.append(sLeft);
				for (uint k=0; k<=m_AttribList.size(); k++) // Each key is checked
				{
					iniFile.GetIniProfileString(sLeft, m_AttribList[k],sTmp);
					m_ListTemp.append(sTmp);  
				}	
				// Each argument of the key is stocked in a 2 day Array. 
				//In fact the Array is a StringList of StringList
        m_ArrayOfList.append(m_ListTemp);
				m_ListTemp.clear();
			}
		}
	}//for
	if (m_ArrayOfList.size() ==0)
	{
	 if (iniFile.IsOpen() == false)
	 {
	       m_ErrorLabel = strFileName + " Configuration file is missing\nThe CSCI initialization is aborted";
		  return -2;
	 }

    else
    {
  		m_ErrorLabel+="No field has been found in file :" + strFileName + "\n";
  		// If no field is found, an error message occurred.
  		iniFile.Close();
    }
		return -1;
	}
	for (i=0; i<m_ArrayOfList.size(); i++)
	{
		//Each Argument is compared to the Argument List
		m_ListTemp = m_ArrayOfList[i];
		bool bEmpty = false;
		for (uint j=1; j<m_AttribList.size(); j++)
		{
			bEmpty = bEmpty || (m_ListTemp[j] == "");
		}
		if (bEmpty)
		{
			m_ErrorLabel+="Error loading KEY : " + m_ListTemp[0] + "\n";
		} 
	}
	if (m_ErrorLabel != "")
	{
		return -1;
	}
	return 0;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function writes a key in a ini file
/// 
///  RETURNS: 
///		-1 if an error occurs otherwise 0
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
int CFiles::WriteKey(QString SecName,QString KeyName,QString String)
{
	CIniFile iniFile(m_FileName);

	if (iniFile.WriteIniProfileString(SecName, KeyName, String))
	{
		iniFile.Close();
		return 0;
	}
	iniFile.Close();
	return -1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function is an accessor
/// 
///  RETURNS: 
///		void
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
void CFiles::SetFileName(QString FileName)
{
	m_FileName = FileName;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function is an accessor
/// 
///  RETURNS: 
///		void
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
void CFiles::SetSectionName(QString SectionName)
{
	m_SectionName = SectionName;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function is an accessor
/// 
///  RETURNS: 
///		void
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
void CFiles::SetAttribList(QStringList sAttribList)
{
	m_AttribList = sAttribList;
}
	
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function is an accessor
/// 
///  RETURNS: 
///		the error label
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
QString CFiles::GetErrorLabel()
{
	return m_ErrorLabel;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function is an accessor
/// 
///  RETURNS: 
///		the file name
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
QString CFiles::GetFileName()
{
	return m_FileName;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function is an accessor
/// 
///  RETURNS: 
///		the section name
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
QString CFiles::GetSectionName()
{

	return m_SectionName;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function is an accessor
/// 
///  RETURNS: 
///		void
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
void CFiles::SetErrorLabel(QString ErrorLabel)
{
	m_ErrorLabel += ErrorLabel;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function is an accessor
/// 
///  RETURNS: 
///		a list of values
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
QValueList<QStringList> CFiles::GetArrayOfList()
{
	return m_ArrayOfList;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function is an accessor
/// 
///  RETURNS: 
///		void
///		             
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------	
void CFiles::ClearAttribList()
{
	m_AttribList.clear();
}

//--------------------------------< THE END >----------------------------------

