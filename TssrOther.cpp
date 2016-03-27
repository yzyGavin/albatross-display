/****************************************************************************
** Form implementation generated from reading ui file 'TssrOther.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "TssrOther.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "TssrOther.ui.h"

/*
 *  Constructs a CTssrOtherDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CTssrOtherDlg::CTssrOtherDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CTssrOtherDlg" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pTssr = new QLabel( this, "pTssr" );
    pTssr->setGeometry( QRect( 6, 5, 86, 18 ) );
    QFont pTssr_font(  pTssr->font() );
    pTssr_font.setFamily( "Liberation Mono Track" );
    pTssr_font.setPointSize( 10 );
    pTssr_font.setBold( TRUE );
    pTssr->setFont( pTssr_font ); 
    pTssr->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    pUntag = new QPushButton( this, "pUntag" );
    pUntag->setGeometry( QRect( 6, 55, 86, 25 ) );
    pUntag->setAutoDefault( TRUE );
    pUntag->setDefault( FALSE );

    pOK = new QPushButton( this, "pOK" );
    pOK->setGeometry( QRect( 6, 86, 86, 25 ) );
    pOK->setAutoDefault( FALSE );
    pOK->setDefault( TRUE );

    pArcid = new QLineEdit( this, "pArcid" );
    pArcid->setGeometry( QRect( 6, 28, 86, 22 ) );
    QFont pArcid_font(  pArcid->font() );
    pArcid_font.setFamily( "Liberation Mono Track" );
    pArcid_font.setPointSize( 10 );
    pArcid->setFont( pArcid_font ); 
    pArcid->setMouseTracking( FALSE );
    pArcid->setFrameShape( QLineEdit::LineEditPanel );
    pArcid->setFrameShadow( QLineEdit::Sunken );
    pArcid->setMaxLength( 7 );
    languageChange();
    resize( QSize(120, 117).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pOK, SIGNAL( clicked() ), this, SLOT( pOKPressed() ) );
    connect( pUntag, SIGNAL( clicked() ), this, SLOT( pUntagPressed() ) );
    connect( pArcid, SIGNAL( textChanged(const QString&) ), this, SLOT( ArcIdChanged(const QString&) ) );

    // tab order
    setTabOrder( pArcid, pUntag );
    setTabOrder( pUntag, pOK );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CTssrOtherDlg::~CTssrOtherDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CTssrOtherDlg::languageChange()
{
    setCaption( tr( "TSSR" ) );
    pTssr->setText( QString::null );
    pUntag->setText( tr( "Untag" ) );
    pOK->setText( tr( "Ok" ) );
    pArcid->setText( QString::null );
    pArcid->setInputMask( QString::null );
}

