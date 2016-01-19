/**
* @file instrzgu100sgyplugin.h
* @brief 断轨双台面轨道衡仪器插件类定义
* @ingroup zgu100sgy
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef ZGU100SGYPLUGIN_H
#define ZGU100SGYPLUGIN_H

#include "instrumentplugin.h"

class Zgu100sgyPlugin : public InstrumentPlugin
{
public:
    Zgu100sgyPlugin();
    ~Zgu100sgyPlugin();

    QStringList keys() const;
    Instrument *create(const QString &key, Workbench *parent);
};

#endif // INSTRZGU100SGYPLUGIN_H
