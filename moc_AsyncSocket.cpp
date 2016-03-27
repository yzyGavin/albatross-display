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
/****************************************************************************
** CAsyncSocket meta object code from reading C++ file 'AsyncSocket.h'
**
** Created: Tue Sep 1 14:56:34 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "AsyncSocket.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CAsyncSocket::className() const
{
    return "CAsyncSocket";
}

QMetaObject *CAsyncSocket::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CAsyncSocket( "CAsyncSocket", &CAsyncSocket::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CAsyncSocket::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CAsyncSocket", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CAsyncSocket::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CAsyncSocket", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CAsyncSocket::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"ReceiveData", 0, 0 };
    static const QUMethod slot_1 = {"SendData", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "ReceiveData()", &slot_0, QMetaData::Private },
	{ "SendData()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"CAsyncSocket", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CAsyncSocket.setMetaObject( metaObj );
    return metaObj;
}

void* CAsyncSocket::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CAsyncSocket" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool CAsyncSocket::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: ReceiveData(); break;
    case 1: SendData(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CAsyncSocket::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CAsyncSocket::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool CAsyncSocket::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
