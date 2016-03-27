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
//  FILENAME: Elements.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the tools to display in the planview as Range and Bearing line,
//		Range Marker, Meteo display, Track Miles Line and Direction Finder display
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
#include "StdAfx.h"
#include "View.h"
#include "AsdView.h"
#include "Elements.h"
#include "RwdView.h"
#include "Track.h"
#include "math.h"
#include "ColorMngr.h"
#include "FontMngr.h"
#include "WdwMngr.h"
#include "./CommonFiles/IniFile.h"
#include "Asterix.h"
#include "AsdApp.h"
#include "TopLevel.h"
#include "Geo.h"
#include "InfoMngr.h"
#include <stdlib.h>

//-GLOBAL DATA-----------------------------------------------------------------

//CVector static parameters
CArray<CVector*,CVector*> CVector::m_sVectorTable;
CVector* CVector::m_psCurVector = NULL;

//CNewRange static parameters
CNewRange* CNewRange::m_psCurRange = NULL;

//CRangeMark static parameters
CArray<CView*,CView*> CRangeMark::m_sViewTable;
CArray<CRangeMark*,CRangeMark*> CRangeMark::m_sRMarkTable;
int CRangeMarkView::m_sInterline=1;
int CRangeMarkView::m_sRange=5;
int CRangeMarkView::m_sOrientation=false;
int CRangeMarkView::m_sNbRangeMarker=0;

//CRangeMark static parameters
CArray<CView*,CView*> CTOORArrow::m_sViewTable;
CArray<CTOORArrow*,CTOORArrow*> CTOORArrow::m_sTOORArrowsTable;


//CTrkMiles static parameters
CArray<CTrkMiles*,CTrkMiles*> CTrkMiles::m_sTrkMilesTable;
CTrkMiles* CTrkMiles::m_psCurTrkMiles = NULL;
int TrkmTimeout=3;


