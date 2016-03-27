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
//  FILENAME: WfAttrib.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		Implementation of class CWFAttrib which manages the windows attributes 
//		and functions eligibilities.
//		Those parameters are retrieved from offline definition according to Group,
//		SubGroup, Role and Location.
//		
// 
//  COMMENTS
//
//  BUILD INFORMATION 
//
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 1  2009/08/31 dalleins
//
//
//
// 
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include "WfAttrib.h"
#include "WinHints.h"
#include "./CommonFiles/IniFile.h"
#include "AsdApp.h"
#include "SKMessageBox.h"
#include "DataMngr.h"
#if (defined Q_WS_X11)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#endif

//-GLOBAL DATA-----------------------------------------------------------------

enum WDWPROPPREDEF { PDF_OFFL=-1, PDF_FALSE=0, PDF_TRUE=1};
WDWPROPPREDEF glob_WPredefTable[nbWindow][nbWindowAttributes] = 
{
	//						WND_CLOSEABILITY WND_COLLAPSEABILITY WND_MOVEABILITY WND_RESIZEABILITY WND_MINMAX_SIZES
		
	/*MAIN_RDR*/			{PDF_FALSE,	PDF_OFFL,	PDF_OFFL,	PDF_OFFL,	PDF_OFFL},
    /*AUX_RDR*/				{PDF_TRUE,	PDF_OFFL,	PDF_TRUE,	PDF_OFFL,	PDF_OFFL},
    /*HGHT_FILTERS*/		{PDF_TRUE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL},
    /*SYS_INFO*/			{PDF_TRUE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL},
    /*BRGHT_CTRL*/			{PDF_TRUE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL},
    /*FIND_TRACK*/			{PDF_TRUE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL},
    /*TRK_CTRL*/			{PDF_FALSE,	PDF_FALSE,	PDF_OFFL,	PDF_FALSE,	PDF_OFFL},
    /*POPUP*/				{PDF_FALSE,	PDF_FALSE,	PDF_OFFL,	PDF_FALSE,	PDF_OFFL},
    /*CHANGE_PASSWD*/		{PDF_TRUE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL},
    /*CUSTOM_SET*/			{PDF_TRUE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL},
    /*SAVE_AS*/				{PDF_TRUE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL},
    /*SIGN_IN*/				{PDF_TRUE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL},
    /*MESSAGE_BOX*/			{PDF_FALSE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL},
    /*CSU_LIST*/			{PDF_TRUE,	PDF_FALSE,	PDF_TRUE,	PDF_FALSE,	PDF_OFFL}

};

const QString COMMONATTRIB="Common";

                       
//-LOCAL DATA------------------------------------------------------------------

