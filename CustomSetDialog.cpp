/****************************************************************************
** Form implementation generated from reading ui file 'CustomSetDialog.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "CustomSetDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "SaveAs.h"
#include "qlineedit.h"
#include "qvalidator.h"
#include "AsdApp.h"
#include "CustomSetDialog.ui.h"

/*
 *  Constructs a CustomizationSetDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CustomizationSetDialog::CustomizationSetDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CustomizationSetDialog" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pbSave = new QPushButton( this, "pbSave" );
    pbSave->setGeometry( QRect( 6, 233, 64, 25 ) );
    QFont pbSave_font(  pbSave->font() );
    pbSave->setFont( pbSave_font ); 

    pbSaveAs = new QPushButton( this, "pbSaveAs" );
    pbSaveAs->setGeometry( QRect( 76, 233, 64, 25 ) );
    QFont pbSaveAs_font(  pbSaveAs->font() );
    pbSaveAs->setFont( pbSaveAs_font ); 

    pbRecall = new QPushButton( this, "pbRecall" );
    pbRecall->setGeometry( QRect( 6, 203, 64, 25 ) );
    QFont pbRecall_font(  pbRecall->font() );
    pbRecall->setFont( pbRecall_font ); 

    pbDelete = new QPushButton( this, "pbDelete" );
    pbDelete->setGeometry( QRect( 76, 203, 64, 25 ) );
    QFont pbDelete_font(  pbDelete->font() );
    pbDelete->setFont( pbDelete_font ); 

    lbCustomSets = new QListBox( this, "lbCustomSets" );
    lbCustomSets->setGeometry( QRect( 10, 23, 126, 175 ) );
    QFont lbCustomSets_font(  lbCustomSets->font() );
    lbCustomSets_font.setFamily( "Liberation Mono Track" );
    lbCustomSets_font.setPointSize( 10 );
    lbCustomSets->setFont( lbCustomSets_font ); 
    lbCustomSets->setHScrollBarMode( QListBox::AlwaysOff );
    lbCustomSets->setSelectionMode( QListBox::Single );

    lCount = new QLabel( this, "lCount" );
    lCount->setGeometry( QRect( 10, 5, 126, 18 ) );
    QFont lCount_font(  lCount->font() );
    lCount_font.setFamily( "Liberation Mono Track" );
    lCount_font.setPointSize( 10 );
    lCount->setFont( lCount_font ); 
    lCount->setTextFormat( QLabel::RichText );
    lCount->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );
    languageChange();
    resize( QSize(146, 263).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pbDelete, SIGNAL( clicked() ), this, SLOT( ClickDelete() ) );
    connect( pbSaveAs, SIGNAL( clicked() ), this, SLOT( ClickSaveAs() ) );
    connect( lbCustomSets, SIGNAL( selectionChanged(QListBoxItem*) ), this, SLOT( SelectionChanged(QListBoxItem*) ) );
    connect( pbRecall, SIGNAL( clicked() ), this, SLOT( ClickRecall() ) );
    connect( pbSave, SIGNAL( clicked() ), this, SLOT( ClickSave() ) );

    // tab order
    setTabOrder( lbCustomSets, pbRecall );
    setTabOrder( pbRecall, pbDelete );
    setTabOrder( pbDelete, pbSave );
    setTabOrder( pbSave, pbSaveAs );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CustomizationSetDialog::~CustomizationSetDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CustomizationSetDialog::languageChange()
{
    setCaption( tr( "Custom Sets" ) );
    pbSave->setText( tr( "Save" ) );
    pbSaveAs->setText( tr( "Save As" ) );
    pbRecall->setText( tr( "Recall" ) );
    pbDelete->setText( tr( "Delete" ) );
    lbCustomSets->clear();
    lbCustomSets->insertItem( tr( "WWWWWWWWWW" ) );
    lbCustomSets->insertItem( tr( "02" ) );
    lbCustomSets->insertItem( tr( "03" ) );
    lbCustomSets->insertItem( tr( "04" ) );
    lbCustomSets->insertItem( tr( "05" ) );
    lbCustomSets->insertItem( tr( "06" ) );
    lbCustomSets->insertItem( tr( "07" ) );
    lbCustomSets->insertItem( tr( "08" ) );
    lbCustomSets->insertItem( tr( "09" ) );
    lbCustomSets->insertItem( tr( "10" ) );
    lCount->setText( tr( "N/30" ) );
}

