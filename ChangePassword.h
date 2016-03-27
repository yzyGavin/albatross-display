/****************************************************************************
** Form interface generated from reading ui file 'ChangePassword.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CCHANGEPASSWD_H
#define CCHANGEPASSWD_H

#include <qvariant.h>
#include <qdialog.h>
#include "AsdApp.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;

class CChangePasswd : public QDialog
{
    Q_OBJECT

public:
    CChangePasswd( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CChangePasswd();

    QLabel* Error;
    QLabel* textLabel5;
    QLabel* textLabel6;
    QLabel* textLabel7;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QLineEdit* CurrentPassword;
    QLineEdit* NewPassword;
    QLineEdit* ConfirmPassword;

public slots:
    virtual void OkClicked();
    virtual void CancelClicked();
    virtual void PasswordChanged( const QString & strPassword );
    virtual void NewPasswordChanged( const QString & strNewPassword );
    virtual void ConfirmPasswordChanged( const QString & strConfirmPassword );
    virtual void mousePressEvent( QMouseEvent * e );

protected:

protected slots:
    virtual void languageChange();

private:
    QString m_strCurPW;
    QString m_strNewPW;
    QString m_strNewConfPW;

    virtual void init();
    void ApplyNamingConventions( QString strName, QString & strCurName, QLineEdit * qle );

};

#endif // CCHANGEPASSWD_H
