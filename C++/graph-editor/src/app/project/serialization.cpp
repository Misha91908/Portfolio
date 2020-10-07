#include <QtWidgets>
#include "serialization.h"

QByteArray ProjectJsonSerializer::serialize(JsonSerializable* obj)
{
    QJsonObject jsonObj;
    obj->write(jsonObj);
    QJsonDocument doc(jsonObj);
    return doc.toJson(QJsonDocument::Indented);
}

void ProjectJsonSerializer::parse(const QString &json, JsonSerializable* jsonSerializable)
{
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());

    // The document wrap a jsonObject
    QJsonObject jsonObj = document.object();

    jsonSerializable->read(jsonObj);

}