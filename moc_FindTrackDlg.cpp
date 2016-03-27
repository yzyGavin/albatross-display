/****************************************************************************
** CFindTrackDlg meta object code from reading C++ file 'FindTrackDlg.h'
**
** Created: Wed Sep 2 12:41:51 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "FindTrackDlg.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CFindTrackDlg::className() const
{
    return "CFindTrackDlg";
}

QMetaObject *CFindTrackDlg::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CFindTrackDlg( "CFindTrackDlg", &CFindTrackDlg::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CFindTrackDlg::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CFindTrackDlg", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CFindTrackDlg::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CFindTrackDlg", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CFindTrackDlg::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"buttonOk_clicked", 0, 0 };
    static const QUMethod slot_1 = {"Init", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "callsign", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"searchedCallsignChanged", 1, param_slot_2 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "buttonOk_clicked()", &slot_0, QMetaData::Public },
	{ "Init()", &slot_1, QMetaData::Public },
	{ "searchedCallsignChanged(const QString&)", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CFindTrackDlg", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CFindTrackDlg.setMetaObject( metaObj );
    return metaObj;
}

void* CFindTrackDlg::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CFindTrackDlg" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CFindTrackDlg::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: buttonOk_clicked(); break;
    case 1: Init(); break;
    case 2: searchedCallsignChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CFindTrackDlg::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CFindTrackDlg::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CFindTrackDlg::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
