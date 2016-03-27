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
//  FILENAME: WfAttrib.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//	  Implementation of class CWFAttrib which manages the windows attributes 
//	  and functions eligibilities.
//	  Those parameters are retrieved from offline definition according to Group,
//	  SubGroup, Role and Location.
// 
//  COMMENTS
//    Adding a Mode:
//    Add the Mode in the file where the mode is computed.
//    Add the corresponding string values in WfAttrib.cpp InitModes.
//
//    Adding a Group:
//    in WfAttrib.h
//    Add the group in the enumeration list (enum Group)
//    Change the value of nbGroup
//    in WfAttrib.cpp
//    in the function InitGroups add the follwing line
//    m_qvvGroup[GroupName]="GroupName";
//
//    Adding a Role:
//    in WfAttrib.h
//    Add the role in the enumeration list (enum Role)
//    Change the value of nbRole
//    in WfAttrib.cpp
//    in the function InitRoles add the follwing line
//    m_qvvRole[RoleName]="RoleName";
//    
//    Adding a function eligibility:
//    in WfAttrib.h
//    Add the function in the enumeration list (FunctionNames)
//    Change the value of nbFunction
//    in WfAttrib.cpp
//    in the function InitFunctions add the following line
//    m_qvvFunctionName[FunctionName]="FunctionName";
//
//    Adding a window eligibility:
//    in WfAttrib.h
//    Add the role in the enumeration list (WindowNames)
//    Change the value of nbWindow
//    in WfAttrib.cpp
//    in the function InitWindows add the follwing line
//    m_qvvWindowName[WindowName]="WindowName";
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

#if !defined(WF_ATTRIB_H)
#define WF_ATTRIB_H

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include <qstring.h>
#include <qvaluevector.h>
#include <qwidget.h>
#include "AsdMenu.h"
#include <qdict.h>

//-GLOBAL DATA-----------------------------------------------------------------

enum NbFunctionWindow{
    nbWindow=14,
    nbFunction=12,
    nbWindowAttributes=5,
    nbWindowGeneralAttributes=1,
    nbMode=3,
};

//-CLASSES DEFINITIONS---------------------------------------------------------

class CWFEligElem
{
public :
	virtual ~CWFEligElem() {};
	int m_aiFunctionEligibility[nbFunction];
};

class CWFAttribElem
{
	friend class CWFAttrib;

public :

	// destructor of the CWFAttribElem class
	virtual ~CWFAttribElem();

private:

	// variables

	// table which stores the value of all attributes present in a window
	QString m_astrWindowAttributes[nbWindow][nbWindowAttributes];
	// set to true if an attribute of a window is available
	bool m_IsastrWindowAttributes[nbWindow][nbWindowAttributes];
	// check if the attribute for Range and bearing is offline defined as available 
	bool m_abRBToGo;
	// true if the range and bearing functionality is available
	bool m_IsabRBToGo;
	// checks if the APP position is offline defined set
	bool m_IsAPPPos;
	// checks the APP position is present in the offline defined file
	bool m_IsIsAPPPos;

	//methods

	// constructor of CWFAttribElem class
	CWFAttribElem();	
	
};

class CWFAttrib
{

public:


	///Windows. See above to modify Windows.
	enum m_WindowNames{
	   MAIN_RDR=0,
	   AUX_RDR,
	   HGHT_FILTERS,
	   SYS_INFO,
	   BRGHT_CTRL,
	   FIND_TRACK,
	   TRK_CTRL,
	   POPUP,
	   CHANGE_PASSWD,
	   CUSTOM_SET,
	   SAVE_AS,
	   SIGN_IN,
	   MESSAGE_BOX,
	   CSU_LIST
	};

	///Functions. See above to modify Functions.
	enum m_FunctionNames{
		Assume_De_assume=0,
		Tag_Track,
		Cfl_Selection,
		Rep_Selection,
		Ack_Emergency_SSR_Code,
		Wtc_Selection,
		ATyp_Selection,
		Ades_Selection,
		Assr_Selection,
		Rwy_Selection,
		Solve_CsCsTagMismatch,
		Ack_Apw_Msaw
	};

