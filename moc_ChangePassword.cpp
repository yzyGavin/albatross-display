/****************************************************************************
** CChangePasswd meta object code from reading C++ file 'ChangePassword.h'
**
** Created: Wed Sep 2 12:41:47 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ChangePassword.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CChangePasswd::className() const
{
    return "CChangePasswd";
}

QMetaObject *CChangePasswd::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CChangePasswd( "CChangePasswd", &CChangePasswd::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CChangePasswd::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CChangePasswd", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CChangePasswd::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CChangePasswd", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CChangePasswd::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"OkClicked", 0, 0 };
    static const QUMethod slot_1 = {"CancelClicked", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "strPassword", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"PasswordChanged", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "strNewPassword", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"NewPasswordChanged", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "strConfirmPassword", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"ConfirmPasswordChanged", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_5 = {"mousePressEvent", 1, param_slot_5 };
    static const QUMethod slot_6 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OkClicked()", &slot_0, QMetaData::Public },
	{ "CancelClicked()", &slot_1, QMetaData::Public },
	{ "PasswordChanged(const QString&)", &slot_2, QMetaData::Public },
	{ "NewPasswordChanged(const QString&)", &slot_3, QMetaData::Public },
	{ "ConfirmPasswordChanged(const QString&)", &slot_4, QMetaData::Public },
	{ "mousePressEvent(QMouseEvent*)", &slot_5, QMetaData::Public },
	{ "languageChange()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CChangePasswd", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CChangePasswd.setMetaObject( metaObj );
    return metaObj;
}

void* CChangePasswd::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CChangePasswd" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CChangePasswd::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OkClicked(); break;
    case 1: CancelClicked(); break;
    case 2: PasswordChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: NewPasswordChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: ConfirmPasswordChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: mousePressEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 6: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CChangePasswd::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CChangePasswd::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CChangePasswd::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
