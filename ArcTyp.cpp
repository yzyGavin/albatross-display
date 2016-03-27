/****************************************************************************
** Form implementation generated from reading ui file 'ArcTyp.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "ArcTyp.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "ArcTyp.ui.h"

/*
 *  Constructs a CATypDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CATypDlg::CATypDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CATypDlg" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 20, 20, sizePolicy().hasHeightForWidth() ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pArcid = new QLabel( this, "pArcid" );
    pArcid->setGeometry( QRect( 6, 5, 86, 18 ) );
    QFont pArcid_font(  pArcid->font() );
    pArcid_font.setFamily( "Liberation Mono Track" );
    pArcid_font.setPointSize( 10 );
    pArcid_font.setBold( TRUE );
    pArcid->setFont( pArcid_font ); 
    pArcid->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    pArcid->setIndent( -1 );

    pListATyp = new QListBox( this, "pListATyp" );
    pListATyp->setGeometry( QRect( 6, 28, 86, 171 ) );
    pListATyp->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, pListATyp->sizePolicy().hasHeightForWidth() ) );
    QFont pListATyp_font(  pListATyp->font() );
    pListATyp_font.setFamily( "Liberation Mono Track" );
    pListATyp_font.setPointSize( 10 );
    pListATyp->setFont( pListATyp_font ); 
    pListATyp->setDragAutoScroll( FALSE );
    pListATyp->setColumnMode( QListBox::FixedNumber );
    pListATyp->setVariableHeight( FALSE );
    languageChange();
    resize( QSize(124, 204).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pListATyp, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( ListAtypClicked(QListBoxItem*) ) );
    connect( pListATyp, SIGNAL( returnPressed(QListBoxItem*) ), this, SLOT( ListAtypClicked(QListBoxItem*) ) );

    // tab order
}

/*
 *  Destroys the object and frees any allocated resources
 */
CATypDlg::~CATypDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CATypDlg::languageChange()
{
    setCaption( tr( "ATYP" ) );
    pArcid->setText( QString::null );
}

