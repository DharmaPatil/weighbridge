/**
* @file wbaction.h
* @brief 菜单处理类定义
* @ingroup ui
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-17
*/
#ifndef WBACTION_H
#define WBACTION_H

#include <QObject>
#include <QIcon>
#include <QMap>
#include <QActionGroup>

class Workbench;
class QActionGroup;
class QAction;
class QKeySequence;

/**
* @brief 图标类定义
*/
class WBIcons
{
public:
    WBIcons();
    QIcon icon(const QString &name);
    QIcon operator[](const QString &name) { return icon(name); }

private:
    QMap<QString, QIcon> icons;
    QString path;
};

/**
* @brief 菜单处理类定义
*/
class WBAction : public QObject
{
    Q_OBJECT
public:
    explicit WBAction(Workbench *parent = 0);
    ~WBAction();

    WBIcons &icons;

    /** 功能分组 */
    QActionGroup *viewActions;
    QActionGroup *weightActions;
    QActionGroup *staticWeightActions;
    QActionGroup *dataActions;
    QActionGroup *calibrateActions;
    QActionGroup *qcActions;
    QActionGroup *databaseActions;
    QActionGroup *analysisActions;
    QActionGroup *logActions;
    QActionGroup *systemActions;
    QActionGroup *helpActions;

    /** 视图 */
    QAction *showWeightAction;
    QAction *showStaticWeightAction;
    QAction *showDataAction;     //查询打印
    QAction *showCalibrateAction;//曲线标定
    QAction *showQcAction;       //
    QAction *showDatabaseAction; //数据备份
    QAction *showAnalysisAction; //数据分析
    QAction *showLogAction;      //系统日志
    QAction *showSystemAction;   //系统配置

    /* 测量 */
    QAction *measureModeAction;   //称重方式
    QAction *startMeasureAction;  //开始
    QAction *pauseMeasureAction;  //停止
    QAction *reproduceAction;     //称重重现
    QAction *drawCurveAction;     //波形显示
    QAction *printRefPointAction; //打印参考点
    QAction *printResultAction;   //打印过衡结果

    /** 静态计量 */
    QAction *saveResultAction;          //保存静态计量数据
    QAction *printStaticResultAction;   //打印静态计量数据
    QAction *queryStaticResultAction;   //查询静态计量数据

    // 数据
    QAction *showResultAction;
    QAction *conditionAction;
    QAction *printRawResultAction;
    QAction *printReportAction;
    QAction *printQueryAction;
    QAction *removeResultAction;

    // 标准曲线
    QAction *calibrationMethodAction;    //标定方法
    QAction *startCalibrateAction;       //标定开始
    QAction *pauseCalibrateAction;       //停止标定
    QAction *registerCalibrationAction;  //保存数据
    QAction *validateCalibrationAction;  //打印数据
    QAction *resetCalibrationAction;     //复位
    QAction *modifyCalibrationAction;    //系数调整

    // 质控
    QAction *qcSelectGroupAction;
    QAction *qcChangeScaleAction;
    QAction *qcDeleteDataAction;
    QAction *qcSettingAction;
    QAction *qcDisplayDataAction;
    QAction *qcDisplayNewAction;
    QAction *qcSwitchLotsNum;
    QAction *qcPrevDataAction;
    QAction *qcNextDataAction;
    QAction *qcPrevPageAction;
    QAction *qcNextPageAction;
    QAction *qcPrintAction;

    // 数据备份
    QAction *dataInputAction;       //数据库导入
    QAction *dataOutputAction;      //数据库导出
    QAction *dataAccumulateAction;  //数据累计
    QAction *dataRepairAction;      //数据库修复

    // 模拟仿真
    QAction *simuDrawingCurveAction; //画曲线
    QAction *simuVertifyAction;
    QAction *simuNextPageAction;
    QAction *simuPrevPageAction;
    QAction *simuFirstPageAction;
    QAction *simuLastPageAction;

    // 系统日志
    QAction *logAllDataAction;
    QAction *logMeasureAction;
    QAction *logSystemAction;

    // 系统维护
    QAction *systemConfigAction;    //系统配置
    QAction *systemParamAction;     //参数设置
    QAction *systemAddUserAction;   //添加用户
    QAction *systemMaintainAction;  //硬件维护

    QAction *aboutAction;     // 帮助
    QAction *exitAction;      // 退出

private:
    Workbench *m_workbench;
    QAction *createAction(const QString &iconName, const QString &text);
    QAction *createAction(const QString &iconName, const QString &text, const QKeySequence &shortcut);
};

#endif // WBACTION_H
