/****************************************************************************
** Form implementation generated from reading ui file 'FlSelectOther.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "FlSelectOther.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "FlSelectOther.ui.h"

/*
 *  Constructs a CFlSelectOther as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CFlSelectOther::CFlSelectOther( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CFlSelectOther" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pushButton7 = new QPushButton( this, "pushButton7" );
    pushButton7->setGeometry( QRect( 1, 55, 38, 30 ) );

    pushButton4 = new QPushButton( this, "pushButton4" );
    pushButton4->setGeometry( QRect( 1, 85, 38, 30 ) );

    pushButton1 = new QPushButton( this, "pushButton1" );
    pushButton1->setGeometry( QRect( 1, 115, 38, 30 ) );

    pushButton5 = new QPushButton( this, "pushButton5" );
    pushButton5->setGeometry( QRect( 39, 85, 38, 30 ) );

    pushButton9 = new QPushButton( this, "pushButton9" );
    pushButton9->setGeometry( QRect( 77, 55, 38, 30 ) );

    pushButton6 = new QPushButton( this, "pushButton6" );
    pushButton6->setGeometry( QRect( 77, 85, 38, 30 ) );

    pushButton2 = new QPushButton( this, "pushButton2" );
    pushButton2->setGeometry( QRect( 39, 115, 38, 30 ) );

    pushButton3 = new QPushButton( this, "pushButton3" );
    pushButton3->setGeometry( QRect( 77, 115, 38, 30 ) );

    pushButton0 = new QPushButton( this, "pushButton0" );
    pushButton0->setGeometry( QRect( 39, 145, 38, 30 ) );

    pushButton8 = new QPushButton( this, "pushButton8" );
    pushButton8->setGeometry( QRect( 39, 55, 38, 30 ) );

    Cls = new QLabel( this, "Cls" );
    Cls->setGeometry( QRect( 7, 5, 102, 18 ) );
    QFont Cls_font(  Cls->font() );
    Cls_font.setFamily( "Liberation Mono Track" );
    Cls_font.setPointSize( 10 );
    Cls_font.setBold( TRUE );
    Cls->setFont( Cls_font ); 
    Cls->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    Level = new QLineEdit( this, "Level" );
    Level->setGeometry( QRect( 33, 28, 50, 22 ) );
    QFont Level_font(  Level->font() );
    Level_font.setFamily( "Liberation Mono Track" );
    Level_font.setPointSize( 10 );
    Level->setFont( Level_font ); 
    Level->setMouseTracking( FALSE );
    Level->setFrameShape( QLineEdit::LineEditPanel );
    Level->setFrameShadow( QLineEdit::Sunken );
    Level->setMaxLength( 3 );
    Level->setAlignment( int( QLineEdit::AlignRight ) );
    languageChange();
    resize( QSize(120, 176).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButton0, SIGNAL( clicked() ), this, SLOT( PushButton0Pressed() ) );
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( PushButton1Pressed() ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( PushButton2Pressed() ) );
    connect( pushButton3, SIGNAL( clicked() ), this, SLOT( PushButton3Pressed() ) );
    connect( pushButton5, SIGNAL( clicked() ), this, SLOT( PushButton5Pressed() ) );
    connect( pushButton6, SIGNAL( clicked() ), this, SLOT( PushButton6Pressed() ) );
    connect( pushButton7, SIGNAL( clicked() ), this, SLOT( PushButton7Pressed() ) );
    connect( pushButton8, SIGNAL( clicked() ), this, SLOT( PushButton8Pressed() ) );
    connect( pushButton9, SIGNAL( clicked() ), this, SLOT( PushButton9Pressed() ) );
    connect( pushButton4, SIGNAL( clicked() ), this, SLOT( PushButton4Pressed() ) );
    connect( Level, SIGNAL( textChanged(const QString&) ), this, SLOT( LevelTextChanged(const QString&) ) );
    connect( Level, SIGNAL( returnPressed() ), this, SLOT( LevelReturnPressed() ) );

    // tab order
    setTabOrder( Level, pushButton0 );
    setTabOrder( pushButton0, pushButton1 );
    setTabOrder( pushButton1, pushButton2 );
    setTabOrder( pushButton2, pushButton3 );
    setTabOrder( pushButton3, pushButton4 );
    setTabOrder( pushButton4, pushButton5 );
    setTabOrder( pushButton5, pushButton6 );
    setTabOrder( pushButton6, pushButton7 );
    setTabOrder( pushButton7, pushButton8 );
    setTabOrder( pushButton8, pushButton9 );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CFlSelectOther::~CFlSelectOther()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CFlSelectOther::languageChange()
{
    setCaption( tr( "FlOther" ) );
    pushButton7->setText( tr( "7" ) );
    pushButton4->setText( tr( "4" ) );
    pushButton1->setText( tr( "1" ) );
    pushButton5->setText( tr( "5" ) );
    pushButton9->setText( tr( "9" ) );
    pushButton6->setText( tr( "6" ) );
    pushButton2->setText( tr( "2" ) );
    pushButton3->setText( tr( "3" ) );
    pushButton0->setText( tr( "0" ) );
    pushButton8->setText( tr( "8" ) );
    Cls->setText( QString::null );
    Level->setText( QString::null );
}

