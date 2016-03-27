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
//  FILENAME: ClientSocket.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the CClientSocket class.
//	  This class allows to create a TCP connection between the ASD and the CCS
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
#include "View.h"
#include "ClientSocket.h"
#include "FlightPlan.h"
#if defined (Q_WS_X11)
#include <sys/ioctl.h>
#define WSAEWOULDBLOCK EWOULDBLOCK
#endif
#if defined (Q_WS_WIN)
#include "Ws2Tcpip.h"
#endif
#include "AsdApp.h"
#include "AsdMenu.h"


//-GLOBAL DATA-----------------------------------------------------------------
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CClientSocket
///		This class allows to create a TCP connection between the ASD and the CCS
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
CClientSocket::CClientSocket()
{
	m_State = notconnected;

#ifdef ENABLE_QDEBUG
	qDebug("CClientSocket::CClientSocket m_State = %d", m_State);
#endif
	
	m_pRemBuff=NULL;
	m_RemCount=0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the copy constructor..
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CClientSocket::CClientSocket(CClientSocket& pClientSocket)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CClientSocket::~CClientSocket()
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method notifies this socket that there is data in the buffer 
///		Argument: nErrorCode is the most recent error on a socket. The following 
///				  error codes apply to the OnReceive member function: 
///					0: The function executed successfully.
///					WSAENETDOWN: The Windows Sockets implementation detected 
///								 that the network subsystem failed. 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CClientSocket::OnReceive(int nErrorCode) 
{	
	CAsyncSocket::OnReceive(nErrorCode);

	BYTE* pBuff=NULL;
	int recv,size,totalsize,curpos=m_RemCount;
	unsigned long nSize;

	// Get Buffer Size
	IOCtl(FIONREAD,&nSize);
	pBuff=new BYTE[nSize+m_RemCount];

	// Restore the data from the m_pRemBuff table
	int i;
	for (i=0;i<m_RemCount;i++)
		pBuff[i]=m_pRemBuff[i];

	totalsize=nSize+m_RemCount;
	
	delete []m_pRemBuff;

	m_pRemBuff=NULL;
	m_RemCount=0;

	while (1) 
	{
		// Retrieve the data from the socket
		recv = Receive(&pBuff[curpos],totalsize-curpos);
		if (recv==-1)
		{
			m_State = notconnected;

#ifdef ENABLE_QDEBUG
			qDebug("CClientSocket::OnReceive m_State = %d", m_State);
#endif

			AsyncSelect(0);
			Close();

			// Send a disconnect message to the parent object
			if (m_pObject)
				m_pObject->OnReceiveMsg(NULL,-1);

			delete []pBuff;

			return;
		}
		curpos+=recv;

		if (curpos==totalsize)
			break;
	}
	curpos=0;

	while (1)
	{
		size=0;

		if (totalsize-curpos>=16)
		{
			for (i=11;i<16;i++)
			{
				size*=10;
				size += pBuff[curpos+i]-'0';
			}
		}

		if ((totalsize-curpos>=16) && (totalsize-curpos>=size+18))
		{// Send Data to the parent object
			if (m_pObject)
				m_pObject->OnReceiveMsg(&pBuff[curpos],size+18);

			curpos+=size+18;
			if (curpos==totalsize)
				break;
		}
		else
		{// Save Data to the m_pRemBuff table
			m_pRemBuff=new BYTE[totalsize-curpos];
			m_RemCount=totalsize-curpos;

			for (i=0;i<m_RemCount;i++)
				m_pRemBuff[i]=pBuff[curpos+i];

			break;
		}
	}

	delete []pBuff;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method notifies this socket that the connected socket is closed 
///		by its process
///		Argument: nErrorCode is the most recent error on a socket. The following 
///				  error codes apply to the OnClose member function: 
///					0: The function executed successfully.
///					WSAENETDOWN: The Windows Sockets implementation detected that the network subsystem failed.
///					WSAECONNRESET: The connection was reset by the remote side.
///					WSAECONNABORTED: The connection was aborted due to timeout or other failure 
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CClientSocket::OnClose(int nErrorCode) 
{
	CAsyncSocket::OnClose(nErrorCode);
	
	if (m_State != notconnected)
	{
	    m_State = notconnected;

#ifdef ENABLE_QDEBUG
	    qDebug("CClientSocket::OnClose m_State = %d", m_State);
#endif
		
	    Close();

		// Send a disconnect message to the parent object
	    if (m_pObject)
	      m_pObject->OnReceiveMsg(NULL,-1);
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method connects the socket to the opeaddr argument
///		Argument: opeaddr is the sockaddr_in structure
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CClientSocket::ConnectToOpe(sockaddr_in opeaddr)
{
	m_Addr=opeaddr;
	
	if (m_State==notconnected)
	{
		int res=Connect((sockaddr*)&m_Addr,sizeof(sockaddr_in));
		DWORD err=GetLastError();
		
		if ((res) || (err==WSAEWOULDBLOCK))
		{
			m_State=connecting;

#ifdef ENABLE_QDEBUG
			qDebug("CClientSocket::ConnectToOpe m_State = %d", m_State);
#endif

		}
		else
		{

#ifdef ENABLE_QDEBUG
			qDebug("CClientSocket::ConnectToOpe m_State = %d because err = %ld", m_State, err);
#endif

		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method notifies this connecting socket that its connection attempt 
///		is completed, whether successfully or in error
///		Argument: nErrorCode is the most recent error on a socket. The following 
///				  error codes apply to the OnConnect member function: 
///					0: The function executed successfully.
///					WSAEADDRINUSE: The specified address is already in use.
///					WSAEADDRNOTAVAIL: The specified address is not available from the local machine.
///					WSAEAFNOSUPPORT: Addresses in the specified family cannot be used with this socket.
///					WSAECONNREFUSED: The attempt to connect was forcefully rejected.
///					WSAEDESTADDRREQ: A destination address is required.
///					WSAEFAULT: The lpSockAddrLen argument is incorrect.
///					WSAEINVAL: The socket is already bound to an address.
///					WSAEISCONN: The socket is already connected.
///					WSAEMFILE: No more file descriptors are available.
///					WSAENETUNREACH: The network cannot be reached from this host at this time.
///					WSAENOBUFS: No buffer space is available. The socket cannot be connected.
///					WSAENOTCONN: The socket is not connected.
///					WSAENOTSOCK: The descriptor is a file, not a socket.
///					WSAETIMEDOUT: The attempt to connect timed out without establishing a connection. 
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CClientSocket::OnConnect(int nErrorCode) 
{
	CAsyncSocket::OnConnect(nErrorCode);

#ifdef ENABLE_QDEBUG
	qDebug("CClientSocket::OnConnect receivednErrorCode = %d", nErrorCode);
#endif

	if (nErrorCode!=0)
	{
		m_State = notconnected;

#ifdef ENABLE_QDEBUG
		qDebug("CClientSocket::OnConnect 1 m_State = %d", m_State);
#endif

		// Send a disconnect message to the parent object
		if (m_pObject)
			m_pObject->OnReceiveMsg(NULL,-1);
	}
	else
	{
		m_State = connected;

#ifdef ENABLE_QDEBUG
		qDebug("CClientSocket::OnConnect 2 m_State = %d", m_State);
#endif

		// Send a disconnect message to the parent object
		if (m_pObject)
			m_pObject->OnReceiveMsg(NULL,0);
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sends an heart beat
///		Argument1: DevNum is the console number
///		Argument2: seqnumbr is the sequence number
///
///  RETURNS:
///		returns -1
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CClientSocket::SendHeartBeat(int DevNum, int seqnumbr)
{
	int snd;
	if (m_State==connected)
	{
		QString msg;
		msg.sprintf(" 3%04d15%03d000000 ",seqnumbr,DevNum);
		msg[0]=1;
		msg[16]=2;
		msg.at(msg.length()-1)=3;
      		snd = CAsyncSocket::Send(msg, msg.length());

#ifdef ENABLE_QDEBUG
		qDebug("Heart beat sent %d characters (%s) %d", snd, msg.ascii(),msg.length() );
#endif

		return snd;//snd;
	}
	return -1;
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
void CClientSocket::Close()
{
	m_State=notconnected;

#ifdef ENABLE_QDEBUG
	qDebug("CClientSocket::Close m_State = %d", m_State);
#endif

	CAsyncSocket::Close();
}

//--------------------------------< THE END >----------------------------------

