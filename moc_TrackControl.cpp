/****************************************************************************
** TrackControl meta object code from reading C++ file 'TrackControl.h'
**
** Created: Wed Sep 2 12:42:08 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "TrackControl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TrackControl::className() const
{
    return "TrackControl";
}

QMetaObject *TrackControl::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TrackControl( "TrackControl", &TrackControl::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TrackControl::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TrackControl", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TrackControl::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TrackControl", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TrackControl::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"SliderVectorValueChanged", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"HistoryDotsValueChanged", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_ptr, "QObject", QUParameter::In }
    };
    static const QUMethod slot_2 = {"TrackControlDestroyed", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"HaloValueChanged", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"FieldsPressed", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"FontSizePressed", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"UncoupledVectorToggled", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"CoupledVectorToggled", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"TimeMarksVectorToggled", 1, param_slot_8 };
    static const QUMethod slot_9 = {"pLcmResetPressed", 0, 0 };
    static const QUMethod slot_10 = {"SliderPressedSlot", 0, 0 };
    static const QUMethod slot_11 = {"SliderReleaseSlot", 0, 0 };
    static const QUMethod slot_12 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SliderVectorValueChanged(int)", &slot_0, QMetaData::Public },
	{ "HistoryDotsValueChanged(int)", &slot_1, QMetaData::Public },
	{ "TrackControlDestroyed(QObject*)", &slot_2, QMetaData::Public },
	{ "HaloValueChanged(int)", &slot_3, QMetaData::Public },
	{ "FieldsPressed(int)", &slot_4, QMetaData::Public },
	{ "FontSizePressed(int)", &slot_5, QMetaData::Public },
	{ "UncoupledVectorToggled(bool)", &slot_6, QMetaData::Public },
	{ "CoupledVectorToggled(bool)", &slot_7, QMetaData::Public },
	{ "TimeMarksVectorToggled(bool)", &slot_8, QMetaData::Public },
	{ "pLcmResetPressed()", &slot_9, QMetaData::Public },
	{ "SliderPressedSlot()", &slot_10, QMetaData::Public },
	{ "SliderReleaseSlot()", &slot_11, QMetaData::Public },
	{ "languageChange()", &slot_12, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"TrackControl", parentObject,
	slot_tbl, 13,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TrackControl.setMetaObject( metaObj );
    return metaObj;
}

void* TrackControl::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TrackControl" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool TrackControl::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SliderVectorValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 1: HistoryDotsValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 2: TrackControlDestroyed((QObject*)static_QUType_ptr.get(_o+1)); break;
    case 3: HaloValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 4: FieldsPressed((int)static_QUType_int.get(_o+1)); break;
    case 5: FontSizePressed((int)static_QUType_int.get(_o+1)); break;
    case 6: UncoupledVectorToggled((bool)static_QUType_bool.get(_o+1)); break;
    case 7: CoupledVectorToggled((bool)static_QUType_bool.get(_o+1)); break;
    case 8: TimeMarksVectorToggled((bool)static_QUType_bool.get(_o+1)); break;
    case 9: pLcmResetPressed(); break;
    case 10: SliderPressedSlot(); break;
    case 11: SliderReleaseSlot(); break;
    case 12: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TrackControl::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TrackControl::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool TrackControl::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
