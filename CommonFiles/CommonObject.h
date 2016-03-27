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
//  FILENAME: CommonObject.h
//  TYPE    : header file
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

#ifndef COMMONOBJECT_H
#define COMMONOBJECT_H

//-GLOBAL DATA-----------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function returns the smallest element between a and b arguments
/// 
///  RETURNS: 
///		an integer
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
inline int _min(int a, int b)
{
	return (((a)<(b)) ? (a) : (b));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function returns the largest element between a and b arguments
/// 
///  RETURNS: 
///		an integer
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
inline int _max(int a, int b)
{
	return (((a)>(b)) ? (a) : (b));
}


//-INCLUDE FILES---------------------------------------------------------------

class CIniFile;
class CCmdUI;
                       

//-CLASSES DEFINITIONS---------------------------------------------------------


class CObject
{
public:
	// constructor
	CObject() {};
	// destructor
	virtual ~CObject() {};	
	// method called when clicking on an item of a menu
	virtual void OnCommand(UINT id){}
	// method called when clicking on an item of a menu given an object as parameter
	virtual void OnCommand(UINT id, CObject*){}
	//	called when an ASD menu has to be updated
	virtual void OnUpdateCmdUI(CCmdUI* ){}
	// virtual method called set the status of the menu item when its
	// associated pull down menu is closed
	virtual void OnCloseMenu(){}
	// virtual method method called when a message is received from an external system
	virtual void OnReceiveMsg(BYTE* ,int,sockaddr_in* = NULL,int =0){}
	// virtual method called to set up the ASD configuration according to the group, subgroup,
	// location and role
	virtual void Setup(CIniFile* ,QString Section,bool Save){}
};


#endif  // end of file



//--------------------------------< THE END >----------------------------------

