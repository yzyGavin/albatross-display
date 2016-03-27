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
//  Revision 1  2009/08/31 dalleins
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


#include "String.h"


//typedef enum {NoType,AliveMsg,RamldMsg,RamhdMsg,MsawMsg,ApwMsg,ClamMsg,StcaMsg} SafetyTypes;

typedef enum {NoFld,SicSac,MsgType,TrkNum,TrkRep,TrkPos,TrkPosPol,TrkSpd,TrkSpdVector,TrkStatus,TrkVSpd,TrkSsr,TrkAlt,TrkModeC,TrkCls,TrkSector,TrkUrg,TrkRad,Trkxpt,Trkxfl,Trksi,TrkAcd,TrkHndIn,TrkHndLogIn,TrkCa,TrkCaSect,TrkMsaw,TrkSource,TrkArea,TrkFplSts,TrkAircraft,TrkAddr,TrkPtid1,RadarReport,
			  WthrScale,WthrLevel,WthrCartVectors,WthrPolarVectors,WthrTotal,TrkNb1,TrkNb2,AlarmNb,VSep,HSep,Callsign,TrkAdes,TrkAdep,TrkAtyp,TrkFlMod,AlarmState,AlarmStatus} AsterixFields;
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

class CAsterixMsg : public CObject
{
public:
	CAsterixMsg(BYTE* pMsg,int TotalLen);
	~CAsterixMsg();
	bool DecodeMessage();
	int GetCategory();
	bool IsFieldPresent(AsterixFields field);
	int GetUIntVal(AsterixFields field);
	int GetIntVal(AsterixFields field);
	CString GetStrVal(AsterixFields field);
	PolarCoord GetPolarCoord(AsterixFields field);
	QPoint GetPos(AsterixFields field);
	BYTE GetByteVal(AsterixFields field,int bytenb);
	int GetVectorsTotal(AsterixFields field);
	PolarVectors GetVectorNb(AsterixFields field,int VectorNb);
	int GetFieldLength(AsterixFields field);
private:
	CAsterixMsg();
	bool  DecodeInternalMessage();
	typedef struct 
	{
		bool Presence;
		int  FirstPos;
		int  Len;
		int  Kind;
	} AsterixFld;
	BYTE * m_pMsg;
	int    m_TotalLen;
	AsterixFld m_FldTable[AlarmStatus+1];
	int m_Category;
	int m_CurPos;
	int m_BlockLen;
	int m_BlockPos;
};

#endif

