/****************************************************************************
** Form implementation generated from reading ui file 'Tssr.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Tssr.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "Tssr.ui.h"

/*
 *  Constructs a CTssrDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CTssrDlg::CTssrDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CTssrDlg" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pSsr = new QLabel( this, "pSsr" );
    pSsr->setGeometry( QRect( 6, 5, 86, 18 ) );
    QFont pSsr_font(  pSsr->font() );
    pSsr_font.setFamily( "Liberation Mono Track" );
    pSsr_font.setPointSize( 10 );
    pSsr_font.setBold( TRUE );
    pSsr->setFont( pSsr_font ); 
    pSsr->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    pOther = new QPushButton( this, "pOther" );
    pOther->setGeometry( QRect( 6, 204, 86, 25 ) );
    pOther->setAutoDefault( FALSE );
    pOther->setDefault( TRUE );

    pSsrList = new QListBox( this, "pSsrList" );
    pSsrList->setGeometry( QRect( 6, 28, 86, 171 ) );
    QFont pSsrList_font(  pSsrList->font() );
    pSsrList_font.setFamily( "Liberation Mono Track" );
    pSsrList_font.setPointSize( 10 );
    pSsrList->setFont( pSsrList_font ); 
    languageChange();
    resize( QSize(120, 234).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pSsrList, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( pSsrListPressed(QListBoxItem*) ) );
    connect( pOther, SIGNAL( clicked() ), this, SLOT( pOtherPressed() ) );
    connect( pSsrList, SIGNAL( returnPressed(QListBoxItem*) ), this, SLOT( pSsrListPressed(QListBoxItem*) ) );
    connect( pSsrList, SIGNAL( selectionChanged(QListBoxItem*) ), this, SLOT( SetItemAtTop(QListBoxItem*) ) );

    // tab order
    setTabOrder( pSsrList, pOther );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CTssrDlg::~CTssrDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CTssrDlg::languageChange()
{
    setCaption( tr( "TSSR" ) );
    pSsr->setText( QString::null );
    pOther->setText( tr( "Other" ) );
}

