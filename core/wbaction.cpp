/**
* @file wbaction.cpp
* @brief 菜单处理类实现
* @ingroup ui
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-17
*/
#include <QActionGroup>
#include <QMenu>
#include <QIcon>

#include "wbaction.h"
#include "workbench.h"
#include "wbapp.h"

//////////////////////////////////////////////////////////////////
/// WBIcons类

/**
* @brief 构造函数
*/
WBIcons::WBIcons()
    : path(":/Resources/")
{

}

/**
* @brief 获取图标
* @param [I] 图标名称
* @retval 图标类
*/
QIcon WBIcons::icon(const QString &name)
{
    QMap<QString, QIcon>::iterator it = icons.find(name);
    if (it != icons.end())
        return *it;

    QIcon i(path + name + ".png");
    icons[name] = i;
    return i;
}

//////////////////////////////////////////////////////////////////
/// WBAction类
///

static QAction *createSeparator(QObject *parent)
{
    QAction * rc = new QAction(parent);
    rc->setSeparator(true);
    return rc;
}

static QActionGroup *createActionGroup(QObject *parent, bool exclusive = false)
{
    QActionGroup * rc = new QActionGroup(parent);
    rc->setExclusive(exclusive);
    return rc;
}

/**
* @brief 构造函数
* @param [I] 父指针
*/
WBAction::WBAction(Workbench *parent)
    : QObject(parent)
    , m_workbench(parent)
    , icons(*qApp->icons())

    , viewActions(createActionGroup(this, true))

    , weightActions(createActionGroup(this))
    , staticWeightActions(createActionGroup(this))
    , dataActions(createActionGroup(this))
    , calibrateActions(createActionGroup(this))
    , qcActions(createActionGroup(this))
    , databaseActions(createActionGroup(this))
    , analysisActions(createActionGroup(this))
    , logActions(createActionGroup(this))
    , systemActions(createActionGroup(this))
    , helpActions(createActionGroup(this))

    // 视图
    , showWeightAction(new QAction(icons["weight"], tr("动衡"), this))
    , showStaticWeightAction(new QAction(icons["static"], tr("静衡"), this))
    , showDataAction(new QAction(icons["find"], tr("查询"), this))
    , showCalibrateAction(new QAction(icons["calibration"], tr("标定"), this))
    , showQcAction(new QAction(icons["qc"], tr("质控"), this))
    , showDatabaseAction(new QAction(icons["backup"], tr("备份"), this))
    , showAnalysisAction(new QAction(icons["curve"], tr("分析"), this))
    , showLogAction(new QAction(icons["all"], tr("日志"), this))
    , showSystemAction(new QAction(icons["setting"], tr("设置"), this))

    // 测量
    , measureModeAction(new QAction(icons["auto"], tr("自动"), this))
    , startMeasureAction(new QAction(icons["start"], tr("启动"), this)) //new QAction(icons["start"], tr("开始"), this)
    , pauseMeasureAction(new QAction(icons["stop"], tr("停止"), this))
    , reproduceAction(new QAction(icons["simu"], tr("模拟过衡"), this))
    , drawCurveAction(new QAction(icons["curve"], tr("显示波形"), this))
    , printRefPointAction(new QAction(icons["refpoint"], tr("保存零点"), this))
    , printResultAction(new QAction(icons["print"], tr("打印结果"), this))

    // 静态测量
    , saveResultAction(new QAction(icons["save"], tr("保存"), this))
    , printStaticResultAction(new QAction(icons["print"], tr("打印"), this))
    , queryStaticResultAction(new QAction(icons["query"], tr("查询"), this))

    // 数据
    , showResultAction(new QAction(icons["result"], tr("全部数据"), this))
    , conditionAction(new QAction(icons["query"], tr("选择"), this))
    , printRawResultAction(new QAction(icons["print"], tr("原始打印"), this))
    , printReportAction(new QAction(icons["print"], tr("报表打印"), this))
    , printQueryAction(new QAction(icons["print"], tr("查询打印"), this))
    , removeResultAction(new QAction(icons["delete"], tr("查询删除"), this))

    // 定标
    , calibrationMethodAction(new QAction(icons["switch"], tr("标定项目"), this))
    , startCalibrateAction(new QAction(icons["start"], tr("启动"), this))
    , pauseCalibrateAction(new QAction(icons["stop"], tr("停止"), this))
    , validateCalibrationAction(new QAction(icons["print"], tr("打印"), this))
    , registerCalibrationAction(new QAction(icons["save"], tr("保存"), this))
    , resetCalibrationAction(new QAction(icons["reset"], tr("复位"), this))
    , modifyCalibrationAction(new QAction(icons["curve"], tr("系数调整"), this))

    // 质控
    , qcSelectGroupAction(new QAction(icons["group"], tr("选择"), this))
    , qcChangeScaleAction(new QAction(icons["calendar"], tr("刻度"), this))
    , qcDeleteDataAction(new QAction(icons["delete"], tr("删除"), this))
    , qcSettingAction(new QAction(icons["setting"], tr("设置"), this))
    , qcDisplayDataAction(new QAction(icons["data"], tr("显示数据"), this))
    , qcDisplayNewAction(new QAction(icons["newcurve"], tr("显示质控"), this))
    , qcSwitchLotsNum(new QAction(icons["switch"], tr("切换批号"), this))
    , qcPrevDataAction(new QAction(icons["left"], tr("前点"), this))
    , qcNextDataAction(new QAction(icons["right"], tr("后点"), this))
    , qcPrevPageAction(new QAction(icons["prevpage"], tr("前页"), this))
    , qcNextPageAction(new QAction(icons["nextpage"], tr("后页"), this))
    , qcPrintAction(new QAction(icons["print"], tr("打印"), this))

    // 数据备份
    , dataInputAction(new QAction(icons["input"], tr("数据导入"), this))
    , dataOutputAction(new QAction(icons["output"], tr("数据导出"), this))
    , dataAccumulateAction(new QAction(icons["total"], tr("结果累计"), this))
    , dataRepairAction(new QAction(icons["datarepair"], tr("数据修复"), this))

    // 模拟仿真
    , simuDrawingCurveAction(new QAction(icons["draw"], tr("绘制"), this))
    , simuVertifyAction(new QAction(icons["verify"], tr("验证"), this))
    , simuNextPageAction(new QAction(icons["prevpage"], tr("前页"), this))
    , simuPrevPageAction(new QAction(icons["nextpage"], tr("后页"), this))
    , simuFirstPageAction(new QAction(icons["first"], tr("首页"), this))
    , simuLastPageAction(new QAction(icons["last"], tr("末页"), this))

    // 系统日志
    , logAllDataAction(new QAction(icons["all"], tr("全部日志"), this))
    , logMeasureAction(new QAction(icons["log"], tr("测量日志"), this))
    , logSystemAction(new QAction(icons["alarm"], tr("系统报警"), this))

    // 系统维护
    , systemConfigAction(new QAction(icons["config"], tr("系统配置"), this))
    , systemParamAction(new QAction(icons["param"], tr("系统参数"), this))
    , systemAddUserAction(new QAction(icons["user"], tr("用户管理"), this))
    , systemMaintainAction(new QAction(icons["hardware"], tr("硬件维护"), this))

    , aboutAction(new QAction(icons["about"], tr("关于..."), this))
    , exitAction(new QAction(icons["exit"], tr("关闭"), this))
{
    // 视图
    viewActions->addAction(showWeightAction);
    viewActions->addAction(showStaticWeightAction);
    viewActions->addAction(showDataAction);
    viewActions->addAction(showCalibrateAction);
    viewActions->addAction(showQcAction);
    viewActions->addAction(showDatabaseAction);
    viewActions->addAction(showAnalysisAction);
    viewActions->addAction(showLogAction);
    viewActions->addAction(showSystemAction);
    showWeightAction->setCheckable(true);
    showStaticWeightAction->setCheckable(true);
    showDataAction->setCheckable(true);
    showCalibrateAction->setCheckable(true);
    showQcAction->setCheckable(true);
    showDatabaseAction->setCheckable(true);
    showAnalysisAction->setCheckable(true);
    showLogAction->setCheckable(true);
    showSystemAction->setCheckable(true);

    // 测量
    weightActions->addAction(measureModeAction);
    weightActions->addAction(startMeasureAction);
    weightActions->addAction(pauseMeasureAction);
    weightActions->addAction(reproduceAction);
    weightActions->addAction(drawCurveAction);
    weightActions->addAction(printRefPointAction);
    weightActions->addAction(printResultAction);
    startMeasureAction->setEnabled(false);

    // 静态计量
    staticWeightActions->addAction(saveResultAction);
    staticWeightActions->addAction(printStaticResultAction);
    staticWeightActions->addAction(queryStaticResultAction);

    // 数据
    dataActions->addAction(showResultAction);
    dataActions->addAction(conditionAction);
    dataActions->addAction(printRawResultAction);
    dataActions->addAction(printReportAction);
    dataActions->addAction(printQueryAction);
    dataActions->addAction(removeResultAction);

    // 定标
    calibrateActions->addAction(calibrationMethodAction);
//    calibcalibrateActions->addAction(registerCalibrationAction);
    calibrateActions->addAction(startCalibrateAction);
    calibrateActions->addAction(pauseCalibrateAction);
    calibrateActions->addAction(validateCalibrationAction);
    calibrateActions->addAction(resetCalibrationAction);
    calibrateActions->addAction(modifyCalibrationAction);
    pauseCalibrateAction->setEnabled(false);

    // 质控
    qcActions->addAction(qcSelectGroupAction);
    qcActions->addAction(qcChangeScaleAction);
    qcActions->addAction(qcDeleteDataAction);
    qcActions->addAction(qcSettingAction);
    qcActions->addAction(qcDisplayDataAction);
    qcActions->addAction(qcDisplayNewAction);
    qcActions->addAction(qcSwitchLotsNum);
    qcActions->addAction(createSeparator(this));
    qcActions->addAction(qcPrevDataAction);
    qcActions->addAction(qcNextDataAction);
    qcActions->addAction(qcPrevPageAction);
    qcActions->addAction(qcNextPageAction);
    qcActions->addAction(createSeparator(this));
    qcActions->addAction(qcPrintAction);
    qcDisplayDataAction->setCheckable(true);
    qcDisplayNewAction->setCheckable(true);

    // 数据备份
    databaseActions->addAction(dataInputAction);
    databaseActions->addAction(dataOutputAction);
    databaseActions->addAction(dataAccumulateAction);
    databaseActions->addAction(dataRepairAction);

    // 模拟
    analysisActions->addAction(simuDrawingCurveAction);
    analysisActions->addAction(simuVertifyAction);
    analysisActions->addAction(simuNextPageAction);
    analysisActions->addAction(simuPrevPageAction);
    analysisActions->addAction(simuFirstPageAction);
    analysisActions->addAction(simuLastPageAction);

    // 日志
    logActions->addAction(logAllDataAction);
    logActions->addAction(logMeasureAction);
    logActions->addAction(logSystemAction);

    // 系统
    systemActions->addAction(systemConfigAction);
    systemActions->addAction(systemParamAction);
    systemActions->addAction(systemAddUserAction);
    systemActions->addAction(systemMaintainAction);

    // 帮助
    helpActions->addAction(aboutAction);
}

WBAction::~WBAction()
{

}

/**
* @brief 创建菜单项目
* @param [I] 图标名
* @param [I] 提示
* @retval 菜单项目
*/
QAction *WBAction::createAction(const QString &iconName, const QString &text)
{
    return new QAction(icons[iconName], text, this);
}

/**
* @brief 创建菜单项目
* @param [I] 图标名
* @param [I] 提示
* @param [I] 快捷键
* @retval 菜单项目
*/
QAction *WBAction::createAction(const QString &iconName, const QString &text, const QKeySequence &shortcut)
{
    QAction *action = createAction(iconName, text);
    action->setShortcut(shortcut);
    return action;
}
