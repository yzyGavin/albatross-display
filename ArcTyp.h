/****************************************************************************
** Form interface generated from reading ui file 'ArcTyp.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CATYPDLG_H
#define CATYPDLG_H

#include <qvariant.h>
#include <qdialog.h>
#include "StdAfx.h"
#include "DataMngr.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QListBox;
class QListBoxItem;
class CFlightPlan;

class CATypDlg : public QDialog
{
    Q_OBJECT

public:
    CATypDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CATypDlg();

    QLabel* pArcid;
    QListBox* pListATyp;

    int m_Default;
    QString m_ATyp;

    virtual void Init( QString Arcid, QString atyp );

public slots:
    virtual void ListAtypClicked( QListBoxItem * item );

protected:
    int m_TimerId;


protected slots:
    virtual void languageChange();

private:
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );

};

#endif // CATYPDLG_H
