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


    static QString getConfPath() { return m_confPath; }
    static QString getStatPath() { return QDir::current().absolutePath() + "/stat/"; }

    static void setConfPath(const QString& path) { m_confPath = path; }

private:
    static QString m_confPath;
};

#endif // JSONMANAGER_H
