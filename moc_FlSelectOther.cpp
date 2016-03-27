/****************************************************************************
** CFlSelectOther meta object code from reading C++ file 'FlSelectOther.h'
**
** Created: Wed Sep 2 12:41:56 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "FlSelectOther.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CFlSelectOther::className() const
{
    return "CFlSelectOther";
}

QMetaObject *CFlSelectOther::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CFlSelectOther( "CFlSelectOther", &CFlSelectOther::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CFlSelectOther::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CFlSelectOther", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CFlSelectOther::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CFlSelectOther", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CFlSelectOther::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"PushButton0Pressed", 0, 0 };
    static const QUMethod slot_1 = {"PushButton1Pressed", 0, 0 };
    static const QUMethod slot_2 = {"PushButton2Pressed", 0, 0 };
    static const QUMethod slot_3 = {"PushButton3Pressed", 0, 0 };
    static const QUMethod slot_4 = {"PushButton4Pressed", 0, 0 };
    static const QUMethod slot_5 = {"PushButton5Pressed", 0, 0 };
    static const QUMethod slot_6 = {"PushButton6Pressed", 0, 0 };
    static const QUMethod slot_7 = {"PushButton7Pressed", 0, 0 };
    static const QUMethod slot_8 = {"PushButton8Pressed", 0, 0 };
    static const QUMethod slot_9 = {"PushButton9Pressed", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "textIni", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"LevelTextChanged", 1, param_slot_10 };
    static const QUMethod slot_11 = {"LevelReturnPressed", 0, 0 };
    static const QUMethod slot_12 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "PushButton0Pressed()", &slot_0, QMetaData::Public },
	{ "PushButton1Pressed()", &slot_1, QMetaData::Public },
	{ "PushButton2Pressed()", &slot_2, QMetaData::Public },
	{ "PushButton3Pressed()", &slot_3, QMetaData::Public },
	{ "PushButton4Pressed()", &slot_4, QMetaData::Public },
	{ "PushButton5Pressed()", &slot_5, QMetaData::Public },
	{ "PushButton6Pressed()", &slot_6, QMetaData::Public },
	{ "PushButton7Pressed()", &slot_7, QMetaData::Public },
	{ "PushButton8Pressed()", &slot_8, QMetaData::Public },
	{ "PushButton9Pressed()", &slot_9, QMetaData::Public },
	{ "LevelTextChanged(const QString&)", &slot_10, QMetaData::Public },
	{ "LevelReturnPressed()", &slot_11, QMetaData::Public },
	{ "languageChange()", &slot_12, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CFlSelectOther", parentObject,
	slot_tbl, 13,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CFlSelectOther.setMetaObject( metaObj );
    return metaObj;
}

void* CFlSelectOther::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CFlSelectOther" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CFlSelectOther::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: PushButton0Pressed(); break;
    case 1: PushButton1Pressed(); break;
    case 2: PushButton2Pressed(); break;
    case 3: PushButton3Pressed(); break;
    case 4: PushButton4Pressed(); break;
    case 5: PushButton5Pressed(); break;
    case 6: PushButton6Pressed(); break;
    case 7: PushButton7Pressed(); break;
    case 8: PushButton8Pressed(); break;
    case 9: PushButton9Pressed(); break;
    case 10: LevelTextChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 11: LevelReturnPressed(); break;
    case 12: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CFlSelectOther::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CFlSelectOther::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CFlSelectOther::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
