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
//  FILENAME: ImportantTrack.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the management of window framing effect
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

#if !defined(IMPORTANTTRACK_H)
#define IMPORTANTTRACK_H

//-INCLUDE FILES---------------------------------------------------------------

#include <qobject.h>
#include <qwidget.h>
#include "AsdApp.h"


//-CLASS DEFINITION------------------------------------------------------------

class CImportantTrack : public QObject
{

	Q_OBJECT

public:

	// dynamic methods
	CImportantTrack();
	~CImportantTrack();
	void ResetImportantCode(QWidget* w);
	void Init();
	void AddWindow(QString strWindowName,QWidget* w);
	void RemoveWindow(QWidget* w);
	void timerEvent(QTimerEvent* e );

private:

	//variables
	int m_iImportantTrackTimer;
	int m_iNbWindowName;
	int m_iNbWindowManaged;
	bool m_bInit;
	CArray<QString,QString> m_strWindowNames;
	CArray<bool,bool> m_bWindowEligibilities;
	CArray<QWidget*,QWidget*> m_qwWindows;
	CArray<int,int> m_iWindowCode;
	CArray<QPoint,QPoint> m_OldWindowPos;

};

#endif  // end of file

//--------------------------------< THE END >----------------------------------
