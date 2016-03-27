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
//  FILENAME: SKMessageBox.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CSKMessageBox which contains the definition of the
//		message windows (information, warning and error) used by the ASD. 
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
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include <qmessagebox.h>
#include "SKMessageBox.h"
#include "WinHints.h"
#include "AsdApp.h"

#if (defined Q_WS_X11)
#include <X11/X.h>
#include <X11/Xlib.h>
#endif


//-GLOBAL DATA-----------------------------------------------------------------

int CSKMessageBox::m_iCount[13];
CSKMessageBox* CSKMessageBox::m_Box =NULL;


//-FUNCTION PROTOTYPES---------------------------------------------------------


//-----------------------------------------------------------------------------
//
/// \class CSKMessageBox
///		This class inherits from the QMessageBox. 
///		Each ASD message box is positioned with an offset according to the other
///		message boxes.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes a counter table. It is used to save the number
///		of message boxes which have the same position.
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CSKMessageBox::Init()
{
	for(int i=0;i<13;i++)
		m_iCount[i]=0;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method constructs a message box with a caption, a text, 
///		an icon, and up to three buttons
///		The icon must be one of the following:
///			NoIcon 
///			Question 
///			Information 
///			Warning 
///			Critical 
///		Each button, button0, button1 and button2, can have one of the following values: 
///			CSKMessageBox::NoButton 
///			CSKMessageBox::Ok 
///			CSKMessageBox::Cancel 
///			CSKMessageBox::Yes 
///			CSKMessageBox::No 
///			CSKMessageBox::Abort 
///			CSKMessageBox::Retry 
///			CSKMessageBox::Ignore 
///			CSKMessageBox::YesAll 
///			CSKMessageBox::NoAll 
///		Use CSKMessageBox::NoButton for the later parameters to have fewer than 
///		three buttons in your message box. If you don't specify any buttons at all, 
///		CSKMessageBox will provide an Ok button. 
///		If parent is 0, the message box becomes an application-global modal dialog box. 
///		If parent is a widget, the message box becomes modal relative to parent.
///		If modal is TRUE the message box is modal; otherwise it is modeless
///
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CSKMessageBox::CSKMessageBox( const QString& caption,
			      const QString &text, Icon icon,
			      int button0, int button1, int button2,
			      QWidget *parent, const char *name,
			      bool modal, WFlags f )
  :QMessageBox(caption,text,icon,button0,button1,button2,parent,name,modal,f)
{
  CWdwMngr::AddWindow(this,"MESSAGE_BOX",CWFAttrib::MESSAGE_BOX, false);
  m_bModal=FALSE;
  m_iIndex=-1;

  if(button0==CSKMessageBox::Cancel || button1==CSKMessageBox::Cancel || button2==CSKMessageBox::Cancel)
	  m_bEscapeEnable=true;
  else
	  m_bEscapeEnable=false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the copy constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CSKMessageBox::CSKMessageBox(CSKMessageBox &skMessageBox)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CSKMessageBox::~CSKMessageBox()
{
  if(m_iIndex>=0&&m_iIndex<13)
	  m_iCount[m_iIndex]--;

  m_Box=NULL;
  CWdwMngr::RemoveWindow(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method allows to show the message box
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CSKMessageBox::show()
{
	this->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
	QDialog::show();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method allows to hide the message box
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CSKMessageBox::hide()
{
	QDialog::hide();

#if (defined Q_WS_X11)
	CWin_Hints* pCWinHints=CAsdApp::GetApp()->GetWinHints();

#endif
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates and shows an information message box with
///		a parent, a caption, a text, and up to three buttons.
/// 
///  RETURNS: 
///		a pointer to the CSKMessageBox
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
CSKMessageBox* CSKMessageBox::ShowInformation( QWidget *parent, const QString& caption, 
				const QString& text,
				int button0, int button1, int button2 )
{
  CSKMessageBox *mb = new CSKMessageBox( caption, text, Information,
					 button0, button1, button2,
					 parent, "qt_msgbox_information", false,
					 WDestructiveClose);
  Q_CHECK_PTR( mb );

  mb->SetPos();
  mb->show();

  return mb;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates and shows a question message box with
///		a parent, a caption, a text, a pnt, and up to three buttons.
///		This message box is modal.
/// 
///  RETURNS: 
///		returns the identity (QMessageBox::Yes, or QMessageBox::No, etc.) 
///		of the button that was clicked.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CSKMessageBox::question( QWidget *parent,
			     const QString& caption, const QString& text, QPoint pnt,
			     int button0, int button1, int button2)
{
  CSKMessageBox *mb = new CSKMessageBox( caption, text, Question,
					 button0, button1, button2,
					 parent, "qt_msgbox_information", TRUE,
					 WDestructiveClose);

  if ((pnt.x() !=0) && (pnt.y()!=0))
	  mb->move(pnt.x(), pnt.y()+20);

  Q_CHECK_PTR( mb );

  mb->m_bModal=TRUE;

  return mb->exec();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates and shows a warning message box with
///		a parent, a caption, a text, and up to three buttons.
///		This message box is modal.
/// 
///  RETURNS: 
///		returns the identity (QMessageBox::Yes, or QMessageBox::No, etc.) 
///		of the button that was clicked.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CSKMessageBox::warning( QWidget *parent,
                          const QString& caption, const QString& text,
                          int button0, int button1, int button2 )
{
    CSKMessageBox *mb = new CSKMessageBox( caption, text, Warning,
                                       button0, button1, button2,
                                       parent, "qt_msgbox_warning", false,
				       WDestructiveClose);
    Q_CHECK_PTR( mb );
	mb->m_bModal=TRUE;

	return mb->exec();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates and shows a warning message box with
///		a parent, a caption, a text, and up to three buttons.
/// 
///  RETURNS: 
///		a pointer to the CSKMessageBox
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CSKMessageBox::ShowWarning( QWidget *parent,
                          const QString& caption, const QString& text,
                          int button0, int button1, int button2 )
{
    CSKMessageBox *mb = new CSKMessageBox( caption, text, Warning,
                                       button0, button1, button2,
                                       parent, "qt_msgbox_warning", false,
				       WDestructiveClose);
    Q_CHECK_PTR( mb );

    mb->SetPos();
    mb->show();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates and shows a critical message box with
///		a parent, a caption, a text, and up to three buttons.
/// 
///  RETURNS: 
///		a pointer to the CSKMessageBox
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CSKMessageBox::ShowCritical( QWidget *parent,
			     const QString& caption, const QString& text,
			     int button0, int button1, int button2 )
{
  CSKMessageBox *mb = new CSKMessageBox( caption, text, Critical,
					 button0, button1, button2,
					 parent, "qt_msgbox_critical", false,
					 WDestructiveClose);
    Q_CHECK_PTR( mb );

    mb->m_bModal=false;
	mb->SetPos();
    mb->show();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates and shows a critical message box with
///		a parent, an icon, a caption, a text, three buttons, 
///		defaultButtonNumber, escapeButtonNumber.
///		This message box is modal
/// 
///  RETURNS: 
///		returns the identity (QMessageBox::Yes, or QMessageBox::No, etc.) minus 1 
///		of the button that was clicked.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CSKMessageBox::textBox( QWidget *parent, QMessageBox::Icon severity,
                    const QString& caption, const QString& text,
                    const QString& button0Text,
                    const QString& button1Text,
                    const QString& button2Text,
                    int defaultButtonNumber,
                    int escapeButtonNumber )
{
  int b[3];
  b[0] = 1;
  b[1] = button1Text.isEmpty() ? 0 : 2;
  b[2] = button2Text.isEmpty() ? 0 : 3;
  
  int i;
  for( i=0; i<3; i++ ) {
    if ( b[i] && defaultButtonNumber == i )
      b[i] += QMessageBox::Default;
    if ( b[i] && escapeButtonNumber == i )
      b[i] += QMessageBox::Escape;
  }
  
  CSKMessageBox *mb = new CSKMessageBox( caption, text, severity,
					 b[0], b[1], b[2],
					 parent, "qt_msgbox_information", TRUE,
					 Qt::WDestructiveClose);
  Q_CHECK_PTR( mb );
  mb->m_bModal=TRUE;

  if ( button0Text.isEmpty() )
    mb->setButtonText( 1, "Ok");
  else
    mb->setButtonText( 1, button0Text );
  if ( b[1] )
    mb->setButtonText( 2, button1Text );
  if ( b[2] )
    mb->setButtonText( 3, button2Text );
  
#ifndef QT_NO_CURSOR
  mb->setCursor( Qt::arrowCursor );
#endif
 
  return mb->exec() - 1;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates and shows a critical message box with
///		a parent, a caption, a text, three buttons, 
///		defaultButtonNumber, escapeButtonNumber.
///		This message box is modal
/// 
///  RETURNS: 
///		returns the identity (QMessageBox::Yes, or QMessageBox::No, etc.) minus 1 
///		of the button that was clicked.
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
int CSKMessageBox::critical( QWidget *parent, const QString &caption,
			     const QString& text,
			     const QString& button0Text,
			     const QString& button1Text,
			     const QString& button2Text,
			     int defaultButtonNumber,
			     int escapeButtonNumber )
{
  return textBox( parent, Critical, caption, text,
		  button0Text, button1Text, button2Text,
		  defaultButtonNumber, escapeButtonNumber );
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the position of the message box.
///		It is called by the ShowInformation, ShowWarning and ShowCritical methods.
///		The position is set to the center of the primary screen plus an offset 
///		according to the previous message box.
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CSKMessageBox::SetPos()
{
    QDesktopWidget* pDesktop=QApplication::desktop();
	QRect rectMonitor=pDesktop->screenGeometry(pDesktop->primaryScreen());

	// Find a free position
	m_iIndex=0;
	for(int i=1;i<13;i++)
	{
		if(m_iCount[i]<m_iCount[i-1])
		{
			m_iIndex=i;
			//break;
		}
	}

	m_iCount[m_iIndex]++;

	move((rectMonitor.topLeft().x()+rectMonitor.width()/2 + 50*m_iIndex), 
		 (rectMonitor.topLeft().y()+rectMonitor.height()/2 + 30*m_iIndex));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method allows to close the non modal message box
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CSKMessageBox::done ( int r ) 
{
	QMessageBox::done(r);
	if(!m_bModal)
		QMessageBox::close();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method allows to delete the non modal message box
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CSKMessageBox::closeEvent(QCloseEvent* )
{
	if(!m_bModal)
		delete this;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method allows to close the message box with the escape key if 
///		the windows has a Cancel button. Otherwise the escape key has no effect.
///
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
void CSKMessageBox::keyPressEvent ( QKeyEvent * e )
{
	switch (e->key())
	{
		case Key_Escape:
			if(!m_bEscapeEnable)
				return;
			break;
		default:
			break;
	}

	QMessageBox::keyPressEvent ( e );
}



//--------------------------------< THE END >----------------------------------
