/****************************************************************************
** CColorDlg meta object code from reading C++ file 'Brightness.h'
**
** Created: Wed Sep 2 12:41:45 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "Brightness.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CColorDlg::className() const
{
    return "CColorDlg";
}

QMetaObject *CColorDlg::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CColorDlg( "CColorDlg", &CColorDlg::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CColorDlg::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CColorDlg", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CColorDlg::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CColorDlg", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CColorDlg::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"MasterSliderValueChanged", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"AocSymbSliderValueChanged", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"AocDbSliderValueChanged", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"OthSymbSliderValueChanged", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"OthDbSliderValueChanged", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"MouseSliderValueChanged", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"WeatherSliderValueChanged", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"MapInSliderValueChanged", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"MapOutSliderValueChanged", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"MapSymbSliderValueChanged", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"RngMarkSliderValueChanged", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"WindowSliderValueChanged", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"AsdToolsSliderValueChanged", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"DFSliderValueChanged", 1, param_slot_13 };
    static const QUMethod slot_14 = {"ResetPressed", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_ptr, "QCloseEvent", QUParameter::In }
    };
    static const QUMethod slot_15 = {"closeEvent", 1, param_slot_15 };
    static const QUMethod slot_16 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "MasterSliderValueChanged(int)", &slot_0, QMetaData::Public },
	{ "AocSymbSliderValueChanged(int)", &slot_1, QMetaData::Public },
	{ "AocDbSliderValueChanged(int)", &slot_2, QMetaData::Public },
	{ "OthSymbSliderValueChanged(int)", &slot_3, QMetaData::Public },
	{ "OthDbSliderValueChanged(int)", &slot_4, QMetaData::Public },
	{ "MouseSliderValueChanged(int)", &slot_5, QMetaData::Public },
	{ "WeatherSliderValueChanged(int)", &slot_6, QMetaData::Public },
	{ "MapInSliderValueChanged(int)", &slot_7, QMetaData::Public },
	{ "MapOutSliderValueChanged(int)", &slot_8, QMetaData::Public },
	{ "MapSymbSliderValueChanged(int)", &slot_9, QMetaData::Public },
	{ "RngMarkSliderValueChanged(int)", &slot_10, QMetaData::Public },
	{ "WindowSliderValueChanged(int)", &slot_11, QMetaData::Public },
	{ "AsdToolsSliderValueChanged(int)", &slot_12, QMetaData::Public },
	{ "DFSliderValueChanged(int)", &slot_13, QMetaData::Public },
	{ "ResetPressed()", &slot_14, QMetaData::Public },
	{ "closeEvent(QCloseEvent*)", &slot_15, QMetaData::Public },
	{ "languageChange()", &slot_16, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CColorDlg", parentObject,
	slot_tbl, 17,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CColorDlg.setMetaObject( metaObj );
    return metaObj;
}

void* CColorDlg::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CColorDlg" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool CColorDlg::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: MasterSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 1: AocSymbSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 2: AocDbSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 3: OthSymbSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 4: OthDbSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 5: MouseSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 6: WeatherSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 7: MapInSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 8: MapOutSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 9: MapSymbSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 10: RngMarkSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 11: WindowSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 12: AsdToolsSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 13: DFSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 14: ResetPressed(); break;
    case 15: closeEvent((QCloseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 16: languageChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CColorDlg::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CColorDlg::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool CColorDlg::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
