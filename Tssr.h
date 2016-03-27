/****************************************************************************
** Form interface generated from reading ui file 'Tssr.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CTSSRDLG_H
#define CTSSRDLG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QListBox;
class QListBoxItem;

class CTssrDlg : public QDialog
{
    Q_OBJECT

public:
    CTssrDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CTssrDlg();

    QLabel* pSsr;
    QPushButton* pOther;
    QListBox* pSsrList;

    bool m_Other;
    QString m_Arcid;

    virtual int DoModal( QString Tssr );

public slots:
    virtual void pSsrListPressed( QListBoxItem * item );
    virtual void pOtherPressed();
    virtual void SetItemAtTop( QListBoxItem * Item );

protected:
    int m_TimerId;


protected slots:
    virtual void languageChange();

private:
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );

};

#endif // CTSSRDLG_H
