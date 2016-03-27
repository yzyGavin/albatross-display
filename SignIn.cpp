/****************************************************************************
** Form implementation generated from reading ui file 'SignIn.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "SignIn.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "SignIn.ui.h"

/*
 *  Constructs a CSignIn as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CSignIn::CSignIn( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CSignIn" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setGeometry( QRect( 6, 32, 68, 22 ) );
    textLabel2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    Error = new QLabel( this, "Error" );
    Error->setGeometry( QRect( 0, 90, 219, 22 ) );
    Error->setFrameShape( QLabel::LineEditPanel );
    Error->setFrameShadow( QLabel::Sunken );
    Error->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 6, 5, 72, 22 ) );
    textLabel1->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setGeometry( QRect( 6, 60, 100, 25 ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( FALSE );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setGeometry( QRect( 113, 60, 100, 25 ) );
    buttonCancel->setAutoDefault( TRUE );

    leUserName = new QLineEdit( this, "leUserName" );
    leUserName->setGeometry( QRect( 84, 5, 89, 22 ) );
    QFont leUserName_font(  leUserName->font() );
    leUserName_font.setFamily( "Liberation Mono Track" );
    leUserName_font.setPointSize( 10 );
    leUserName->setFont( leUserName_font ); 
    leUserName->setMouseTracking( FALSE );
    leUserName->setFrameShape( QLineEdit::LineEditPanel );
    leUserName->setFrameShadow( QLineEdit::Sunken );
    leUserName->setMaxLength( 8 );

    lePassword = new QLineEdit( this, "lePassword" );
    lePassword->setGeometry( QRect( 84, 32, 129, 22 ) );
    QFont lePassword_font(  lePassword->font() );
    lePassword_font.setFamily( "Liberation Mono Track" );
    lePassword_font.setPointSize( 10 );
    lePassword->setFont( lePassword_font ); 
    lePassword->setMouseTracking( FALSE );
    lePassword->setFrameShape( QLineEdit::LineEditPanel );
    lePassword->setFrameShadow( QLineEdit::Sunken );
    lePassword->setMaxLength( 12 );
    lePassword->setEchoMode( QLineEdit::Password );
    languageChange();
    resize( QSize(219, 112).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( OkClicked() ) );
    connect( leUserName, SIGNAL( textChanged(const QString&) ), this, SLOT( UserNameChanged(const QString&) ) );
    connect( lePassword, SIGNAL( textChanged(const QString&) ), this, SLOT( PasswordChanged(const QString&) ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( CancelClicked() ) );

    // tab order
    setTabOrder( leUserName, lePassword );
    setTabOrder( lePassword, buttonOk );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CSignIn::~CSignIn()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CSignIn::languageChange()
{
    setCaption( tr( "Sign In" ) );
    textLabel2->setText( tr( "Password" ) );
    Error->setText( QString::null );
    textLabel1->setText( tr( "Username" ) );
    buttonOk->setCaption( QString::null );
    buttonOk->setText( tr( "Ok" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    leUserName->setText( QString::null );
    lePassword->setText( QString::null );
}

