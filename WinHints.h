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
//  FILENAME: WinHints.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CWin_Hints which manages the interface with the 
//		window Manager.
//		Allows to display the ASD windows (colors, border presentation, content of
//		the title bar, decoration buttons ...) as required in Design documents.
//		Allows to have the correct window behavior: collapse, move, automatic
//		repositionning...  
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

//-INCLUDE FILES---------------------------------------------------------------

#if !defined(CWIN_HINTS_H)
#define CWIN_HINTS_H

#include <qwidget.h>
#include <qprocess.h>
#include <qtimer.h>
#include <qptrlist.h>

//-GLOBAL DATA-----------------------------------------------------------------



//-CLASSES DEFINITIONS---------------------------------------------------------

class CCmdDelayed
{
public:

	//variables
	int m_iTimer;
	int m_iCmdType;
	int m_iWinId;
	QString m_strCmd;
	QString m_strWName;

	//pointers
	QWidget* m_Widget;

	//methods
	CCmdDelayed(int iTimer, const QString & strCmd, const QString & strWName, int iCmdType, int iWinId, QWidget* m_widget);
	CCmdDelayed(const CCmdDelayed & cmd);
	virtual ~CCmdDelayed();	
};



class CWin_Hints : public QObject
{
public:


	enum m_MwmHints{

		MWM_HINTS_FUNCTIONS=	(1L << 0),
		MWM_HINTS_DECORATIONS=	(1L << 1),
		MWM_HINTS_INPUT_MODE=	(1L << 2),
		MWM_HINTS_STATUS=	        (1L << 3),

		MWM_FUNC_ALL=		(1L << 0),
		MWM_FUNC_RESIZE=		(1L << 1),
		MWM_FUNC_MOVE=		(1L << 2),
		MWM_FUNC_MINIMIZE=	        (1L << 3),
		MWM_FUNC_MAXIMIZE=	        (1L << 4),
		MWM_FUNC_CLOSE=		(1L << 5),
    

		MWM_DECOR_ALL=		(1L << 0),
		MWM_DECOR_BORDER=	        (1L << 1),
		MWM_DECOR_RESIZEH=	        (1L << 2),
		MWM_DECOR_TITLE=		(1L << 3),
		MWM_DECOR_MENU=		(1L << 4),
		MWM_DECOR_MINIMIZE=	        (1L << 5),
		MWM_DECOR_MAXIMIZE=	        (1L << 6),


		MWM_INPUT_MODELESS=			        0,
		MWM_INPUT_PRIMARY_APPLICATION_MODAL=	1,
		MWM_INPUT_SYSTEM_MODAL=			2,
		MWM_INPUT_FULL_APPLICATION_MODAL=	        3,


		MWM_TEAROFF_WINDOW=	(1L << 0),
	};  

	CWin_Hints();
	virtual ~CWin_Hints();

	bool ChangeWindow(QString Window,long functions, long decorations, bool Forced);
	void CollapseWindow(QWidget* w,bool bDisplayCollapse);
	void ChangeBorderColor(QWidget* w, int iCode);
	QRect GetXWindowGeometry(QWidget* w);

private:



	QProcess m_qpProc;
	bool m_bOccupied;
	
	void GetRGBColor(QString OfflineColor, QString& sred, QString& sgreen, QString& sblue);
};

#endif //CWIN_HINTS_H

//--------------------------------< THE END >----------------------------------
