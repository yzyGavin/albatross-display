/****************************************************************************
** Form implementation generated from reading ui file 'FindTrackDlg.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "FindTrackDlg.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "FindTrackDlg.ui.h"

/*
 *  Constructs a CFindTrackDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CFindTrackDlg::CFindTrackDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CFindTrackDlg" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 10, 110, 180, 20 ) );
    textLabel1->setFrameShape( QLabel::LineEditPanel );
    textLabel1->setFrameShadow( QLabel::Sunken );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setGeometry( QRect( 11, 71, 82, 30 ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setGeometry( QRect( 109, 71, 82, 30 ) );
    buttonCancel->setAutoDefault( TRUE );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setGeometry( QRect( 50, 10, 131, 21 ) );

    textEdit1 = new QLineEdit( this, "textEdit1" );
    textEdit1->setGeometry( QRect( 50, 40, 89, 20 ) );
    textEdit1->setFrameShape( QLineEdit::LineEditPanel );
    textEdit1->setFrameShadow( QLineEdit::Sunken );
    textEdit1->setMaxLength( 7 );
    languageChange();
    resize( QSize(200, 134).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( buttonOk_clicked() ) );
    connect( textEdit1, SIGNAL( textChanged(const QString&) ), this, SLOT( searchedCallsignChanged(const QString&) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CFindTrackDlg::~CFindTrackDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CFindTrackDlg::languageChange()
{
    setCaption( tr( "FindTrack" ) );
    textLabel1->setText( QString::null );
    buttonOk->setCaption( QString::null );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    textLabel2->setText( tr( "Callsign/SSR" ) );
}

