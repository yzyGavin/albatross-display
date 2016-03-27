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
//  FILENAME: VisuMap.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CMapElement and other classes which allow to 
//		manage and display map elementary elements such as: vectors, surfaces,
//		text and beacons.
//		The following attributes are defined : Color, Font, FontSize, Style,
//		the priority, Text, Info field, Beacon type and a position.
//				
// 
//  COMMENTS
//
//  BUILD INFORMATION 
//
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 1  2009/08/31 dalleins//
//
//
// 
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include "View.h"
#include "DisplayElement.h"
#include "VisuMap.h"
#include "math.h"
#include "ColorMngr.h"
#include "AsdApp.h"
#include <stdlib.h>
#include "Track.h"

//-GLOBAL DATA-----------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ARC 0x40000000
#define PI 3.14159

CArray<CView*,CView*> CVisuMap::m_ViewTable;
CArray<CVisuMap*,CVisuMap*> CVisuMap::m_MapTable;
CAsdMenu* CVisuMap::m_pMenu=NULL;
QString CVisuMap::m_Version="No Map Version";
QString CVisuMap::m_GrlBeacon="";

//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------
//-----------------------------------------------------------------------------
//
/// \class CMapVectors
/// This class is used to map attributes of type vector contained in the mds files
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMapVectors::~CMapVectors()
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
CMapVectors::CMapVectors(m_MapParam& param)
{
	m_Kind=Ivectors;
	m_Info = param.Info;
	m_PenStyle = param.PenStyle;
	m_Color = param.Color;
	if (m_PenStyle==Qt::MPenStyle)
		for (int p=0;p<8;p++)
		{
			m_Patt[p]  = param.Patt[p];
			if (m_Patt[p]==0)
				m_PattLen=p-1;
		}
	m_PointTable.SetSize(param.PointTable.GetSize());
	for (int i=0;i<param.PointTable.GetSize();i++)
	{
		m_PointTable[i]=param.PointTable[i];
	}
	m_Prio = param.Prio;
}

//-----------------------------------------------------------------------------
//
/// \class CMapBeacon
/// This class is used to map attributes of type beacon contained in the mds files
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMapBeacon::~CMapBeacon()
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
CMapBeacon::CMapBeacon(m_MapParam& param)
{
	m_Kind=Ibeacon;
	m_Info = param.Info;
	m_Color = param.Color;
	m_BeaconName = param.Beacon;
	m_Point = param.PointTable[0];
	m_Char  = param.Text[0].latin1();
	m_Font = param.Font;
	m_Fontsize = param.FontSize;
	m_Prio = param.Prio;
}

//-----------------------------------------------------------------------------
//
/// \class CMapText
/// This class is used to map attributes of type info contained in the mds files
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMapText::~CMapText()
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
CMapText::CMapText(m_MapParam& param)
{
	m_Kind=Itext;
	m_Info = param.Info;
	m_Color = param.Color;
	m_Point = param.PointTable[0];
	m_Text  = param.Text;
	m_IsCentered=false;
	m_Font = param.Font;
	m_FontSize = param.FontSize;
	m_Prio = param.Prio;
}

//-----------------------------------------------------------------------------
//
/// \class CMapSurface
/// This class is used to map attributes of type surface contained in the mds files
// 
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMapSurface::~CMapSurface()
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
CMapSurface::CMapSurface(m_MapParam& param)
{
	m_Kind=Isurface;
	m_Info = param.Info;
	m_PenStyle = param.PenStyle;
	m_Color = param.Color;
	m_BrushStyle = param.BrushStyle;
	m_HatchStyle = param.HatchStyle;
	if (m_BrushStyle==Qt::CustomPattern)
		for (int p=0;p<8;p++)
			m_Patt[p] = param.Patt[p];
	m_Color2 = param.Color2;
	m_PointTable.SetSize(param.PointTable.GetSize());
	for (int i=0;i<param.PointTable.GetSize();i++)
	{
		m_PointTable[i]=param.PointTable[i];
	}
	m_Prio = param.Prio;
}


