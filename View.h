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
//  FILENAME: View.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of the reference class CView which is inheritted by all
//		other classes linked to radar windows.
//				
// 
//  COMMENTS
//		This class only contains virtual methods which are defined and applied 
//		for any other inheritting classes.
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
#if !defined(AFX_DOCVIEW_H)
#define AFX_DOCVIEW_H

//-INCLUDE FILES---------------------------------------------------------------
#include <qwidget.h>
#include "StdAfx.h"
#include "CommonFiles/CommonObject.h"
//-GLOBAL DATA-----------------------------------------------------------------

//-CLASSES DEFINITIONS---------------------------------------------------------

/* CDS BREAK JRP 2006/05/19 Multiple inheritance with QWidget which is not an abstract class.
       The alternative solution would be to inherit CObject from QWidget
       But the implementation of QT does not allow this design. */
class CView : public QWidget,public CObject
{
	Q_OBJECT

public:
	CView();
	~CView();
	virtual void OnUpdate( CView* , long , CObject*  ){}
	virtual int GetViewParam(int )
	{
		return 0;
	}
	virtual void SetViewParam(int ,int ){}
};


#endif

//--------------------------------< THE END >----------------------------------
