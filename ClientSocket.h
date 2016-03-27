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
//  FILENAME: ClientSocket.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the CClientSocket class.
//	  This class allows to create a TCP connection between the ASD and the CCS
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

#if !defined(CLIENTSOCKET_H_INCLUDED_)
#define CLIENTSOCKET_H_INCLUDED_


//-GLOBAL DATA-----------------------------------------------------------------
enum CLISOCK_STATE {notconnected,connecting,connected};


//-CLASS DEFINITION---------------------------------------------------------

class CClientSocket : public CAsyncSocket
{
	friend class CCSSource;

// Operations
private:

	//Constructor
	CClientSocket();
	//Copy constructor
	CClientSocket(CClientSocket& pClientSocket);
	//Destructor
	virtual ~CClientSocket();
	//This method closes the socket
	void Close();
	//This method connects the socket with the information given in argument
	void ConnectToOpe(sockaddr_in OpeAddr);
	//This method sends an heart beat
	int SendHeartBeat(int DevNum, int seqnumbr);

	// Overrides
	//This method notifies the socket that there is data to be treated in the buffer
	virtual void OnReceive(int nErrorCode);
	//This method notifies this socket that the connected socket is closed 
	//by its associated process
	virtual void OnClose(int nErrorCode);
	//This method notifies this connecting socket that its connection attempt 
	//is completed or failed
	virtual void OnConnect(int nErrorCode);
	//This method returns the status of the socket
	CLISOCK_STATE getState() 
	{ 
		return m_State; 
	};
	//This method is an accessor
	void setObject(CObject* pobj) 
	{ 
		m_pObject = pobj; 
	};

// Implementation
protected:
	//Socket status
	CLISOCK_STATE m_State;
	//Associated parent object
	CObject* m_pObject;
	//Coordinates of the socket (Ip Adress and port number)
	sockaddr_in m_Addr;
	//Temporary buffer filled at reception before sending the whole message to
	//parent object
	BYTE* m_pRemBuff;
	//Length of the received message
	int m_RemCount;
};


#endif // end of file

//--------------------------------< THE END >----------------------------------
