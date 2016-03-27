/****************************************************************************
** Form interface generated from reading ui file 'SignIn.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CSIGNIN_H
#define CSIGNIN_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;

class CSignIn : public QDialog
{
    Q_OBJECT

public:
    CSignIn( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CSignIn();

    QLabel* textLabel2;
    QLabel* Error;
    QLabel* textLabel1;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QLineEdit* leUserName;
    QLineEdit* lePassword;

    void init();

public slots:
    virtual void UserNameChanged( const QString & strUserName );
    virtual void OkClicked();
    virtual void CancelClicked();
    virtual void PasswordChanged( const QString & strPassword );
    virtual void mousePressEvent( QMouseEvent * e );
    virtual void closeEvent( QCloseEvent * );

protected:

protected slots:
    virtual void languageChange();

private:
    QString m_strUser;
    QString m_strPW;

    void keyPressEvent( QKeyEvent * e );

};

#endif // CSIGNIN_H
