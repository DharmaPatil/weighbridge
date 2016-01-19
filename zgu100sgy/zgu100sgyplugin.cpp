/**
* @file instrzgu100sgyplugin.cpp
* @brief 断轨双台面轨道衡仪器插件类实现
* @ingroup zgu100sgy
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-29
*/
#include "zgu100sgyplugin.h"
#include "zgu100sgy.h"

#define KEY "zgu100sgy"

Q_EXPORT_PLUGIN2(InstrumentFactoryInterface, Zgu100sgyPlugin)

Zgu100sgyPlugin::Zgu100sgyPlugin()
{

}

Zgu100sgyPlugin::~Zgu100sgyPlugin()
{

}

QStringList Zgu100sgyPlugin::keys() const
{
    return QStringList() << KEY;
}

Instrument *Zgu100sgyPlugin::create(const QString &key, Workbench *parent)
{
    if (key == KEY)
        return new Zgu100sgy(parent);
    else
        return 0;
}
