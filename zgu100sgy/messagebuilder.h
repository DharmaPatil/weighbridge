/**
* @file messagebuilder.h
* @brief 命令帧封装类
* @ingroup zgu100sgy
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-29
*/
#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include <QDataStream>
#include <QByteArray>

class MessageBuilder : public QDataStream
{
public:
	QByteArray buffer;
	quint8 addr;
	QString path;
	int gates;

	MessageBuilder(int address);
	MessageBuilder(const QString &path);

	bool seek(int pos);
	bool seekToData();
	int discardRoute();
	void reset();
	int pos() const;		// 当前文件位置
	int size() const;		// 文件大小
	int left() const;		// 剩余文件大小
	bool checkDataSize();	// 读取一字节长度, 并检查剩余部分长度是否与该长度相同

	bool isExceptionResponse() const;
	int gateCount() const { return gates; }

	// bit
	MessageBuilder &beginWriteBit(quint16 addr, quint16 count);
	MessageBuilder &writeBit(quint16 addr, quint16 count, const void *ptr);
	MessageBuilder &writeBit(quint16 addr, quint16 count, quint64 value);
	MessageBuilder &writeBit(quint16 addr, bool value);
	MessageBuilder &readRwBit(quint16 addr, quint16 count);
	MessageBuilder &readRoBit(quint16 addr, quint16 count);

	// word
	MessageBuilder &beginWriteWord(quint16 addr, quint16 count);
	MessageBuilder &writeWord(quint16 addr, quint16 count, const void *ptr);
	MessageBuilder &writeWord(quint16 addr, quint16 value);
	MessageBuilder &readWord(quint16 addr, quint16 count);
	MessageBuilder &readRoWord(quint16 addr, quint16 count);

	// file
	MessageBuilder &beginReadFile(int requestCount);
	MessageBuilder &appendReadFile(quint16 file, quint16 addr, quint16 count);
	MessageBuilder &beginWriteFile(quint16 file, quint16 addr, quint16 count);

	// diagnostic
	MessageBuilder &readExceptionStatus();
	MessageBuilder &diagnose(quint16 subfunction, quint16 data);
	MessageBuilder &diagnose(quint16 subfunction);
	MessageBuilder &testCommunication(const char *testData, int size = -1);
	MessageBuilder &restart(bool clearEvent = true);
	MessageBuilder &enterBootloaderMode();

	// event
	MessageBuilder &readEventCount();
	MessageBuilder &readEventLog();

	// device id
	MessageBuilder &readDevId();
	MessageBuilder &readObject(quint8 startId);

	// encapsulate
	MessageBuilder &encapsulate();		// 透明转发
	MessageBuilder &route(quint8 addr);	// 网关转发

	// bootloader
	MessageBuilder &loadBootloaderInfo();
	MessageBuilder &eraseFlashPage(quint32 addr);
	MessageBuilder &readPageCRC();
	MessageBuilder &writeFlashPage(quint32 addr);
};

#endif // MESSAGEBUILDER_H
