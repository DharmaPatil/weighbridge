/**
* @file systemsetuppagewidget.h
* @brief 系统配置类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef SYSTEMSETUPPAGEWIDGET_H
#define SYSTEMSETUPPAGEWIDGET_H

#include "optionwidget.h"

class QLineEdit;
class QRadioButton;
class QCheckBox;
class QLabel;
class QGroupBox;
class QGridLayout;
class QComboBox;
class QVBoxLayout;

class SystemSetupPageWidget : public OptionWidget
{
    Q_OBJECT

public:
    SystemSetupPageWidget(Workbench *wb, QWidget *parent = 0);
    ~SystemSetupPageWidget();

private slots:
    void save();

private:
    void machineLayout(QWidget *parent = 0);  //
    void softwareLayout(QWidget *parent = 0);
    void portLayout(QWidget *parent = 0);
    void plantformLayout(QWidget *parent = 0);

    int getConfigIndexRange(const QString configValue, QStringList &range);

private:
    enum
    {
        MaxPlantformNum = 3,
        MaxChannelNum = 4,
        TotalChannelNum = 12,
        ChannleFuncNum = 4
    };

    //通用设置相关
    QComboBox *m_plantformNum;
    QLineEdit *m_plantformLen[MaxPlantformNum];
    QLineEdit *m_sampleDeep;
    QLineEdit *m_maxWeight;
    QComboBox *m_minResolution;
    QComboBox *m_adType;
    QComboBox *m_traincodePort;
    QLineEdit *m_traincodeAttr;
    QComboBox *m_instrPort;
    QLineEdit *m_instrAttr;
    QLineEdit *m_totalChannel;
    QComboBox *m_ch[MaxPlantformNum][MaxChannelNum];
    QRadioButton *m_swSyn[4];
    QRadioButton  *m_chSynMethodSel[MaxPlantformNum][ChannleFuncNum];
    QCheckBox *m_plantformType;
    QCheckBox *m_chSynMode[2];
    QCheckBox *m_plantformEn[MaxPlantformNum];
    QLabel *m_chlayout[MaxPlantformNum];

    QGridLayout *grid1;
    QGroupBox *machine;
    QGridLayout *grid2;
    QGroupBox *software;
    QGridLayout *grid3;
    QVBoxLayout *chLayout;
    QGroupBox *plantform[MaxPlantformNum];

    QString dbVal;                  /**< 读取数据库临时变量 */
    QStringList range;
    int curIndex;
};

#endif // SYSTEMSETUPPAGEWIDGET_H
