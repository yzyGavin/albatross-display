/*
 * Copyright (C) 2009 SkySoft-ATM 
 *		ROUTE DE PRE-BOIS 15-17
 *		CH-1215 GENEVA
 *		SWITZERLAND
 * 		tel int 00 41 22 417 4770
 *  
 * 		for any Albatross related question : <info@albatross.aero>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * The Court of competent jurisdiction is Geneva Switzerland 
*/
//-----------------------------------------------------------------------------
// 
//  SYSTEM  : Albatross
//  PROCESS : Display
//  FILENAME: CustomSetDialog.ui.h
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the interactions of the customization set window
// 
//  COMMENTS
// 
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 1  2009/08/31 dalleins
//
//
// 
//-----------------------------------------------------------------------------
/****************************************************************************
 ** ui.h extension file, included from the uic-generated form implementation.
 **
 ** If you want to add, delete, or rename functions or slots, use
 ** Qt Designer to update this file, preserving your code.
 **
 ** You should not define a constructor or destructor in this file.
 ** Instead, write your code in functions called init() and destroy().
 ** These will automatically be called by the form's constructor and
 ** destructor.
 *****************************************************************************/

//-INCLUDE FILES---------------------------------------------------------------
#include "WdwMngr.h"
#include "WinHints.h"
#include "UserMngr.h"
#include "TopLevel.h"

