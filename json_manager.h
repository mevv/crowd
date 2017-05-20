#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QDir>


class JsonManager
{
public:
    JsonManager();

    static QJsonObject parseJsonFile(QString filename);

    static bool serializeJson(const QJsonObject& json, QString filename);


    static QString getConfPath() { return QDir::current().absolutePath() + "/config.json"; }

    static QString getStatPath() { return QDir::current().absolutePath() + "/stat/"; }
};

#endif // JSONMANAGER_H
