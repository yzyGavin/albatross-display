/****************************************************************************
** Form implementation generated from reading ui file 'Ades.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Ades.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "Ades.ui.h"

/*
 *  Constructs a CAdesDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CAdesDlg::CAdesDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CAdesDlg" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 20, 20, sizePolicy().hasHeightForWidth() ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pAlt1 = new QPushButton( this, "pAlt1" );
    pAlt1->setEnabled( FALSE );
    pAlt1->setGeometry( QRect( 6, 28, 86, 25 ) );
    QFont pAlt1_font(  pAlt1->font() );
    pAlt1_font.setFamily( "Liberation Mono Track" );
    pAlt1_font.setPointSize( 10 );
    pAlt1->setFont( pAlt1_font ); 
    pAlt1->setAutoDefault( TRUE );

    pAlt2 = new QPushButton( this, "pAlt2" );
    pAlt2->setEnabled( FALSE );
    pAlt2->setGeometry( QRect( 6, 58, 86, 25 ) );
    QFont pAlt2_font(  pAlt2->font() );
    pAlt2_font.setFamily( "Liberation Mono Track" );
    pAlt2_font.setPointSize( 10 );
    pAlt2->setFont( pAlt2_font ); 

    pArcid = new QLabel( this, "pArcid" );
    pArcid->setGeometry( QRect( 6, 5, 86, 18 ) );
    QFont pArcid_font(  pArcid->font() );
    pArcid_font.setFamily( "Liberation Mono Track" );
    pArcid_font.setPointSize( 10 );
    pArcid_font.setBold( TRUE );
    pArcid->setFont( pArcid_font ); 
    pArcid->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    pArcid->setIndent( -1 );

    pListAdes = new QListBox( this, "pListAdes" );
    pListAdes->setGeometry( QRect( 6, 88, 86, 171 ) );
    pListAdes->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, pListAdes->sizePolicy().hasHeightForWidth() ) );
    QFont pListAdes_font(  pListAdes->font() );
    pListAdes_font.setFamily( "Liberation Mono Track" );
    pListAdes_font.setPointSize( 10 );
    pListAdes->setFont( pListAdes_font ); 
    pListAdes->setDragAutoScroll( FALSE );
    pListAdes->setColumnMode( QListBox::FixedNumber );
    pListAdes->setVariableHeight( FALSE );
    languageChange();
    resize( QSize(120, 264).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pAlt1, SIGNAL( clicked() ), this, SLOT( pAlt1Clicked() ) );
    connect( pAlt2, SIGNAL( clicked() ), this, SLOT( pAlt2Clicked() ) );
    connect( pListAdes, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( pListAdesClicked(QListBoxItem*) ) );
    connect( pListAdes, SIGNAL( returnPressed(QListBoxItem*) ), this, SLOT( pListAdesClicked(QListBoxItem*) ) );

    // tab order
    setTabOrder( pAlt1, pAlt2 );
    setTabOrder( pAlt2, pListAdes );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CAdesDlg::~CAdesDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CAdesDlg::languageChange()
{
    setCaption( tr( "ADES" ) );
    pAlt1->setText( tr( "Alt1" ) );
    pAlt2->setText( tr( "Alt2" ) );
    pArcid->setText( QString::null );
}

