/****************************************************************************
** Form implementation generated from reading ui file 'FlSelect.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "FlSelect.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "FlSelect.ui.h"

/*
 *  Constructs a CFlSelectDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CFlSelectDlg::CFlSelectDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CFlSelectDlg" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 20, 20, sizePolicy().hasHeightForWidth() ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pDepDefault = new QPushButton( this, "pDepDefault" );
    pDepDefault->setGeometry( QRect( 6, 28, 86, 25 ) );
    QFont pDepDefault_font(  pDepDefault->font() );
    pDepDefault_font.setFamily( "Liberation Mono Track" );
    pDepDefault_font.setPointSize( 10 );
    pDepDefault->setFont( pDepDefault_font ); 

    pArrDefault = new QPushButton( this, "pArrDefault" );
    pArrDefault->setGeometry( QRect( 6, 234, 86, 25 ) );
    QFont pArrDefault_font(  pArrDefault->font() );
    pArrDefault_font.setFamily( "Liberation Mono Track" );
    pArrDefault_font.setPointSize( 10 );
    pArrDefault->setFont( pArrDefault_font ); 

    pOther = new QPushButton( this, "pOther" );
    pOther->setGeometry( QRect( 6, 264, 86, 25 ) );

    pListLevel = new QListBox( this, "pListLevel" );
    pListLevel->setGeometry( QRect( 6, 58, 86, 171 ) );
    pListLevel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, pListLevel->sizePolicy().hasHeightForWidth() ) );
    QFont pListLevel_font(  pListLevel->font() );
    pListLevel_font.setFamily( "Liberation Mono Track" );
    pListLevel_font.setPointSize( 10 );
    pListLevel->setFont( pListLevel_font ); 
    pListLevel->setDragAutoScroll( FALSE );
    pListLevel->setColumnMode( QListBox::FixedNumber );
    pListLevel->setVariableHeight( FALSE );

    pArcid = new QLabel( this, "pArcid" );
    pArcid->setGeometry( QRect( 6, 5, 86, 18 ) );
    QFont pArcid_font(  pArcid->font() );
    pArcid_font.setFamily( "Liberation Mono Track" );
    pArcid_font.setPointSize( 10 );
    pArcid_font.setBold( TRUE );
    pArcid->setFont( pArcid_font ); 
    pArcid->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    pArcid->setIndent( -1 );
    languageChange();
    resize( QSize(124, 294).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pDepDefault, SIGNAL( clicked() ), this, SLOT( pDepDefaultPressed() ) );
    connect( pArrDefault, SIGNAL( clicked() ), this, SLOT( pArrDefaultPressed() ) );
    connect( pOther, SIGNAL( clicked() ), this, SLOT( pOtherPressed() ) );
    connect( pListLevel, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( pListLevelClicked(QListBoxItem*) ) );
    connect( pListLevel, SIGNAL( returnPressed(QListBoxItem*) ), this, SLOT( pListLevelClicked(QListBoxItem*) ) );

    // tab order
    setTabOrder( pDepDefault, pArrDefault );
    setTabOrder( pArrDefault, pListLevel );
    setTabOrder( pListLevel, pOther );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CFlSelectDlg::~CFlSelectDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CFlSelectDlg::languageChange()
{
    setCaption( tr( "CFL" ) );
    pDepDefault->setText( tr( "350" ) );
    pArrDefault->setText( tr( "190" ) );
    pOther->setText( tr( "Other" ) );
    pArcid->setText( QString::null );
}

