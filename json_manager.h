#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <QString>
#include <QJsonObject>

#define PATH_TO_CONF "/home/peter/programming/diploma/config.json"
#define PATH_TO_STAT "/home/peter/programming/stat/"

class JsonManager
{
public:
    JsonManager();

    static QJsonObject parseJsonFile(QString filename);

    static bool serializeJson(const QJsonObject& json, QString filename);
};

#endif // JSONMANAGER_H
