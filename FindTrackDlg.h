/****************************************************************************
** Form interface generated from reading ui file 'FindTrackDlg.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CFINDTRACKDLG_H
#define CFINDTRACKDLG_H

#include <qvariant.h>
#include <qdialog.h>
#include "StdAfx.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;

class CFindTrackDlg : public QDialog
{
    Q_OBJECT

public:
    CFindTrackDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CFindTrackDlg();

    QLabel* textLabel1;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QLabel* textLabel2;
    QLineEdit* textEdit1;

public slots:
    virtual void buttonOk_clicked();
    virtual void Init();
    virtual void searchedCallsignChanged( const QString & callsign );

protected:

protected slots:
    virtual void languageChange();

};

#endif // CFINDTRACKDLG_H
