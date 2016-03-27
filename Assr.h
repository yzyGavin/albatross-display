/****************************************************************************
** Form interface generated from reading ui file 'Assr.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CASSR_H
#define CASSR_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;

class CAssr : public QDialog
{
    Q_OBJECT

public:
    CAssr( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CAssr();

    QLabel* Cls;
    QPushButton* pushButton1;
    QPushButton* pushButton4;
    QPushButton* pushButton7;
    QPushButton* pushButton5;
    QPushButton* pushButton2;
    QPushButton* pushButton6;
    QPushButton* pushButton3;
    QPushButton* pushButton0;
    QPushButton* pushButton9;
    QPushButton* pushButton8;
    QLineEdit* Assr;

    QString m_Assr;

    virtual void ComputeChar( char btn );
    virtual void Init();

public slots:
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void timerEvent( QTimerEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );
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
    virtual void AssrTextChanged( const QString & text );

protected:
    int m_TimerId;


protected slots:
    virtual void languageChange();

};

#endif // CASSR_H
