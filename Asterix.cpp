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
//  FILENAME: Asterix.cpp
//  TYPE    : c++ code-file
// 
//
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//    This file contains the Asterix Data management
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
#include "StdAfx.h"
#include "Asterix.h"

	typedef enum {fix,var,rep,exl,sub} typfld;
	typedef enum {intval,strval,pospol,posxy,bytearray,vectors} kindfld;

	typedef struct {
		int len;
		typfld typ;
		AsterixFields fld;
		kindfld kind;
	} itemdef;

	//cat 01
	const int FldNbr01=23;
	const itemdef UAP01[FldNbr01]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{1,var,TrkRep,bytearray},{2,fix,TrkNum,intval},{4,fix,TrkPosPol,pospol},{4,fix,TrkPos,posxy},{4,fix,TrkSpd,pospol},{2,fix,TrkSsr,intval},
														{2,fix,TrkAlt,intval},{2,fix,NoFld,intval},{1,var,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,var,TrkStatus,bytearray},{1,var,NoFld,bytearray},
														{2,fix,NoFld,intval},{2,fix,NoFld,intval},{4,fix,NoFld,intval},{2,fix,NoFld,intval},{1,var,NoFld,intval},{1,exl,NoFld,intval},{0,fix,NoFld,intval},
														{1,fix,NoFld,intval}};
	const int FldNbr01P=22;
	const itemdef UAP01P[FldNbr01P]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{1,var,TrkRep,bytearray},{4,fix,TrkPosPol,pospol},{2,fix,TrkSsr,intval},{2,fix,TrkAlt,intval},{1,var,NoFld,intval},{2,fix,NoFld,intval},
														{2,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{4,fix,NoFld,intval},{2,fix,NoFld,intval},{1,var,NoFld,intval},
														{1,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{1,exl,NoFld,intval},{0,fix,NoFld,intval}};
	const int FldNbr02=12;
	const itemdef UAP02[FldNbr02]={{0,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,MsgType,intval},{1,fix,NoFld,intval},{3,fix,NoFld,pospol},{2,fix,NoFld,posxy},{1,var,NoFld,pospol},{1,var,NoFld,intval},
														{2,rep,NoFld,intval},{8,fix,NoFld,intval},{2,fix,NoFld,intval},{1,var,NoFld,intval}};
	//cat 30 Artas
	const int FldNbr30=53;
	const itemdef UAP30[FldNbr30]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{2,fix,NoFld,intval},{1,var,NoFld,intval},{1,fix,NoFld,intval},{2,fix,TrkNum,intval},{3,fix,NoFld,intval},{4,fix,NoFld,intval},
														{4,fix,TrkPos,posxy},{4,fix,NoFld,intval},{4,fix,TrkSpd,posxy},{2,fix,TrkSsr,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,TrkAlt,intval},
														{1,var,TrkStatus,bytearray},{1,fix,NoFld,intval},{1,fix,TrkFlMod,intval},{2,fix,TrkVSpd,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,TrkRad,intval},
														{4,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{3,exl,NoFld,intval},{2,fix,NoFld,intval},{7,fix,TrkCls,strval},{2,fix,NoFld,intval},
														{4,fix,TrkAdep,intval},{4,fix,TrkAdes,intval},{1,fix,NoFld,intval},{4,fix,TrkAtyp,intval},{2,rep,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},
														{2,fix,NoFld,intval},{3,fix,NoFld,intval},{3,fix,TrkAddr,intval},{6,fix,TrkAircraft,bytearray},{1,fix,NoFld,intval},{4,fix,NoFld,intval},{4,fix,NoFld,intval},
														{4,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{3,fix,NoFld,intval},{2,fix,NoFld,intval},
														{3,var,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval}};

	
	//cat 48 Mode S
	const int SubFldNbr48_130=7;
	const itemdef SubUAP48_130[SubFldNbr48_130]={{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval}};

	const int SubFldNbr48_120=7;
	const itemdef SubUAP48_120[SubFldNbr48_120]={{2,fix,NoFld,intval},{6,rep,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval}};

	const int FldNbr48=29;
	const itemdef UAP48[FldNbr48]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{3,fix,NoFld,intval},{1,var,TrkRep,bytearray},{4,fix,TrkPosPol,pospol},{2,fix,TrkSsr,intval},{2,fix,TrkAlt,intval},{(int)&SubUAP48_130,sub,NoFld,bytearray},
														{3,fix,TrkAddr,intval},{6,fix,TrkAircraft,bytearray},{8,rep,NoFld,intval},{2,fix,TrkNum,intval},{4,fix,TrkPos,posxy},{4,fix,TrkSpd,pospol},{1,var,TrkStatus,bytearray},
														{4,fix,NoFld,intval},{1,var,NoFld,intval},{2,fix,NoFld,intval},{4,fix,NoFld,intval},{2,fix,NoFld,intval},{(int)&SubUAP48_120,sub,NoFld,intval},{2,fix,NoFld,intval},
														{7,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{1,var,NoFld,intval},{0,fix,NoFld,intval}};
		
	//cat 34 Mode S service
	const int SubFldNbr34_050=7;
	const itemdef SubUAP34_050[SubFldNbr34_050]={{1,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{0,fix,NoFld,intval}};

	const int SubFldNbr34_060=7;
	const itemdef SubUAP34_060[SubFldNbr34_060]={{1,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{0,fix,NoFld,intval}};

	const int FldNbr34=15;
	const itemdef UAP34[FldNbr34]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{1,fix,MsgType,intval},{3,fix,NoFld,bytearray},{1,fix,NoFld,pospol},{2,fix,NoFld,intval},{(int)&SubUAP34_050,sub,NoFld,intval},{(int)&SubUAP34_060,sub,NoFld,intval},
														{2,rep,NoFld,intval},{8,fix,NoFld,intval},{1,fix,NoFld,intval},{8,fix,NoFld,intval},{2,fix,NoFld,intval},{1,sub,NoFld,intval},{1,sub,NoFld,intval}};
	//cat 08 Weather
	const int FldNbr08=13;
	const itemdef UAP08[FldNbr08]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{1,fix,MsgType,intval},{1,var,WthrLevel,intval},{3,rep,WthrCartVectors,bytearray},{4,rep,WthrPolarVectors,bytearray},{2,fix,NoFld,intval},{2,rep,NoFld,intval},
									{3,fix,NoFld,intval},{3,var,WthrScale,bytearray},{1,var,NoFld,intval},{2,fix,WthrTotal,intval},{4,rep,NoFld,intval}};
	//cat 04 Safety
	const int FldNbr04=29;
	const itemdef UAP04[FldNbr04]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{1,fix,MsgType,intval},{2,fix,NoFld,intval},{3,fix,NoFld,intval},{2,fix,TrkNb1,intval},{2,fix,AlarmNb,intval},{1,var,AlarmState,bytearray},
														{2,fix,VSep,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{6,fix,NoFld,intval},{2,fix,TrkNb2,intval},{1,var,AlarmStatus,bytearray},{3,fix,NoFld,intval},
														{2,fix,HSep,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{7,fix,Callsign,strval},{7,fix,NoFld,strval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},
														{2,fix,NoFld,intval},{1,var,NoFld,intval},{1,var,NoFld,intval},{1,fix,NoFld,intval},{6,fix,NoFld,intval},{1,fix,NoFld,intval},{6,fix,NoFld,intval}};
	//cat 252 Service Message
	const int FldNbr252=46;
	const itemdef UAP252[FldNbr252]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{2,fix,NoFld,intval},{3,fix,NoFld,intval},{1,fix,MsgType,intval},{1,var,NoFld,intval},{2,rep,NoFld,bytearray},{2,var,NoFld,intval},
															{5,fix,NoFld,intval},{4,fix,NoFld,intval},{2,rep,NoFld,intval},{2,rep,NoFld,intval},{7,rep,NoFld,intval},{4,rep,NoFld,intval},{4,rep,NoFld,intval},
															{6,rep,NoFld,intval},{4,fix,NoFld,intval},{2,fix,NoFld,intval},{3,var,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,var,NoFld,intval},
															{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},
															{1,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{3,rep,NoFld,intval},{6,rep,NoFld,intval},{2,rep,NoFld,intval},{2,fix,NoFld,intval},
															{2,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{3,fix,NoFld,intval},{2,fix,NoFld,intval},
															{2,fix,NoFld,intval},{3,fix,NoFld,intval},{2,fix,NoFld,intval}};
	const int FldNbr252C=14;
	const itemdef UAP252C[FldNbr252C]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{2,fix,NoFld,intval},{3,fix,NoFld,intval},{1,fix,MsgType,intval},{1,rep,NoFld,intval},{8,fix,NoFld,intval},{1,fix,NoFld,intval},
															{1,fix,NoFld,intval},{6,rep,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval}};
	//cat 242
	const int FldNbr242=92;
	const itemdef UAP242[FldNbr242]={{0,fix,NoFld,intval},{2,fix,TrkSource,intval},{3,fix,NoFld,intval},{2,fix,TrkNum,intval},{2,fix,NoFld,intval},{2,var,TrkStatus,bytearray},{2,var,NoFld,intval},{2,var,TrkMsaw,bytearray},
														{2,var,TrkCa,bytearray},{2,var,TrkFplSts,bytearray},{8,fix,TrkPos,posxy},{2,fix,TrkArea,intval},{4,fix,TrkSpd,posxy},{4,fix,NoFld,intval},{4,fix,TrkAlt,intval},
														{2,fix,NoFld,intval},{2,fix,TrkVSpd,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,TrkSsr,intval},{2,fix,TrkUrg,intval},{2,fix,NoFld,intval},
														{2,fix,TrkRad,strval},{1,rep,TrkAcd,bytearray},{1,rep,NoFld,bytearray},{1,rep,NoFld,intval},{4,fix,NoFld,intval},{1,rep,NoFld,intval},{4,fix,NoFld,intval},
														{1,rep,TrkCaSect,bytearray},{4,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{4,fix,NoFld,intval},{4,fix,NoFld,intval},{2,var,NoFld,intval},
														{2,var,NoFld,intval},{1,rep,NoFld,intval},{1,rep,NoFld,intval},{2,rep,NoFld,intval},{2,rep,NoFld,intval},{1,exl,NoFld,intval},{1,exl,NoFld,intval},
														{2,exl,NoFld,intval},{2,exl,NoFld,intval},{2,exl,NoFld,intval},{2,exl,TrkCls,strval},{1,fix,TrkSector,intval},{1,fix,NoFld,intval},{1,rep,NoFld,intval},
														{1,fix,TrkHndIn,intval},{1,fix,TrkHndLogIn,intval},{1,rep,NoFld,intval},{2,fix,Trksi,strval},{4,fix,NoFld,intval},{1,fix,NoFld,intval},{1,exl,NoFld,intval},
														{1,exl,NoFld,intval},{1,exl,NoFld,intval},{1,exl,NoFld,intval},{1,exl,NoFld,intval},{1,exl,Trkxpt,strval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},
														{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,Trkxfl,intval},{2,fix,NoFld,intval},{4,fix,NoFld,intval},{4,fix,NoFld,intval},{2,rep,NoFld,intval},
														{2,fix,NoFld,intval},{3,fix,NoFld,intval},{8,fix,TrkAircraft,strval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{4,fix,NoFld,intval},
														{4,fix,NoFld,intval},{2,var,NoFld,intval},{2,var,NoFld,intval},{1,rep,NoFld,intval},{1,rep,NoFld,intval},{2,rep,NoFld,intval},{2,rep,NoFld,intval},
														{1,exl,NoFld,intval},{1,exl,NoFld,intval},{1,exl,NoFld,intval},{1,exl,NoFld,intval},{1,exl,NoFld,intval},{1,exl,NoFld,intval},{1,exl,NoFld,intval}};
		
	const int FldNbr243=15;
	const itemdef UAP243[FldNbr243]={{0,fix,NoFld,intval},{2,fix,NoFld,intval},{3,fix,NoFld,intval},{2,fix,MsgType,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{4,fix,NoFld,intval},{2,var,NoFld,intval},
															{1,rep,NoFld,intval},{2,rep,NoFld,intval},{0,exl,NoFld,intval},{2,exl,NoFld,intval},{2,exl,NoFld,intval},{1,exl,NoFld,intval},{1,exl,NoFld,intval}};

	//cat 62 new Artas

	const int SubFldNbr62_380=28;
	const itemdef SubUAP62_380[SubFldNbr62_380]={{3,fix,TrkAddr,intval},{6,fix,TrkAircraft,bytearray},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},
												{1,var,NoFld,intval},{15,rep,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{7,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},
												{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{8,fix,NoFld,intval},{1,fix,NoFld,intval},
												{6,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{8,rep,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval}};

	const int SubFldNbr62_290=10;
	const itemdef SubUAP62_290[SubFldNbr62_290]={{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},
												{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval}};
	const int SubFldNbr62_295=31;
	const itemdef SubUAP62_295[SubFldNbr62_295]={{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},
												{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},
												{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},
												{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},
												{1,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval}};

	const int SubFldNbr62_390=18;
	const itemdef SubUAP62_390[SubFldNbr62_390]={{2,fix,NoFld,intval},{7,fix,TrkCls,strval},{4,fix,NoFld,intval},{1,fix,NoFld,intval},{4,fix,NoFld,intval},{1,fix,NoFld,intval},{4,fix,NoFld,intval},
												{4,fix,NoFld,intval},{3,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{4,rep,NoFld,intval},{6,fix,NoFld,intval},{1,fix,NoFld,intval},
												{7,fix,NoFld,intval},{7,fix,NoFld,intval},{2,fix,NoFld,intval},{7,fix,NoFld,intval}};


	const int SubFldNbr62_110=7;
	const itemdef SubUAP62_110[SubFldNbr62_110]={{1,fix,NoFld,intval},{4,fix,NoFld,intval},{6,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval}};

	const int SubFldNbr62_500=8;
	const itemdef SubUAP62_500[SubFldNbr62_500]={{4,fix,NoFld,intval},{2,fix,NoFld,intval},{4,fix,NoFld,intval},{1,fix,NoFld,intval},{1,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},
												{1,fix,NoFld,intval}};

	const int SubFldNbr62_340=7;
	const itemdef SubUAP62_340[SubFldNbr62_340]={{2,fix,NoFld,intval},{4,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{1,fix,NoFld,intval}};

	const int FldNbr62=36;
	const itemdef UAP62[FldNbr62]={{0,fix,NoFld,intval},{2,fix,SicSac,intval},{0,fix,NoFld,intval},{1,fix,NoFld,intval},{3,fix,NoFld,intval},{8,fix,NoFld,intval},{6,fix,TrkPos,posxy},{4,fix,TrkSpd,posxy},
														{2,fix,NoFld,intval},{2,fix,TrkSsr,intval},{7,fix,NoFld,posxy},{(int)&SubUAP62_380,sub,NoFld,intval},{2,fix,TrkNum,intval},{1,var,TrkStatus,bytearray},{(int)&SubUAP62_290,sub,NoFld,bytearray},
														{1,fix,NoFld,intval},{(int)&SubUAP62_295,sub,NoFld,intval},{2,fix,NoFld,intval},{2,fix,NoFld,intval},{2,fix,TrkAlt,intval},{2,fix,TrkVSpd,intval},{(int)&SubUAP62_390,sub,NoFld,intval},
														{1,var,NoFld,intval},{1,fix,NoFld,intval},{(int)&SubUAP62_110,sub,NoFld,bytearray},{2,fix,NoFld,intval},{3,var,NoFld,intval},{(int)&SubUAP62_500,sub,NoFld,bytearray},{(int)&SubUAP62_340,sub,NoFld,bytearray},
														{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval},{0,fix,NoFld,intval}};


	const int MaxFldNbr=FldNbr242;

	CAsterixMsg::CAsterixMsg()
	{
		m_CurPos=0;
		m_BlockPos=0;
		m_BlockLen=0;
	}

	CAsterixMsg::~CAsterixMsg()
	{
		delete m_pMsg;
	}

	CAsterixMsg::CAsterixMsg(BYTE* pMsg,int TotalLen)
	{
		m_pMsg = new BYTE[TotalLen];
		for (int i=0;i<TotalLen;i++)
			m_pMsg[i]=pMsg[i];
		m_TotalLen=TotalLen;
		m_CurPos=0;
		m_BlockPos=0;
		m_BlockLen=0;
	}

bool CAsterixMsg::DecodeInternalMessage()
{
	bool fldexist[MaxFldNbr];
	int fldnbr=0,i;
	const itemdef* UAP=NULL,*SubUAP=NULL;
	int curpos=m_CurPos,curfld=1,fld,subfldnbr;
	int subfldpos[35];
	int subfldlen[35];
	switch (m_Category)
	{
	case 1:
		fldnbr=FldNbr01;
		UAP=UAP01;
		break;
	case 2:
		fldnbr=FldNbr02;
		UAP=UAP02;
		break;
	case 4:
		fldnbr=FldNbr04;
		UAP=UAP04;
		break;
	case 8:
		fldnbr=FldNbr08;
		UAP=UAP08;
		break;
	case 30:
		fldnbr=FldNbr30;
		UAP=UAP30;
		break;
	case 34:
		fldnbr=FldNbr34;
		UAP=UAP34;
		break;
	case 48:
		fldnbr=FldNbr48;
		UAP=UAP48;
		break;
	case 62:
		fldnbr=FldNbr62;
		UAP=UAP62;
		break;
	case 242:
		fldnbr=FldNbr242;
		UAP=UAP242;
		break;
	case 243:
		fldnbr=FldNbr243;
		UAP=UAP243;
		break;
	case 252:
		fldnbr=FldNbr252;
		UAP=UAP252;
		break;
	}
	for (i=0;i<fldnbr;i++)
		fldexist[i] = false;
	int lenfld=0;
	int tmppos,tmppos2;
	//decode fspec
	do
	{
		for (i=0;i<7;i++)
		{
			if (m_pMsg[curpos]&(1<<(7-i)))
				fldexist[curfld]=true;
			curfld++;
		}
	}
	while (m_pMsg[curpos++]&1);
	for (fld=1;fld<fldnbr;fld++)
	{
		if (fldexist[fld])
		{
			switch (UAP[fld].typ)
			{
			case fix:
				lenfld=UAP[fld].len;
				break;
			case var:
				lenfld=UAP[fld].len;
				while (m_pMsg[curpos+lenfld-1]&1)
					lenfld+=UAP[fld].len;
				break;
			case rep:
				lenfld=1+m_pMsg[curpos]*UAP[fld].len;
				break;
			case exl:
				lenfld = m_pMsg[curpos];
				break;
			case sub:
				subfldnbr=0;
				tmppos=curpos;
				tmppos2=curpos+1;
				while (m_pMsg[tmppos2-1]&1)
					tmppos2++;
				lenfld=0;
				SubUAP=(itemdef*)UAP[fld].len;
				do
				{
					for (int i=subfldnbr;i<subfldnbr+7;i++)
					{
						subfldpos[i]=-1;
						if (m_pMsg[tmppos]&(1<<(7-i)))
						{
							subfldpos[i]=tmppos2;
							switch (SubUAP[i].typ)
							{
							case fix:
								subfldlen[i]=SubUAP[i].len;
								lenfld+=SubUAP[i].len;
								tmppos2+=SubUAP[i].len;
								break;
							case var:
								subfldlen[i]=SubUAP[i].len;
								lenfld+=SubUAP[i].len;
								tmppos2+=SubUAP[i].len;
								while (m_pMsg[tmppos2+SubUAP[i].len-1]&1)
								{
									subfldlen[i]=SubUAP[i].len;
									lenfld+=SubUAP[i].len;
									tmppos2+=SubUAP[i].len;
								}
								break;
							case rep:
								{
									subfldlen[i]=1+m_pMsg[tmppos2]*SubUAP[i].len;
									lenfld=1+m_pMsg[tmppos2]*SubUAP[i].len;
									tmppos2+=1+m_pMsg[tmppos2]*SubUAP[i].len;
								}
								break;
							}
						}
					}
					subfldnbr+=7;
					tmppos++;
					lenfld++;
				}
				while (m_pMsg[tmppos-1]&1);
				break;
			}

			if ((m_Category==1) && (UAP[fld].fld==TrkRep) && ((m_pMsg[curpos] & 0x80)!=0x80))
			{
				UAP=UAP01P;
				fldnbr=FldNbr01P;
			}
			if ((m_Category==252) && (UAP[fld].fld==MsgType))
			{
				if ((m_pMsg[curpos]==0x11) || (m_pMsg[curpos]==0x12) || (m_pMsg[curpos]==0x13))
				{	UAP=UAP252C;
					fldnbr=FldNbr252C;
				}
			}
			if (UAP[fld].fld!=NoFld)
			{
				m_FldTable[UAP[fld].fld].Presence=true;
				if (UAP[fld].typ==exl)
				{
					m_FldTable[UAP[fld].fld].FirstPos=curpos+1;
					m_FldTable[UAP[fld].fld].Len=lenfld-1;
				}
				else
				{
					m_FldTable[UAP[fld].fld].FirstPos=curpos;
					m_FldTable[UAP[fld].fld].Len=lenfld;
				}
				m_FldTable[UAP[fld].fld].Kind=UAP[fld].kind;
			}
			if (UAP[fld].typ==sub)
			{
				for (int i=0;i<subfldnbr;i++)
				{
					if ((subfldpos[i]!=-1) && (SubUAP[i].fld!=NoFld))
					{
						m_FldTable[SubUAP[i].fld].Presence=true;
						if (SubUAP[i].typ==exl)
						{
							m_FldTable[SubUAP[i].fld].FirstPos=subfldpos[i]+1;
							m_FldTable[SubUAP[i].fld].Len=subfldlen[i]-1;
						}
						else
						{
							m_FldTable[SubUAP[i].fld].FirstPos=subfldpos[i];
							m_FldTable[SubUAP[i].fld].Len=subfldlen[i];
						}
						m_FldTable[SubUAP[i].fld].Kind=SubUAP[i].kind;
					}
				}
			}
			curpos+=lenfld;
		}
	}
	m_CurPos=curpos;
	return true;
}


bool CAsterixMsg::DecodeMessage()
{
	if (m_CurPos==m_TotalLen) 
		return false;
	if ((m_CurPos>=m_TotalLen) || (m_CurPos>m_BlockPos+m_BlockLen))
		return false;
	//first message, find category
	if (m_CurPos==m_BlockPos+m_BlockLen)
	{
		m_Category=m_pMsg[m_CurPos];
		m_BlockLen=m_pMsg[m_CurPos+1]*256+m_pMsg[m_CurPos+2];
		m_BlockPos=m_CurPos;
		m_CurPos+=3;
	}
	for (int i=0;i<AlarmStatus+1;i++)
		m_FldTable[i].Presence=false;
	return DecodeInternalMessage();

}

int CAsterixMsg::GetCategory()
{
	return m_Category;
}

bool CAsterixMsg::IsFieldPresent(AsterixFields field)
{
	return m_FldTable[field].Presence;
}

int CAsterixMsg::GetUIntVal(AsterixFields field)
{
	int CurVal=0;
	if ((m_FldTable[field].Presence) && (m_FldTable[field].Kind==intval))
	{
		for (int i=0;i<m_FldTable[field].Len;i++)
		{
			CurVal<<=8;
			CurVal+=m_pMsg[m_FldTable[field].FirstPos+i];
		}
	}
	return CurVal;
}
int CAsterixMsg::GetIntVal(AsterixFields field)
{
	int CurVal=0;
	bool neg=false;
	int mask=0xFFFFFFFF;
	if ((m_FldTable[field].Presence) && (m_FldTable[field].Kind==intval))
	{
		for (int i=0;i<m_FldTable[field].Len;i++)
		{
			CurVal<<=8;
			CurVal+=m_pMsg[m_FldTable[field].FirstPos+i];
			if ((i==0) && (CurVal & 0x80))
				neg=true;
			if (neg)
				mask<<=8;
		}
	}
	if (neg)
		return (CurVal|mask);
	else
		return CurVal;
}

CString CAsterixMsg::GetStrVal(AsterixFields field)
{
	CString str;
	if ((m_FldTable[field].Presence) && (m_FldTable[field].Kind==strval))
	{
		for (int i=0;i<m_FldTable[field].Len;i++)
			str+=m_pMsg[m_FldTable[field].FirstPos+i];
	}
	return str;
}

BYTE CAsterixMsg::GetByteVal(AsterixFields field,int bytenb)
{
	if ((m_FldTable[field].Presence) && (m_FldTable[field].Kind==bytearray))
	{
		if (bytenb<m_FldTable[field].Len)
			return m_pMsg[m_FldTable[field].FirstPos+bytenb];
	}
	return 0;
}

int CAsterixMsg::GetVectorsTotal(AsterixFields field)
{
	if ((m_FldTable[field].Presence) && (m_FldTable[field].Kind==vectors))
	{
			return m_pMsg[m_FldTable[field].FirstPos];
	}
	return 0;
}
	

PolarVectors CAsterixMsg::GetVectorNb(AsterixFields field,int VectorNb)
{
	PolarVectors CurVector={0,0,0};
	if ((m_FldTable[field].Presence) && (m_FldTable[field].Kind==vectors))
	{
		int pos=m_FldTable[field].FirstPos+1+4*VectorNb;
		CurVector.start_range=m_pMsg[pos];
		CurVector.end_range=m_pMsg[pos+1];
		CurVector.azimuth=(m_pMsg[pos+2]<<8)+m_pMsg[pos+3];
	}
	return CurVector;
}


PolarCoord CAsterixMsg::GetPolarCoord(AsterixFields field)
{
	PolarCoord CurCoord={0,0};
	if ((m_FldTable[field].Presence) && (m_FldTable[field].Kind==pospol))
	{
		int pos=m_FldTable[field].FirstPos;
		CurCoord.Range=(m_pMsg[pos]<<8)+m_pMsg[pos+1];
		CurCoord.Azimuth=(m_pMsg[pos+2]<<8)+m_pMsg[pos+3];
	}
	return CurCoord;
}

QPoint CAsterixMsg::GetPos(AsterixFields field)
{
	QPoint CurPos=QPoint(0,0);
	int x,y;
	if ((m_FldTable[field].Presence) && (m_FldTable[field].Kind==posxy))
	{
		if (m_FldTable[field].Len==4)
		{
		int pos=m_FldTable[field].FirstPos;
		x=(m_pMsg[pos]<<8)+m_pMsg[pos+1];
		if (x & (1<<15)) 
			x |= 0xFFFF0000;
		y=(m_pMsg[pos+2]<<8)+m_pMsg[pos+3];
		if (y & (1<<15)) 
			y |= 0xFFFF0000;
		CurPos=QPoint(x,y);
		}
		if (m_FldTable[field].Len==6)
		{
		int pos=m_FldTable[field].FirstPos;
		x=(m_pMsg[pos]<<16)+(m_pMsg[pos+1]<<8)+m_pMsg[pos+2];
		if (x & (1<<23)) 
			x |= 0xFF000000;
		y=(m_pMsg[pos+3]<<16)+(m_pMsg[pos+4]<<8)+m_pMsg[pos+5];
		if (y & (1<<23)) 
			y |= 0xFF000000;
		CurPos=QPoint(x,y);
		}
		if (m_FldTable[field].Len==8)
		{
		int pos=m_FldTable[field].FirstPos;
		x=(m_pMsg[pos]<<24)+(m_pMsg[pos+1]<<16)+(m_pMsg[pos+2]<<8)+m_pMsg[pos+3];
		if (x & (1<<31)) 
			x |= 0x80000000;
		y=(m_pMsg[pos+4]<<24)+(m_pMsg[pos+5]<<16)+(m_pMsg[pos+6]<<8)+m_pMsg[pos+7];
		if (y & (1<<31)) 
			y |= 0x80000000;
		CurPos=QPoint(x,y);
		}
		
	}
	return CurPos;
}

int CAsterixMsg::GetFieldLength(AsterixFields field)
{
	if (m_FldTable[field].Presence)
		return m_FldTable[field].Len;
	return 0;
}
