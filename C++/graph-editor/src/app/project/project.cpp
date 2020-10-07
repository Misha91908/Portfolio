#include "project.h"
#include "../widgets/graphs.h"
#include "../widgets/lineparameters.h"

Project::Project(editorWindow& parent) : _parent(parent)
{
    this->_plot = new Graphs(this->_parent);
    this->set_title("untitled");
}

void Project::read(const QJsonObject &jsonObj)
{
    if(this->find_key(jsonObj.keys(), QString("name")) && this->find_key(jsonObj.keys(), QString("data_x")) &&
       this->find_key(jsonObj.keys(), QString("data_y")) && this->find_key(jsonObj.keys(), QString("graph")))
    {
        this->set_title(jsonObj["name"].toString());
        this->_data_x = jsonarray_to_double_list(jsonObj["data_x"].toArray());
        this->_data_y = jsonarray_to_double_list(jsonObj["data_y"].toArray());

        this->plot()->set_x_axis_name(jsonObj["graph"].toObject()["x_axis"].toObject()["name"].toString());
        this->plot()->set_x_axis_dimension(jsonObj["graph"].toObject()["x_axis"].toObject()["dimension"].toString());
        this->plot()->set_x_dimension_active(jsonObj["graph"].toObject()["x_axis"].toObject()["dimension_active"].toBool());

        this->plot()->set_y_axis_name(jsonObj["graph"].toObject()["y_axis"].toObject()["name"].toString());
        this->plot()->set_y_axis_dimension(jsonObj["graph"].toObject()["y_axis"].toObject()["dimension"].toString());
        this->plot()->set_y_dimension_active(jsonObj["graph"].toObject()["y_axis"].toObject()["dimension_active"].toBool());

        this->plot()->lines_array_pointer()->clear();
        QJsonArray lines = jsonObj["graph"].toObject()["lines"].toArray();
        while (lines.count())
        {
            QJsonObject i = lines.takeAt(0).toObject();
            LineParameters *l = new LineParameters();
            this->plot()->update_connections();
            l->set_legend(i["legend"].toString());
            l->set_active_line(i["is_active"].toBool());
            l->set_line_type(i["line_type"].toString());
            l->set_line_color(i["line_color"].toString());
            l->set_dots(i["dots"].toBool());
            l->set_line_width(i["line_width"].toInt());
            this->plot()->lines_array_pointer()->append(l);
        }
    }
}

void Project::write(QJsonObject &jsonObj) const
{
    jsonObj["name"] = this->title();


    QJsonArray x = double_list_to_jsonarray(this->data_x());
    jsonObj["data_x"] = x;
    QJsonArray y = double_list_to_jsonarray(this->data_y());
    jsonObj["data_y"] = y;


    QJsonObject x_axis_properties;
    x_axis_properties.insert("name", this->plot()->x_axis_name());
    x_axis_properties.insert("dimension", this->plot()->x_axis_dimension());
    x_axis_properties.insert("dimension_active", this->plot()->x_dimension_active());

    QJsonObject y_axis_properties;
    y_axis_properties.insert("name", this->plot()->y_axis_name());
    y_axis_properties.insert("dimension", this->plot()->y_axis_dimension());
    y_axis_properties.insert("dimension_active", this->plot()->y_dimension_active());

    QJsonArray lines;
    QList<LineParameters*> l = this->plot()->Lines();
    for (int i = 0; i < l.length(); ++i)
    {
        QJsonObject line;
        line.insert("legend", l[i]->legend());
        line.insert("is_active", l[i]->is_active());
        line.insert("line_type", l[i]->line_type());
        line.insert("line_color", l[i]->line_color());
        line.insert("dots", l[i]->dots());
        line.insert("line_width", l[i]->line_width());
        lines.append(line);
    }

    QJsonObject graph;
    graph.insert("x_axis", x_axis_properties);
    graph.insert("y_axis", y_axis_properties);
    graph.insert("lines", lines);


    jsonObj.insert("graph", graph);
}

void Project::clear_data()
{
    _data_x.clear();
    _data_y.clear();
    _plot->lines_array_pointer()->clear();
}

Graphs* Project::plot() const
{
    return _plot;
}

QList<QList<double>> Project::data_x() const
{
    return _data_x;
}

QList<QList<double>> Project::data_y() const
{
    return _data_y;
}

QString Project::title() const
{
    return _title;
}

void Project::set_title(const QString &title)
{
    _title = title;
}

QList<QList<double>>* Project::datax_array_pointer()
{
    return &_data_x;
}

QList<QList<double>>* Project::datay_array_pointer()
{
    return &_data_y;
}

bool Project::find_key(QStringList keys_list, QString key)
{
    foreach(QString i, keys_list)
        {
            if(i == key)
                return true;
        }
    return false;
}

QJsonArray Project::double_list_to_jsonarray(QList<QList<double>> data) const
{
    QJsonArray jsonArray;
    QJsonArray arr;

    foreach(QList<double> i, data)
    {
        foreach(double j, i)
        {
            arr.append(j);
        }
        jsonArray.append(arr);
        while(arr.count()) {
            arr.pop_back();
        }
    }

    return jsonArray;
}

QList<QList<double>> Project::jsonarray_to_double_list(QJsonArray jsonArray) const
{
    QList<QList<double>> data;

    int i = 0;
    foreach (const QJsonValue & subarr, jsonArray)
    {
        QJsonArray arr = subarr.toArray();
        QList<double> row;
        foreach (const QJsonValue & value, arr)
            {
                row.append(value.toDouble());
            }
        data.append(row);
        i++;
    }
    return data;
}

Project::~Project(){}
