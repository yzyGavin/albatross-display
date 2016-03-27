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
//  FILENAME: StdAfx.h
//  TYPE    : header file
// 
//-----------------------------------------------------------------------------
// 
//  DESCRIPTION 
//		This file contains type declarations used by any other files.
//				
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
#if !defined(STDAFX_H)
#define STDAFX_H

//-INCLUDE FILES---------------------------------------------------------------

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "qpoint.h"
#include "qsize.h"
#include "qrect.h"
#include "qpainter.h"
#include "qwidget.h"
#include "qbitmap.h"
#include "qdatetime.h"
#include "qpopupmenu.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qvaluevector.h"
#include "qdir.h"
#include "qregexp.h"
#include "qsocketnotifier.h" 
#include "qcursor.h"
#include "qbutton.h"
#include "qcheckbox.h"
#include "qdesktopwidget.h"
#include "qapplication.h"
#include "qstatusbar.h"
#include "qlabel.h"
#include "SKMessageBox.h"
#include "qlistbox.h"
#include "qthread.h"
#include <time.h>


//-GLOBAL DATA-----------------------------------------------------------------

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL			    *PBOOL;
typedef BOOL		        *LPBOOL;
typedef BYTE			    *PBYTE;
typedef BYTE		        *LPBYTE;
typedef int		            *PINT;
typedef int					*LPINT;
typedef WORD			    *PWORD;
typedef WORD				*LPWORD;
typedef long			    *LPLONG;
typedef DWORD				*PDWORD;
typedef DWORD			    *LPDWORD;
typedef void	            *LPVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef const char *LPCSTR, *PCSTR;
typedef char *LPSTR, *PSTR;

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function retrieves the high-order byte from the given integer value
///		Argument: I is an integer
/// 
///  RETURNS: 
///		a BYTE
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
inline BYTE INT2SAC(int I)
{
	return ((I & 0xFF00) >> 8);
}

//-----------------------------------------------------------------------------
// 
///  DESCRIPTION: 
///		This function retrieves the low-order byte from the given integer value
///		Argument: I is an integer
/// 
///  RETURNS: 
///		a BYTE
///
///  REQUIREMENT ID: None
//
//-----------------------------------------------------------------------------
inline BYTE INT2SIC(int I)
{
	return (I & 0x00FF);
}

#if (defined Q_WS_WIN)
#include <winsock2.h>
#endif
#if (defined Q_WS_X11)
#include <netdb.h>
#endif


//-CLASSES DEFINITIONS---------------------------------------------------------

#include "./CommonFiles/CommonObject.h"
#include "./CommonFiles/CommonMenu.h"
#include "./CommonFiles/Templ.h"
#endif

//#ifdef _DEBUG
//#define ENABLE_QDEBUG
//#endif

//--------------------------------< THE END >----------------------------------
