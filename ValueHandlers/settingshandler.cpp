#include "settingshandler.h"

SettingsHandler* SettingsHandler::instance = nullptr;
SettingsHandler* SettingsHandler::GetInstance()
{
    if(instance == nullptr)
    {
        instance = new SettingsHandler();
    }
    return instance;
}

SettingsHandler::SettingsHandler()
{

}
