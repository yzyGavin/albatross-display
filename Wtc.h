/****************************************************************************
** Form interface generated from reading ui file 'Wtc.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CWTCDLG_H
#define CWTCDLG_H

#include <qvariant.h>
#include <qdialog.h>
#include "StdAfx.h"
#include "DataMngr.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListBox;
class QListBoxItem;
class QLabel;
class CFlightPlan;

class CWtcDlg : public QDialog
{
    Q_OBJECT

public:
    CWtcDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CWtcDlg();

    QListBox* pListWtc;
    QLabel* pArcid;

    QString m_Wtc;

    virtual void Init( QString Arcid, QString wtc, QString Atyp );

public slots:
    virtual void pListWtcClicked( QListBoxItem * item );

protected:
    int m_TimerId;


protected slots:
    virtual void languageChange();

private:
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );

};

#endif // CWTCDLG_H
