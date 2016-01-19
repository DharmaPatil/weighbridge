/**
* @file instrparamheader.h
* @brief 仪器参数类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef INSTRPARAMHEADER_H
#define INSTRPARAMHEADER_H

/**
* @brief 仪器参数类
*/
class InstrParamHeader
{
    //台面机械参数
    struct board
    {
        float boardlen;  //台面长度
        float weight;    //
        float percent;   //
        float dirL;      //
        float dirR;      //
        int boardNum;    //
    };

    //压力传感器一致性因数
    struct consis_pressure
    {
        float sensor1;
        float sensor2;
        float sensor3;
        float sensor4;
    };

    //剪力传感器一致性因数
    struct consis_shear
    {
        float sensor1;
        float sensor2;
        float sensor3;
        float sensor4;
    };

    //通道增益
    struct factor
    {
        float channel1;
        float channel2;
        float channel3;
        float channel4;
        float channel5;
        float channel6;
        float channel7;
        float channel8;
    };
    int size;
};

#endif // INSTRPARAMHEADER_H
