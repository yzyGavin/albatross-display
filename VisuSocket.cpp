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
//  FILENAME: VisuSocket.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CRcvSocket which allows to create a socket for 
//		reception.
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

#include "StdAfx.h"
#include "AsyncSocket.h"
#include "VisuSocket.h"
#include "AsdApp.h"

#if defined (Q_WS_WIN)
#include "Ws2Tcpip.h"
#endif

#if defined (Q_WS_X11)
#include <arpa/inet.h>
#endif

//-GLOBAL DATA-----------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CWSStatusTimerID 1051
#define CWSStatusTimerVAL 10000


//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CRcvSocket
///		This class allows to create a socket for reception.
///		It manages a timeout on the the received data which indicates 
///		a status of this data.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CRcvSocket::CRcvSocket() : m_TimeOut(-1), m_bStartTimeOut(false)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the copy constructor.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CRcvSocket::CRcvSocket(CRcvSocket& pRcvSocket)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor.
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CRcvSocket::~CRcvSocket()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks the received data status.
/// 
///  RETURNS: 
///		true if the last data has been received less than 
///		twice the timeout, otherwise false.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
bool CRcvSocket::IsTimeOut()
{
	if (m_TimeOut == -1)
		return false;
	
	if (!m_bStartTimeOut)
		return true;
	
	// Get the current time
	QDateTime now = QDateTime::currentDateTime();
	
	// Compare the current time, the last received data time and the timeout
	if (now.toTime_t() - m_LastReceived.toTime_t() > 2 * m_TimeOut)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method notifies there is data in the buffer 
///		Argument: nErrorCode is the most recent error on a socket. The following 
///				  error codes apply to the OnReceive member function: 
///					0: The function is executed successfully.
///					WSAENETDOWN: The Window Sockets implementation detects 
///								 that the network subsystem failed. 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CRcvSocket::OnReceive(int nErrorCode) 
{
	CAsyncSocket::OnReceive(nErrorCode);
	
	// Update the received data time
	m_LastReceived = QDateTime::currentDateTime();
	m_bStartTimeOut = true;
	
	// retrieve the data
	BYTE buffer[20000];
	int len,sizeaddr=sizeof(sockaddr_in);
	sockaddr_in addr;
	len=ReceiveFrom(buffer,20000,(sockaddr*)&addr,&sizeaddr);
	
	// re-send to the object parent
	if (m_pRcvObject)
		m_pRcvObject->OnReceiveMsg(buffer,len,&addr,m_SocketId);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates the socket
///		Argument1: pObject is a pointer to the CObject object which is saved.
///				   The received data is re-sent via this saved object.
///		Argument2: timeout
///		Argument3: addr is a sockaddr_in structure
///		Argument4: mcastaddr is a in_addr structure
///		Argument5: socketid is a socket ID which is saved and re-sent to 
///				   the parent object via the OnReceive method.
///
///  RETURNS:
///		0 by default
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CRcvSocket::Create(CObject*pObject,int timeout, sockaddr_in addr,in_addr mcastaddr,int socketid)
{
	m_bStartTimeOut = false;
	m_Addr=addr;
	m_SocketId=socketid;
	
	if (mcastaddr.s_addr!=INADDR_NONE)
	{
		addr.sin_addr.s_addr=0;
	}
	
	// Save parent object
	m_pRcvObject=pObject;
	
	Socket(SOCK_DGRAM,FD_READ);
	int ret=Bind((sockaddr*)&addr,sizeof(sockaddr_in));
	DWORD err=GetLastError();
	BOOL arg=TRUE;
	
	if ((ret==0) && (err==10048))
	{
		ret=SetSockOpt(SO_REUSEADDR,&arg,1);
		ret=Bind((sockaddr*)&addr,sizeof(sockaddr_in));
		err=GetLastError();
	}
	
	if (mcastaddr.s_addr!=INADDR_NONE)
	{
		ip_mreq mrMReq;
		mrMReq.imr_interface=m_Addr.sin_addr;
		mrMReq.imr_multiaddr=mcastaddr;
		ret=setsockopt((SOCKET) *this, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mrMReq, sizeof(mrMReq));
		err=GetLastError();
	}
	
	QString str;
	QString str1=inet_ntoa(mcastaddr);
	str.sprintf("Opening Receive socket (Datagram)  %d Port=%.5d Address=%s mcastaddr %s",(SOCKET)*this,htons(addr.sin_port),inet_ntoa(m_Addr.sin_addr),str1.ascii());
	CAsdApp::GetApp()->WriteLogMsg(str,LogInformation);
	
	m_TimeOut = timeout;
	m_LastReceived = QDateTime::currentDateTime();
	
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method closes the socket
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CRcvSocket::Close()
{
	QString str;
	str.sprintf("Closing Receive socket (Datagram)  %d Port=%5d",(SOCKET) *this,htons(m_Addr.sin_port));
	CAsdApp::GetApp()->WriteLogMsg(str,LogInformation);
	CAsyncSocket::Close();
	m_bStartTimeOut = false;
}

//--------------------------------< THE END >----------------------------------
