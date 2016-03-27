/****************************************************************************
** Form interface generated from reading ui file 'Runway.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CRWYDLG_H
#define CRWYDLG_H

#include <qvariant.h>
#include <qdialog.h>
#include "StdAfx.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListBox;
class QListBoxItem;
class QLabel;
class CFlightPlan;

class CRwyDlg : public QDialog
{
    Q_OBJECT

public:
    CRwyDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CRwyDlg();

    QListBox* pRwyList;
    QLabel* pArcid;

    QString m_Rwy;

    virtual void Init( QString Arcid, QString Ades, QString Rwy, QString ParallelRwy );

public slots:
    virtual void pRwyListClicked( QListBoxItem * item );

protected:
    int m_TimerId;


protected slots:
    virtual void languageChange();

private:
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );

};

#endif // CRWYDLG_H