	//methods

	CWFAttrib(const QString & strDataSet,bool bSetMinMaxSize);
	virtual ~CWFAttrib();
	///returns true if the efName function is available for erRole and emSysMode
	///returns false in the other case. 
	///The returned value is unknown if one of the three parameters is out
	///of range.
	bool FunctionEligibility(int efName, QString Group, QString SubGroup, QString Location, QString Role);
	///returns true if the ewName window is available for erRole and emSysMode
	///returns false in the other case. 
	///The returned value is unknown if one of the three parameters is out
	///of range.
	bool WindowEligibility(int ewName, QString Group, QString SubGroup, QString Location, QString Role);
	bool SetMinMaxAttributes(QWidget* w,int ewName, QString Group, QString SubGroup, QString Location, QString Role, bool FromCustomSet);
	bool WindowAttributes(QString Window,int ewName, QString Group, QString SubGroup, QString Location, QString Role, bool Forced);
	///Set the off-line defined attributes to the window.
	bool HasWindowTitle(QWidget* w,int ewName, QString Group, QString SubGroup, QString Location, QString Role);
	bool RBToGo(QString Group, QString SubGroup, QString Location, QString Role);
	///Value vectors giving a string representation of a (Role,Mode,Window,
	///Function) enumerate.	
	bool IsMainCollapsible(QString Group, QString SubGroup, QString Location, QString Role);
	bool IsAPPPos(QString Group, QString SubGroup, QString Location, QString Role);

private:

	//variables
	bool m_bSetMinMaxSize;
	QString m_strDataSet;
	QString m_strVersion;
	QString m_strFileGeneral;
	QDict<CWFAttribElem> m_MapOfWFAttrib;
	QDict<CWFEligElem> m_MapOfWFElig;
	int m_aiWindowGeneralAttrib[nbWindow][nbWindowGeneralAttributes];
	bool m_bDataSetOk;
	enum m_WindowAttributes{
	  WND_CLOSEABILITY=0,
	  WND_COLLAPSEABILITY,
	  WND_MOVEABILITY,
	  WND_RESIZEABILITY,
	  WND_MINMAX_SIZES
	};
	enum m_MinMaxPosition{
		MIN_H=0,
		MAX_H,
		MIN_V,
		MAX_V
	};
	enum WindowGeneralAttributes{
		WND_PRIORITIES=0,
	};	
	typedef QValueVector<QString> StringVector;	
	StringVector m_qvvWindowName;
	StringVector m_qvvFunctionName;
	StringVector m_qvvWindowAttributes;

	//methods
	CWFAttrib();
	void InitAttributes();
	void InitDataSet();
	void InitWindows();
	void InitFunctions();
	QString FileFrom(QString Group, QString SubGroup, QString Location, QString Role);
	bool ReadOneEligibilitie(QString Group, QString SubGroup, QString Location, QString Role, QString& Error);
	void ReadEligibilities();
	bool ReadOneAttribute(QString Group, QString SubGroup, QString Location, QString Role, QString& Error);
	void ReadAttributes();
	bool ReadBoolAttribute(int ewName,int iAttrib, QString Group, QString SubGroup, QString Location, QString Role);
	int ReadOneIntAttribute(int ewName,int iAttrib, QString Group, QString SubGroup, QString Location, QString Role, int iPos);
	CWFAttribElem* GetAttribElem(QString Group, QString SubGroup, QString Location, QString Role, int nbParam);
	CWFEligElem* GetEligElem(QString Group, QString SubGroup, QString Location, QString Role);
	CWFAttribElem* CreateAttribElem(QString Group, QString SubGroup, QString Location, QString Role);
	CWFEligElem* CreateEligElem(QString Group, QString SubGroup, QString Location, QString Role);
	QString BuilMapKey(QString Group, QString SubGroup, QString Location, QString Role);	
};

#endif //WF_ATTRIB_H

//--------------------------------< THE END >----------------------------------

