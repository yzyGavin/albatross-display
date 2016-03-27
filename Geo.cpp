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
//  FILENAME: Geo.cpp
//  TYPE    : c++ code-file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains the management of geographical points
//		and also the conversion functions in order to display it
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
//-----------------------------------------------------------------------------

//-INCLUDE FILES---------------------------------------------------------------

#include "StdAfx.h"
#include "Geo.h"
#include "String.h"
#include "./CommonFiles/IniFile.h"
#include "math.h"
#include "AsdApp.h"

//-GLOBAL DATA-----------------------------------------------------------------

   const double CGeo::PI = 3.14159265359;
   const double CGeo::RADIAN_PER_DEGREE = PI/180.0;
   const double CGeo::MINUTES_PER_DEGREE = 60.0;
   const double CGeo::SECONDS_PER_MINUTE = 60.0;
   const double CGeo::SECONDS_PER_DEGREE = CGeo::SECONDS_PER_MINUTE * CGeo::MINUTES_PER_DEGREE;
   const double CGeo::KM_PER_NAUTICAL_MILE = 1.852;

   // Equatorial earth radius
   const double CGeo::KM_EARTH_RADIUS = 6378.160 ;
   const double CGeo::NM_EARTH_RADIUS = CGeo::KM_EARTH_RADIUS / CGeo::KM_PER_NAUTICAL_MILE ;

   // Local earth radius computed at center init.
   double CGeo::KM_LOCAL_EARTH_RADIUS = CGeo::KM_EARTH_RADIUS;
   double CGeo::NM_LOCAL_EARTH_RADIUS = CGeo::NM_EARTH_RADIUS ;
  
   // New excentricity values on 21th march 2005
   const double CGeo::EXCENTRICITY = 0.081819191;

   double CGeo::m_CenterLatDec;
   double CGeo::m_CenterLongDec;
   CArray <TPoint, TPoint> CGeo::m_MapPoint;


//-PROTOTYPES---------------------------------------------------------

