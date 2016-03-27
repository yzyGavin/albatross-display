/****************************************************************************
** Form implementation generated from reading ui file 'HeightFilterDlg.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "HeightFilterDlg.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "HeightFilterDlg.ui.h"

/*
 *  Constructs a CHeightFilterDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CHeightFilterDlg::CHeightFilterDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CHeightFilterDlg" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 

    DepartureGroup = new QGroupBox( this, "DepartureGroup" );
    DepartureGroup->setEnabled( TRUE );
    DepartureGroup->setGeometry( QRect( 6, 5, 52, 47 ) );
    DepartureGroup->setAlignment( int( QGroupBox::AlignHCenter ) );

    pDepartureEdit = new QLineEdit( DepartureGroup, "pDepartureEdit" );
    pDepartureEdit->setGeometry( QRect( 6, 18, 40, 22 ) );
    QFont pDepartureEdit_font(  pDepartureEdit->font() );
    pDepartureEdit_font.setFamily( "Liberation Mono Track" );
    pDepartureEdit_font.setPointSize( 10 );
    pDepartureEdit->setFont( pDepartureEdit_font ); 
    pDepartureEdit->setMouseTracking( FALSE );
    pDepartureEdit->setFrameShape( QLineEdit::LineEditPanel );
    pDepartureEdit->setFrameShadow( QLineEdit::Sunken );
    pDepartureEdit->setMaxLength( 3 );

    LowerGroup = new QGroupBox( this, "LowerGroup" );
    LowerGroup->setEnabled( TRUE );
    LowerGroup->setGeometry( QRect( 56, 5, 52, 47 ) );
    LowerGroup->setAlignment( int( QGroupBox::AlignHCenter ) );

    pLowerEdit = new QLineEdit( LowerGroup, "pLowerEdit" );
    pLowerEdit->setGeometry( QRect( 6, 18, 40, 22 ) );
    QFont pLowerEdit_font(  pLowerEdit->font() );
    pLowerEdit_font.setFamily( "Liberation Mono Track" );
    pLowerEdit_font.setPointSize( 10 );
    pLowerEdit->setFont( pLowerEdit_font ); 
    pLowerEdit->setMouseTracking( FALSE );
    pLowerEdit->setFrameShape( QLineEdit::LineEditPanel );
    pLowerEdit->setFrameShadow( QLineEdit::Sunken );
    pLowerEdit->setMaxLength( 3 );

    OkButton = new QPushButton( this, "OkButton" );
    OkButton->setGeometry( QRect( 6, 57, 73, 25 ) );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setGeometry( QRect( 85, 57, 73, 25 ) );
    buttonCancel->setFocusPolicy( QPushButton::TabFocus );
    buttonCancel->setAutoDefault( TRUE );

    UpperGroup = new QGroupBox( this, "UpperGroup" );
    UpperGroup->setEnabled( TRUE );
    UpperGroup->setGeometry( QRect( 106, 5, 52, 47 ) );
    UpperGroup->setAlignment( int( QGroupBox::AlignHCenter ) );

    pUpperEdit = new QLineEdit( UpperGroup, "pUpperEdit" );
    pUpperEdit->setGeometry( QRect( 6, 18, 40, 22 ) );
    QFont pUpperEdit_font(  pUpperEdit->font() );
    pUpperEdit_font.setFamily( "Liberation Mono Track" );
    pUpperEdit_font.setPointSize( 10 );
    pUpperEdit->setFont( pUpperEdit_font ); 
    pUpperEdit->setMouseTracking( FALSE );
    pUpperEdit->setFrameShape( QLineEdit::LineEditPanel );
    pUpperEdit->setFrameShadow( QLineEdit::Sunken );
    pUpperEdit->setMaxLength( 3 );

    Error = new QLabel( this, "Error" );
    Error->setGeometry( QRect( 0, 87, 164, 22 ) );
    Error->setFrameShape( QLabel::LineEditPanel );
    Error->setFrameShadow( QLabel::Sunken );
    Error->setAlignment( int( QLabel::AlignVCenter ) );
    languageChange();
    resize( QSize(164, 109).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pLowerEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( pLowerEditTextChanged(const QString&) ) );
    connect( pUpperEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( pUpperEditTextChanged(const QString&) ) );
    connect( pDepartureEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( pDepartureEditTextChanged(const QString&) ) );
    connect( OkButton, SIGNAL( clicked() ), this, SLOT( OkClicked() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( CancelClicked() ) );

    // tab order
    setTabOrder( pDepartureEdit, pLowerEdit );
    setTabOrder( pLowerEdit, pUpperEdit );
    setTabOrder( pUpperEdit, OkButton );
    setTabOrder( OkButton, buttonCancel );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CHeightFilterDlg::~CHeightFilterDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CHeightFilterDlg::languageChange()
{
    setCaption( tr( "Height Filter" ) );
    DepartureGroup->setTitle( tr( "Dep" ) );
    pDepartureEdit->setText( QString::null );
    LowerGroup->setTitle( tr( "Low" ) );
    pLowerEdit->setText( QString::null );
    OkButton->setText( tr( "Ok" ) );
    OkButton->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    UpperGroup->setTitle( tr( "High" ) );
    pUpperEdit->setText( QString::null );
    Error->setText( QString::null );
}

