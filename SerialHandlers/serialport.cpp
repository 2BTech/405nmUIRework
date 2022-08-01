#include "serialport.h"

#ifndef Q_OS_WIN

SerialPort::SerialPort()
{
    readBuffer = new char[200];

#ifndef USE_THREAD_OBJECTS
    readThread = QThread::create(std::bind(&SerialPort::readThreadFunct, this));
#else
    writeThread = new SerialWriteThread(&fd, &writeMutex, &writeBuffer, &is_open);
    readThread = new SerialReadThread(&fd, &readBuffer, &readBufferMutex, &maxReadBufferSize, &is_open);
    connect(readThread, &SerialReadThread::ReadyRead, this, &SerialPort::ReadyRead);
    connect(writeThread, &SerialWriteThread::FinishedWriting, this, &SerialPort::OnWriteThreadFinish);
#endif

    //connect(this, &SerialPort::ReadyRead, this, &SerialPort::OnReadyRead);
}

SerialPort::SerialPort(const QString &portName, BaudRate baudrate) : SerialPort()
{
    this->portname = portName;
    this->baudrate = baudrate;
}

SerialPort::SerialPort(const QString &portName, BaudRate baudrate, DataBits dataBits, Parity parity, StopBits stopBits) : SerialPort(portName, baudrate)
{
    this->dataBits = dataBits;
    this->parity = parity;
    this->stopBits = stopBits;
}

SerialPort::~SerialPort()
{
    if (is_open)
    {
        closePort();
    }
    writeThread->deleteLater();
    readThread->deleteLater();

    readThread = Q_NULLPTR;
    writeThread = Q_NULLPTR;
}

void SerialPort::setPortName(const QString &portName)
{
    if (is_open)
    {
        qWarning("ERROR: Trying to set portname while connection is active");
    }
    else
    {
        this->portname = portName;
    }
}

QString SerialPort::getPortName()
{
    return portname;
}

void SerialPort::setBaudRate(BaudRate baudrate)
{
    if (is_open)
    {
        qWarning("ERROR: Trying to set baudrate while connection is active");
    }
    else
    {
        this->baudrate = baudrate;
    }
}

BaudRate SerialPort::getBaudRate()
{
    return baudrate;
}

void SerialPort::setDataBits(DataBits dataBits)
{
    if (is_open)
    {
        qWarning("ERROR: Trying to set databits while connection is active");
    }
    else
    {
        this->dataBits = dataBits;
    }
}

DataBits SerialPort::getDataBits()
{
    return dataBits;
}

void SerialPort::setParity(Parity parity)
{
    if (is_open)
    {
        qWarning("ERROR: Trying to set parity while connection is active");
    }
    else
    {
        this->parity = parity;
    }
}

Parity SerialPort::getParity()
{
    return parity;
}

void SerialPort::setStopBits(StopBits stopBits)
{
    if (is_open)
    {
        qWarning("ERROR: Trying to set stop bits while connection is active");
    }
    else
    {
        this->stopBits = stopBits;
    }
}

StopBits SerialPort::getStopBits()
{
    return stopBits;
}

void SerialPort::setReadTimeout(int timeoutms)
{
    readTimeoutms = timeoutms;
}

int SerialPort::getReadTimeout()
{
    return readTimeoutms;
}

void SerialPort::setReadBufferSize(int size)
{
//    char* nBuffer = new char[size];

//    memcpy(nBuffer, readBuffer, readBufferLength < size ? readBufferLength : size);

//    delete [] readBuffer;
//    readBuffer = nBuffer;

    readBufferMutex.lock();

    if (readBuffer.length() > size)
    {
        readBuffer.resize(size);
    }

    maxReadBufferSize = size;
    readBufferMutex.unlock();
}

bool SerialPort::openPort(QIODevice::OpenMode openMode)
{

#ifdef DEBUG_SERIAL_PORT
    qDebug() << (QString("Attempting to connect serial port to " + portname).toLatin1());
#endif

    if (is_open)
    {
        qWarning("ERROR: Serialport is already open");
        return true;
    }

    if (portname.isEmpty())
    {
        qWarning("ERROR: Tried to open serial port with empty name");
        return false;
    }

    switch(openMode)
    {
    case QIODevice::ReadOnly:
        fd = open(const_cast<char*>(portname.toStdString().c_str()), O_RDONLY);
        break;

    case QIODevice::WriteOnly:
        fd = open(const_cast<char*>(portname.toStdString().c_str()), O_WRONLY);
        break;

    case QIODevice::ReadWrite:
        fd = open(const_cast<char*>(portname.toStdString().c_str()), O_RDWR);
        break;

    default:
        qWarning("ERROR: Unsupported open mode in serial port");
        break;
    }
//    fd = open(const_cast<char*>(portname.toStdString().c_str()), O_RDWR);

    if (fd == -1)
    {
        qWarning("ERROR: Failed to open serial port");
        return false;
    }
    else
    {
        ConfigureTermios();
        is_open = true;
        readThread->start();
        return true;
    }
}