//-PROTOTYPES---------------------------------------------------------
//-----------------------------------------------------------------------------
//
/// \class CustomizationSetDialog
/// This class is used to create the Custom set dialog window and sets the user
///	interactions linked with it
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called after the constructor to initialize the 
///		customization set window
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CustomizationSetDialog::init()
{
	m_SaveAsDialog=NULL;
	m_bSaveAsDisplayed=false;
	m_strDirectory=CAsdApp::GetApp()->getCustomDir();
	m_strUserName=CAsdApp::GetApp()->GetUserMngr()->GetUserName();
	m_iNbMaxCustomSet=30;
	m_pqlbiCSelect=NULL;
	m_pqlbiPreviousSelected=NULL;
	m_pqlbiPressed=NULL;
	m_pqlbiNotPressed=NULL;
	m_bSelectChanged=false;
	m_bButtonPressed=false;
	m_piniCustomSets=NULL;
	//Read the list of custom set of the user file.
	lbCustomSets->clear();
	m_bOneSelected=false;
	LoadCustomSet();
	
	lbCustomSets->verticalScrollBar()->setLineStep(19);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called to destroy the customization set window
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CustomizationSetDialog::destroy()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called to delete a Custom set when the 
///		user click AB on Delete button of the customization set window
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::DeleteCustomSet(QListBoxItem* pqlbiToDelete)
{
        CIniFile iniCustomSets;
        QString FileName = m_strDirectory+"/"+m_strUserName;
        QFile userfile(FileName);
                                                                                
        if (iniCustomSets.Open(FileName))
        {
                if (pqlbiToDelete!=NULL)
                {
                        //Remove from file.
                        m_strUserName=CAsdApp::GetApp()->GetUserMngr()->GetUserName();
                                                                                
                        bool isopen = userfile.open(IO_WriteOnly);
                        if (!isopen)
                        {
                                CSKMessageBox::critical( 0, "Error Message","An error occured. No permission to write in Customization set file.");
                                CWdwMngr::HideWindow(this);
                                return ;
                        }
                        if(!iniCustomSets.DeleteIniProfileSection(pqlbiToDelete->text()))
                        {// The line is no present
                                CSKMessageBox::critical( 0, "Error Message","An error occured. This customization set could not be deleted.");
                                CWdwMngr::HideWindow(this);
                                return;
                        }
                                                                                
                        iniCustomSets.Close();
                        //Synchro avec ODD. (envoie de messages).
                        lbCustomSets->removeItem(lbCustomSets->index(pqlbiToDelete));
                }
                m_iCustomCount--;
                m_bOneSelected=false;
                ComputeEligibility();
        }
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method adds a custom set in the username file
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::AddCustomSet(const QString & strItem)
{
  ComputeEligibility();

  if(CSetupMngr::Setup(m_strDirectory+"/"+m_strUserName,strItem,true))
  {
	QListBoxItem* pqlbiExisting=lbCustomSets->findItem(strItem,Qt::ExactMatch|Qt::CaseSensitive);
	if (pqlbiExisting!=NULL)
		DeleteCustomSet(pqlbiExisting);
	  lbCustomSets->insertItem(strItem);
	  m_iCustomCount++;
	  lbCustomSets->sort();

  }
  else
	  CWdwMngr::HideWindow(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method loads the list of custom sets available for
///		the username and displayed it on the custom set window
/// 
///  RETURNS: 
///		true by default
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CustomizationSetDialog::LoadCustomSet()
{
	lbCustomSets->clear();
	m_iCustomCount=0;
	m_strUserName=CAsdApp::GetApp()->GetUserMngr()->GetUserName();
  
	// Check the presence of the file
	QFile userfile(m_strDirectory+"/"+m_strUserName);
	if (!userfile.open(IO_ReadOnly))
	{
		CSKMessageBox::critical( 0, "Error Message","An error occured. This customization set could not be read.");
		return false;
	}
	else
		userfile.close();

	// retrieves the list of custom sets available for the username
	CIniFile iniCustomSets(m_strDirectory+"/"+m_strUserName);
	QString strSections;
	QChar cSep('\0');
	iniCustomSets.GetIniProfileSectionNames(strSections);
			
	// CDS BREAK JPM 2006/05/10 Use of ptr->fld access cannot be used in this 
	//							case the iterator is of type QStringList 
	QStringList qslSections(QStringList::split(cSep,strSections));
	for ( QStringList::Iterator it = qslSections.begin(); it != qslSections.end(); ++it ) 
	{
		if ((*it).length()<11)
		{
			m_iCustomCount++;
			lbCustomSets->insertItem(*it);
		}
	}
	qDebug("Custom count0:%d",m_iCustomCount);
	lbCustomSets->sort();
	lbCustomSets->clearSelection();
	m_bOneSelected=false;
	ComputeEligibility();

	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method enables the button in the custom set window
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctCzs050, CIAsdFctCzs052, CIAsdFctCzs060 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::ComputeEligibility()
{

  if (m_bSaveAsDisplayed)
  {
	pbSaveAs->setEnabled(false);
	pbSave->setEnabled(false);
	pbDelete->setEnabled(false);
	pbRecall->setEnabled(false);
  }
  else
  {
  QString strCount;
  if (m_iCustomCount==m_iNbMaxCustomSet)
    {
      strCount+="<font color=";
      strCount+=(CColorMngr::GetColor(CColorMngr::GetColorNb("WARNING"))).name();
      strCount+=">";
      strCount+=QString::number(m_iCustomCount)+"/"+QString::number(m_iNbMaxCustomSet);
    }
  else
    strCount+=QString::number(m_iCustomCount)+"/"+QString::number(m_iNbMaxCustomSet);
  lCount->setText(strCount);

  	pbSaveAs->setEnabled(m_iCustomCount<m_iNbMaxCustomSet);
	pbSave->setEnabled(m_bOneSelected);
	pbDelete->setEnabled(m_bOneSelected);
  
	pbRecall->setEnabled(m_bOneSelected);
  }

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user click AB on Delete button
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctCzs085 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::ClickDelete()
{
  DeleteCustomSet(lbCustomSets->selectedItem());
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called when the user click AB on SaveAs button
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctCzs080 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::ClickSaveAs()
{

  m_SaveAsDialog=CTopLevel::m_psSaveAs;
 
  lbCustomSets->clearSelection();
  m_bOneSelected=false;
  m_bSaveAsDisplayed=true;
  ComputeEligibility();

  CTopLevel::m_psSaveAs->move(QCursor::pos());

  CTopLevel::m_psSaveAs->leName->setText("");
  CWdwMngr::ShowWindow(CTopLevel::m_psSaveAs,CWFAttrib::SAVE_AS);

  if (CTopLevel::m_psSaveAs->exec()==QDialog::Accepted)
  {
    AddCustomSet(CTopLevel::m_psSaveAs->leName->text());
  }
  m_bSaveAsDisplayed=false;
  ComputeEligibility();
  if (CTopLevel::m_psSaveAs)
  {
	m_SaveAsDialog=NULL;
  }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called each time a custom set is 
///		selected in the custom set window
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::SelectionChanged( QListBoxItem * )
{
  m_bOneSelected=true;
  ComputeEligibility(); 
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called each time the user clicks AB on Recall button
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctCzs065 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::ClickRecall()
{
  if(!CSetupMngr::Setup(m_strDirectory+"/"+m_strUserName,lbCustomSets->selectedItem()->text(),false))
  {
		CWdwMngr::HideWindow(this);
  }
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called each time the user clicks AB on Save button
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctCzs075 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::ClickSave()
{
  if(!CSetupMngr::Setup(m_strDirectory+"/"+m_strUserName,lbCustomSets->selectedItem()->text(),true))
  {
	  CWdwMngr::HideWindow(this);
  }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called each time the user clicks WB
///		anywhere on the custom set window
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctCzs125
//
//-----------------------------------------------------------------------------
void CustomizationSetDialog::mousePressEvent( QMouseEvent * e )
{
	if (e->button()==MidButton)
	{
		CWdwMngr::HideWindow(this);
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called to close the custom set window
///		At the same time the new default position is set for the window
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctCzs125
// 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::closeEvent( QCloseEvent * )
{
	CAsdApp::GetApp()->getDefTlmPosList()->SetDefaultTlmPos("CUSTOM_SET", this->x(), this->y(), CAsdApp::GetApp()->desktop()->screenNumber(this));
	CWdwMngr::HideWindow(this);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called every time the user press a key
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::keyPressEvent( QKeyEvent * e )
{
	switch (e->key())
	{
		case Key_Escape:
			return;
		default:
			break;
	}

	QDialog::keyPressEvent ( e );
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is called in case the save as window is open
///		whereas the custom set window is to be closed
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE 
//-----------------------------------------------------------------------------
void CustomizationSetDialog::CloseSaveAsWnd()
{
	if (m_SaveAsDialog)
	{
		CWdwMngr::HideWindow(m_SaveAsDialog);
		m_SaveAsDialog=NULL;
	}
}


//--------------------------------< THE END >----------------------------------
