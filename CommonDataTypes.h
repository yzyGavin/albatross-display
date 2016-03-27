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
//  FILENAME: CommonDataTypes.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//	This class is used to represent a track identifier as SIC, SAC, TRACK NUMBER.
//	It also manages alert acknowledgement and classification.
// 
//  COMMENTS
//    
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

#if !defined(AFX_COMMONDATATYPES__INCLUDED_)
#define AFX_COMMONDATATYPES__INCLUDED_


//-INCLUDE FILES---------------------------------------------------------------
#include "StdAfx.h"
#include <qlineedit.h>


//-DATA------------------------------------------------------------------------
#define TssrData 0
#define AflData 1
#define GspdData 2
#define ArcidData 3
#define WtcData 4
#define ArctypData 5
#define RwyData 6
#define CflData 9
#define XptData 11
#define XflData 12
#define EtxData 13
#define VspdData 14
#define RvrData 15
#define FltrulData 16
#define RvsmData 17
#define N833Data 18
#define AlertData 19
#define CompData 20
#define RflData 21
#define AdesData 24
#define AdepData 25
#define AssrData 26
#define EmrgData 27
#define SidData 28
#define TssrPbData 29
#define MiData 30
#define FAttData 31
#define FlightCatData 32
#define AsterData 33
#define ArcidMismatch 35
#define SlashData 36
#define ModeSData 37
#define CoordData 38
#define FlightIdData 41
#define SEquippedData 42
#define MaxFld 43

#define LimitedUncoupledDb 0
#define FullAccDb 1
#define FullAppDb 2
#define TaggedLimitedUncoupledDb 3
#define ExtendedAccDb 4
#define ExtendedAppDb 5
#define LimitedDb 6
#define ExtendedTaggedDb 7

#define TIMEOUT_RADAR 4


typedef enum {SelClr,SelBkClr,FrameClr,AocClr,CoupledClr,UncoupledClr,DangerClr,WarningClr,HoldClr,InfoClr,TagClr,RocdClr,ModeSTag} ColorKinds;
extern LPCSTR  TrkColorNames[];
typedef enum {TrkSymbSel,TrkLabSel,TrkDef,TrkSymb,TrkPast,TrkLead,TrkArcid,TrkAfl,TrkHxy,TrkAls,Trk833,TrkEmg,TrkCfl,TrkVel,TrkRvsm,TrkTagged,TrkTssrPb,TrkCoord,TrkAtt,TrkMism,TrkRocd} ColorTyp;
typedef enum {UNCOUPLED,COUPLED,TAGGED,MODESTAGGED} AssociationType;


const QString LOCAL_AIRPORT = "EBBR";
const int DEFAULT_AFL = -100;
const int GARBLED_INVALID = -200;

enum SystemMode {NORMAL=0, FAILED, NBSYSTEMMODE};


typedef enum LogTypes{LogInformation=0,LogWarning=1,LogError=2} LogTypes;

typedef enum {NoTypeMsg,AliveMsg,RamldMsg,RamhdMsg,MsawMsg,ApwMsg,ClamMsg,StcaMsg} SafetyTypes;

typedef struct {
	int AlarmNb;
	BYTE sic, sac;
	int TrkNum1;
	int TrkNum2;
	int HSep;
	int VSep;
	char AreaName[8];
	QDateTime LastRcvTime;
} AlarmDef;


//-PROTOTYPES------------------------------------------------------------------
class CLogStub
{
public :
	CLogStub() {};
	virtual ~CLogStub() {};
	virtual void WriteLogMsg(const QString & strMessage, const LogTypes &Type) =0;
	virtual void WriteTraceBuffer(BYTE* buffer, int len, bool bOnOneLine, bool bReceive, sockaddr_in* addr) =0;
};

typedef struct
{
	bool bPresence;
	bool bAck;
} AlarmDesc ;

typedef struct
{
	int Number;
	QLabel* Name;
	QLabel* Status;
} ExtLineEdit ;

class CKey : public QObject
{
	friend class CKeyList;
	friend class QPtrList<CKey>;
	friend class CKeyListIterator;
private :
	int m_Key1;
	int m_Key2;
	int m_Key3;
	bool m_Mark;
	bool m_Ack;
	int m_Order;
	QDateTime m_LastUpdate;
private :
	CKey(int key1, int key2, int key3, bool Ack=false, int order = -1);
	~CKey();
	int GetElapsedRefreshTime();
	void UpdateTime();
};


