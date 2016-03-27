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
//  FILENAME: AsynSocket.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file manages the asynchron sockets.
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


#if !defined(AFX_ASYNCSOCKET__INCLUDED_)
#define AFX_ASYNCSOCKET__INCLUDED_

//-INCLUDE FILES---------------------------------------------------------------
#include "qsocketnotifier.h"
#if (defined Q_WS_WIN)
#include <winsock2.h>
#endif
#if (defined Q_WS_X11)
#include "StdAfx.h"
#include <netdb.h>
#define FD_READ 0x1
#define FD_WRITE 0x2
#define FD_OOB 0x4
#define FD_ACCEPT 0x8
#define FD_CONNECT 0x10
#define FD_CLOSE 0x20
#define SOCKET int
#endif
#include "string.h"

//-CLASS DEFINITION---------------------------------------------------------
class CAsyncSocket : public QObject
{
    Q_OBJECT

private:
	CAsyncSocket(const CAsyncSocket& rSrc);    // no implementation
	void operator=(const CAsyncSocket& rSrc);  // no implementation
	SOCKET m_hSocket;

// Construction
public:
	CAsyncSocket();
	BOOL Create(UINT nSocketPort = 0, int nSocketType=SOCK_STREAM,
		long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
		LPCSTR lpszSocketAddress = NULL);

// Attributes
public:

	operator SOCKET() const;
	BOOL Attach(SOCKET hSocket, long lEvent =
		FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);
	SOCKET Detach();

	BOOL GetPeerName(QString& rPeerAddress, UINT& rPeerPort);
	BOOL GetPeerName(sockaddr* lpSockAddr, int* lpSockAddrLen);

	BOOL GetSockName(QString& rSocketAddress, UINT& rSocketPort);
	BOOL GetSockName(sockaddr* lpSockAddr, int* lpSockAddrLen);

	BOOL SetSockOpt(int nOptionName, const void* lpOptionValue,
		int nOptionLen, int nLevel = SOL_SOCKET);
	BOOL GetSockOpt(int nOptionName, void* lpOptionValue,
		int* lpOptionLen, int nLevel = SOL_SOCKET);

	static CAsyncSocket* FromHandle(SOCKET hSocket);
	static int GetLastError();

// Operations
public:

	virtual BOOL Accept(CAsyncSocket& rConnectedSocket,
		sockaddr* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

	BOOL Bind(UINT nSocketPort, LPCSTR lpszSocketAddress = NULL);
	BOOL Bind (const sockaddr* lpSockAddr, int nSockAddrLen);

	virtual void Close();

	BOOL Connect(LPCSTR lpszHostAddress, UINT nHostPort);
	BOOL Connect(const sockaddr* lpSockAddr, int nSockAddrLen);

	BOOL IOCtl(long lCommand, DWORD* lpArgument);

	BOOL Listen(int nConnectionBacklog=5);

	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);

	int ReceiveFrom(void* lpBuf, int nBufLen,
		QString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);
	int ReceiveFrom(void* lpBuf, int nBufLen,
		sockaddr* lpSockAddr, int* lpSockAddrLen, int nFlags = 0);

	enum { receives = 0, sends = 1, both = 2 };
	BOOL ShutDown(int nHow = sends);

	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);

	int SendTo(const void* lpBuf, int nBufLen,
		UINT nHostPort, LPCSTR lpszHostAddress = NULL, int nFlags = 0);
	int SendTo(const void* lpBuf, int nBufLen,
		const sockaddr* lpSockAddr, int nSockAddrLen, int nFlags = 0);

	BOOL AsyncSelect(long lEvent =
		FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);

// Overridable callbacks
protected:
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);

// Implementation
public:
	virtual ~CAsyncSocket();
	
	static void InitSocket();

	BOOL Socket(int nSocketType=SOCK_STREAM, long lEvent =
		FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
		int nProtocolType = 0, int nAddressFormat = PF_INET);


private :
	QSocketNotifier* m_pReadNotify;
	QSocketNotifier* m_pWriteNotify;
	bool m_IsConnecting;
	bool m_IsListening;
	int m_SocketType;
	UINT m_Mask;
	static QObject* m_pParent;
private slots:
	void ReceiveData();
	void SendData();
};
#endif // end of file  

//--------------------------------< THE END >----------------------------------
