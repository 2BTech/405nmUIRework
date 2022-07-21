#include "settingshandler.h"

SettingsHandler* SettingsHandler::instance = Q_NULLPTR;
SettingsHandler* SettingsHandler::GetInstance()
{
    if(instance == Q_NULLPTR)
    {
        instance = new SettingsHandler();
    }
    return instance;
}

SettingsHandler::SettingsHandler()
{

}
