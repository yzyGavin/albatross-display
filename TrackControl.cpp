/****************************************************************************
** Form implementation generated from reading ui file 'TrackControl.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "TrackControl.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qslider.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "TrackControl.ui.h"
/*
 *  Constructs a TrackControl as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
TrackControl::TrackControl( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "TrackControl" );
    QFont f( font() );
    f.setFamily( "Liberation Mono Track" );
    f.setPointSize( 11 );
    setFont( f ); 

    textLabel3 = new QLabel( this, "textLabel3" );
    textLabel3->setGeometry( QRect( 12, 179, 24, 18 ) );
    textLabel3->setAlignment( int( QLabel::AlignCenter ) );

    HaloLabel = new QLabel( this, "HaloLabel" );
    HaloLabel->setGeometry( QRect( 12, 207, 24, 18 ) );
    HaloLabel->setLineWidth( 1 );
    HaloLabel->setAlignment( int( QLabel::AlignCenter ) );

    textLabel5 = new QLabel( this, "textLabel5" );
    textLabel5->setGeometry( QRect( 11, 252, 24, 18 ) );
    textLabel5->setAlignment( int( QLabel::AlignCenter ) );

    DotsLabel = new QLabel( this, "DotsLabel" );
    DotsLabel->setGeometry( QRect( 12, 135, 24, 18 ) );
    DotsLabel->setAlignment( int( QLabel::AlignCenter ) );

    HistoryDots = new QSlider( this, "HistoryDots" );
    HistoryDots->setGeometry( QRect( 18, 156, 290, 21 ) );
    HistoryDots->setMaxValue( 10 );
    HistoryDots->setPageStep( 1 );
    HistoryDots->setOrientation( QSlider::Horizontal );
    HistoryDots->setTickmarks( QSlider::Below );
    HistoryDots->setTickInterval( 1 );

    Prediction_Vector = new QButtonGroup( this, "Prediction_Vector" );
    Prediction_Vector->setGeometry( QRect( 6, 6, 320, 120 ) );

    UncoupledVector = new QCheckBox( Prediction_Vector, "UncoupledVector" );
    UncoupledVector->setGeometry( QRect( 6, 25, 90, 18 ) );

    textLabel1 = new QLabel( Prediction_Vector, "textLabel1" );
    textLabel1->setGeometry( QRect( 6, 94, 24, 18 ) );
    textLabel1->setAlignment( int( QLabel::AlignCenter ) );

    VectorLabel = new QLabel( Prediction_Vector, "VectorLabel" );
    VectorLabel->setGeometry( QRect( 5, 49, 26, 18 ) );
    VectorLabel->setAlignment( int( QLabel::AlignCenter ) );

    SliderVector = new QSlider( Prediction_Vector, "SliderVector" );
    SliderVector->setGeometry( QRect( 12, 70, 290, 21 ) );
    SliderVector->setMaxValue( 18 );
    SliderVector->setLineStep( 1 );
    SliderVector->setPageStep( 1 );
    SliderVector->setOrientation( QSlider::Horizontal );
    SliderVector->setTickmarks( QSlider::Below );
    SliderVector->setTickInterval( 1 );

    textLabel2 = new QLabel( Prediction_Vector, "textLabel2" );
    textLabel2->setGeometry( QRect( 285, 94, 26, 18 ) );
    textLabel2->setAlignment( int( QLabel::AlignCenter ) );

    TimeMarksVector = new QCheckBox( Prediction_Vector, "TimeMarksVector" );
    TimeMarksVector->setGeometry( QRect( 215, 25, 97, 18 ) );

    CoupledVector = new QCheckBox( Prediction_Vector, "CoupledVector" );
    CoupledVector->setGeometry( QRect( 118, 25, 75, 18 ) );

    textLabel2_2_2 = new QLabel( Prediction_Vector, "textLabel2_2_2" );
    textLabel2_2_2->setGeometry( QRect( 118, 94, 80, 18 ) );
    textLabel2_2_2->setAlignment( int( QLabel::AlignCenter ) );

    textLabel4 = new QLabel( this, "textLabel4" );
    textLabel4->setGeometry( QRect( 293, 179, 24, 18 ) );
    textLabel4->setAlignment( int( QLabel::AlignCenter ) );

    textLabel6 = new QLabel( this, "textLabel6" );
    textLabel6->setGeometry( QRect( 293, 252, 24, 18 ) );
    textLabel6->setAlignment( int( QLabel::AlignCenter ) );

    textLabel2_2 = new QLabel( this, "textLabel2_2" );
    textLabel2_2->setGeometry( QRect( 118, 179, 80, 18 ) );
    textLabel2_2->setAlignment( int( QLabel::AlignCenter ) );

    textLabel3_2 = new QLabel( this, "textLabel3_2" );
    textLabel3_2->setGeometry( QRect( 118, 252, 80, 18 ) );
    textLabel3_2->setAlignment( int( QLabel::AlignCenter ) );

    Halo = new QSlider( this, "Halo" );
    Halo->setEnabled( TRUE );
    Halo->setGeometry( QRect( 18, 227, 290, 21 ) );
    Halo->setMaxValue( 5 );
    Halo->setLineStep( 1 );
    Halo->setPageStep( 1 );
    Halo->setOrientation( QSlider::Horizontal );
    Halo->setTickmarks( QSlider::Below );
    Halo->setTickInterval( 1 );

    Fields = new QButtonGroup( this, "Fields" );
    Fields->setGeometry( QRect( 6, 280, 320, 210 ) );

    textLabel5_2 = new QLabel( Fields, "textLabel5_2" );
    textLabel5_2->setGeometry( QRect( 12, 66, 60, 18 ) );
    textLabel5_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );

    textLabel16 = new QLabel( Fields, "textLabel16" );
    textLabel16->setGeometry( QRect( 116, 25, 35, 18 ) );
    textLabel16->setAlignment( int( QLabel::AlignCenter ) );

    textLabel6_2 = new QLabel( Fields, "textLabel6_2" );
    textLabel6_2->setGeometry( QRect( 68, 25, 35, 18 ) );
    textLabel6_2->setAlignment( int( QLabel::AlignCenter ) );

    textLabel18 = new QLabel( Fields, "textLabel18" );
    textLabel18->setGeometry( QRect( 212, 25, 35, 18 ) );
    textLabel18->setAlignment( int( QLabel::AlignCenter ) );

    textLabel4_2 = new QLabel( Fields, "textLabel4_2" );
    textLabel4_2->setGeometry( QRect( 12, 46, 60, 18 ) );
    textLabel4_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );

    textLabel19 = new QLabel( Fields, "textLabel19" );
    textLabel19->setGeometry( QRect( 263, 25, 28, 18 ) );
    textLabel19->setAlignment( int( QLabel::AlignCenter ) );

    Field0 = new QCheckBox( Fields, "Field0" );
    Field0->setGeometry( QRect( 79, 48, 13, 13 ) );
    Fields->insert( Field0, 0 );

    Field1 = new QCheckBox( Fields, "Field1" );
    Field1->setGeometry( QRect( 79, 68, 13, 13 ) );
    Fields->insert( Field1, 1 );

    Field11 = new QCheckBox( Fields, "Field11" );
    Field11->setGeometry( QRect( 127, 68, 13, 13 ) );
    Fields->insert( Field11, 2 );

    Field25 = new QCheckBox( Fields, "Field25" );
    Field25->setGeometry( QRect( 271, 48, 13, 13 ) );
    Fields->insert( Field25, 0 );

    Field10 = new QCheckBox( Fields, "Field10" );
    Field10->setGeometry( QRect( 127, 48, 13, 13 ) );
    Fields->insert( Field10, 2 );

    Field27 = new QCheckBox( Fields, "Field27" );
    Field27->setGeometry( QRect( 271, 108, 16, 16 ) );

    Field13 = new QCheckBox( Fields, "Field13" );
    Field13->setGeometry( QRect( 127, 88, 13, 13 ) );
    Fields->insert( Field13, 0 );

    Field5 = new QCheckBox( Fields, "Field5" );
    Field5->setGeometry( QRect( 79, 128, 13, 13 ) );
    Fields->insert( Field5, 2 );

    textLabel9 = new QLabel( Fields, "textLabel9" );
    textLabel9->setGeometry( QRect( 12, 106, 60, 18 ) );
    textLabel9->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );

    Field16 = new QCheckBox( Fields, "Field16" );
    Field16->setGeometry( QRect( 127, 148, 16, 16 ) );

    Field14 = new QCheckBox( Fields, "Field14" );
    Field14->setGeometry( QRect( 127, 108, 16, 16 ) );

    Field6 = new QCheckBox( Fields, "Field6" );
    Field6->setGeometry( QRect( 79, 148, 13, 13 ) );
    Fields->insert( Field6, 2 );

    Field22 = new QCheckBox( Fields, "Field22" );
    Field22->setGeometry( QRect( 223, 88, 13, 13 ) );
    Fields->insert( Field22, 0 );

    textLabel8 = new QLabel( Fields, "textLabel8" );
    textLabel8->setGeometry( QRect( 12, 86, 60, 18 ) );
    textLabel8->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );

    textLabel14 = new QLabel( Fields, "textLabel14" );
    textLabel14->setGeometry( QRect( 12, 186, 60, 18 ) );
    textLabel14->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );

    Field3 = new QCheckBox( Fields, "Field3" );
    Field3->setGeometry( QRect( 79, 88, 13, 13 ) );
    Fields->insert( Field3, 3 );

    Field24 = new QCheckBox( Fields, "Field24" );
    Field24->setGeometry( QRect( 223, 148, 16, 16 ) );

    Field8 = new QCheckBox( Fields, "Field8" );
    Field8->setGeometry( QRect( 79, 188, 13, 13 ) );
    Fields->insert( Field8, 2 );

    textLabel11 = new QLabel( Fields, "textLabel11" );
    textLabel11->setGeometry( QRect( 12, 146, 60, 18 ) );
    textLabel11->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );

    Field7 = new QCheckBox( Fields, "Field7" );
    Field7->setGeometry( QRect( 79, 168, 13, 13 ) );
    Fields->insert( Field7, 2 );

    Field15 = new QCheckBox( Fields, "Field15" );
    Field15->setGeometry( QRect( 127, 128, 16, 16 ) );

    Field4 = new QCheckBox( Fields, "Field4" );
    Field4->setGeometry( QRect( 79, 108, 13, 13 ) );
    Fields->insert( Field4, 2 );

    textLabel13 = new QLabel( Fields, "textLabel13" );
    textLabel13->setGeometry( QRect( 12, 166, 60, 18 ) );
    textLabel13->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );

    Field28 = new QCheckBox( Fields, "Field28" );
    Field28->setGeometry( QRect( 271, 148, 16, 16 ) );

    textLabel10 = new QLabel( Fields, "textLabel10" );
    textLabel10->setGeometry( QRect( 12, 126, 60, 18 ) );
    textLabel10->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );

    Field26 = new QCheckBox( Fields, "Field26" );
    Field26->setGeometry( QRect( 271, 88, 13, 13 ) );
    Fields->insert( Field26, 0 );

    Field23 = new QCheckBox( Fields, "Field23" );
    Field23->setGeometry( QRect( 223, 108, 16, 16 ) );

    pLcmReset = new QPushButton( this, "pLcmReset" );
    pLcmReset->setGeometry( QRect( 0, 550, 330, 30 ) );
    pLcmReset->setToggleButton( FALSE );
    pLcmReset->setOn( FALSE );
    pLcmReset->setAutoDefault( TRUE );
    pLcmReset->setDefault( TRUE );

    FontSize = new QButtonGroup( this, "FontSize" );
    FontSize->setGeometry( QRect( 6, 497, 320, 51 ) );

    FontSize1 = new QRadioButton( FontSize, "FontSize1" );
    FontSize1->setGeometry( QRect( 11, 25, 90, 18 ) );
    QFont FontSize1_font(  FontSize1->font() );
    FontSize1_font.setPointSize( 10 );
    FontSize1->setFont( FontSize1_font ); 
    FontSize->insert( FontSize1, 0 );

    FontSize2 = new QRadioButton( FontSize, "FontSize2" );
    FontSize2->setGeometry( QRect( 115, 25, 90, 18 ) );
    QFont FontSize2_font(  FontSize2->font() );
    FontSize2->setFont( FontSize2_font ); 
    FontSize->insert( FontSize2, 1 );

    FontSize3 = new QRadioButton( FontSize, "FontSize3" );
    FontSize3->setGeometry( QRect( 217, 25, 100, 18 ) );
    QFont FontSize3_font(  FontSize3->font() );
    FontSize3_font.setPointSize( 12 );
    FontSize3->setFont( FontSize3_font ); 
    FontSize->insert( FontSize3, 2 );
    languageChange();
    resize( QSize(332, 585).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( SliderVector, SIGNAL( valueChanged(int) ), this, SLOT( SliderVectorValueChanged(int) ) );
    connect( HistoryDots, SIGNAL( valueChanged(int) ), this, SLOT( HistoryDotsValueChanged(int) ) );
    connect( Halo, SIGNAL( valueChanged(int) ), this, SLOT( HaloValueChanged(int) ) );
    connect( FontSize, SIGNAL( clicked(int) ), this, SLOT( FontSizePressed(int) ) );
    connect( Fields, SIGNAL( clicked(int) ), this, SLOT( FieldsPressed(int) ) );
    connect( UncoupledVector, SIGNAL( toggled(bool) ), this, SLOT( UncoupledVectorToggled(bool) ) );
    connect( CoupledVector, SIGNAL( toggled(bool) ), this, SLOT( CoupledVectorToggled(bool) ) );
    connect( TimeMarksVector, SIGNAL( toggled(bool) ), this, SLOT( TimeMarksVectorToggled(bool) ) );
    connect( pLcmReset, SIGNAL( clicked() ), this, SLOT( pLcmResetPressed() ) );
    connect( SliderVector, SIGNAL( sliderPressed() ), this, SLOT( SliderPressedSlot() ) );
    connect( SliderVector, SIGNAL( sliderReleased() ), this, SLOT( SliderReleaseSlot() ) );
    connect( HistoryDots, SIGNAL( sliderPressed() ), this, SLOT( SliderPressedSlot() ) );
    connect( HistoryDots, SIGNAL( sliderReleased() ), this, SLOT( SliderReleaseSlot() ) );
    connect( Halo, SIGNAL( sliderPressed() ), this, SLOT( SliderPressedSlot() ) );
    connect( Halo, SIGNAL( sliderReleased() ), this, SLOT( SliderReleaseSlot() ) );

    // tab order
    setTabOrder( UncoupledVector, CoupledVector );
    setTabOrder( CoupledVector, TimeMarksVector );
    setTabOrder( TimeMarksVector, SliderVector );
    setTabOrder( SliderVector, HistoryDots );
    setTabOrder( HistoryDots, Halo );
    setTabOrder( Halo, Field0 );
    setTabOrder( Field0, Field1 );
    setTabOrder( Field1, Field3 );
    setTabOrder( Field3, Field4 );
    setTabOrder( Field4, Field5 );
    setTabOrder( Field5, Field6 );
    setTabOrder( Field6, Field7 );
    setTabOrder( Field7, Field8 );
    setTabOrder( Field8, Field10 );
    setTabOrder( Field10, Field11 );
    setTabOrder( Field11, Field13 );
    setTabOrder( Field13, Field14 );
    setTabOrder( Field14, Field15 );
    setTabOrder( Field15, Field16 );
    setTabOrder( Field16, Field22 );
    setTabOrder( Field22, Field23 );
    setTabOrder( Field23, Field24 );
    setTabOrder( Field24, Field25 );
    setTabOrder( Field25, Field26 );
    setTabOrder( Field26, Field27 );
    setTabOrder( Field27, Field28 );
    setTabOrder( Field28, FontSize1 );
    setTabOrder( FontSize1, FontSize2 );
    setTabOrder( FontSize2, FontSize3 );
    setTabOrder( FontSize3, pLcmReset );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
TrackControl::~TrackControl()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void TrackControl::languageChange()
{
    setCaption( tr( "Track Control" ) );
    textLabel3->setText( tr( "0" ) );
    HaloLabel->setText( tr( "0" ) );
    textLabel5->setText( tr( "0" ) );
    DotsLabel->setText( tr( "5" ) );
    Prediction_Vector->setTitle( tr( "Prediction Vector" ) );
    UncoupledVector->setText( tr( "Uncoupled" ) );
    textLabel1->setText( tr( "0" ) );
    VectorLabel->setText( tr( "0" ) );
    textLabel2->setText( tr( "180" ) );
    TimeMarksVector->setText( tr( "Time Marks" ) );
    CoupledVector->setText( tr( "Coupled" ) );
    textLabel2_2_2->setText( tr( "Length (s)" ) );
    textLabel4->setText( tr( "10" ) );
    textLabel6->setText( tr( "5" ) );
    textLabel2_2->setText( tr( "Afterglows" ) );
    textLabel3_2->setText( tr( "Halo (Nm)" ) );
    Fields->setTitle( tr( "Data Block Optional Fields" ) );
    textLabel5_2->setText( tr( "RWY" ) );
    textLabel16->setText( tr( "Lim" ) );
    textLabel6_2->setText( tr( "Full" ) );
    textLabel18->setText( tr( "Unc" ) );
    textLabel4_2->setText( tr( "ATYP" ) );
    textLabel19->setText( tr( "Tag" ) );
    Field0->setText( QString::null );
    Field1->setText( QString::null );
    Field11->setText( QString::null );
    Field25->setText( QString::null );
    Field10->setText( QString::null );
    Field27->setText( tr( "checkBox24" ) );
    Field13->setText( QString::null );
    Field5->setText( QString::null );
    textLabel9->setText( tr( "ROC/D" ) );
    Field16->setText( tr( "checkBox26" ) );
    Field14->setText( tr( "checkBox24" ) );
    Field6->setText( QString::null );
    Field22->setText( QString::null );
    textLabel8->setText( tr( "GSP" ) );
    textLabel14->setText( tr( "SID" ) );
    Field3->setText( QString::null );
    Field24->setText( tr( "checkBox24" ) );
    Field8->setText( QString::null );
    textLabel11->setText( tr( "MOD S" ) );
    Field7->setText( QString::null );
    Field15->setText( tr( "checkBox25" ) );
    Field4->setText( QString::null );
    textLabel13->setText( tr( "ADES" ) );
    Field28->setText( tr( "checkBox24" ) );
    textLabel10->setText( tr( "RVR" ) );
    Field26->setText( QString::null );
    Field23->setText( tr( "checkBox24" ) );
    pLcmReset->setText( tr( "LCM Reset" ) );
    FontSize->setTitle( tr( "Data Block Font Size" ) );
    FontSize1->setText( tr( "Size 1" ) );
    FontSize2->setText( tr( "Size 2" ) );
    FontSize3->setText( tr( "Size 3" ) );
}

