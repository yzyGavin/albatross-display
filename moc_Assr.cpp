/****************************************************************************
** CAssr meta object code from reading C++ file 'Assr.h'
**
** Created: Wed Sep 2 12:41:43 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "Assr.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CAssr::className() const
{
    return "CAssr";
}

QMetaObject *CAssr::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CAssr( "CAssr", &CAssr::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CAssr::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CAssr", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CAssr::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CAssr", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CAssr::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_0 = {"mouseMoveEvent", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "e", &static_QUType_ptr, "QTimerEvent", QUParameter::In }
    };
    static const QUMethod slot_1 = {"timerEvent", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_2 = {"mousePressEvent", 1, param_slot_2 };
    static const QUMethod slot_3 = {"PushButton0Pressed", 0, 0 };
    static const QUMethod slot_4 = {"PushButton1Pressed", 0, 0 };
    static const QUMethod slot_5 = {"PushButton2Pressed", 0, 0 };
    static const QUMethod slot_6 = {"PushButton3Pressed", 0, 0 };
    static const QUMethod slot_7 = {"PushButton4Pressed", 0, 0 };
    static const QUMethod slot_8 = {"PushButton5Pressed", 0, 0 };
    static const QUMethod slot_9 = {"PushButton6Pressed", 0, 0 };
    static const QUMethod slot_10 = {"PushButton7Pressed", 0, 0 };
    static const QUMethod slot_11 = {"PushButton8Pressed", 0, 0 };
    static const QUMethod slot_12 = {"PushButton9Pressed", 0, 0 };
    static const QUParameter param_slot_13[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"AssrTextChanged", 1, param_slot_13 };
    static const QUMethod slot_14 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "mouseMoveEvent(QMouseEvent*)", &slot_0, QMetaData::Public },
	{ "timerEvent(QTimerEvent*)", &slot_1, QMetaData::Public },
	{ "mousePressEvent(QMouseEvent*)", &slot_2, QMetaData::Public },
	{ "PushButton0Pressed()", &slot_3, QMetaData::Public },
	{ "PushButton1Pressed()", &slot_4, QMetaData::Public },
	{ "PushButton2Pressed()", &slot_5, QMetaData::Public },
	{ "PushButton3Pressed()", &slot_6, QMetaData::Public },
	{ "PushButton4Pressed()", &slot_7, QMetaData::Public },
	{ "PushButton5Pressed()", &slot_8, QMetaData::Public },
	{ "PushButton6Pressed()", &slot_9, QMetaData::Public },
	{ "PushButton7Pressed()", &slot_10, QMetaData::Public },
	{ "PushButton8Pressed()", &slot_11, QMetaData::Public },
	{ "PushButton9Pressed()", &slot_12, QMetaData::Public },
	{ "AssrTextChanged(const QString&)", &slot_13, QMetaData::Public },
	{ "languageChange()", &slot_14, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CAssr", parentObject,
	slot_tbl, 15,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CAssr.setMetaObject( metaObj );
    return metaObj;
}

void* CAssr::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CAssr" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CAssr::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: mouseMoveEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 1: timerEvent((QTimerEvent*)static_QUType_ptr.get(_o+1)); break;
    case 2: mousePressEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 3: PushButton0Pressed(); break;
    case 4: PushButton1Pressed(); break;
    case 5: PushButton2Pressed(); break;
    case 6: PushButton3Pressed(); break;
    case 7: PushButton4Pressed(); break;
    case 8: PushButton5Pressed(); break;
    case 9: PushButton6Pressed(); break;
    case 10: PushButton7Pressed(); break;
    case 11: PushButton8Pressed(); break;
    case 12: PushButton9Pressed(); break;
    case 13: AssrTextChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 14: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CAssr::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CAssr::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CAssr::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
