/****************************************************************************
** Form interface generated from reading ui file 'HeightFilterDlg.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CHEIGHTFILTERDLG_H
#define CHEIGHTFILTERDLG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QLabel;
class CView;

class CHeightFilterDlg : public QDialog
{
    Q_OBJECT

public:
    CHeightFilterDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CHeightFilterDlg();

    QGroupBox* DepartureGroup;
    QLineEdit* pDepartureEdit;
    QGroupBox* LowerGroup;
    QLineEdit* pLowerEdit;
    QPushButton* OkButton;
    QPushButton* buttonCancel;
    QGroupBox* UpperGroup;
    QLineEdit* pUpperEdit;
    QLabel* Error;

    CView* m_pView;

    virtual void Init();
    virtual bool CheckConsistency();
    virtual bool TextChanged( const QString & text, QLineEdit * Edit );
    virtual void init();
    virtual void mousePressEvent( QMouseEvent * e );

public slots:
    virtual void OkClicked();
    virtual void CancelClicked();
    virtual void pLowerEditTextChanged( const QString & strval );
    virtual void pUpperEditTextChanged( const QString & strval );
    virtual void pDepartureEditTextChanged( const QString & strval );

protected:
    int m_TimerId;
    QString m_UpperValue;
    QString m_LowerValue;
    QString m_DepartureValue;


protected slots:
    virtual void languageChange();

private:
    void keyPressEvent( QKeyEvent * e );

};

#endif // CHEIGHTFILTERDLG_H
