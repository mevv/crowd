#include "json_manager.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

QString JsonManager::m_confPath("config.json");

JsonManager::JsonManager()
{

}

QJsonObject JsonManager::parseJsonFile(QString filename)
{
    QFile file;
    QString val;

    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    val = file.readAll();

    file.close();

    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());

    return document.object();
}

bool JsonManager::serializeJson(const QJsonObject& json, QString filename)
{
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&file);

    QJsonDocument doc(json);
    QByteArray bytes = doc.toJson();

    stream << bytes;

    file.close();

    return true;
}
