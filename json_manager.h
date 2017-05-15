#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <QString>
#include <QJsonObject>

#define PATH_TO_CONF "/home/valera/source/crowd/config.json"

class JsonManager
{
public:
    JsonManager();

    static QJsonObject parseJsonFile(QString filename);

    static bool serializeJson(const QJsonObject& json, QString filename);
};

#endif // JSONMANAGER_H
