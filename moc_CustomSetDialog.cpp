/****************************************************************************
** CustomizationSetDialog meta object code from reading C++ file 'CustomSetDialog.h'
**
** Created: Wed Sep 2 12:41:49 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "CustomSetDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CustomizationSetDialog::className() const
{
    return "CustomizationSetDialog";
}

QMetaObject *CustomizationSetDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CustomizationSetDialog( "CustomizationSetDialog", &CustomizationSetDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CustomizationSetDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CustomizationSetDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CustomizationSetDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CustomizationSetDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CustomizationSetDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pqlbiToDelete", &static_QUType_ptr, "QListBoxItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"DeleteCustomSet", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "strItem", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"AddCustomSet", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_2 = {"LoadCustomSet", 1, param_slot_2 };
    static const QUMethod slot_3 = {"ComputeEligibility", 0, 0 };
    static const QUMethod slot_4 = {"ClickDelete", 0, 0 };
    static const QUMethod slot_5 = {"ClickSaveAs", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_ptr, "QListBoxItem", QUParameter::In }
    };
    static const QUMethod slot_6 = {"SelectionChanged", 1, param_slot_6 };
    static const QUMethod slot_7 = {"ClickRecall", 0, 0 };
    static const QUMethod slot_8 = {"ClickSave", 0, 0 };
    static const QUParameter param_slot_9[] = {
	{ 0, &static_QUType_ptr, "QCloseEvent", QUParameter::In }
    };
    static const QUMethod slot_9 = {"closeEvent", 1, param_slot_9 };
    static const QUMethod slot_10 = {"CloseSaveAsWnd", 0, 0 };
    static const QUMethod slot_11 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "DeleteCustomSet(QListBoxItem*)", &slot_0, QMetaData::Public },
	{ "AddCustomSet(const QString&)", &slot_1, QMetaData::Public },
	{ "LoadCustomSet()", &slot_2, QMetaData::Public },
	{ "ComputeEligibility()", &slot_3, QMetaData::Public },
	{ "ClickDelete()", &slot_4, QMetaData::Public },
	{ "ClickSaveAs()", &slot_5, QMetaData::Public },
	{ "SelectionChanged(QListBoxItem*)", &slot_6, QMetaData::Public },
	{ "ClickRecall()", &slot_7, QMetaData::Public },
	{ "ClickSave()", &slot_8, QMetaData::Public },
	{ "closeEvent(QCloseEvent*)", &slot_9, QMetaData::Public },
	{ "CloseSaveAsWnd()", &slot_10, QMetaData::Public },
	{ "languageChange()", &slot_11, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CustomizationSetDialog", parentObject,
	slot_tbl, 12,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CustomizationSetDialog.setMetaObject( metaObj );
    return metaObj;
}

void* CustomizationSetDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CustomizationSetDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CustomizationSetDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: DeleteCustomSet((QListBoxItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: AddCustomSet((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: static_QUType_bool.set(_o,LoadCustomSet()); break;
    case 3: ComputeEligibility(); break;
    case 4: ClickDelete(); break;
    case 5: ClickSaveAs(); break;
    case 6: SelectionChanged((QListBoxItem*)static_QUType_ptr.get(_o+1)); break;
    case 7: ClickRecall(); break;
    case 8: ClickSave(); break;
    case 9: closeEvent((QCloseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 10: CloseSaveAsWnd(); break;
    case 11: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CustomizationSetDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CustomizationSetDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CustomizationSetDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
