/****************************************************************************
** Form interface generated from reading ui file 'FlSelect.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CFLSELECTDLG_H
#define CFLSELECTDLG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QListBox;
class QListBoxItem;
class QLabel;
class CFlightPlan;

class CFlSelectDlg : public QDialog
{
    Q_OBJECT

public:
    CFlSelectDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CFlSelectDlg();

    QPushButton* pDepDefault;
    QPushButton* pArrDefault;
    QPushButton* pOther;
    QListBox* pListLevel;
    QLabel* pArcid;

    QString m_KdOfAppClear;
    QPoint m_ListPos;
    int m_Default;
    int m_Cfl;
    QPoint m_ArrPos;
    QPoint m_DepPos;
    QPoint m_OtherPos;
    QSize PopupSize;
    bool m_FirstInit;
    int m_iKdAppClearNb;

    virtual void init();

public slots:
    virtual void pDepDefaultPressed();
    virtual void pArrDefaultPressed();
    virtual void pOtherPressed();
    virtual void pListLevelClicked( QListBoxItem * item );
    virtual void Init( QString Arcid, bool IsFpl, QString FltRul, QString Adep, QString DepRwy, bool Dep, bool Arr, int OldValue, QString AppKind, bool DisplayAppKind );

protected:
    int m_TimerId;


protected slots:
    virtual void languageChange();

private:
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );

};

#endif // CFLSELECTDLG_H
