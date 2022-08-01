#include "utilities.h"

Utilities::Utilities()
{

}

QString Utilities::GetIPAddress()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    QString current;
    QStringList split;
    for (int i = 0; i < addresses.length(); i++)
    {
        if (addresses.at(i).protocol() == QAbstractSocket::IPv4Protocol && addresses.at(i) != localhost)
        {
            current = addresses.at(i).toString();
            split = current.split('.');
            if(split.count() == 4 && split[3] != "1")
            {
                return current;
            }
        }
    }
    return "Not connected";
}
