/****************************************************************************
** Form interface generated from reading ui file 'Ades.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CADESDLG_H
#define CADESDLG_H

#include <qvariant.h>
#include <qdialog.h>
#include "StdAfx.h"
#include "DataMngr.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QListBox;
class QListBoxItem;
class CFlightPlan;

class CAdesDlg : public QDialog
{
    Q_OBJECT

public:
    CAdesDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CAdesDlg();

    QPushButton* pAlt1;
    QPushButton* pAlt2;
    QLabel* pArcid;
    QListBox* pListAdes;

    QString m_Ades;
    QString m_Alt1;
    QString m_Alt2;

public slots:
    virtual void pListAdesClicked( QListBoxItem * item );
    virtual void pAlt1Clicked();
    virtual void pAlt2Clicked();
    virtual void Init( QString Arcid, QString Altrnt1, QString Altrnt2, QString ades );

protected:
    int m_TimerId;


protected slots:
    virtual void languageChange();

private:
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );

};

#endif // CADESDLG_H
