/**
* @file  machineType.h
* @brief 机型文件
* @author walterqin(walterqin@hotmail.com)
* @date 2015-09-04
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-09-04    QinGenjian    create
*
*/
#ifndef MACHINECONFIG_H
#define MACHINECONFIG_H

#include "global.h"
#include <QString>
#include <QStringList>
#include <QMap>

/**
* @brief 机型
*/
#define WEIGHTBRIDGE3           0x03  /**<  */

/** @brief 软件版本显示机型定义 */
#define MachineType     "RW3000"


/** @brief 厂家定义 */
enum {
    RuiWei = 10,             /**< 瑞威 */
};

/** @brief 机型定义 */
enum {
    ZGU_100_DG = 1,
    ZGU_100_C,
    ZGU_100_SGY,
    ZGU_100_SG,
    ZGU_100_SY,
    ZGU_100_TGY,
    ZGU_100_TG,
    ZGU_100_TY,
    ZGU_100_BDG,
    ZGU_100_BDB,
    Simulator,
};

/** @brief 厂家定义 */
#define FACTORY RuiWei

/** @brief 机型定义 */
#define Machine_Type    ZGU_100_TGY

/** @brief 最大测试项目数 */
#define USECHANNEL 12
#define BOARDNUM 3

/**
* @brief 机型配置类
*/
class COMMON_EXPORT MachineConfig
{
public:
    /** 构造函数 */
    MachineConfig() {}

    static int getMachine();
    static int getFactory();
    static QString getMachineName();
    static QString getFactoryID();
    static QString getFactoryCode() ;
    static QString getDatabasePath();
    static QString getCarriageTypeName(int type);
    static QString getConfigurePath();
    static QString getMachineModel(int type);

private:
    static int machineType;    /**< 机型代码 */
    static int factoryCode;    /**< 厂家代号 */

};

#endif // MACHINECONFIG_H
