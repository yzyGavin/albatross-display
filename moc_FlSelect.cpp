/****************************************************************************
** CFlSelectDlg meta object code from reading C++ file 'FlSelect.h'
**
** Created: Wed Sep 2 12:41:54 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "FlSelect.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CFlSelectDlg::className() const
{
    return "CFlSelectDlg";
}

QMetaObject *CFlSelectDlg::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CFlSelectDlg( "CFlSelectDlg", &CFlSelectDlg::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CFlSelectDlg::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CFlSelectDlg", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CFlSelectDlg::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CFlSelectDlg", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CFlSelectDlg::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"pDepDefaultPressed", 0, 0 };
    static const QUMethod slot_1 = {"pArrDefaultPressed", 0, 0 };
    static const QUMethod slot_2 = {"pOtherPressed", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "item", &static_QUType_ptr, "QListBoxItem", QUParameter::In }
    };
    static const QUMethod slot_3 = {"pListLevelClicked", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "Arcid", &static_QUType_QString, 0, QUParameter::In },
	{ "IsFpl", &static_QUType_bool, 0, QUParameter::In },
	{ "FltRul", &static_QUType_QString, 0, QUParameter::In },
	{ "Adep", &static_QUType_QString, 0, QUParameter::In },
	{ "DepRwy", &static_QUType_QString, 0, QUParameter::In },
	{ "Dep", &static_QUType_bool, 0, QUParameter::In },
	{ "Arr", &static_QUType_bool, 0, QUParameter::In },
	{ "OldValue", &static_QUType_int, 0, QUParameter::In },
	{ "AppKind", &static_QUType_QString, 0, QUParameter::In },
	{ "DisplayAppKind", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"Init", 10, param_slot_4 };
    static const QUMethod slot_5 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "pDepDefaultPressed()", &slot_0, QMetaData::Public },
	{ "pArrDefaultPressed()", &slot_1, QMetaData::Public },
	{ "pOtherPressed()", &slot_2, QMetaData::Public },
	{ "pListLevelClicked(QListBoxItem*)", &slot_3, QMetaData::Public },
	{ "Init(QString,bool,QString,QString,QString,bool,bool,int,QString,bool)", &slot_4, QMetaData::Public },
	{ "languageChange()", &slot_5, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CFlSelectDlg", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CFlSelectDlg.setMetaObject( metaObj );
    return metaObj;
}

void* CFlSelectDlg::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CFlSelectDlg" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CFlSelectDlg::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: pDepDefaultPressed(); break;
    case 1: pArrDefaultPressed(); break;
    case 2: pOtherPressed(); break;
    case 3: pListLevelClicked((QListBoxItem*)static_QUType_ptr.get(_o+1)); break;
    case 4: Init((QString)static_QUType_QString.get(_o+1),(bool)static_QUType_bool.get(_o+2),(QString)static_QUType_QString.get(_o+3),(QString)static_QUType_QString.get(_o+4),(QString)static_QUType_QString.get(_o+5),(bool)static_QUType_bool.get(_o+6),(bool)static_QUType_bool.get(_o+7),(int)static_QUType_int.get(_o+8),(QString)static_QUType_QString.get(_o+9),(bool)static_QUType_bool.get(_o+10)); break;
    case 5: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CFlSelectDlg::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CFlSelectDlg::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CFlSelectDlg::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
