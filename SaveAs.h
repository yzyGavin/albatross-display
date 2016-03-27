/****************************************************************************
** Form interface generated from reading ui file 'SaveAs.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CSSAVEAS_H
#define CSSAVEAS_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLineEdit;

class CSSaveAs : public QDialog
{
    Q_OBJECT

public:
    CSSaveAs( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CSSaveAs();

    QPushButton* pbOk;
    QLineEdit* leName;

    virtual void mousePressEvent( QMouseEvent * e );

public slots:
    virtual void LeCheckText( const QString & strText );

protected:

protected slots:
    virtual void languageChange();

private:
    QString m_strName;

    void keyPressEvent( QKeyEvent * e );

};

#endif // CSSAVEAS_H
