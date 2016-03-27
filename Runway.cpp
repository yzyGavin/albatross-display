/****************************************************************************
** Form implementation generated from reading ui file 'Runway.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Runway.h"

#include <qvariant.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "Runway.ui.h"

/*
 *  Constructs a CRwyDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CRwyDlg::CRwyDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CRwyDlg" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 20, 20, sizePolicy().hasHeightForWidth() ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pRwyList = new QListBox( this, "pRwyList" );
    pRwyList->setGeometry( QRect( 6, 28, 86, 171 ) );
    pRwyList->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, pRwyList->sizePolicy().hasHeightForWidth() ) );
    QFont pRwyList_font(  pRwyList->font() );
    pRwyList_font.setFamily( "Liberation Mono Track" );
    pRwyList_font.setPointSize( 10 );
    pRwyList->setFont( pRwyList_font ); 
    pRwyList->setDragAutoScroll( FALSE );
    pRwyList->setColumnMode( QListBox::FixedNumber );
    pRwyList->setVariableHeight( FALSE );

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
    resize( QSize(120, 204).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pRwyList, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( pRwyListClicked(QListBoxItem*) ) );
    connect( pRwyList, SIGNAL( returnPressed(QListBoxItem*) ), this, SLOT( pRwyListClicked(QListBoxItem*) ) );

    // tab order
}

/*
 *  Destroys the object and frees any allocated resources
 */
CRwyDlg::~CRwyDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CRwyDlg::languageChange()
{
    setCaption( tr( "RWY" ) );
    pArcid->setText( QString::null );
}

