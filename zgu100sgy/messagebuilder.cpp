/**
* @file messagebuilder.cpp
* @brief 命令帧封装类实现
* @ingroup zgu100sgy
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-29
*/

#include "messagebuilder.h"
#include <QStringList>

#define REQUEST(n)		quint8(n)

MessageBuilder::MessageBuilder(int address)
	: QDataStream(&buffer, QIODevice::ReadWrite)
	, addr(address)
	, gates(0)
{
	setByteOrder(BigEndian);
	buffer.reserve(128);
	*this << addr;
}

MessageBuilder::MessageBuilder(const QString &path)
	: QDataStream(&buffer, QIODevice::ReadWrite)
{
	this->path = path;
	setByteOrder(BigEndian);
	buffer.reserve(128);

	QStringList list = path.split('/', QString::SkipEmptyParts);
	addr = (quint8)list.first().toInt();
	list.takeFirst();
	*this << addr;

	gates = list.size();

	while(!list.isEmpty())
	{
		QString s = list.takeFirst();
		addr = (quint8)s.toInt();
		route(addr);
	}
}

bool MessageBuilder::seek(int pos)
{
	resetStatus();
	return device()->seek(pos);
}

bool MessageBuilder::seekToData()
{
	return seek(2);
}

// 丢弃路由数据, 返回丢弃的数量, 应当与gates相同
int MessageBuilder::discardRoute()
{
	int len = 0;
	int count = 0;
	while(len < buffer.size()
		  && buffer[len + 1] == (char)0x2B
		  && buffer[len + 2] == (char)0x0C)
	{
		len += 3;
		++count;
	}

	if (len > 0)
	{
		buffer = buffer.mid(len);
		resetStatus();
		seek(0);
	}

	return count;
}

void MessageBuilder::reset()
{
	resetStatus();
	buffer.resize(0);
	buffer.reserve(128);
	seek(0);

	if (path.isEmpty())
		*this << addr;
	else
	{
		QStringList list = path.split('/', QString::SkipEmptyParts);
		addr = (quint8)list.first().toInt();
		list.takeFirst();
		*this << addr;

		gates = list.size();

		while(!list.isEmpty())
		{
			QString s = list.takeFirst();
			addr = (quint8)s.toInt();
			route(addr);
		}
	}
}

int MessageBuilder::pos() const
{
	return device()->pos();
}

int MessageBuilder::size() const
{
	return device()->size();
}

int MessageBuilder::left() const
{
	return size() - pos();
}

bool MessageBuilder::checkDataSize()
{
	quint8 len;
	*this >> len;
	return len == left();
}

bool MessageBuilder::isExceptionResponse() const
{
	return buffer.size() >= 3 && (buffer[1] & 0x80) != 0;
}

MessageBuilder &MessageBuilder::beginWriteBit(quint16 addr, quint16 count)
{
	*this << REQUEST(0x0F) << addr << count << quint8((count + 7) / 8);
	return *this;
}

MessageBuilder &MessageBuilder::writeBit(quint16 addr, quint16 count, const void *ptr)
{
	beginWriteBit(addr, count);
	const quint8 *p = (const quint8 *)ptr;

	for(count = (count + 7) / 8; count--; )
		*this << *p++;

	return *this;
}

MessageBuilder &MessageBuilder::writeBit(quint16 addr, quint16 count, quint64 value)
{
	Q_ASSERT(count <= 64);
	count = qMin<quint16>(64, count);
	return writeBit(addr, count, &value);
}

MessageBuilder &MessageBuilder::writeBit(quint16 addr, bool value)
{
	*this << REQUEST(0x05) << addr << (value ? quint16(0xFF00) : quint16(0x000));
	return *this;
}

MessageBuilder &MessageBuilder::readRwBit(quint16 addr, quint16 count)
{
	*this << REQUEST(0x01) << addr << count;
	return *this;
}

MessageBuilder &MessageBuilder::readRoBit(quint16 addr, quint16 count)
{
	*this << REQUEST(0x02) << addr << count;
	return *this;
}

MessageBuilder &MessageBuilder::beginWriteWord(quint16 addr, quint16 count)
{
	*this << REQUEST(0x10) << addr << count << quint8(2*count);
	return *this;
}