//-----------------------------------------------------------------------------
//
/// \class CGeo
/// This class is used to manage geographical points, perform conversions 
///	for display purpose.
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the contructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CGeo::CGeo()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method is the destructor
/// 
///  RETURNS: 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
CGeo::~CGeo()
{
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts the latitude from geographic to decimal coordinates
/// 
///  RETURNS: 
///		decimal coordinates
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
double  CGeo::GeographicToDecimal(Latitude  lat)
{
	double res;
	res = (double)lat.deg +  (double)(lat.min)/MINUTES_PER_DEGREE + lat.sec/SECONDS_PER_DEGREE;
		
	if (lat.NORTH) 
		return res;
	else
		return -(res);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts the longitude from geographic to decimal coordinates
/// 
///  RETURNS: 
///		decimal coordinates
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
double  CGeo::GeographicToDecimal (Longitude lng)
{
	double res;
	res = (double)lng.deg + (double)(lng.min)/ MINUTES_PER_DEGREE + (double)(lng.sec)/ SECONDS_PER_DEGREE;
    if (lng.EAST)
      return res;
    else
      return -res;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts the latitude from decimal to geographic coordinates
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CGeo::DecimalToGeographic (double val, Latitude& lat)
{
	int deg,min,sec;

	deg = (int)(val);
	sec = (int)( (val - (double)deg)*3600.0);
	min = (sec/60);
	sec = (sec%60);
	lat.deg =deg;
	lat.min = min;
	lat.sec =sec;
	lat.NORTH = (val >=0.0);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts the longitude from decimal to geographic coordinates
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CGeo::DecimalToGeographic (double val, Longitude& lng)
{
	int deg,min,sec;
	deg = (int)(val);
	sec = (int)( (val - (double)deg)*3600.0);
	min = (sec/60);
	sec = (sec%60);
	lng.deg =deg;
	lng.min = min;
	lng.sec =sec;
	lng.EAST = (val >=0);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts the latitude from decimal to radian coordinates
/// 
///  RETURNS: 
///		the latitude in radian
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
double CGeo::DecimalToRadian (double lat)
{
    return lat * RADIAN_PER_DEGREE;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts the latitude from radian to decimal coordinates
/// 
///  RETURNS: 
///		the latitude in radian
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
double CGeo::LatRadianToDecimal (double rad)
{
	double res;
	res =  fmod((rad / RADIAN_PER_DEGREE), (2.0 * 90.0));

	if (res >= 90.0)
     return ( res + (2.0 *-90.0));
    else
      return res;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts the longitude from radian to decimal coordinates
/// 
///  RETURNS: 
///		the longitude in radian
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
double CGeo::LongRadianToDecimal (double rad)
{
	double res;
    res =  fmod((rad / RADIAN_PER_DEGREE),(2.0 * 180.0));
    if (res >= 180.0)
      return (res + 2.0 *(-180.0));
    else
      return res;
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the center of the radar window taking a latitude
///		and a longitude in parameter
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CGeo::InitCenter(Latitude lat, Longitude lng)
{
	CGeo::m_CenterLatDec = CGeo::GeographicToDecimal(lat);
	CGeo::m_CenterLongDec = CGeo::GeographicToDecimal(lng);
	CGeo::ComputeLocalEarthRadius(CGeo::m_CenterLatDec,CGeo::m_CenterLongDec);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method initializes the center of the radar window with the center
///		put in argument with the following format: ddmmssN or ddmmssS + dddmmssE
///		or dddmmssW
/// 
///  RETURNS: 
///		true by default, false if the format is not respected
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
BOOL CGeo::InitCenter(QString center)
{
	// Expected format : ddmmssN or ddmmssS + dddmmssE or dddmmssW
	int p1, p2;
	center=center.stripWhiteSpace();

	p1 = center.find("N",0);
	if(!p1) 
	{
		p1 = center.find("S");
		if(!p1) 
			return FALSE;
	}
	p2 = center.find("E");
	if(!p2)
	{
		p2 = center.find("W");
		if(!p2) 
			return FALSE;
	}
	
	QString dum, deg, min, sec;
	
	dum = center;
	// Latitude decoding

	//degree
	deg = dum.mid(0,2);
	dum.remove(0,2);

	//Minutes
	min = dum.mid(0,2);
	dum.remove(0,2);

	//Seconds
	sec = dum.mid(0,2);
	dum.remove(0,2);
				
	Latitude lat;
	lat.deg = atoi(deg.ascii());
	lat.min = atoi(min.ascii());
	lat.sec = atoi(sec.ascii());
	lat.NORTH = (dum[0]=='N');

	dum.remove(0,1);
	
	// Longitude decoding

	//degree
	dum=dum.stripWhiteSpace();
	deg = dum.mid(0,3);
	dum.remove(0,3);
	
	//Minutes
	dum=dum.stripWhiteSpace();
	min = dum.mid(0,2);
	dum.remove(0,2);
	
	//Seconds
	dum=dum.stripWhiteSpace();
	sec = dum.mid(0,2);
	dum.remove(0,2);
	
	Longitude lng;
	lng.deg = atoi(deg.ascii());
	lng.min = atoi(min.ascii());
	lng.sec = atoi(sec.ascii());
	lng.EAST = (dum[0] == 'E');

	CGeo::InitCenter(lat,lng);
	return TRUE;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method transforms the geographical latitude to conformal latitude
/// 
///  RETURNS: 
///		value of conformal latitude
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
double CGeo::GeoToConf(double lat)
{
	double latsin = sin(lat);
	double factor1 = (1.0 - (EXCENTRICITY *latsin)) / (1.0 + (EXCENTRICITY * latsin));
	double factor = pow(factor1,(EXCENTRICITY / 2.0));
	return ((2.0 * atan( tan(PI / 4.0 + lat / 2.0) * factor)) - PI/2.0);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method transforms the confromal latitude to geographical latitude
/// 
///  RETURNS: 
///		value of the geographical latitude
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
double CGeo::ConfToGeo(double lat)
{
	double latsin = sin(lat);
	double factor1 = (1.0 + (CGeo::EXCENTRICITY *latsin))/(1.0 - (CGeo::EXCENTRICITY *latsin));
	double factor =   pow(factor1, (CGeo::EXCENTRICITY/2.0));
    return ((2.0 * atan(tan((PI /4.0)+(lat/2.0)) *factor )) - (PI /2.0));
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method sets the earth radius in kilometers for the latitude put in
///		argument
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
void CGeo::ComputeLocalEarthRadius(double lat, double )
{
	double latradian = CGeo::DecimalToRadian(lat);
	double conflatradian = CGeo::GeoToConf(latradian);
	double latsin = sin(latradian);
	double den = cos(conflatradian) *  sqrt(1.0 - (EXCENTRICITY * EXCENTRICITY * latsin * latsin));
	double factor = cos(latradian)/den;
	CGeo::KM_LOCAL_EARTH_RADIUS = CGeo::KM_EARTH_RADIUS * factor;
	CGeo::NM_LOCAL_EARTH_RADIUS = CGeo::NM_EARTH_RADIUS * factor;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts a value in 32th of nautical miles
/// 
///  RETURNS: 
///		nautical miles
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
long CGeo::RoundTo32(double val)
{
	long dum;
	dum = (long)(val*32.0);
	if(fabs(val*32.0 -dum) > 0.5)
	{
		if (dum>=0) 
			dum++;
		else 
			dum--;
	}
	return dum;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts a 32th of nautical miles into a distance
/// 
///  RETURNS: 
///		distance 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------
double CGeo::RoundFr32(long val)
{
	double dum;
	dum = (val/32.0);

	return dum;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts a geographical latitude and longitude into
///		stereographic
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------					 
void CGeo::GeographicToStereographic(Latitude lat, Longitude lng, double& Lat, double& Lng)
{

	double dtlong = CGeo::DecimalToRadian(CGeo::GeographicToDecimal(lng)) - CGeo::DecimalToRadian(CGeo::m_CenterLongDec);
	double latradian = CGeo::DecimalToRadian(CGeo::GeographicToDecimal(lat));
	double conflatradian = CGeo::GeoToConf(latradian);
	double simucenterlatradian = CGeo::DecimalToRadian(CGeo::m_CenterLatDec);
	double simucenterconflatradian = CGeo::GeoToConf(simucenterlatradian);
	double divisor = 1.0 + sin(conflatradian)* sin(simucenterconflatradian)
		+ cos(conflatradian)*cos(simucenterconflatradian)*cos(dtlong);
	double diameter = 2.0 * CGeo::NM_LOCAL_EARTH_RADIUS;

	Lng = ((diameter*sin(dtlong)*cos(conflatradian))/divisor);
	Lat = diameter*(sin(conflatradian)*cos(simucenterconflatradian) - cos(conflatradian)*sin(simucenterconflatradian)*cos(dtlong))/divisor;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts a stereographic into geographic coordinates
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------	
void CGeo::StereographicToGeographic(double lat, double lng,Latitude& Lat, Longitude& Long)
{
	double x =  CGeo::RoundFr32((long)lng);
	double y =  CGeo::RoundFr32((long)lat);

	double lc1 = CGeo::DecimalToRadian(CGeo::m_CenterLatDec);
	double latcenter = CGeo::GeoToConf(lc1);
	double longcenter = CGeo::DecimalToRadian(CGeo::m_CenterLongDec);

	double K = 1.0 - ((x*x +y*y) / (4.0*CGeo::NM_LOCAL_EARTH_RADIUS*CGeo::NM_LOCAL_EARTH_RADIUS));

	double rlong = x/ ((CGeo::NM_LOCAL_EARTH_RADIUS*cos(latcenter)*K) - (y*sin(latcenter)));
	double dlong = longcenter + atan(rlong);
	double rlat =  (cos(dlong - longcenter)
					* (y+(K*CGeo::NM_LOCAL_EARTH_RADIUS*tan(latcenter)))) / 
					((CGeo::NM_LOCAL_EARTH_RADIUS * K) - (y * tan(latcenter)));

	double dlat1 = atan(rlat);
	double dlat2 = CGeo::ConfToGeo(dlat1);
	double dlat = CGeo::LatRadianToDecimal(dlat2);

	CGeo::DecimalToGeographic(CGeo::LongRadianToDecimal(dlong),Long);
	CGeo::DecimalToGeographic(dlat,Lat);
}


//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method loads a latitude and longitude with the following format: 
///		ddmmssN or ddmmssS + dddmmssE or dddmmssW from the characteristic points
///		offline file and stores them in the	m_MapPoint table
/// 
///  RETURNS: 
///		void
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------	
void CGeo::LoadPoints(QString IniName)
{
	CString line;
	QString name, dum, infir,deg, min, sec, holdPoint;		
	QFile inpointsfile(IniName+"CHAR_PTS.TXT");

	if( inpointsfile.open(IO_ReadOnly))
	{
		while (inpointsfile.readLine(line,1024)!=-1) 
		{
			line=line.stripWhiteSpace();
			if ((line.find("--") != 0) && (line.GetNbrOfArgument('|')==4))
			{
				//name field decoding
				line.GetArgumentNb(1,'|',name);
				name=name.stripWhiteSpace();

				//Coordinates field decoding
				line.GetArgumentNb(2,'|',dum);
				dum=dum.stripWhiteSpace();
	
				// Latitude decoding
				//degree
				deg = dum.mid(0,2);
				dum.remove(0,2);
				//Minutes
				min = dum.mid(0,2);
				dum.remove(0,2);
				//Seconds
				sec = dum.mid(0,2);
				dum.remove(0,2);

				Latitude lat;
				lat.deg = atoi(deg.ascii());
				lat.min = atoi(min.ascii());
				lat.sec = atoi(sec.ascii());
				lat.NORTH = (dum[0]=='N');

				dum.remove(0,1);

				// Longitude decoding
				//degree
				dum=dum.stripWhiteSpace();
				deg = dum.mid(0,3);
				dum.remove(0,3);
				//Minutes
				dum=dum.stripWhiteSpace();
				min = dum.mid(0,2);
				dum.remove(0,2);
				//Seconds
				dum=dum.stripWhiteSpace();
				sec = dum.mid(0,2);
				dum.remove(0,2);

				Longitude lng;
				lng.deg = atoi(deg.ascii());
				lng.min = atoi(min.ascii());
				lng.sec = atoi (sec);

				lng.EAST = (dum[0] == 'E');

				double dlat,dlong;
				CGeo::GeographicToStereographic(lat,lng, dlat, dlong);
				int x,y;

				x = CGeo::RoundTo32(dlong);
				y = CGeo::RoundTo32(dlat);

				TPoint point;
				int tst=-1;
				for (int i=0;i<m_MapPoint.GetSize();i++)
				{
					if (m_MapPoint[i].name.compare(name)==0)
					{
						tst=i;
						break;
					}
				}
				if(tst !=-1)
				{
					QString err;
					err.sprintf(" Point conversion stopped due to 2 equals points name : %s", name.ascii());
					return ;
				}

				point.x = x;
				point.y = y;
				point.name=name;
				line.GetArgumentNb(3,'|',infir);
				infir=infir.stripWhiteSpace();
				line.GetArgumentNb(4,'|',holdPoint);
				holdPoint= holdPoint.stripWhiteSpace();
				point.isHold = holdPoint;
				if (infir=="Y")
					CGeo::m_MapPoint.Add(point);
			}
			else
				CAsdApp::GetApp()->WriteLogMsg("Wrong number of arguments in CHAR.txt file",LogError);
		}
		inpointsfile.close();
	}
	else
	{
		CAsdApp::GetApp()->WriteLogMsg("File "+IniName+"CHAR_PTS.TXT does not exist",LogError);
		CAsdApp::GetApp()->setOfflineOk(false);
	}
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method converts a latitude and longitude with the following format: 
///		ddmmssN or ddmmssS + dddmmssE or dddmmssW into a QPoint
/// 
///  RETURNS: 
///		true if the point is successfuly retrieved, false otherwise 
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------	
BOOL  CGeo::GetPoint(QString name, QPoint& point)
{
	TPoint lpoint;
	bool exist=false;
	for (int i=0;i<m_MapPoint.GetSize();i++)
	{
		if (m_MapPoint[i].name.compare(name)==0)
		{
			lpoint=m_MapPoint[i];
			exist=true;
			break;
		}
	}
	if (exist)
	{
		point.setX(lpoint.x);
		point.setY(lpoint.y);
	}
	return exist;
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This method retrieves the init center from the <version>.ini file with the
///		following format:ddmmssN or ddmmssS + dddmmssE or dddmmssW and saves it into
///     a QPoint
/// 
///  RETURNS: 
///		true if the InitCenter method has been successfully performed, else false
///
///  REQUIREMENT ID: NONE
//-----------------------------------------------------------------------------	
BOOL  CGeo::InitCenterInIni(QString IniName)
{
	CIniFile IniFile(IniName);
	QString LatLong;
	IniFile.GetIniProfileString("MOSAIC","CENTER",LatLong);
	LatLong=LatLong.stripWhiteSpace();
	return InitCenter(LatLong);
}


//--------------------------------< THE END >----------------------------------
