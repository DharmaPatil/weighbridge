/**
* @file instrumentplugin.h
* @brief 插件接口基类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef INSTRUMENTPLUGIN_H
#define INSTRUMENTPLUGIN_H

#include "core_global.h"
#include <QtPlugin>
#include <QFactoryInterface>

class Instrument;
class Workbench;

class CORE_EXPORT InstrumentFactoryInterface : public QFactoryInterface
{
public:
	virtual Instrument *create(const QString &key, Workbench *parent) = 0;
};

#define InstrumentFactoryInterface_iid "rw.wb.InstrumentFactoryInterface"

Q_DECLARE_INTERFACE(InstrumentFactoryInterface, InstrumentFactoryInterface_iid)

class CORE_EXPORT InstrumentPlugin : public QObject, public InstrumentFactoryInterface
{
	Q_OBJECT
	Q_INTERFACES(InstrumentFactoryInterface)

public:
	explicit InstrumentPlugin(QObject *parent = 0);
	virtual ~InstrumentPlugin();

	virtual QStringList keys() const = 0;
};

#endif // INSTRUMENTPLUGIN_H
