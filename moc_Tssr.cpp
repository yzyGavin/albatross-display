/****************************************************************************
** CTssrDlg meta object code from reading C++ file 'Tssr.h'
**
** Created: Wed Sep 2 12:42:10 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "Tssr.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CTssrDlg::className() const
{
    return "CTssrDlg";
}

QMetaObject *CTssrDlg::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CTssrDlg( "CTssrDlg", &CTssrDlg::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CTssrDlg::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CTssrDlg", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CTssrDlg::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CTssrDlg", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CTssrDlg::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "item", &static_QUType_ptr, "QListBoxItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"pSsrListPressed", 1, param_slot_0 };
    static const QUMethod slot_1 = {"pOtherPressed", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "Item", &static_QUType_ptr, "QListBoxItem", QUParameter::In }
    };
    static const QUMethod slot_2 = {"SetItemAtTop", 1, param_slot_2 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "pSsrListPressed(QListBoxItem*)", &slot_0, QMetaData::Public },
	{ "pOtherPressed()", &slot_1, QMetaData::Public },
	{ "SetItemAtTop(QListBoxItem*)", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CTssrDlg", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CTssrDlg.setMetaObject( metaObj );
    return metaObj;
}

void* CTssrDlg::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CTssrDlg" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CTssrDlg::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: pSsrListPressed((QListBoxItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: pOtherPressed(); break;
    case 2: SetItemAtTop((QListBoxItem*)static_QUType_ptr.get(_o+1)); break;
    case 3: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CTssrDlg::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CTssrDlg::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CTssrDlg::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
