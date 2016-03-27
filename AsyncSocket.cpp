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
//  FILENAME: AsynSocket.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file manages the asynchron sockets.
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

#if (defined Q_WS_X11)
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif


//-GLOBAL DATA-----------------------------------------------------------------
#if (defined Q_WS_WIN)
	#define socklen_t int
#endif

#define WM_SOCKET_NOTIFY    0x0373

CArray<CAsyncSocket*,CAsyncSocket*> m_SocketTable;


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CAsyncSocket
///		This class manages the asynchron sockets.
///		
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor:
/// 
///  RETURNS:
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CAsyncSocket::CAsyncSocket()
{
	m_hSocket=INVALID_SOCKET;
	m_pReadNotify=NULL;
	m_pWriteNotify=NULL;
	m_IsConnecting=false;
	m_IsListening=false;
	m_SocketType=0;
	m_Mask=0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates and attaches the socket
///		Argument1: nSocketType must be equal to SOCK_STREAM (TCP) or SOCK_DGRAM (UDP).
///		Argument2: lEvent is a bitmask which specifies a combination of network
///				   events in which the application is interested:
///					FD_READ   Want to receive notification of readiness for reading.
///					FD_WRITE   Want to receive notification of readiness for writing.
///					FD_OOB   Want to receive notification of the arrival of out-of-band data.
///					FD_ACCEPT   Want to /receive notification of incoming connections.
///					FD_CONNECT   Want to receive notification of completed connection.
///					FD_CLOSE   Want to receive notification of socket closure. 
///		Argument3: nProtocolType is a particular protocol to be used with 
///				   the socket that is specific to the indicated address family.
///				   It is equal to 0 by default.
///		Argument4: nAddressFormat is an address family specification
///				   It is equal to PF_INET by default.
///
///  RETURNS:
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::Socket(int nSocketType, long lEvent, int nProtocolType, int nAddressFormat)
{
	SOCKET hSocket=::socket(nAddressFormat,nSocketType,nProtocolType);
	if (hSocket==INVALID_SOCKET)
		return FALSE;
	m_SocketType=nSocketType;
	return  Attach(hSocket,lEvent);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates the socket
///		Argument1: nSocketPort is a port to be used with the socket, 
///				   or 0 if you want Windows Sockets to select a port
///		Argument2: nSocketType must be equal to SOCK_STREAM (TCP) or SOCK_DGRAM (UDP).
///		Argument3: lEvent is a bitmask which specifies a combination of network
///				   events in which the application is interested:
///					FD_READ   Want to receive notification of readiness for reading.
///					FD_WRITE   Want to receive notification of readiness for writing.
///					FD_OOB   Want to receive notification of the arrival of out-of-band data.
///					FD_ACCEPT   Want to /receive notification of incoming connections.
///					FD_CONNECT   Want to receive notification of completed connection.
///					FD_CLOSE   Want to receive notification of socket closure. 
///		Argument4: lpszSocketAddress is a pointer to a string containing the network 
///				   address of the connected socket, a dotted number such as "128.56.22.8"
///
///  RETURNS:
///		returns true if the socket is creating, otherwise false.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::Create(UINT nSocketPort,int nSocketType,long lEvent,LPCSTR lpszSocketAddress)
{
	if (Socket(nSocketType, lEvent))
	{
		if (Bind(nSocketPort,lpszSocketAddress))
			return TRUE;
		Close();
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the operator SOCKET
///
///  RETURNS:
///		SOCKET
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
// conversion operator not checked by rule checker
CAsyncSocket::operator SOCKET() const
{
	return m_hSocket;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method attaches the hSocket handle to an CAsyncSocket object. 
///		The SOCKET handle is stored in the object’s m_hSocket data member
///
///  RETURNS:
///		returns true if the socket is attached, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::Attach(SOCKET hSocket,long lEvent)
{
	m_hSocket=hSocket;
	m_SocketTable.Add(this);
	return AsyncSelect(lEvent);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method detaches the SOCKET handle in the m_hSocket data member 
///		from the CAsyncSocket object and set m_hSocket to NULL
///
///  RETURNS:
///		returns the SOCKET
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
SOCKET CAsyncSocket::Detach()
{
	SOCKET hSocket = m_hSocket;
	for (int i=0;i<m_SocketTable.GetSize();i++)
	{
		if (m_SocketTable[i]==this)
		{
			m_SocketTable.RemoveAt(i);
			break;
		}
	}
	m_hSocket=INVALID_SOCKET;
	return hSocket;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the address of the peer socket to which 
///		this socket is connected
///		Argument1: lpSockAddr is a pointer to the SOCKADDR structure 
///				   that receives the name of the peer socket
///		Argument2: lpSockAddrLen is a pointer to the length of the address
///				   in lpSockAddr in bytes. On return, the lpSockAddrLen argument
///				   contains the actual size of lpSockAddr returned in bytes.
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::GetPeerName(sockaddr* lpSockAddr,int* lpSockAddrLen)
{
	return (::getpeername(m_hSocket,lpSockAddr,(socklen_t*)lpSockAddrLen)!=SOCKET_ERROR);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the address of the peer socket to which 
///		this socket is connected
///		Argument1: rPeerAddress is a reference to a QString object 
///				   that receives a dotted number IP address
///		Argument2: rPeerPort is a reference to a UINT that stores a port
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::GetPeerName(QString& rPeerAddress, UINT& rPeerPort)
{
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = GetPeerName((sockaddr*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rPeerPort = ntohs(sockAddr.sin_port);
		rPeerAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the local name for a socket
///		Argument1: lpSockAddr is a pointer to the SOCKADDR structure 
///				   that receives the address of the socket
///		Argument2: lpSockAddrLen is a pointer to the length of the address 
///				   in lpSockAddr in byte
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::GetSockName(sockaddr* lpSockAddr,int* lpSockAddrLen)
{
	return (::getsockname(m_hSocket,lpSockAddr,(socklen_t*)lpSockAddrLen)!=SOCKET_ERROR);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the local name for a socket
///		Argument1: rSocketAddress is a reference to a QString object 
///				   that receives a dotted number IP address
///		Argument2: rSocketPort is a reference to a UINT that stores a port
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::GetSockName(QString& rSocketAddress, UINT& rSocketPort)
{
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = GetSockName((sockaddr*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method requests event notification for a socket 
///		Argument1: lEvent is a bitmask which specifies a combination 
///				   of network events in which the application is interested:
///					FD_READ   Want to receive notification of readiness for reading.
///					FD_WRITE   Want to receive notification of readiness for writing.
///					FD_OOB   Want to receive notification of the arrival of out-of-band data.
///					FD_ACCEPT   Want to /receive notification of incoming connections.
///					FD_CONNECT   Want to receive notification of completed connection.
///					FD_CLOSE   Want to receive notification of socket closure.
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::AsyncSelect(long lEvent)
{
    BOOL ret;
	if (m_hSocket!=INVALID_SOCKET)
	{
		if (!m_pReadNotify)
		{
			m_pReadNotify = new QSocketNotifier(m_hSocket, QSocketNotifier::Read,this);
			ret=QObject::connect(m_pReadNotify, SIGNAL(activated(int)),this, SLOT(ReceiveData()) );
		}
		if ((!m_pWriteNotify) )
		{
			m_pWriteNotify = new QSocketNotifier(m_hSocket, QSocketNotifier::Write,this);
			ret=QObject::connect(m_pWriteNotify, SIGNAL(activated(int)),this, SLOT(SendData()) );
		}
		
		if (lEvent & (FD_READ|FD_ACCEPT))
		{
			if (!m_pReadNotify->isEnabled())
				m_pReadNotify->setEnabled(true);
			
		}
		else
		{
			if (m_pReadNotify->isEnabled())
				m_pReadNotify->setEnabled(false);
		}
		if (lEvent & (FD_WRITE|FD_CONNECT))
		{
			if (!m_pWriteNotify->isEnabled())
				m_pWriteNotify->setEnabled(true);
		}
		else
		{
			if (m_pWriteNotify->isEnabled())
				m_pWriteNotify->setEnabled(false);
		}
	}
	m_Mask=lEvent;

	return TRUE;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method accepts a connection on a socket 
///		Argument1: rConnectedSocket is a reference identifying a new socket 
///				   that is available for connection 
///		Argument2: lpSockAddr is a pointer to a SOCKADDR structure that receives 
///				   the address of the connecting socket, as known on the network
///		Argument3: lpSockAddrLen is a pointer to the length of the address
///				   in lpSockAddr in bytes
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::Accept(CAsyncSocket& rConnectedSocket,sockaddr* lpSockAddr, int* lpSockAddrLen)
{
	ASSERT(rConnectedSocket.m_hSocket == INVALID_SOCKET);

	SOCKET hTemp = accept(m_hSocket, lpSockAddr, (socklen_t*)lpSockAddrLen);

	if (hTemp == INVALID_SOCKET)
	{
		rConnectedSocket.m_hSocket = INVALID_SOCKET;
	}
	else 
	{
		rConnectedSocket.m_hSocket=hTemp;
		m_SocketTable.Add(&rConnectedSocket);
	}

	return (hTemp != INVALID_SOCKET);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method associates a local address with the socket 
///		Argument1: lpSockAddr is a pointer to a SOCKADDR structure that contains 
///				   the address to assign to this socket
///		Argument2: lpSockAddrLen is the length of the address in lpSockAddr in bytes
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::Bind(const sockaddr* lpSockAddr,int nSockAddrLen)
{
	return (bind(m_hSocket,lpSockAddr,nSockAddrLen)!=SOCKET_ERROR);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method associates a local address with the socket 
///		Argument1: nSocketPort is the port identifying the socket application
///		Argument2: lpszSocketAddress is the network address, 
///				   a dotted number such as "128.56.22.8". 
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::Bind(UINT nSocketPort, LPCSTR lpszSocketAddress )
{
	sockaddr_in sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = (LPSTR)lpszSocketAddress;
	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		DWORD lResult = inet_addr(lpszAscii);
		if (lResult == INADDR_NONE)
		{
			return FALSE;
		}
		sockAddr.sin_addr.s_addr = lResult;
	}

	sockAddr.sin_port = htons((u_short)nSocketPort);

	return Bind((sockaddr*)&sockAddr, sizeof(sockAddr));

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sends data on a connected socket.Send is used to write 
///		outgoing data on connected stream or datagram sockets.
///		Argument1: lpBuf is a buffer containing the data to be transmitted
///		Argument2: nBufLen is the length of the data in lpBuf in bytes
///		Argument3: nFlags specifies the way in which the call is made
///				   The semantics of this function are determined by the socket 
///				   options and the nFlags parameter. The latter is constructed 
///				   by combining any of the following values with the C++ OR operator:
///		 MSG_DONTROUTE: Specifies that the data should not be subject to routing. 
///		 MSG_OOB: Send out-of-band data (SOCK_STREAM only).
///
///  RETURNS:
///		If no error occurs, Send returns the total number of characters send.
///		Otherwise, a value of SOCKET_ERROR is returned. 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CAsyncSocket::Send(const void *lpBuf,int nBufLen,int nFlags)
{
	return ::send(m_hSocket,(const char*)lpBuf,nBufLen,nFlags);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sends data to a specific destination. SendTo is used to 
///		write outgoing data on connected stream or datagram sockets.
///		Argument1: lpBuf is a buffer containing the data to be transmitted
///		Argument2: nBufLen is the length of the data in lpBuf in bytes
///		Argument3: lpSockAddr is a pointer to a SOCKADDR structure that contains 
///				   the address of the target socket
///		Argument4: nSockAddrLen is the length of the address in lpSockAddr in bytes
///		Argument5: nFlags specifies the way in which the call is made
///				   The semantics of this function are determined by the socket 
///				   options and the nFlags parameter. The latter is constructed 
///				   by combining any of the following values with the C++ OR operator:
///		 MSG_DONTROUTE: Specifies that the data should not be subject to routing. 
///		 MSG_OOB: Send out-of-band data (SOCK_STREAM only).
///
///  RETURNS:
///		If no error occurs, Send returns the total number of characters send.
///		Otherwise, a value of SOCKET_ERROR is returned. 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CAsyncSocket::SendTo(const void * lpBuf,int nBufLen,const sockaddr* lpSockAddr,int nSockAddrLen,int nFlags)
{
	return ::sendto(m_hSocket,(const char*)lpBuf,nBufLen,nFlags,lpSockAddr,nSockAddrLen);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sends data to a specific destination. SendTo is used to 
///		write outgoing data on connected stream or datagram sockets.
///		Argument1: lpBuf is a buffer containing the data to be transmitted
///		Argument2: nBufLen is the length of the data in lpBuf in bytes
///		Argument3: nHostPort is the port identifying the socket application
///		Argument4: lpszHostAddress is The network address of the socket to 
///				   which this object is connected: a machine name, or a dotted number.
///		Argument5: nFlags specifies the way in which the call is made
///				   The semantics of this function are determined by the socket 
///				   options and the nFlags parameter. The latter is constructed 
///				   by combining any of the following values with the C++ OR operator:
///		 MSG_DONTROUTE: Specifies that the data should not be subject to routing. 
///		 MSG_OOB: Send out-of-band data (SOCK_STREAM only).
///
///  RETURNS:
///		If no error occurs, Send returns the total number of characters send.
///		Otherwise, a value of SOCKET_ERROR is returned. 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CAsyncSocket::SendTo(const void* lpBuf, int nBufLen,UINT nHostPort, LPCSTR lpszHostAddress, int nFlags)
{
	sockaddr_in sockAddr;

	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = (LPSTR)lpszHostAddress;
	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	else
	{
		sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);
		if (sockAddr.sin_addr.s_addr == INADDR_NONE)
		{
			hostent* lphost = gethostbyname(lpszAscii);
			if (lphost != NULL)
				sockAddr.sin_addr.s_addr = ((in_addr*)lphost->h_addr)->s_addr;
			else
			{
				return SOCKET_ERROR;
			}
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	return SendTo(lpBuf, nBufLen, (sockaddr*)&sockAddr, sizeof(sockAddr), nFlags);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method receives data from a socket. This function is used for 
///		connected stream or datagram sockets and is used to read incoming data.
///		Argument1: lpBuf is a buffer for the incoming data
///		Argument2: nBufLen is length of lpBuf in bytes
///		Argument3: nFlags specifies the way in which the call is made
///				   The semantics of this function are determined by the socket 
///				   options and the nFlags parameter. The latter is constructed 
///				   by combining any of the following values with the C++ OR operator:
///		 MSG_PEEK: Peek at the incoming data. The data is copied into
///				   the buffer but is not removed from the input queue. 
///		 MSG_OOB: Send out-of-band data (SOCK_STREAM only).
///
///  RETURNS:
///		If no error occurs, Receive returns the number of bytes received.
///		If the connection has been closed, it returns 0.
///		Otherwise, a value of SOCKET_ERROR is returned. 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CAsyncSocket::Receive(void *lpBuf,int nBufLen,int nFlags)
{
	return ::recv(m_hSocket,(char*)lpBuf,nBufLen,nFlags);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method receives a datagram and store the source address in the 
///		SOCKADDR structure. This function is used to read incoming data on a
///		(possibly connected) socket and capture the address from which 
///		the data was sent.
///		Argument1: lpBuf is a buffer for the incoming data
///		Argument2: nBufLen is length of lpBuf in bytes
///		Argument3: lpSockAddr is a pointer to a SOCKADDR structure that holds  
///				   the source address upon return
///		Argument4: lpSockAddrLen is a pointer to the length of the source 
///				   address in lpSockAddr in bytes
///		Argument5: nFlags specifies the way in which the call is made
///				   The semantics of this function are determined by the socket 
///				   options and the nFlags parameter. The latter is constructed 
///				   by combining any of the following values with the C++ OR operator:
///		 MSG_PEEK: Peek at the incoming data. The data is copied into
///				   the buffer but is not removed from the input queue. 
///		 MSG_OOB: Send out-of-band data (SOCK_STREAM only).
///
///  RETURNS:
///		If no error occurs, Receive returns the number of bytes received.
///		If the connection has been closed, it returns 0.
///		Otherwise, a value of SOCKET_ERROR is returned. 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen,sockaddr* lpSockAddr, int* lpSockAddrLen, int nFlags)
{
	return ::recvfrom(m_hSocket,(char*)lpBuf,nBufLen,nFlags,lpSockAddr,(socklen_t*)lpSockAddrLen);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method receives a datagram and store the source address in the 
///		rSocketAddress. This function is used to read incoming data on a 
///		(possibly connected) socket and capture the address from which 
///		the data was sent. 
///		Argument1: lpBuf is a buffer for the incoming data
///		Argument2: nBufLen is length of lpBuf in bytes
///		Argument3: rSocketAddress is a reference to a QString object that 
///				   receives a dotted number IP address
///		Argument4: rSocketPort is a reference to a UINT that stores a port
///		Argument5: nFlags specifies the way in which the call is made
///				   The semantics of this function are determined by the socket 
///				   options and the nFlags parameter. The latter is constructed 
///				   by combining any of the following values with the C++ OR operator:
///		 MSG_PEEK: Peek at the incoming data. The data is copied into
///				   the buffer but is not removed from the input queue. 
///		 MSG_OOB: Send out-of-band data (SOCK_STREAM only).
///
///  RETURNS:
///		If no error occurs, Receive returns the number of bytes received.
///		If the connection has been closed, it returns 0.
///		Otherwise, a value of SOCKET_ERROR is returned. 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen, QString& rSocketAddress, UINT& rSocketPort, int nFlags)
{
	sockaddr_in sockAddr;

	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	int nResult = ReceiveFrom(lpBuf, nBufLen, (sockaddr*)&sockAddr, &nSockAddrLen, nFlags);
	if(nResult != SOCKET_ERROR)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return nResult;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method establishes a connection to an unconnected stream or 
///		datagram socket
///		the data was sent. 
///		Argument1: lpSockAddr is a pointer to a SOCKADDR structure that contains   
///				   the address of the connected socket
///		Argument2: lpSockAddrLen is the length of the address in lpSockAddr in bytes
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::Connect(const sockaddr* lpSockAddr,int nSockAddrLen)
{
	if (::connect(m_hSocket,lpSockAddr,nSockAddrLen)!=SOCKET_ERROR)
	{
		m_IsConnecting=true;
		return TRUE;
	}
	#if (defined Q_WS_WIN)
		DWORD err =WSAGetLastError();
	#else
		DWORD err =GetLastError();
	#endif
	if (err==10035)
	{
		m_IsConnecting=true;
		if (m_Mask & (FD_WRITE|FD_CONNECT))
		{
		}
		return TRUE;
	}
	return FALSE;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method establishes a connection to an unconnected stream or 
///		datagram socket
///		the data was sent. 
///		Argument1: lpszHostAddress is The network address of the socket to 
///				   which this object is connected: a machine name such as 
///				   "ftp.microsoft.com", or a dotted number such as "128.56.22.8".
///		Argument2: nHostPort is the port identifying the socket application
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::Connect(LPCSTR lpszHostAddress, UINT nHostPort)
{
	ASSERT(lpszHostAddress != NULL);

	sockaddr_in sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = (LPSTR)lpszHostAddress;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		hostent* lphost = gethostbyname(lpszAscii);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((in_addr*)lphost->h_addr)->s_addr;
		else
		{
			return FALSE;
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	return Connect((sockaddr*)&sockAddr, sizeof(sockAddr));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method listens for incoming connection requests
///		Argument1: nConnectionBacklog is the maximum length to which the queue 
///				   of pending connections can grow. Valid range is from 1 to 5
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::Listen(int nConnectionBacklog)
{
	int ret=listen(m_hSocket,nConnectionBacklog);
	if (ret!=SOCKET_ERROR)
		m_IsListening=true;

	return (ret!=SOCKET_ERROR);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method controls the mode of a socket
///		Argument1: lCommand is the command to perform on the socket:
///					FIONBIO: Enable or disable nonblocking mode on the socket. 
///							 The lpArgument parameter points at a DWORD, which 
///							 is nonzero if nonblocking mode is to be enabled and 
///							 zero if it is to be disabled. If AsyncSelect has 
///							 been issued on a socket, then any attempt to use 
///							 IOCtl to set the socket back to blocking mode will 
///							 fail with WSAEINVAL. To set the socket back to blocking 
///							 mode and prevent the WSAEINVAL error, an application 
///							 must first disable AsyncSelect by calling AsyncSelect 
///							 with the lEvent parameter equal to 0, then call IOCtl.
///					FIONREAD: Determine the maximum number of bytes that can be 
///							 read with one Receive call from this socket. The 
///							 lpArgument parameter points at a DWORD in which 
///							 IOCtl stores the result. If this socket is of type 
///							 SOCK_STREAM, FIONREAD returns the total amount of 
///							 data which can be read in a single Receive; this is 
///							 normally the same as the total amount of data queued 
///							 on the socket. If this socket is of type SOCK_DGRAM, 
///							 FIONREAD returns the size of the first datagram 
///							 queued on the socket.
///					SIOCATMARK: Determine whether all out-of-band data has been 
///							 read. This applies only to a socket of type SOCK_STREAM 
///							 which has been configured for in-line reception of any 
///							 out-of-band data (SO_OOBINLINE). If no out-of-band data 
///							 is waiting to be read, the operation returns nonzero. 
///							 Otherwise it returns 0, and the next Receive or ReceiveFrom 
///							 performed on the socket will retrieve some or all of 
///							 the data preceding the "mark"; the application should 
///							 use the SIOCATMARK operation to determine whether any
///							 data remains. If there is any normal data preceding 
///							 the "urgent" (out-of-band) data, it will be received 
///							 in order. (Note that a Receive or ReceiveFrom will 
///							 never mix out-of-band and normal data in the same call.) 
///							 The lpArgument parameter points at a DWORD in which 
///							 IOCtl stores the result. 
///		Argument2: lpArgument is a pointer to a parameter for lCommand
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::IOCtl(long lCommand,DWORD* lpArgument)
{
#if (defined Q_WS_WIN)
	return (ioctlsocket(m_hSocket,lCommand,lpArgument)!=SOCKET_ERROR);
#endif
#if (defined Q_WS_X11)
	return (ioctl(m_hSocket,lCommand,lpArgument)!=SOCKET_ERROR);
#endif

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
void CAsyncSocket::Close()
{
	if (m_hSocket!=INVALID_SOCKET)
	{
		AsyncSelect(0);
		if (m_pReadNotify)
		{
			delete m_pReadNotify;
			m_pReadNotify=NULL;
		}
		if (m_pWriteNotify)
		{
			delete m_pWriteNotify;
			m_pWriteNotify=NULL;
		}
#if (defined Q_WS_WIN)
		closesocket(Detach());
#endif
#if (defined Q_WS_X11)
		close(Detach());
#endif
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CAsyncSocket::~CAsyncSocket()
{
	if (m_hSocket!=INVALID_SOCKET)
		Close();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method returns a pointer to a CAsyncSocket object. When given a 
///		SOCKET handle, if a CAsyncSocket object is not attached to the handle, 
///		the member function returns NULL
///		Argument: hSocket contains a handle to a socket.
///
///  RETURNS:
///		returns a pointer to an CAsyncSocket object, or NULL if there is no 
///		CAsyncSocket object attached to hSocket
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CAsyncSocket* CAsyncSocket::FromHandle(SOCKET hSocket)
{
	for (int i=0;i<m_SocketTable.GetSize();i++)
	{
		if (m_SocketTable[i]->m_hSocket==hSocket)
		{
			return m_SocketTable[i];
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method disables sends and/or receives on the socket. ShutDown is 
///		used on all types of sockets to disable reception, transmission, 
///		or both. If nHow is 0, subsequent receives on the socket will be 
///		disallowed. This has no effect on the lower protocol layers.
///		Argument: nHow is a flag that describes what types of operation will 
///				  no longer be allowed, using the following enumerated values: 
///					receives = 0
///					sends = 1
///					both = 2 
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::ShutDown(int nHow)
{
	return (shutdown(m_hSocket,nHow)!=SOCKET_ERROR);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets a socket option
///		Argument1: nOptionName is the socket option for which the value is to be set
///		Argument2: lpOptionValue is a pointer to the buffer in which the value 
///				   for the requested option is supplied
///		Argument3: nOptionLen is the size of the lpOptionValue buffer in bytes
///		Argument4: nLevel is the level at which the option is defined; 
///				   the only supported levels are SOL_SOCKET and IPPROTO_TCP
///
///  RETURNS:
///		returns true if the function is successful, otherwise false
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
BOOL CAsyncSocket::SetSockOpt(int nOptionName,const void* lpOptionValue,int nOptionLen,int nLevel)
{
	return setsockopt(m_hSocket,nLevel,nOptionName,(char *)lpOptionValue,nOptionLen);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the error status for the last operation that failed.
///
///  RETURNS:
///		returns value indicates the error code for the last Windows Sockets 
///		API routine performed by this thread
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CAsyncSocket::GetLastError()
{
#ifdef Q_WS_WIN
	return WSAGetLastError();
#else
	return errno;
#endif
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the socket
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsyncSocket::InitSocket()
{
#if (defined Q_WS_WIN)
	WSADATA wsadata;
	WSAStartup (MAKEWORD(2,2),&wsadata);
#endif
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method receives data from a socket
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsyncSocket::ReceiveData()
{
	int nErrorCode=0;
	DWORD nBytes=0;
        DWORD err;
	#if (defined Q_WS_WIN)
		err =WSAGetLastError();
	#else
		err =GetLastError();
	#endif
	if (!IOCtl(FIONREAD, &nBytes))
		nErrorCode = 0;
	if ((nBytes != 0) || (nErrorCode != 0))
	{
		if (nBytes)
			OnReceive(nErrorCode);
		else
		{
		}
	}
	else
	{
		if (m_IsListening)
			OnAccept(nErrorCode);
		else
		{
#if (defined Q_WS_X11)
			OnClose(nErrorCode);
#endif
		}
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sends data on a connected socket
///
///  RETURNS:
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CAsyncSocket::SendData()
{
	int error=0,len=sizeof(error);
	getsockopt(m_hSocket,SOL_SOCKET,SO_ERROR,(char*)&error,(socklen_t*)&len);
	int nErrorCode=error;
	if (error)
	{
		if (m_IsConnecting)
		{
			m_IsConnecting=false;
			if (m_Mask & FD_CONNECT)
				OnConnect(nErrorCode);
		}
		else
		{
			if (m_Mask & FD_CLOSE)
				OnClose(nErrorCode);
		}
	}
	else
	{
		if (m_IsConnecting)
		{
			m_IsConnecting=false;
			if (m_Mask & FD_CONNECT)
				OnConnect(nErrorCode);
		}
		else
		{
			if (m_Mask & FD_WRITE)
				OnSend(nErrorCode);
		}
	}
	if (m_pWriteNotify->isEnabled())
		m_pWriteNotify->setEnabled(false);
	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method notifies this socket that there is data in the buffer 
///		that can be retrieved by calling the Receive member function
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
void CAsyncSocket::OnReceive(int )
{}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method notifies this socket that there is data in the buffer 
///		that can be retrieved by calling the Receive member function
///		Argument: nErrorCode is the most recent error on a socket. The following 
///				  error codes apply to the OnSend member function: 
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
void CAsyncSocket::OnSend(int )
{}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method notifies the receiving socket that the sending socket has 
///		out-of-band data to send
///		Argument: nErrorCode is the most recent error on a socket. The following 
///				  error codes apply to the OnOutOfBandData member function: 
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
void CAsyncSocket::OnOutOfBandData(int )
{}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method notifies a listening socket that it can accept pending 
///		connection requests by calling the Accept member function
///		Argument: nErrorCode is the most recent error on a socket. The following 
///				  error codes apply to the OnAccept member function: 
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
void CAsyncSocket::OnAccept(int )
{}

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
void CAsyncSocket::OnConnect(int )
{}

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
void CAsyncSocket::OnClose(int )
{}

//--------------------------------< THE END >----------------------------------

