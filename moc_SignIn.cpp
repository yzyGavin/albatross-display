/****************************************************************************
** CSignIn meta object code from reading C++ file 'SignIn.h'
**
** Created: Wed Sep 2 12:42:04 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SignIn.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CSignIn::className() const
{
    return "CSignIn";
}

QMetaObject *CSignIn::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CSignIn( "CSignIn", &CSignIn::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CSignIn::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CSignIn", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CSignIn::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CSignIn", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CSignIn::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "strUserName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"UserNameChanged", 1, param_slot_0 };
    static const QUMethod slot_1 = {"OkClicked", 0, 0 };
    static const QUMethod slot_2 = {"CancelClicked", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "strPassword", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"PasswordChanged", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_4 = {"mousePressEvent", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_ptr, "QCloseEvent", QUParameter::In }
    };
    static const QUMethod slot_5 = {"closeEvent", 1, param_slot_5 };
    static const QUMethod slot_6 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "UserNameChanged(const QString&)", &slot_0, QMetaData::Public },
	{ "OkClicked()", &slot_1, QMetaData::Public },
	{ "CancelClicked()", &slot_2, QMetaData::Public },
	{ "PasswordChanged(const QString&)", &slot_3, QMetaData::Public },
	{ "mousePressEvent(QMouseEvent*)", &slot_4, QMetaData::Public },
	{ "closeEvent(QCloseEvent*)", &slot_5, QMetaData::Public },
	{ "languageChange()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CSignIn", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CSignIn.setMetaObject( metaObj );
    return metaObj;
}

void* CSignIn::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CSignIn" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CSignIn::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: UserNameChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: OkClicked(); break;
    case 2: CancelClicked(); break;
    case 3: PasswordChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: mousePressEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 5: closeEvent((QCloseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 6: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CSignIn::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CSignIn::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CSignIn::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