//-----------------------------------------------------------------------------
//
/// \class CVisuMap
/// This class is used to map attributes of type surface contained in the mds files
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
CVisuMap::CVisuMap()
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
CVisuMap::CVisuMap(CVisuMap& pVisuMap)
{
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
CVisuMap::~CVisuMap()
{
	int elem;
	for (elem=0;elem<m_ElementTable.GetSize();elem++)
		delete m_ElementTable[elem];
	m_ElementTable.RemoveAll();
	for (elem=0;elem<m_ElemTable.GetSize();elem++)
	{
		((CRwdView*)m_ViewTable[elem])->RemoveElement(m_ElemTable[elem]);
		delete m_ElemTable[elem];
	}
	for (int mapnb=0;mapnb<m_MapTable.GetSize();mapnb++)
	{
		if (m_MapTable[mapnb]==this)
			m_MapTable.RemoveAt(mapnb);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method stores for each map the name, size and time of creation or update
/// 
///  RETURNS: 
///	 void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CVisuMap::GetAttributes(QString* &name,long &size,QTime* &mtime)
{
	name = &m_MapName;
	size = m_Size;
	mtime = &m_Time;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method is a constructor which extracts the information contains in the 
///	 mds file and stores it in a map element table
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: CIAsdFctMapGen005 
//-----------------------------------------------------------------------------
// CDS BREAK JPM 19/05/2006 Bad method lenght rule violated. The code has been 
//							factorized as much as possible.
//							
CVisuMap::CVisuMap(QString MapName)
{
//QString strMessage;
//strMessage.sprintf("loading map %s", MapName.ascii());
//CAsdApp::GetApp()->WriteLogMsg(strMessage, LogError);
	
	typedef enum {title,vectors,beacon,text,surface,none} elements;
	typedef enum {Item,Color,Color2,Penstyle,Brushstyle,Font,Size,Text,Mode,Info,Pos,PosTable,Prio,Beacon,None} keywords;
	LPCSTR ElemNames[none+1] = {"Title","Vectors","Beacon","Text","Surface",""};
	LPCSTR KeyNames[None+1] = {"Item","Color","Color2","Penstyle","Brushstyle","Font","Size","Text","Mode","Info","Pos","Postable","Prio","Beacon",""};
	LPCSTR PenStyle[5] = {"PS_SOLID","PS_DASH","PS_DOT","PS_DASHDOT","PS_DASHDOTDOT"};
	Qt::PenStyle QtPenStyle[5] = {Qt::SolidLine,Qt::DashLine ,Qt::DotLine ,Qt::DashDotLine,Qt::DashDotDotLine};
	LPCSTR HatchStyle[6] = {"HS_HORIZONTAL","HS_VERTICAL","HS_FDIAGONAL","HS_BDIAGONAL","HS_CROSS","HS_DIAGCROSS"};
	Qt::BrushStyle QtHatchStyle[6] = {Qt::HorPattern,Qt::VerPattern,Qt::FDiagPattern,Qt::BDiagPattern,Qt::CrossPattern,Qt::DiagCrossPattern};
	QFileInfo status(MapName);
	QFile MapFile(MapName);
	bool ret = MapFile.open(IO_ReadOnly|IO_Translate);
//if (!ret)
//{
//QString strMessage;
//strMessage.sprintf("Error loading map %s", MapName.ascii());
//CAsdApp::GetApp()->WriteLogMsg(strMessage, LogError);
//}
	m_MapName = MapName;
	m_Size = status.size();
	m_Time = status.lastModified().time();
	QString line;
	BOOL notEof = TRUE;
	int index1,index2,index3,ind1,ind2,ind3,elem2,nbpoint;
	QString Key,Elem,StrX,Style,tmpname;
	keywords key;
	elements elem;
	CMapElement::m_MapParam param;
	QPoint point;
	CMapElement* pMapElement=NULL;
	m_WasSelected = false;
	m_IsSelectable =true;
	
	//find ID and filename
	tmpname=MapName;
	m_FileName=MapName;
	ind1=tmpname.findRev('/');
	if (ind1!=-1)
	{
		tmpname=tmpname.right(tmpname.length()-ind1-1);
		m_FileName=tmpname;
	}
	ind1=tmpname.findRev('.');
	if (ind1!=-1)
		tmpname=tmpname.left(ind1);
	ind1=tmpname.findRev('_');
	if (ind1!=-1)
		tmpname=tmpname.right(tmpname.length()-ind1-1);
	m_Id=atoi(tmpname);

	// read the mds file
	while (notEof)
	{
		line="";
		notEof=(MapFile.readLine(line,20000)!=-1);
		elem = none;
		if (line.length())
		{
			if ((line.length()>=2) && ((line.left(2)=="--") || (line.left(2)=="//")))
				continue;
			index2=0;
			while (1)
			{
				index1 = line.find('<',index2);
				if (index1==-1)
				{
					//finish element
					switch (elem)
					{
					case title:
						// title retrieved from the mds file to construct the map menu
						m_MapMenuName = param.Text;
						ind1 = m_MapMenuName.findRev('\\');
						//m_Restricted =((ind1!=-1) && (m_MapMenuName[ind1+1]=='*'));
						m_Info = param.Info;
						break;
					case beacon:
						// name of the beacon retrieved from the mds file
						pMapElement = new CMapBeacon(param);
						m_ElementTable.Add(pMapElement);
						break;
					case text:
						// information on maps
						pMapElement = new CMapText(param);
						m_ElementTable.Add(pMapElement);
						break;
					case vectors:
						// map of type vectors
						pMapElement = new CMapVectors(param);
						m_ElementTable.Add(pMapElement);
						break;
					case surface:
						// map of type surface
						pMapElement = new CMapSurface(param);
						m_ElementTable.Add(pMapElement);
						break;
					case none:
						break;
					default:
						break;
					}
					break;
				}
				index2 = line.find('>',index1+1);
				if (index2==-1)
					break;
				index3 = line.find('=',index1+1);
				Key = line.mid(index1+1,index3-index1-1);
				Key.stripWhiteSpace();
				Elem = line.mid(index3+1,index2-index3-1);
				Elem.stripWhiteSpace();
				key = None;
				for (int key2=Item;key2<None;key2++)
				{
					if (Key.compare(KeyNames[key2])==0)
					{
						key = (keywords)key2;
						break;
					}
				}
				// extract the attributes contained on mds files
				// all these attributes are explained in the MDS Format description file
				switch (key)
				{
				case None:
					continue;
					break;
				case Item:
					for (elem2=title;elem2<none;elem2++)
					{	
						if (Elem.compare(ElemNames[elem2])==0)
						{
							elem = (elements)elem2;
							break;
						}
					}
					if (elem==none)
						continue;
					break;
				case Color:
					param.Color=strtol(Elem,NULL,16);//no database color field;
					param.Color.setRgb(param.Color.red(),param.Color.green(),param.Color.blue());
					break;
				case Color2:
					param.Color2=strtol(Elem,NULL,16);//no database color field;
					param.Color2.setRgb(param.Color2.red(),param.Color2.green(),param.Color2.blue());
					break;
				case Font:
					ind1 = Elem.find('\"');
					if (ind1==-1)
						continue;
					ind2 = Elem.find('\"',ind1+1);
					if (ind2==-1)
						continue;
					param.Font = Elem.mid(ind1+1,ind2-ind1-1);
					break;
				case Size:
					param.FontSize = atoi(Elem);
					break;
				case Prio:
					param.Prio = atoi(Elem);
					break;
				case Text:
					ind1 = Elem.find('\"');
					if (ind1==-1)
						continue;
					ind2 = Elem.find('\"',ind1+1);
					if (ind2==-1)
						continue;
					param.Text = Elem.mid(ind1+1,ind2-ind1-1);
					break;
				case Pos:
					param.PointTable.RemoveAll();
					ind1 = Elem.find('(');
					if (ind1==-1) 
						continue;
					ind2 = Elem.find(',',ind1+1);
					if (ind2==-1) 
						continue;
					ind3 = Elem.find(')',ind2+1);
					if (ind3==-1) 
						continue;
					point.setX(atoi(Elem.mid(ind1+1,ind2-ind1-1)));
					point.setY(atoi(Elem.mid(ind2+1,ind3-ind2-1)));
					param.PointTable.Add(point);
					break;
				case PosTable:
					param.PointTable.RemoveAll();
					ind3 = Elem.find(' ');
					if (ind3==-1)
						continue;
					for (nbpoint = atoi(Elem.left(ind3));nbpoint>0;nbpoint--)
					{
						ind1 = Elem.find('(',ind3+1);
						if (ind1==-1) 
							continue;
						ind2 = Elem.find(',',ind1+1);
						if (ind2==-1) 
							continue;
						ind3 = Elem.find(')',ind2+1);
						if (ind3==-1) 
							continue;

						StrX = Elem.mid(ind1+1,ind2-ind1-1);
						StrX.stripWhiteSpace();
						if (StrX.compare("ARC")==0)
							point.setX(ARC);
						else
							point.setX(atoi(StrX));
						point.setY(atoi(Elem.mid(ind2+1,ind3-ind2-1)));
						param.PointTable.Add(point);
					}
					break;
				case Info:
					ind1=Elem.find("\"");
					if (ind1!=-1)
					{
						ind2 = Elem.find("\"",ind1+1);
						if (ind2==-1)
							ind2= Elem.length()-1;
						param.Info = Elem.mid(ind1+1,ind2-ind1-1);
					}
					break;
				case Beacon:

					ind1=Elem.find("\"");
					if (ind1!=-1)
					{
						ind2 = Elem.find("\"",ind1+1);
						if (ind2==-1)
							ind2= Elem.length()-1;
						param.Beacon = Elem.mid(ind1+1,ind2-ind1-1);
					}
					break;
				case Penstyle:
					param.PenStyle = Qt::SolidLine;
					ind1 = Elem.find(' ');
					if (ind1!=-1)
						Style = Elem.left(ind1);
					else
						Style = Elem;
					if (Style.compare("PS_USERSTYLE")==0)
					{
						param.PenStyle=Qt::SolidLine;//MPenStyle;
						for (int p=0;p<8;p++)
							param.Patt[p]=0;
						if (ind1==-1)
							continue;
						Elem = Elem.right(Elem.length()-ind1-1);
						if (Elem.length()<=16)
							ind2 = Elem.length();
						else
							ind2 = 16;
						for (ind1=0;ind1<ind2;ind1+=2)
							param.Patt[ind1/2] = atoi(Elem.mid(ind1,2));
					}
					else
					{
						for (ind1=0;ind1<5;ind1++)
						{
							if (Elem.compare(PenStyle[ind1])==0)
							{
								param.PenStyle = QtPenStyle[ind1];
								break;
							}
						}
					}
					break;
				case Brushstyle:
					ind1 = Elem.find(' ');
					if (ind1!=-1)
						Style = Elem.left(ind1);
					else
						Style = Elem;
					if (Style.compare("BS_SOLID")==0)
						param.BrushStyle=Qt::SolidPattern;
					if (Style.compare("BS_HATCHED")==0)
					{
						param.BrushStyle=Qt::HorPattern;
						if (ind1==-1)
							continue;
						Elem = Elem.right(Elem.length()-ind1-1);
						for (ind1=0;ind1<6;ind1++)
						{
							if (Elem.compare(HatchStyle[ind1])==0)
							{
								param.BrushStyle = QtHatchStyle[ind1];
								break;
							}
						}
					}
					if (Style.compare("BS_PATTERN")==0)
					{
						param.BrushStyle=Qt::CustomPattern;
						for (int p=0;p<8;p++)
							param.Patt[p]=0;
						if (ind1==-1)
							continue;
						Elem = Elem.right(Elem.length()-ind1-1);
						if (Elem.length()<=64)
							ind2 = Elem.length();
						else
							ind2 = 64;
						for (ind1=0;ind1<ind2;ind1++)
						{
							if (Elem[ind1]=='0')
							{
								param.Patt[ind1/8] += 1<<(7-(ind1%8));
							}
						}
					}
					break;
				case Mode:
					ind1 = Elem.find(' ');
					if (ind1==-1)
						continue;
					Elem = Elem.right(Elem.length()-ind1-1);
					break;
				default:
					break;
				}

			}
		}
	}
	m_ElemTable.SetSize(m_ViewTable.GetSize());
	// creates an instance to display the map via the CViewMap
	for (int view=0;view<m_ViewTable.GetSize();view++)
	{
		CViewMap* pViewMap = new CViewMap();
		pViewMap->Init(this,m_ViewTable[view]);
		m_ElemTable[view]=pViewMap;
		((CRwdView*)m_ViewTable[view])->AddElement(pViewMap);
	}
//strMessage.sprintf("end loading map %s", MapName.ascii());
//CAsdApp::GetApp()->WriteLogMsg(strMessage, LogError);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method gets the information text on a map having the highest priority
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CVisuMap::GetInformation(CView* pView,QPoint point,QString& info)
{
	int viewnbr=-1;
	for (int view=0;view<m_ViewTable.GetSize();view++)
	{
		if (m_ViewTable[view]==pView)
		{
			viewnbr=view;
			break;
		}
	}
	if (viewnbr!=-1)
	{
		bool fnd = false;
		for(int priority_nb = 1; priority_nb <= PRIO_MAP_MAX; priority_nb++)
		{
			//Get information maps by inverse priority order
			for(int pos = 0; pos < m_MapTable.GetSize() ;pos++)
			{
				if (m_MapTable[pos]->m_ElemTable[viewnbr]->GetInformation(point,priority_nb,info))
				{
					fnd = true;
					break;
				}
			}
			if (fnd)
				break;
		}
	}
}




//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method checks if the map is selected on the top level menu
/// 
///  RETURNS: 
///  true if the map is selected otherwise false
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
BOOL CVisuMap::IsSelected(CView* pView)
{
	for (int view=0;view<m_ViewTable.GetSize();view++)
	{
		if ((pView==NULL) || (m_ViewTable[view]==pView))
			return m_ElemTable[view]->m_Selected;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method turns the map selected on the radar window depending on selection
///	 parameter
/// 
///  RETURNS: 
///  true if the map is selected otherwise false
///                    
///  REQUIREMENT ID: CIAsdFctMapGen060, CIAsdFctMapGen065 
//-----------------------------------------------------------------------------
void CVisuMap::SetSelection(BOOL selection,CView* pView)
{
	for (int view=0;view<m_ViewTable.GetSize();view++)
	{
		if ((pView==NULL) || (m_ViewTable[view]==pView))
		{
			if (m_ElemTable[view]->m_Selected!=selection)
			{
				m_ElemTable[view]->m_Selected = selection;
				if (m_ElemTable[view]->m_Selected)
						m_ElemTable[view]->OnUpdate();
				if (!m_ElemTable[view]->m_Selected && m_ElemTable[view]->m_pLastSelectedElement)
				{
					m_ElemTable[view]->m_pLastSelectedElement->SelectElement(false);
					m_ElemTable[view]->m_pLastSelectedElement=NULL;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method gets the map menu according to the map file
/// 
///  RETURNS: 
///  the name of the map menu
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
QString CVisuMap::GetMenuName()
{	
	return m_MapMenuName;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method adds the map element in the radar window (pView)
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CVisuMap::AddView(CView* pView)
{
	m_ViewTable.Add(pView);
	for (int map=0;map<m_MapTable.GetSize();map++)
	{
		if (m_MapTable[map])
		{
			CViewMap* pElem = new CViewMap();
			pElem->Init(m_MapTable[map],pView);
			m_MapTable[map]->m_ElemTable.Add(pElem);
			((CRwdView*)pView)->AddElement(pElem);
			pElem->OnUpdate();
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method removes the map element from the radar window (pView)
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CVisuMap::RemoveView(CView* pView)
{
	for (int view=0;view<m_ViewTable.GetSize();view++)
	{
		if (pView==m_ViewTable[view])
		{
			m_ViewTable.RemoveAt(view);
			for (int map=0;map<m_MapTable.GetSize();map++)
			{
				if (m_MapTable[map])
				{
					delete m_MapTable[map]->m_ElemTable[view];
					m_MapTable[map]->m_ElemTable.RemoveAt(view);
				}
			}
			break;
		}
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method copy the elements of the source view in the destination view
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CVisuMap::CopyView(CView* pDestView,CView* pSrcView)
{
	int src=-1,dest=-1;
	for (int view=0;view<m_ViewTable.GetSize();view++)
	{
		if (pSrcView==m_ViewTable[view])
		{
			src=view;
		}
		if (pDestView==m_ViewTable[view])
		{
			dest=view;
		}
	}
	if ((src!=-1) && (dest!=-1))
	{
		for (int map=0;map<m_MapTable.GetSize();map++)
		{
			if (m_MapTable[map])
			{
				m_MapTable[map]->m_ElemTable[dest]->m_Selected=m_MapTable[map]->m_ElemTable[src]->m_Selected;
				m_MapTable[map]->m_ElemTable[dest]->OnUpdate();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method reloads a map already displayed if this one has changed
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctRcgMap020, CIAsdFctRcgMap025, CIAsdFctRcgMap026 
//-----------------------------------------------------------------------------
void CVisuMap::ReloadMap()
{
	// CDS BREAK NGT 08/06/2006 Bad method length rule is not violated considering
	//							only the real length of the function.

	QTime time1;
	time1.start();
#ifdef ENABLE_QDEBUG
	qDebug("Init0"+time1.toString("h:m:s:z"));
#endif
	CAsdMenu* pMenu=NULL;
	CArray<QString,QString> NameTable;
	CArray<DWORD,DWORD>  SizeTable;
	CArray<QTime,QTime> TimeTable;
	CArray<BYTE,BYTE> OKTable;
	DWORD size;
	BOOL NeedUpdate = FALSE,Fnd;
	QString FileSearch;
	int mapnb;

	FileSearch = CAsdApp::GetApp()->GetDataSetDir()+"/OfflineMaps";
    QDir fd(FileSearch);
    bool res=fd.exists();
    if (!res)
      return;
    fd.setFilter( QDir::Files | QDir::NoSymLinks );
    fd.setSorting( QDir::Name);
	// retrieves all the map files
	fd.setNameFilter("*.mds");
	const QFileInfoList *list = fd.entryInfoList();
    QFileInfoListIterator it( *list );
    QFileInfo *fi=NULL;
	
	time1.start();
#ifdef ENABLE_QDEBUG
	qDebug("Init1"+time1.toString("h:m:s:z"));
#endif
	fi = it.current();
	while (fi != 0)
	{
		// store the size and the last modified time of the map for the mds files
		NameTable.Add(fi->filePath());
		size = fi->size();;
		SizeTable.Add(size);
		TimeTable.Add(fi->lastModified().time());
		OKTable.Add(TRUE);
		++it;
		fi = it.current();
	}
	//modify of delete the existing maps
	time1.start();
#ifdef ENABLE_QDEBUG
	qDebug("Init2"+time1.toString("h:m:s:z"));
#endif
	for (mapnb=0;mapnb<m_MapTable.GetSize();mapnb++)
	{
		QString* mapname=NULL;
		long mapsize=0;
		QTime* mapmtime=NULL;
		m_MapTable[mapnb]->GetAttributes(mapname,mapsize,mapmtime);
		Fnd = FALSE;
		if (mapname)
		{
			for (int filenb=0;filenb<NameTable.GetSize();filenb++)
			{
				// for each map displayed, checks if the size and the time have changed
				// if so update the corresponding map
				if (*mapname == NameTable[filenb])
				{
					if ((((DWORD)mapsize!=SizeTable[filenb]) || (*mapmtime!=TimeTable[filenb])) && (SizeTable[filenb]!=0))
					{						
						CVisuMap* pMap=m_MapTable[mapnb];
      					m_MapTable[mapnb] = NULL;
						m_MapTable[mapnb] = new CVisuMap(NameTable[filenb]);

						for (int view=0;view<m_ViewTable.GetSize();view++)
						{
							BOOL bIsSelected=pMap->IsSelected(m_ViewTable[view]);
							if(bIsSelected)
								m_MapTable[mapnb]->SetSelection(true,m_ViewTable[view]);
						}
						delete [] pMap;
						NeedUpdate  = TRUE;
					}
					
					OKTable[filenb] = FALSE;					
					Fnd = TRUE;
					break;
				}
			}
		}

		//this map does not exist anymore
		if (!Fnd)
		{
			CVisuMap* pMap=m_MapTable[mapnb];
			m_MapTable[mapnb] = NULL;
			delete [] pMap;
			NeedUpdate  = TRUE;
		}
	}
	//clean the deleted maps
	for (mapnb=m_MapTable.GetSize()-1;mapnb>=0;mapnb--)
		if (m_MapTable[mapnb]==NULL)
			m_MapTable.RemoveAt(mapnb);
	//create the new maps
	time1.start();
#ifdef ENABLE_QDEBUG
	qDebug("Init3"+time1.toString("h:m:s:z"));
#endif
	for (int filenb=0;filenb<NameTable.GetSize();filenb++)
	{
		if ((OKTable[filenb]) && (SizeTable[filenb]!=0))
		{
			QFile file(NameTable[filenb]);
			if (file.open(IO_ReadOnly))
			{
				file.close();
				CVisuMap* pMap = new CVisuMap(NameTable[filenb]);
				m_MapTable.Add(pMap);
				NeedUpdate  = TRUE;
			}
		}
	}
	time1.start();
#ifdef ENABLE_QDEBUG
	qDebug("Init4"+time1.toString("h:m:s:z"));
#endif
	if (NeedUpdate)
	{
		for (int view=0;view<m_ViewTable.GetSize();view++)
		{
			m_ViewTable[view]->update();
		}
		if (m_MapTable.GetSize())
		{
			pMenu = new CAsdMenu();
		}
		for(int pos = 0; pos < m_MapTable.GetSize() ;pos++)
		{
			// update the map menu
			CAsdMenu* curmenu = pMenu;
			int newpos,curpos = 0;
			QString MAPName = m_MapTable[pos]->GetMenuName();
			do
			{
			newpos = MAPName.find("\\",curpos);
			if (newpos==-1)
			{
			 curmenu->AddItem(MAPName.right(MAPName.length()-curpos),ID_MAPS+pos);
			 break;
			}
			else
			{
			 QString menuname,name = MAPName.mid(curpos,newpos-curpos);
			 bool fnd = false;
			 for (int men=0;men<curmenu->GetItemCount();men++)
			 {
				menuname=curmenu->GetItemText(men);
				if (menuname==name)
				{
					curmenu=curmenu->GetPopupMenu(men);
					fnd = true;
					break;
				}
			 }
			 if (!fnd)
			 {
				 CAsdMenu* newmenu=new CAsdMenu();
				 curmenu->AddItem(name,newmenu);
	 			 curmenu=newmenu;
			 }
			}
			curpos = newpos+1;
			}
			while (newpos!=-1);
		}
		delete [] m_pMenu;
		m_pMenu = pMenu;		
	}
	//find version if existing
	QString mapname;
	for(int pos = 0; pos <m_MapTable.GetSize() ;pos++)
	{
		mapname = m_MapTable[pos]->GetMenuName();

		m_Version="No Map Version";
		if (mapname.find("VERSION")!=-1)
		{
			{
				if (m_MapTable[pos]->m_ElementTable.GetSize()>=1)
					m_Version = "Map Version :"+((CMapText*)m_MapTable[pos]->m_ElementTable[0])->m_Text;
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method loads the graphical route leg maps when the route is in modification
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CVisuMap::LoadGrlMap(CAsdView* view)
{
	QTime time1;
	time1.start();
	CArray<QString,QString> NameTable;
	CArray<DWORD,DWORD>  SizeTable;
	CArray<QTime,QTime> TimeTable;
	CArray<BYTE,BYTE> OKTable;
	DWORD size;
	BOOL Fnd;
	QString FileSearch;
	int mapnb;

	FileSearch = CAsdApp::GetApp()->GetDataSetDir();
    QDir fd(FileSearch);
    bool res=fd.exists();
    if (!res)
      return;
    fd.setFilter( QDir::Files | QDir::NoSymLinks );
    fd.setSorting( QDir::Name);
	// load the specific file for graphical route leg modification
	fd.setNameFilter("*.mds");
	const QFileInfoList *list = fd.entryInfoList();
    QFileInfoListIterator it( *list );
    QFileInfo *fi=NULL;
	
	CVisuMap* pMap=NULL;
	time1.start();
#ifdef ENABLE_QDEBUG
	qDebug("Init1"+time1.toString("h:m:s:z"));
#endif
	fi = it.current();
	while (fi != 0)
	{
		NameTable.Add(fi->filePath());
		size = fi->size();;
		SizeTable.Add(size);
		TimeTable.Add(fi->lastModified().time());
		OKTable.Add(TRUE);
		++it;
		fi = it.current();
	}
	time1.start();
	for (mapnb=0;mapnb<m_MapTable.GetSize();mapnb++)
	{
		QString* mapname=NULL;
		long mapsize=0;
		QTime* mapmtime=NULL;
		m_MapTable[mapnb]->GetAttributes(mapname,mapsize,mapmtime);
		Fnd = FALSE;
		if (mapname)
		{
			
			CVisuMap* pMap=m_MapTable[mapnb];
			pMap->m_IsSelectable = false;
		}
	}

	//create the specific new maps for graphical route leg modification
	time1.start();
#ifdef ENABLE_QDEBUG
	qDebug("Init3"+time1.toString("h:m:s:z"));
#endif
	for (int filenb=0;filenb<NameTable.GetSize();filenb++)
	{
		if ((OKTable[filenb]) && (SizeTable[filenb]!=0))
		{
			QFile file(NameTable[filenb]);
			if (file.open(IO_ReadOnly))
			{
				file.close();
				pMap = new CVisuMap(NameTable[filenb]);
				pMap->SetSelection(TRUE, view);
				pMap->m_IsSelectable = true;
				m_MapTable.Add(pMap);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method removes the graphical route leg maps when the route is nomore in
///  route modification 
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CVisuMap::RemoveGrlMap()
{
	QTime time1;
	time1.start();
	CArray<QString,QString> NameTable;
	CArray<DWORD,DWORD>  SizeTable;
	CArray<QTime,QTime> TimeTable;
	CArray<BYTE,BYTE> OKTable;
	DWORD size;
	QString FileSearch;
	int mapnb;

	FileSearch = CAsdApp::GetApp()->GetDataSetDir();
    QDir fd(FileSearch);
    bool res=fd.exists();
    if (!res)
      return;
    fd.setFilter( QDir::Files | QDir::NoSymLinks );
    fd.setSorting( QDir::Name);
	fd.setNameFilter("*.mds");
	const QFileInfoList *list = fd.entryInfoList();
    QFileInfoListIterator it( *list );
    QFileInfo *fi=NULL;
	
	CVisuMap* pMap=NULL;
	time1.start();
#ifdef ENABLE_QDEBUG
	qDebug("Init1"+time1.toString("h:m:s:z"));
#endif
	fi = it.current();
	while (fi != 0)
	{
		NameTable.Add(fi->filePath());
		size = fi->size();;
		SizeTable.Add(size);
		TimeTable.Add(fi->lastModified().time());
		OKTable.Add(TRUE);
		++it;
		fi = it.current();
	}
	time1.start();
	for (mapnb=0;mapnb<m_MapTable.GetSize();mapnb++)
	{
		QString* mapname=NULL;
		long mapsize=0;
		QTime* mapmtime=NULL;
		m_MapTable[mapnb]->GetAttributes(mapname,mapsize,mapmtime);
		if (mapname)
		{
			CVisuMap* pMap=m_MapTable[mapnb];
			pMap->m_IsSelectable=true;
            for (int filenb=0;filenb<NameTable.GetSize();filenb++)
            {
				// delete the maps specific for the graphical route leg modification
				if (*mapname == NameTable[filenb])
				{
					CVisuMap* pMap=m_MapTable[mapnb];
					m_MapTable[mapnb] = NULL;
					delete [] pMap;
					break;
				}
			}
		}
	}

        //clean the deleted maps
        for (mapnb=m_MapTable.GetSize()-1;mapnb>=0;mapnb--)
                if (m_MapTable[mapnb]==NULL)
                        m_MapTable.RemoveAt(mapnb);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method calls the reload map method at initialization
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CVisuMap::InitTable()
{
	m_pMenu = NULL;
	ReloadMap();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method removes the map element on the table and the associated map menu
///  if the map does not exist anymore
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CVisuMap::RemoveTables()
{
	for (int map=m_MapTable.GetSize()-1;map>=0;map--)
	{
		delete m_MapTable[map];
	}
	delete [] m_pMenu;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method is called with the default custom set to display the map
///  on the radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctMapGen010 
//-----------------------------------------------------------------------------
void CVisuMap::Setup(CView* pView,QString& Key,bool save)
{
	QString mapstr,filename;
	int pos=0;
	if (save)
	{
		for (int map=0;map<m_MapTable.GetSize();map++)
		{
			if (m_MapTable[map]->IsSelected(pView))
			{
				if (!mapstr.isEmpty())
					mapstr+=',';
				mapstr+=m_MapTable[map]->m_FileName;
			}
		}
		Key=mapstr;
	}
	else
	{
			mapstr=Key+",";
			for (int map=0;map<m_MapTable.GetSize();map++)
			{
				m_MapTable[map]->SetSelection(false,pView);
			}
			while (pos<(int)mapstr.length())
			{
				while (mapstr[pos]!=',')
					filename+=mapstr[pos++];
				for (int map=0;map<m_MapTable.GetSize();map++)
					if (m_MapTable[map]->m_FileName.compare(filename)==0)
						m_MapTable[map]->SetSelection(true,pView);
				pos++;
				filename="";
			}
			if (pView)
				((CAsdView*)pView)->update();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method updates the map menu from the radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctMapGen025 
//-----------------------------------------------------------------------------
void CVisuMap::OnMaps(UINT nID,CView* pView)
{
	int map = nID-ID_MAPS;
	if (map<m_MapTable.GetSize())
	{
		BOOL selection = m_MapTable[map]->IsSelected(pView); 
		selection = !selection;
		m_MapTable[map]->SetSelection(selection,pView);
		for (int view=0;view<m_ViewTable.GetSize();view++)
		{
			((CAsdView*)m_ViewTable[view])->update();
			((CAsdView*)m_ViewTable[view])->InvalidateRect(NULL);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method retrieves the map menu
/// 
///  RETURNS: 
///  a pointer of the map menu
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CAsdMenu* CVisuMap::GetMenu()
{
	return m_pMenu;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///	 This method retrieves a specific map according to the map menu ID parameter
/// 
///  RETURNS: 
///  a map of type CVisuMap
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CVisuMap* CVisuMap::GetMap(UINT nID)
{
	int map = nID-ID_MAPS;
	if (map<m_MapTable.GetSize())
	{
		return m_MapTable[map];
	}
	return NULL;
}


//-----------------------------------------------------------------------------
//
/// \class CViewMap
/// This class is used to display all the map elements (beacon, surface, text...)
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
CViewMap::CViewMap()
{
	m_Type=MapView;
	m_Selected=false;
	m_pLastSelectedElement=NULL;
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
CViewMap::~CViewMap()
{
	for (int elem=0;elem<m_ElemTable.GetSize();elem++)
		delete m_ElemTable[elem];
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates for each map element the corresponding displayed map
///     element
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewMap::Init(LPVOID pData,CView* pView,CDisplayElement* )
{
	CViewMapElement* pElem=NULL;
	m_pMap = (CVisuMap*)pData;
	m_pView = (CAsdView*) pView;
	for (int elem=0;elem<m_pMap->m_ElementTable.GetSize();elem++)
	{
		if (m_pMap->m_ElementTable[elem]->m_Kind==Ivectors)
		{
			pElem = new CViewVectors();
		}
		if (m_pMap->m_ElementTable[elem]->m_Kind==Isurface)
		{
			pElem = new CViewSurface();
		}
		if (m_pMap->m_ElementTable[elem]->m_Kind==Itext)
		{
			pElem = new CViewText();
		}
		if (m_pMap->m_ElementTable[elem]->m_Kind==Ibeacon)
		{
			pElem = new CViewBeacon();
		}
		pElem->Init(m_pMap->m_ElementTable[elem],pView,this);
		m_ElemTable.Add(pElem);
	}	
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when a click AB is performed on a map element
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewMap::OnRButtonDown(UINT nFlags,QPoint point)
{
	//nFlags is not used but is necessary for call of generic OnRButtonDown function
	if (m_Selected)
	{
		for (int elem=0;elem<m_ElemTable.GetSize();elem++)
		{
				m_ElemTable[elem]->OnRButtonDown(0,point);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when a click SB is performed on a map element
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewMap::OnLButtonDown(UINT nFlags,QPoint point)
{
	//nFlags is not used but is necessary for call of generic OnRButtonDown function
	if (m_Selected)
	{
		for (int elem=0;elem<m_ElemTable.GetSize();elem++)
		{
				m_ElemTable[elem]->OnLButtonDown(0,point);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when a map element has to be drawn
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctRwsFltHgt030 
//-----------------------------------------------------------------------------
void CViewMap::OnDraw(QPainter* pDC,int Prio)
{
	if (m_Selected)
	{
		for (int elem=0;elem<m_ElemTable.GetSize();elem++)
		{
			m_ElemTable[elem]->OnDraw(pDC,Prio);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when a map element has to be updated
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctRwsFltHgt030 
//-----------------------------------------------------------------------------
void CViewMap::OnUpdate()
{
	if (m_Selected)
	{
		for (int elem=0;elem<m_ElemTable.GetSize();elem++)
		{
			m_ElemTable[elem]->OnUpdate();
		}
		m_pView->InvalidateRect(NULL);
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the priority of a map in order to be selected 
///		on the radar window
/// 
///  RETURNS: 
///  the priority of the map
///                    
///  REQUIREMENT ID: CIAsdFctMapGen045, CIAsdFctMapGen050, CIAsdFctMapGen041
//-----------------------------------------------------------------------------
int CViewMap::GetPriority(QPoint point,bool Check)
{
  int prio=PRIO_MAP_MAX;
	QString info;
	bool Found = false;

	if (m_Selected)
	{
		CViewMapElement* pLastMapElem=m_pLastSelectedElement;
		CMapElement* pElem = NULL;

		for(int priority_nb = 1; priority_nb < prio; priority_nb++)
		{
			for (int elem=m_ElemTable.GetSize()-1;elem>=0;elem--)
			{						
				pElem =(m_ElemTable[elem]->GetInformation(point,priority_nb,info));
				if ((pElem) && (info.length()) && m_pMap->m_IsSelectable)
				{
					CVisuMap::m_GrlBeacon = info;
					m_pLastSelectedElement=m_ElemTable[elem];
					prio=priority_nb;
					Found = true;
					break;
				}
			}
			if (Found)
				break;
		}

		if (prio!=PRIO_MAP_MAX)
		{
			if ((Check) && (pLastMapElem) && (pLastMapElem!=m_pLastSelectedElement))
			{
				if (pLastMapElem)
					pLastMapElem->SelectElement(false);
				if (m_pLastSelectedElement)
					m_pLastSelectedElement->SelectElement(true);
			}
			return PRIO_MAP_MAX-prio;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method selects a map element on the radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewMap::SelectElement(bool Select)
{
	if (m_Selected)
	{
		if (m_pLastSelectedElement)
			m_pLastSelectedElement->SelectElement(Select);
		m_pView->InvalidateRect(NULL);
		if (!Select)
			m_pLastSelectedElement=NULL;
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves information on a map element on the radar window
/// 
///  RETURNS: 
///  the information of a map element
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMapElement* CViewMap::GetInformation(QPoint point,int priority,QString& info)
{
	info = "";
	if (m_Selected)
	{
	CMapElement* pElem = NULL;
	for (int elem=0;elem<m_ElemTable.GetSize();elem++)
	{
			pElem =(m_ElemTable[elem]->GetInformation(point,priority,info));
			if (pElem)
				break;
	}
	if ((pElem) && (!info.length()))
	{
		info = m_pMap->m_Info;
	}
	if (pElem)
		return pElem;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
/// \class CViewVectors
/// This class is used to display the map element of type vector
// 
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the contructor element
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CViewVectors::CViewVectors()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the contructor element
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CViewVectors::CViewVectors(CViewVectors& pViewVectors)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor element
/// 
///  RETURNS: 
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CViewVectors::~CViewVectors()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method stores the attributes of type vector in order to display it
///     on a radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewVectors::Init(LPVOID pData,CView* pView,CDisplayElement* )
{
	m_pVector = (CMapVectors*)pData;
	m_pView = (CAsdView*)pView;
	m_Select=false;
	OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method draws the map of type vector in the radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID:  
//-----------------------------------------------------------------------------
void CViewVectors::OnDraw(QPainter* pDC,int Prio)
{
	if (Prio!=m_pVector->m_Prio)
		return;
	QPen pen(m_Color,0,(Qt::PenStyle)m_pVector->m_PenStyle);
	pDC->setPen(pen);
					
	for (int pnt=1;pnt<m_PointTable.GetSize();pnt++)
	{
		if (m_PointTable[pnt].x()!=(int)ARC)
		{
			pDC->drawLine(m_PointTable[pnt-1],m_PointTable[pnt]);
		}
		else
		{
			QRect rect=QRect(QPoint(m_PointTable[pnt+1].x()-m_PointTable[pnt].y(),m_PointTable[pnt+1].y()-m_PointTable[pnt].y()),
                  QPoint(m_PointTable[pnt+1].x()+m_PointTable[pnt].y(),m_PointTable[pnt+1].y()+m_PointTable[pnt].y()));
			rect=rect.normalize();
			QPoint pt=m_PointTable[pnt+1];
			
			int anglestart=(int)(atan2(double(m_PointTable[pnt-1].y()-pt.y()),double(m_PointTable[pnt-1].x()-pt.x()))*180.0*16.0/PI);
			int angleend=(int)(atan2(double(m_PointTable[pnt+2].y()-pt.y()),double(m_PointTable[pnt+2].x()-pt.x()))*180.0*16.0/PI);
			
			if (m_PointTable[pnt-1].x()>m_PointTable[pnt+1].x())
				pDC->drawArc(rect,90-anglestart,(angleend-anglestart));
			else
				pDC->drawArc(rect,90-anglestart,-(angleend-anglestart));			
			pnt+=2;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the attributes of the vector map type
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctMapGen045, CIAsdFctMapGen050 
//-----------------------------------------------------------------------------
void CViewVectors::OnUpdate()
{
	int i;
	m_PointTable.RemoveAll();
	for (i=0;i<m_pVector->m_PointTable.GetSize();i++)
	{
		m_PointTable.Add(m_pVector->m_PointTable[i]);
		if (m_PointTable[i].x()==(int)ARC)
		{
			QSize size(m_PointTable[i].y(),m_PointTable[i].y());
			m_pView->LPtoDP(&size);
			m_PointTable[i].setY(size.height());
		}
		else
			m_pView->LPtoDP(&m_PointTable[i]);
	}
	m_BoundingRect = QRect(m_PointTable[0],m_PointTable[0]);
	for (i=0;i<m_PointTable.GetSize();i++)
	{
		if (m_PointTable[i].x()!=(int)ARC)
		{
		if (m_PointTable[i].x()<m_BoundingRect.left())
			m_BoundingRect.setLeft(m_PointTable[i].x());
		if (m_PointTable[i].x()>m_BoundingRect.right())
			m_BoundingRect.setRight(m_PointTable[i].x());
		if (m_PointTable[i].y()<m_BoundingRect.top())
			m_BoundingRect.setTop(m_PointTable[i].y());
		if (m_PointTable[i].y()>m_BoundingRect.bottom())
			m_BoundingRect.setBottom(m_PointTable[i].y());
		}
		else
		{
			QRect rect=QRect(QPoint(m_PointTable[i+1].x()-m_PointTable[i].y(),m_PointTable[i+1].y()-m_PointTable[i].y()),QPoint(m_PointTable[i+1].x()+m_PointTable[i].y(),m_PointTable[i+1].y()+m_PointTable[i].y()));
			rect=rect.normalize();
			m_BoundingRect|=rect;
			i+=2;
		}
	}
	m_BoundingRect=m_BoundingRect.normalize();
	if (!m_Select)
		m_Color = CColorMngr::TransformColor(m_pVector->m_Color,LineCategory);
	else
		m_Color = CColorMngr::TransformColorNb(CColorMngr::GetColorNb("SELECTED_FRGRND"),LineCategory);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the information on vector type maps
/// 
///  RETURNS: 
///  the attributes of the map vector element
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMapElement* CViewVectors::GetInformation(QPoint point,int priority,QString &info)
{
	QRect bound;
	QPoint vect1,vect2,vect3,vect4;
	int dist,vectprod,dist2;
	double ang1,ang2,ang3;
	bool fnd = false;
	if ((priority==m_pVector->m_Prio) && (m_BoundingRect.contains(point)))
	{
		for (int pnt=0;pnt<m_PointTable.GetSize()-1;pnt++)
		{
			if (m_PointTable[pnt+1].x()!=(int)ARC)
			{
				bound = QRect(m_PointTable[pnt],m_PointTable[pnt+1]);
				bound=bound.normalize();
				if (bound.contains(point))
				{
					vect1 = m_PointTable[pnt+1]-m_PointTable[pnt];
					vect2 = point - m_PointTable[pnt];
					dist = (vect1.x()*vect1.x())+(vect1.y()*vect1.y());
					vectprod = (vect1.x()*vect2.x())+(vect1.y()*vect2.y());
					vect3.setX(int(double(vect1.x())*double(vectprod)/double(dist)));
					vect3.setY(int(double(vect1.y())*double(vectprod)/double(dist)));
					vect4 = vect2-vect3;
					dist2 = vect4.x()*vect4.x()+vect4.y()*vect4.y();
					if (dist2<=20)
					{
						fnd = true;
						break;
					}
				}
			}
			else
			{
				vect3 = point-m_PointTable[pnt+2];
				dist = (int)sqrt(vect3.x()*vect3.x()+vect3.y()*vect3.y());
				if (abs(dist-abs(m_PointTable[pnt+1].y()))<=20)
				{//check angles
					vect1 = m_PointTable[pnt]-m_PointTable[pnt+2];
					vect2 = m_PointTable[pnt+3]-m_PointTable[pnt+2];
					if (m_PointTable[pnt+1].y()>0)
					{
						ang1 = atan2(vect1.x(),vect1.y());
						ang2 = atan2(vect2.x(),vect2.y());
					}
					else
					{
						ang1 = atan2(vect2.x(),vect2.y());
						ang2 = atan2(vect1.x(),vect1.y());
					}
					ang3 = atan2(vect3.x(),vect3.y());
					if (ang2<ang1)
					{
						if (((ang3>=ang1) && (ang3<=PI)) || ((ang3>=-PI) && (ang3<=ang2)))
						{
							fnd = true;
							break;
						}
					}
					else
					{
						if ((ang3>=ang1) && (ang3<=ang2))
						{
							fnd = true;
							break;
						}
					}
				}
				pnt+=2;
			}
		}
	}
	if (fnd)
	{
		info = m_pVector->m_Info;
		return m_pVector;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
/// \class CViewSurface
/// This class is used to display the map element of type surface
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
CViewSurface::CViewSurface()
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
CViewSurface::CViewSurface(CViewSurface& pViewSurface)
{
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
CViewSurface::~CViewSurface()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method stores the attributes of type surface in order to display it
///     on a radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewSurface::Init(LPVOID pData,CView* pView,CDisplayElement* )
{
	m_pSurface = (CMapSurface*)pData;
	m_pView = (CAsdView*)pView;
	m_Select=false;
	OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method draws the map of type surface in the radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID:  
//-----------------------------------------------------------------------------
void CViewSurface::OnDraw(QPainter* pDC,int Prio)
{
	if ( (Prio!=m_pSurface->m_Prio))
		return;
	QPen pen(m_Color,0,(Qt::PenStyle)m_pSurface->m_PenStyle);
	pDC->setPen(pen);						
	QBrush brush(m_Color2,(Qt::BrushStyle)m_pSurface->m_BrushStyle);

	if (m_pSurface->m_BrushStyle==Qt::CustomPattern)
	{
		QBitmap bmp(8,8,&m_pSurface->m_Patt[0]);
		brush.setPixmap(bmp);
		pDC->setBackgroundColor(QColor(0,0,0));
		pDC->setPen(m_Color);
	}
	pDC->setBrush(brush);
	QPointArray pnttab(m_PointTable.GetSize());
	for (int i=0;i<m_PointTable.GetSize();i++)
		pnttab[i]=m_PointTable[i];
	pDC->drawPolygon(pnttab,FALSE,0,m_PointTable.GetSize());

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the attributes of the surface map type
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctMapGen045, CIAsdFctMapGen050 
//-----------------------------------------------------------------------------
void CViewSurface::OnUpdate()
{
	m_PointTable.RemoveAll();
	double ang1,ang2,curang,step;
	QPoint vect1,vect2,curpos;
	for (int pnt=0;pnt<m_pSurface->m_PointTable.GetSize();pnt++)
	{
		if (m_pSurface->m_PointTable[pnt].x()!=(int)ARC)
		{
			m_PointTable.Add(m_pSurface->m_PointTable[pnt]);
			m_pView->LPtoDP(&m_PointTable[m_PointTable.GetSize()-1]);
		}
		else
		{
			vect1 = m_pSurface->m_PointTable[pnt-1]-m_pSurface->m_PointTable[pnt+1];
			vect2 = m_pSurface->m_PointTable[pnt+2]-m_pSurface->m_PointTable[pnt+1];
			ang1 = atan2(vect1.x(),vect1.y());
			ang2 = atan2(vect2.x(),vect2.y());
			if (m_pSurface->m_PointTable[pnt].y()<0)
			{
				step = 0.1;
				if (ang2<=ang1)
					ang2 += 2*PI;
			}
			else
			{
				step = -0.1;
				if (ang2>=ang1)
					ang1 +=2*PI;
			}
			curang = ang1+step;
			while (((m_pSurface->m_PointTable[pnt].y()<0) &&(curang<ang2)) || ((m_pSurface->m_PointTable[pnt].y()>0) && (curang>ang2)))
			{
				curpos.setX(m_pSurface->m_PointTable[pnt+1].x()+(int)(abs(m_pSurface->m_PointTable[pnt].y())*sin(curang)));
				curpos.setY(m_pSurface->m_PointTable[pnt+1].y()+(int)(abs(m_pSurface->m_PointTable[pnt].y())*cos(curang)));
				m_PointTable.Add(curpos);
				m_pView->LPtoDP(&m_PointTable[m_PointTable.GetSize()-1]);
				curang +=step;
			}
			m_PointTable.Add(m_pSurface->m_PointTable[pnt+2]);
			m_pView->LPtoDP(&m_PointTable[m_PointTable.GetSize()-1]);
			pnt+=2;
		}
	}

	m_BoundingRect = QRect(m_PointTable[0],m_PointTable[0]);
	for (int i=0;i<m_PointTable.GetSize();i++)
	{
		{
		if (m_PointTable[i].x()<m_BoundingRect.left())
			m_BoundingRect.setLeft(m_PointTable[i].x());
		if (m_PointTable[i].x()>m_BoundingRect.right())
			m_BoundingRect.setRight(m_PointTable[i].x());
		if (m_PointTable[i].y()<m_BoundingRect.top())
			m_BoundingRect.setTop(m_PointTable[i].y());
		if (m_PointTable[i].y()>m_BoundingRect.bottom())
			m_BoundingRect.setBottom(m_PointTable[i].y());
		}
	}
	m_BoundingRect=m_BoundingRect.normalize();
	if (!m_Select)
	{
		m_Color = CColorMngr::TransformColor(m_pSurface->m_Color,OtherMapCategory);
		m_Color2 = CColorMngr::TransformColor(m_pSurface->m_Color2,OtherMapCategory);
	}
	else
	     {
	       m_Color = CColorMngr::TransformColorNb(CColorMngr::GetColorNb("SELECTED_FRGRND"),OtherMapCategory);
	       m_Color2 = CColorMngr::TransformColorNb(CColorMngr::GetColorNb("SELECTED_FRGRND"),OtherMapCategory);
	     }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the information on surface type maps
/// 
///  RETURNS: 
///  the attributes of the map surface element
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMapElement* CViewSurface::GetInformation(QPoint point,int priority,QString &info)
{
	if ((priority==m_pSurface->m_Prio) && (m_BoundingRect.contains(point)))
	{
		QPointArray pnttab(m_PointTable.GetSize());
		for (int i=0;i<m_PointTable.GetSize();i++)
			pnttab[i]=m_PointTable[i];
		QRegion rgn(pnttab);

		if (rgn.contains(point))
		{
			info = m_pSurface->m_Info;
			return m_pSurface;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
/// \class CViewText
/// This class is used to display the map element of type text
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
CViewText::CViewText()
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
CViewText::CViewText(CViewText& pViewText)
{
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
CViewText::~CViewText()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method stores the attributes of type text in order to display it
///     on a radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewText::Init(LPVOID pData,CView* pView,CDisplayElement* )
{
	m_pText = (CMapText*)pData;
	m_pView = (CAsdView*)pView;
	m_Select=false;
	OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method draws the map of type text in the radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID:  
//-----------------------------------------------------------------------------
void CViewText::OnDraw(QPainter* pDC ,int Prio)
{
	if (Prio!=m_pText->m_Prio)
		return;
     QFont fontsymbol(m_pText->m_Font,m_pText->m_FontSize/10);
     pDC->setFont(fontsymbol);
     pDC->setPen(m_Color);
     QSize TextExt_Symb,TextOffset;
	 QRect tmp=pDC->boundingRect(0,0,0,0,Qt::AlignLeft,"D");
	 TextOffset = QSize(tmp.width(),tmp.height());
	 tmp=pDC->boundingRect(0,0,0,0,Qt::AlignLeft,m_pText->m_Text);
     TextExt_Symb = QSize(tmp.width(),tmp.height());
	 if (!m_pText->m_IsCentered)
	 {
	     pDC->drawText(m_Point.x()+TextOffset.width(),m_Point.y()-TextOffset.height(),m_pText->m_Text);
		 m_BoundingRect = QRect(QPoint(m_Point.x()+TextOffset.width(),m_Point.y()+TextOffset.height()),QPoint(m_Point.x()+TextOffset.width()+TextExt_Symb.width(),m_Point.y()+TextOffset.height()-TextExt_Symb.height()));
	 }
	 else
	 {
	     pDC->drawText(m_Point.x()-TextExt_Symb.width()/2,m_Point.y()-TextExt_Symb.height()/2,m_pText->m_Text);
		 m_BoundingRect = QRect(QPoint(m_Point.x()-TextExt_Symb.width()/2,m_Point.y()-TextExt_Symb.height()/2),QPoint(m_Point.x()+TextExt_Symb.width()/2,m_Point.y()+TextExt_Symb.height()/2));
	 }
	 m_BoundingRect=m_BoundingRect.normalize();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the attributes of the text map type
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctMapGen045, CIAsdFctMapGen050 
//-----------------------------------------------------------------------------
void CViewText::OnUpdate()
{
	m_Point = m_pText->m_Point;
	m_pView->LPtoDP(&m_Point);
	if (!m_Select)
		m_Color = CColorMngr::TransformColor(m_pText->m_Color,OtherMapCategory);
	else
	    m_Color = CColorMngr::TransformColorNb(CColorMngr::GetColorNb("SELECTED_FRGRND"),OtherMapCategory);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the information on text type maps
/// 
///  RETURNS: 
///  the attributes of the map surface element
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMapElement* CViewText::GetInformation(QPoint point,int priority,QString &info)
{
	if ((priority==m_pText->m_Prio) && (m_BoundingRect.contains(point)))
	{
		info = m_pText->m_Info;
		return m_pText;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
/// \class CViewBeacon
/// This class is used to display the map element of type beacon
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
CViewBeacon::CViewBeacon()
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
CViewBeacon::CViewBeacon(CViewBeacon& pViewBeacon)
{
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
CViewBeacon::~CViewBeacon()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method stores the attributes of type beacon in order to display it
///     on a radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewBeacon::Init(LPVOID pData,CView* pView,CDisplayElement* )
{
	m_pBeacon = (CMapBeacon*)pData;
	m_pView = (CAsdView*)pView;
	m_pSymbol = CGraphicSymbol::GetSymbol(m_pBeacon->m_Char);
	m_Select=false;
	OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method draws the map of type beacon in the radar window
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctMapGen045, CIAsdFctMapGen050 
//-----------------------------------------------------------------------------
void CViewBeacon::OnDraw(QPainter* pDC ,int Prio)
{
	if (Prio!=m_pBeacon->m_Prio)
		return;
	if (m_pSymbol)
	{
	    pDC->setPen(m_Color);
		if (m_pView->RectVisible(&m_BoundingRect))
			m_pSymbol->Draw(pDC,m_Point);
	}
	else
	{
     QFont fontsymbol(m_pBeacon->m_Font,m_pBeacon->m_Fontsize/10);
     pDC->setFont(fontsymbol);
     pDC->setPen(m_Color);
     QString symbol = ""+m_pBeacon->m_Char;
     QSize TextExt_Symb;
	 QRect tmp=pDC->boundingRect(0,0,0,0,Qt::AlignLeft,symbol);
     TextExt_Symb = QSize(tmp.width()/2,tmp.height()/2);
     pDC->drawText(m_Point.x()-TextExt_Symb.width(),m_Point.y()-TextExt_Symb.height(),symbol);
	 m_BoundingRect = QRect(QPoint(m_Point.x()-TextExt_Symb.width(),m_Point.y()-TextExt_Symb.height()),QPoint(m_Point.x()+TextExt_Symb.width(),m_Point.y()+TextExt_Symb.height()));
	 m_BoundingRect=m_BoundingRect.normalize();
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method updates the attributes of the beacon map type
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: CIAsdFctMapGen045, CIAsdFctMapGen050 
//-----------------------------------------------------------------------------
void CViewBeacon::OnUpdate()
{
	m_Point = m_pBeacon->m_Point;
	m_pView->LPtoDP(&m_Point);
	if (!m_Select)
		m_Color = CColorMngr::TransformColor(m_pBeacon->m_Color,NavCategory);
	else
	    m_Color = CColorMngr::TransformColorNb(CColorMngr::GetColorNb("SELECTED_FRGRND"),NavCategory);
	if (m_pSymbol)
	  m_BoundingRect=m_pSymbol->GetBoundingRect(m_Point);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method starts the modification of a route upon a Click AB on a beacon
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewBeacon::OnLButtonDown(UINT nFlags,QPoint point)
{
/*	bool Selected = false;

	CAsdApp* pApp = CAsdApp::GetApp();
	if ( pApp->getCurrentRoute() )
	{
		Selected = pApp->getCurrentRoute()->m_Select;
		pApp->getCurrentRoute()->m_pParent->m_SelElem = route;
	}

	if (m_Select && Selected)
	{
		if (pApp->getCurrentRoute())
		{
			pApp->getCurrentRoute()->ClickABOnRoute(nFlags, point, m_pBeacon->m_Info);
		}
	}*/
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method ends the modification of a route upon a Click SB on a beacon
/// 
///  RETURNS: 
///  void
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CViewBeacon::OnRButtonDown(UINT nFlags,QPoint point)
{
/*	bool Selected = false;

	if (CAsdApp::GetApp()->getRoute())
		Selected = CAsdApp::GetApp()->getRoute()->m_Select;

	if (m_Select && Selected)
	{
		if (CAsdApp::GetApp()->getRoute())
		{
//			CAsdApp::GetApp()->getRoute()->ClickSBOnRoute(nFlags, point, m_pBeacon->m_Info);
		}
	}*/
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the information on beacon type maps
/// 
///  RETURNS: 
///  the attributes of the map surface element
///                    
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
CMapElement* CViewBeacon::GetInformation(QPoint point,int priority,QString &info)
{
	if ((priority==m_pBeacon->m_Prio) && (m_BoundingRect.contains(point)))
	{
		info = m_pBeacon->m_Info;
		return m_pBeacon;
	}
	return NULL;
}




//--------------------------------< THE END >----------------------------------
