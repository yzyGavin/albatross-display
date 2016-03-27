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
//  FILENAME: VisuSocket.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CRcvSocket which allows to create a socket for 
//		reception.  
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
//-----------------------------------------------------------------------------
#if !defined(AFX_VISUSOCKET_H__1DF43774_2FB4_11D4_A486_00C04F72F475__INCLUDED_)
#define AFX_VISUSOCKET_H__1DF43774_2FB4_11D4_A486_00C04F72F475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-INCLUDE FILES---------------------------------------------------------------

#include "AsyncSocket.h"

//-GLOBAL DATA-----------------------------------------------------------------

//-CLASSES DEFINITIONS---------------------------------------------------------

class CRcvSocket : public CAsyncSocket
{
	friend class CAsterixSource;
	friend class CTrackMngr;

private:
	// variables
	QDateTime m_LastReceived;
	bool m_bStartTimeOut;

	// virtual methods
	virtual void OnReceive(int nErrorCode);

	// methods
	CRcvSocket();
	CRcvSocket(CRcvSocket& pRcvSocket);
	virtual ~CRcvSocket();
	int  Create(CObject* pObject,int timeout, sockaddr_in addr,in_addr mcastaddr,int socketid=0);
	void Close();	
	bool IsTimeOut();
	
protected:
	//variables
	sockaddr_in m_Addr;
	CObject* m_pRcvObject;
	int m_SocketId;
	int m_TimeOut;	

};

#endif // end of file

//--------------------------------< THE END >----------------------------------

