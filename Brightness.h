/****************************************************************************
** Form interface generated from reading ui file 'Brightness.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CCOLORDLG_H
#define CCOLORDLG_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSlider;
class QLabel;
class QPushButton;

class CColorDlg : public QWidget
{
    Q_OBJECT

public:
    CColorDlg( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~CColorDlg();

    QSlider* OthSymbSlider;
    QSlider* WeatherSlider;
    QSlider* MouseSlider;
    QSlider* MasterSlider;
    QSlider* AocSymbSlider;
    QSlider* AocDbSlider;
    QLabel* Label_IDC_STATIC;
    QLabel* Label_IDC_STATIC1;
    QLabel* Label_IDC_STATIC2;
    QLabel* Label_IDC_STATIC3;
    QLabel* Label_IDC_STATIC4;
    QLabel* Label_IDC_STATIC5;
    QLabel* Label_IDC_STATIC6;
    QLabel* Label_IDC_STATIC7;
    QLabel* Label_IDC_STATIC11;
    QLabel* Label_IDC_STATIC8;
    QLabel* Label_IDC_STATIC9;
    QLabel* Label_IDC_STATIC8_2;
    QLabel* Label_IDC_STATIC10;
    QSlider* MapSymbSlider;
    QSlider* MapOutSlider;
    QSlider* MapInSlider;
    QSlider* DFSlider;
    QSlider* AsdToolsSlider;
    QSlider* WindowSlider;
    QPushButton* pushButton1;
    QLabel* textLabel2;
    QSlider* OthDbSlider;
    QSlider* RangeMarkSlider;

    int m_OldMasterValue;
    bool m_IsInit;

    void Init();
    virtual void mousePressEvent( QMouseEvent * e );

public slots:
    virtual void MasterSliderValueChanged( int value );
    virtual void AocSymbSliderValueChanged( int value );
    virtual void AocDbSliderValueChanged( int value );
    virtual void OthSymbSliderValueChanged( int value );
    virtual void OthDbSliderValueChanged( int value );
    virtual void MouseSliderValueChanged( int value );
    virtual void WeatherSliderValueChanged( int value );
    virtual void MapInSliderValueChanged( int value );
    virtual void MapOutSliderValueChanged( int value );
    virtual void MapSymbSliderValueChanged( int value );
    virtual void RngMarkSliderValueChanged( int value );
    virtual void WindowSliderValueChanged( int value );
    virtual void AsdToolsSliderValueChanged( int );
    virtual void DFSliderValueChanged(int value);
    virtual void ResetPressed();
    virtual void closeEvent( QCloseEvent * );

protected:

protected slots:
    virtual void languageChange();

};

#endif // CCOLORDLG_H