//-PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CWFAttribElem
/// This class is used to set the attributes for each ASD windows
// 
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: CIAsdFctRwsAux015 (glob_WPredefTable table)
//-----------------------------------------------------------------------------
CWFAttribElem::CWFAttribElem()
{
	for(int k=0;k<nbWindowAttributes;k++)
	{
		for (int l=0;l<nbWindow;l++)
		{
			m_IsastrWindowAttributes[l][k]=false;
		}
	}
	m_IsabRBToGo = false;
	m_IsIsAPPPos = false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWFAttribElem::~CWFAttribElem()
{
}

//-----------------------------------------------------------------------------
//
/// \class CWFAttrib
/// This class is used to call the functions which sets the attributes for
///	ASD windows
// 
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWFAttrib::CWFAttrib(const QString & strDataSet, bool bSetMinMaxSize):m_qvvWindowName(nbWindow),m_qvvFunctionName(nbFunction),m_qvvWindowAttributes(nbWindowAttributes)
{
  m_bDataSetOk=true;
  m_strDataSet=strDataSet;
  m_strVersion=strDataSet.section('-',-1,-1,QString::SectionSkipEmpty);
  m_strFileGeneral=strDataSet+"/"+m_strVersion+".ini";
  m_bSetMinMaxSize=bSetMinMaxSize;
  InitDataSet();
  InitAttributes();
  InitWindows();
  InitFunctions();
  ReadEligibilities();
  ReadAttributes();
  if (!m_bDataSetOk)
    exit(0);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the constructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWFAttrib::CWFAttrib()
{

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWFAttrib::~CWFAttrib()
{
	{
		QDictIterator<CWFAttribElem> iter(m_MapOfWFAttrib);
		CWFAttribElem* pelem = iter.current();
		while (pelem != NULL)
		{
			++iter;
			delete pelem;
			pelem = iter.current();
		}
	}
	{
		QDictIterator<CWFEligElem> iter(m_MapOfWFElig);
		CWFEligElem* pelem = iter.current();
		while (pelem != NULL)
		{
			++iter;
			delete pelem;
			pelem = iter.current();
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the window attribute names
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWFAttrib::InitAttributes()
{
  m_qvvWindowAttributes[WND_CLOSEABILITY]="WND_CLOSEABILITY";
  m_qvvWindowAttributes[WND_COLLAPSEABILITY]="WND_COLLAPSEABILITY";
  m_qvvWindowAttributes[WND_MOVEABILITY]="WND_MOVEABILITY";
  m_qvvWindowAttributes[WND_RESIZEABILITY]="WND_RESIZEABILITY";
  m_qvvWindowAttributes[WND_MINMAX_SIZES]="WND_MINMAX_SIZES";
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the groups names, the role, the location
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWFAttrib::InitDataSet()
{
	CIniFile file(CAsdApp::GetApp()->GetApplicationIni());
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the window names
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWFAttrib::InitWindows()
{
  m_qvvWindowName[MAIN_RDR]="MAIN_RDR";
  m_qvvWindowName[AUX_RDR]="AUX_RDR";
  m_qvvWindowName[HGHT_FILTERS]="HGHT_FILTERS";
  m_qvvWindowName[SYS_INFO]="SYS_INFO";
  m_qvvWindowName[BRGHT_CTRL]="BRGHT_CTRL";
  m_qvvWindowName[FIND_TRACK]="FIND_TRACK";
  m_qvvWindowName[TRK_CTRL]="TRK_CTRL";
  m_qvvWindowName[POPUP]="POPUP";
  m_qvvWindowName[CHANGE_PASSWD]="CHANGE_PASSWD";
  m_qvvWindowName[CUSTOM_SET]="CUSTOM_SET";
  m_qvvWindowName[SAVE_AS]="SAVE_AS";
  m_qvvWindowName[SIGN_IN]="SIGN_IN";
  m_qvvWindowName[MESSAGE_BOX]="MESSAGE_BOX";
  m_qvvWindowName[CSU_LIST]="CSU_LIST";
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the function names
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctEll010
//-----------------------------------------------------------------------------
void CWFAttrib::InitFunctions()
{
  m_qvvFunctionName[Assume_De_assume]="Assume/De-assume";
  m_qvvFunctionName[Tag_Track]="Tag_Track";
  m_qvvFunctionName[Cfl_Selection]="Cfl_Selection";
  m_qvvFunctionName[Rep_Selection]="Rep_Selection";
  m_qvvFunctionName[Ack_Emergency_SSR_Code]="Ack_Emergency_SSR_Code";
  m_qvvFunctionName[Wtc_Selection]="Wtc_Selection";
  m_qvvFunctionName[ATyp_Selection]="ATyp_Selection";
  m_qvvFunctionName[Ades_Selection]="Ades_Selection";
  m_qvvFunctionName[Assr_Selection]="Assr_Selection";
  m_qvvFunctionName[Rwy_Selection]="Rwy_Selection";
  m_qvvFunctionName[Solve_CsCsTagMismatch]="Solve_CsCsTagMismatch";
  m_qvvFunctionName[Ack_Apw_Msaw]="Ack_Apw_Msaw";
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method reads the ini file containing the function eligibilities.
/// 
///  RETURNS: 
///		true if the function eligibility has been found for the group, 
///		subgroup, location, role otherwise return false
///
///  REQUIREMENT ID: CIAsdFctEll010
//-----------------------------------------------------------------------------
bool CWFAttrib::ReadOneEligibilitie(QString Group, QString SubGroup, QString Location, QString Role, QString& Error)
{
	Error="";
	bool cr = true;
	QString strFileName=FileFrom(Group, SubGroup, Location, Role);
#ifdef ENABLE_QDEBUG
	qDebug("reading file %s",strFileName.ascii() );
#endif
	CIniFile iniFctEligible(strFileName);
	// Checks if the group, subgroup, location, role exist
	if (iniFctEligible.exists())
	{
		CArray<QString,QString> ReturnedStringArray;
		iniFctEligible.GetIniProfileSection("FCT_ELIGIBILITIES", ReturnedStringArray);
		//Checks if the section FCT_ELIGIBILITIES for the group, subgroup, location, role exist
		if (ReturnedStringArray.GetSize() > 0)
		{
			CWFEligElem* pElig = CreateEligElem(Group, SubGroup, Location, Role);
			for (int k=0;k<nbFunction;k++) 
			{
				QString strAvailable=QString::null;
				int iKeyRead=0;
				iKeyRead=iniFctEligible.GetIniProfileString("FCT_ELIGIBILITIES",m_qvvFunctionName[k],strAvailable);
				// checks if the function elligibility exist for the group subgroup, location, role
				if (iKeyRead != 0 && !strAvailable.isEmpty())
					pElig->m_aiFunctionEligibility[k]=((strAvailable=="y")||(strAvailable=="Y")||(strAvailable=="1"));
				else
				{
					cr = false;
					m_bDataSetOk=false;
					pElig->m_aiFunctionEligibility[k]=true;
					QString strMsg;
					strMsg.sprintf("Can't find %s function eligibility for Group %s SubGroup %s Location %s Role %s.", m_qvvFunctionName[k].ascii(), Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
					CAsdApp::GetApp()->WriteLogMsg(strMsg,LogError);
				}
			}
		}
		else
		{
			QString strMsg;
			strMsg.sprintf("Can't find section %s for Group %s SubGroup %s Location %s Role %s.", "FCT_ELIGIBILITIES", Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
			Error += strMsg;
			cr = false;
		}
	}
	else
	{
		QString strMsg;
		strMsg.sprintf("Can't find file for Group %s SubGroup %s Location %s Role %s.", Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
		Error += strMsg;
		cr = false;
	}
	return cr;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the function elligibility section exists in the 
///		ini file for the group, subgroup, location, role and
///		if not it checks for the group, subgroup, location, COMMON and so on
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: CIAsdFctEll010
//-----------------------------------------------------------------------------
void CWFAttrib::ReadEligibilities()
{
	QString Error="";
	if (!ReadOneEligibilitie(COMMONATTRIB, COMMONATTRIB, COMMONATTRIB, COMMONATTRIB, Error))
	{
		m_bDataSetOk=false;
		CAsdApp::GetApp()->WriteLogMsg(Error, LogError);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks for the availability of function elligibility 
///		in the ini file for the group, subgroup, location, role
/// 
///  RETURNS: 
///		true if the function eligibility is available for the group, 
///		subgroup, location, role otherwise return false
///
///  REQUIREMENT ID: CIAsdFctRwsRsv006
//-----------------------------------------------------------------------------
bool CWFAttrib::ReadOneAttribute(QString Group, QString SubGroup, QString Location, QString Role, QString& Error)
{
	Error = "";
	bool cr = true;
	QString strFileName=FileFrom(Group, SubGroup, Location, Role);
	CIniFile iniWndAttrib(strFileName);
	if (iniWndAttrib.exists())
	{
		CWFAttribElem* pAttrib = CreateAttribElem(Group, SubGroup, Location, Role);
		for(int k=0;k<nbWindowAttributes;k++)
		{
			QString strSection=m_qvvWindowAttributes[k];
			CArray<QString,QString> ReturnedStringArray;
			iniWndAttrib.GetIniProfileSection(strSection, ReturnedStringArray);
			if (ReturnedStringArray.GetSize() > 0)
			{
				for (int l=0;l<nbWindow;l++)
				{
					QString strAvailable=QString::null;
					int iKeyRead=0;
					// Read the availability for the attributes in the section
					// for the group, subgroup, location, role
					iKeyRead=iniWndAttrib.GetIniProfileString(strSection,m_qvvWindowName[l],strAvailable);
					if ((iKeyRead!=0)&&(!strAvailable.isEmpty()))
					{
						pAttrib->m_astrWindowAttributes[l][k]=strAvailable;
						pAttrib->m_IsastrWindowAttributes[l][k]=true;
					}
					else if (k!=WND_MINMAX_SIZES || (l== MAIN_RDR || l== AUX_RDR))
					{
						cr = false;
						m_bDataSetOk=false;
						QString strMsg;
						strMsg.sprintf("Can't find %s attribute for the window %s for Group %s SubGroup %s Location %s Role %s.", strSection.ascii(), m_qvvWindowName[l].ascii(), Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
						CAsdApp::GetApp()->WriteLogMsg(strMsg,LogError);
					}
				}
			}
			else
			{
				QString strMsg;
				strMsg.sprintf("Can't find section %s for Group %s SubGroup %s Location %s Role %s.", strSection.ascii(), Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
				Error += strMsg;
				cr = false;
			}
		}
		//Read time to go for range and bearing.
	{
		CArray<QString,QString> ReturnedStringArray;
		QString strSection = "RANGE_BEARING";
		QString strProperty = "DISPLAY_TIME_TO_GO";
		iniWndAttrib.GetIniProfileSection(strSection.ascii(), ReturnedStringArray);
		if (ReturnedStringArray.GetSize() > 0)
		{
			QString strRBToGo;
			int iKeyRead=iniWndAttrib.GetIniProfileString(strSection.ascii(), strProperty.ascii(), strRBToGo);
			if (iKeyRead)
			{
				pAttrib->m_abRBToGo=((strRBToGo=="Y")||(strRBToGo=="y")||(strRBToGo=="yes")||(strRBToGo=="YES")||(strRBToGo=="1"));
				pAttrib->m_IsabRBToGo=true;
			}
			else
			{
				cr = false;
				m_bDataSetOk=false;
				QString strMsg;
				strMsg.sprintf("Can't find %s - %s for Group %s SubGroup %s Location %s Role %s.", strSection.ascii(), strProperty.ascii(), Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
				CAsdApp::GetApp()->WriteLogMsg(strMsg,LogError);
			}
		}
		else
		{
			QString strMsg;
			strMsg.sprintf("Can't find section %s for Group %s SubGroup %s Location %s Role %s.", strSection.ascii(), Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
			Error += strMsg;
			cr = false;
		}
	}

	{
		CArray<QString,QString> ReturnedStringArray;
		QString strSection = "FULL_DB_TYPE";
		QString strProperty = "DB_TYPE";
		// read the data type for the full datablock
		iniWndAttrib.GetIniProfileSection(strSection.ascii(), ReturnedStringArray);
		if (ReturnedStringArray.GetSize() > 0)
		{
			QString strAPPPos;
			int iKeyRead=iniWndAttrib.GetIniProfileString("FULL_DB_TYPE","DB_TYPE",strAPPPos);
			if (iKeyRead)
			{
				pAttrib->m_IsAPPPos=(strAPPPos=="APP");
				pAttrib->m_IsIsAPPPos=true;
			}
			else
			{
				cr = false;
				m_bDataSetOk=false;
				QString strMsg;
				strMsg.sprintf("Can't find %s - %s for Group %s SubGroup %s Location %s Role %s.", strSection.ascii(), strProperty.ascii(), Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
				CAsdApp::GetApp()->WriteLogMsg(strMsg,LogError);
			}
		}
		else
		{
			QString strMsg;
			strMsg.sprintf("Can't find section %s for Group %s SubGroup %s Location %s Role %s.", strSection.ascii(), Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
			Error += strMsg;
			cr = false;
		}
	}

	}
	else
	{
		QString strMsg;
		strMsg.sprintf("Can't find file for Group %s SubGroup %s Location %s Role %s.", Group.ascii(), SubGroup.ascii(), Location.ascii(), Role.ascii());
		Error += strMsg;
		cr = false;
	}
	return cr;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the elligibility section exists in the ini file 
///		for the group, subgroup, location, role and
///		if not it checks for the group, subgroup, location, COMMON and so on
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CWFAttrib::ReadAttributes()
{
	QString Error;

	if (!ReadOneAttribute(COMMONATTRIB, COMMONATTRIB, COMMONATTRIB, COMMONATTRIB, Error))
	{
		m_bDataSetOk=false;
		CAsdApp::GetApp()->WriteLogMsg(Error, LogError);
	}

	CIniFile iniWndAttribGeneral(m_strFileGeneral);
	if (!iniWndAttribGeneral.exists())
	{
		m_bDataSetOk=false;
		QString strMsg;
		strMsg.sprintf("Can't find file %s.",m_strFileGeneral.ascii());
		CAsdApp::GetApp()->WriteLogMsg(strMsg,LogError);
	}
	else
	{
		// Look for windows priority in the <version>.ini file
		QString strSection = "WND_PRIORITIES";
		for (int i=0;i<nbWindow;i++)
		{
			QString strAvailable=QString::null;
			int iKeyRead=0;
			iKeyRead=iniWndAttribGeneral.GetIniProfileString(strSection.ascii(),m_qvvWindowName[i],strAvailable);
			if (iKeyRead!=0||!strAvailable.isEmpty())
				m_aiWindowGeneralAttrib[i][WND_PRIORITIES]=strAvailable.toInt();
			else
			{
				m_bDataSetOk=false;
				QString strMsg;
				strMsg.sprintf("Can't find %s - %s in %s file.", strSection.ascii(), m_qvvWindowName[i].ascii(), m_strFileGeneral.ascii());
				CAsdApp::GetApp()->WriteLogMsg(strMsg,LogError);
			}
		}
    }
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the Group, SubGroup, Location, Role ini file
/// 
///  RETURNS: 
///		the GroupSubgroupLocationRole.ini file
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
QString CWFAttrib::FileFrom(QString Group, QString SubGroup, QString Location, QString Role)
{
  QString strFile=m_strDataSet + GrpSbgrpLocRolDepDir;
  strFile+=Group;
  strFile+=SubGroup;
  strFile+=Location;
  strFile+=Role;
  strFile+=".ini";
  return strFile;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method looks for window elligibilities according to the group, subgroup,
///		subgroup, location, role
/// 
///  RETURNS: 
///		true to match with specifications.
///	    For now, any window is predefined to be available whatever the Group/SubGroup/Location/Role is. 
///	    it is intentionally left empty
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWFAttrib::WindowEligibility(int ewName, QString Group, QString SubGroup, QString Location, QString Role)
{
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method looks for function elligibility for the group, subgroup,
///		subgroup, location, role
/// 
///  RETURNS: 
///		true if the efName function is available for erRole and emSysMode
///		returns false in the other case. 
///		The returned value is unknown if one of the three parameters is out
///		of range.
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWFAttrib::FunctionEligibility(int efName, QString Group, QString SubGroup, QString Location, QString Role)
{
	bool cr = false;
  	CWFEligElem* pElig = GetEligElem(Group, SubGroup, Location, Role);
	if (pElig)
		cr = pElig->m_aiFunctionEligibility[efName];
	return cr;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method looks if the window has boolean pre-defined attributes. If so, it 
///		takes it into account. If not, it looks for the offline defined attributes.
/// 
///  RETURNS: 
///		true if the attributes is available
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWFAttrib::ReadBoolAttribute(int ewName,int iAttrib,QString Group, QString SubGroup, QString Location, QString Role)
{

	if (glob_WPredefTable[ewName][iAttrib]!=PDF_OFFL)
		return glob_WPredefTable[ewName][iAttrib];
	else
	{
		QString strAttrib;
		for(int n = 0; n <=4 ; n++)
		{
  			CWFAttribElem* pAttrib = GetAttribElem(Group, SubGroup, Location, Role, n);
			if (pAttrib && pAttrib->m_IsastrWindowAttributes[ewName][iAttrib])
			{
				strAttrib = pAttrib->m_astrWindowAttributes[ewName][iAttrib];
				break;
			}
		}
		return ((strAttrib=="Y")||(strAttrib=="y")||(strAttrib=="yes")||(strAttrib=="YES")||(strAttrib=="1"));
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method reads the value of the attributes iAttrib given as parameter
/// 
///  RETURNS: 
///		the value of the attributes
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
int CWFAttrib::ReadOneIntAttribute(int ewName,int iAttrib, QString Group, QString SubGroup, QString Location, QString Role, int iPos)
{
	QString strAttrib;
	for(int n = 0; n <=4 ; n++)
	{
  		CWFAttribElem* pAttrib = GetAttribElem(Group, SubGroup, Location, Role, n);
		if (pAttrib && pAttrib->m_IsastrWindowAttributes[ewName][iAttrib])
		{
			strAttrib = pAttrib->m_astrWindowAttributes[ewName][iAttrib];
			break;
		}
	}
	return strAttrib.section('|',iPos,iPos).toInt();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks the window (w) has a title bar
/// 
///  RETURNS: 
///		true if the window has a title bar otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWFAttrib::HasWindowTitle(QWidget* w,int ewName, QString Group, QString SubGroup, QString Location, QString Role)
{
	bool HasTitle=false;
	if (ReadBoolAttribute(ewName,WND_MOVEABILITY, Group, SubGroup, Location, Role))
		HasTitle=true;

	return HasTitle;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the minimum and maximum size of the radar window when 
///		it is collapsible
/// 
///  RETURNS: 
///		true by default
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWFAttrib::SetMinMaxAttributes(QWidget* w,int ewName, QString Group, QString SubGroup, QString Location, QString Role, bool FromCustomSet)
{
  QDesktopWidget* pDesk=CAsdApp::GetApp()->desktop();
  QRect rect=pDesk->screenGeometry(pDesk->primaryScreen());

  int iMinHSize=ReadOneIntAttribute(ewName,WND_MINMAX_SIZES, Group, SubGroup, Location, Role, MIN_H);
  int iMaxHSize=ReadOneIntAttribute(ewName,WND_MINMAX_SIZES, Group, SubGroup, Location, Role, MAX_H);
  int iMinVSize=ReadOneIntAttribute(ewName,WND_MINMAX_SIZES, Group, SubGroup, Location, Role, MIN_V);
  int iMaxVSize=ReadOneIntAttribute(ewName,WND_MINMAX_SIZES, Group, SubGroup, Location, Role, MAX_V);

  if (iMaxHSize > rect.width()-8)
	iMaxHSize=rect.width()-8;
  if (iMaxVSize > rect.height()-8)
  {
        iMaxVSize=rect.height()-8;
	if (ewName==MAIN_RDR)
  	{
		if ((ReadBoolAttribute(ewName,WND_COLLAPSEABILITY, Group, SubGroup, Location, Role)))
        		iMaxVSize=iMaxVSize-CDataMngr::GetTitleOffset();
	}
  }	


  if (m_bSetMinMaxSize&&(iMinHSize!=0)&&(iMinVSize>0))
    w->setMinimumSize(iMinHSize,iMinVSize);

  if (m_bSetMinMaxSize&&(iMaxHSize!=0)&&(iMaxVSize>0))
  {
    w->setMaximumSize(iMaxHSize,iMaxVSize);
    QRect rect = QRect(w->x(),w->y(),iMaxHSize,iMaxVSize);
    w->setGeometry(rect);
  }

  return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the collapsibility, resizeability, moveability of a radar
///		window
/// 
///  RETURNS: 
///		true by default
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWFAttrib::WindowAttributes(QString Window,int ewName, QString Group, QString SubGroup, QString Location, QString Role, bool Forced)
{
  long functions=0x00000000;
  long decorations=0x00000000;
  CWin_Hints* pCWinHints=CAsdApp::GetApp()->GetWinHints();
                                                                                
  decorations|=CWin_Hints::MWM_DECOR_BORDER;
                                                                                
  if (ReadBoolAttribute(ewName,WND_CLOSEABILITY, Group, SubGroup, Location, Role))
    functions|=CWin_Hints::MWM_FUNC_CLOSE;
                                                                                
  if (ReadBoolAttribute(ewName,WND_COLLAPSEABILITY, Group, SubGroup, Location, Role))
    {
      functions|=CWin_Hints::MWM_FUNC_MINIMIZE;
      decorations|=CWin_Hints::MWM_DECOR_MINIMIZE;
    }
                                                                                
  if (ReadBoolAttribute(ewName,WND_MOVEABILITY, Group, SubGroup, Location, Role))
    {
      functions|=CWin_Hints::MWM_FUNC_MOVE;
      decorations|=CWin_Hints::MWM_DECOR_TITLE;
                                                                                
    }
                                                                                
  if (ReadBoolAttribute(ewName,WND_RESIZEABILITY, Group, SubGroup, Location, Role))
    {
      functions|=CWin_Hints::MWM_FUNC_RESIZE;
      decorations|=CWin_Hints::MWM_DECOR_RESIZEH;
    }
  //pCWinHints->ChangeWindow(Window,functions,decorations, Forced);
  return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks for the range and bearing time to go window
/// 
///  RETURNS: 
///		true if the time to go of the range and bearing is available
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWFAttrib::RBToGo(QString Group, QString SubGroup, QString Location, QString Role)
{
	bool cr = false;
	for(int n = 0; n <=4 ; n++)
	{
  		CWFAttribElem* pAttrib = GetAttribElem(Group, SubGroup, Location, Role, n);
		if (pAttrib && pAttrib->m_IsabRBToGo)
		{
			cr = pAttrib->m_abRBToGo;
			break;
		}
	}
	return cr;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the position is an Approach position
/// 
///  RETURNS: 
///		true if it is an approach position otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWFAttrib::IsAPPPos(QString Group, QString SubGroup, QString Location, QString Role)
{
	bool cr = false;
	for(int n = 0; n <=4 ; n++)
	{
  		CWFAttribElem* pAttrib = GetAttribElem(Group, SubGroup, Location, Role, n);
		if (pAttrib && pAttrib->m_IsIsAPPPos)
		{
			cr = pAttrib->m_IsAPPPos;
			break;
		}
	}
	return cr;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method checks if the main radar window is collapsable
/// 
///  RETURNS: 
///		true if the main is collapsable otherwise false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
bool CWFAttrib::IsMainCollapsible(QString Group, QString SubGroup, QString Location, QString Role)
{
	bool cr = false;
	cr =ReadBoolAttribute(CWFAttrib::MAIN_RDR,CWFAttrib::WND_COLLAPSEABILITY,Group, SubGroup, Location, Role);
	return cr;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves a pointer of type CWFAttribElem in order to have
///		all the attributes for windows and functions ellegibilities depending on 
///		Group, SubGroup, Location, Role.
///		If the Role, Location, SubGroup and Group does not exist, it takes the
///		COMMON section
/// 
///  RETURNS: 
///		pointer of type CWFAttribElem
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWFAttribElem* CWFAttrib::GetAttribElem(QString Group, QString SubGroup, QString Location, QString Role, int nbParam)
{
	QString key;
	switch (nbParam)
	{
	case 0 :
		key = BuilMapKey(Group, SubGroup, Location, Role);
		break;
	case 1 :
		key = BuilMapKey(Group, SubGroup, Location, COMMONATTRIB);
		break;
	case 2 :
		key = BuilMapKey(Group, SubGroup, COMMONATTRIB, COMMONATTRIB);
		break;
	case 3 :
		key = BuilMapKey(Group, COMMONATTRIB, COMMONATTRIB, COMMONATTRIB);
		break;
	case 4 :
		key = BuilMapKey(COMMONATTRIB, COMMONATTRIB, COMMONATTRIB, COMMONATTRIB);
		break;
	default :
		break;
	}
	CWFAttribElem* pAttrib = m_MapOfWFAttrib.find(key);
	return pAttrib;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves a pointer of type CWFAttribElem in order to have the 
///		windows and functions elligibilities
///		If the Role, Location, SubGroup and Group does not exist, it takes the
///		COMMON section
/// 
///  RETURNS: 
///		pointer of type CWFAttribElem
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWFEligElem* CWFAttrib::GetEligElem(QString Group, QString SubGroup, QString Location, QString Role)
{
	QString key = BuilMapKey(Group, SubGroup, Location, Role);
	CWFEligElem* pElig = m_MapOfWFElig.find(key);
	if (!pElig)
	{
		key = BuilMapKey(Group, SubGroup, Location, COMMONATTRIB);
		pElig = m_MapOfWFElig.find(key);
		if (!pElig)
		{
			key = BuilMapKey(Group, SubGroup, COMMONATTRIB, COMMONATTRIB);
			pElig = m_MapOfWFElig.find(key);
			if (!pElig)
			{
				key = BuilMapKey(Group, COMMONATTRIB, COMMONATTRIB, COMMONATTRIB);
				pElig = m_MapOfWFElig.find(key);
				if (!pElig)
				{
					key = BuilMapKey(COMMONATTRIB, COMMONATTRIB, COMMONATTRIB, COMMONATTRIB);
					pElig = m_MapOfWFElig.find(key);
				}
			}
		}
	}
	return pElig;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method inserts the window or function elligibility if not yet present 
///		in the m_mapOfWFElig table
/// 
///  RETURNS: 
///		pointer of type CWFAttribElem
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWFEligElem* CWFAttrib::CreateEligElem(QString Group, QString SubGroup, QString Location, QString Role)
{
	CWFEligElem* pElig = GetEligElem(Group, SubGroup, Location, Role);
	if (!pElig)
	{
		QString key = BuilMapKey(Group, SubGroup, Location, Role);
		pElig = new CWFEligElem;
		m_MapOfWFElig.insert(key, pElig);
	}
	return pElig;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method inserts attributes for the window or function elligibility 
///		if not yet present in the m_mapOfWFAttrib table
/// 
///  RETURNS: 
///		pointer of type CWFAttribElem
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CWFAttribElem* CWFAttrib::CreateAttribElem(QString Group, QString SubGroup, QString Location, QString Role)
{
	CWFAttribElem* pAttrib = GetAttribElem(Group, SubGroup, Location, Role, 0);
	if (!pAttrib)
	{
		QString key = BuilMapKey(Group, SubGroup, Location, Role);
		pAttrib = new CWFAttribElem;
		m_MapOfWFAttrib.insert(key, pAttrib);
	}
	return pAttrib;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method creates the right Group, Subgroup, Location and Role name 
///		in order to open the right offline file
/// 
///  RETURNS: 
///		the name of the ini file 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
QString CWFAttrib::BuilMapKey(QString Group, QString SubGroup, QString Location, QString Role)
{
	QString ret;
	ret = Group+SubGroup+Location+Role;
	return ret;
}

//--------------------------------< THE END >----------------------------------

