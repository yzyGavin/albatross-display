/****************************************************************************
** Form interface generated from reading ui file 'TrackControl.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef TRACKCONTROL_H
#define TRACKCONTROL_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QSlider;
class QButtonGroup;
class QCheckBox;
class QPushButton;
class QRadioButton;
class CRwdView;

class TrackControl : public QDialog
{
    Q_OBJECT

public:
    TrackControl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~TrackControl();

    QLabel* textLabel3;
    QLabel* HaloLabel;
    QLabel* textLabel5;
    QLabel* DotsLabel;
    QSlider* HistoryDots;
    QButtonGroup* Prediction_Vector;
    QCheckBox* UncoupledVector;
    QLabel* textLabel1;
    QLabel* VectorLabel;
    QSlider* SliderVector;
    QLabel* textLabel2;
    QCheckBox* TimeMarksVector;
    QCheckBox* CoupledVector;
    QLabel* textLabel2_2_2;
    QLabel* textLabel4;
    QLabel* textLabel6;
    QLabel* textLabel2_2;
    QLabel* textLabel3_2;
    QSlider* Halo;
    QButtonGroup* Fields;
    QLabel* textLabel5_2;
    QLabel* textLabel16;
    QLabel* textLabel6_2;
    QLabel* textLabel18;
    QLabel* textLabel4_2;
    QLabel* textLabel19;
    QCheckBox* Field0;
    QCheckBox* Field1;
    QCheckBox* Field11;
    QCheckBox* Field25;
    QCheckBox* Field10;
    QCheckBox* Field27;
    QCheckBox* Field13;
    QCheckBox* Field5;
    QLabel* textLabel9;
    QCheckBox* Field16;
    QCheckBox* Field14;
    QCheckBox* Field6;
    QCheckBox* Field22;
    QLabel* textLabel8;
    QLabel* textLabel14;
    QCheckBox* Field3;
    QCheckBox* Field24;
    QCheckBox* Field8;
    QLabel* textLabel11;
    QCheckBox* Field7;
    QCheckBox* Field15;
    QCheckBox* Field4;
    QLabel* textLabel13;
    QCheckBox* Field28;
    QLabel* textLabel10;
    QCheckBox* Field26;
    QCheckBox* Field23;
    QPushButton* pLcmReset;
    QButtonGroup* FontSize;
    QRadioButton* FontSize1;
    QRadioButton* FontSize2;
    QRadioButton* FontSize3;

    CRwdView* m_pView;

    void Init();
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void init();
    virtual void mousePressEvent( QMouseEvent * e );

public slots:
    virtual void SliderVectorValueChanged( int value );
    virtual void HistoryDotsValueChanged( int value );
    virtual void TrackControlDestroyed( QObject * );
    virtual void HaloValueChanged( int value );
    virtual void FieldsPressed( int value );
    virtual void FontSizePressed( int value );
    virtual void UncoupledVectorToggled( bool on );
    virtual void CoupledVectorToggled( bool on );
    virtual void TimeMarksVectorToggled( bool on );
    virtual void pLcmResetPressed();
    virtual void SliderPressedSlot();
    virtual void SliderReleaseSlot();

protected:
    bool m_bSliderPressed;
    int m_TimerId;


protected slots:
    virtual void languageChange();

};

#endif // TRACKCONTROL_H
