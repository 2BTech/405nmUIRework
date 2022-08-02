#include "modbustcpbase.h"

ModbusTCPBase::ModbusTCPBase()
{

}

QString ModbusTCPBase::PeerIP()
{
    return peerIP;
}

unsigned short ModbusTCPBase::Port()
{
    return port;
}

QString ModbusTCPBase::TargetIP()
{
    return targetIP;
}
