/****************************************************************************
** Form implementation generated from reading ui file 'SystemInfo.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "SystemInfo.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "SystemInfo.ui.h"

/*
 *  Constructs a CSystemInfo as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
CSystemInfo::CSystemInfo( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "CSystemInfo" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 19, 10, 80, 20 ) );

    pUsername = new QLabel( this, "pUsername" );
    pUsername->setGeometry( QRect( 101, 10, 80, 21 ) );

    pSystemMode = new QLabel( this, "pSystemMode" );
    pSystemMode->setGeometry( QRect( 311, 10, 110, 21 ) );

    textLabel12 = new QLabel( this, "textLabel12" );
    textLabel12->setGeometry( QRect( 206, 10, 101, 21 ) );

    line2_4 = new QFrame( this, "line2_4" );
    line2_4->setGeometry( QRect( 0, 30, 430, 16 ) );
    line2_4->setFrameShape( QFrame::HLine );
    line2_4->setFrameShadow( QFrame::Sunken );
    line2_4->setFrameShape( QFrame::HLine );

    textLabel14 = new QLabel( this, "textLabel14" );
    textLabel14->setGeometry( QRect( 16, 50, 102, 21 ) );

    pTrackService = new QLabel( this, "pTrackService" );
    pTrackService->setGeometry( QRect( 121, 50, 110, 21 ) );
    pTrackService->setBackgroundMode( QLabel::PaletteBackground );
    pTrackService->setBackgroundOrigin( QLabel::ParentOrigin );

    textLabel16 = new QLabel( this, "textLabel16" );
    textLabel16->setGeometry( QRect( 283, 50, 87, 21 ) );

    pWeather = new QLabel( this, "pWeather" );
    pWeather->setGeometry( QRect( 372, 50, 50, 21 ) );

    line2 = new QFrame( this, "line2" );
    line2->setGeometry( QRect( 0, 70, 430, 16 ) );
    line2->setFrameShape( QFrame::HLine );
    line2->setFrameShadow( QFrame::Sunken );
    line2->setFrameShape( QFrame::HLine );

    textLabel22_2 = new QLabel( this, "textLabel22_2" );
    textLabel22_2->setGeometry( QRect( 221, 150, 141, 20 ) );

    AdaDataSetVersion = new QLabel( this, "AdaDataSetVersion" );
    AdaDataSetVersion->setGeometry( QRect( 364, 150, 70, 20 ) );

    textLabel19 = new QLabel( this, "textLabel19" );
    textLabel19->setGeometry( QRect( 144, 130, 130, 20 ) );

    line2_3_2 = new QFrame( this, "line2_3_2" );
    line2_3_2->setGeometry( QRect( 0, 110, 430, 16 ) );
    line2_3_2->setFrameShape( QFrame::HLine );
    line2_3_2->setFrameShadow( QFrame::Sunken );
    line2_3_2->setFrameShape( QFrame::HLine );

    textLabel20_2 = new QLabel( this, "textLabel20_2" );
    textLabel20_2->setGeometry( QRect( 5, 150, 52, 20 ) );

    pASDVersion = new QLabel( this, "pASDVersion" );
    pASDVersion->setGeometry( QRect( 60, 150, 70, 20 ) );

    textLabel18 = new QLabel( this, "textLabel18" );
    textLabel18->setGeometry( QRect( 20, 90, 86, 18 ) );

    pRocd = new QLabel( this, "pRocd" );
    pRocd->setGeometry( QRect( 111, 88, 50, 18 ) );

    pAPPACC = new QLabel( this, "pAPPACC" );
    pAPPACC->setGeometry( QRect( 360, 88, 70, 21 ) );

    textLabel18_2 = new QLabel( this, "textLabel18_2" );
    textLabel18_2->setGeometry( QRect( 280, 90, 70, 18 ) );
    languageChange();
    resize( QSize(434, 176).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CSystemInfo::~CSystemInfo()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CSystemInfo::languageChange()
{
    setCaption( tr( "System Information" ) );
    textLabel1->setText( tr( "Username : " ) );
    pUsername->setText( QString::null );
    pSystemMode->setText( QString::null );
    textLabel12->setText( tr( "System Mode : " ) );
    textLabel14->setText( tr( "Track Service : " ) );
    pTrackService->setText( QString::null );
    textLabel16->setText( tr( "Wx Service : " ) );
    pWeather->setText( QString::null );
    textLabel22_2->setText( tr( "Adaptation Data Set :" ) );
    AdaDataSetVersion->setText( QString::null );
    textLabel19->setText( tr( "Software Versions" ) );
    textLabel20_2->setText( tr( "Viewer :" ) );
    pASDVersion->setText( QString::null );
    textLabel18->setText( tr( "ROC/D Unit :" ) );
    pRocd->setText( QString::null );
    pAPPACC->setText( QString::null );
    textLabel18_2->setText( tr( "DB Type :" ) );
}

