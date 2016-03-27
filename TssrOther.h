/****************************************************************************
** Form interface generated from reading ui file 'TssrOther.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CTSSROTHERDLG_H
#define CTSSROTHERDLG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;

class CTssrOtherDlg : public QDialog
{
    Q_OBJECT

public:
    CTssrOtherDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CTssrOtherDlg();

    QLabel* pTssr;
    QPushButton* pUntag;
    QPushButton* pOK;
    QLineEdit* pArcid;

    bool m_Untag;
    QString m_Arcid;

    virtual int DoModal( QString Tssr, QString Arcid );

public slots:
    virtual void pOKPressed();
    virtual void pUntagPressed();
    virtual void ArcIdChanged( const QString & strInput );

protected:
    int m_TimerId;


protected slots:
    virtual void languageChange();

private:
    virtual void closeEvent( QCloseEvent * );
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );

};

#endif // CTSSROTHERDLG_H
