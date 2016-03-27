/****************************************************************************
** Form implementation generated from reading ui file 'Wtc.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Wtc.h"

#include <qvariant.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "Wtc.ui.h"

/*
 *  Constructs a CWtcDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CWtcDlg::CWtcDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CWtcDlg" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 20, 20, sizePolicy().hasHeightForWidth() ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pListWtc = new QListBox( this, "pListWtc" );
    pListWtc->setGeometry( QRect( 6, 28, 86, 61 ) );
    pListWtc->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, pListWtc->sizePolicy().hasHeightForWidth() ) );
    QFont pListWtc_font(  pListWtc->font() );
    pListWtc_font.setFamily( "Liberation Mono Track" );
    pListWtc_font.setPointSize( 10 );
    pListWtc->setFont( pListWtc_font ); 
    pListWtc->setDragAutoScroll( FALSE );
    pListWtc->setColumnMode( QListBox::FixedNumber );
    pListWtc->setVariableHeight( FALSE );

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
    resize( QSize(120, 94).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pListWtc, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( pListWtcClicked(QListBoxItem*) ) );
    connect( pListWtc, SIGNAL( returnPressed(QListBoxItem*) ), this, SLOT( pListWtcClicked(QListBoxItem*) ) );

    // tab order
}

/*
 *  Destroys the object and frees any allocated resources
 */
CWtcDlg::~CWtcDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CWtcDlg::languageChange()
{
    setCaption( tr( "WTC" ) );
    pArcid->setText( QString::null );
}

