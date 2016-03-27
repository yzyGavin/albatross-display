/****************************************************************************
** Form implementation generated from reading ui file 'Brightness.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Brightness.h"

#include <qvariant.h>
#include <qslider.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "Brightness.ui.h"

/*
 *  Constructs a CColorDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
CColorDlg::CColorDlg( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "CColorDlg" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 

    OthSymbSlider = new QSlider( this, "OthSymbSlider" );
    OthSymbSlider->setGeometry( QRect( 6, 167, 272, 20 ) );
    OthSymbSlider->setMinValue( 50 );
    OthSymbSlider->setMaxValue( 120 );
    OthSymbSlider->setValue( 100 );
    OthSymbSlider->setOrientation( QSlider::Horizontal );
    OthSymbSlider->setTickmarks( QSlider::Below );
    OthSymbSlider->setTickInterval( 70 );

    WeatherSlider = new QSlider( this, "WeatherSlider" );
    WeatherSlider->setGeometry( QRect( 6, 311, 272, 20 ) );
    WeatherSlider->setMinValue( 50 );
    WeatherSlider->setMaxValue( 120 );
    WeatherSlider->setValue( 100 );
    WeatherSlider->setOrientation( QSlider::Horizontal );
    WeatherSlider->setTickmarks( QSlider::Below );
    WeatherSlider->setTickInterval( 70 );

    MouseSlider = new QSlider( this, "MouseSlider" );
    MouseSlider->setGeometry( QRect( 6, 263, 272, 20 ) );
    MouseSlider->setMinValue( 50 );
    MouseSlider->setMaxValue( 120 );
    MouseSlider->setValue( 100 );
    MouseSlider->setOrientation( QSlider::Horizontal );
    MouseSlider->setTickmarks( QSlider::Below );
    MouseSlider->setTickInterval( 70 );

    MasterSlider = new QSlider( this, "MasterSlider" );
    MasterSlider->setGeometry( QRect( 6, 23, 272, 20 ) );
    MasterSlider->setMinValue( 50 );
    MasterSlider->setMaxValue( 120 );
    MasterSlider->setValue( 100 );
    MasterSlider->setOrientation( QSlider::Horizontal );
    MasterSlider->setTickmarks( QSlider::Below );
    MasterSlider->setTickInterval( 70 );

    AocSymbSlider = new QSlider( this, "AocSymbSlider" );
    AocSymbSlider->setGeometry( QRect( 6, 71, 272, 20 ) );
    AocSymbSlider->setMinValue( 50 );
    AocSymbSlider->setMaxValue( 120 );
    AocSymbSlider->setValue( 100 );
    AocSymbSlider->setOrientation( QSlider::Horizontal );
    AocSymbSlider->setTickmarks( QSlider::Below );
    AocSymbSlider->setTickInterval( 70 );

    AocDbSlider = new QSlider( this, "AocDbSlider" );
    AocDbSlider->setGeometry( QRect( 6, 119, 272, 20 ) );
    AocDbSlider->setMinValue( 50 );
    AocDbSlider->setMaxValue( 120 );
    AocDbSlider->setValue( 100 );
    AocDbSlider->setOrientation( QSlider::Horizontal );
    AocDbSlider->setTickmarks( QSlider::Below );
    AocDbSlider->setTickInterval( 70 );

    Label_IDC_STATIC = new QLabel( this, "Label_IDC_STATIC" );
    Label_IDC_STATIC->setGeometry( QRect( 6, 53, 272, 18 ) );
    Label_IDC_STATIC->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC->setFrameShape( QLabel::NoFrame );
    Label_IDC_STATIC->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC->setLineWidth( 2 );
    Label_IDC_STATIC->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC1 = new QLabel( this, "Label_IDC_STATIC1" );
    Label_IDC_STATIC1->setGeometry( QRect( 6, 101, 272, 18 ) );
    Label_IDC_STATIC1->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC1->setFrameShape( QLabel::NoFrame );
    Label_IDC_STATIC1->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC1->setLineWidth( 2 );
    Label_IDC_STATIC1->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC2 = new QLabel( this, "Label_IDC_STATIC2" );
    Label_IDC_STATIC2->setGeometry( QRect( 5, 149, 272, 18 ) );
    Label_IDC_STATIC2->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC2->setFrameShape( QLabel::NoFrame );
    Label_IDC_STATIC2->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC2->setLineWidth( 2 );
    Label_IDC_STATIC2->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC3 = new QLabel( this, "Label_IDC_STATIC3" );
    Label_IDC_STATIC3->setGeometry( QRect( 6, 197, 272, 18 ) );
    Label_IDC_STATIC3->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC3->setFrameShape( QLabel::NoFrame );
    Label_IDC_STATIC3->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC3->setLineWidth( 2 );
    Label_IDC_STATIC3->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC4 = new QLabel( this, "Label_IDC_STATIC4" );
    Label_IDC_STATIC4->setGeometry( QRect( 6, 245, 272, 18 ) );
    Label_IDC_STATIC4->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC4->setFrameShape( QLabel::NoFrame );
    Label_IDC_STATIC4->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC4->setLineWidth( 2 );
    Label_IDC_STATIC4->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC5 = new QLabel( this, "Label_IDC_STATIC5" );
    Label_IDC_STATIC5->setGeometry( QRect( 6, 293, 272, 18 ) );
    Label_IDC_STATIC5->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC5->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC5->setLineWidth( 2 );
    Label_IDC_STATIC5->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC6 = new QLabel( this, "Label_IDC_STATIC6" );
    Label_IDC_STATIC6->setGeometry( QRect( 6, 341, 272, 18 ) );
    Label_IDC_STATIC6->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC6->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC6->setLineWidth( 2 );
    Label_IDC_STATIC6->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC7 = new QLabel( this, "Label_IDC_STATIC7" );
    Label_IDC_STATIC7->setGeometry( QRect( 6, 389, 272, 18 ) );
    Label_IDC_STATIC7->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC7->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC7->setLineWidth( 2 );
    Label_IDC_STATIC7->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC11 = new QLabel( this, "Label_IDC_STATIC11" );
    Label_IDC_STATIC11->setGeometry( QRect( 6, 629, 272, 18 ) );
    Label_IDC_STATIC11->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC11->setFrameShape( QLabel::NoFrame );
    Label_IDC_STATIC11->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC11->setLineWidth( 2 );
    Label_IDC_STATIC11->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC8 = new QLabel( this, "Label_IDC_STATIC8" );
    Label_IDC_STATIC8->setGeometry( QRect( 6, 437, 272, 18 ) );
    Label_IDC_STATIC8->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC8->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC8->setLineWidth( 2 );
    Label_IDC_STATIC8->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC9 = new QLabel( this, "Label_IDC_STATIC9" );
    Label_IDC_STATIC9->setGeometry( QRect( 7, 485, 272, 18 ) );
    Label_IDC_STATIC9->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC9->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC9->setLineWidth( 2 );
    Label_IDC_STATIC9->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC8_2 = new QLabel( this, "Label_IDC_STATIC8_2" );
    Label_IDC_STATIC8_2->setGeometry( QRect( 6, 533, 272, 18 ) );
    Label_IDC_STATIC8_2->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC8_2->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC8_2->setLineWidth( 2 );
    Label_IDC_STATIC8_2->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    Label_IDC_STATIC10 = new QLabel( this, "Label_IDC_STATIC10" );
    Label_IDC_STATIC10->setGeometry( QRect( 7, 581, 272, 18 ) );
    Label_IDC_STATIC10->setFocusPolicy( QLabel::NoFocus );
    Label_IDC_STATIC10->setFrameShadow( QLabel::Plain );
    Label_IDC_STATIC10->setLineWidth( 2 );
    Label_IDC_STATIC10->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );

    MapSymbSlider = new QSlider( this, "MapSymbSlider" );
    MapSymbSlider->setGeometry( QRect( 6, 455, 272, 20 ) );
    MapSymbSlider->setMinValue( 50 );
    MapSymbSlider->setMaxValue( 120 );
    MapSymbSlider->setValue( 100 );
    MapSymbSlider->setOrientation( QSlider::Horizontal );
    MapSymbSlider->setTickmarks( QSlider::Below );
    MapSymbSlider->setTickInterval( 70 );

    MapOutSlider = new QSlider( this, "MapOutSlider" );
    MapOutSlider->setGeometry( QRect( 6, 407, 272, 20 ) );
    MapOutSlider->setMinValue( 50 );
    MapOutSlider->setMaxValue( 120 );
    MapOutSlider->setValue( 100 );
    MapOutSlider->setOrientation( QSlider::Horizontal );
    MapOutSlider->setTickmarks( QSlider::Below );
    MapOutSlider->setTickInterval( 70 );

    MapInSlider = new QSlider( this, "MapInSlider" );
    MapInSlider->setGeometry( QRect( 6, 359, 272, 20 ) );
    MapInSlider->setMinValue( 50 );
    MapInSlider->setMaxValue( 120 );
    MapInSlider->setValue( 100 );
    MapInSlider->setOrientation( QSlider::Horizontal );
    MapInSlider->setTickmarks( QSlider::Below );
    MapInSlider->setTickInterval( 70 );

    DFSlider = new QSlider( this, "DFSlider" );
    DFSlider->setGeometry( QRect( 6, 503, 272, 20 ) );
    DFSlider->setMinValue( 50 );
    DFSlider->setMaxValue( 120 );
    DFSlider->setValue( 100 );
    DFSlider->setOrientation( QSlider::Horizontal );
    DFSlider->setTickmarks( QSlider::Below );
    DFSlider->setTickInterval( 70 );

    AsdToolsSlider = new QSlider( this, "AsdToolsSlider" );
    AsdToolsSlider->setGeometry( QRect( 6, 551, 272, 20 ) );
    AsdToolsSlider->setMinValue( 50 );
    AsdToolsSlider->setMaxValue( 120 );
    AsdToolsSlider->setValue( 100 );
    AsdToolsSlider->setOrientation( QSlider::Horizontal );
    AsdToolsSlider->setTickmarks( QSlider::Below );
    AsdToolsSlider->setTickInterval( 70 );

    WindowSlider = new QSlider( this, "WindowSlider" );
    WindowSlider->setGeometry( QRect( 6, 647, 272, 20 ) );
    WindowSlider->setMinValue( 50 );
    WindowSlider->setMaxValue( 120 );
    WindowSlider->setValue( 100 );
    WindowSlider->setOrientation( QSlider::Horizontal );
    WindowSlider->setTickmarks( QSlider::Below );
    WindowSlider->setTickInterval( 70 );

    pushButton1 = new QPushButton( this, "pushButton1" );
    pushButton1->setGeometry( QRect( 8, 681, 272, 30 ) );
    pushButton1->setDefault( TRUE );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setGeometry( QRect( 6, 5, 272, 18 ) );
    textLabel2->setAlignment( int( QLabel::AlignCenter ) );

    OthDbSlider = new QSlider( this, "OthDbSlider" );
    OthDbSlider->setGeometry( QRect( 6, 215, 272, 20 ) );
    OthDbSlider->setMinValue( 50 );
    OthDbSlider->setMaxValue( 120 );
    OthDbSlider->setValue( 100 );
    OthDbSlider->setOrientation( QSlider::Horizontal );
    OthDbSlider->setTickmarks( QSlider::Below );
    OthDbSlider->setTickInterval( 70 );

    RangeMarkSlider = new QSlider( this, "RangeMarkSlider" );
    RangeMarkSlider->setGeometry( QRect( 7, 599, 272, 20 ) );
    RangeMarkSlider->setMinValue( 50 );
    RangeMarkSlider->setMaxValue( 120 );
    RangeMarkSlider->setValue( 100 );
    RangeMarkSlider->setOrientation( QSlider::Horizontal );
    RangeMarkSlider->setTickmarks( QSlider::Below );
    RangeMarkSlider->setTickInterval( 70 );
    languageChange();
    resize( QSize(284, 719).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( MasterSlider, SIGNAL( valueChanged(int) ), this, SLOT( MasterSliderValueChanged(int) ) );
    connect( AocSymbSlider, SIGNAL( valueChanged(int) ), this, SLOT( AocSymbSliderValueChanged(int) ) );
    connect( AocDbSlider, SIGNAL( valueChanged(int) ), this, SLOT( AocDbSliderValueChanged(int) ) );
    connect( OthSymbSlider, SIGNAL( valueChanged(int) ), this, SLOT( OthSymbSliderValueChanged(int) ) );
    connect( OthDbSlider, SIGNAL( valueChanged(int) ), this, SLOT( OthDbSliderValueChanged(int) ) );
    connect( MouseSlider, SIGNAL( valueChanged(int) ), this, SLOT( MouseSliderValueChanged(int) ) );
    connect( WeatherSlider, SIGNAL( valueChanged(int) ), this, SLOT( WeatherSliderValueChanged(int) ) );
    connect( MapInSlider, SIGNAL( valueChanged(int) ), this, SLOT( MapInSliderValueChanged(int) ) );
    connect( MapOutSlider, SIGNAL( valueChanged(int) ), this, SLOT( MapOutSliderValueChanged(int) ) );
    connect( MapSymbSlider, SIGNAL( valueChanged(int) ), this, SLOT( MapSymbSliderValueChanged(int) ) );
    connect( WindowSlider, SIGNAL( valueChanged(int) ), this, SLOT( WindowSliderValueChanged(int) ) );
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( ResetPressed() ) );
    connect( AsdToolsSlider, SIGNAL( valueChanged(int) ), this, SLOT( AsdToolsSliderValueChanged(int) ) );
    connect( DFSlider, SIGNAL( valueChanged(int) ), this, SLOT( DFSliderValueChanged(int) ) );
    connect( RangeMarkSlider, SIGNAL( valueChanged(int) ), this, SLOT( RngMarkSliderValueChanged(int) ) );

    // tab order
    setTabOrder( MasterSlider, AocSymbSlider );
    setTabOrder( AocSymbSlider, AocDbSlider );
    setTabOrder( AocDbSlider, OthSymbSlider );
    setTabOrder( OthSymbSlider, OthDbSlider );
    setTabOrder( OthDbSlider, MouseSlider );
    setTabOrder( MouseSlider, WeatherSlider );
    setTabOrder( WeatherSlider, MapInSlider );
    setTabOrder( MapInSlider, MapOutSlider );
    setTabOrder( MapOutSlider, MapSymbSlider );
    setTabOrder( MapSymbSlider, DFSlider );
    setTabOrder( DFSlider, AsdToolsSlider );
    setTabOrder( AsdToolsSlider, RangeMarkSlider );
    setTabOrder( RangeMarkSlider, WindowSlider );
    setTabOrder( WindowSlider, pushButton1 );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CColorDlg::~CColorDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CColorDlg::languageChange()
{
    setCaption( tr( "Brightness Control" ) );
    QToolTip::add( OthSymbSlider, QString::null );
    QToolTip::add( WeatherSlider, QString::null );
    QToolTip::add( MouseSlider, QString::null );
    QToolTip::add( AocDbSlider, QString::null );
    Label_IDC_STATIC->setText( tr( "Assumed Track Symbols" ) );
    Label_IDC_STATIC1->setText( tr( "Data Blocks of Assumed Tracks" ) );
    Label_IDC_STATIC2->setText( tr( "Other Track Symbols" ) );
    Label_IDC_STATIC3->setText( tr( "Data Blocks of Other Tracks" ) );
    Label_IDC_STATIC4->setText( tr( "Mouse Pointer" ) );
    Label_IDC_STATIC5->setText( tr( "Weather" ) );
    Label_IDC_STATIC6->setText( tr( "Map Infills" ) );
    Label_IDC_STATIC7->setText( tr( "Map Outlines" ) );
    Label_IDC_STATIC11->setText( tr( "Other Windows" ) );
    Label_IDC_STATIC8->setText( tr( "Map Symbols" ) );
    Label_IDC_STATIC9->setText( tr( "DF Vectors" ) );
    Label_IDC_STATIC8_2->setText( tr( "ASD Tools" ) );
    Label_IDC_STATIC10->setText( tr( "Range Markers" ) );
    QToolTip::add( MapSymbSlider, QString::null );
    QToolTip::add( MapOutSlider, QString::null );
    QToolTip::add( MapInSlider, QString::null );
    QToolTip::add( DFSlider, QString::null );
    QToolTip::add( AsdToolsSlider, QString::null );
    QToolTip::add( WindowSlider, QString::null );
    pushButton1->setText( tr( "Reset" ) );
    textLabel2->setText( tr( "Master" ) );
    QToolTip::add( OthDbSlider, QString::null );
    QToolTip::add( RangeMarkSlider, QString::null );
}

