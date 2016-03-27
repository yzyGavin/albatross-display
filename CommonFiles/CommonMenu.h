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
//  FILENAME: CommonMenu.h
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
//
// 
//-----------------------------------------------------------------------------



#ifndef COMMONMENU_H
#define COMMONMENU_H



//-GLOBAL DATA-----------------------------------------------------------------



//-INCLUDE FILES---------------------------------------------------------------

typedef unsigned int        UINT;
class CAsdMenu;
class CMenuWnd;
//class CIniFile;

#include "qcolor.h"


                       

//-CLASSES DEFINITIONS---------------------------------------------------------
class CCmdUI
{
	friend class CMenuWnd;

public:

	//variables
	CAsdMenu* m_pMenu;
	int m_MenuIndex;
	UINT m_Id;

	//methods
	CCmdUI();
	CCmdUI(CCmdUI& pCmdUI);
	virtual ~CCmdUI();
	void SetCheck(bool Check);
	void Enable(bool Enable);
	UINT GetID();
	void SetTextColor(QColor* pColor);	
};

#endif  // end of file

//--------------------------------< THE END >----------------------------------

