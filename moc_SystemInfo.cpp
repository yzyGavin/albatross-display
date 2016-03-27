/****************************************************************************
** CSystemInfo meta object code from reading C++ file 'SystemInfo.h'
**
** Created: Wed Sep 2 12:42:05 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SystemInfo.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CSystemInfo::className() const
{
    return "CSystemInfo";
}

QMetaObject *CSystemInfo::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CSystemInfo( "CSystemInfo", &CSystemInfo::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CSystemInfo::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CSystemInfo", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CSystemInfo::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CSystemInfo", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CSystemInfo::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"init", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "QCloseEvent", QUParameter::In }
    };
    static const QUMethod slot_1 = {"closeEvent", 1, param_slot_1 };
    static const QUMethod slot_2 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "init()", &slot_0, QMetaData::Public },
	{ "closeEvent(QCloseEvent*)", &slot_1, QMetaData::Public },
	{ "languageChange()", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CSystemInfo", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CSystemInfo.setMetaObject( metaObj );
    return metaObj;
}

void* CSystemInfo::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CSystemInfo" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool CSystemInfo::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: init(); break;
    case 1: closeEvent((QCloseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 2: languageChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CSystemInfo::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CSystemInfo::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool CSystemInfo::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
