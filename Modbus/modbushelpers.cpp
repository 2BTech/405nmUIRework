#include "modbushelpers.h"

Helpers::Helpers()
{

}

QString Helpers::HexToDec(QByteArray arr)
{
//    if(arr.length() > 1)
//    {
//        QString t = QString::number(arr.at(0)).append(',');
//        t.append(HexToDec(arr.remove(0, 1)));
//        return t;
//    }
//    else
//    {
//        QString temp = QString::number(arr.at(0));
//        return temp;
//    }

    QString str = "";
    for(char c : arr)
    {
        str.append(QString::number(c).append(','));
    }
    if(str.count() > 0)
    {
        str.remove(str.count() - 1, 1);
    }
    return str;
}

QString Helpers::ParsePeerIP(QString peer)
{
    if(peer.contains(':'))
    {
        QStringList split = peer.split(':');

        if(split.length() == 4)
        {
            return split[2];
        }
        else
        {
            return split[1];
        }
    }
    else
    {
        return peer;
    }
}

QString Helpers::FunctCodeToName(int code)
{
#define READ_COILS              0x01
#define READ_INPUTS             0x02
#define READ_HOLDING_REGISTERS  0x03
#define READ_INPUT_REGISTERS    0x04
#define SET_SINGLE_COIL         0x05
#define SET_SINGLE_REGISTER     0x06
#define SET_MULTIPLE_COILS      0x0F
#define SET_MULTIPLE_REGISTERS  0x10

    switch (code)
    {
    case 0x01:
        return "Read Coils";

    case 0x02:
        return "Read Inputs";

    case 0x03:
        return "Read Holding Registers";

    case 0x04:
        return "Read Input Registers";

    case 0x05:
        return "Set Single Coil";

    case 0x06:
        return "Set Single Register";

    case 0x0F:
        return "Set Multiple Coils";

    case 0x10:
        return "Set Multiple Registers";

    default:
        return "Bad Function Code";
    }
}