void SerialPort::closePort()
{
    if(fd != -1)
    {
        auto retVal = close(fd);
        if(retVal != 0)
        {
            qWarning(QString("ERROR: Failed to close serial port at " + portname).toLatin1());
        }

        fd = -1;
    }
    is_open = false;
}

bool SerialPort::isOpen()
{
    return is_open && fd >= 0;
}

void SerialPort::writeData(const QString &data)
{
    writeData(data.toStdString().c_str(), data.length());
}

void SerialPort::writeData(const QByteArray &data)
{
    writeData(data.data(), data.length());
}

void SerialPort::writeData(const char* data, int len)
{
    if (is_open || fd < 0)
    {
//        int writeResult = write(fd, data, len);

//        if (writeResult < 0)
//        {
//            qWarning("ERROR: Failed to write message");
//        }

        writeMutex.lock();
        //qDebug() << "Appending data to write";
        writeBuffer.append(data, len);
        //qDebug() << "Checking if thread should start";
        if (!writeThreadActive)
        {
            writeThreadActive = true;
            //qDebug() << "Starting thread";
#ifndef USE_THREAD_OBJECTS
            StartWriteThread();
#else
            writeThread->start();
#endif
        }
        //qDebug() << "Releasing write mutex";
        writeMutex.unlock();
    }
    else
    {
        qWarning("ERROR: Trying to write message to closed serial port");
    }
}

qint64 SerialPort::readData(QString &data)
{
    if (is_open && fd >= 0)
    {
        readBufferMutex.lock();
        //qDebug() << "Read buffer: " << readBuffer;
        data.append(readBuffer);
        readBuffer.clear();
        readBufferMutex.unlock();
        return data.length();
    }
    else
    {
        qWarning("ERROR: Failed to read from serial port because it is not open");
        return -1;
    }
}

qint64 SerialPort::readData(QByteArray &data)
{
    data.clear();

    if (is_open && fd >= 0)
    {
        readBufferMutex.lock();
        data.append(readBuffer);
        data.clear();
        readBufferMutex.unlock();
        return data.length();
    }
    else
    {
        qWarning("ERROR: Failed to read from serial port because it is not open");
        return -1;
    }
}

qint64 SerialPort::readData(char* buffer, qint64 maxLen)
{
    readBufferMutex.lock();
    if (maxLen > readBuffer.length())
    {
        maxLen = readBuffer.length();
    }

    memcpy(buffer, readBuffer.data(), maxLen);
    readBuffer.remove(0, maxLen);

    readBufferMutex.unlock();
    return maxLen;
}

int SerialPort::available()
{
    return readBuffer.length();
}

