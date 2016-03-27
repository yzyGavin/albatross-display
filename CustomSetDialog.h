/****************************************************************************
** Form interface generated from reading ui file 'CustomSetDialog.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CUSTOMIZATIONSETDIALOG_H
#define CUSTOMIZATIONSETDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include "ColorMngr.h"
#include "qmessagebox.h"
#include "qstring.h"
#include "./CommonFiles/IniFile.h"
#include "StdAfx.h"
#include "qfile.h"
#include "qstringlist.h"
#include "SetupMngr.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QListBox;
class QListBoxItem;
class QLabel;

class CustomizationSetDialog : public QDialog
{
    Q_OBJECT

public:
    CustomizationSetDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CustomizationSetDialog();

    QPushButton* pbSave;
    QPushButton* pbSaveAs;
    QPushButton* pbRecall;
    QPushButton* pbDelete;
    QListBox* lbCustomSets;
    QLabel* lCount;

    virtual void mousePressEvent( QMouseEvent * e );

public slots:
    virtual void DeleteCustomSet( QListBoxItem * pqlbiToDelete );
    virtual void AddCustomSet( const QString & strItem );
    virtual bool LoadCustomSet();
    virtual void ComputeEligibility();
    virtual void ClickDelete();
    virtual void ClickSaveAs();
    virtual void SelectionChanged( QListBoxItem * );
    virtual void ClickRecall();
    virtual void ClickSave();
    virtual void closeEvent( QCloseEvent * );
    virtual void CloseSaveAsWnd();

protected:

protected slots:
    virtual void languageChange();

private:
    bool m_bSaveAsDisplayed;
    QListBoxItem* m_pqlbiNotPressed;
    QListBoxItem* m_pqlbiPressed;
    bool m_bButtonPressed;
    QListBoxItem* m_pqlbiCSelect;
    bool m_bSelectChanged;
    bool m_bOneSelected;
    int m_iCustomCount;
    QString m_strUserName;
    QString m_strDirectory;
    QListBoxItem* m_pqlbiPreviousSelected;
    int m_iNbMaxCustomSet;
    CIniFile* m_piniCustomSets;
    QDialog* m_SaveAsDialog;

    void init();
    void destroy();
    void keyPressEvent( QKeyEvent * e );

};

#endif // CUSTOMIZATIONSETDIALOG_H
