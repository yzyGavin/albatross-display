/****************************************************************************
** Form interface generated from reading ui file 'FlSelectOther.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CFLSELECTOTHER_H
#define CFLSELECTOTHER_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QLineEdit;

class CFlSelectOther : public QDialog
{
    Q_OBJECT

public:
    CFlSelectOther( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CFlSelectOther();

    QPushButton* pushButton7;
    QPushButton* pushButton4;
    QPushButton* pushButton1;
    QPushButton* pushButton5;
    QPushButton* pushButton9;
    QPushButton* pushButton6;
    QPushButton* pushButton2;
    QPushButton* pushButton3;
    QPushButton* pushButton0;
    QPushButton* pushButton8;
    QLabel* Cls;
    QLineEdit* Level;

    int m_Level;
    bool ButtonPressed;

    virtual void ComputeChar( char btn );
    virtual void Init( int cfl );
    virtual void destroy();

public slots:
    virtual void PushButton0Pressed();
    virtual void PushButton1Pressed();
    virtual void PushButton2Pressed();
    virtual void PushButton3Pressed();
    virtual void PushButton4Pressed();
    virtual void PushButton5Pressed();
    virtual void PushButton6Pressed();
    virtual void PushButton7Pressed();
    virtual void PushButton8Pressed();
    virtual void PushButton9Pressed();
    virtual void LevelTextChanged( const QString & textIni );
    virtual void LevelReturnPressed();

protected:
    int m_TimerId;


protected slots:
    virtual void languageChange();

private:
    QString m_strLevel;

    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );

};

#endif // CFLSELECTOTHER_H
