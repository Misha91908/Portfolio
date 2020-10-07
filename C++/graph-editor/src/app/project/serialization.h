#ifndef GRAPHIC_EDITOR_SERIALIZATION_H
#define GRAPHIC_EDITOR_SERIALIZATION_H

#include <QtWidgets>

class JsonSerializable
{
public:
    virtual ~JsonSerializable() {}

    virtual void read(const QJsonObject &jsonObj) = 0;
    virtual void write(QJsonObject &jsonObj) const = 0;
};

class ProjectJsonSerializer
{

public:

    static QByteArray serialize(JsonSerializable* obj);

    static void parse(const QString &json, JsonSerializable* jsonSerializable);

private:

    ProjectJsonSerializer();

};

#endif //GRAPHIC_EDITOR_SERIALIZATION_H
