/****************************************************************************
** Form implementation generated from reading ui file 'SaveAs.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "SaveAs.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "SaveAs.ui.h"

/*
 *  Constructs a CSSaveAs as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CSSaveAs::CSSaveAs( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CSSaveAs" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 
    setSizeGripEnabled( FALSE );

    pbOk = new QPushButton( this, "pbOk" );
    pbOk->setEnabled( FALSE );
    pbOk->setGeometry( QRect( 6, 32, 109, 25 ) );

    leName = new QLineEdit( this, "leName" );
    leName->setGeometry( QRect( 6, 5, 109, 22 ) );
    QFont leName_font(  leName->font() );
    leName_font.setFamily( "Liberation Mono Track" );
    leName_font.setPointSize( 10 );
    leName->setFont( leName_font ); 
    leName->setMouseTracking( FALSE );
    leName->setAcceptDrops( FALSE );
    leName->setFrameShape( QLineEdit::LineEditPanel );
    leName->setFrameShadow( QLineEdit::Sunken );
    leName->setMaxLength( 10 );
    languageChange();
    resize( QSize(124, 62).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pbOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( leName, SIGNAL( textChanged(const QString&) ), this, SLOT( LeCheckText(const QString&) ) );

    // tab order
    setTabOrder( leName, pbOk );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CSSaveAs::~CSSaveAs()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CSSaveAs::languageChange()
{
    setCaption( tr( "Save As" ) );
    pbOk->setText( tr( "Ok" ) );
    leName->setText( QString::null );
}

