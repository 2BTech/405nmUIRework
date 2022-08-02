#ifndef MODBUSHELPERS_H
#define MODBUSHELPERS_H

// Receive and handle messages
#include <QString>
// Splitting strings
#include <QStringList>

// This class defines a collection of useful functions for the modbus classes.
class Helpers
{
public:
    Helpers();

    // Converts a byte array into a hex string.
    static QString HexToDec(QByteArray arr);

    // This function handles parsing a peerIP string. It returns the ip address from the strimg.
    static QString ParsePeerIP(QString peer);

    // Converts a function code into its string representation.
    static QString FunctCodeToName(int code);
};

#endif // MODBUSHELPERS_H
