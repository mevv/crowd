#include "json_manager.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

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
    auto fileJson = JsonManager::parseJsonFile(filename);

    fileJson["agent"] = json;

    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&file);

    QJsonDocument doc(fileJson);
    QByteArray bytes = doc.toJson();

    stream << bytes;

    file.close();

    return true;
}
