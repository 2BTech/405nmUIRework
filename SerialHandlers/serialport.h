#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtGlobal>

#ifndef Q_OS_WIN

#include <QString>
// Signals and slots
#include <QObject>
// Read flags
#include <QIODevice>
// For reading and writing to the serial port
#include <fstream>
// Read and write threads, async
#include <QThread>
// Locking common objects
#include <QMutex>
// Logging info
#include <QDebug>
// UNIX standard function definitions, close, read, write
#include <unistd.h>
// File control definitions
#include <fcntl.h>
// Used for TCGETS2, which is required for custom baud rates
#include <sys/ioctl.h>
// manipulating serial port settings
#include <asm/termbits.h>

#include "serialwritethread.h"
#include "serialreadthread.h"

enum class BaudRate {
    B_0,
    B_50,
    B_75,
    B_110,
    B_134,
    B_150,
    B_200,
    B_300,
    B_600,
    B_1200,
    B_1800,
    B_2400,
    B_4800,
    B_9600,
    B_19200,
    B_38400,
    B_57600,
    B_115200,
    B_230400,
    B_460800,
};

enum class DataBits {
    FIVE,
    SIX,
    SEVEN,
    EIGHT
};

enum class Parity {
    NONE,
    EVEN,
    ODD
};

enum class StopBits {
    ONE,
    TWO
};

#define USE_THREAD_OBJECTS

// https://github.com/gbmhunter/CppLinuxSerial/blob/master/include/CppLinuxSerial/SerialPort.hpp
#define DEBUG_SERIAL_PORT
class SerialPort : public QObject
{
    Q_OBJECT
public:
    SerialPort();
    SerialPort(const QString &portName, BaudRate baudrate);
    SerialPort(const QString &portName, BaudRate baudrate, DataBits numDataBits, Parity parity, StopBits numStopBits);
    ~SerialPort();

    void setPortName(const QString &portName);
    QString getPortName();

    void setBaudRate(BaudRate baudrate);
    BaudRate getBaudRate();

    void setDataBits(DataBits dataBits);
    DataBits getDataBits();

    void setParity(Parity parity);
    Parity getParity();

    void setStopBits(StopBits stopBits);
    StopBits getStopBits();

    void setReadTimeout(int timeoutms);
    int getReadTimeout();

    void setReadBufferSize(int readBufferSize);
    int getReadBufferSize();

    bool openPort(QIODevice::OpenMode openMode);
    void closePort();
    bool isOpen();

    void writeData(const QString &data);
    void writeData(const QByteArray &data);
    void writeData(const char* data, int len);

    qint64 readData(QString &data);
    qint64 readData(QByteArray &data);
    qint64 readData(char* buffer, qint64 maxLen);

    int available();

signals:
    void ReadyRead();

private slots:
    void OnReadyRead();
    void OnWriteThreadFinish()
    {
#ifndef USE_THREAD_OBJECTS
        writeThread->deleteLater();
        writeThread = Q_NULLPTR;
#endif

        //qDebug() << "Write thread finshed. Buffer size: " << writeBuffer.length();
        if (writeBuffer.length() > 0)
        {
#ifndef USE_THREAD_OBJECTS
            StartWriteThread();
#else
            writeThread->start();
#endif
        }
        else
        {
            writeThreadActive = false;
        }
    }

private:
    // Name of port to connect to
    QString portname = "unset";
    // baud rate for serial connection
    BaudRate baudrate = BaudRate::B_19200;
    // Data bits
    DataBits dataBits = DataBits::EIGHT;
    // Parity
    Parity parity = Parity::NONE;
    // Stop bits
    StopBits stopBits = StopBits::ONE;
    // File descriptor
    int fd = -1;
    // read timeout in ms
    int readTimeoutms = -1;
    // read buffer
    QByteArray readBuffer;
    // read buffer length
    int maxReadBufferSize = 4096;
    // Mutex for locking the read buffer
    QMutex readBufferMutex;
    // is the serial port open
    bool is_open = false;
    // Write buffer
    QByteArray writeBuffer;
    // Write mutex
    QMutex writeMutex;
    // Is the write thread active
    bool writeThreadActive = false;

    void ConfigureTermios();

    void SetTermios2(termios2 tty);
    termios2 getTermios2();


#ifndef USE_THREAD_OBJECTS
    QThread* writeThread = Q_NULLPTR;
    QThread* readThread = Q_NULLPTR;

    void readThreadFunct();
    void writeThreadFunct();

    void StartWriteThread()
    {
        writeThread = QThread::create(std::bind(&SerialPort::writeThreadFunct, this));
        connect(writeThread, &QThread::finished, this, &SerialPort::OnWriteThreadFinish);
        writeThread->start();
    }
#else
    SerialWriteThread* writeThread = Q_NULLPTR;
    SerialReadThread* readThread = Q_NULLPTR;
#endif
};

#endif // ifndef Q_OS_WIN

#endif // SERIALPORT_H