MessageBuilder &MessageBuilder::writeWord(quint16 addr, quint16 count, const void *ptr)
{
	beginWriteWord(addr, count);
	const quint16 *p = (const quint16 *)ptr;
	for(; count--; )
		*this << *p++;
	return *this;
}

MessageBuilder &MessageBuilder::writeWord(quint16 addr, quint16 value)
{
	*this << REQUEST(0x06) << addr << value;
	return *this;
}

MessageBuilder &MessageBuilder::readWord(quint16 addr, quint16 count)
{
	*this << REQUEST(0x03) << addr << count;
	return *this;
}

MessageBuilder &MessageBuilder::readRoWord(quint16 addr, quint16 count)
{
	*this << REQUEST(0x04) << addr << count;
	return *this;
}

MessageBuilder &MessageBuilder::beginReadFile(int requestCount)
{
	*this << REQUEST(0x14) << quint8(requestCount * 7);
	return *this;
}

MessageBuilder &MessageBuilder::appendReadFile(quint16 file, quint16 addr, quint16 count)
{
	*this << quint8(6) << file << addr << count;
	return *this;
}

MessageBuilder &MessageBuilder::beginWriteFile(quint16 file, quint16 addr, quint16 count)
{
	*this << REQUEST(0x15) << quint8(7 + count * 2) << quint8(6) << file << addr << count;
	return *this;
}

MessageBuilder &MessageBuilder::readExceptionStatus()
{
	*this << REQUEST(0x07);
	return *this;
}

MessageBuilder &MessageBuilder::diagnose(quint16 subfunction, quint16 data)
{
	*this << REQUEST(0x08) << subfunction << data;
	return *this;
}

MessageBuilder &MessageBuilder::diagnose(quint16 subfunction)
{
	*this << REQUEST(0x08) << subfunction;
	return *this;
}

MessageBuilder &MessageBuilder::testCommunication(const char *testData, int size)
{
	diagnose(0x00);
	if (size == -1)
		size = qstrlen(testData);
	writeRawData(testData, size);
	return *this;
}

MessageBuilder &MessageBuilder::restart(bool clearEvent)
{
	return diagnose(0x01, clearEvent ? 0xFF00 : 0x0000);
}

MessageBuilder &MessageBuilder::enterBootloaderMode()
{
	return diagnose(0x01, 0xFFFF);
}

MessageBuilder &MessageBuilder::readEventCount()
{
	*this << REQUEST(0x0B);
	return *this;
}

MessageBuilder &MessageBuilder::readEventLog()
{
	*this << REQUEST(0x0C);
	return *this;
}

MessageBuilder &MessageBuilder::readDevId()
{
	*this << REQUEST(0x11);
	return *this;
}

MessageBuilder &MessageBuilder::readObject(quint8 startId)
{
	*this << REQUEST(0x2B) << quint8(0x0E) << quint8(1) << startId;
	return *this;
}

MessageBuilder &MessageBuilder::encapsulate()
{
	*this << REQUEST(0x2B) << quint8(0x0D);
	return *this;
}

MessageBuilder &MessageBuilder::route(quint8 addr)
{
	*this << REQUEST(0x2B) << quint8(0x0C) << addr;
	return *this;
}

MessageBuilder &MessageBuilder::loadBootloaderInfo()
{
	*this << REQUEST(0x41);
	return *this;
}

MessageBuilder &MessageBuilder::eraseFlashPage(quint32 addr)
{
	const quint8 flashKey0 = 0xA5;
	const quint8 flashKey1 = 0xF1;
	*this << REQUEST(0x42) << addr << flashKey0 << flashKey1;
	return *this;
}

MessageBuilder &MessageBuilder::readPageCRC()
{
	*this << REQUEST(0x43);
	return *this;
}

MessageBuilder &MessageBuilder::writeFlashPage(quint32 addr)
{
	const quint8 flashKey0 = 0xA5;
	const quint8 flashKey1 = 0xF1;
	*this << REQUEST(0x44) << addr << flashKey0 << flashKey1;
	return *this;
}
