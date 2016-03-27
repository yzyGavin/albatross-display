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
//  FILENAME: GraphicSymbol.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the management of graphical symbols in the ASD
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

#if !defined(AFX_GRAPHICSYMBOL_H__64F8592B_2898_4394_9200_E892EE4F6C9B__INCLUDED_)
#define AFX_GRAPHICSYMBOL_H__64F8592B_2898_4394_9200_E892EE4F6C9B__INCLUDED_

//-GLOBAL DATA-----------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonObject.h"
#include "Templ.h"

//-CLASS DEFINITION------------------------------------------------------------

class CGraphicSymbol : public CObject  
{

public:
	
	//virtual methods
	virtual ~CGraphicSymbol();

	//static methods
	static CGraphicSymbol* GetSymbol(QString Name);
	static CGraphicSymbol* GetSymbol(char Char);
	static bool InitTables(QString DefaultOfflinePath, QString& reason);
	//adds a single symbol provided name and geometrical description
	static bool addSymbolToTable(QString name, QString description);
	//updates a symbol description
	static bool updateTableSymbol(QString name, QString description);		

	//methods
	CGraphicSymbol();	
	void Draw(QPainter* pDC,QPoint pos);
	QRect GetBoundingRect(QPoint pos);
	
private:

	//variables
	typedef enum {rct,circle,pnt,str} m_ElemTypes;
	// defines the caracteristics of the symbol
	typedef struct
	{
		// type of the symbol
		m_ElemTypes type;
		// position of the symbol
		QPoint    pos1;
		// position of the symbol
		QPoint	  pos2;
		// diameter of the symbol
		int		  diam;
		// set to true if the symbol is filled otherwise false
		bool      fill;
	} m_Elems;
	QString m_Name;
	char    m_Char;
	QRect   m_BoundingRect;
	CArray<m_Elems,m_Elems> m_ElemTable;
	static CArray<CGraphicSymbol*,CGraphicSymbol*> m_SymbolTable;
	static QString m_Version;
};

#endif  // end of file

//--------------------------------< THE END >---------------------------------- 
