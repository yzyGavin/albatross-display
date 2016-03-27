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
//  FILENAME: GraphicSymbol.cpp
//  TYPE    : c++ code-file
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

typedef unsigned int        UINT;
typedef unsigned char       BYTE;
typedef int                 BOOL;

#include "qglobal.h"
#include "Templ.h"
#if (defined Q_WS_WIN)
#include <winsock2.h>
#endif
#if (defined Q_WS_X11)
#include <netdb.h>
#endif

#include "qstringlist.h"
#include <time.h>
#include "CommonObject.h"
#include "CommonMenu.h"
#include "qwidget.h"
#include "GraphicSymbol.h"
#include "qpainter.h"
#include "qdir.h"

//-GLOBAL DATA-----------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

CArray<CGraphicSymbol*,CGraphicSymbol*> CGraphicSymbol::m_SymbolTable;
QString CGraphicSymbol::m_Version="No Symbol Version";

//-PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CGraphicSymbol
/// This class retrieves the offline defined ASD symbols to store them on a table.
///	It also allows to draw the symbols on a planview
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
CGraphicSymbol::CGraphicSymbol()
{
	m_Char=0;
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
CGraphicSymbol::~CGraphicSymbol()
{

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes a table where the data is 
///		offline defined in the Symbols.ini file
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctIniAds025
//-----------------------------------------------------------------------------
bool CGraphicSymbol::InitTables(QString DefaultOfflinePath, QString& reason)
{
	QString curstr,tmpstr;
	// Retrieve the <version>_Symbols.ini file
	QFile file(DefaultOfflinePath+"Symbols.ini");
	BOOL res=file.open(IO_ReadOnly);
	if (!file.isOpen())
	{
		reason = "File "+DefaultOfflinePath+"Symbols.ini does not exist";
		return false;
	}
	int fnd,x1,y1,x2,y2;
	CGraphicSymbol* pSymb = NULL;
	char styl,typ;
	m_Elems curelem;
	QRect brect,tmprect;
	while (res)
	{
		if (file.readLine(curstr,512)==-1)
			break;

		if (curstr.length() && ((curstr[0]==';') || (curstr[0]=='#')))
		{
			if ((curstr.length()>=9) && (curstr.left(9).compare(";VERSION=")==0))
				m_Version=curstr.right(curstr.length()-9);
		}
		else
		{
			// read the file and create symbols that can be circles, rectangle or points
			// and store them in a table
			fnd = curstr.find('|');
			if (fnd!=-1)
			{
				pSymb = new CGraphicSymbol[1];
				pSymb->m_Name=curstr.left(fnd);
				brect=QRect();
				curstr=curstr.right(curstr.length()-fnd-1);
				curstr.stripWhiteSpace();
				pSymb->m_Char=curstr[0].latin1();
				curstr=curstr.right(curstr.length()-2);
				while (!curstr.isEmpty())
				{
					fnd = curstr.find('|');
					if (fnd!=-1)
					{
						tmpstr=curstr.left(fnd);
						curstr=curstr.right(curstr.length()-fnd-1);
						curstr.stripWhiteSpace();
						sscanf(tmpstr,"%c,%c",&styl,&typ);
						curelem.fill = (styl=='S');
						switch (typ)
						{
						case 'C':
							curelem.type=circle;
							fnd=tmpstr.find('(');
							tmpstr=tmpstr.right(tmpstr.length()-fnd);
							sscanf(tmpstr,"(%d,%d),%d",&x1,&y1,&curelem.diam);
							curelem.pos1=QPoint(x1,y1);
							brect.setLeft(_min(curelem.pos1.x()-curelem.diam,brect.left()));
							brect.setRight(_max(curelem.pos1.x()+curelem.diam,brect.right()));
							brect.setTop(_min(-curelem.pos1.y()-curelem.diam,brect.top()));
							brect.setBottom(_max(-curelem.pos1.y()+curelem.diam,brect.bottom()));
							pSymb->m_ElemTable.Add(curelem);
							break;
						case 'R':
							curelem.type=rct;
							fnd=tmpstr.find('(');
							tmpstr=tmpstr.right(tmpstr.length()-fnd);
							sscanf(tmpstr,"(%d,%d,%d,%d)",&x1,&y1,&x2,&y2);
							curelem.pos1=QPoint(x1,y1);
							curelem.pos2=QPoint(x2,y2);
							tmprect = QRect(QPoint(curelem.pos1.x(),-curelem.pos1.y()),QPoint(curelem.pos2.x(),-curelem.pos2.y()));
							tmprect=tmprect.normalize();
							brect.setLeft(_min(tmprect.left(),brect.left()));
							brect.setRight(_max(tmprect.right(),brect.right()));
							brect.setTop(_min(tmprect.top(),brect.top()));
							brect.setBottom(_max(tmprect.bottom(),brect.bottom()));
							pSymb->m_ElemTable.Add(curelem);
							break;
						case 'P':
							curelem.type=pnt;
							curelem.diam=0;
							while (1)
							{
								fnd=tmpstr.find('(');
								if (fnd==-1)
								{
									pSymb->m_ElemTable[pSymb->m_ElemTable.GetSize()-1].diam=-1;
									break;
								}
								tmpstr=tmpstr.right(tmpstr.length()-fnd);
								sscanf(tmpstr,"(%d,%d)",&x1,&y1);
								curelem.pos1=QPoint(x1,y1);
								brect.setLeft(_min(curelem.pos1.x(),brect.left()));
								brect.setRight(_max(curelem.pos1.x(),brect.right()));
								brect.setTop(_min(-curelem.pos1.y(),brect.top()));
								brect.setBottom(_max(-curelem.pos1.y(),brect.bottom()));
								tmpstr=tmpstr.right(tmpstr.length()-1);
								pSymb->m_ElemTable.Add(curelem);
								curelem.diam=1;
							}
							break;
						default:
							break;
						}
					}
					else
						break;
				}
				brect=brect.normalize();
				pSymb->m_BoundingRect = brect;
				m_SymbolTable.Add(pSymb);
			}
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method adds in the m_SymbolTable table the single symbol put in 
///		argument with its provided name and geometrical description
/// 
///  RETURNS: 
///		true if symbol is successfully added otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CGraphicSymbol::addSymbolToTable(QString name, QString description)
{
	QString curstr(description),tmpstr;
	int fnd,x1,y1,x2,y2;
	CGraphicSymbol* pSymb = 0;
	char styl,typ;
	m_Elems curelem;
	QRect brect,tmprect;
	bool symbolOk(true);

	pSymb = new CGraphicSymbol[1];
	pSymb->m_Name = name;
	brect=QRect();

	while (!curstr.isEmpty())
	{
		fnd = curstr.find('|');
		if (fnd!=-1)
		{
			tmpstr=curstr.left(fnd);
			curstr=curstr.right(curstr.length()-fnd-1);
			curstr.stripWhiteSpace();
			if (sscanf(tmpstr,"%c,%c",&styl,&typ) != 2)
				symbolOk = false;
			curelem.fill = (styl=='S');
			switch (typ)
			{
			case 'C':
				curelem.type=circle;
				fnd=tmpstr.find('(');
				tmpstr=tmpstr.right(tmpstr.length()-fnd);
				if (sscanf(tmpstr,"(%d,%d),%d",&x1,&y1,&curelem.diam) != 3)
					symbolOk = false;
				curelem.pos1=QPoint(x1,y1);
				brect.setLeft(_min(curelem.pos1.x()-curelem.diam,brect.left()));
				brect.setRight(_max(curelem.pos1.x()+curelem.diam,brect.right()));
				brect.setTop(_min(-curelem.pos1.y()-curelem.diam,brect.top()));
				brect.setBottom(_max(-curelem.pos1.y()+curelem.diam,brect.bottom()));
				if (symbolOk)
					pSymb->m_ElemTable.Add(curelem);
				break;
			case 'R':
				curelem.type=rct;
				fnd=tmpstr.find('(');
				tmpstr=tmpstr.right(tmpstr.length()-fnd);
				if (sscanf(tmpstr,"(%d,%d,%d,%d)",&x1,&y1,&x2,&y2) != 4)
					symbolOk = false;
				curelem.pos1=QPoint(x1,y1);
				curelem.pos2=QPoint(x2,y2);
				tmprect = QRect(QPoint(curelem.pos1.x(),-curelem.pos1.y()),QPoint(curelem.pos2.x(),-curelem.pos2.y()));
				tmprect=tmprect.normalize();
				brect.setLeft(_min(tmprect.left(),brect.left()));
				brect.setRight(_max(tmprect.right(),brect.right()));
				brect.setTop(_min(tmprect.top(),brect.top()));
				brect.setBottom(_max(tmprect.bottom(),brect.bottom()));
				if (symbolOk)
					pSymb->m_ElemTable.Add(curelem);
				break;
			case 'P':
				curelem.type=pnt;
				curelem.diam=0;
				while (1)
				{
					fnd=tmpstr.find('(');
					if (fnd==-1)
					{
						pSymb->m_ElemTable[pSymb->m_ElemTable.GetSize()-1].diam=-1;
						break;
					}
					tmpstr=tmpstr.right(tmpstr.length()-fnd);
					if (sscanf(tmpstr,"(%d,%d)",&x1,&y1) != 2)
					{
						symbolOk = false;
						delete [] pSymb;
						return symbolOk;
					}
					curelem.pos1=QPoint(x1,y1);
					brect.setLeft(_min(curelem.pos1.x(),brect.left()));
					brect.setRight(_max(curelem.pos1.x(),brect.right()));
					brect.setTop(_min(-curelem.pos1.y(),brect.top()));
					brect.setBottom(_max(-curelem.pos1.y(),brect.bottom()));
					tmpstr=tmpstr.right(tmpstr.length()-1);
					if (symbolOk)
						pSymb->m_ElemTable.Add(curelem);
					curelem.diam=1;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			symbolOk = false;
			break;
		}
	}
	brect=brect.normalize();
	pSymb->m_BoundingRect = brect;
	if (symbolOk)
		m_SymbolTable.Add(pSymb);

	return symbolOk;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates a symbol description
/// 
///  RETURNS: 
///		true if symbol is successfully retrieved otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CGraphicSymbol::updateTableSymbol(QString name, QString description)
{
	//Search for existing symbol index
	int oldIndex(0);
	for (int i = 0; i < m_SymbolTable.GetSize(); i++)
		if (m_SymbolTable[i]->m_Name == name)
			oldIndex = i;

	if (addSymbolToTable(name, description))
	{
		m_SymbolTable.RemoveAt(oldIndex);
		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method draws the symbol (circle, points or rectangles) thanks to the
///		position of the symbol in the planview given as parameter 
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CGraphicSymbol::Draw(QPainter* pDC,QPoint pos)
{
	m_Elems curelem;
	QRect rect;
	QPen tmppen=pDC->pen();
	QColor curcolor = tmppen.color();
	QPen pen(curcolor);
	QBrush brush(curcolor);
	pDC->setBrush(brush);
	QPointArray PntTab(30);
	QPoint lastpos;
	int curpos=0;
	for (int i=0;i<m_ElemTable.GetSize();i++)
	{
		curelem = m_ElemTable[i];
		switch(curelem.type)
		{
		case circle:
			if (curelem.diam!=0)
			{
				rect =QRect(QPoint(pos.x()+curelem.pos1.x()-curelem.diam,pos.y()-curelem.pos1.y()-curelem.diam),QPoint(pos.x()+curelem.pos1.x()+curelem.diam,pos.y()-curelem.pos1.y()+curelem.diam));
				rect=rect.normalize();
				if (curelem.fill)
					pDC->drawEllipse(rect);
				else
					pDC->drawArc(rect,0,360*16);
			}
			else
				pDC->drawPoint(curelem.pos1.x()+pos.x(),-curelem.pos1.y()+pos.y());
			break;
		case pnt:
			if (curelem.fill)
			{
				if (curelem.diam==0)
					curpos=0;
				PntTab.setPoint(curpos,QPoint(curelem.pos1.x()+pos.x(),-curelem.pos1.y()+pos.y()));
				curpos++;
				if (curelem.diam==-1)
					pDC->drawPolygon(PntTab,FALSE,0,curpos);
			}
			else
			{
				if (curelem.diam!=0)
					pDC->drawLine(lastpos,QPoint(curelem.pos1.x()+pos.x(),-curelem.pos1.y()+pos.y()));
				lastpos=QPoint(curelem.pos1.x()+pos.x(),-curelem.pos1.y()+pos.y());
			}
			break;
		case rct:
			rect =QRect(QPoint(pos.x()+curelem.pos1.x(),pos.y()-curelem.pos1.y()),QPoint(pos.x()+curelem.pos2.x(),pos.y()-curelem.pos2.y()));
			rect=rect.normalize();
			if (curelem.fill)
				pDC->drawRect(rect);
			else
			{
				PntTab[0]=PntTab[4]=rect.topLeft();
				PntTab[1]=rect.topRight();
				PntTab[2]=rect.bottomRight();
				PntTab[3]=rect.bottomLeft();
				pDC->drawPolyline(PntTab,0,5);
			}
			break;
		default:
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves a symbol according to the Name put in parameter 
/// 
///  RETURNS: 
///		the symbol of type CGraphicSymbol
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CGraphicSymbol* CGraphicSymbol::GetSymbol(QString Name)
{
	for (int i=0;i<m_SymbolTable.GetSize();i++)
	{
		if (m_SymbolTable[i]->m_Name.compare(Name)==0)
			return m_SymbolTable[i];
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves a symbol according to the character given as parameter 
/// 
///  RETURNS: 
///		the symbol of type CGraphicSymbol
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CGraphicSymbol* CGraphicSymbol::GetSymbol(char Char)
{
	for (int i=0;i<m_SymbolTable.GetSize();i++)
	{
		if (m_SymbolTable[i]->m_Char==Char)
			return m_SymbolTable[i];
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the borders of a symbol according its position.
/// 
///  RETURNS: 
///		a rectangle Qrect
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
QRect CGraphicSymbol::GetBoundingRect(QPoint pos)
{
	QRect rect=m_BoundingRect;
	rect.addCoords(pos.x(),pos.y(),pos.x(),pos.y());
	return rect;
}

//--------------------------------< THE END >----------------------------------