void SerialPort::ConfigureTermios()
{
    termios2 tty = getTermios2();

    tty.c_cflag &= ~CSIZE;
    switch (dataBits)
    {
    case DataBits::FIVE:
        tty.c_cflag |= CS5;
        break;

    case DataBits::SIX:
        tty.c_cflag |= CS6;
        break;

    case DataBits::SEVEN:
        tty.c_cflag |= CS7;
        break;

    case DataBits::EIGHT:
        tty.c_cflag |= CS8;
        break;

    default:
        qWarning("ERROR: Invalid data bits");
        break;
    }

    switch(parity)
    {
    case Parity::NONE:
        tty.c_cflag &= ~PARENB;
        break;

    case Parity::EVEN:
        tty.c_cflag |= PARENB;
        tty.c_cflag &= ~PARODD; // Clearing PARODD makes the parity even
        break;

    case Parity::ODD:
        tty.c_cflag |= PARENB;
        tty.c_cflag |= PARODD;
        break;

    default:
        qWarning("ERROR: Invalid parity");
        break;
    }

    switch (stopBits)
    {
    case StopBits::ONE:
        tty.c_cflag &= ~CSTOPB;
        break;

    case StopBits::TWO:
        tty.c_cflag |= CSTOPB;
        break;

    default:
        qWarning("ERROR: Invalid stop bits");
        break;
    }

    tty.c_cflag &= ~CBAUD;
    tty.c_cflag |= CBAUDEX;
    switch(baudrate)
    {
        case BaudRate::B_0:
            tty.c_ispeed = 0;
            tty.c_ospeed = 0;
            break;
        case BaudRate::B_50:
            tty.c_ispeed = 50;
            tty.c_ospeed = 50;
            break;
        case BaudRate::B_75:
            tty.c_ispeed = 75;
            tty.c_ospeed = 75;
            break;
        case BaudRate::B_110:
            tty.c_ispeed = 110;
            tty.c_ospeed = 110;
            break;
        case BaudRate::B_134:
            tty.c_ispeed = 134;
            tty.c_ospeed = 134;
            break;
        case BaudRate::B_150:
            tty.c_ispeed = 150;
            tty.c_ospeed = 150;
            break;
        case BaudRate::B_200:
            tty.c_ispeed = 200;
            tty.c_ospeed = 200;
            break;
        case BaudRate::B_300:
            tty.c_ispeed = 300;
            tty.c_ospeed = 300;
            break;
        case BaudRate::B_600:
            tty.c_ispeed = 600;
            tty.c_ospeed = 600;
            break;
        case BaudRate::B_1200:
            tty.c_ispeed = 1200;
            tty.c_ospeed = 1200;
            break;
        case BaudRate::B_1800:
            tty.c_ispeed = 1800;
            tty.c_ospeed = 1800;
            break;
        case BaudRate::B_2400:
            tty.c_ispeed = 2400;
            tty.c_ospeed = 2400;
            break;
        case BaudRate::B_4800:
            tty.c_ispeed = 4800;
            tty.c_ospeed = 4800;
            break;
        case BaudRate::B_9600:
            tty.c_ispeed = 9600;
            tty.c_ospeed = 9600;
            break;
        case BaudRate::B_19200:
            tty.c_ispeed = 19200;
            tty.c_ospeed = 19200;
            break;
        case BaudRate::B_38400:
            tty.c_ispeed = 38400;
            tty.c_ospeed = 38400;
            break;
        case BaudRate::B_57600:
            tty.c_ispeed = 57600;
            tty.c_ospeed = 57600;
            break;
        case BaudRate::B_115200:
            tty.c_ispeed = 115200;
            tty.c_ospeed = 115200;
            break;
        case BaudRate::B_230400:
            tty.c_ispeed = 230400;
            tty.c_ospeed = 230400;
            break;
        case BaudRate::B_460800:
            tty.c_ispeed = 460800;
            tty.c_ospeed = 460800;
            break;
        default:
        qWarning("ERROR: Invalid baud rate");
        break;
    }

    tty.c_oflag = 0;
    tty.c_oflag &= ~OPOST;

    if(readTimeoutms == -1)
    {
        // Always wait for at least one byte, this could
        // block indefinitely
        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 1;
    }
    else if(readTimeoutms == 0)
    {
        // Setting both to 0 will give a non-blocking read
        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 0;
    }
    else if(readTimeoutms > 0)
    {
        tty.c_cc[VTIME] = (cc_t)(readTimeoutms/100);    // 0.5 seconds read timeout
        tty.c_cc[VMIN] = 0;
    }

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_lflag &= ~ICANON;

    tty.c_lflag &= ~(ECHO);
    tty.c_lflag &= ~ECHOE;								// Turn off echo erase (echo erase only relevant if canonical input is active)
    tty.c_lflag &= ~ECHONL;								//
    tty.c_lflag &= ~ISIG;

    SetTermios2(tty);
}

void SerialPort::SetTermios2(termios2 tty)
{
    ioctl(fd, TCSETS2, &tty);
}

termios2 SerialPort::getTermios2()
{
    struct termios2 term2;

    ioctl(fd, TCGETS2, &term2);

    return term2;
}

#ifndef USE_THREAD_OBJECTS
void SerialPort::readThreadFunct()
{
    char readBuffer[256];
    memset(readBuffer, 0, 256);

    while (is_open)
    {
        int count = read(fd, &readBuffer[0], 255);

        if (count > 0)
        {
            readBuffer[count] = 0;
            qDebug() << ("Read in data");
            readBufferMutex.lock();
            this->readBuffer.append(readBuffer);

            //qDebug() << "Local read buffer: " << readBuffer;
            //qDebug() << "Global read buffer: " << this->readBuffer;

            // ToDo, replace with ring/circular buffer
            if (this->readBuffer.length() > maxReadBufferSize)
            {
                qDebug() << ("Shrunk read buffer");
                this->readBuffer.remove(0, this->readBuffer.length() - maxReadBufferSize);
            }

            readBufferMutex.unlock();
            emit ReadyRead();
        }
    }
}

void SerialPort::writeThreadFunct()
{
    //qDebug() << "Started write thread";
    while(is_open && !writeBuffer.isEmpty())
    {
        writeMutex.lock();
        QByteArray toWrite = writeBuffer.left(200);
        writeBuffer.remove(0, toWrite.length());
        int writeResult = write(fd, toWrite.data(), toWrite.length());
        writeMutex.unlock();

        if (writeResult < 0)
        {
            qDebug() << ("ERROR: Failed to write bytes to serial port");
        }
//        else
//        {
//            qDebug() << "Write result: " << writeResult;
//        }
    }
    //qDebug() << "Finished write thread";
}
#endif

void SerialPort::OnReadyRead()
{
    QString data;

    readData(data);
    qDebug() << "Read in: " << data;
}


#endif // ifndef Q_OS_WIN
