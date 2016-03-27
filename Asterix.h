/*
 * Copyright (C) 2013 SkySoft-ATM 
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
//  FILENAME: Asterix.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    Header file for the asterix data management
// 
//  COMMENTS
// 
//-----------------------------------------------------------------------------
// 
//  CHANGE HISTORY
// 
//  Revision 2  2013/12/29 dalleins
//
//
//
// 
//
//
//
// 
//-----------------------------------------------------------------------------
#if !defined(ASTERIX_H)
#define ASTERIX_H

//-INCLUDE FILES---------------------------------------------------------------

#include "String.h"

//-GLOBAL DATA-----------------------------------------------------------------


typedef enum {NoFld,SicSac,MsgType,TrkNum,TrkRep,TrkPos,TrkPosPol,TrkTraj,TrkSpd,TrkSpdVector,TrkStatus,TrkVSpd,TrkSsr,TrkAlt,TrkModeC,TrkCls,TrkSector,TrkUrg,TrkRad,Trkxpt,Trkxfl,Trksi,TrkAcd,TrkHndIn,TrkHndLogIn,TrkCa,TrkCaSect,TrkMsaw,TrkSource,TrkArea,TrkFplSts,TrkAircraft,TrkAddr,TrkPtid1,RadarReport,
			  WthrScale,WthrLevel,WthrCartRangeVectors,WthrPolarVectors,WthrCartXYVectors,WthrTotal,TrkNb1,TrkNb2,AlarmNb,VSep,HSep,Callsign,TrkAdes,TrkAdep,TrkAtyp,TrkFlMod,SensorId,SensorStatus,ServiceStatus,AlarmState,FAcSelAlt,IAcSelAlt,AlarmStatus} AsterixFields;
typedef struct
{
	int start_range;
	int end_range;
	int azimuth;
} PolarVectors;

typedef struct
{
	int Range;
	int Azimuth;
} PolarCoord;

//-CLASS DEFINITION---------------------------------------------------------

class CAsterixMsg : public CObject
{
public:

	// methods
	
	// contructors of CAsterixMsg class
	CAsterixMsg(BYTE* pMsg,int TotalLen);
	CAsterixMsg(CAsterixMsg& pAster);
	// destructor of CAsterixMsg class
	~CAsterixMsg();

	// decodes the asterix messages.
	bool DecodeMessage();
	// returns the category of the current ASTERIX message
	int GetCategory();
	// indicates if a field is present in the asterix message
	bool IsFieldPresent(AsterixFields field);
	// returns the value of an unsigned integer field present in the asterix message
	int GetUIntVal(AsterixFields field);
	// returns the value of an unsigned integer field present in the asterix message
	int GetIntVal(AsterixFields field);
	// returns the value of a string field present in the asterix message
	CString GetStrVal(AsterixFields field);
	// returns the value of a Polard Coord present in the asterix message
	PolarCoord GetPolarCoord(AsterixFields field);
	// gets the coordinates of a posxy field from the asterix message
	QPoint GetPos(AsterixFields field);
	// returns the value of an byte field present in the asterix message
	BYTE GetByteVal(AsterixFields field,int bytenb);
	// returns the value of a Vector field present in the asterix message
	int GetVectorsTotal(AsterixFields field);
	// returns the value of a Polar Vector field present in the asterix message
	PolarVectors GetVectorNb(AsterixFields field,int VectorNb);
	// returns the length of a field present in the asterix message
	int GetFieldLength(AsterixFields field);
	int PrintDecodedMessage();

private:

	// variables

	// legnth of the asterix message
	int    m_TotalLen;
	// structure that gives some information on each asterix fields
	typedef struct 
	{
		//Whether the ASTERIX field is present in the received message
		bool Presence;
		//Position in the message
		int  FirstPos;
		//Length of the field
		int  Len;
		//Type of the field
		int  Kind;
	} m_AsterixFld;
	// table that stores the asterix fields
	m_AsterixFld m_FldTable[AlarmStatus+1];
	// category of the asterix message
	int m_Category;
	// position of a track given in the asterix message
	int m_CurPos;
	// length of the asterix message
	int m_BlockLen;
	// position in the asterix message
	int m_BlockPos;
	// asterix message
	BYTE* m_pMsg;


	// methods

	// constructor of CAsterixMsg class
	CAsterixMsg();
	// decode the asterix message
	bool  DecodeInternalMessage();

};

#endif

