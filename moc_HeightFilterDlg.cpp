/****************************************************************************
** CHeightFilterDlg meta object code from reading C++ file 'HeightFilterDlg.h'
**
** Created: Wed Sep 2 12:41:58 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "HeightFilterDlg.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CHeightFilterDlg::className() const
{
    return "CHeightFilterDlg";
}

QMetaObject *CHeightFilterDlg::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CHeightFilterDlg( "CHeightFilterDlg", &CHeightFilterDlg::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CHeightFilterDlg::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CHeightFilterDlg", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CHeightFilterDlg::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CHeightFilterDlg", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CHeightFilterDlg::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"OkClicked", 0, 0 };
    static const QUMethod slot_1 = {"CancelClicked", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "strval", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"pLowerEditTextChanged", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "strval", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"pUpperEditTextChanged", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "strval", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"pDepartureEditTextChanged", 1, param_slot_4 };
    static const QUMethod slot_5 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OkClicked()", &slot_0, QMetaData::Public },
	{ "CancelClicked()", &slot_1, QMetaData::Public },
	{ "pLowerEditTextChanged(const QString&)", &slot_2, QMetaData::Public },
	{ "pUpperEditTextChanged(const QString&)", &slot_3, QMetaData::Public },
	{ "pDepartureEditTextChanged(const QString&)", &slot_4, QMetaData::Public },
	{ "languageChange()", &slot_5, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CHeightFilterDlg", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CHeightFilterDlg.setMetaObject( metaObj );
    return metaObj;
}

void* CHeightFilterDlg::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CHeightFilterDlg" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CHeightFilterDlg::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OkClicked(); break;
    case 1: CancelClicked(); break;
    case 2: pLowerEditTextChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: pUpperEditTextChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: pDepartureEditTextChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CHeightFilterDlg::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CHeightFilterDlg::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CHeightFilterDlg::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
