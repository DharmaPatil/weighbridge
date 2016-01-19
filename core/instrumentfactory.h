/**
* @file instrumentfactory.h
* @brief 仪器构建类定义，这是Singleton模式
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef INSTRUMENTFACTORY_H
#define INSTRUMENTFACTORY_H

class QObject;
class QString;
class Instrument;
class Workbench;

class  InstrumentFactory
{
public:
    static Instrument *create(const QString &model, Workbench *parent = 0);
};

#endif // INSTRUMENTFACTORY_H
