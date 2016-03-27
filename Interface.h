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
//  FILENAME: Interface.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CAsterixSource and CInterfaceList.
//		Those classes allows to create a socket for reception, and to set the 
//		list of available communication interfaces.
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

#if !defined(AFX_INTERFACE__INCLUDED_)
#define AFX_INTERFACE__INCLUDED_

//-INCLUDE FILES---------------------------------------------------------------

#include "VisuSocket.h"
#include "String.h"

//-GLOBAL DATA-----------------------------------------------------------------

char *InsertHostName(char* buffer, int length, int& newlength);
bool DecodeAddr(in_addr& addr, CString str,QString FieldName, QString& Error);
bool DecodePort(unsigned short& port, CString str,QString FieldName, QString& Error);

class CRcvSocket;
class CObject;

//-CLASSES DEFINITIONS---------------------------------------------------------

class INTERFACELAN
{
public:
	//methods 
	virtual ~INTERFACELAN();
	//variables
	QString m_Name;
	in_addr m_Addr;
	in_addr m_NetAddr;
};

class CInterfaceList
{
private :
	//variables
	CArray<INTERFACELAN, INTERFACELAN> m_Interfaces;
public :
	//methods
	virtual ~CInterfaceList();
	bool Read(CIniFile& IniFile, QString &Error);
	INTERFACELAN* Get(QString name);
};

class CAsterixSource
{
	friend class CCSSource;
	friend class CWeatherMngr;
	friend class CTrackServer;
	friend class CTrackMngr;

public:
	CAsterixSource();
	CAsterixSource(CAsterixSource& pAsterixSource);	
	virtual ~CAsterixSource();
	CAsterixSource &operator=( const CAsterixSource & );


protected:
		

private:

	//variable
	in_addr m_Addr;
	unsigned short m_Port;	
	int m_TimeOut;
	char m_Serid;
	QString m_Name;
	unsigned short m_SendPort;

	//pointers
	INTERFACELAN* m_pInterface;
	CRcvSocket   *m_pRcvSocket;

	//methods		
	void SetMngr(CObject* pobj);
	void CreateRcvSocket();
	void CreateSndSocket();
	int SendTo(const void* lpBuf, int snd);
	void Close();
	bool GetState();

	//pointers	
	CAsyncSocket *m_pSendSocket; 	
	CObject* m_pObject;

	//methods
	void SetMngr();
	sockaddr_in GetAddr();	
};

#endif

//--------------------------------< THE END >----------------------------------
