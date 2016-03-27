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
//  FILENAME: CommonMenu.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 1  2009/08/31 dalleins
//
// 
//-----------------------------------------------------------------------------
//
/// \class CObject
/// This class s used in order to receive messages (sockets)
//
//-----------------------------------------------------------------------------
//-INCLUDE FILES---------------------------------------------------------------

typedef unsigned char       BYTE;
typedef unsigned int        UINT;


#include "Templ.h"
#include "qstringlist.h"

#if (defined Q_WS_WIN)
#include <winsock2.h>
#endif
#if (defined Q_WS_X11)
#include <netdb.h>
#endif


#include <time.h>
#include "CommonObject.h"
#include "CommonMenu.h"
#include "qwidget.h"
#include "../AsdMenu.h"
//-GLOBAL DATA-----------------------------------------------------------------
                       
//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------
//-----------------------------------------------------------------------------
//
/// \class CCmdUI
/// This class is used to set the presentation of a menu item
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CCmdUI::CCmdUI(CCmdUI& pCmdUI)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the constructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CCmdUI::CCmdUI()
{
	m_pMenu=NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CCmdUI::~CCmdUI()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the ID of a menu item
/// 
///  RETURNS: 
///		the ID of a menu item
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
UINT CCmdUI::GetID()
{
	return m_Id;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the color of a menu item
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CCmdUI::SetTextColor(QColor* pColor)
{
	if (m_pMenu)
	{
		m_pMenu->SetItemTextColor(m_MenuIndex, pColor);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method toggles the checkbox of a menu item depending on Check parameter
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CCmdUI::SetCheck(bool Check)
{
	if (m_pMenu)
	{
		m_pMenu->SetChecked(m_MenuIndex,Check);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method toggles the grayed-out of a menu item depending on Enable parameter
/// 
///  RETURNS: 
///		void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CCmdUI::Enable(bool Enable)
{
	if (m_pMenu)
	{
		m_pMenu->SetDisabled(m_MenuIndex,!Enable);
	}
}

//--------------------------------< THE END >----------------------------------

