/****************************************************************************
** CTssrOtherDlg meta object code from reading C++ file 'TssrOther.h'
**
** Created: Wed Sep 2 12:42:11 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "TssrOther.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CTssrOtherDlg::className() const
{
    return "CTssrOtherDlg";
}

QMetaObject *CTssrOtherDlg::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CTssrOtherDlg( "CTssrOtherDlg", &CTssrOtherDlg::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CTssrOtherDlg::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CTssrOtherDlg", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CTssrOtherDlg::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CTssrOtherDlg", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CTssrOtherDlg::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"pOKPressed", 0, 0 };
    static const QUMethod slot_1 = {"pUntagPressed", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "strInput", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"ArcIdChanged", 1, param_slot_2 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "pOKPressed()", &slot_0, QMetaData::Public },
	{ "pUntagPressed()", &slot_1, QMetaData::Public },
	{ "ArcIdChanged(const QString&)", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CTssrOtherDlg", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CTssrOtherDlg.setMetaObject( metaObj );
    return metaObj;
}

void* CTssrOtherDlg::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CTssrOtherDlg" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CTssrOtherDlg::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: pOKPressed(); break;
    case 1: pUntagPressed(); break;
    case 2: ArcIdChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CTssrOtherDlg::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CTssrOtherDlg::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CTssrOtherDlg::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