//-FUNCTION PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CVector
///  This class manages the settings of a range and bearing line.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
///  Argument : CView*, view on which the range and bearing line must be displayed
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CVector::CVector(CView* pView)
{
	m_FPoint = QPoint(0,0);
	m_LPoint = QPoint(0,0);
	m_pFTrack = NULL;
	m_pLTrack = NULL;
	m_pOldLTrack = NULL;
	m_Distance=0;
	m_Bearing=0;
	m_Time=0;
	m_State = firstcreated;
	m_IsCreated=false;
	//The object is associated to a CVectorView object that will be its graphical
	//representation
	CVectorView* pElem = new CVectorView();
	pElem->Init(this,pView);
	m_pElem=pElem;
	((CRwdView*)pView)->AddElement(pElem);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CVector&, CVector object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CVector::CVector(CVector& pVector)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CVector::~CVector()
{
	((CRwdView*)m_pElem->m_pView)->RemoveElement(m_pElem);
	delete m_pElem;
	for (int vect=0;vect<m_sVectorTable.GetSize();vect++)
	{
		if (m_sVectorTable[vect]==this)
		{
			m_sVectorTable.RemoveAt(vect);
			break;
		}
	}
	//if the RB is associated to a track the association is canceled
	if (m_pFTrack)
		m_pFTrack->AssociateVector(this,false);
	if (m_pLTrack)
		m_pLTrack->AssociateVector(this,false);

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function manages the attribution or modification of the first extremity 
///  of a RB line. 
///  Argument 1 : QPoint, position where the click AB is performed
///  Argument 2 : CView*, view on which the range and bearing line must be displayed
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CVector::SetFirstPoint(QPoint point,CView* pView)
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	switch (this->m_State)
	{
	case firstcreated:
		//No extremity is attributed yet
		//both extremities are set to the same point, with the new status
		//the m_LPoint will be updated with the mouse pointer position by the 
		//CViewTrack::OnMouseMove function
		m_FPoint = point;
		m_LPoint = point;
		//the status is changed
		m_State = modify;
		//We check if the first selected point cannot be associated to a track
		m_pFTrack=pMngr->FindTrackByPos(point,pView);
		if (m_pFTrack)
			m_pFTrack->AssociateVector(this);
		//The display is updated
		Update();
		break;
	case modify:
		//One extremity is already attributed
		//The new selected point becomes the first associated point and replaces the 
		//former one
		m_FPoint = point;
		//The track association with the former extremity is canceled
		if (m_pFTrack)
			m_pFTrack->AssociateVector(this,false);
		m_pFTrack = NULL;
		//We check if the newly selected point cannot be associated to a track
		m_pFTrack=pMngr->FindTrackByPos(point,pView);
		if (m_pFTrack)
			m_pFTrack->AssociateVector(this);
		//The display is updated
		Update();
		break;
	case created:
		//both extremities are attributed
		break;
	default :
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function manages the attribution or modification of the second extremity 
///  of a RB line. 
///  Argument 1 : QPoint, position where the click AB is performed
///  Argument 2 : CView*, view on which the range and bearing line must be displayed
///  Argument 3 : bool, whether the function is called to end the creation or to 
///				  update the RB line presentation or not
///  Argument 4 : bool, whether the modification of one extremity is canceled or not	
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CVector::SetLastPoint(QPoint point,CView* pView,bool endcreation,bool modcnl)
{
	CTrackMngr* pMngr=CAsdApp::GetApp()->GetTrackMngr();
	bool IsModifiable = true;
	QRect updRect;
	CTrack* pTmpTrk=NULL;
	switch (this->m_State)
	{
	case firstcreated:
		//No extremity is attributed yet
		break;
	case modify:
		//One extremity is already attributed
		//If the modification mode is canceled
		if (modcnl)
		{
			//The status is switched back to created
			m_State = created;
			//Former point is retrieved
			m_LPoint=m_OldLPoint;
			//Any new track association is canceled
			if (m_pOldLTrack!=m_pLTrack)
			{
				if (m_pLTrack)
					m_pLTrack->AssociateVector(this,false);
				m_pLTrack = m_pOldLTrack;
			}
			m_pOldLTrack=NULL;
			((CRwdView*)m_pElem->m_pView)->InvalidateRect(&m_pElem->m_BoundingRect);
			((CRwdView*)m_pElem->m_pView)->SetModifyElement(NULL);
			m_psCurVector=NULL;
			((CRwdView*)m_pElem->m_pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
		}
		else
		{
			//even if the last extremity is not defined, if the mouse pointer
			//is over a track, the association must be performed in order to 
			//display the time to go in the label
			pTmpTrk=pMngr->FindTrackByPos(point,pView);
			if (pTmpTrk!=m_pLTrack)
			{
				if (m_pLTrack)
					m_pLTrack->AssociateVector(this,false);
				m_pLTrack = pTmpTrk;
				if (m_pLTrack)
					m_pLTrack->AssociateVector(this);
			}
			//The last extremity is set to the mouse pointer position
			m_LPoint = point;
			//If the second extremity is set (click SB action) the status
			//is set to created and any former last extremity definition
			//is canceled
			if (endcreation)
			{
				if (m_pOldLTrack)
					m_pOldLTrack->AssociateVector(this,false);
				m_pOldLTrack=NULL;
				m_State = created;
				m_IsCreated=true;
				((CRwdView*)m_pElem->m_pView)->InvalidateRect(&m_pElem->m_BoundingRect);
				((CRwdView*)m_pElem->m_pView)->SetModifyElement(NULL);
				m_psCurVector=NULL;
				((CRwdView*)m_pElem->m_pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
			}
		}
		Update();
		break;
	case created:
		//both extremities are attributed
		if (!endcreation)
		{
			IsModifiable &= ((CRwdView*)m_pElem->m_pView)->IsModifyNull();
			//If the interaction is allowed, upon a click AB on the label
			//the extremity attached to the label can be modified
			if (IsModifiable)
			{
				//The status is then changed 
				m_State = modify;
				m_psCurVector=this;
				updRect = m_pElem->m_BoundingRect;
				updRect=updRect.normalize();
				//The former extremity is saved to be able to come back to the
				//former situation (cancel modification mode)
				if (m_IsCreated)
				{
					m_OldLPoint=m_LPoint;
					if (m_pLTrack)
					{
						m_pOldLTrack=m_pLTrack;
						m_pLTrack=NULL;
					}
				}
				((CRwdView*)m_pElem->m_pView)->InvalidateRect(&m_pElem->m_BoundingRect);
				((CRwdView*)m_pElem->m_pView)->SetModifyElement(m_pElem);
				((CRwdView*)m_pElem->m_pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPCrosshair));
			}
		}
		else
		{
			//If the RB is already created and the function is called with the attribute
			//endcreation set to true, the RB presentation is removed from display
			delete m_pElem;
		}
		break;
	default :
		break;
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called from the CTrack class each time the position of a
///  track attached to a RB line is updated. 
///  Argument 1 : CTrack*, the updated track
///  Argument 2 : bool, whether the track is deleted or not 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl040, CIAsdFctRwsTlsRbl041
//
//-----------------------------------------------------------------------------
void CVector::UpdateTrk(CTrack* pTrack,bool cancel)
{
	//If the track is attached to the first line extremity
	if (pTrack==m_pFTrack)
	{
		if (cancel)
			m_pFTrack=NULL;
		else
			m_FPoint = m_pFTrack->GetPos();
	}
	//If the track is attached to the second line extremity
	if (pTrack==m_pLTrack)
	{
		if (cancel)
			m_pLTrack=NULL;
		else
			m_LPoint = m_pLTrack->GetPos();
	}
	//If the track is attached to the line extremity being modified
	if (pTrack==m_pOldLTrack)
	{
		if (cancel)
			m_pOldLTrack=NULL;
		else
			m_OldLPoint = m_pOldLTrack->GetPos();
	}	
	Update();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function updates the position of the line and the information to be 
///  displayed in the label. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CVector::Update()
{
	double bearing;
	double dist;
	double speed;
	//Length of the line
	dist = (((double)(m_LPoint.x()) - (double)(m_FPoint.x())) *
		((double)(m_LPoint.x()) - (double)(m_FPoint.x())) +
		((double)(m_LPoint.y()) - (double)(m_FPoint.y())) *
		((double)(m_LPoint.y()) - (double)(m_FPoint.y())));
	dist = sqrt(dist);
	m_Distance = (int)dist;
	//Bearing of the line from First point to Last point 
	if ((m_LPoint.x() == m_FPoint.x())&&(m_LPoint.y() == m_FPoint.y()))
		bearing = 0;
	else
	{
		bearing = atan2(((double)m_LPoint.y() - (double)m_FPoint.y()),
			((double)m_LPoint.x() - (double)m_FPoint.x()));
		bearing = (1.570796327) - bearing;
		if (bearing <0) 
			bearing += 6.283185307;
		bearing = bearing * (180.0/3.141592654);
		bearing +=0.5;
		m_Bearing = ((int)bearing)%360;
	}
	m_Time = -1;
	//If the line is attached to a track, the Time to go is calculated
	if (m_pFTrack && !m_pLTrack)
	{
		speed = m_pFTrack->GetSpeed();
		m_Time=(int)(dist*10/speed);
	}
	//The presentation is updatedint layer,
	m_pElem->OnUpdate();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the maximum RB line (6) is reached for the
///  radar window put in argument.
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  bool, whether the maximum number of RB line per radar window is reached
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CVector::IsMaxNumberReached(CView* pView)
{
	int nb=0;
	//Check in the m_sVectorTable how many RB are associated to the view
 	for (int v=0;v<m_sVectorTable.GetSize();v++)
		if (m_sVectorTable[v]->m_pElem->m_pView==pView)
			nb++;
	return (nb>=6);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to start the creation of a RB line.
///  Argument : CView*, radar window on which the line will be created 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CVector::Create(CView* pView)
{
	bool IsModifiable = true;
	//The RB line being modified or created is identified by the m_psCurVector
	//pointer
	//If the pointer is null, no RB is under modification or creation and the 
	//creation mode can be initiated
	if (!m_psCurVector)
	{
		IsModifiable &= ((CRwdView*)pView)->IsModifyNull();
		if (IsModifiable)
		{
			m_psCurVector= new CVector(pView);
			m_sVectorTable.Add(m_psCurVector);
			m_psCurVector->m_State = firstcreated;
			((CRwdView*)pView)->SetModifyElement(m_psCurVector->m_pElem);
			((CRwdView*)pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPCrosshair));
		}
	}
	//If the pointer is not null it must be set to null
	else
	{
		((CRwdView*)pView)->SetModifyElement(NULL);
		delete m_psCurVector;
		m_psCurVector = NULL;
		((CRwdView*)pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the CWP is under RB creation or modification
///  mode or not.
/// 
///  RETURNS: 
///  bool, whether the CWP is on RB creation or modification mode or not
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CVector::IsInCreation()
{
	//The RB line being modified or created is identified by the m_psCurVector
	//pointer
	return m_psCurVector!=NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function removes from display (delete object) the created RB lines for
///  all the existing radar windows.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CVector::RemoveTables()
{
	//If a line is being modified or created, it is deleted anyway
	if (m_psCurVector)
	{
		if (m_psCurVector->m_pElem && m_psCurVector->m_pElem->m_pView)
		{
			((CRwdView*)m_psCurVector->m_pElem->m_pView)->SetModifyElement(NULL);
			((CRwdView*)m_psCurVector->m_pElem->m_pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
		}
		delete m_psCurVector;
		m_psCurVector = NULL;
	}
	for (int vect=m_sVectorTable.GetUpperBound();vect>=0;vect--)
		delete m_sVectorTable[vect];
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function removes from display (delete object) all the created RB lines
///  associated to the radar window put in argument.
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CVector::RemoveView(CView* pView)
{
  for (int trkm=0;trkm<m_sVectorTable.GetSize();trkm++)
    {
      if ((m_sVectorTable[trkm]) && (m_sVectorTable[trkm]->m_pElem->m_pView==pView))
		delete m_sVectorTable[trkm];
    }
  //We ensure no creation mode is stucked
  if (m_psCurVector!=NULL)
    {
      ((CRwdView*)pView)->SetModifyElement(NULL);
      m_psCurVector=NULL;
    }
}


//-----------------------------------------------------------------------------
//
/// \class CVectorView
///  This class manages the display of a range and bearing line.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CVectorView::CVectorView()
{
	m_Type=VectorView;
	m_BoundingRect=QRect();
	m_Select = false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CVectorView&, CVectorView object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CVectorView::CVectorView(CVectorView& pVectorView)
{
	//not yet implemented
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CVectorView::~CVectorView()
{
	QRect updRect = m_BoundingRect;
	((CRwdView*)m_pView)->InvalidateRect(&updRect);
	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions draws the Range and Bearing line. 
///  Argument 1 : QPainter*, painting device
///  Argument 2 : int, drawn layer number
///  Argument 3 : int not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl020,CIAsdFctRwsTlsRbl025, CIAsdFctRwsTlsRbl085
//
//-----------------------------------------------------------------------------
void CVectorView::OnDraw(QPainter* pDC,int Prio )
{
	int colornb;
	QPen pen;
	QBrush brush;
	QString Line1,Line2,Line3;
	QSize size1,size2,size3,sizecirc(3,3),offset(15,15);
	QPoint labelpos;
	//Get the offline defined font
	QFont font=CFontMngr::GetFontByName("RNGBEARLINE_LABEL");
	//The line is drawn if it is under creation or modification (layer overlay) 
	//or if it is already created (layer primary)
	if (Prio!=0)
	  return;
	if ((m_pVector->m_State==CVector::modify)|| (m_pVector->m_State==CVector::created))
	{
		//Get the offline defined color according to the RB status
		if (m_pVector->m_State==CVector::modify)
			colornb=CColorMngr::GetColorNb("RW_TOOLS");
		else
			colornb=CColorMngr::GetColorNb("RW_TOOLS");
		if (m_Select)
			colornb=CColorMngr::GetColorNb("SELECTED_FRGRND");
		pen.setStyle(Qt::SolidLine);
		pen.setColor(CColorMngr::TransformColorNb(colornb,AsdToolsCategory));
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(CColorMngr::TransformColorNb(colornb,AsdToolsCategory));
		pDC->setPen(pen);
		pDC->setBrush(brush);
		pDC->drawLine(m_FPoint,m_LPoint);
		if (m_Select)
		{
			QRect backGrndRect=m_TextRect;
			backGrndRect.addCoords(-5,0,3,4);
			pDC->fillRect(backGrndRect,CColorMngr::GetColor(CColorMngr::GetColorNb("SELECTED_BKGRND")));
		}
		QRect pntrect;
		//If the line is under creation or modification, the point at the last extremity
		//must not be displayed
		if (m_pVector->m_State!=CVector::modify)
		  {
			pntrect=QRect(m_LPoint,m_LPoint);
			pntrect.addCoords(-sizecirc.width(),-sizecirc.height(),sizecirc.width(),sizecirc.height());
			pDC->drawEllipse(pntrect);
		  }
		pntrect=QRect(m_FPoint,m_FPoint);
		pntrect.addCoords(-sizecirc.width(),-sizecirc.height(),sizecirc.width(),sizecirc.height());
		pDC->drawEllipse(pntrect);
		//The label lines are defined
		QString degres="N0";
		Line1.sprintf("%.3d%s",m_pVector->m_Bearing,degres.ascii());
		Line2.sprintf("%.1d.%.1d",m_pVector->m_Distance/32,(m_pVector->m_Distance*10/32)%10);
		if ((m_pVector->m_Time!=-1) && (CAsdApp::GetApp()->RBToGo()))
		{
			if (m_pVector->m_Time>=3600)
				Line3="59'59";
			else if (m_pVector->m_Time>=60)
				Line3.sprintf("%d'%.2d",(m_pVector->m_Time%3600)/60,m_pVector->m_Time%60);
			else
				Line3.sprintf("0'%.2d",m_pVector->m_Time%60);
		}
		pDC->setFont(font);
		//The label is positionned and drawn according to the last extremity position and the bearing
		QRect tmp=pDC->boundingRect(0,0,0,0,Qt::AlignLeft,Line1);
		size1=QSize(tmp.width(),tmp.height());
		tmp=pDC->boundingRect(0,0,0,0,Qt::AlignLeft,Line2);
		size2=QSize(tmp.width(),tmp.height());
		tmp=pDC->boundingRect(0,0,0,0,Qt::AlignLeft,Line3);
		size3=QSize(tmp.width(),tmp.height());
		labelpos=m_LPoint;
		if ((m_pVector->m_Bearing<=180) || (m_pVector->m_Bearing>180))
		{
			pDC->drawText(labelpos.x()+offset.width(),labelpos.y()-offset.height()+size1.height(),Line1);
			pDC->drawText(labelpos.x()+offset.width(),labelpos.y()-offset.height()+size1.height()+size2.height(),Line2);
			pDC->drawText(labelpos.x()+offset.width(),labelpos.y()-offset.height()+size1.height()+size2.height()+size3.height(),Line3);
		}
		else
		{
			pDC->drawText(labelpos.x()-size1.width()-offset.width(),labelpos.y()-offset.height()+size1.height(),Line1);
			pDC->drawText(labelpos.x()-size2.width()-offset.width(),labelpos.y()-offset.height()+size1.height()+size2.height(),Line2);
			pDC->drawText(labelpos.x()-size3.width()-offset.width(),labelpos.y()-offset.height()+size1.height()+size2.height()+size3.height(),Line3);
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions retrieves the Range and Bearing line settings from the associated
///  CVector class before the display is updated (OnDraw function is called). 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl025, CIAsdFctRwsTlsRbl085
//
//-----------------------------------------------------------------------------
void CVectorView::OnUpdate()
{
	QPainter pDC(m_pView);
	//First point
	m_FPoint = m_pVector->m_FPoint;
	((CAsdView*)m_pView)->LPtoDP(&m_FPoint);
	//Last point
	m_LPoint = m_pVector->m_LPoint;
	((CAsdView*)m_pView)->LPtoDP(&m_LPoint);
	QRect oldRect = m_BoundingRect;
	m_BoundingRect = QRect(m_FPoint,m_LPoint);
	m_BoundingRect=m_BoundingRect.normalize();
	QSize sizecirc(3,3),offset(15,15);
	m_BoundingRect.addCoords(-sizecirc.width(),-sizecirc.height(),sizecirc.width(),sizecirc.height());
	//Label position and settings
	QString Line1,Line2,Line3;
	QPoint labelpos;
	QFont font=CFontMngr::GetFontByName("RNGBEARLINE_LABEL");
    QString degres="N0";
    Line1.sprintf("%.3d%s",m_pVector->m_Bearing,degres.ascii());
	Line2.sprintf("%.1d.%.1d",m_pVector->m_Distance/32,(m_pVector->m_Distance*10/32)%10);
	if ((m_pVector->m_Time!=-1) && (CAsdApp::GetApp()->RBToGo()))
		Line3="59'59";

	//The rect of the screen to be updated is calculated 
	pDC.setFont(font);
	QSize size1,size2,size3;
	QRect tmp=pDC.boundingRect(0,0,0,0,Qt::AlignLeft,Line1);
	size1=QSize(tmp.width(),tmp.height());
	tmp=pDC.boundingRect(0,0,0,0,Qt::AlignLeft,Line2);
	size2=QSize(tmp.width(),tmp.height());
	if (size2.width()>size1.width())
		size1.setWidth(size2.width());
	tmp=pDC.boundingRect(0,0,0,0,Qt::AlignLeft,Line3);
	size3=QSize(tmp.width(),tmp.height());
	if (size3.width()>size1.width())
		size1.setWidth(size3.width());
	labelpos=m_LPoint;
	if ((m_pVector->m_Bearing<=180) || (m_pVector->m_Bearing>180))
		m_TextRect=QRect(QPoint(labelpos.x()+offset.width(),labelpos.y()-offset.height()),QPoint(labelpos.x()+offset.width()+size1.width(),labelpos.y()-offset.height()+size1.height()+size2.height()+size3.height()));
	else
		m_TextRect=QRect(QPoint(labelpos.x()-offset.width(),labelpos.y()-offset.height()),QPoint(labelpos.x()-offset.width()-size1.width(),labelpos.y()-offset.height()+size1.height()+size2.height()+size3.height()));
	m_TextRect=m_TextRect.normalize();
	m_BoundingRect|=m_TextRect;
	m_BoundingRect.addCoords(-6,-6,6,6);
	oldRect|=m_BoundingRect;
	oldRect=oldRect.normalize();
	
	((CRwdView*)m_pView)->InvalidateRect(&oldRect);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions associates the Range and bearing line presentation class to the
///  radar window on which it will be displayed and to the CVector class that will
///  provide the settings.
///  Argument 1 : LPVOID, associated class
///  Argument 2 : CView*, associated radar window
///  Argument 3 : CDisplayElement*, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CVectorView::Init(LPVOID pData,CView *pView,CDisplayElement* )
{
	m_pView = pView;
	m_pVector = (CVector*)pData;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns an integer corresponding to the fact that the range and
///  bearing line must present the implicit focus or not.
///  Argument 1 : QPoint, position of the mouse pointer
///  Argument 2 : bool, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl045
//
//-----------------------------------------------------------------------------
int CVectorView::GetPriority(QPoint point,bool )
{
	//A RB line cannot be selected if the CWP is under a tool modification or
	//creation mode
	if (CRangeMark::IsRangeMarkInModif() || CTrkMiles::IsInCreation() 
		|| CVector::IsInCreation() || CNewRange::IsInCreation() || CRwdView::IsInViewModif())
		return 0;
	QRect bound;
	QPoint vect1,vect2,vect3,vect4;
	int vectprod,dist,dist2;
	//If the line is created and the mouse pointer is over the line or the label
	//the function returns the prio associated to the RB lines
	if ((m_pVector->m_State==CVector::created) && (m_BoundingRect.contains(point)))
	{
		if (m_TextRect.contains(point))
			return PRIO_VECTOR;
		bound = QRect(m_FPoint,m_LPoint);
		bound=bound.normalize();
		bound.addCoords(-5,-5,5,5);
		if (bound.contains(point))
		{
			vect1 = m_LPoint-m_FPoint;
			vect2 = point - m_FPoint;
			dist = (vect1.x()*vect1.x())+(vect1.y()*vect1.y());
			vectprod = (vect1.x()*vect2.x())+(vect1.y()*vect2.y());
			vect3.setX( int(double(vect1.x())*double(vectprod)/double(dist)));
			vect3.setY( int(double(vect1.y())*double(vectprod)/double(dist)));
			vect4 = vect2-vect3;
			dist2 = vect4.x()*vect4.x()+vect4.y()*vect4.y();
			if (dist2<=20)
				return PRIO_VECTOR;
		}
	}
	return 0;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the mouse pointer is moved while the range
///  and bearing line is under creation or modification mode.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl090
//
//-----------------------------------------------------------------------------
void CVectorView::OnMouseMove(UINT nFlags,QPoint point)
{
	((CAsdView*)m_pView)->DPtoLP(&point);
    QRect radarwndrect=m_pView->geometry();
    QPoint globalpnt=QCursor::pos();
    QRect statusbarrect=((CRwdView*)m_pView)->GetStatusBar()->rect();

	switch (m_pVector->m_State)
	{
	case CVector::firstcreated:
		//If the mouse pointer leaves the radar window on which the creation mode has been 
		//invoked, the creation mode is canceled
        if (globalpnt.x()>=radarwndrect.x()+radarwndrect.width() || globalpnt.x()<=radarwndrect.x() 
				|| globalpnt.y()<=radarwndrect.y() 
				|| globalpnt.y()>=radarwndrect.y()+radarwndrect.height()-statusbarrect.height())
			m_pVector->SetFirstPoint(point,m_pView);
		break;
	case CVector::modify:
		//If the mouse pointer leaves the radar window on which the creation or modification 
		//mode has been invoked, the mode is canceled
		if (globalpnt.x()>=radarwndrect.x()+radarwndrect.width() || globalpnt.x()<=radarwndrect.x() || globalpnt.y()<=radarwndrect.y() || globalpnt.y()>=radarwndrect.y()+radarwndrect.height()-statusbarrect.height())
		{
			//The line is removed from display
			if (!m_pVector->m_IsCreated)
			{
				m_pVector->SetLastPoint(point,m_pView,true);
				delete m_pVector;
			}
			else
				//The line extremity modified is set back to its previous position
				m_pVector->SetLastPoint(point,m_pView,false,true);
		}
		else
			//The function SetLastPoint is called with the end modification argument
			//set to false to draw the line from the first created point to the mouse pointer
			m_pVector->SetLastPoint(point,m_pView,false);
		break;
	default:
		break;
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click AB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl035, CIAsdFctRwsTlsRbl050
//
//-----------------------------------------------------------------------------
void CVectorView::OnLButtonDown(UINT nFlags,QPoint point)
{
	QPoint pnt=point;
	((CAsdView*)m_pView)->DPtoLP(&point);
	switch (m_pVector->m_State)
	{
	case CVector::firstcreated:
		//The first point is defined
		m_pVector->SetFirstPoint(point,m_pView);
		break;
	case CVector::modify:
		//The first point is modified
		m_pVector->SetFirstPoint(point,m_pView);
		break;
	case CVector::created:
		//if allowed upon a click AB on the label the extremity attached to the label
		//can be modified
		if (m_Select && m_TextRect.contains(pnt))
			m_pVector->SetLastPoint(point,m_pView,false);
		break;
	default :
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click WB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl055, CIAsdFctRwsTlsRbl070
//
//-----------------------------------------------------------------------------
void CVectorView::OnMButtonDown(UINT nFlags,QPoint point)
{
	((CAsdView*)m_pView)->DPtoLP(&point);
	switch (m_pVector->m_State)
	{
	case CVector::firstcreated:
		//The line is created so that is can be destroyed
		m_pVector->SetFirstPoint(point,m_pView);
		m_pVector->SetLastPoint(point,m_pView,true);
		delete m_pVector;
		break;
	case CVector::modify:
		//If the line is in creation mode it is destroyed
		if (!m_pVector->m_IsCreated)
		{
			m_pVector->SetLastPoint(point,m_pView,true);
			delete m_pVector;
		}
		else
		//if the line is in modification mode, the last point is set back to
		//its previous position
		{
			m_pVector->SetLastPoint(point,m_pView,false,true);
		}
		break;
	case CVector::created:
		//The line is destroyed
		if (m_Select)
			delete m_pVector;
		break;
	default :
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click SB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl060, CIAsdFctRwsTlsRbl065
//
//-----------------------------------------------------------------------------
void CVectorView::OnRButtonDown(UINT nFlags,QPoint point)
{
	QPoint pnt=point,tmp;
	CTrack* pTmpTrk = NULL;
	((CAsdView*)m_pView)->DPtoLP(&point);
	switch (m_pVector->m_State)
	{
	case CVector::modify:
		//The second extremity is set
		m_pVector->SetLastPoint(point,m_pView,true);
		break;
	case CVector::created:
		//A click SB on the label toggles the first and last points (the label
		//changes of attached extremity)
		if (m_TextRect.contains(pnt))
		{
			tmp=m_pVector->m_FPoint;
			m_pVector->m_FPoint=m_pVector->m_LPoint;
			m_pVector->m_LPoint=tmp;
			pTmpTrk=m_pVector->m_pFTrack;
			m_pVector->m_pFTrack=m_pVector->m_pLTrack;
			m_pVector->m_pLTrack=pTmpTrk;
			m_pVector->Update();
		}
		break;
	default:
		break;
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the Escape key press action.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRbl015, CIAsdFctKfk085
//
//-----------------------------------------------------------------------------
void CVectorView::OnKeyDown()
{
	QPoint point=QCursor::pos();
	point=m_pView->mapFromGlobal(point);
	switch (m_pVector->m_State)
	{
	case CVector::firstcreated:
		//The line is created so that is can be destroyed
		m_pVector->SetFirstPoint(point,m_pView);
		m_pVector->SetLastPoint(point,m_pView,true);
		delete m_pVector;
		break;
	case CVector::modify:
		//If the line is in creation mode it is destroyed
	    if (!m_pVector->m_IsCreated)
		{
		    m_pVector->SetLastPoint(point,m_pView,true);
		    delete m_pVector;
		}
		//if the line is in modification mode, the last point is set back to
		//its previous position
		else
		    m_pVector->SetLastPoint(point,m_pView,false,true);
		break;
	case CVector::created:
		break;
	default :
		break;

	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selection status of the line to the one put in argument.
///  Argument : bool, Selection status to be applied
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CVectorView::SelectElement(bool Select)
{
	m_Select = Select;
	OnUpdate();
}


//-----------------------------------------------------------------------------
//
/// \class CNewRange
///  This class manages the settings and the status of a the creation of a new 
///  radar window.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
///  Argument : CView*, parent view of the created view
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CNewRange::CNewRange(CView* pView)
{
	m_InitPoint=QPoint(0,0);
	m_EndPoint=QPoint(0,0);
	//The object is associated to a CNewRangeView object that will be the graphical
	//representation of the radar window creation
	CNewRangeView* pElem = new CNewRangeView();
	pElem->Init(this,pView);
	m_pElem=pElem;
	((CRwdView*)pView)->AddElement(pElem);
	((CRwdView*)pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPCrosshair));
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CNewRange&, CNewRange object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CNewRange::CNewRange(CNewRange& pNewRange)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CNewRange::~CNewRange()
{
	((CRwdView*)m_pElem->m_pView)->RemoveElement(m_pElem);
	((CRwdView*)m_pElem->m_pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
	delete m_pElem;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function manages the attribution of the first corner of the new view rect. 
///  Argument 1 : QPoint, position where the click AB is performed
///  Argument 2 : CView*, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRange::SetFirstPoint(QPoint point,CView* )
{
	switch (m_State)
	{
	case firstcreated:
		//If corner is attibuted yet, the first one is and the status of view is set
		//to modify
		m_InitPoint = point;
		m_State = modify;
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function manages the attribution of the second corner of the view. 
///  Argument 1 : QPoint, position where the click AB is performed
///  Argument 2 : CView*, parent view
///  Argument 3 : bool, whether the function is called to end the creation or not
///  Argument 4 : bool, whether the modification of one extremity is canceled or not	
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRange::SetLastPoint(QPoint point,CView* pView,bool endcreation,bool cancel)
{
	QRect RangeRect;
	switch (m_State)
	{
	case firstcreated:
		//The creation is canceled
		if (cancel)
		{
			((CRwdView*)pView)->InvalidateRect(&m_pElem->m_BoundingRect);
			((CRwdView*)pView)->SetModifyElement(NULL);
			m_psCurRange=NULL;
			delete this;
		}
		break;
	case modify:
		//If one corner is attributed the second is set
		m_EndPoint = point;
		m_pElem->OnUpdate();
		//If the parameter endcreation is set to true,
		if (endcreation)
		{
			((CRwdView*)pView)->InvalidateRect(&m_pElem->m_BoundingRect);
			((CRwdView*)pView)->SetModifyElement(NULL);
			//And the creation is not canceled, the created view borders
			//are validated and a TpView object (radar window) is initialized
			if (!cancel)
			{
				RangeRect =QRect(m_InitPoint,m_EndPoint);
				RangeRect=RangeRect.normalize();
				((CRwdView*)pView)->CreateSecondaryWnd(RangeRect,(CRwdView*)pView);
			}
			m_psCurRange=NULL;
			delete this;

		}
		break;
	case created:
		break;
	default :
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to start the creation of a view rect.
///  Argument : CView*, parent view 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRange::Create(CView* pView)
{
	bool IsModifiable = true;
	//The view rect being created is identified by the m_psCurRange
	//pointer
	//If the pointer is null, no new view is under creation and the 
	//creation mode can be initiated
	if (!m_psCurRange)
	{
		IsModifiable &= ((CRwdView*)pView)->IsModifyNull();
		if (IsModifiable)
		{
			m_psCurRange= new CNewRange(pView);
			m_psCurRange->m_State = firstcreated;
			((CRwdView*)pView)->SetModifyElement(m_psCurRange->m_pElem);
		}
	}
	//If the pointer is not null it must be set to null
	else
	{
		((CRwdView*)pView)->SetModifyElement(NULL);
		delete m_psCurRange->m_pElem;
		m_psCurRange = NULL;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the CWP is under a new view creation mode or not.
/// 
///  RETURNS: 
///  bool, whether the CWP is on creation mode or not
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CNewRange::IsInCreation()
{
	//The view rect being created is identified by the m_psCurRange
	//pointer
	return (m_psCurRange!=NULL);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function removes from display (delete object) any view under creation
///  associated to the radar window put in argument.
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRange::RemoveView(CView* pView)
{
	if (m_psCurRange)
	{
		if (pView==m_psCurRange->m_pElem->m_pView)
		{
			delete m_psCurRange->m_pElem;
			m_psCurRange=NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//
/// \class CNewRangeView
///  This class manages the display of a radar window under creation.
//
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CNewRangeView::CNewRangeView()
{
	m_Type=NewRangeView;
	m_BoundingRect=QRect();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CNewRangeView&, CNewRangeView object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CNewRangeView::CNewRangeView(CNewRangeView& pNewRangeView)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CNewRangeView::~CNewRangeView()
{
	((CRwdView*)m_pView)->InvalidateRect(&m_BoundingRect);	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions associates the view rect under creation presentation class to the
///  parent radar windowand to the CNewRange class that will provide the settings.
///  Argument 1 : LPVOID, associated class
///  Argument 2 : CView*, parent radar window
///  Argument 3 : CDisplayElement*, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRangeView::Init(LPVOID pData,CView *pView,CDisplayElement* )
{
	m_pView = pView;
	m_pNewRange = (CNewRange*)pData;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions draws the rect of the radar window being created. 
///  Argument 1 : QPainter*, painting device
///  Argument 2 : int, drawn layer number
///  Argument 3 : int not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRangeView::OnDraw(QPainter* pDC,int Prio )
{
  if ( (m_pNewRange->m_State==CNewRange::modify) && (Prio==0))
	{
		QPen pen(QColor(255,255,255));
		pDC->setPen(pen);
		QRect rect(m_InitPoint,m_EndPoint);
		pDC->drawRect(rect);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions retrieves view rect settings from the associated
///  CNewRange class before the display is updated (OnDraw function is called). 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRangeView::OnUpdate()
{
	QRect UpdRect = m_BoundingRect;
	m_InitPoint=m_pNewRange->m_InitPoint;
	((CRwdView*)m_pView)->LPtoDP(&m_InitPoint);
	m_EndPoint=m_pNewRange->m_EndPoint;
	((CRwdView*)m_pView)->LPtoDP(&m_EndPoint);
	//The rect of the screen to be updated is calculated
	m_BoundingRect=QRect(m_InitPoint,m_EndPoint);
	m_BoundingRect=m_BoundingRect.normalize();
	UpdRect|=m_BoundingRect;
	((CRwdView*)m_pView)->InvalidateRect(&UpdRect);	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the mouse pointer is moved while a radar
///  window is under creation mode.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsAux030
//
//-----------------------------------------------------------------------------
void CNewRangeView::OnMouseMove(UINT nFlags,QPoint point)
{
	QRect radarwndrect=m_pView->geometry();
    QPoint globalpnt=QCursor::pos();
    QRect statusbarrect=((CRwdView*)m_pView)->GetStatusBar()->rect();

	QPoint pos=point;
	((CRwdView*)m_pView)->DPtoLP(&pos);
	switch (m_pNewRange->m_State)
	{
	case CNewRange::firstcreated:
		//If the mouse pointer leaves the radar window on which the creation or modification 
		//mode has been invoked, the mode is canceled
      if (globalpnt.x()>=radarwndrect.x()+radarwndrect.width() || globalpnt.x()<=radarwndrect.x() || globalpnt.y()<=radarwndrect.y() || globalpnt.y()>=radarwndrect.y()+radarwndrect.height()-statusbarrect.height())
		  m_pNewRange->SetLastPoint(pos,m_pView,true,true);
		break;
	case CNewRange::modify:
		//If the mouse pointer leaves the radar window on which the creation mode has been 
		//invoked, the creation mode is canceled
      if (globalpnt.x()>=radarwndrect.x()+radarwndrect.width() || globalpnt.x()<=radarwndrect.x() || globalpnt.y()<=radarwndrect.y() || globalpnt.y()>=radarwndrect.y()+radarwndrect.height()-statusbarrect.height())
	  	  m_pNewRange->SetLastPoint(pos,m_pView,true,true);
	  else
			//The function SetLastPoint is called with the end modification argument
			//set to false to draw the line from the first created point to the mouse pointer
		  m_pNewRange->SetLastPoint(pos,m_pView,false);
		break;
	default:
		break;
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click AB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRangeView::OnLButtonDown(UINT nFlags,QPoint point)
{
	((CRwdView*)m_pView)->DPtoLP(&point);
	switch (m_pNewRange->m_State)
	{
	case CNewRange::firstcreated:
		//The first point is defined
		m_pNewRange->SetFirstPoint(point,m_pView);
		break;
	case CNewRange::modify:
		//The second point is defined
		m_pNewRange->SetLastPoint(point,m_pView,true);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click WB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRangeView::OnMButtonDown(UINT nFlags,QPoint point)
{
	((CRwdView*)m_pView)->DPtoLP(&point);
	switch (m_pNewRange->m_State)
	{
	case CNewRange::firstcreated:
		//the rect is removed from display
		m_pNewRange->SetLastPoint(point,m_pView,true,true);
		break;
	case CNewRange::modify:
		//the rect is removed from display
		m_pNewRange->SetLastPoint(point,m_pView,true,true);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the Escape key press action.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CNewRangeView::OnKeyDown()
{
	QPoint point=QPoint(0,0);
	switch (m_pNewRange->m_State)
	{
	case CNewRange::firstcreated:
		//the rect is removed from display
		m_pNewRange->SetLastPoint(point,m_pView,true,true);
		break;
	case CNewRange::modify:
		//the rect is removed from display
		m_pNewRange->SetLastPoint(point,m_pView,true,true);
		break;
	default:
		break;
	}
}


//-----------------------------------------------------------------------------
//
/// \class CMeteoData
///  This class manages the weather information received from the weather radar
///  service (CWeatherMngr class) for one radar window.
//
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CMeteoData::CMeteoData()
{
	m_IsDisplayed=false; 
	m_CurActive=-1;
	m_Radar = QPoint(0,0);
	m_RecptStarted=false;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CMeteoData&, CMeteoData object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CMeteoData::CMeteoData(CMeteoData& pmeteo)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CMeteoData::~CMeteoData()
{
	CWeatherMngr* pMngr=CAsdApp::GetApp()->GetWeatherMngr();
	for (int view=0;view<pMngr->m_ViewTable.GetSize();view++)
	{
		((CRwdView*)pMngr->m_ViewTable[view])->RemoveElement(m_ElemTable[view]);
		delete m_ElemTable[view];
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions retrieves from the offline definition the colors associated
///  to the 8 precipitation levels.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsWet015
//
//-----------------------------------------------------------------------------
void CMeteoData::Init()
{
	QString str;
	for (int i=0;i<=7;i++)
	{
		str.sprintf("WEATHER_LVL_%d",i);
		m_ColorLevelNb[i]=CColorMngr::GetColorNb(str);
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions is called to decode a Asterix message received from the weather
///  radar service.
///  Argument : CAsterixMsg*, received message
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsWet006
//
//-----------------------------------------------------------------------------
void CMeteoData::UpdateFromRadar(CAsterixMsg* pMsg)
{
	int MsgTyp,Recpt=0,curlevel=0,vectornb, tmp;
	m_VectorElem curvector;
	int start,end,total,X,Y,length;
	float azimuth,azoffset=(float)(3.14159/256.0);

	//Check if a complete weather update has already been received (it is saved 
	//in the table m_VectorTable at the position 0
	//if it is the case a new set of weather info is to be received at the position 1
	//of the same table
	if (m_CurActive==0)
		Recpt=1;
	if (pMsg->IsFieldPresent(MsgType))
	{
		MsgTyp = pMsg->GetUIntVal(MsgType);
		//Check if the message is the last message of the weather situation update
		if ((MsgTyp==0xFE) && (m_RecptStarted) && (pMsg->IsFieldPresent(WthrTotal)))
			m_RecptStarted=false;
		else
		{
			//Start the reception of a new weather info update
			if (MsgTyp==0xFE)
			{
				m_RecptStarted=true;
				m_VectorTable[Recpt].VectorTable.RemoveAll();
			}
			else
			{
				//if another type of message is received it is discraded 
				if (!m_RecptStarted)
					return;
			}
			if (MsgTyp==0xFF)
				m_RecptStarted=false;
		}
	}
	//The precipitation level is set
	if (pMsg->IsFieldPresent(WthrLevel))
		curlevel=(pMsg->GetUIntVal(WthrLevel)&0x70)>>4;
	//The weather data is received as vectors
	if(pMsg->IsFieldPresent(WthrCartVectors))
	{
		vectornb=pMsg->GetByteVal(WthrCartVectors,0);
		for (int i=0;i<vectornb;i++)
		{
			curvector.Level = curlevel;
			X=pMsg->GetByteVal(WthrCartVectors,i*3+1);
			Y=pMsg->GetByteVal(WthrCartVectors,i*3+2);
			length=pMsg->GetByteVal(WthrCartVectors,i*3+3)*m_Scale*2;
			if (X &0x80)
				X|=0xFFFFFF00;
			if (Y &0x80)
				Y|=0xFFFFFF00;
			azimuth=atan2(X,Y);
			start=sqrt((double)(X)*(double)X+(double)Y*(double)Y)*m_Scale*2;
			end=start+length;
			curvector.pos1.setX((int)(start*sin(azimuth-azoffset))+m_Radar.x());
			curvector.pos1.setY((int)(start*cos(azimuth-azoffset))+m_Radar.y());
			curvector.pos2.setX((int)(start*sin(azimuth+azoffset))+m_Radar.x());
			curvector.pos2.setY((int)(start*cos(azimuth+azoffset))+m_Radar.y());
			curvector.pos3.setX((int)(end*sin(azimuth+azoffset))+m_Radar.x());
			curvector.pos3.setY((int)(end*cos(azimuth+azoffset))+m_Radar.y());
			curvector.pos4.setX((int)(end*sin(azimuth-azoffset))+m_Radar.x());
			curvector.pos4.setY((int)(end*cos(azimuth-azoffset))+m_Radar.y());
			m_VectorTable[Recpt].VectorTable.Add(curvector);
		}
								
	}
	//The weather data is received as vectors
	if(pMsg->IsFieldPresent(WthrPolarVectors))
	{
		vectornb=pMsg->GetByteVal(WthrPolarVectors,0);
		for (int i=0;i<vectornb;i++)
		{
			curvector.Level = curlevel;
			start=pMsg->GetByteVal(WthrPolarVectors,i*4+1)*m_Scale;
			end=pMsg->GetByteVal(WthrPolarVectors,i*4+2)*m_Scale;
			azimuth=(float)((pMsg->GetByteVal(WthrPolarVectors,i*4+3)*256+pMsg->GetByteVal(WthrPolarVectors,i*4+4))*3.14159/32768.0);
			curvector.pos1.setX((int)(start*sin(azimuth-azoffset))+m_Radar.x());
			curvector.pos1.setY((int)(start*cos(azimuth-azoffset))+m_Radar.y());
			curvector.pos2.setX((int)(start*sin(azimuth+azoffset))+m_Radar.x());
			curvector.pos2.setY((int)(start*cos(azimuth+azoffset))+m_Radar.y());
			curvector.pos3.setX((int)(end*sin(azimuth+azoffset))+m_Radar.x());
			curvector.pos3.setY((int)(end*cos(azimuth+azoffset))+m_Radar.y());
			curvector.pos4.setX((int)(end*sin(azimuth-azoffset))+m_Radar.x());
			curvector.pos4.setY((int)(end*cos(azimuth-azoffset))+m_Radar.y());
			m_VectorTable[Recpt].VectorTable.Add(curvector);
		}
								
	}
	//The scale of the received weather info is retrieved
	if (pMsg->IsFieldPresent(WthrScale))
	{
		tmp=pMsg->GetByteVal(WthrScale,0);
		m_Scale =(int) pow(2,(-2+((tmp&0xF8)>>3))); 
		
	}
	//If the update is complete
	if (pMsg->IsFieldPresent(WthrTotal))
	{
		total=pMsg->GetUIntVal(WthrTotal);
		if (total==m_VectorTable[Recpt].VectorTable.GetSize())
		{
			//The currently displayed weather data is removed and the 
			//newly recieved is applied
			if (m_CurActive!=-1)
				m_VectorTable[m_CurActive].VectorTable.RemoveAll();
			m_CurActive=Recpt;
			//Update the display
			for (int i=0;i<m_ElemTable.GetSize();i++)
				m_ElemTable[i]->OnUpdate();
		}

	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the weather information is displayed or not.
///
///  RETURNS: 
///  bool, whether the weather information is displayed or not
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CMeteoData::IsMeteoDisplayed()
{
	return m_IsDisplayed;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the weather display status to the one put in argument.
///  Argument : bool, whether the weather must be displayed or not
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CMeteoData::DisplayMeteo(bool display)
{
	if (m_IsDisplayed!=display)
	{
		m_IsDisplayed=display;
		//Display or remove from display the weather info
		for (int i=0;i<m_ElemTable.GetSize();i++)
			m_ElemTable[i]->OnUpdate();
	}
}


//-----------------------------------------------------------------------------
//
/// \class CMeteoView
///  This class manages the display of the weather information retrieved from the 
///  associated CMeteoData class for one radar window.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
///  Argument : int, Sub layer in the Layer_3 (Underlayer) on which the weather 
///				information must be displayed
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CMeteoView::CMeteoView(int Prio)
{
	m_Prio = Prio;
	m_Type=MeteoView;
	m_BoundingRect=QRect();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CMeteoView&, CMeteoView object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CMeteoView::CMeteoView(CMeteoView& pMeteoView)
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CMeteoView::~CMeteoView()
{
	((CRwdView*)m_pView)->InvalidateRect(&m_BoundingRect);	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions associates the weather presentation class to the
///  radar window on which it will be displayed and to the CMeteData class that will
///  provide the settings.
///  Argument 1 : LPVOID, associated class
///  Argument 2 : CView*, associated radar window
///  Argument 3 : CDisplayElement*, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CMeteoView::Init(LPVOID pData,CView *pView,CDisplayElement* )
{
	m_pView = pView;
	m_pMeteo = (CMeteoData*)pData;
	m_pMeteo->Init();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions retrieves the weather settings from the associated
///  CMeteoData class before the display is updated (OnDraw function is called). 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CMeteoView::OnUpdate()
{
	CMeteoData::m_VectorElem curvector;
	m_VectorTable.RemoveAll();
	if (m_pMeteo->m_CurActive!=-1)
	{
		for (int i=0;i<m_pMeteo->m_VectorTable[m_pMeteo->m_CurActive].VectorTable.GetSize();i++)
		{
			curvector=m_pMeteo->m_VectorTable[m_pMeteo->m_CurActive].VectorTable[i];
			((CRwdView*)m_pView)->LPtoDP(&curvector.pos1);
			((CRwdView*)m_pView)->LPtoDP(&curvector.pos2);
			((CRwdView*)m_pView)->LPtoDP(&curvector.pos3);
			((CRwdView*)m_pView)->LPtoDP(&curvector.pos4);
			m_VectorTable.Add(curvector);
		}
	}
	//The whole screen needs to be updated
	((CRwdView*)m_pView)->InvalidateRect(NULL);	

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions draws the weather presentation. 
///  Argument 1 : QPainter*, painting device
///  Argument 2 : int, drawn layer number
///  Argument 3 : int, drawn sub layer number
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsWet010, CIAsdFctRwsWet015, CIAsdFctRwsWet035
//
//-----------------------------------------------------------------------------
void CMeteoView::OnDraw(QPainter* pDC,int prio)
{
	QBrush brush(QColor(0,0,0),Qt::CustomPattern);
	uchar Patt[8]={0x88,0x00,0x22,0x00,0x88,0x00,0x22,0x00};
	int curlevel=-1;
	//Check if the currently drawn layer and sublayer are correct
	if ((m_pMeteo->m_IsDisplayed) && (prio==m_Prio))
	{
		QBitmap bmp(8,8,&Patt[0]);
		brush.setPixmap(bmp);
		pDC->setBackgroundColor(QColor(0,0,0));
		QRect rect;
		rect=m_pView->rect();
		pDC->setBrush(brush);
		//Each vector is displayed with the color corresponding to its level
		for (int i=0;i<m_VectorTable.GetSize();i++)
		{
			if (curlevel!=m_VectorTable[i].Level)
			{
				curlevel=m_VectorTable[i].Level;
				pDC->setPen(Qt::NoPen);
				brush.setColor(CColorMngr::GetColor(m_pMeteo->m_ColorLevelNb[curlevel]));
				pDC->setBrush(brush);
			}
			QPointArray pnttab(4);
			pnttab[0]=m_VectorTable[i].pos1;
			pnttab[1]=m_VectorTable[i].pos2;
			pnttab[2]=m_VectorTable[i].pos3;
			pnttab[3]=m_VectorTable[i].pos4;
			pDC->drawPolygon(pnttab,FALSE,0,4);
		}
	}
}


//-----------------------------------------------------------------------------
//
/// \class CWeatherMngr
///  This class manages the reception of the weather information from the weather
///  radar service, and manages the radar windows for which weather info must be
///  displayed.
//
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
///  The Sub layer in the Layer_3 (Underlayer) on which the weather information
///  must be displayed is set.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CWeatherMngr::CWeatherMngr() : m_WthrPrio(10)
{
	m_IsFailed=true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CWeatherMngr&, CWeatherMngr object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CWeatherMngr::CWeatherMngr(CWeatherMngr& pWeatherMngr)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CWeatherMngr::~CWeatherMngr()
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called in order to retrieve the offline defined information
///  about the weather radar service. 
///  Argument 1 : CIniFile&, file to be read
///  Argument 2 : CInterfaceList&, list of connection interfaces
///  Argument 3 : QString&, error message to be logged if an info is missing or is wrong
/// 
///  RETURNS: 
///  bool, whether the information is correct or not
///
///  REQUIREMENT ID: CIAsdItfHwi005
//
//-----------------------------------------------------------------------------
bool CWeatherMngr::Read(CIniFile& IniFile, CInterfaceList& itfs, QString& Error)
{

	CString KeyRead;
	CString KeyName;
	QString CurArg;

	int i;
	bool found = false;
	//Initialization of the weather radar service
	for (i='A'; i <= 'Z'; i++)
	{
		m_pServers[i - 'A'] = NULL;
		KeyName.sprintf("%c",i);
		int len=IniFile.GetIniProfileString("WEATHER_SERVICE",KeyName,KeyRead);
		if (len)
		{
			int nbarg=KeyRead.GetNbrOfArgument('|');
			if (nbarg==8)
			{
				CAsterixSource* tmpRcvSocket = new CAsterixSource;
				//Identifier
				tmpRcvSocket->m_Serid= i;
				//name
				KeyRead.GetArgumentNb(1,'|',CurArg);
				tmpRcvSocket->m_Name=CurArg;
				//sic number
				KeyRead.GetArgumentNb(2,'|',CurArg);
				CurArg = CurArg.stripWhiteSpace();
				m_Sic = atoi(CurArg.ascii());
				//sac number
				KeyRead.GetArgumentNb(3,'|',CurArg);
				CurArg = CurArg.stripWhiteSpace();
				m_Sac = atoi(CurArg.ascii());
				//rotation period
				KeyRead.GetArgumentNb(4,'|',CurArg);
				CurArg = CurArg.stripWhiteSpace();
				m_Period = atof(CurArg.ascii());
				//parameter not used for now
				KeyRead.GetArgumentNb(5,'|',CurArg);
				CurArg = CurArg.stripWhiteSpace();
				//IP adress
				KeyRead.GetArgumentNb(6,'|',CurArg);
				CurArg = CurArg.stripWhiteSpace();
				int ok=DecodeAddr(tmpRcvSocket->m_Addr, CurArg, "weather radar " + tmpRcvSocket->m_Name, Error);
				if (!ok) 
					return false;
				//Port number
				KeyRead.GetArgumentNb(7,'|',CurArg);
				CurArg = CurArg.stripWhiteSpace();
				ok=DecodePort(tmpRcvSocket->m_Port, CurArg, "weather radar " + tmpRcvSocket->m_Name, Error);
				if (!ok)
					return false;
				//Interface description
				KeyRead.GetArgumentNb(8,'|',CurArg);
				CurArg = CurArg.stripWhiteSpace();
				tmpRcvSocket->m_pInterface=itfs.Get(CurArg);
				tmpRcvSocket->m_TimeOut = m_Period*2.5;
				if (!tmpRcvSocket->m_pInterface)
				{
					Error = "Interface not found for weather source ";
					return false;
				}
				found = true;
				m_pServers[i - 'A'] = tmpRcvSocket;
			}
			else
			{
				Error = "Bad number of argument for weather source "+ KeyName;
				return false;
			}
		}
	}
	if (!found)
	{
		Error = "No definition found for weather source ";
		return false;
	}

	//Sets the display sub-layer 
	QString sWthrPrio;
	int len = IniFile.GetIniProfileString("RDRWNDW_PRIO","WTHR_PRIO", sWthrPrio);
	if (len > 0)
	{
		int tmpWthrPrio = atoi(sWthrPrio);
		if (tmpWthrPrio >= 1 && tmpWthrPrio <= 100)
			m_WthrPrio = tmpWthrPrio;
	}
	else
	{
		Error="No WTHR_PRIO found in version.ini [RDRWNDW_PRIO]. Stopping ASD.";
		return false;
	}

	//Sets the weather radar service to be used according to the sectorization
	found=false;
	Secto_WthRdServ tmpSectoSocket;
	for (int lev=1;;lev++)
	{
		KeyName.sprintf("WRS%d",lev);
		IniFile.GetIniProfileString("WEATHER_RADAR_SECTO",KeyName,KeyRead);
		if (!KeyRead.length())
			break;
		if (KeyRead.GetNbrOfArgument('|')==2)
		{
			found=true;
			KeyRead.GetArgumentNb(1,'|',tmpSectoSocket.Sector);
			tmpSectoSocket.Sector.stripWhiteSpace();
			KeyRead.GetArgumentNb(2,'|',tmpSectoSocket.Id);
			tmpSectoSocket.Id.stripWhiteSpace();
			m_SectoSocketTable.Add(tmpSectoSocket);
		}
	}
	if (!found)
	{
		Error = "No definition found for weather source and sector link";
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to initialize the reception socket list  
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CWeatherMngr::Init()
{
	m_iCurrentSocket=-1;
	for (int i='A'; i <= 'Z'; i++)
		if (m_pServers[i - 'A'])
			m_pServers[i - 'A']->SetMngr(this);
	OpenCurrentWeatherSocket();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to connect the reception socket corresponding to 
///  the CWP current sectorization  
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsWrs005
//
//-----------------------------------------------------------------------------
void CWeatherMngr::OpenCurrentWeatherSocket()
{
	//Former socket is closed
	if (m_iCurrentSocket!=-1)
	{
		m_pServers[m_iCurrentSocket]->Close();
		m_iCurrentSocket=-1;
	}
	

	CInfoMngr* pInfoMngr= CAsdApp::GetApp()->GetInfoMngr();
	QString strLogCsu="A";//pInfoMngr->GetLogicalCsu();
	QString WeatherRdServId;
	QString serid;
	for (int secto=0;secto<m_SectoSocketTable.GetSize();secto++)
	{
		if (m_SectoSocketTable[secto].Sector==strLogCsu)
			WeatherRdServId=m_SectoSocketTable[secto].Id;
	}
	//The primary CSU is retrieved
	for (int i='A'; i <= 'Z'; i++)
	{
		if (m_pServers[i - 'A'])
		{
			serid.sprintf("%c",m_pServers[i - 'A']->m_Serid);
			//the corresponding socket is opened
			if (WeatherRdServId==serid)
			{
				m_iCurrentSocket=i - 'A';
				m_pServers[i - 'A']->CreateRcvSocket();
			}
		}
	
	}
	
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function adds a new radar window in the list of radar windows that must
///  be able to display weather presentation.
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsWet035
//
//-----------------------------------------------------------------------------
void CWeatherMngr::AddView(CView* pView)
{
	m_ViewTable.Add(pView);
	//A new weather presentation class is created
	CMeteoView* pElem = new CMeteoView(m_WthrPrio);
	pElem->Init(&m_CurMeteo,pView);
	m_CurMeteo.m_ElemTable.Add(pElem);
	((CRwdView*)pView)->AddElement(pElem);
	pElem->OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function removes the radar window put in argument from the list of radar
///  windows that must be able to display weather presentation.
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CWeatherMngr::RemoveView(CView* pView)
{
	for (int view=0;view<m_ViewTable.GetSize();view++)
	{
		if (pView==m_ViewTable[view])
		{
			m_ViewTable.RemoveAt(view);
			delete m_CurMeteo.m_ElemTable[view];
			m_CurMeteo.m_ElemTable.RemoveAt(view);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions is called when an Asterix message is received from the weather
///  radar service.
///  Argument 1 : BYTE*, received message
///  Argument 2 : int, length of the received message
///  Argument 3 : sockaddr_in*, not used in this function
///  Argument 4 : int, not used in this function
/// $(B!F(B
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdItfComIra010, CIAsdItfComGen125, CIAsdFctSyn002
//
//-----------------------------------------------------------------------------
void CWeatherMngr::OnReceiveMsg(BYTE* pMsg,int Len,sockaddr_in* ,int )
{
	//A new Asterix message is defined
	CAsterixMsg* pAstMsg= new CAsterixMsg(pMsg,Len);
	while (pAstMsg->DecodeMessage())
	{
		switch (pAstMsg->GetCategory())
		{
		case 8:
			//Weather category
			//The message is decoded by the CMeteoData class
			m_CurMeteo.UpdateFromRadar(pAstMsg);
			//The status of the weather information is set to OK as soon as a message
			//of this category is received
			if (m_IsFailed)
			{
				m_IsFailed=false;
				CTopLevel::UpdateSystemInfo();
				CRwdView::UpdateAllStatusBar();
			}
			//Availability timers are reseted and newly started to detect a non reception of
			//messages
			if (m_Timer1Id)
				killTimer(m_Timer1Id);
			if (m_Timer2Id)
				killTimer(m_Timer2Id);
			m_Timer1Id=startTimer(CAsdApp::GetApp()->getWeatherDiscard()*1000);
			m_Timer2Id=startTimer(CAsdApp::GetApp()->getWeatherDiscard()*2*1000);
			break;
		default :
			break;
		}
	}
	// CDS BREAK NGT 2006/05/16 The rule : "empty brackets must be used for delete 
	//					when de-allocating arrays" cannot be used in this case. 
	delete pAstMsg;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time a started timer of the class reaches its
///  value. 
///  Argument : QTimerEvent*, The timer event.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsWet055, CIAsdFctRwsWrs016
//
//-----------------------------------------------------------------------------
void CWeatherMngr::timerEvent(QTimerEvent* e)
{
	if (e->timerId()==m_Timer1Id)
	{
		//if no weather message has been received during 30s (Cp_Asd060), "	the "Weather" menu
		//item of the Top Level Menu "Airsitu" pull-down menu is grayed out and the 
		//status bar and system information must be updated
		m_IsFailed=true;
		CTopLevel::UpdateSystemInfo();
		CRwdView::UpdateAllStatusBar();
		killTimer(m_Timer1Id);
		m_Timer1Id=0;
	}
	if (e->timerId()==m_Timer2Id)
	{
		//if no weather message has been received during 60s (Cp_Asd665)
		//the weather presentation, if displayed is removed from display
		if (m_CurMeteo.m_CurActive!=-1)
			m_CurMeteo.m_VectorTable[m_CurMeteo.m_CurActive].VectorTable.RemoveAll();
		for (int i=0;i<m_CurMeteo.m_ElemTable.GetSize();i++)
			m_CurMeteo.m_ElemTable[i]->OnUpdate();
		killTimer(m_Timer2Id);
		m_Timer2Id=0;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the weather information is displayed or not 
/// 
///  RETURNS: 
///  bool, whether the weather information is displayed or not
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CWeatherMngr::IsWeatherOn()
{
	return m_CurMeteo.IsMeteoDisplayed();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the weather radar service is failed or not 
/// 
///  RETURNS: 
///  bool, whether the weather radar service is failed or not
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CWeatherMngr::IsWeatherFailed()
{
	return m_IsFailed;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the weather information display status to the one put in
///  argument.
///  Argument : bool, display status to be applied.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CWeatherMngr::DisplayWeather(bool on)
{
	m_CurMeteo.DisplayMeteo(on);
}



//-----------------------------------------------------------------------------
//
/// \class CRangeMark
///  This class manages the settings of a set of range markers.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CRangeMark::CRangeMark()
{
	for (int view=0;view<m_sViewTable.GetSize();view++)
	{
		//For each radar window a range marker presentation (CRangeMarkView object)
		//is created 
		CRangeMarkView* pElem = new CRangeMarkView();
		pElem->Init(this,m_sViewTable[view]);
		m_ElemTable.Add(pElem);
		((CRwdView*)m_sViewTable[view])->AddElement(pElem);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CRangeMark&, CRangeMark object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CRangeMark::CRangeMark(CRangeMark *pRangeMark)
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CRangeMark::~CRangeMark()
{
	//The m_sViewTable table (list of radar windows displaying range markers) is 
	//emptied
	for (int view=0;view<m_sViewTable.GetSize();view++)
	{
		((CRwdView*)m_sViewTable[view])->RemoveElement(m_ElemTable[view]);
		delete m_ElemTable[view];
	}
	//All the range marker presentations are destroyed
	for (int vect=0;vect<m_sRMarkTable.GetSize();vect++)
	{
		if (m_sRMarkTable[vect]==this)
		{
			m_sRMarkTable.RemoveAt(vect);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function adds a new radar window in the list of radar windows that must
///  be able to display a range marker instance.
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CRangeMark::AddView(CView* pView)
{
	m_sViewTable.Add(pView);
	for (int vect=0;vect<m_sRMarkTable.GetSize();vect++)
	{
		if (m_sRMarkTable[vect])
		{
			//A range marker presentation is created
			CRangeMarkView* pElem = new CRangeMarkView();
			pElem->Init(m_sRMarkTable[vect],pView);
			m_sRMarkTable[vect]->m_ElemTable.Add(pElem);
			((CRwdView*)pView)->AddElement(pElem);
			pElem->OnUpdate();
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function removes the radar window put in argument from the list of radar
///  windows that must be able to display range markers.
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CRangeMark::RemoveView(CView* pView)
{
	for (int view=0;view<m_sViewTable.GetSize();view++)
	{
		if (pView==m_sViewTable[view])
		{
			for (int vect=0;vect<m_sRMarkTable.GetSize();vect++)
			{
				//If the close radar window was displaying a range marker, it is
				//removed from display
				if (((CRwdView*)m_sViewTable[view])->IsElementDisplayed(m_sRMarkTable[vect]->m_ElemTable[view]))
					{
					//If the removed range marker is the last displayed, the tool is
					//canceled, i.e. if a new radar window is created it will not
					//display a range marker
						int i=CRangeMarkView::GetNbRangeMarker();
						if (i==1)
						{
							delete m_sRMarkTable[0];
							CRangeMarkView::SetNbRangeMarker(0);
						}
						//In the other case the object is destroyed and the number
						//of displayed range markers is decreased
						else if (i>1)
						{
							m_sRMarkTable[vect]->m_ElemTable.RemoveAt(view);
							i--;
							CRangeMarkView::SetNbRangeMarker(i);
						}
					}
				else
					m_sRMarkTable[vect]->m_ElemTable.RemoveAt(view);
			}
			m_sViewTable.RemoveAt(view);

			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to display a range marker in every opened radar 
///  window. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk010
//
//-----------------------------------------------------------------------------
void CRangeMark::Create()
{
	bool IsModifiable = true;
	if (!m_sRMarkTable.GetSize())
	{	
		//if no range marker was previously displayed, a new set of range markers
		//is defined
		for (int view=0;view<m_sViewTable.GetSize();view++)
			IsModifiable &= ((CRwdView*)m_sViewTable[view])->IsModifyNull();
		if (IsModifiable)
		{
			CRangeMark* NewRangeMark = NULL;
			NewRangeMark = new CRangeMark();
			m_sRMarkTable.Add(NewRangeMark);
			for (int view=0;view<m_sViewTable.GetSize();view++)
			{
				NewRangeMark->m_ElemTable[view]->OnUpdate();
			}
		}
	}
	else
	{
		//if a range marker was previously displayed, all the range markers are
		//destroyed
		delete m_sRMarkTable[0];
		CRangeMarkView::SetNbRangeMarker(0);
		//and a new set of range markers is defined
		for (int view=0;view<m_sViewTable.GetSize();view++)
			IsModifiable &= ((CRwdView*)m_sViewTable[view])->IsModifyNull();
		if (IsModifiable)
		{
			CRangeMark* NewRangeMark = NULL;
			NewRangeMark = new CRangeMark();
			m_sRMarkTable.Add(NewRangeMark);
			for (int view=0;view<m_sViewTable.GetSize();view++)
			{
				NewRangeMark->m_ElemTable[view]->OnUpdate();
			}
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether a range marker is being moved or not. 
/// 
///  RETURNS: 
///  bool, whether a range marker is being moved or not
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CRangeMark::IsRangeMarkInModif()
{
	for (int view=0;view<m_sViewTable.GetSize();view++)
	{
		for (int vect=0;vect<m_sRMarkTable.GetSize();vect++)
		{
			if (m_sRMarkTable[vect]->m_ElemTable[view]->IsMove())
				return true;
		}
	}
	return false;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the range marker presentation object associated to 
///  the view put in agurment. 
///  Argument : CView*, radar window
/// 
///  RETURNS: 
///  CRangeMarkView*, range marker displayed in the radar window
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CRangeMarkView* CRangeMark::GetCRangeMarkView(CView* pView)
{
	for (int view=0;view<m_sViewTable.GetSize();view++)
	{
		if (pView==m_sViewTable[view])
		{
			for (int vect=0;vect<m_sRMarkTable.GetSize();vect++)
			{
				if (((CRwdView*)m_sViewTable[view])->IsElementDisplayed(m_sRMarkTable[vect]->m_ElemTable[view]))
					return m_sRMarkTable[vect]->m_ElemTable[view];
			}
		}
	}
	return NULL;
}


//-----------------------------------------------------------------------------
//
/// \class CRangeMarkView
///  This class manages the display of a range marker instance.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CRangeMarkView::CRangeMarkView()
{
	m_BoundingRect=QRect();
	m_Select = false;
	m_OnMove=false;
	m_pView=NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CRangeMarkView&, CRangeMarkView object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CRangeMarkView::CRangeMarkView(CRangeMarkView& pRangeMarkView)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CRangeMarkView::~CRangeMarkView()
{
	QRect updRect = m_BoundingRect;
	((CRwdView*)m_pView)->InvalidateRect(&updRect);
	m_pView=NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  accessore
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CRangeMarkView::IsMove()
{
	return m_OnMove;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions draws the range marker. 
///  Argument 1 : QPainter*, painting device
///  Argument 2 : int, drawn layer number
///  Argument 3 : int, drawn sub layer number
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk015, CIAsdFctRwsTlsRmk020
//
//-----------------------------------------------------------------------------
void CRangeMarkView::OnDraw(QPainter* pDC,int prio)
{
	//Check if the layer and sub layer are correct
	if ((prio!=7))
		return;
	QPen pen;
	//retrieve the tool offline defined color
	int ColorNb=CColorMngr::GetColorNb("RW_TOOLS");
	if (m_Select)
		ColorNb=CColorMngr::GetColorNb("SELECTED_FRGRND");
	//Associates the color to the corresponding slider of the brightness control
	//window
	pen.setColor(CColorMngr::TransformColorNb(ColorNb,RangeMarkersCategory));
	pDC->setPen(pen);
	QPoint pnt,pnt1,pnt2,pntcenter;
	pntcenter=m_CenterPoint;
	((CAsdView*)m_pView)->DPtoLP(&pntcenter);

	//according to the orientation, the center and the length of the range marker
	//the two lines are drawn
	if (m_sOrientation)
	{
		pnt=pntcenter;
		pnt1=QPoint(pnt.x()-m_sRange*32,pnt.y());
		pnt2=QPoint(pnt.x(),pnt.y()-m_sRange*32);
	}
	else
	{
		pnt=pntcenter;
		pnt1=QPoint(pnt.x()+m_sRange*32,pnt.y());
		pnt2=QPoint(pnt.x(),pnt.y()+m_sRange*32);
	}
	((CAsdView*)m_pView)->LPtoDP(&pnt);
	((CAsdView*)m_pView)->LPtoDP(&pnt1);
	((CAsdView*)m_pView)->LPtoDP(&pnt2);
	pDC->drawLine(pnt,pnt1);
	pDC->drawLine(pnt,pnt2);
	//Draw the end of line marker
	pDC->drawLine(QPoint(pnt1.x(),pnt1.y()-4),QPoint(pnt1.x(),pnt1.y()+4));
	pDC->drawLine(QPoint(pnt2.x()-4,pnt2.y()),QPoint(pnt2.x()+4,pnt2.y()));
	//Draw the interline
	for (int i=m_sInterline;i<m_sRange;i+=m_sInterline)
	{
		if (m_sOrientation)
		{
			pnt=pntcenter;
			pnt1=QPoint(pnt.x()-i*32,pnt.y());
			pnt2=QPoint(pnt.x(),pnt.y()-i*32);
		}
		else
		{
			pnt=pntcenter;
			pnt1=QPoint(pnt.x()+i*32,pnt.y());
			pnt2=QPoint(pnt.x(),pnt.y()+i*32);
		}
		((CAsdView*)m_pView)->LPtoDP(&pnt);
		((CAsdView*)m_pView)->LPtoDP(&pnt1);
		((CAsdView*)m_pView)->LPtoDP(&pnt2);
		pDC->drawLine(QPoint(pnt1.x(),pnt1.y()-2),QPoint(pnt1.x(),pnt1.y()+2));
		pDC->drawLine(QPoint(pnt2.x()-2,pnt2.y()),QPoint(pnt2.x()+2,pnt2.y()));
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns an integer corresponding to the fact that the range 
///  marker must present the implicit focus or not.
///  Argument 1 : QPoint, position of the mouse pointer
///  Argument 2 : bool, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
int CRangeMarkView::GetPriority(QPoint point,bool )
{
	//If the range marker is moved it is selected
	if (m_OnMove)
		return PRIO_RANGEMARK_MOVE;
	//If the CWP is already in a tool creation or modification mode the range marker
	//must not present the implicit focus
	if (CRangeMark::IsRangeMarkInModif() || CTrkMiles::IsInCreation() 
		|| CVector::IsInCreation() || CNewRange::IsInCreation() || CRwdView::IsInViewModif())
		return 0;
	QPoint pnt,pnt1,pnt2,pntcenter;
	pntcenter=m_CenterPoint;
	((CAsdView*)m_pView)->DPtoLP(&pntcenter);

	//Check whether the mouse pointer is over the range marker
	if (m_sOrientation)
	{
		pnt=pntcenter;
		pnt1=QPoint(pnt.x()-m_sRange*32,pnt.y());
		pnt2=QPoint(pnt.x(),pnt.y()-m_sRange*32);
	}
	else
	{
		pnt=pntcenter;
		pnt1=QPoint(pnt.x()+m_sRange*32,pnt.y());
		pnt2=QPoint(pnt.x(),pnt.y()+m_sRange*32);
	}	
	((CAsdView*)m_pView)->LPtoDP(&pnt);
	((CAsdView*)m_pView)->LPtoDP(&pnt1);
	((CAsdView*)m_pView)->LPtoDP(&pnt2);
	QRect bound;
	QPoint vect1,vect2,vect3,vect4;
	int vectprod,dist,dist2;
	bound = QRect(pnt1,m_CenterPoint);
	bound=bound.normalize();
	bound.addCoords(-5,-5,5,5);
	if (bound.contains(point))
	{
		vect1 = pnt1-m_CenterPoint;
		vect2 = point - m_CenterPoint;
		dist = (vect1.x()*vect1.x())+(vect1.y()*vect1.y());
		vectprod = (vect1.x()*vect2.x())+(vect1.y()*vect2.y());
		vect3.setX( int(double(vect1.x())*double(vectprod)/double(dist)));
		vect3.setY( int(double(vect1.y())*double(vectprod)/double(dist)));
		vect4 = vect2-vect3;
		dist2 = vect4.x()*vect4.x()+vect4.y()*vect4.y();
		//A tolerance threshold is applied for the selection of a range marker
		if (dist2<=20)
		{
			return PRIO_RANGEMARK;
		}
	}
	bound = QRect(pnt2,m_CenterPoint);
	bound=bound.normalize();
	bound.addCoords(-5,-5,5,5);
	if (bound.contains(point))
	{
		vect1 = pnt2-m_CenterPoint;
		vect2 = point - m_CenterPoint;
		dist = (vect1.x()*vect1.x())+(vect1.y()*vect1.y());
		vectprod = (vect1.x()*vect2.x())+(vect1.y()*vect2.y());
		vect3.setX( int(double(vect1.x())*double(vectprod)/double(dist)));
		vect3.setY( int(double(vect1.y())*double(vectprod)/double(dist)));
		vect4 = vect2-vect3;
		dist2 = vect4.x()*vect4.x()+vect4.y()*vect4.y();
		//A tolerance threshold is applied for the selection of a range marker
		if (dist2<=20)
		{
			return PRIO_RANGEMARK;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selection status of the range marker to the one put in argument.
///  Argument : bool, Selection status to be applied
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CRangeMarkView::SelectElement(bool Select)
{
	m_Select = Select;
	OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the mouse pointer is moved while the range
///  marker is on the motion mode.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk025, CIAsdFctRwsTlsRmk027
//
//-----------------------------------------------------------------------------
void CRangeMarkView::OnMouseMove(UINT nFlags,QPoint point)
{
	//If the range marker is being moved
	if (m_OnMove)
	{
		QRect radarwndrect=m_pView->geometry();
		QPoint globalpnt=QCursor::pos();
		QRect statusbarrect=((CRwdView*)m_pView)->GetStatusBar()->rect();
		//If the mouse pointer leaves the radar window on which the motion mode has been 
		//invoked, the motion mode is canceled
		if (globalpnt.x()>=radarwndrect.x()+radarwndrect.width() || globalpnt.x()<=radarwndrect.x() || globalpnt.y()<=radarwndrect.y() || globalpnt.y()>=radarwndrect.y()+radarwndrect.height()-statusbarrect.height())
		{
			//the range marker remains displayed at its last position within the radar window
			//before the mouse pointer left it
			m_OnMove=false;
			((CRwdView*)m_pView)->SetModifyElement(NULL);
			SelectElement(false);
		}
		else
		{
			//The position of the range marker is updated
			m_CenterPoint=point+m_GripPoint;
			OnUpdate();
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click AB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk025
//
//-----------------------------------------------------------------------------
void CRangeMarkView::OnLButtonDown(UINT nFlags,QPoint point)
{
	//If the range marker was being moved, the motion mode is canceled and the 
	//current position of the range marker becomes its new position
	if (m_OnMove)
	{
		m_OnMove=false;
		m_CenterPoint=point+m_GripPoint;
		((CRwdView*)m_pView)->SetModifyElement(NULL);
	}
	else
	{
		//If the range marker has the implicit focus, the motion mode is initialized
		//the range marker follows the mouse pointer
		if (m_Select)
		{
			bool IsModifiable = true;
			for (int view=0;view<CRangeMark::m_sViewTable.GetSize();view++)
				IsModifiable &= ((CRwdView*)CRangeMark::m_sViewTable[view])->IsModifyNull();
			if (IsModifiable)
			{
				((CRwdView*)m_pView)->SetModifyElement(this);
				m_OnMove=true;
				m_GripPoint=m_CenterPoint-point;
			}
	
		}
	}
	OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click WB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk030
//
//-----------------------------------------------------------------------------
void CRangeMarkView::OnMButtonDown(UINT nFlags,QPoint pnt)
{
	int i=GetNbRangeMarker();
	//If the range marker has the implicit focus and is not being moved, the instance
	//is removed from display
	if (m_Select && !m_OnMove)
	{
		//If the range marker was the last displayed, the tool is canceled
		if (i==1)
		{
			delete CRangeMark::m_sRMarkTable[0];
			CRangeMarkView::SetNbRangeMarker(0);
		}
		//In the other case the instance is removed from display
		else
		{
			bool IsModifiable = true;
			for (int view=0;view<CRangeMark::m_sViewTable.GetSize();view++)
			IsModifiable &= ((CRwdView*)CRangeMark::m_sViewTable[view])->IsModifyNull();
			if (IsModifiable)
			{
				((CRwdView*)m_pView)->RemoveElement(this);
				int i=GetNbRangeMarker();
				i--;
				SetNbRangeMarker(i);
			}
		}	
	}
	else
	{
		//If the range marker was being moved the motion mode is canceled and it
		//remains displayed at it current position
		if (m_OnMove)
		{
			m_OnMove=false;
			m_CenterPoint=pnt+m_GripPoint;
			((CRwdView*)m_pView)->SetModifyElement(NULL);
			OnUpdate();
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to set the number of displayed range markers across
///  all available radar windows.
///  Argument : int, new number of range markers
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CRangeMarkView::SetNbRangeMarker(int nb)
{
	m_sNbRangeMarker=nb;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns the number of displayed range markers across
///  all available radar windows.
///
///  RETURNS: 
///  int, the number of displayed range markers
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
int CRangeMarkView::GetNbRangeMarker()
{
	return m_sNbRangeMarker;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions calculates the part of the screen that needs to be updated when
///  the range marker will be drawn. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CRangeMarkView::OnUpdate()
{
	QRect UpdRect = m_BoundingRect;
	QPoint pnt1,pnt2,pntcenter;
	pntcenter=m_CenterPoint;
	((CAsdView*)m_pView)->DPtoLP(&pntcenter);
	
	if (m_sOrientation)
	{
		pnt1=QPoint(pntcenter.x()-m_sRange*32,pntcenter.y());
		pnt2=QPoint(pntcenter.x(),pntcenter.y()-m_sRange*32);
	}
	else
	{
		pnt1=QPoint(pntcenter.x()+m_sRange*32,pntcenter.y());
		pnt2=QPoint(pntcenter.x(),pntcenter.y()+m_sRange*32);
	}
	((CAsdView*)m_pView)->LPtoDP(&pnt1);
	((CAsdView*)m_pView)->LPtoDP(&pnt2);

	m_BoundingRect=QRect(pnt1,pnt2);
	m_BoundingRect=m_BoundingRect.normalize();
	m_BoundingRect.addCoords(-5,-5,5,5);
	UpdRect|=m_BoundingRect;
	((CRwdView*)m_pView)->InvalidateRect(&UpdRect);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions initializes the settings of the range marker instance. 
///  Argument 1 : LPVOID, parent object
///  Argument 2 : CView*, radar window on which the range marker must be displayed
///  Argument 3 : CDisplayElement*, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CRangeMarkView::Init(LPVOID pData,CView *pView,CDisplayElement* )
{
	QPoint pnt;
	//The number of instance is incremented
	int i=GetNbRangeMarker();
	i++;
	SetNbRangeMarker(i);
	
	//to retrieve offline defined offsets for Range markers initial display
	int offhbl=CAsdApp::GetApp()->getOffsetHBL();
	int offvbl=CAsdApp::GetApp()->getOffsetVBL();
	int offhtr=CAsdApp::GetApp()->getOffsetHTR();
	int offvtr=CAsdApp::GetApp()->getOffsetVTR();

	//The associated view is set 
	m_pView = pView;

	m_pRangeMark = (CRangeMark*)pData;
	QRect rect=m_pView->rect();

	//The center is position according to the orientation and the offsets
	//offline defined
	if (!GetOrientation())
	{
		//The status bar height is to be taken into account
		QRect rectstb=((CRwdView*)m_pView)->GetStatusBar()->rect();
		pnt=QPoint(offhbl,rect.height()-(rectstb.height()+offvbl)); 
		m_CenterPoint=pnt;
	}
	else
	{
		pnt=QPoint(rect.width()-offhtr,offvtr);
		m_CenterPoint=pnt;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions positions back the range marker at its initial position (offline
///  defined). 
///  Argument : QRect, rect of the associated radar window
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CRangeMarkView::SetInitialPosition(QRect rect)
{
	QPoint pnt;
	//to retrieve offline defined offsets for Range markers initial display
	int offhbl=CAsdApp::GetApp()->getOffsetHBL();
	int offvbl=CAsdApp::GetApp()->getOffsetVBL();
	int offhtr=CAsdApp::GetApp()->getOffsetHTR();
	int offvtr=CAsdApp::GetApp()->getOffsetVTR();

	if (!GetOrientation())
	{
		QRect rectstb=((CRwdView*)m_pView)->GetStatusBar()->rect();
		pnt=QPoint(offhbl,rect.height()-(rectstb.height()+offvbl)); 
		m_CenterPoint=pnt;
	}
	else
	{
		pnt=QPoint(rect.width()-offhtr,offvtr);
		m_CenterPoint=pnt;
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions sets the range to be applied to every range marker. 
///  Argument : int, category of the range marker (3NM or 5NM)
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CRangeMarkView::SetRange(int range)
{
	m_sRange = range;
	for (int view=0;view<CRangeMark::m_sViewTable.GetSize();view++)
	{
		((CRwdView*)CRangeMark::m_sViewTable[view])->OnUpdate(NULL,0,NULL);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions returns the category of the range markers (3NM or 5NM). 
/// 
///  RETURNS: 
///  int, category of the range markers (3NM or 5NM)
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
int CRangeMarkView::GetRange()
{
	return m_sRange;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions sets the orientation to be applied to every range marker. 
///  Argument : bool, orientation of the range marker (BL or TR)
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CRangeMarkView::SetOrientation(bool orientation)
{
	m_sOrientation = orientation;
	for (int view=0;view<CRangeMark::m_sViewTable.GetSize();view++)
	{
		((CRwdView*)CRangeMark::m_sViewTable[view])->OnUpdate(NULL,0,NULL);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions returns the orientation of the range markers (BL or TR). 
/// 
///  RETURNS: 
///  int, orientation of the range markers 
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CRangeMarkView::GetOrientation()
{
	return m_sOrientation;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the Escape key press action.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk030
//
//-----------------------------------------------------------------------------
void CRangeMarkView::OnKeyDown()
{
	//If the range marker was on motion mode, the mode is canceled and the range
	//marker remains at its current position
	if (m_OnMove)
	{
		m_OnMove=false;
		((CRwdView*)m_pView)->SetModifyElement(NULL);
	}
	OnUpdate();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to cancel the resize effect of attached borders.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk021
//
//-----------------------------------------------------------------------------
void CRangeMarkView::SetResizeOffset(QPoint offset)
{
	m_CenterPoint=m_CenterPoint+offset;
	OnUpdate();
}




//-----------------------------------------------------------------------------
//
/// \class CTrkMiles
///  This class manages the settings of Track Mile lines.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
///  Argument : CView*, view on which the Track Mile line must be displayed
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml040
//
//-----------------------------------------------------------------------------
CTrkMiles::CTrkMiles(CView* pView)
{
	m_pTrack = NULL;
	//The object is associated to a CTrkMilesView object that will be its graphical
	//representation
	CTrkMilesView* pElem = new CTrkMilesView();
	pElem->Init(this,pView);
	m_pElem=pElem;
	((CRwdView*)pView)->AddElement(pElem);
	((CRwdView*)pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPCrosshair));
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CTrkMiles&, CTrkMiles object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTrkMiles::CTrkMiles(CTrkMiles& pTrkMiles)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTrkMiles::~CTrkMiles()
{
	//The instance is removed from the m_sTrkMilesTable table
	for (int trkm=0;trkm<m_sTrkMilesTable.GetSize();trkm++)
	{
		if (m_sTrkMilesTable[trkm]==this)
		{
			m_sTrkMilesTable.RemoveAt(trkm);
			break;
		}
	}
	if (m_State!=created)
	{
		((CRwdView*)m_pElem->GetView())->SetModifyElement(NULL);
	}
	//If the instance was in creation mode, the m_psCurTrkMiles pointer is freed
	if (m_psCurTrkMiles==this)
		m_psCurTrkMiles=NULL;
	if (m_pTrack)
		m_pTrack->AssociateTrkMiles(this,false);
	//The TrackMile timer is canceled if no more Track mile line is displayed
	if (!m_sTrkMilesTable.GetSize())
		killTimer(m_TrkMileTimer);
	((CRwdView*)m_pElem->GetView())->RemoveElement(m_pElem);
	delete m_pElem;

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to start the creation of a Track Mile line.
///  Argument 1 : CView*, radar window on which the line will be created 
///  Argument 2 : CTrack*, track for which the tool is invoked
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTrkMiles::Create(CView* pView,CTrack* pTrack)
{
	bool IsModifiable = true;
	//The track mile line being modified or created is identified by the m_psCurTrkMiles
	//static pointer
	//If the pointer is null, no instance is under creation mode and the 
	//creation mode can be started
	if (!m_psCurTrkMiles)
	{
		IsModifiable &= ((CRwdView*)pView)->IsModifyNull();
		if (IsModifiable)
		{
			m_psCurTrkMiles= new CTrkMiles(pView);
			m_sTrkMilesTable.Add(m_psCurTrkMiles);
			
			((CRwdView*)pView)->SetModifyElement(m_psCurTrkMiles->m_pElem);

			//The track is associated to the tool in order to follow the 
			//track motion
			m_psCurTrkMiles->m_pTrack=pTrack;
			m_psCurTrkMiles->m_pTrack->AssociateTrkMiles(m_psCurTrkMiles,true);
			m_psCurTrkMiles->m_State=modify;
			//Two points are added in order to define a line to be drawn
			//the last point will follow the mouse pointer motion until the
			//creation mode is canceled
			m_psCurTrkMiles->m_PointTable.Add(pTrack->GetPos());
			m_psCurTrkMiles->m_PointTable.Add(pTrack->GetPos());
		}
	}
	else
	{
		//If the pointer is not null it must be set to null
		if (m_psCurTrkMiles->m_State!=created)
			((CRwdView*)pView)->SetModifyElement(NULL);
		delete m_psCurTrkMiles->m_pElem;
		m_psCurTrkMiles = NULL;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns whether the CWP is under TML creation mode or not.
/// 
///  RETURNS: 
///  bool, whether the CWP is on TML creation mode or not
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
bool CTrkMiles::IsInCreation()
{
	//The track mile line being created is identified by the m_psCurTrkMiles
	//static pointer
	return (m_psCurTrkMiles!=NULL);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function manages the addition of a new point in the track mile lines.
///  Argument 1 : QPoint, position of the mouse pointer
///  Argument 2 : bool, whether a new point must be added or not
///  Argument 3 : bool, whether the creation mode must end or not
///  Argument 4 : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml015, CIAsdFctRwsTlsTml020, CIAsdFctRwsTlsTml030
//
//-----------------------------------------------------------------------------
void CTrkMiles::AddPoint(QPoint point,bool next,bool created,CView* pView)
{
	//If the creation mode is still activated
	if (!created)
	{
		//the last point of the table is updated with the mouse pointer position
		m_PointTable[m_PointTable.GetUpperBound()]=point;
		//A new leg is created and the updated last point becomes fix
		if (next)
			m_PointTable.Add(point);
	}
	else
	{
		//The track mile lines creation is ended
		m_State=CTrkMiles::created;
		((CRwdView*)pView)->SetModifyElement(NULL);
		//The length is calculated to be displayed in the label
		m_Distance=0;
		for (int i=1;i<m_PointTable.GetSize();i++)
			m_Distance+=sqrt((m_PointTable[i-1].x()-m_PointTable[i].x())*(m_PointTable[i-1].x()-m_PointTable[i].x())+(m_PointTable[i-1].y()-m_PointTable[i].y())*(m_PointTable[i-1].y()-m_PointTable[i].y()));
		//the auto deletion timer is launched
		m_FinishTime=QDateTime::currentDateTime();
		m_FinishTime=m_FinishTime.addSecs(TrkmTimeout);
		m_TrkMileTimer=startTimer(1000);
		m_psCurTrkMiles=NULL;
	}
	m_pElem->OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called from the CTrack class each time the position of the
///  track attached to track mile line is updated. 
///  Argument 1 : CTrack*, the updated track
///  Argument 2 : bool, whether the track is deleted or not 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml036
//
//-----------------------------------------------------------------------------
void CTrkMiles::UpdateTrk(CTrack* pTrack,bool update)
{
	if (update)
	{
		//The position of the tarck is updated
		m_PointTable[0]=pTrack->GetPos();
		m_Distance=0;
		//the length of the track mile line is updated
		for (int i=1;i<m_PointTable.GetSize();i++)
		{
			m_Distance+=sqrt((m_PointTable[i-1].x()-m_PointTable[i].x())*(m_PointTable[i-1].x()-m_PointTable[i].x())+(m_PointTable[i-1].y()-m_PointTable[i].y())*(m_PointTable[i-1].y()-m_PointTable[i].y()));
		}
		m_pElem->OnUpdate();
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called each time a started timer of the class reaches its
///  value. 
///  Argument : QTimerEvent*, The timer event.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml055
//
//-----------------------------------------------------------------------------
void CTrkMiles::timerEvent(QTimerEvent* e)
{
	if (e->timerId()==m_TrkMileTimer)
	{
		QDateTime curtime=QDateTime::currentDateTime();
		//All the track mile lines for which the deletion time is reached are removed
		//from display
		for (int trkm=0;trkm<m_sTrkMilesTable.GetSize();trkm++)
		{
			if ((m_sTrkMilesTable[trkm]) && (m_sTrkMilesTable[trkm]->m_State==created) && ((m_sTrkMilesTable[trkm])->m_FinishTime<curtime))
				delete m_sTrkMilesTable[trkm];
		}
		
	}
	//If no more Track mile lines are displayed, the deletion timer is stopped
	if (!m_sTrkMilesTable.GetSize())
		killTimer(m_TrkMileTimer);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function removes the track mile lines associated to the radar window 
///  put in argument
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTrkMiles::RemoveView(CView* pView)
{
  for (int trkm=0;trkm<m_sTrkMilesTable.GetSize();trkm++)
    {
      if ((m_sTrkMilesTable[trkm]) && (m_sTrkMilesTable[trkm]->m_pElem->GetView() ==pView))
			delete m_sTrkMilesTable[trkm];
    }
}


//-----------------------------------------------------------------------------
//
/// \class CTrkMilesView
///  This class manages the display of a track mile line instance.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTrkMilesView::CTrkMilesView()
{
	m_BoundingRect=QRect();
	m_Select = false;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CTrkMilesView&, CTrkMilesView object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTrkMilesView::CTrkMilesView(CTrkMilesView& pTrkMilesView)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTrkMilesView::~CTrkMilesView()
{
	QRect updRect = m_BoundingRect;
	((CRwdView*)m_pView)->InvalidateRect(&updRect);
	
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  accessor
/// 
///  RETURNS: 
///  view
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CView* CTrkMilesView::GetView()
{
	return m_pView;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions draws the Range and Bearing line. 
///  Argument 1 : QPainter*, painting device
///  Argument 2 : int, drawn layer number
///  Argument 3 : int not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml010, CIAsdFctRwsTlsTml035, CIAsdFctRwsTlsTml045,
///					CIAsdFctRwsTlsTml050
//
//-----------------------------------------------------------------------------
void CTrkMilesView::OnDraw(QPainter* pDC,int Prio)
{
	int colornb;
	QString text;
	int LabelOffset=15;
	QPen pen;
	//If the layer is correct, the lines can be drawn
	if (Prio==0)
	  return;
	if ((m_pTrkMiles->m_State==CTrkMiles::created) || (m_pTrkMiles->m_State==CTrkMiles::modify))
	{
		colornb=CColorMngr::GetColorNb("RW_TOOLS");
		if (m_Select)
		  colornb=CColorMngr::GetColorNb("SELECTED_FRGRND");
		pen.setColor(CColorMngr::TransformColorNb(colornb,AsdToolsCategory));
		pDC->setPen(pen);
		//each line is drawn
		for (int i=1;i<m_PointTable.GetSize();i++)
			pDC->drawLine(m_PointTable[i-1],m_PointTable[i]);
		//If the track mile line is created, the label must be displayed
		if (m_pTrkMiles->m_State==CTrkMiles::created)
		{
			QFont fnt=CFontMngr::GetFontByName("RNGBEARLINE_LABEL");
			pDC->setFont(fnt);
			text.sprintf("%2.1f",m_pTrkMiles->m_Distance/32.0);
			QSize size=pDC->boundingRect(0,0,0,0,Qt::AlignLeft,text).size();
			if ((m_PointTable[m_PointTable.GetUpperBound()].x())<(m_PointTable[(m_PointTable.GetUpperBound()-1)].x()))
				pDC->drawText(m_PointTable[m_PointTable.GetUpperBound()].x()-LabelOffset-size.width(),m_PointTable[m_PointTable.GetUpperBound()].y(),text);
			else
				pDC->drawText(m_PointTable[m_PointTable.GetUpperBound()].x()+LabelOffset,m_PointTable[m_PointTable.GetUpperBound()].y(),text);
		}
	}

}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions retrieves the Track Mile line settings to calculate the part
///  of the screen that needs to be updated when the display is updated (OnDraw
///  function is called). 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml010
//
//-----------------------------------------------------------------------------
void CTrkMilesView::OnUpdate()
{
	QPainter pDC(m_pView);
	QRect oldRect=m_BoundingRect;
	QString text;
	m_PointTable.RemoveAll();
	//The rect of the lines is added
	for (int i=0;i<m_pTrkMiles->m_PointTable.GetSize();i++)
	{
		m_PointTable.Add(m_pTrkMiles->m_PointTable[i]);
		((CAsdView*)m_pView)->LPtoDP(&m_PointTable[i]);
		if (i>0)
		{
			QRect rect(m_PointTable[i-1],m_PointTable[i]);
			rect=rect.normalize();
			m_BoundingRect|=rect;
		}
	}
	//If the line is created the label rect is added
	if (m_pTrkMiles->m_State==CTrkMiles::created)
	{
		QFont fnt=CFontMngr::GetFontByName("RNGBEARLINE_LABEL");
		pDC.setFont(fnt);
		text.sprintf("%2.1f",m_pTrkMiles->m_Distance/32.0);
		QSize size=pDC.boundingRect(0,0,0,0,Qt::AlignLeft,text).size();
		
		if ((m_PointTable[m_PointTable.GetUpperBound()].x())<(m_PointTable[(m_PointTable.GetUpperBound()-1)].x()))
		  m_TextRect=QRect(QPoint(m_PointTable[m_PointTable.GetUpperBound()].x()-5-size.width(),m_PointTable[m_PointTable.GetUpperBound()].y()-size.height()),size);
		else
		  m_TextRect=QRect(QPoint(m_PointTable[m_PointTable.GetUpperBound()].x()+5,m_PointTable[m_PointTable.GetUpperBound()].y()-5-size.height()),size);
		m_TextRect=m_TextRect.normalize();
	}
	m_BoundingRect|=m_TextRect;
	m_BoundingRect=m_BoundingRect.normalize();
	m_BoundingRect.addCoords(-6,-6,6,6);
	oldRect|=m_BoundingRect;
	oldRect=oldRect.normalize();
	((CRwdView*)m_pView)->InvalidateRect(&oldRect);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions associates the Track Mile line presentation class to the
///  radar window on which it will be displayed and to the CTrkMiles class that will
///  provide the settings.
///  Argument 1 : LPVOID, associated class
///  Argument 2 : CView*, associated radar window
///  Argument 3 : CDisplayElement*, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTrkMilesView::Init(LPVOID pData,CView *pView,CDisplayElement* )
{
	m_pView = pView;
	m_pTrkMiles = (CTrkMiles*)pData;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns an integer corresponding to the fact that the Track
///  Mile line must present the implicit focus or not.
///  Argument 1 : QPoint, position of the mouse pointer
///  Argument 2 : bool, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
int CTrkMilesView::GetPriority(QPoint point,bool )
{
	QRect bound;
	QPoint vect1,vect2,vect3,vect4;
	int vectprod,dist,dist2;
	//If the instance is created and if the mouse pointer is within the part of the screen
	//where the track mile line is displayed
	if ((m_pTrkMiles->m_State==CTrkMiles::created) && (m_BoundingRect.contains(point)))
	{
		//Check if the mouse pointer is over the label
		if (m_TextRect.contains(point))
			return PRIO_TRKMILES;
		//Check if the mouse pointer is over a leg with a tolerance threshold
		for (int i=1;i<m_pTrkMiles->m_PointTable.GetSize();i++)
		{
			bound = QRect(m_PointTable[i-1],m_PointTable[i]);
			bound=bound.normalize();
			bound.addCoords(-5,-5,5,5);
			if (bound.contains(point))
			{
				vect1 = m_PointTable[i]-m_PointTable[i-1];
				vect2 = point -m_PointTable[i-1];
				dist = (vect1.x()*vect1.x())+(vect1.y()*vect1.y());
				vectprod = (vect1.x()*vect2.x())+(vect1.y()*vect2.y());
				vect3.setX( int(double(vect1.x())*double(vectprod)/double(dist)));
				vect3.setY( int(double(vect1.y())*double(vectprod)/double(dist)));
				vect4 = vect2-vect3;
				dist2 = vect4.x()*vect4.x()+vect4.y()*vect4.y();
				if (dist2<=20)
					return PRIO_TRKMILES;
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called as soon as the mouse pointer is moved while the Track 
///  Mile line is under creation mode.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml026, CIAsdFctRwsTlsTml040
//
//-----------------------------------------------------------------------------
void CTrkMilesView::OnMouseMove(UINT nFlags,QPoint point)
{
	QRect radarwndrect=m_pView->geometry();
	QPoint globalpnt=QCursor::pos();
	QRect statusbarrect=((CRwdView*)m_pView)->GetStatusBar()->rect();

	QPoint pnt=point;
	((CAsdView*)m_pView)->DPtoLP(&pnt);
	switch (m_pTrkMiles->m_State)
	{
		case CTrkMiles::modify:
			//If the mouse pointer leaves the radar window on which the creation mode has been 
			//invoked, the creation mode is canceled and the line is removed from display
			if (globalpnt.x()>=radarwndrect.x()+radarwndrect.width() || globalpnt.x()<=radarwndrect.x() || globalpnt.y()<=radarwndrect.y() || globalpnt.y()>=radarwndrect.y()+radarwndrect.height()-statusbarrect.height())
			{
				((CRwdView*)m_pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
				if (m_pTrkMiles)
					delete m_pTrkMiles;
			}
			else
				//The function AddPoint is called with the end modification argument
				//set to false to draw the last line to the mouse pointer
				m_pTrkMiles->AddPoint(pnt,false,false,m_pView);
			break;
		default:
		break;
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click AB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml015
//
//-----------------------------------------------------------------------------
void CTrkMilesView::OnLButtonDown(UINT nFlags,QPoint point)
{
	QPoint pnt=point;
	((CAsdView*)m_pView)->DPtoLP(&pnt);
	switch (m_pTrkMiles->m_State)
	{
	case CTrkMiles::created:
		break;
	case CTrkMiles::modify:
		//A new point is added
		m_pTrkMiles->AddPoint(pnt,true,false,m_pView);
		break;
	default :
		break;
	}		
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click WB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml025, CIAsdFctRwsTlsTml040, CIAsdFctRwsTlsTml060
//
//-----------------------------------------------------------------------------
void CTrkMilesView::OnMButtonDown(UINT nFlags,QPoint pnt)
{
	switch (m_pTrkMiles->m_State)
	{
	case CTrkMiles::modify:
		//The line is removed from display and the mode creation is canceled
		((CRwdView*)m_pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
		if (m_pTrkMiles)
		  delete m_pTrkMiles;
		break;
	case CTrkMiles::created:
		//The line is removed from display if it presents the implicit focus
		if (m_Select)
			delete m_pTrkMiles;
		break;
	default :
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the Escape key press action.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml025, CIAsdFctRwsTlsTml040, CIAsdFctRwsTlsTml060
///					 CIAsdFctKfk095
//
//-----------------------------------------------------------------------------
void CTrkMilesView::OnKeyDown()
{
	switch (m_pTrkMiles->m_State)
	{
	case CTrkMiles::modify:
		//The line is removed from display and the mode creation is canceled
        ((CRwdView*)m_pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
		if (m_pTrkMiles)
		  delete m_pTrkMiles;
		break;
	case CTrkMiles::created:
	        break;
	default :
		break;
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click SB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsTml010, CIAsdFctRwsTlsTml020, CIAsdFctRwsTlsTml040
//
//-----------------------------------------------------------------------------
void CTrkMilesView::OnRButtonDown(UINT nFlags,QPoint point)
{
	QPoint pnt=point;
	((CAsdView*)m_pView)->DPtoLP(&pnt);
	switch (m_pTrkMiles->m_State)
	{
	case CTrkMiles::created:
		break;
	case CTrkMiles::modify:
		//The last point of the line is defined and the creation mode is canceled,underla
		m_pTrkMiles->AddPoint(pnt,false,true,m_pView);
		((CRwdView*)m_pView)->setCursor(*CAsdApp::GetApp()->GetCursor(MPNormalSelect));
		break;
	default :
		break;
	}		
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selection status of the instance to the one put in argument.
///  Argument : bool, Selection status to be applied
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTrkMilesView::SelectElement(bool Select)
{
	m_Select = Select;
	OnUpdate();
}





//-----------------------------------------------------------------------------
//
/// \class CTOORArrow
///  This class manages the settings of the several set of Arrows indicating that
///  a searched track is out of range of all the Radar Wisdows.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTOORArrow::CTOORArrow(CTrack* pTrack)
{
	m_pTrack = pTrack;
	for (int view=0;view<m_sViewTable.GetSize();view++)
	{
		//For each radar window an arrow presentation (CTOORArrowView object)
		//is created 
		CTOORArrowView* pElem = new CTOORArrowView(m_pTrack->GetPos());
		pElem->Init(this,m_sViewTable[view]);
		m_ElemTable.Add(pElem);
		((CRwdView*)m_sViewTable[view])->AddElement(pElem);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class. 
///  Argument : CTOORArrow&, CTOORArrow object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTOORArrow::CTOORArrow(CTOORArrow *pTOORArrow)
{
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTOORArrow::~CTOORArrow()
{
	if (m_pTrack)
		m_pTrack->AssociateTOORArrow(this,false);

	//The m_sViewTable table (list of radar windows displaying arrows) is 
	//emptied
	for (int view=0;view<m_sViewTable.GetSize();view++)
	{
		((CRwdView*)m_sViewTable[view])->RemoveElement(m_ElemTable[view]);
		delete m_ElemTable[view];
	}
	//All the arrows presentations are destroyed
	for (int vect=0;vect<m_sTOORArrowsTable.GetSize();vect++)
	{
		if (m_sTOORArrowsTable[vect]==this)
		{
			m_sTOORArrowsTable.RemoveAt(vect);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function adds a new radar window in the list of radar windows that must
///  be able to display arrows.
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTOORArrow::AddView(CView* pView)
{
	m_sViewTable.Add(pView);
	for (int vect=0;vect<m_sTOORArrowsTable.GetSize();vect++)
	{
		if (m_sTOORArrowsTable[vect])
		{
			QPoint trkpos = m_sTOORArrowsTable[vect]->m_pTrack->GetPos();
			//A new arrow presentation is created
			CTOORArrowView* pElem = new CTOORArrowView(trkpos);
			pElem->Init(m_sTOORArrowsTable[vect],pView);
			m_sTOORArrowsTable[vect]->m_ElemTable.Add(pElem);
			((CRwdView*)pView)->AddElement(pElem);
			pElem->OnUpdate();
		}
	}
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function removes the radar window put in argument from the list of radar
///  windows that must be able to display arrows.
///  Argument : CView*, radar window 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTOORArrow::RemoveView(CView* pView)
{
	for (int view=0;view<m_sViewTable.GetSize();view++)
	{
		if (pView==m_sViewTable[view])
		{
			for (int vect=0;vect<m_sTOORArrowsTable.GetSize();vect++)
			{
				//If the close radar window was displaying a range marker, it is
				//removed from display
				if (((CRwdView*)m_sViewTable[view])->IsElementDisplayed(m_sTOORArrowsTable[vect]->m_ElemTable[view]))
					{
					//If the removed range marker is the last displayed, the tool is
					//canceled, i.e. if a new radar window is created it will not
					//display a range marker
					//	int i=CTOORArrowView::GetNbTOORArrow();
					//	if (i==1)
					//	{
					//		delete m_sTOORArrowsTable[0];
					//		CTOORArrowView::SetNbTOORArrow(0);
					//	}
					//	//In the other case the object is destroyed and the number
					//	//of displayed range markers is decreased
					//	else if (i>1)
					//	{
							m_sTOORArrowsTable[vect]->m_ElemTable.RemoveAt(view);
					//		i--;
					//		CTOORArrowView::SetNbTOORArrow(i);
					//	}
					}
				else
					m_sTOORArrowsTable[vect]->m_ElemTable.RemoveAt(view);
			}
			m_sViewTable.RemoveAt(view);

			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to display a Track Out Of Range Arrow in every opened radar 
///  window. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk010
//
//-----------------------------------------------------------------------------
void CTOORArrow::Create(CTrack* pTrack)
{
	bool IsModifiable = true;
	BYTE sic, sac;
	int trknum1;
	int trknum2;
	bool found = false;

	if (m_sTOORArrowsTable.GetSize())
	{	
		pTrack->GetTrkNum(sic,sac,trknum2);
		for (int arrow=0;arrow<m_sTOORArrowsTable.GetSize(); arrow++)
		{
			m_sTOORArrowsTable[arrow]->m_pTrack->GetTrkNum(sic, sac, trknum1);
			if (trknum1 == trknum2)
				found = true;
		}
	}
	if (!found)
	{
	//if not found a new set of arrows is defined
		for (int view=0;view<m_sViewTable.GetSize();view++)
			IsModifiable &= ((CRwdView*)m_sViewTable[view])->IsModifyNull();
		if (IsModifiable)
		{
			CTOORArrow* NewTOORArrow = NULL;
			NewTOORArrow = new CTOORArrow(pTrack);
			m_sTOORArrowsTable.Add(NewTOORArrow);
			for (int view=0;view<m_sViewTable.GetSize();view++)
			{
				NewTOORArrow->m_ElemTable[view]->OnUpdate();
			}
			pTrack->AssociateTOORArrow(NewTOORArrow,true);

		}

	}
}





//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called from the CTrack class each time the position of the
///  track attached to TOOR Arrow is updated. 
///  Argument 1 : CTrack*, the updated track
///  Argument 2 : bool, whether the track is deleted or not 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CTOORArrow::UpdateTrk(CTrack* pTrack,bool update)
{
	if (update)
	{
		for (int view=0;view<m_sViewTable.GetSize();view++)
		{
			m_ElemTable[view]->UpdateFromTrack(pTrack->GetPos());
		}
	}
	else
	{
		m_pTrack->AssociateTOORArrow(this,false);
		CancelTOORArrow(pTrack);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called from the CRwdView class each time the center of the
///  Radar window is updated. 
///  Argument 1 : CTrack*, the updated track
///  Argument 2 : bool, whether the track is deleted or not 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CTOORArrow::UpdateFromView(CView* RwdWiew)
{
	if (m_sTOORArrowsTable.GetSize())
	{	
		for (int arrow=0;arrow<m_sTOORArrowsTable.GetSize(); arrow++)
		{
			for (int view=0;view<m_sTOORArrowsTable[arrow]->m_sViewTable.GetSize();view++)
			{
				if (m_sTOORArrowsTable[arrow]->m_sViewTable[view] == RwdWiew)
				{
					m_sTOORArrowsTable[arrow]->m_ElemTable[view]->UpdateFromRadarView();

//					if (m_sTOORArrowsTable[arrow]->m_pTrack->m_ElemTable[view]->m_BoundingRect.intersects(m_sViewTable[view]->rect())==true)
					if (m_sTOORArrowsTable[arrow]->m_pTrack->GetViewTrack(RwdWiew)->m_BoundingRect.intersects(m_sViewTable[view]->rect())==true)
						m_sTOORArrowsTable[arrow]->CancelTOORArrow(m_sTOORArrowsTable[arrow]->m_pTrack);
				}
			}
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to remove from display the Arrow correpsonding 
///  to a found track
///  Argument : CTrack*, the found track
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: 
//
//-----------------------------------------------------------------------------
void CTOORArrow::CancelTOORArrow(CTrack* pTrack)
{
	BYTE sic, sac;
	int trknum1;
	int trknum2;

	if (m_sTOORArrowsTable.GetSize())
	{	
		pTrack->GetTrkNum(sic,sac,trknum2);
		for (int arrow=0;arrow<m_sTOORArrowsTable.GetSize(); arrow++)
		{
			m_sTOORArrowsTable[arrow]->m_pTrack->GetTrkNum(sic, sac, trknum1);
			if (trknum1 == trknum2)
				delete m_sTOORArrowsTable[arrow];
		}
	}

}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function removes from display (delete object) the created RB lines for
///  all the existing radar windows.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTOORArrow::RemoveAllArrows()
{
	if (m_sTOORArrowsTable.GetSize())
	{	
		for (int arrow=0;arrow<m_sTOORArrowsTable.GetSize(); arrow++)
		{
			delete m_sTOORArrowsTable[arrow];
		}
	}

}



//-----------------------------------------------------------------------------
//
/// \class CTOORArrowView
///  This class manages the display of an arrow instance.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Constructor of the class. Initialization of member parameters.
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTOORArrowView::CTOORArrowView(QPoint trkpos)
{
	m_BoundingRect=QRect();
	m_Select = false;
	m_TrkOutOfRgePos = trkpos;
	m_pView=NULL;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Copy constructor of the class.
///  Argument : CTOORArrowView&, CTOORArrowView object to be initialized
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTOORArrowView::CTOORArrowView(CTOORArrowView& pRangeMarkView)
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  Destructor of the class. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
CTOORArrowView::~CTOORArrowView()
{
	QRect updRect = m_BoundingRect;
	((CRwdView*)m_pView)->InvalidateRect(&updRect);
	m_pView=NULL;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions draws the range marker. 
///  Argument 1 : QPainter*, painting device
///  Argument 2 : int, drawn layer number
///  Argument 3 : int, drawn sub layer number
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk015, CIAsdFctRwsTlsRmk020
//
//-----------------------------------------------------------------------------
void CTOORArrowView::OnDraw(QPainter* pDC,int prio)
{
	//Check if the layer and sub layer are correct
	if ((prio!=7))
		return;
	QPen pen;
	//retrieve the tool offline defined color
	int ColorNb=CColorMngr::GetColorNb("RW_TOOLS");
	if (m_Select)
		ColorNb=CColorMngr::GetColorNb("SELECTED_FRGRND");
	//Associates the color to the corresponding slider of the brightness control
	//window
	pen.setColor(CColorMngr::TransformColorNb(ColorNb,RangeMarkersCategory));
	pDC->setPen(pen);


	QPoint pnt1, pnt2, pnt3, pnt4, pnt5, pnt6;
	QPoint pntcenter;
	pntcenter=m_CenterPoint;

	switch (m_sOrientation)
	{
		case TOOR_N:
			pnt1=QPoint(pntcenter.x()-20,pntcenter.y()+20);
			pnt2=QPoint(pntcenter.x()-10,pntcenter.y()+20);
			pnt3=QPoint(pntcenter.x()-10,pntcenter.y()+50);
			pnt4=QPoint(pntcenter.x()+10,pntcenter.y()+50);
			pnt5=QPoint(pntcenter.x()+10,pntcenter.y()+20);
			pnt6=QPoint(pntcenter.x()+20,pntcenter.y()+20);
			break;
		case TOOR_NE:
			pnt1=QPoint(pntcenter.x()-30,pntcenter.y());
			pnt2=QPoint(pntcenter.x()-20,pntcenter.y()+10);
			pnt3=QPoint(pntcenter.x()-40,pntcenter.y()+30);
			pnt4=QPoint(pntcenter.x()-30,pntcenter.y()+40);
			pnt5=QPoint(pntcenter.x()-10,pntcenter.y()+20);
			pnt6=QPoint(pntcenter.x(),pntcenter.y()+30);
			break;
		case TOOR_E:
			pnt1=QPoint(pntcenter.x()-20,pntcenter.y()-20);
			pnt2=QPoint(pntcenter.x()-20,pntcenter.y()-10);
			pnt3=QPoint(pntcenter.x()-50,pntcenter.y()-10);
			pnt4=QPoint(pntcenter.x()-50,pntcenter.y()+10);
			pnt5=QPoint(pntcenter.x()-20,pntcenter.y()+10);
			pnt6=QPoint(pntcenter.x()-20,pntcenter.y()+20);
			break;
		case TOOR_SE:
			pnt1=QPoint(pntcenter.x(),pntcenter.y()-30);
			pnt2=QPoint(pntcenter.x()-10,pntcenter.y()-20);
			pnt3=QPoint(pntcenter.x()-30,pntcenter.y()-40);
			pnt4=QPoint(pntcenter.x()-40,pntcenter.y()-30);
			pnt5=QPoint(pntcenter.x()-20,pntcenter.y()-10);
			pnt6=QPoint(pntcenter.x()-30,pntcenter.y());
			break;
		case TOOR_S:
			pnt1=QPoint(pntcenter.x()+20,pntcenter.y()-20);
			pnt2=QPoint(pntcenter.x()+10,pntcenter.y()-20);
			pnt3=QPoint(pntcenter.x()+10,pntcenter.y()-50);
			pnt4=QPoint(pntcenter.x()-10,pntcenter.y()-50);
			pnt5=QPoint(pntcenter.x()-10,pntcenter.y()-20);
			pnt6=QPoint(pntcenter.x()-20,pntcenter.y()-20);
			break;
		case TOOR_SW:
			pnt1=QPoint(pntcenter.x()+30,pntcenter.y());
			pnt2=QPoint(pntcenter.x()+20,pntcenter.y()-10);
			pnt3=QPoint(pntcenter.x()+40,pntcenter.y()-30);
			pnt4=QPoint(pntcenter.x()+30,pntcenter.y()-40);
			pnt5=QPoint(pntcenter.x()+10,pntcenter.y()-20);
			pnt6=QPoint(pntcenter.x(),pntcenter.y()-30);
			break;
		case TOOR_W:
			pnt1=QPoint(pntcenter.x()+20,pntcenter.y()+20);
			pnt2=QPoint(pntcenter.x()+20,pntcenter.y()+10);
			pnt3=QPoint(pntcenter.x()+50,pntcenter.y()+10);
			pnt4=QPoint(pntcenter.x()+50,pntcenter.y()-10);
			pnt5=QPoint(pntcenter.x()+20,pntcenter.y()-10);
			pnt6=QPoint(pntcenter.x()+20,pntcenter.y()-20);
			break;
		case TOOR_NW:
			pnt1=QPoint(pntcenter.x(),pntcenter.y()+30);
			pnt2=QPoint(pntcenter.x()+10,pntcenter.y()+20);
			pnt3=QPoint(pntcenter.x()+30,pntcenter.y()+40);
			pnt4=QPoint(pntcenter.x()+40,pntcenter.y()+30);
			pnt5=QPoint(pntcenter.x()+20,pntcenter.y()+10);
			pnt6=QPoint(pntcenter.x()+30,pntcenter.y());
			break;
		default:
			break;
	}

	pDC->drawLine(pntcenter,pnt1);
	pDC->drawLine(pnt1,pnt2);
	pDC->drawLine(pnt2,pnt3);
	pDC->drawLine(pnt3,pnt4);
	pDC->drawLine(pnt4,pnt5);
	pDC->drawLine(pnt5,pnt6);
	pDC->drawLine(pnt6,pntcenter);

}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function returns an integer corresponding to the fact that the range 
///  marker must present the implicit focus or not.
///  Argument 1 : QPoint, position of the mouse pointer
///  Argument 2 : bool, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
int CTOORArrowView::GetPriority(QPoint point,bool )
{
	//If the CWP is already in a tool creation or modification mode the range marker
	//must not present the implicit focus
	if (CTrkMiles::IsInCreation() || CVector::IsInCreation() || CNewRange::IsInCreation() || CRwdView::IsInViewModif())
		return 0;

	if (m_BoundingRect.contains(point))
		return PRIO_RANGEMARK;

	return 0;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function sets the selection status of the range marker to the one put in argument.
///  Argument : bool, Selection status to be applied
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTOORArrowView::SelectElement(bool Select)
{
	m_Select = Select;
	OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to manage the click AB action.
///  Argument 1 : UINT, event description
///  Argument 2 : QPoint, mouse pointer position
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk025
//
//-----------------------------------------------------------------------------
void CTOORArrowView::OnLButtonDown(UINT nFlags,QPoint point)
{
	//If the range marker has the implicit focus, the motion mode is initialized
	//the range marker follows the mouse pointer
	if (m_Select)
	{
		bool IsModifiable = true;
		for (int view=0;view<CTOORArrow::m_sViewTable.GetSize();view++)
			IsModifiable &= ((CRwdView*)CTOORArrow::m_sViewTable[view])->IsModifyNull();
		if (IsModifiable)
		{
			((CRwdView*)m_pView)->SetCentre(m_TrkOutOfRgePos);
			((CRwdView*)m_pView)->OnUpdate(NULL,0,NULL);
			m_pTOORArrow->CancelTOORArrow(m_pTOORArrow->m_pTrack);
		}
	}
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions calculates the part of the screen that needs to be updated when
///  the arrow will be drawn. 
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTOORArrowView::OnUpdate()
{
	QRect UpdRect = m_BoundingRect;
	QPoint pnt1,pnt2,pntcenter;

	pntcenter=m_CenterPoint;
	//((CAsdView*)m_pView)->DPtoLP(&pntcenter);
	
	switch (m_sOrientation)
	{
		case TOOR_N:
			pnt1=QPoint(pntcenter.x()-20,pntcenter.y()+50);
			pnt2=QPoint(pntcenter.x()+20,pntcenter.y());
			break;
		case TOOR_NE:
			pnt1=QPoint(pntcenter.x()-40,pntcenter.y()+40);
			pnt2=QPoint(pntcenter.x(),pntcenter.y());
			break;
		case TOOR_E:
			pnt1=QPoint(pntcenter.x()-50,pntcenter.y()+20);
			pnt2=QPoint(pntcenter.x(),pntcenter.y()-20);
			break;
		case TOOR_SE:
			pnt1=QPoint(pntcenter.x()-40,pntcenter.y());
			pnt2=QPoint(pntcenter.x(),pntcenter.y()-40);
			break;
		case TOOR_S:
			pnt1=QPoint(pntcenter.x()-20,pntcenter.y());
			pnt2=QPoint(pntcenter.x()+20,pntcenter.y()-50);
			break;
		case TOOR_SW:
			pnt1=QPoint(pntcenter.x(),pntcenter.y());
			pnt2=QPoint(pntcenter.x()+40,pntcenter.y()-40);
			break;
		case TOOR_W:
			pnt1=QPoint(pntcenter.x(),pntcenter.y()+20);
			pnt2=QPoint(pntcenter.x()+50,pntcenter.y()-20);
			break;
		case TOOR_NW:
			pnt1=QPoint(pntcenter.x(),pntcenter.y()+40);
			pnt2=QPoint(pntcenter.x()+40,pntcenter.y());
			break;
	}

//	((CAsdView*)m_pView)->LPtoDP(&pnt1);
//	((CAsdView*)m_pView)->LPtoDP(&pnt2);

	m_BoundingRect=QRect(pnt1,pnt2);
	m_BoundingRect=m_BoundingRect.normalize();
	m_BoundingRect.addCoords(-10,-10,10,10);
	UpdRect|=m_BoundingRect;
	((CRwdView*)m_pView)->InvalidateRect(&UpdRect);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions initializes the settings of the range marker instance. 
///  Argument 1 : LPVOID, parent object
///  Argument 2 : CView*, radar window on which the range marker must be displayed
///  Argument 3 : CDisplayElement*, not used in this function
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTOORArrowView::Init(LPVOID pData,CView *pView,CDisplayElement* )
{
	//The associated view is set 
	m_pView = pView;
	m_pTOORArrow = (CTOORArrow*)pData;

	m_sOrientation = GetTOORDirection();
	m_CenterPoint = SetCenterArrowPos();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions updates the Arrow display following the reception of track
///  update
///  Argument 1 : QPoint: new position of the track
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTOORArrowView::UpdateFromTrack(QPoint point)
{
	m_TrkOutOfRgePos = point;
	m_sOrientation = GetTOORDirection();
	m_CenterPoint = SetCenterArrowPos();
	OnUpdate();
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions updates the Arrow display following the modification of the 
///  Radar window range or center
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
void CTOORArrowView::UpdateFromRadarView()
{
	m_sOrientation = GetTOORDirection();
	m_CenterPoint = SetCenterArrowPos();
	OnUpdate();
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function positions the arrow at its position 
///  
///  Argument : None
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
QPoint CTOORArrowView::SetCenterArrowPos()
{
	QPoint pnt;
	QRect rectRadar = m_pView->rect();
	QRect rectstb=((CRwdView*)m_pView)->GetStatusBar()->rect();

	int offx = 20;
	int offy = 20;

	switch (m_sOrientation)
	{
		case TOOR_N:
			pnt=QPoint((rectRadar.width())/2,offy);
			break;
		case TOOR_NE:
			pnt=QPoint(rectRadar.width()-offx,offy);
			break;
		case TOOR_E:
			pnt=QPoint(rectRadar.width()-offx,(rectRadar.height()-rectstb.height())/2); 
			break;
		case TOOR_SE:
			pnt=QPoint(rectRadar.width()-offx,rectRadar.height()-rectstb.height()-offy);
			break;
		case TOOR_S:
			pnt=QPoint((rectRadar.width())/2,rectRadar.height()-rectstb.height()-offy);
			break;
		case TOOR_SW:
			pnt=QPoint(offx,rectRadar.height()-rectstb.height()-offy);
			break;
		case TOOR_W:
			pnt=QPoint(offx,rectRadar.height()-(rectRadar.height()-rectstb.height())/2);
			break;
		case TOOR_NW:
			pnt=QPoint(offx,offy);
			break;
		default:
			break;
	}
	return pnt;
}



//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This functions determines the direction in which the track to be found is
///  compared to the center of the radar window
///  
///  Argument : None
/// 
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: NONE 
//
//-----------------------------------------------------------------------------
TOORDirection CTOORArrowView::GetTOORDirection()
{
	QPoint centre = ((CRwdView*)m_pView)->GetCentre();
	QPoint dif = QPoint(m_TrkOutOfRgePos.x()-centre.x(), m_TrkOutOfRgePos.y()-centre.y());

	if (dif.x()>0)//right side
	{
		if (dif.y()>0)//top right side
		{
			if (2*dif.x()<dif.y())
				return TOOR_N;
			else
			{
				if (dif.x()>2*dif.y())
					return TOOR_E;
				else
					return TOOR_NE;
			}
		}
		else//bottom right side
		{
			if (2*dif.x()<abs(dif.y()))
				return TOOR_S;
			else
			{
				if (dif.x()>2*abs(dif.y()))
					return TOOR_E;
				else
					return TOOR_SE;
			}
		}
	}
	else//left side
	{
		if (dif.y()>0)//top left side
		{
			if (2*abs(dif.x())<dif.y())
				return TOOR_N;
			else
			{
				if (abs(dif.x())>2*dif.y())
					return TOOR_W;
				else
					return TOOR_NW;
			}
		}
		else//bottom left side
		{
			if (2*abs(dif.x())<abs(dif.y()))
				return TOOR_S;
			else
			{
				if (abs(dif.x())>2*abs(dif.y()))
					return TOOR_W;
				else
					return TOOR_SW;
			}
		}
	}
}





//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///  This function is called to cancel the resize effect of attached borders.
///
///  RETURNS: 
///  void
///
///  REQUIREMENT ID: CIAsdFctRwsTlsRmk021
//
//-----------------------------------------------------------------------------
void CTOORArrowView::SetResizeOffset(QPoint offset)
{
	m_CenterPoint=m_CenterPoint+offset;
	OnUpdate();
}



//--------------------------------< THE END >----------------------------------

