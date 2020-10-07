#ifndef PROJECT_H
#define PROJECT_H

#include <QtCore>
#include "../editorwindow.h"
#include"../widgets/graphs.h"
#include "serialization.h"
#include "../widgets/lineparameters.h"

class editorWindow;

class Project : public QObject, public JsonSerializable
{
    Q_PROPERTY(QString title READ title WRITE set_title)
    Q_PROPERTY(Graphs* plot READ plot)
    Q_PROPERTY(QList<QList<double>> data_x READ data_x)
    Q_PROPERTY(QList<QList<double>> data_y READ data_y)

public:
    Project(editorWindow& parent);
    ~Project();

    Graphs* plot() const;

    QList<QList<double>> data_x() const;
    QList<QList<double>> data_y() const;

    QString title() const;
    void set_title(const QString& title);

    QList<QList<double>>* datax_array_pointer();
    QList<QList<double>>* datay_array_pointer();
    void clear_data();

    void read(const QJsonObject &jsonObj);
    void write(QJsonObject &jsonObj) const;

private:
    QJsonArray double_list_to_jsonarray(QList<QList<double>> data) const;
    QList<QList<double>> jsonarray_to_double_list(QJsonArray jsonArray) const;
    bool find_key(QStringList keys_list, QString key);

    editorWindow& _parent;

    Graphs *_plot;
    QString _title;
    QList<QList<double>> _data_x;
    QList<QList<double>> _data_y;
};

#endif // PROJECT_H
