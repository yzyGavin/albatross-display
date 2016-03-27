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
//  FILENAME: Geo.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the management of geographical points
//		and also the conversion functions in order to display it
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
// 
//-----------------------------------------------------------------------------

#if !defined(AFX_GEO_H__19A73878_BBC0_456C_81A4_AAD0421E7D1C__INCLUDED_)
#define AFX_GEO_H__19A73878_BBC0_456C_81A4_AAD0421E7D1C__INCLUDED_

//-GLOBAL DATA-----------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./CommonFiles/Templ.h"



typedef struct
{
	BOOL NORTH ;
	int deg;
	int min;
	int sec;
}Latitude;

typedef struct
{
	BOOL EAST  ;
	int deg  ;
	int min  ;
	int sec  ;
}Longitude;

typedef struct 
{
	QString name;
	QString isHold;
	long x;
	long y;
	QString Lat;
	QString Long;
}TPoint;

//-CLASS DEFINITION---------------------------------------------------------

class CGeo : public CObject  
{
	friend class CAsdApp;
	friend class CFlightPlan;
	friend class CRwdView;

private:

	//variables 
	static CArray <TPoint, TPoint> m_MapPoint;

	//methods
	CGeo();

	//virtual methods
	virtual ~CGeo();

	//static methods
	static double  GeographicToDecimal(Latitude  lat); 
	static double  GeographicToDecimal (Longitude lng);
	static void DecimalToGeographic (double val, Latitude& lat);
	static void DecimalToGeographic (double val, Longitude& lng);
	static double DecimalToRadian (double lat);
	static double LatRadianToDecimal (double rad);
	static double LongRadianToDecimal (double rad);
	static double GeoToConf(double lat);
	static double ConfToGeo(double lat);
	static BOOL InitCenter(QString center);
	static BOOL InitCenterInIni(QString IniName);
	static void ComputeLocalEarthRadius(double lat, double lng);	
	static void StereographicToGeographic(double lat, double lng,Latitude& Lat, Longitude& Long);	
	static void GeographicToStereographic(Latitude lat, Longitude lng, double& Lat, double& Lng);
	static void LoadPoints(QString IniName);
	static BOOL  GetPoint(QString name, QPoint& point);
	static long RoundTo32(double val);
	static double RoundFr32(long val);	
	
protected :

	//variables
	static double m_CenterLatDec;
	static double m_CenterLongDec;
	static const double PI ;
	static const double RADIAN_PER_DEGREE;
	static const double MINUTES_PER_DEGREE;
	static const double SECONDS_PER_MINUTE;
	static const double SECONDS_PER_DEGREE;
	static const double KM_PER_NAUTICAL_MILE;	
	//Equatorial earth radius
	static const double KM_EARTH_RADIUS ;
	static const double NM_EARTH_RADIUS ;	
	//Local earth radius computed at center initialization.
	static double KM_LOCAL_EARTH_RADIUS ;
	static double NM_LOCAL_EARTH_RADIUS ;	
	static const double EXCENTRICITY;

	//methods
	static void InitCenter(Latitude lat, Longitude lng);
};

#endif  // end of file

//--------------------------------< THE END >---------------------------------- 
