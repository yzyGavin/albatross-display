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
//  FILENAME: Interface.cpp
//  TYPE    : c++ code-file 
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
//	  x
//
//  BUILD INFORMATION 
//    x
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

#include "StdAfx.h"
#if defined (Q_WS_WIN)
#include "Ws2Tcpip.h"
#endif
#include "./CommonFiles/IniFile.h"
#include "Interface.h"
#if (defined Q_WS_X11)
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <net/if.h>
#include <unistd.h>
#endif

//-GLOBAL DATA-----------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///    Adds the hostname at the begining of a buffer.
///    
/// 
///  RETURNS: a new buffer with the updated message. This buffer must be 
///  destroyed by the calling method.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
char *InsertHostName(char* buffer, int length, int& newlength)
{
	char str[257];
	gethostname(str,257);
	int lenhost=strlen(str);
	newlength=1 + lenhost + length;
	char* newbuffer = new char[newlength];
	newbuffer[0]=lenhost;
	memcpy(&newbuffer[1], str, lenhost);
	memcpy(&newbuffer[1+lenhost], buffer, length);
	return newbuffer;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///    This method fills the in_addr structure with the string in attribute. 
///    the string can have two format :
///      - ip address as xxx.xxx.xxx.xx
///      - hostname
///    
/// 
///  RETURNS: true if the ip adress is valid. false otherwise 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool DecodeAddr(in_addr& addr, CString str, QString FieldName, QString& Error)
{
	addr.s_addr=inet_addr(str);
	if (addr.s_addr==INADDR_NONE)
	{
		hostent* pHost=gethostbyname(str);
		if (pHost)
			addr.s_addr=*((DWORD*)pHost->h_addr_list[0]);
	}
	if (FieldName.length() && addr.s_addr==INADDR_NONE)
	{
		Error="Invalid address ("+str+") for "+FieldName;
		return false;
	}
	return true;

}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///    This method fills the port number with a string put in argument. 
///    the string can have two format :
///      - port number as xxxxx
///      - service name
///    
/// 
///  RETURNS: true if the port is valid, false otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool DecodePort(unsigned short& port, CString str,QString FieldName, QString& Error)
{
	port=0;
	if (str.isNumeric())
		port=htons(atoi(str.ascii()));
	else
	{
		servent* pServ=getservbyname(str,"UDP");
		if (pServ)
			port=pServ->s_port;
	}
	if (FieldName.length() && port==0)
	{
		Error="Invalid port ("+str+") for "+FieldName;
		return false;
	}
	return true;
}

//-LOCAL DATA------------------------------------------------------------------
 

//-FUNCTION PROTOTYPES---------------------------------------------------------


//-START OF CODE---------------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class INTERFACELAN
/// this class contains no method and is used to define an Interface
/// 
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		destructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
INTERFACELAN::~INTERFACELAN()
{
}



//-----------------------------------------------------------------------------
//
/// \class CAsterixSource
/// this class manages UDP sockets for reception or sending of messages over ip network
/// 
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		constructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CAsterixSource::CAsterixSource()
: m_pSendSocket(NULL), m_pRcvSocket(NULL), m_pInterface(NULL), m_pObject(NULL), m_TimeOut(-1)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		copy constructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CAsterixSource::CAsterixSource(CAsterixSource& pAsterixSource)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		destructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CAsterixSource::~CAsterixSource()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		returns the status of the network interface.
/// 
///  RETURNS: false if nothing received since timeout seconds, true otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CAsterixSource::GetState()
{
	if (m_pRcvSocket)
		return !m_pRcvSocket->IsTimeOut();
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		registers the object that will handle messages received on the socket
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CAsterixSource::SetMngr(CObject* pobj)
{
	m_pObject = pobj;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		creates a sending UDP socket
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CAsterixSource::CreateSndSocket()
{
	if (m_pInterface)
	{
		m_pSendSocket = new CAsyncSocket;
		m_pSendSocket->Create(0,SOCK_DGRAM);
		in_addr int_addr = m_pInterface->m_Addr;
		setsockopt((SOCKET)*m_pSendSocket, IPPROTO_IP, IP_MULTICAST_IF,(char *)&int_addr, sizeof(int_addr));
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		sends a buffer on the socket, must call CreateSndSocket before.
/// 
///  RETURNS: the number of octet sent, -1 if nothing is sent.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CAsterixSource::SendTo(const void* lpBuf, int snd)
{
	if (m_pSendSocket)
	{
		sockaddr_in m_OpAddr=GetAddr();
		return m_pSendSocket->SendTo(lpBuf,snd,(sockaddr*)&m_OpAddr,sizeof(sockaddr_in));
	}
	else
		return -1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		creates a receiving UDP socket.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CAsterixSource::CreateRcvSocket()
{
	m_pRcvSocket = new CRcvSocket();
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	if (m_pInterface)
		addr.sin_addr=m_pInterface->m_Addr;
	else
		addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	addr.sin_port=m_Port;
	m_pRcvSocket->Create(m_pObject, m_TimeOut, addr, m_Addr, m_Serid);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		closes the receiving UDP socket.
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CAsterixSource::Close()
{
	if (m_pRcvSocket)
		m_pRcvSocket->Close();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		copies the socket parameter from another object CAsterixSource
/// 
///  RETURNS: a reference on this object
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CAsterixSource& CAsterixSource::operator=( const CAsterixSource & src)
{
	if (&src != this)
	{
		m_Name = src.m_Name;
		m_Addr = src.m_Addr;
		m_Port = src.m_Port;
		m_pInterface = src.m_pInterface;
		return *this;
	}
	else
		return *this;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		returns the ip address and the port of the socket.
/// 
///  RETURNS: sockaddr_in structure giving ip address and the port number
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
sockaddr_in CAsterixSource::GetAddr()
{
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_addr=m_Addr;
	addr.sin_port=m_Port;
	return addr;
}


//-----------------------------------------------------------------------------
//
/// \class CInterfaceList
/// this class manages the list of available interfaces
/// 
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///    destructor
/// 
///  RETURNS: NONE
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CInterfaceList::~CInterfaceList()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
//    Initializes the interface list from the data set and retrieves the actual 
///   configuration of the network coupling devices on the computer.
/// 
///  RETURNS: true if no error. false otherwise. 
///          The attribute Error returns the description of the error, if any.
///
///  REQUIREMENT ID: CIAsdFctIniGen075 
//-----------------------------------------------------------------------------
bool CInterfaceList::Read(CIniFile& IniFile, QString &Error)
{
	INTERFACELAN CurInterface;
	CString KeyRead;
	CString KeyName, CurArg;
	for (int i=1;;i++)
	{
		KeyName.sprintf("INT%d",i);
		int len=IniFile.GetIniProfileString("INTERFACES",KeyName,KeyRead);
		if(i==1&&!len)
		{
			Error="No INT1 found in version.ini [INTERFACES]. Stopping ASD.";
			return false;
		}
		if (!len)
			break;
		int nbarg=KeyRead.GetNbrOfArgument('|');
		if (nbarg==2)
		{
			KeyRead.GetArgumentNb(1,'|',CurArg);
			CurInterface.m_Name=CurArg;
			KeyRead.GetArgumentNb(2,'|',CurArg);
			int ok=DecodeAddr(CurInterface.m_NetAddr, CurArg, "interface "+ CurInterface.m_Name, Error);
			if (!ok) 
				return false;
			m_Interfaces.Add(CurInterface);
		}
	}

	CAsyncSocket InterfaceSocket;
	InterfaceSocket.Create();
#if (defined Q_WS_WIN)
	DWORD interfacenb=0;
	INTERFACE_INFO Info[10];
	WSAIoctl((SOCKET)InterfaceSocket,SIO_GET_INTERFACE_LIST,NULL,0,&Info,sizeof(&Info),&interfacenb,NULL,NULL);
	interfacenb/=sizeof(INTERFACE_INFO);
	in_addr pAddrLocal,pAddrMask;
	for (UINT inf=0;inf<interfacenb;inf++)
	{
		pAddrLocal = Info[inf].iiAddress.AddressIn.sin_addr;
		pAddrMask  = Info[inf].iiNetmask.AddressIn.sin_addr;
		for (int inf=0;inf<m_Interfaces.GetSize();inf++)
		{
			if ((pAddrLocal.S_un.S_addr&pAddrMask.S_un.S_addr)==m_Interfaces[inf].m_NetAddr.S_un.S_addr)
				m_Interfaces[inf].m_Addr=pAddrLocal;
		}
	}
#endif
#if (defined Q_WS_X11)
	int n;
	struct ifconf ifc;
	char buf[4096];
	struct ifreq *ifr = NULL;
	in_addr AddrLocal,AddrMask;
	memset(&ifc,0,sizeof(ifc));
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if (InterfaceSocket.IOCtl(SIOCGIFCONF,  (DWORD*)&ifc) >= 0) 
	{
		ifr = ifc.ifc_req;
		n=ifc.ifc_len/sizeof(struct ifreq);
		n--;
		while (n >= 0)
		{
#ifdef ENABLE_QDEBUG
			qDebug("Printing name: %s" ,ifr->ifr_name);
#endif
			
			if(InterfaceSocket.IOCtl( SIOCGIFADDR, (DWORD *)ifr) >= 0)
			{
				AddrLocal=*(struct in_addr *)(&ifr->ifr_addr.sa_data[2]);
#ifdef ENABLE_QDEBUG
				qDebug("Local addr: %x",AddrLocal.s_addr);
#endif
			}
			
			if(InterfaceSocket.IOCtl( SIOCGIFNETMASK, (DWORD *)ifr) >= 0)
			{
				AddrMask=*(struct in_addr *)(&ifr->ifr_netmask.sa_data[2] );
#ifdef ENABLE_QDEBUG
				qDebug("Local mask: %x" ,AddrMask.s_addr);
#endif
			}
			for (int inf=0;inf<m_Interfaces.GetSize();inf++)
			{
				DWORD tst=(AddrLocal.s_addr)&(AddrMask.s_addr);
#ifdef ENABLE_QDEBUG
				qDebug("Local Network: %x and Interface %x" ,(int)tst,m_Interfaces[inf].m_NetAddr.s_addr);
#endif
				if ((AddrLocal.s_addr&AddrMask.s_addr)==m_Interfaces[inf].m_NetAddr.s_addr)
				{
					m_Interfaces[inf].m_Addr=AddrLocal;
#ifdef ENABLE_QDEBUG
					qDebug("Printing name: %s" ,ifr->ifr_name);
#endif
				}
			}
			ifr++;
			n--;
		}
	}
#endif

	return true;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///    returns an interface object corresponding to the name of the interface
///		given in attribute.
/// 
///  RETURNS: the interface object if found, NULL otherwise.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
INTERFACELAN* CInterfaceList::Get(QString name)
{
	for(int i = 0; i < m_Interfaces.GetSize(); i++)
		if (m_Interfaces[i].m_Name == name)
			return &m_Interfaces[i];
	return NULL;
}


//--------------------------------< THE END >----------------------------------

