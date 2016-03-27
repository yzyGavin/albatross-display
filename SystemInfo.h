/****************************************************************************
** Form interface generated from reading ui file 'SystemInfo.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CSYSTEMINFO_H
#define CSYSTEMINFO_H

#include <qvariant.h>
#include <qwidget.h>
#include "CommonDataTypes.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QFrame;

class CSystemInfo : public QWidget
{
    Q_OBJECT

public:
    CSystemInfo( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~CSystemInfo();

    QLabel* textLabel1;
    QLabel* pUsername;
    QLabel* pSystemMode;
    QLabel* textLabel12;
    QFrame* line2_4;
    QLabel* textLabel14;
    QLabel* pTrackService;
    QLabel* textLabel16;
    QLabel* pWeather;
    QFrame* line2;
    QLabel* textLabel22_2;
    QLabel* AdaDataSetVersion;
    QLabel* textLabel19;
    QFrame* line2_3_2;
    QLabel* textLabel20_2;
    QLabel* pASDVersion;
    QLabel* textLabel18;
    QLabel* pRocd;
    QLabel* pAPPACC;
    QLabel* textLabel18_2;

    virtual void Update();

public slots:
    virtual void init();
    virtual void closeEvent( QCloseEvent * );

protected:
    QValueList<ExtLineEdit> m_ExtLineTable;


protected slots:
    virtual void languageChange();

private:
    virtual void mousePressEvent( QMouseEvent * e );

};

#endif // CSYSTEMINFO_H
