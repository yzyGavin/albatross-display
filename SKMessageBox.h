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
//  FILENAME: SKMessageBox.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CSKMessageBox which contains the definition of the
//		message windows (information, warning and error) used by the ASD. 
//				
// 
//  COMMENTS
//		If any other function is needed, the function must be redefined
//		within this file to add the message box to the list of window.
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
#if !defined(SKMESSAGEBOX_H)
#define SKMESSAGEBOX_H

//-INCLUDE FILES---------------------------------------------------------------

#include <qmessagebox.h>
#include <qstring.h>
#include <qwidget.h>
#include "WdwMngr.h"

//-GLOBAL DATA-----------------------------------------------------------------

//-CLASSES DEFINITIONS---------------------------------------------------------

class CSKMessageBox : public QMessageBox
{

public:
	//variables
	static CSKMessageBox* m_Box;

	//methods
	~CSKMessageBox();
	static void Init();
  
	static CSKMessageBox* ShowInformation( QWidget *parent, const QString &caption,
		  const QString& text,
		  int button0=CSKMessageBox::Ok, int button1=0, int button2=0 );

	static int question( QWidget *parent, const QString &caption,
				   const QString& text, QPoint pnt,
				   int button0, int button1=0, int button2=0);

	static int warning( QWidget *parent, const QString &caption,
				  const QString& text,
				  int button0=CSKMessageBox::Ok, int button1=0, int button2=0 );

	static void ShowWarning( QWidget *parent, const QString &caption,
				  const QString& text,
				  int button0=CSKMessageBox::Ok, int button1=0, int button2=0 );
  
	static int critical( QWidget *parent, const QString &caption,
				   const QString& text,
				   const QString& button0Text = QString::null,
				   const QString& button1Text = QString::null,
				   const QString& button2Text = QString::null,
				   int defaultButtonNumber = 0,
				   int escapeButtonNumber = -1 );
	static void ShowCritical( QWidget *parent, const QString &caption,
				   const QString& text,
				   int button0=CSKMessageBox::Ok, int button1=0, int button2=0 );

private:
	//variables
	static int m_iCount[13];
	int m_iIndex;
	bool m_bModal;
	bool m_bEscapeEnable;

	//methods
	CSKMessageBox( const QString& caption, const QString &text, Icon icon,
			 int button0, int button1, int button2,
			 QWidget* parent=0, const char* name=0, bool modal=TRUE,
			 WFlags f=WStyle_DialogBorder  );
	CSKMessageBox(CSKMessageBox &skMessageBox);

	void show();
	void hide();

	static int textBox( QWidget *parent, QMessageBox::Icon severity,
						const QString& caption, const QString& text,
						const QString& button0Text,
						const QString& button1Text,
						const QString& button2Text,
						int defaultButtonNumber,
						int escapeButtonNumber );

	void done ( int r );
	void closeEvent (QCloseEvent* e);
	void keyPressEvent ( QKeyEvent * e );
	void SetPos();
	
};

#endif

//--------------------------------< THE END >----------------------------------
