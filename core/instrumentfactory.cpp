/**
* @file instrumentfactory.cpp
* @brief 仪器构建类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#include <QPluginLoader>

#include "instrumentfactory.h"
#include "instrumentplugin.h"
#include "instrument.h"
#include "siminstrument.h"
#include "global.h"
#include "wbapp.h"

Instrument *InstrumentFactory::create(const QString &model, Workbench *parent)
{
    if (model == "sim")
        return new SimInstrument(parent);

    QString filename = model + ".dll";
    QPluginLoader loader(qApp->applicationDirPath() + "/" + filename);
    QObject *obj = loader.instance();
    if (!obj)
    {
        critical(WBApp::tr("无法加载%1\n%2").arg(filename).arg(loader.errorString()));
        loader.unload();
        return 0;
    }

    InstrumentPlugin *plugin = qobject_cast<InstrumentPlugin*>(obj);
    Instrument *instr = plugin ? plugin->create(model, parent) : 0;
    if (!instr)
    {
        critical(WBApp::tr("无效的插件%1\n%2").arg(filename).arg(loader.errorString()));
        loader.unload();
        return 0;
    }

    return instr;
}




