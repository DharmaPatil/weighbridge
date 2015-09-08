/**
* @file global.h
* @brief 定义项目中用到的全局数据，包括：
*        全局常数
*        全局API
*        全局变量
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-07
*/

#ifndef GLOBAL_H
#define GLOBAL_H

const bool Use_Local_Func = false;
const bool Use_Dll_Func = true;

#define RAWFILENO 1
#define REFFILENO 2
#define FLTFILENO 3
#define DIFFILENO 4
#define IDXFILENO 5

const int conBufSize = 300000; //采集数据缓存最大长度
const int conFilterLen = 10;   //过滤区长度
const int conDifStep = 13;     //查分步长

const int conSampRate = 100;   //采样率
const int conKeepDataSize = 100; //上称后前面保留的数据长度
const int conRefPointDataSize = 25; //计算参考点数据长度
const int conRenewRefPointDataSize = 128; //更新参考点数据长度
const int conTrainOnDataSize = 13; //判断上称数据长度

#define LEFTDIRECTION 1
#define RIGHTDIRECTION 2

#define WHEELUP 1
#define WHEELDN -1

#define CARRIAGE4 1
#define WAGON4 2
#define ENGINE4 3
#define ENGINE6 4

const int conUseCh = 12;
const int conBoardNum = 3;


#endif // GLOBAL_H