class CKeyList
{
	friend class CKeyListIterator;
private :
	//List of keys
	QPtrList<CKey> m_Keys;
	//This method returns the key associated to the keys put in argument
	CKey* FindKey(int key1, int key2, int key3 = -1);
public :
	//Constructor
	CKeyList();
	//Destructor
	virtual ~CKeyList();
	//this method returns the number of objects stored
	int GetCount();
	//This method removes the object associated to the following keys
	bool RemoveKey(int key1, int key2, int key3);
	//This method adds a Key to the list
	void AddKey(int key1, int key2, int key3, int order=-1);
	//This method returns whether the list is empty or not
	bool IsEmpty();
	//This method returns the track number (Key3) associated to the keys put in argument
	int GetKey(int key1, int key2);
	//This method returns the elapsed time between the current time and the last
	//update time.
	int GetElapsedRefreshTime(int key1, int key2, int key3);
	//This method empties the list
	void RemoveAll();
	//This method copies the list in attribute in the current list
	void Copy(const CKeyList& src);
	//This method sets the Ack flag of an entry
	bool Ack(int key1, int key2, int key3, bool Ack=true);
	//This method resets the update marker for all entries in the list
	void UnMarkAll();
	//This method removes the entries that are no more updated
	void RemoveUnMarked();
};

class CKeyListIterator
{
private :
	//Iterator object
	QPtrListIterator<CKey> m_It;
public:
	//Constructor
    CKeyListIterator(const CKeyList &l);
	//Destructor
    virtual ~CKeyListIterator();
	//Redefinition of the current() function 
    bool current(int *key1, int *key2, int *key3=NULL, bool* Ack=NULL, int* order=NULL);
	//Redefinition of the ++ operation (iteration in the list)
    void operator++();
};

typedef struct {
	int		m_SSR;
	bool	m_IsCoupled;
	bool	m_IsTagged;
	QString m_CsTag;
	QString m_Arcid;
	bool	m_IsAoc;
	char	m_Mnemo;
	QString m_Sid;
	char	m_A;
	int		m_CurAlt;
	QPoint  m_CurPos;	

	QString m_strArcId;
	QString m_strSSR;
	QString m_strAFL;

	bool m_IsToggled;
	bool m_bAck;
	bool m_bDisplayArcId;
	bool m_bPresenseStca;

}AlarmListData; // this structure stores all information contained in the alert list.

typedef struct {
	unsigned int  m_SSR;
	bool m_IsHighlight;
	bool m_Mismatch;
	QString m_ConflictingArcid;
	bool m_ConflictHasBeenSolved;
	AssociationType m_ConflictingAssociation;
	bool m_IsCoupled;
	bool m_IsTagged;
	AssociationType m_AssociationType;
	QString m_CsTag;
	QString m_Arcid;
	bool m_IsManUntag;
	QString m_Wtc;
	bool m_IsDeparture;
	bool m_IsArrival;
	bool m_IsAoc;
	char m_Mnemo;
	QString m_Adep;
	QString m_Ades;
	QString m_Cfl;
	QString m_ASpeed;
	QString m_ArrRwy;
	QString m_ParallelRwy;
	QString m_Arctyp;
	QString m_FltRul;
	QString m_Xpt;
	QString m_Xfl;
	QString m_Etx;
	QString m_Rvr;
	QString m_Rfl;
	QString m_Sid;
	QString m_ASsr;
	QString m_Route;
	QString m_833;
	QString m_Rvsm;
	QString m_FplCat;
	QString m_Pfl;
	QString m_DepRwy;
	QString m_Altrnt1;
	QString m_Altrnt2;
	QString m_FlightId;
	QString m_CurFix;
	QString m_KdOfAppClear;
	bool	m_SStatus;
	float   m_TrkRcd;
	uchar	m_TrkFlMod;
	bool	m_FlModConflict;
	bool    m_IsTrkRcd;
	int     m_RocdFtMin;
	int     m_RocdFtNm;
	int		m_Speed;
	bool	m_IsPrimary;
	bool	m_IsSecondary;
	int		m_CurAlt;
	bool	m_IsAfl;
	bool	m_bNegatifAfl;
	QSize	m_Velocity;
	QSize	m_RealVelocity;
	bool	m_EmergAck;
	QString  m_Eobt;
	QString  m_NextSectorNameOverFlown;
} TrackData; // this structure stores all the informations of a tracks. It is also used by LocalTagMngr to store local information concening a track.


#endif

//--------------------------------< THE END >----------------------------------

