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
//  FILENAME: WdwMngr.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CWdwMngr which manages all actions or interactions
//		on windows.
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
//  Revision 1  2009/08/31 dalleins
//
//
//
// 
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include <stdlib.h>
#include "WdwMngr.h"
#include "./CommonFiles/IniFile.h"
#include "AsdApp.h"
#if (defined Q_WS_X11)
#include <X11/X.h>
#include <X11/Xlib.h>
#include "WinHints.h"
#include "WfAttrib.h"
#endif
#include "ImportantTrack.h" 

//-GLOBAL DATA-----------------------------------------------------------------

CArray<QString,QString> m_PrioNameTable;
CArray<WORD,WORD> m_PrioTable;

CArray<WORD,WORD> m_WndPrioTable;
CArray<QWidget*,QWidget*> m_WndTable;


CArray<bool,bool> m_baCollapsed; 
CArray<QSize,QSize> m_qaSize;
CArray<QSize,QSize> m_qaMinSize;
CImportantTrack m_pImportantTrackTable;
CArray<QWidget*,QWidget*> m_pRecordMove;
CArray<QPoint,QPoint> m_qpMove;
CArray<long,long> m_qAttrib;

//-LOCAL DATA------------------------------------------------------------------
 

//-PROTOTYPES---------------------------------------------------------
//-----------------------------------------------------------------------------
//
/// \class CWdwMngr
/// This class is used to managed ASD windows 
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves all window priorities and sets it on the table
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::InitTables()
{
	m_pImportantTrackTable.Init();
	QString AllKey,PrioKey,WindowName;
	int pos=0,alllen,prio,len;

	CIniFile file(CAsdApp::GetApp()->GetApplicationIni());
	alllen=file.GetIniProfileSection("WND_PRIORITIES",PrioKey);
	pos=0;
	while(pos<alllen)
	{
		while (PrioKey[pos]!=0)
			WindowName+=PrioKey[pos++];
		len=file.GetIniProfileString("WND_PRIORITIES",WindowName,AllKey);
		WindowName=WindowName.upper();
		prio = atoi(AllKey);
		m_PrioTable.Add(prio);
		m_PrioNameTable.Add(WindowName);
		pos++;
		WindowName="";
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method adds the window in the table  sorted by priority 
///		in order to be managed and set the window
///		in a decollapse or collapse form depending on IsCollapse parameter
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWdwMngr::AddWindow(QWidget* pWnd,QString PrioName, int attrib, bool IsCollapse)
{
	int prio=0,i;
	bool Found=false;
	QWidget* widget=NULL;
	QPoint pnt = QPoint(-2,-2);
	long Attrib =-1;
	
	m_pImportantTrackTable.AddWindow(PrioName,pWnd); 
	for (i=0;i<m_PrioNameTable.GetSize();i++)
	{
		if (PrioName.upper().compare(m_PrioNameTable[i])==0)
		{
			prio=m_PrioTable[i];
			break;
		}
	}
	for (i=0;i<m_WndTable.GetSize();i++)
	{
		if (m_WndPrioTable[i]>=prio)
		{
			m_WndTable.InsertAt(i,pWnd);
			m_WndPrioTable.InsertAt(i,prio);
			m_baCollapsed.InsertAt(i,IsCollapse);
			m_qaSize.InsertAt(i,pWnd->size());
			m_qaMinSize.InsertAt(i,pWnd->minimumSize());
			m_pRecordMove.InsertAt(i,widget);
			m_qpMove.InsertAt(i,pnt);
			m_qAttrib.InsertAt(i,Attrib);
			Found=true;
			break;
		}
	}

	if (!Found)
	{
		m_WndTable.Add(pWnd);
		m_WndPrioTable.Add(prio);
		m_baCollapsed.Add(IsCollapse);
		m_qaSize.Add(pWnd->size());
		m_qaMinSize.Add(pWnd->minimumSize());
		m_pRecordMove.Add(widget);
                m_qpMove.Add(pnt);
		m_qAttrib.Add(Attrib);
		qDebug("Window Prio:%s",PrioName.ascii());
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method removes the window from the table because
///		the window has been closed
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::RemoveWindow(QWidget* pWnd)
{
	CWin_Hints* pCWinHints=CAsdApp::GetApp()->GetWinHints();


	m_pImportantTrackTable.RemoveWindow(pWnd); 
	for (int i=(m_WndTable.GetSize()-1);i>=0;i--)
	{
		if (m_WndTable[i]==pWnd)
		{
			m_WndTable.RemoveAt(i);
			m_WndPrioTable.RemoveAt(i);
			m_baCollapsed.RemoveAt(i);
			m_qaSize.RemoveAt(i);
			m_qaMinSize.RemoveAt(i);
			m_pRecordMove.RemoveAt(i);
			m_qpMove.RemoveAt(i);
			m_qAttrib.RemoveAt(i);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method ckecks if the window lies inside of the screen
/// 
///  RETURNS: 
///		true if the window lies inside of the screen otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWdwMngr::CheckWindowInside(QRect qrWindow, int* monitor)
{
	QDesktopWidget* pDesktop=QApplication::desktop();
	QPoint pnt[5];
	pnt[0]=qrWindow.center();
	pnt[1]=qrWindow.topLeft();
	pnt[2]=qrWindow.topRight();
	pnt[3]=qrWindow.bottomRight();
	pnt[4]=qrWindow.bottomLeft();

	bool identical=true;
	for (int j=0;j<5;j++)
	{
		monitor[j]= pDesktop->screenNumber(pnt[j]);
		if ((monitor[j]==-1) || ((j>0) && (monitor[j]!=monitor[0])))
		identical = false;
	}
	return identical;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method moves a window if this one does not lie inside of the screen
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::MoveWindowInside(QWidget* w)
{ 
	QRect rect,monrect;
	int monitor[5],curmonitor;
	bool identical = true;
	QSize offset;
	int i,j;
	QDesktopWidget* pDesktop=QApplication::desktop();
	if (!m_WndTable.GetSize())
	return;

	for (i=0;i<m_WndTable.GetSize();i++)
	{
		if (m_WndTable[i]==w)
		{
			rect=w->frameGeometry();
			if (rect.width() - w->width()>8)
				return;
			identical=CheckWindowInside(rect,monitor);
			/// the window lies outside of the screen
			if (!identical)
			{
				curmonitor=-1;
				for (j=0;j<5;j++)
				{
					if (monitor[j]!=-1)
					{
						curmonitor=monitor[j];
						break;
					}
				}
				// get the right screen number to move it on the primary screen if the window
				// is too big on the current screen
				// then move the window on this screen
				monrect = pDesktop->screenGeometry(curmonitor);
				if (rect.width()> monrect.width() || rect.height()> monrect.height())
					curmonitor=-1;
				if (curmonitor==-1)
					curmonitor= pDesktop->primaryScreen();
				monrect = pDesktop->screenGeometry(curmonitor);              
				offset = QSize(0,0);
				if (rect.left()<monrect.left())
					offset.setWidth(monrect.left()-rect.left());
				if ((rect.width()<=monrect.width()) && (rect.right()>monrect.right()))
					offset.setWidth(monrect.right()-rect.right());
				if (rect.top()<monrect.top())
					offset.setHeight(monrect.top()-rect.top());
				if ((rect.height()<=monrect.height()) && (rect.bottom()>monrect.bottom()))
					offset.setHeight(monrect.bottom()-rect.bottom());
				if ((rect.x()!=rect.left()+offset.width()) || (rect.y()!=rect.top()+offset.height()))
					CAsdApp::GetApp()->RecordMove(w,QPoint(rect.left()+offset.width(),rect.top()+offset.height()));
			}
			else
				CAsdApp::GetApp()->CancelMove(w);
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sorts the window by priority order in the table
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::SwitchOrderWindowTable(int i, int priohead)
{
	QWidget* pTmp2=m_WndTable[i];
	m_WndTable.RemoveAt(i);
	m_WndTable.InsertAt(priohead,pTmp2);

	bool bTmp3=m_baCollapsed[i];
	m_baCollapsed.RemoveAt(i);
	m_baCollapsed.InsertAt(priohead,bTmp3);

	QSize qsTemp4=m_qaSize[i];
	m_qaSize.RemoveAt(i);
	m_qaSize.InsertAt(priohead,qsTemp4);

	QSize qsTemp5=m_qaMinSize[i];
	m_qaMinSize.RemoveAt(i);
	m_qaMinSize.InsertAt(priohead,qsTemp5);

	QWidget* qsTemp6=m_pRecordMove[i];
	m_pRecordMove.RemoveAt(i);
    m_pRecordMove.InsertAt(priohead,qsTemp6);

	QPoint qsTemp7=m_qpMove[i];
    m_qpMove.RemoveAt(i);
    m_qpMove.InsertAt(priohead,qsTemp7);

	long qsTemp8=m_qAttrib[i];
	m_qAttrib.RemoveAt(i);
	m_qAttrib.InsertAt(priohead,qsTemp8);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the windows (w) above or behind other windows
///		according to the priority order sets on the window table.
///		If this window has the same priority than another window, the windows 
///		which have the focus is set above
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWdwMngr::OrderWindows(QWidget* w)
{
  QWidget* pTmp=NULL;
  QPoint mousepnt;
  int i,j,priohead;

	mousepnt=QCursor::pos();
	if (w)
		pTmp=w;
	else
	{
		pTmp=QApplication::widgetAt(mousepnt);
		if (!pTmp)
	  	pTmp=qApp->mainWidget();
	}
	if (pTmp)
	{
		// Switch a window having a priority higher than another one in front of the m_WndTable
		// if the window (w) has the same priority than another window which is first in the  
		// table, the window (w) will be switched in the table to be in front of the screen
		for (i=0;i<m_WndTable.GetSize();i++)
		{
			if ((m_WndTable[i]==pTmp) || (pTmp->parent()==m_WndTable[i]))
			{
				priohead=i;
				for (j=i-1;j>=0;j--)
				{
					if ((m_WndPrioTable[j]!=m_WndPrioTable[i]) || (m_WndPrioTable[j]==m_WndPrioTable[0]))
					{
						priohead=j+1;
						break;
					}
				}
				if ((priohead!=i) && (m_WndPrioTable[priohead]==m_WndPrioTable[i]))
					SwitchOrderWindowTable(i,priohead);
				else if (m_WndPrioTable[priohead]==m_WndPrioTable[0])
					SwitchOrderWindowTable(priohead,0);
			}
		}

#if (defined Q_WS_WIN)
#ifdef ENABLE_QDEBUG
		qDebug("Begin Window Prio");
#endif
		for (i=1;i<m_WndTable.GetSize();i++)
		{
			HWND pTmp=GetNextWindow(m_WndTable[i-1]->winId(),GW_HWNDNEXT);
			if (pTmp!=m_WndTable[i]->winId())
				SetWindowPos(m_WndTable[i]->winId(),m_WndTable[i-1]->winId(),0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSENDCHANGING);
		}
#endif


#if (defined Q_WS_X11)
		Window *pCurTab,root,parent;
		UINT nbr=0;
#ifdef ENABLE_QDEBUG
		qDebug("Wdw Mngr: Query Tree");
#endif
		// display the window according to the priority order
		// and set the first window in the m_WndTable as the root window
		// this window has the highest priority
		XQueryTree(m_WndTable[0]->x11Display(),DefaultRootWindow(m_WndTable[0]->x11Display()),&root,&parent,&pCurTab,&nbr);
#ifdef ENABLE_QDEBUG
		qDebug("Wdw Mngr: End Query Tree %d",nbr);
#endif
		CArray<WId,WId> CurTable;
		CArray<int,int> CurPrio;
		CArray<int,int> CurPrioVal;
		CArray<QString,QString> CurPrioName;
		bool fnd;
		QString str;
		
		for (i=0;i<m_WndTable.GetSize();i++)
		{
			if (m_WndTable[i]->isVisible())
			{
				fnd=false;
				for (j=0;j<(int)nbr;j++)
				{
					// set a table of window priority according to the visibility of the window (CurTable)
			   		if (m_WndTable[i]->winId()==pCurTab[j])
					{
						CurTable.Add(m_WndTable[i]->winId());
						CurPrio.Add(j);
						CurPrioVal.Add(m_WndPrioTable[i]);
						str.sprintf(" Prio=%d,K=%d",m_WndPrioTable[i],j);
#ifdef ENABLE_QDEBUG
						qDebug(str);
#endif
						fnd=true;
						break;
					}
				}
				if (!fnd)
				{
					// if the window is not already displayed, display it on the screen according to the priority
					// and put it inside the table CurTable
					Window *pTmpCurTab,tmproot,tmpparent1,tmpparent2;
					UINT tmpnbr=0;
					XQueryTree(m_WndTable[i]->x11Display(),m_WndTable[i]->winId(),&tmproot,&tmpparent1,&pTmpCurTab,&tmpnbr);
					XFree(pTmpCurTab);
					XQueryTree(m_WndTable[i]->x11Display(),tmpparent1,&tmproot,&tmpparent2,&pTmpCurTab,&tmpnbr);
					XFree(pTmpCurTab);
				for (j=0;j<(int)nbr;j++)
				{
			   		if ((tmpparent1==pCurTab[j]) || (tmpparent2==pCurTab[j]))
					{
						CurTable.Add(pCurTab[j]);
						CurPrio.Add(j);
						CurPrioVal.Add(m_WndPrioTable[i]);
						fnd=true;
						break;
					}
				}
			}
			if (!fnd)
			{
					str.sprintf(" Prio=%d",m_WndPrioTable[i]);
#ifdef ENABLE_QDEBUG
					qDebug(str);
#endif
						
				}
		}
	}
	Window* pTable=NULL;

	{

		{
			pTable=new Window[CurTable.GetSize()];
 			for (int i=0;i<CurTable.GetSize();i++)
				pTable[i]=CurTable[i];
			
			XRestackWindows(m_WndTable[0]->x11Display(),pTable,CurTable.GetSize());
			delete [] pTable;
			QString str1,str2,str;

		}
	}
	
	XFree(pCurTab);
#endif
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the window which has the keyboard focus 
///		above or behind other windows according to the priority order set on 
///		the window table. If this window has the same priority than another window,
///		the window which has the focus is set above
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::OnMouseMove()
{
	QPoint mousepnt;
	int i,j,priohead;
	mousepnt=QCursor::pos();
	QWidget* pTmp=NULL;
	pTmp=QApplication::widgetAt(mousepnt);
	bool priochange = false;

	for (i=0;i<m_WndTable.GetSize();i++)
	{
		if ((m_WndTable[i]==pTmp) || (pTmp->parent()==m_WndTable[i]))
		{
			// check for the priority of the selected window
			priohead=i;
			for (j=i-1;j>=0;j--)
			{
				if (m_WndPrioTable[j]!=m_WndPrioTable[i])
				{
					priohead=j+1;
					break;
				}
			}
			// put the selected window above the windows which have the same priority
			if ((priohead!=i) && (m_WndPrioTable[priohead]==m_WndPrioTable[i]))
			{
				QWidget* pTmp2=m_WndTable[i];
				m_WndTable.RemoveAt(i);
				m_WndTable.InsertAt(priohead,pTmp2);
				priochange=true;
				bool bTmp3=m_baCollapsed[i];
				m_baCollapsed.RemoveAt(i);
				m_baCollapsed.InsertAt(priohead,bTmp3);
				QSize qsTemp4=m_qaSize[i];
				m_qaSize.RemoveAt(i);
				m_qaSize.InsertAt(priohead,qsTemp4);
				QSize qsTemp5=m_qaMinSize[i];
				m_qaMinSize.RemoveAt(i);
				m_qaMinSize.InsertAt(priohead,qsTemp5);
				QWidget* qsTemp6=m_pRecordMove[i];
                m_pRecordMove.RemoveAt(i);
                m_pRecordMove.InsertAt(priohead,qsTemp6);
                QPoint qsTemp7=m_qpMove[i];
                m_qpMove.RemoveAt(i);
                m_qpMove.InsertAt(priohead,qsTemp7);
				long qsTemp8=m_qAttrib[i];
				m_qAttrib.RemoveAt(i);
				m_qAttrib.InsertAt(priohead,qsTemp8);
			}
		}
	}
	if (priochange)
	{
		for (i=1;i<m_WndTable.GetSize();i++)
		{
			m_WndTable[i]->stackUnder(m_WndTable[i-1]);
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the window is collpased or not
/// 
///  RETURNS: 
///		true if the window is collapsed otherwise false
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CWdwMngr::IsWindowCollapsed(QWidget* w)
{
	int iWidgetPos=-1;
	for (int i=0;i<m_WndTable.GetSize();i++)
    {
		if (m_WndTable[i]->winId()==w->winId())
		{
			iWidgetPos=i;
			break;
		}
    }
	if (iWidgetPos==-1)
		return false;
	if (m_baCollapsed[iWidgetPos])
		return true;
	else 
		return false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the size of the window in the collapse size
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::WindowSize(QWidget* pW)
{
#ifdef ENABLE_QDEBUG
	  qDebug("WindowSize: Collapse window");
#endif
	  int iNewWidth=pW->caption().length()*10+30;
	  QSize qs(iNewWidth,0);
	  pW->setMinimumSize(qs);
	  pW->resize(qs);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the window in a collpase form
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWdwMngr::SetCollapse(QWidget* w, bool isCollapsed)
{
	CWin_Hints* pCWinHints=CAsdApp::GetApp()->GetWinHints();	
	int iWidgetPos=-1;

	for (int i=0;i<m_WndTable.GetSize();i++)
    	{
      		if (m_WndTable[i]->winId()==w->winId())
        	{
          		iWidgetPos=i;
         		 break;
        	}
    	}

	if (iWidgetPos==-1)
		return;

	m_baCollapsed[iWidgetPos]=isCollapsed;


	if (isCollapsed)
	{
		m_baCollapsed[iWidgetPos]=true;
		//10 should be the maximum width of a character and
		//30 is the width of the 2 buttons.
		int iNewWidth=w->caption().length()*10+30;
		QSize qs(iNewWidth,0);
		w->setMinimumSize(qs);
		w->resize(qs);
		w->showNormal();
#if (defined Q_WS_X11)
		pCWinHints->CollapseWindow(w,true);
#endif
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the size of the window w
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWdwMngr::SetWindowSize(QWidget* w)
{
	int iWidgetPos=-1;
        
	for (int i=0;i<m_WndTable.GetSize();i++)
        {
                if (m_WndTable[i]==w)
                {
                        iWidgetPos=i;
                         break;
                }
        }

	m_qaSize[iWidgetPos] = w->size();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the size of the decollapsed window w
/// 
///  RETURNS: 
///		the size of the decollapsed window 
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
QSize CWdwMngr::GetDecollapseSize(QWidget* w)
{
	int iWidgetPos=-1;
	QSize qs;
        
	for (int i=0;i<m_WndTable.GetSize();i++)
        {
                if (m_WndTable[i]==w)
                {
                        iWidgetPos=i;
                         break;
                }
        }

        if (iWidgetPos==-1)
                qs =QSize(2,2);
	else
		qs= m_qaSize[iWidgetPos];

	return qs;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the size of the window w to sizeWidth and sizeHeight
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::SetWindowSize(QWidget* w, int sizeWidth, int sizeHeight)
{
	int iWidgetPos=-1;
	QSize size(sizeWidth,sizeHeight);

	for (int i=0;i<m_WndTable.GetSize();i++)
	{
		if (m_WndTable[i]==w)
		{
			iWidgetPos=i;
			break;
		}
	}

	if (iWidgetPos==-1)
		return;
	else
		m_qaSize[iWidgetPos] = size;
                                                                                                                                    
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method gets the ID of the window w
/// 
///  RETURNS: 
///		the ID if the window
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
int CWdwMngr::WindowId(QWidget* w)
{
	int WinId=-1;
	for (int i=0;i<m_WndTable.GetSize();i++)
    	{
      		if (m_WndTable[i]==w)
        	{
			if ("Main Radar"==w->caption())
				WinId=CWFAttrib::MAIN_RDR;	
        		if ("Auxiliary Window"==w->caption())
				WinId=CWFAttrib::AUX_RDR;
        		if ("Height Filter"==w->caption())
				WinId=CWFAttrib::HGHT_FILTERS;
        		if ("System Information"==w->caption())
				WinId=CWFAttrib::SYS_INFO;
        		if ("Brightness Control"==w->caption())
				WinId=CWFAttrib::BRGHT_CTRL;
        		if ("Track Control"==w->caption())
				WinId=CWFAttrib::TRK_CTRL;
        		if ("Change Password"==w->caption())
				WinId=CWFAttrib::CHANGE_PASSWD;
        		if ("Custom Sets"==w->caption())
				WinId=CWFAttrib::CUSTOM_SET;
		        if ("Save As"==w->caption())
				WinId=CWFAttrib::SAVE_AS;	
        		if ("Sign In"==w->caption())
				WinId=CWFAttrib::SIGN_IN;
        		if ("Find Track"==w->caption())
				WinId=CWFAttrib::FIND_TRACK;
        		if ("Full FPL List"==w->caption())
				WinId=CWFAttrib::CSU_LIST;
        		if ("Coupled FPL List"==w->caption())
				WinId=CWFAttrib::CSU_LIST;
        		if ("Mode S Tagged FPL List"==w->caption())
				WinId=CWFAttrib::CSU_LIST;
        		if ("Tagged FPL List"==w->caption())
				WinId=CWFAttrib::CSU_LIST;
        		if ("BIN FPL List"==w->caption())
				WinId=CWFAttrib::CSU_LIST;
		}
	}
	return WinId;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method collapses the window when the user
///		clicks AB on the collapsed button decoration and decollpase the window 
///		when he/she clicks AB on the decollpased button decoration
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWdwMngr::CollapseUncollapseWindow(QObject* pQObj)
{
	CWin_Hints* pCWinHints=CAsdApp::GetApp()->GetWinHints();

	//Find window receiving Collapse Event.
	QWidget* pW=((QWidget*) pQObj);
	int iWidgetPos=-1;
	for (int i=0;i<m_WndTable.GetSize();i++)
	{
		if (m_WndTable[i]==pW)
		{
			iWidgetPos=i;
			break;
		}
	}

	//The minimized window is not managed.
	if (iWidgetPos==-1)
	return;

	if (pW->windowState()==Qt::WindowMinimized)
	{
		// decollapse the window if it was collapsed
		if (m_baCollapsed[iWidgetPos])
		{
#ifdef ENABLE_QDEBUG
			qDebug("decollapse window");
#endif

			m_baCollapsed[iWidgetPos]=false;
			pW->resize(m_qaSize[iWidgetPos]);
			pW->setMinimumSize(m_qaMinSize[iWidgetPos]);
			pW->showNormal(); 
#if (defined Q_WS_X11)
			pCWinHints->CollapseWindow(pW,false);
			if (pW->caption()=="Auxiliary Window")
				CAsdApp::GetApp()->WindowAttributes("Auxiliary Window", CWFAttrib::AUX_RDR, true);
			else if (pW->caption()=="Main Radar")
				CAsdApp::GetApp()->WindowAttributes("Main Radar", CWFAttrib::MAIN_RDR, true);
#endif
		}
		else
		{
			// Collapse the window if it was decollapsed
#ifdef ENABLE_QDEBUG
			qDebug("Collapse window");
#endif
			m_baCollapsed[iWidgetPos]=true;
			m_qaSize[iWidgetPos]=pW->size();
			m_qaMinSize[iWidgetPos]=pW->minimumSize();
			//10 should be the maximum width of a character and
			//30 is the width of the 2 buttons.
			int iNewWidth=pW->caption().length()*10+30;
			QSize qs(iNewWidth,0);
			pW->setMinimumSize(qs);
			pW->resize(qs);
			pW->showNormal();
#if (defined Q_WS_X11)
			pCWinHints->CollapseWindow(pW,true);
#endif
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the order of the window in the window
///		tables that is sort by priority
/// 
///  RETURNS: 
///		the number of the window in the table
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int  CWdwMngr::GetZOrder(QWidget* pWnd)
{
  for (int i=0;i<m_WndTable.GetSize();i++)
    {
      if (m_WndTable[i]==pWnd)
	return i;
    }
  return -1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method resets the border color of the window
///		pWnd if an important track was lying underneath it
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void  CWdwMngr::ResetImportantCode(QWidget* pWnd)
{
	m_pImportantTrackTable.ResetImportantCode(pWnd);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method records the attributes collapse, moveable, 
///		resize, closeable (functions parameter) of the window (Window parameter)
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::SetAttributes(QString Window, long functions)
{
	for (int i=0;i<m_WndTable.GetSize();i++)
	{
		if (m_WndTable[i]->caption()==Window)
			m_qAttrib[i]=functions;
	}	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the collapsability, resizeability, 
///		closeability, moveability have been changed following a change of
///		group, subgroup, location, role
/// 
///  RETURNS: 
///		true if the attributes for the window have changed otherwise false
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
bool CWdwMngr::IsAttributesChanged(QString Window, long functions)
{
	bool Changed=true;

	for (int i=0;i<m_WndTable.GetSize();i++)
        {
                if ((m_WndTable[i]->caption()==Window) && (m_qAttrib[i]==functions))
		{
                        Changed = false;
			break;
		}
        }	
	return Changed;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the window attributes for all ASD windows
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::SetWindowAttributes()
{
	CAsdApp::GetApp()->WindowAttributes("Main Radar", CWFAttrib::MAIN_RDR);
	for (int i=0;i<m_WndTable.GetSize();i++)
        {
                if (m_WndTable[i]->caption()=="Main Radar")
			CAsdApp::GetApp()->SetMinMaxAttributes(m_WndTable[i],CWFAttrib::MAIN_RDR, false);
	}
	CAsdApp::GetApp()->WindowAttributes("Auxiliary Window", CWFAttrib::AUX_RDR);
	CAsdApp::GetApp()->WindowAttributes("Sign In", CWFAttrib::SIGN_IN);
	CAsdApp::GetApp()->WindowAttributes("System Information", CWFAttrib::SYS_INFO);
	CAsdApp::GetApp()->WindowAttributes("Brightness Control", CWFAttrib::BRGHT_CTRL);
	CAsdApp::GetApp()->WindowAttributes("Find Track", CWFAttrib::FIND_TRACK);
	CAsdApp::GetApp()->WindowAttributes("Height Filter", CWFAttrib::HGHT_FILTERS);
	CAsdApp::GetApp()->WindowAttributes("TSSR", CWFAttrib::POPUP);
	CAsdApp::GetApp()->WindowAttributes("CFL", CWFAttrib::POPUP);
	CAsdApp::GetApp()->WindowAttributes("WTC", CWFAttrib::POPUP);
	CAsdApp::GetApp()->WindowAttributes("ATYP", CWFAttrib::POPUP);
	CAsdApp::GetApp()->WindowAttributes("ADES", CWFAttrib::POPUP);
	CAsdApp::GetApp()->WindowAttributes("ASSR", CWFAttrib::POPUP);
	CAsdApp::GetApp()->WindowAttributes("Change Password", CWFAttrib::CHANGE_PASSWD);
	CAsdApp::GetApp()->WindowAttributes("Custom Sets", CWFAttrib::CUSTOM_SET);
	CAsdApp::GetApp()->WindowAttributes("Track Control", CWFAttrib::TRK_CTRL);
	CAsdApp::GetApp()->WindowAttributes("Save As", CWFAttrib::SAVE_AS);
	CAsdApp::GetApp()->WindowAttributes("Full FPL List", CWFAttrib::CSU_LIST);
	CAsdApp::GetApp()->WindowAttributes("Coupled FPL List", CWFAttrib::CSU_LIST);
	CAsdApp::GetApp()->WindowAttributes("Mode S Tagged FPL List", CWFAttrib::CSU_LIST);
	CAsdApp::GetApp()->WindowAttributes("Tagged FPL List", CWFAttrib::CSU_LIST);
	CAsdApp::GetApp()->WindowAttributes("BIN FPL List", CWFAttrib::CSU_LIST);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method makes the window pWnd visible
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::ShowWindow(QWidget* pWnd, int attrib)
{
	pWnd->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
	pWnd->show();
	if (pWnd)
		OrderWindows(pWnd);
	ResetImportantCode(pWnd);
	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method makes the window pWnd visible.
///		These windows are displayed through the custom set
///		and also sets the window in collapse form depending on IsCollapsed
///		parameter
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWdwMngr::ShowCustomSetWindow(QWidget* pWnd, int attrib, bool IsCollapsed)
{
	CWin_Hints* pCWinHints=CAsdApp::GetApp()->GetWinHints();
    pWnd->show();
    pWnd->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
	
	#if (defined Q_WS_X11)
	if (attrib!=-1)
	{
			if (IsCollapsed)
				pCWinHints->CollapseWindow(pWnd,IsCollapsed);
	}
 	#endif
    
	ResetImportantCode(pWnd);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method hides the specified window
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::HideWindow(QWidget* pWnd)
{
	pWnd->hide();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the window has been moved
/// 
///  RETURNS: 
///		true if the window has been moved otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWdwMngr::IsRecordMoved()
{
	bool RecordMove = false;

	for (int i=0; i<m_WndTable.GetSize();i++)
    {
		if (m_pRecordMove[i])
			RecordMove =true;
    }
	return RecordMove;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method records the position of the window that has been moved
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWdwMngr::SetRecordMove(QWidget* w, QPoint p)
{
	if (w->isVisible())
	{
		for (int i=0; i<m_WndTable.GetSize();i++)
		{
			if (w->winId()==m_WndTable[i]->winId())
			{
				m_pRecordMove[i]=w;
				m_qpMove[i]=p;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method cancels the fact that a window has been moved
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CWdwMngr::CancelRecordMove(QWidget* w)
{
	for (int i=0; i<m_WndTable.GetSize();i++)
	{
		if (w->winId()==m_WndTable[i]->winId())
		{
			if (m_pRecordMove[i])
			{			
				m_pRecordMove[i]=NULL;
				m_qpMove[i]=QPoint(-2,-2);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method moves all windows to their recorded positions
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWdwMngr::MoveWindow()
{
	for (int i=0; i<m_WndTable.GetSize();i++)
        {
		if (m_pRecordMove[i])
		{
			m_pRecordMove[i]->move(m_qpMove[i]);
			m_pRecordMove[i]=NULL;
		}
        }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method calls the SetRecordMove method
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWdwMngr::RecordMove(QWidget* w, QPoint p)
{
        SetRecordMove(w,p);
}
           
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method calls the CancelRecordMove method
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------                                                                                                         
void CWdwMngr::CancelMove(QWidget* w)
{
        CancelRecordMove(w);
}

//--------------------------------< THE END >----------------------------------
