/**
* @file  machineConfig.cpp
* @brief 机型配置类实现
* @author walterqin(walterqin@hotmail.com)
* @date 2015-09-04
* @ingroup configure
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-09-04    QinGenjian    create
*
*/
#include "machineconfig.h"
#include <QDebug>
#include <QDir>

int MachineConfig::machineType = Machine_Type;
int MachineConfig::factoryCode = FACTORY;

/**
* @brief 获取机型
* @return 机型
*/
int MachineConfig::getMachine()
{
    return machineType;
}

/**
* @brief 获取厂家
* @return 厂家
*/
int MachineConfig::getFactory()
{
    return factoryCode;
}

/**
* @brief 获取机型显示名称
* @return 机型显示名称
*/
QString MachineConfig::getMachineName()
{
    return QString(MachineType);
}

/**
* @brief 获取厂家显示代码
* @return 厂家显示代码
*/
QString MachineConfig::getFactoryCode()
{
    return QString();
}

/**
* @brief 获取厂家ID
* @return 厂家ID
*/
QString MachineConfig::getFactoryID()
{
    return QString();
}


/**
* @brief 获取数据库路径
* @retval 路径
*/
QString MachineConfig::getDatabasePath()
{
    QString path = QDir::currentPath() + QString("/db");
    return path;
}

/**
* @brief 获取车型名称
* @param [I] 车型
* @retval 车型名称
*/
QString MachineConfig::getCarriageTypeName(int type)
{
    switch (type)
    {
    case CARRIAGE4:
        return QString("K");
    case WAGON4:
        return QString("H");
    case ENGINE4:
        return QString("E-4");
    case ENGINE6:
        return QString("E-6");
    default:
        return QString("N/A");
    }
}

QString MachineConfig::getConfigurePath()
{
    return QDir::currentPath() + QString("/instr");
}

/**
* @brief 获取仪器类型名称
* @param [I] 类别
* @retval 类型名称
*/
QString MachineConfig::getMachineModel(int type)
{
    switch(type)
    {
    case ZGU_100_DG:
        return QString("dg");
    case ZGU_100_C:
        return QString("c");
    case ZGU_100_SGY:
        return QString("sgy");
    case ZGU_100_SG:
        return QString("sg");
    case ZGU_100_SY:
        return QString("sy");
    case ZGU_100_TGY:
        return QString("tgy");
    case ZGU_100_TG:
        return QString("tg");
    case ZGU_100_TY:
        return QString("ty");
    case ZGU_100_BDG:
        return QString("bdg");
    case ZGU_100_BDB:
        return QString("bdb");
    default:
        return QString("sim");
    }
}
