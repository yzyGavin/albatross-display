/****************************************************************************
** Form implementation generated from reading ui file 'ChangePassword.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "ChangePassword.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "ChangePassword.ui.h"

/*
 *  Constructs a CChangePasswd as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CChangePasswd::CChangePasswd( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CChangePasswd" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    Error = new QLabel( this, "Error" );
    Error->setGeometry( QRect( 0, 116, 303, 22 ) );
    Error->setFrameShape( QLabel::LineEditPanel );
    Error->setFrameShadow( QLabel::Sunken );

    textLabel5 = new QLabel( this, "textLabel5" );
    textLabel5->setGeometry( QRect( 6, 5, 130, 22 ) );
    textLabel5->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    textLabel6 = new QLabel( this, "textLabel6" );
    textLabel6->setGeometry( QRect( 6, 32, 110, 22 ) );
    textLabel6->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    textLabel7 = new QLabel( this, "textLabel7" );
    textLabel7->setGeometry( QRect( 5, 59, 157, 18 ) );
    textLabel7->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );
    textLabel7->setIndent( -1 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setGeometry( QRect( 34, 86, 100, 25 ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( FALSE );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setGeometry( QRect( 169, 87, 100, 25 ) );
    buttonCancel->setFocusPolicy( QPushButton::TabFocus );
    buttonCancel->setAutoDefault( TRUE );

    CurrentPassword = new QLineEdit( this, "CurrentPassword" );
    CurrentPassword->setGeometry( QRect( 168, 5, 129, 22 ) );
    QFont CurrentPassword_font(  CurrentPassword->font() );
    CurrentPassword_font.setFamily( "Liberation Mono Track" );
    CurrentPassword_font.setPointSize( 10 );
    CurrentPassword->setFont( CurrentPassword_font ); 
    CurrentPassword->setMouseTracking( FALSE );
    CurrentPassword->setMaxLength( 12 );
    CurrentPassword->setEchoMode( QLineEdit::Password );

    NewPassword = new QLineEdit( this, "NewPassword" );
    NewPassword->setGeometry( QRect( 168, 32, 129, 22 ) );
    QFont NewPassword_font(  NewPassword->font() );
    NewPassword_font.setFamily( "Liberation Mono Track" );
    NewPassword_font.setPointSize( 10 );
    NewPassword->setFont( NewPassword_font ); 
    NewPassword->setMouseTracking( FALSE );
    NewPassword->setFrameShape( QLineEdit::LineEditPanel );
    NewPassword->setFrameShadow( QLineEdit::Sunken );
    NewPassword->setMaxLength( 12 );
    NewPassword->setEchoMode( QLineEdit::Password );

    ConfirmPassword = new QLineEdit( this, "ConfirmPassword" );
    ConfirmPassword->setGeometry( QRect( 168, 59, 129, 22 ) );
    QFont ConfirmPassword_font(  ConfirmPassword->font() );
    ConfirmPassword_font.setFamily( "Liberation Mono Track" );
    ConfirmPassword_font.setPointSize( 10 );
    ConfirmPassword->setFont( ConfirmPassword_font ); 
    ConfirmPassword->setMouseTracking( FALSE );
    ConfirmPassword->setFrameShape( QLineEdit::LineEditPanel );
    ConfirmPassword->setFrameShadow( QLineEdit::Sunken );
    ConfirmPassword->setMaxLength( 12 );
    ConfirmPassword->setEchoMode( QLineEdit::Password );
    ConfirmPassword->setAlignment( int( QLineEdit::AlignAuto ) );
    languageChange();
    resize( QSize(303, 138).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( OkClicked() ) );
    connect( CurrentPassword, SIGNAL( textChanged(const QString&) ), this, SLOT( PasswordChanged(const QString&) ) );
    connect( ConfirmPassword, SIGNAL( textChanged(const QString&) ), this, SLOT( ConfirmPasswordChanged(const QString&) ) );
    connect( NewPassword, SIGNAL( textChanged(const QString&) ), this, SLOT( NewPasswordChanged(const QString&) ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( CancelClicked() ) );

    // tab order
    setTabOrder( CurrentPassword, NewPassword );
    setTabOrder( NewPassword, ConfirmPassword );
    setTabOrder( ConfirmPassword, buttonOk );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CChangePasswd::~CChangePasswd()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CChangePasswd::languageChange()
{
    setCaption( tr( "Change Password" ) );
    Error->setText( QString::null );
    textLabel5->setText( tr( "Current Password" ) );
    textLabel6->setText( tr( "New Password" ) );
    textLabel7->setText( tr( "Confirm New Password" ) );
    buttonOk->setCaption( QString::null );
    buttonOk->setText( tr( "Ok" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    CurrentPassword->setText( QString::null );
}

